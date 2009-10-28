
project (ogre-test)
cmake_minimum_required (VERSION 2.6)

include (FindQt4)
find_package (Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)

if (NOT QT4_FOUND)
    message (FATAL_ERROR "Qt4 not found")
endif (NOT QT4_FOUND)

include(${QT_USE_FILE})

# cmake_module_path (.)             # where cmake modules are located
# cmake_install_prefix (/usr)       # where make install points
# cmake_build_type ()               # debug, release, ...
# build_shared_libs ()              # switch between static/shared

# add_subdirectory (.)              # recurse into subdirs

# manually use 4.6-beta
#include_directories (/usr/local/Trolltech/Qt-4.6.0/include/QtCore /usr/local/Trolltech/Qt-4.6.0/include/QtGui)
#link_directories (/usr/local/Trolltech/Qt-4.6.0/lib)
#add_definitions (-DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NO_KEYWORDS -DQT_DEBUG)

# add_dependencies (main dep)       # Add a dependency between top-level targets.

qt4_generate_moc (ogrewidget.h ogrewidget_moc.cpp)
qt4_generate_moc (testwidget.h testwidget_moc.cpp)
qt4_generate_moc (viewmodel.h viewmodel_moc.cpp)

add_executable (ogre-test main.cpp 
    testwidget.cpp testwidget_moc.cpp
    ogrewidget.cpp ogrewidget_moc.cpp
    viewmodel.cpp viewmodel_moc.cpp)
    
target_link_libraries (ogre-test OgreMain ${QT_LIBRARIES})
