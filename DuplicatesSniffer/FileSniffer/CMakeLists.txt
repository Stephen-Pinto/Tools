# CMakeList.txt : CMake project for DuplicateSniffer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

#boost library setup

set(BOOST_ROOT "C:\\Libraries\\boost_1_64_0")
set(INTL_TBB_ROOT "C:\\Libraries\\tbb_2019")
set(FILE_SNIFFER_ROOT ".")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    set(BOOST_STATIC_SYSTEM_LIBRARY ${Boost_SYSTEM_LIBRARY_DEBUG})
    set(BOOST_STATIC_FILESYSTEM_LIBRARY ${Boost_FILESYSTEM_LIBRARY_DEBUG})
else()
    set(BOOST_STATIC_SYSTEM_LIBRARY ${Boost_SYSTEM_LIBRARY_RELEASE})
    set(BOOST_STATIC_FILESYSTEM_LIBRARY ${Boost_FILESYSTEM_LIBRARY_RELEASE})
endif()

set(Boost_NO_SYSTEM_PATHS true)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.64 REQUIRED 
  COMPONENTS
  system program_options thread filesystem)

set(INTL_TBB_INCLUDE_DIRS ${INTL_TBB_ROOT}/include)
set(INTL_TBB_LIBRARIES ${INTL_TBB_ROOT}/lib/vc14)

include_directories(${Boost_INCLUDE_DIRS} ${INTL_TBB_INCLUDE_DIRS})
link_directories(${INTL_TBB_LIBRARIES})
link_libraries(${Boost_LIBRARIES})

# Add source to this project's executable.
file(GLOB FileSniffer_SRC
    "*.h"
    "*.cpp"
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include/common ${CMAKE_CURRENT_SOURCE_DIR}/include/engine)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

#target_compile_options(FileSniffer PRIVATE -fexceptions)
#add_compile_options(/MT)

add_executable (FileSniffer ${FileSniffer_SRC})
#target_link_libraries(FileSniffer Boost::boost Boost::filesystem)
#target_compile_options(FileSniffer PRIVATE /MT)
target_link_libraries(FileSniffer tbb tbb_debug)

get_directory_property(_my_link_dirs LINK_DIRECTORIES)
message(STATUS "_my_link_dirs = ${INTL_TBB_LIBRARIES}")