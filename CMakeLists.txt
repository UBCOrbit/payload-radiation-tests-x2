cmake_minimum_required (VERSION 3.1)
project (Radiation-Tests CXX)

set (CMAKE_CXX_STANDARD 11)

file (GLOB_RECURSE SOURCES src/*.cpp)

FIND_PACKAGE ( Threads REQUIRED )

add_executable (radtests ${SOURCES})

TARGET_LINK_LIBRARIES ( radtests ${CMAKE_THREAD_LIBS_INIT} )