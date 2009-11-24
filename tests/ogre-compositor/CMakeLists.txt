project (ogretest)
cmake_minimum_required (VERSION 2.6)

include (../../sagase-cmake/Sagase.cmake)

include (FindQt4)
find_package (Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)

if (NOT QT4_FOUND)
    message (FATAL_ERROR "Qt4 not found")
endif (NOT QT4_FOUND)

include(${QT_USE_FILE})

if (MSVC)
    file (TO_CMAKE_PATH $ENV{OGRE_HOME} ENV_OGRE_HOME)
endif ()

sagase_configure_package (OGRE 
    NAMES Ogre OgreSDK ogre OGRE
    COMPONENTS Ogre ogre OGRE OgreMain 
    PREFIXES ${ENV_OGRE_HOME})

sagase_configure_report (OGRE) 

# manually use custom ogre
#set (OGRE_INCLUDE_DIRS ../ogre/build)
#set (OGRE_LIBRARY_DIRS ../ogre/build)
#set (OGRE_CFLAGS_OTHER -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN)

# manually use 4.6-beta
#set (QT_LIBRARIES QtCore QtGui)
#include_directories (/usr/local/Trolltech/Qt-4.6.0/include /usr/local/Trolltech/Qt-4.6.0/include/QtCore /usr/local/Trolltech/Qt-4.6.0/include/QtGui)
#link_directories (/usr/local/Trolltech/Qt-4.6.0/lib)
#add_definitions (-DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NO_KEYWORDS -DQT_DEBUG)

include_directories (${OGRE_INCLUDE_DIRS})             # Add include directories to the build.
link_directories (${OGRE_LIBRARY_DIRS})                # Specify directories in which the linker will look for libraries.
add_definitions (${OGRE_CFLAGS_OTHER})                  # Adds -D define flags to the compilation of source files.

qt4_generate_moc (main.h main_moc.cpp)

add_executable (ogretest main_moc.cpp main.cpp)
    
if (MSVC)
    target_link_libraries (ogretest ${OGRE_LIBRARIES} ${QT_LIBRARIES} debug ${OGRE_DEBUG_LIBRARIES})
else ()
    target_link_libraries (ogretest ${OGRE_LIBRARIES} ${QT_LIBRARIES})
endif ()
