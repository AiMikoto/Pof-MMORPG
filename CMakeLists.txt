cmake_minimum_required(VERSION 3.10.0)
project(pof)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dep/glfw)

file(GLOB SERVER_SOURCES "src/server/*.cpp")
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
find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)
include_directories(${Boost_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

link_libraries(${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

add_library(chat STATIC src/lib/chat.cpp)
add_library(user_card STATIC src/common/user_card.cpp)
add_library(log STATIC src/lib/log.cpp)
add_library(crypto STATIC src/lib/crypto.cpp)
target_link_libraries(crypto OpenSSL::SSL)
add_library(call STATIC src/lib/call.cpp)
add_library(database STATIC src/lib/database.cpp)
target_link_libraries(database ${PostgreSQL_LIBRARIES})
link_libraries(${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} log call user_card crypto database chat)
add_library(protocol STATIC src/lib/protocol.cpp)
link_libraries(${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} log call protocol user_card crypto database chat)

# idk how to set this for windows
set(OPENSSL openssl)

add_custom_target(rsa_key DEPENDS priv pub)
add_custom_target(keys_f DEPENDS keys)
add_custom_command(OUTPUT keys
	COMMAND ${CMAKE_COMMAND} -E make_directory keys
)
add_custom_target(priv DEPENDS keys/private_key.pem keys_f)
add_custom_command(OUTPUT keys/private_key.pem
	COMMAND ${OPENSSL} genpkey -algorithm RSA -out keys/private_key.pem -pkeyopt rsa_keygen_bits:16384
)
add_custom_target(pub DEPENDS keys/public_key.pem priv)
add_custom_command(OUTPUT keys/public_key.pem
	COMMAND ${OPENSSL} rsa -pubout -in keys/private_key.pem -out keys/public_key.pem
)

add_executable(server ${SERVER_SOURCES})
add_executable(instance ${INSTANCE_SOURCES})
add_executable(gfx ${GRAPHICS_SOURCES})
target_link_libraries(gfx glfw)
target_link_libraries(gfx OpenGL::GL)
add_executable(client ${CLIENT_SOURCES})
add_executable(world_test ${WORLD_STRUCTURE_SOURCES} src/test/world_structure_test.cpp)
add_executable(call_test src/test/call_test.cpp)
add_executable(crypto_test src/test/crypto_test.cpp)
add_dependencies(server rsa_key)
add_dependencies(instance rsa_key)
add_dependencies(client rsa_key)

file(COPY dockerfiles DESTINATION .)
add_custom_target(docker DEPENDS bin/instance.tar bin/server.tar bin/database.tar)
add_custom_command(OUTPUT bin/instance.tar
	COMMAND docker build -t pof_instance -f ./dockerfiles/instance/Dockerfile .
	COMMAND docker save pof_instance -o bin/instance.tar
)
add_custom_command(OUTPUT bin/server.tar
	COMMAND docker build -t pof_server -f ./dockerfiles/server/Dockerfile .
	COMMAND docker save pof_server -o bin/server.tar
)
add_custom_command(OUTPUT bin/database.tar
	COMMAND docker build -t pof_database -f ./dockerfiles/database/Dockerfile .
	COMMAND docker save pof_database -o bin/database.tar
)
add_dependencies(docker instance server rsa_key)
