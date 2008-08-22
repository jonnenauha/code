/* main.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <list>
#include <vector>

#include <cstdlib>
#include <cstring>

using namespace std;

#include <parameters.hpp>
#include <parsing.hpp>
#include <sip.hpp>
#include <state.hpp>
#include <server.hpp>

// global reference to server instance 
// created in main.cpp
extern Server *glb_server;

// log output macro
#define  VFVW_LOG(fmt, ...)  fprintf(stderr, "[VFVW] %s:%d - ", __FILE__, __LINE__);	\
	fprintf(stderr, fmt, __VA_ARGS__);	\
	fprintf(stderr, "\r\n")

// defines
#define	VFVW_HANDLE	"xxxx"

#define VFVW_SL_VOLUME_MIN		(-100)
#define VFVW_SL_VOLUME_MAX		24
#define VFVW_SL_VOLUME_RANGE	(VFVW_SL_VOLUME_MAX - VFVW_SL_VOLUME_MIN)

#define VFVW_PJ_VOLUME_MIN		0
//#define VFVW_PJ_VOLUME_MAX		255
#define VFVW_PJ_VOLUME_MAX		3
#define VFVW_PJ_VOLUME_RANGE	(VFVW_PJ_VOLUME_MAX - VFVW_PJ_VOLUME_MIN)


#endif //_MAIN_H_
