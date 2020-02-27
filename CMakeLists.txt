cmake_minimum_required(VERSION 3.10.0)
project(pof)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dep/glfw)

file(GLOB INSTANCE_SOURCES "src/instance/*.cpp")
file(GLOB GRAPHICS_SOURCES "src/graphics/*.cpp" "src/graphics/*.c")
file(GLOB CLIENT_SOURCES "src/client/*.cpp")
file(GLOB WORLD_STRUCTURE_SOURCES "src/world_structure/*.cpp")
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.72 COMPONENTS system thread log log_setup REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${Boost_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

link_libraries(${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

add_library(log STATIC src/lib/log.cpp)
add_library(call STATIC src/lib/call.cpp)

link_libraries(${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} log call)

add_executable(instance ${INSTANCE_SOURCES})
add_executable(gfx ${GRAPHICS_SOURCES})
target_link_libraries(gfx glfw)
target_link_libraries(gfx OpenGL::GL)
add_executable(client ${CLIENT_SOURCES})
add_executable(world_test ${WORLD_STRUCTURE_SOURCES} src/test/world_structure_test.cpp)
add_executable(call_test ${WORLD_STRUCTURE_SOURCES} src/test/call_test.cpp)
