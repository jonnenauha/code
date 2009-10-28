project (ogretest)
cmake_minimum_required (VERSION 2.6)

include (FindQt4)
find_package (Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)

if (NOT QT4_FOUND)
    message (FATAL_ERROR "Qt4 not found")
endif (NOT QT4_FOUND)

include(${QT_USE_FILE})

find_package (PkgConfig)
pkg_check_modules (OGRE OGRE)
pkg_check_modules (OIS OIS)

include_directories (${OGRE_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS})             # Add include directories to the build.
link_directories (${OGRE_LIBRARIES_DIRS} ${OIS_LIBRARIES_DIRS})                # Specify directories in which the linker will look for libraries.
add_definitions (${OGRE_CFLAGS_OTHER} ${OIS_CFLAGS_OTHER})                  # Adds -D define flags to the compilation of source files.

qt4_generate_moc (main.h main_moc.cpp)

add_executable (ogretest main_moc.cpp main.cpp)
    
target_link_libraries (ogretest ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${QT_LIBRARIES})
