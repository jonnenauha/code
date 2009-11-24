project (ogre-test)
cmake_minimum_required (VERSION 2.6)

include (../../sagase-cmake/Sagase.cmake)

include (FindQt4)
find_package (Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)
find_package (PkgConfig)

sagase_configure_package (OGRE 
    NAMES Ogre OgreSDK ogre OGRE
    COMPONENTS Ogre ogre OGRE OgreMain 
    PREFIXES ${ENV_OGRE_HOME})


sagase_configure_package (OIS 
    NAMES Ois ois OIS 
    COMPONENTS OIS
    PREFIXES ${OIS_HOME})
    
sagase_configure_report (OGRE) 
sagase_configure_report (OIS)

if (NOT QT4_FOUND)
    message (FATAL_ERROR "Qt4 not found")
endif (NOT QT4_FOUND)

include (${QT_USE_FILE})

include_directories (${OGRE_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS})
link_directories (${OGRE_LIBRARY_DIRS} ${OIS_LIBRARY_DIRS})
add_definitions (${OGRE_CFLAGS_OTHER} ${OIS_CFLAGS_OTHER})

add_executable (ogretest main.cpp)
    
target_link_libraries (ogretest ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${QT_LIBRARIES})
