project (ogre-test)
cmake_minimum_required (VERSION 2.6)

include (FindQt4)
find_package (Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)
find_package (PkgConfig)

if (MSVC)
    set (OGRE_INCLUDE_DIRS "C:/OgreSDK/include")
    set (OGRE_LIBRARY_DIRS "C:/OgreSDK/lib")
else ()
    pkg_check_modules (OIS OIS)
    pkg_check_modules (OGRE OGRE)
endif ()


if (NOT QT4_FOUND)
    message (FATAL_ERROR "Qt4 not found")
endif (NOT QT4_FOUND)

include (${QT_USE_FILE})

include_directories (${OGRE_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS})
link_directories (${OGRE_LIBRARY_DIRS} ${OIS_LIBRARY_DIRS})
add_definitions (${OGRE_CFLAGS_OTHER} ${OIS_CFLAGS_OTHER})

add_executable (ogretest main.cpp)
    
target_link_libraries (ogretest ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${QT_LIBRARIES})
