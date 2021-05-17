# CMakeList.txt : CMake project for duplicate_sniffer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#Adding boost
set(Boost_INCLUDE_DIR "D:\\Workspace\\CPP\\Libraries\\boost_1_69_0")
set(Boost_LIBRARY_DIR "D:\\Workspace\\CPP\\Libraries\\boost_1_69_0")
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# Add source to this project's executable.
add_executable (duplicate_sniffer "duplicate_sniffer.cpp" "duplicate_sniffer.h")

# TODO: Add tests and install targets if needed.
target_link_libraries( main ${Boost_LIBRARIES} )
