cmake_minimum_required (VERSION 3.1)
project (Radiation-Tests)

set (CMAKE_CXX_STANDARD 11)

file (GLOB_RECURSE SOURCES src/*.cpp)
add_executable (radiation-tests ${SOURCES})
