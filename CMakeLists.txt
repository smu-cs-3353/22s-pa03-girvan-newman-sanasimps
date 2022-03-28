cmake_minimum_required(VERSION 3.15)
project(22s_pa03)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE -O3)

set(BOOST_ROOT "/usr/local/Cellar/boost/1.78.0_1/include")
find_package(Boost 1.78.0)
include_directories(${BOOST_ROOT})

#set(1 file1)

#foreach(file IN LISTS 1)
#    configure_file(${file} ${file} COPYONLY)
#endforeach()

add_executable(pa03 src/main.cpp)