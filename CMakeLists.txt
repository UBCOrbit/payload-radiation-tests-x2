cmake_minimum_required (VERSION 3.7)
project (Radiation-Tests)

file (GLOB SOURCES *.cpp)
add_executable (radiation-tests ${SOURCES})
