cmake_minimum_required(VERSION 3.28)
project(mancala-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(mancala ${SOURCES})