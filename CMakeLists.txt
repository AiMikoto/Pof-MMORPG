cmake_minimum_required(VERSION 3.10.0)
project(x)

file(GLOB INSTANCE_SOURCES "src/instance/*.cpp")
file(GLOB WORLD_STRUCTURE_SOURCES "src/world_structure/*.cpp")
file(GLOB SHARED_SOURCES "src/lib/*.cpp")
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.72 COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
link_libraries(${Boost_LIBRARIES})

add_executable(instance ${INSTANCE_SOURCES} ${SHARED_SOURCES})
add_executable(world_test ${WORLD_STRUCTURE_SOURCES} ${SHARED_SOURCES} src/test/world_structure_test.cpp)
