# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.11)

project ("MiniTools")

# Include sub-projects.
add_subdirectory ("FileSniffer")

#add_compile_options("/std:c++latest")
#if(MSVC)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
#endif(MSVC)