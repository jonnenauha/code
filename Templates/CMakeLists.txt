
project (main-proj)
cmake_minimum_required (VERSION 2.6)

# cmake_module_path (.)             # where cmake modules are located
# cmake_install_prefix (/usr)       # where make install points
# cmake_build_type ()               # debug, release, ...
# build_shared_libs ()              # switch between static/shared

# find_package (PkgConfig)
# pkg_check_modules (PREFIX module) # use pkg-config to find module

# add_subdirectory (.)              # recurse into subdirs

# include_directories (.)           # Specify directories in which the preprocessor will look for includes.
# link_directories (.)              # Specify directories in which the linker will look for libraries.

# add_definitions (-DLART)          # Adds -D define flags to the compilation of source files.
# add_dependencies (main dep)       # Add a dependency between top-level targets.

# add_library (foo foo.cpp)         # create a library
add_executable (main main.cpp)      # create an executable

# target_link_libraries (main foo)  # Link a target to given libraries.
