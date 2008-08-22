/* calibration.cpp
 *
 *			Soju Matsumoto -- 20070420
 */

#include <HD/hd.h>
#include <stdio.h>
#include <assert.h>

#ifndef WIN32
#include <unistd.h>
#define TRUE 1
#define FALSE 0
#endif

#include "calibration.h"

static HDCallbackCode HDCALLBACK UpdateCalibrationCallback(void *pUserData);
static HDCallbackCode HDCALLBACK CalibrationStatusCallback(void *pUserData);
static HDCallbackCode HDCALLBACK DevicePositionCallback(void *pUserData);

static HDenum GetCalibrationStatus();
static HDboolean CheckCalibration(HDenum calibrationStyle);
static void PrintDevicePosition();

/*******************************************************************************
 main function
*******************************************************************************/
int DoCalibration(void)
{
	HHD hHD;
	HDErrorInfo error;
	int supportedCalibrationStyles;
	int calibrationStyle;

	hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		fprintf(stderr, "Failed to initialize haptic device");
		return -1;
	}

	printf("Found device '%s'.\n", hdGetString(HD_DEVICE_MODEL_TYPE));

	/* Choose a calibration style.  Some devices may support multiple types of 
		calibration.  In that case, prefer auto calibration over inkwell 
		calibration, and prefer inkwell calibration over reset encoders. */

	hdGetIntegerv(HD_CALIBRATION_STYLE, &supportedCalibrationStyles);
	if(supportedCalibrationStyles & HD_CALIBRATION_ENCODER_RESET)
		calibrationStyle = HD_CALIBRATION_ENCODER_RESET;

	if(supportedCalibrationStyles & HD_CALIBRATION_INKWELL)
		calibrationStyle = HD_CALIBRATION_INKWELL;

	if(supportedCalibrationStyles & HD_CALIBRATION_AUTO)
		calibrationStyle = HD_CALIBRATION_AUTO;

	/* Some haptic devices only support manual encoder calibration via a
		hardware reset. This requires that the endpoint be placed at a known
		physical location when the reset is commanded. For the PHANTOM haptic
		devices, this means positioning the device so that all links are
		orthogonal. Also, this reset is typically performed before the servoloop
		is running, and only technically needs to be performed once after each
		time the device is plugged in. */

	if(calibrationStyle == HD_CALIBRATION_ENCODER_RESET)
	{
		hdUpdateCalibration(calibrationStyle);
		if (hdCheckCalibration() == HD_CALIBRATION_OK)
			printf("Calibration complete.\n\n");

		if(HD_DEVICE_ERROR(error = hdGetError()))
		{
			fprintf(stderr, "Reset encoders reset failed.");
			return -1;
		}
	}

	hdStartScheduler();
	if(HD_DEVICE_ERROR(error = hdGetError()))
	{
		fprintf(stderr, "Failed to start the scheduler");
		return -1;           
	}

	/* Some haptic devices are calibrated when the gimbal is placed into
		the device inkwell and updateCalibration is called.  This form of
		calibration is always performed after the servoloop has started 
		running. */
	if(calibrationStyle  == HD_CALIBRATION_INKWELL)
	{
		if(GetCalibrationStatus() == HD_CALIBRATION_NEEDS_MANUAL_INPUT)
		{
			printf("Please place the device into the inkwell ");
			printf("for calibration.\n\n");
		}
	}


	/* Regular calibration should be checked periodically while the
		servoloop is running. In some cases, like the PHANTOM Desktop,
		calibration can be continually refined as the device is moved
		throughout its workspace.  For other devices that require inkwell
		reset, such as the PHANToM Omni, calibration is successfully
		performed whenever the device is put into the inkwell. */
	if(CheckCalibration(calibrationStyle))
		PrintDevicePosition();
#ifdef WIN32
	Sleep(100);
#else
	usleep(100);
#endif

	hdStopScheduler();
	hdDisableDevice(hHD);

	return 0;
	}

/******************************************************************************
 * Scheduler Callbacks
 *****************************************************************************/
HDCallbackCode HDCALLBACK UpdateCalibrationCallback(void *pUserData)
{
	HDenum *calibrationStyle = (unsigned int *)pUserData;
	if(hdCheckCalibration() == HD_CALIBRATION_NEEDS_UPDATE)
		hdUpdateCalibration(*calibrationStyle);

	return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK CalibrationStatusCallback(void *pUserData)
{
	HDenum *pStatus = (HDenum *)pUserData;

	hdBeginFrame(hdGetCurrentDevice());
	*pStatus = hdCheckCalibration();
	hdEndFrame(hdGetCurrentDevice());

	return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK DevicePositionCallback(void *pUserData)
{
	HDdouble *pPosition = (HDdouble *)pUserData;
	hdGetDoublev(HD_CURRENT_POSITION, pPosition);

	return HD_CALLBACK_DONE;
}

HDenum GetCalibrationStatus()
{
	HDenum status;
	hdScheduleSynchronous(CalibrationStatusCallback, &status,
							HD_DEFAULT_SCHEDULER_PRIORITY);
	return status;
}
/*
  キャリブレーションをチェック
  */
HDboolean CheckCalibration(HDenum calibrationStyle)
{
	HDenum status = GetCalibrationStatus();

	if(status == HD_CALIBRATION_OK)
		return TRUE;

	else if(status == HD_CALIBRATION_NEEDS_MANUAL_INPUT)
	{
		printf("Calibration requires manual input...\n");
		return FALSE;
	}
	else if(status == HD_CALIBRATION_NEEDS_UPDATE)
	{
		hdScheduleSynchronous(UpdateCalibrationCallback, &calibrationStyle,
			HD_DEFAULT_SCHEDULER_PRIORITY);
		if(HD_DEVICE_ERROR(hdGetError()))
		{
			printf("\nFailed to update calibration.\n\n");
			return FALSE;
		}
		else
		{
			printf("\nCalibration updated successfully.\n\n");
			return TRUE;
		}
	}
	else
	{
		assert(!"Unknown calibration status");
		return FALSE;
	}
}

void PrintDevicePosition()
{
	HDdouble position[3];

	hdScheduleSynchronous(DevicePositionCallback, position,
		HD_DEFAULT_SCHEDULER_PRIORITY);
	    
	printf("Device position: %.3f %.3f %.3f\n", 
		position[0], position[1], position[2]);
}

/*****************************************************************************/

