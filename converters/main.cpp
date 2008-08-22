
/* main.cpp -- main module
 *
 *			Ryan McDougall -- 20070101
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <object.h>
#include <vrml.h>
#include <graphics/triangle.hpp>

using namespace ThreeDInc;

//=============================================================================
// 
template <typename T> void deletefun (T* obj) { delete obj; }

//=============================================================================
// Main entry point
int main (int argc, char** argv)
{
    logstream& log = get_logstream ("main");

    if (argc != 2)
    {
	std::cout << "usage: " << argv[0] << " <file name>" << std::endl;
	return 0;
    }
		
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

    // cleanup
    std::for_each (list.begin(), list.end(), deletefun<ThreeDInc::Object>);
}
