cmake_minimum_required(VERSION 3.0.0)
project(x)

file(GLOB INSTANCE_SOURCES "src/instance/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(instance ${INSTANCE_SOURCES})
