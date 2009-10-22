
project (ogretest)
cmake_minimum_required (VERSION 2.6)

# cmake_module_path (.)             # where cmake modules are located
# cmake_install_prefix (/usr)       # where make install points
# cmake_build_type ()               # debug, release, ...
# build_shared_libs ()              # switch between static/shared

find_package (PkgConfig)
pkg_check_modules (OGRE OGRE)
pkg_check_modules (OIS OIS)

# add_subdirectory (.)              # recurse into subdirs

include_directories (${OGRE_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS})             # Add include directories to the build.
link_directories (${OGRE_LIBRARIES_DIRS} ${OIS_LIBRARIES_DIRS})                # Specify directories in which the linker will look for libraries.

add_definitions (${OGRE_CFLAGS_OTHER} ${OIS_CFLAGS_OTHER})                  # Adds -D define flags to the compilation of source files.
# add_dependencies (main dep)       # Add a dependency between top-level targets.

add_executable (ogretest main.cpp)
    
target_link_libraries (ogretest ${OGRE_LIBRARIES} ${OIS_LIBRARIES})
