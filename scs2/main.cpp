/* main.cpp -- main module
 *
 * Design decisions:
 *
 * Threading:
 *
 * - for users to feel the graphics and haptics output is realisitc, 
 *   the graphics loop needs to update 30-60 times per second, and 
 *   the haptics loop needs to update 1000 times per second
 * 
 * - both the graphics thread and haptics threads are managed by GLUT
 *   and OpenHaptics Toolkit respectively, and are updated as often
 *   as possible to maintain this rate
 * 
 * - the SmartCollision computations can be time consuming and should be kept
 *   in its own thread, however to simplify the code, we can call 
 *   SmartCollision from the graphics or haptics thread
 * 
 * - for performance reasons we decided to place SmartCollision calls in the
 *   graphics loop. more realistic motion on faster machines can be had by
 *   placing the code in the haptics loop, or ideally in its own thread
 *
 * Coordinate systems:
 * 
 * - the haptics device has its own native coordinate system that is used to 
 *   report the position and orientation of the haptic pointer. of course
 *   opengl has its own coordinate system defined by the modelview matrix, and
 *   used to transform the local coordinates of the input model.
 * 
 * - to correctly compute collisions, one must have a unified coordinate system
 *   containing the pointer position and orientation, and the position and 
 *   orientation of the model. the translation and rotational penetration depth
 *   vectors returned by SmartCollision will be in this system.
 * 
 * - to manage the conversions from points from one system to the next, one must
 *   maintain a transformation to/from the desired systems. to map things from 
 *   the haptic system (called the workspace) to the graphics space, OHT 
 *   provides a function called hduMapWorkspaceModel() which takes a modelview 
 *   and projection matrix and computes a suitable map. the source for this 
 *   function are distributed with the OHT source.
 *
 * - for the purpose of simplifying the code, we have chosen to use the 
 *   graphics coordinate system as the common system.
 *
 *			Ryan McDougall -- 20070101
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <cmath>
#include <cassert>

#include <GL/glut.h>
#include <HD/hd.h>
#include <HDU/hdu.h>
#include <HDU/hduVector.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>

#include <sc.h>

#include <types.h>
#include <object.h>
#include <util/numeric.hpp>
#include <objfileloader.hpp>
#include <linear/linoperator.hpp>
#include <vrml.h>

// application global variables
ThreeDInc::Window window;
ThreeDInc::Mouse mouse;
ThreeDInc::Pointer pointer;
ThreeDInc::Control control;
ThreeDInc::Camera cam;

using ThreeDInc::Object;
using ThreeDInc::index_t;
using ThreeDInc::vertex_t;
using ThreeDInc::normal_t;
using ThreeDInc::real_t;
using ThreeDInc::object_list;
using ThreeDInc::VEC3_SIZE;
using ThreeDInc::MATRIX4_SIZE;

using ThreeDInc::cross;
    
// SC variables
SCSceneManager manager (SC_OBJECT_TYPE_TRIANGLE_SOUP);
SCObject scobject (SC_OBJECT_TYPE_TRIANGLE_SOUP);
SCObject scptrobject (SC_OBJECT_TYPE_TRIANGLE_SOUP);

// transform from model space to haptic workspace
vertex_t haptic_graphic_map [MATRIX4_SIZE];
vertex_t haptic_collision_map [MATRIX4_SIZE];

// temporary variables
real_t R [MATRIX4_SIZE];
real_t T [MATRIX4_SIZE];
real_t U [MATRIX4_SIZE];
real_t v [VEC3_SIZE];
real_t pd [VEC3_SIZE] = { 0.0, 0.0, 0.0 };
real_t last_pos[VEC3_SIZE] = { 0.0, 0.0, 0.0 };
bool bCollision = false;

// constant matrices
real_t I [MATRIX4_SIZE] = {
    1.0, 0.0, 0.0, 0.0, 
    0.0, 1.0, 0.0, 0.0, 
    0.0, 0.0, 1.0, 0.0, 
    0.0, 0.0, 0.0, 1.0, 
};

// OpenHaptics handles
HHD device_handl;
HDCallbackCode planecb_handl;

// object geometry pointers
const vertex_t *vertices;
const normal_t *normals;
const index_t *indices;
size_t numvert = 0;
size_t numind = 0;
size_t numnorm = 0;

// pointer box geometry
const size_t numboxvert = 3*4*6;
const vertex_t box_vertices [numboxvert] = {
    1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0,
    1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0,
    -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0,
    1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0,
    1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0 
};

const size_t numboxnorm = 3*4*6;
const normal_t box_normals [numboxnorm] = {
    0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 
    1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 
    -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 
    0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 
    0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
};

const size_t numboxind = 4*6;
const index_t box_indices [numboxind] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 
};

// control variables
float zoom = -100.0;
float roty = 0.0;
float rotx = 0.0;

//=============================================================================
template <typename T> void deletefun (T* obj) { delete obj; }

void print_usage_and_exit (const char* prgname)
{
    std::cerr << prgname << ": [[object file name] ...]" << std::endl; 
    exit (0);

}

// initialize application parameters
void init_scene ()
{
    window.height = 1024;
    window.width = 1024;

    cam.fov = 45;
    cam.near = 1.0;
    cam.far = 200;
    cam.dir[0] = 0;
    cam.dir[1] = 0;
    cam.dir[2] = 1;
    cam.pos[0] = 0;
    cam.pos[1] = 0;
    cam.pos[2] = -10;
    cam.up[0] = 0;
    cam.up[1] = 1;
    cam.up[2] = 0;

    cross (cam.dir, cam.up, cam.cross);
}

// initialize OpenGL parameters
void init_opengl ()
{
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    
    glPolygonMode (GL_FRONT, GL_FILL);
    
    glClearColor (0.0, 0.0, 0.0, 0.0);

    assert (glGetError() == GL_NO_ERROR);
}

// initialize SC parameters
void init_sc ()
{
    manager.SetAttributeDouble (SC_SCENE_MANAGER_MAX_DISTANCE, 1000);
    manager.SetAttributeDouble (SC_SCENE_MANAGER_TOLERANCE, 0.1);
    manager.SetAttributeInteger (SC_SCENE_MANAGER_MAX_ITERATION, 200);
    manager.SetAttributeEnum (SC_SCENE_MANAGER_ROTATION_MODE, SC_ROTATION_MODE_INPUT);
}

// Haptic plane callback to get the position of the pointer
HDCallbackCode HDCALLBACK get_haptic_position_cb (void *data)
{
    hdBeginFrame (hdGetCurrentDevice());
    
    // Load haptic info into the pointer struct
    std::copy (pointer.pos, pointer.pos+3, pointer.prevpos);
    hdGetDoublev (HD_CURRENT_POSITION, pointer.pos);
    hdGetDoublev (HD_CURRENT_TRANSFORM, pointer.trans);

	real_t force [VEC3_SIZE] = { 0.0, 0.0, 0.0 };

	// force calculation
	if( bCollision )
	{
		force[0] = -0.4*(pd[0] + pointer.pos[0] - last_pos[0]);
		force[1] = -0.4*(pd[1] + pointer.pos[1] - last_pos[1]);
		force[2] = -0.4*(pd[2] + pointer.pos[2] - last_pos[2]);
	}

	hdSetDoublev (HD_CURRENT_FORCE, force );
    
    hdEndFrame (hdGetCurrentDevice());

    // In case of error, terminate the callback
    HDErrorInfo error;
    if (HD_DEVICE_ERROR (error = hdGetError()))
    {
	hduPrintError (stderr, &error, "Error detected during main scheduler callback\n");

	if (hduIsSchedulerError (&error))
	    return HD_CALLBACK_DONE;  
    }

    return HD_CALLBACK_CONTINUE;
}

// Initialize the device, create a callback to handle plane forces, terminate 
// upon key press.
void init_openhl ()
{
    HDErrorInfo error;

    // Initialize the default haptic device
    device_handl = hdInitDevice (HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR (error = hdGetError()))
    {
	hduPrintError (stderr, &error, "Failed to initialize haptic device");
	exit (-1);
    }

    // Start the servo scheduler and enable forces
    hdEnable (HD_FORCE_OUTPUT);
    hdStartScheduler();

    if (HD_DEVICE_ERROR (error = hdGetError()))
    {
	hduPrintError (stderr, &error, "Failed to start the scheduler");
	exit (-1);
    }

    // Schedule our the frictionless plane callback, which will then run at 
    // servoloop rates and command forces if the user penetrates the plane
    planecb_handl = hdScheduleAsynchronous
	(get_haptic_position_cb, 0, HD_DEFAULT_SCHEDULER_PRIORITY);

}

// cleanup and shutdown the haptic device, cleanup all callbacks
void final_openhl ()
{
    hdStopScheduler();
    hdUnschedule (planecb_handl);
    hdDisableDevice (device_handl);
}

// draw simple axes in the origin
void drawaxes ()
{
    glDisable (GL_LIGHTING);
    glBegin (GL_LINES);
    glColor3d (1.0, 0.0, 0.0);
    glVertex3d (0.0, 0.0, 0.0);
    glVertex3d (1000.0, 0.0, 0.0);
    glColor3d (0.0, 1.0, 0.0);
    glVertex3d (0.0, 0.0, 0.0);
    glVertex3d (0.0, 1000.0, 0.0);
    glColor3d (0.0, 0.0, 1.0);
    glVertex3d (0.0, 0.0, 0.0);
    glVertex3d (0.0, 0.0, 1000.0);
    glEnd();
    glEnable (GL_LIGHTING);

}

// GLUT display callback
void display ()
{    
    // clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // simple controls
	glLoadIdentity();
    glTranslated (0, 0, zoom);
    glRotated (roty, 0.0, 1.0, 0.0);
    glRotated (rotx, 1.0, 0.0, 0.0);

    // Orientation lines
    drawaxes ();

    // draw object
    glVertexPointer (3, GL_DOUBLE, 0, vertices);
    glNormalPointer (GL_DOUBLE, 0, normals);
    glDrawElements (GL_TRIANGLES, numind, GL_UNSIGNED_INT, indices);

    // get workspace maps
    glGetDoublev (GL_MODELVIEW_MATRIX, T);
    glGetDoublev (GL_PROJECTION_MATRIX, U);
    hduMapWorkspaceModel (T, U, haptic_graphic_map);

	// set object's collision transform
	glGetDoublev (GL_MODELVIEW_MATRIX, T);
    manager.SetTransformation (2, SC_TRANSFORMATION_MATRIX, T);

    // transform to workspace coords
    glMultMatrixd (haptic_graphic_map);
    
    // transform to pointer coords
    glMultMatrixd (pointer.trans);

	// set object's collision transform
	glGetDoublev (GL_MODELVIEW_MATRIX, T);
    manager.SetTransformation (1, SC_TRANSFORMATION_MATRIX, T);

    // get status from SC manager
	if( manager.UpdateStatus() == SC_NO_ERROR ){

		// check collision
		double dist;
		manager.GetStatus( SC_DISTANCE, &dist );
		if( dist < 0 )
		{
			bCollision = true;
		}
		else
		{
			bCollision = false;
		}
	}else{
		bCollision = false;
	}
	// calc TPD
	manager.GetStatus (SC_TPD_VECTOR, pd);

	// save current haptic position
	last_pos[0] = pointer.pos[0];
	last_pos[1] = pointer.pos[1];
	last_pos[2] = pointer.pos[2];

	// avoid penetration of the pointer graphics
	if( bCollision )
	{
		manager.GetStatus (SC_PD_INVERSE_MATRIX, T);	
		glMultMatrixd (T);
	}

	// draw oriented pointer
    glVertexPointer (3, GL_DOUBLE, 0, box_vertices);
    glNormalPointer (GL_DOUBLE, 0, box_normals);
    glDrawElements (GL_QUADS, numboxind, GL_UNSIGNED_INT, box_indices);


//	std::cout << "TPD: " << pd[0] << " " << pd[1] << " " << pd[2] << endl;

    glutSwapBuffers();
    assert (glGetError() == GL_NO_ERROR);
}

// GLUT window reshape callback
void reshape (int w, int h)
{
    window.width = w;
    window.height = h;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (cam.fov, (real_t) w / (real_t) h, cam.near, cam.far);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();  

    assert (glGetError() == GL_NO_ERROR);
}

// GLUT keyboard callback
void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
	case 27:
	    glutDestroyWindow (window.id);
	    exit (0);
	    break;

	case 'w': control.forward = 0.1;
		  break;

	case 's': control.forward = -0.1;
		  break;

	case 'a': control.slide = -0.1;
		  break;

	case 'd': control.slide = 0.1;
		  break;

	case 'e': control.climb = 0.1;
		  break;

	case 'q': control.climb = -0.1;
		  break;

	case 'y': zoom += 1.0f;
		  break;

	case 'h': zoom -= 1.0f;
		  break;

	case 'i': rotx += 5.0f;
		  break;

	case 'k': rotx -= 5.0f;
		  break;

	case 'l': roty += 5.0f;
		  break;

	case 'j': roty -= 5.0f;
		  break;

	default:
		std::cerr << "unknown keypress: " << (int) key << std::endl;
    }

    glutPostRedisplay();
    assert (glGetError() == GL_NO_ERROR);
}

void motion (int x, int y)
{
    // keep old coords
    mouse.prevpos[0] = mouse.curpos[0];
    mouse.prevpos[1] = mouse.curpos[1];

    // readjust origin to middle of screen
    mouse.curpos[0] = mouse.accel * (x - (real_t) window.width/2);
    mouse.curpos[1] = mouse.accel * ((real_t) window.height/2 - y);

    // edge fliping
    if ((mouse.curpos[0] * mouse.prevpos[0]) < 0.0) mouse.prevpos[0] *= -1.0;
    if ((mouse.curpos[1] * mouse.prevpos[1]) < 0.0) mouse.prevpos[1] *= -1.0;
}

void sc_check_error (int code)
{
    switch (code)
    {
	case SC_NO_ERROR:
	    break;

	case SC_ERROR_INVALID_TYPE:
	    std::cerr << "SC_ERROR_INVALID_TYPE" << std::endl;
	    exit (-1);

	case SC_ERROR_FAILED:
	    std::cerr << "SC_ERROR_FAILED" << std::endl;
	    exit (-1);

	case SC_ERROR_INVALID_TYPE_COMBINATION:
	    std::cerr << "SC_ERROR_INVALID_TYPE_COMBINATION" << std::endl;
	    exit (-1);

	case SC_ERROR_INVALID_DATA:
	    std::cerr << "SC_ERROR_INVALID_DATA" << std::endl;
	    exit (-1);

	case SC_ERROR_INVALID_BVH_FILE:
	    std::cerr << "SC_ERROR_INVALID_BVH_FILE" << std::endl;
	    exit (-1);

	case SC_ERROR_INVALID_LICENSE:
	    std::cerr << "SC_ERROR_INVALID_LICENSE" << std::endl;
	    exit (-1);

	case SC_ERROR_BAD_ALLOCATION:
	    std::cerr << "SC_ERROR_BAD_ALLOCATION" << std::endl;
	    exit (-1);

	case SC_ERROR_RUNTIME:
	    std::cerr << "SC_ERROR_RUNTIME" << std::endl;
	    exit (-1);

	default:
	    std::cerr << "unknown error" << std::endl;
	    exit (-1);
    }
}

//=============================================================================
// Main entry point
int main (int argc, char** argv)
{
    logstream& log = get_logstream ("main");

    //ObjFileLoader loader;
    Object *obj;

    // command line parsing
    if (argc < 2)
	print_usage_and_exit (argv[0]);

    ThreeDInc::VRMLSceneLoader loader;
    loader.open_file (argv[1]);
    ThreeDInc::object_list list (loader.get_objects());

    size_t n = 0;
    ThreeDInc::object_list::iterator iter, end;
    for (iter = list.begin(), end = list.end();
	 iter != end; ++iter)
    {
	log_info (log, "object " << n++ << ": " << **iter);
    }

    // put the object into global pointers
    obj = *(list.begin());
    vertices = obj->vertex_ptr();
    indices = obj->index_ptr();
    normals = obj->normal_ptr();
    numvert = obj->num_vertex;
    numind = obj->num_index;
    numnorm = obj->num_normal;
    
    // add the pointer geometry to SC
    sc_check_error (scptrobject.AddTriangles (box_vertices, numboxvert/3, box_indices, numboxind/3));

    manager.AddObject (1, &scptrobject); 
    manager.AddObjectToGroup (1, 1);

    // add the object geometry to SC
    sc_check_error (scobject.AddTriangles (vertices, numvert/3, indices, numind/3));

    manager.AddObject (2, &scobject);
    manager.AddObjectToGroup (2, 2);

    // initialize scene
    init_scene();
    init_sc();

    // initialize GLUT
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (window.width, window.height);

    // create window 
    window.id = glutCreateWindow ("testing");

    // set up callback functions
    glutDisplayFunc (display);
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutMotionFunc (motion);
    glutPassiveMotionFunc (motion);

    // initialize OpenGL and OpenHaptics
    init_opengl();
    init_openhl();

    // start graphics loop
    glutMainLoop();

    // finalize
    final_openhl ();

    // cleanup
    std::for_each (list.begin(), list.end(), deletefun<ThreeDInc::Object>);

    return 0;
}

