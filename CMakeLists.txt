cmake_minimum_required(VERSION 3.10.0)
project(pof)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TEST OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)

add_subdirectory(dep/glm)
add_subdirectory(dep/assimp)
add_subdirectory(dep/glfw)
add_subdirectory(dep/libpqxx)

file(GLOB SERVER_SOURCES "src/server/*.cpp")
file(GLOB CHAT_SERVER_SOURCES "src/chat_server/*.cpp")
file(GLOB INSTANCE_SOURCES "src/instance/*.cpp")
file(GLOB GRAPHICS_SOURCES "src/graphics/*.cpp" "src/graphics/*.c" "src/graphics/model/*.cpp")
file(GLOB MANIPULATOR_SOURCES "src/manipulator/*.cpp")
file(GLOB CLIENT_SOURCES "src/client/*.cpp")
file(GLOB WORLD_STRUCTURE_SOURCES "src/world_structure/*.cpp")
file(GLOB PHYS_SOURCES "src/phys/*.cpp")
file(GLOB CORE_SOURCES "src/core/*.cpp")
file(GLOB SCENE_SOURCES "src/scene/*.cpp")
file(GLOB CONTROLS_SOURCES "src/controls/*.cpp")
file(GLOB COMPONENTS_SOURCES "src/components/*.cpp")
file(GLOB UI_SOURCES "src/ui/*.cpp")
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.72 COMPONENTS system thread log log_setup REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${Boost_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

link_libraries(glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl)
add_library(chat STATIC src/lib/chat.cpp)
link_libraries(chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat)
add_library(uuid STATIC src/lib/uuid.cpp)
link_libraries(uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid)
add_library(user_card STATIC src/common/user_card.cpp)
link_libraries(user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card)
add_library(log STATIC src/lib/log.cpp)
link_libraries(log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log)
add_library(crypto STATIC src/lib/crypto.cpp)
target_link_libraries(crypto OpenSSL::SSL)
link_libraries(crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto)
add_library(call STATIC src/lib/call.cpp)
link_libraries(call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call)
add_library(database STATIC src/lib/database.cpp)
target_link_libraries(database pqxx)
link_libraries(database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database)
add_library(protocol STATIC src/lib/protocol.cpp)
link_libraries(protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol)
add_library(rcon STATIC src/lib/rcon.cpp)
link_libraries(rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon)
add_library(core STATIC ${CORE_SOURCES})
link_libraries(core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core)
add_library(scene STATIC ${SCENE_SOURCES})
link_libraries(scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene)
add_library(control STATIC ${CONTROLS_SOURCES})
link_libraries(control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control)
add_library(components STATIC ${COMPONENTS_SOURCES})
link_libraries(components control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control components)
add_library(phys STATIC ${PHYS_SOURCES})
link_libraries(phys components control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control components phys)
add_library(nuklear STATIC src/lib/nuklear.cpp)
link_libraries(nuklear phys components control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control components phys nuklear)
add_library(graphics STATIC ${GRAPHICS_SOURCES})
link_libraries(graphics nuklear phys components control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control components phys nuklear graphics)
add_library(ui STATIC ${UI_SOURCES})
link_libraries(ui graphics nuklear phys components control scene core rcon protocol database call crypto log user_card uuid chat glfw glm assimp OpenGL::GL ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl chat uuid user_card log crypto call database protocol rcon core scene control components phys nuklear graphics ui)

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

add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})
add_executable(chat_server ${CHAT_SERVER_SOURCES})
add_executable(instance ${INSTANCE_SOURCES})
add_executable(gfx ${GRAPHICS_SOURCES})
if (UNIX)
include_directories(/usr/include)
add_executable(manipulator ${MANIPULATOR_SOURCES})
target_link_libraries(manipulator readline)
else (UNIX)
add_executable(manipulator ${MANIPULATOR_SOURCES})
endif (UNIX)
add_executable(world_test ${WORLD_STRUCTURE_SOURCES} src/test/world_structure_test.cpp)
add_executable(call_test src/test/call_test.cpp)
add_executable(crypto_test src/test/crypto_test.cpp)
add_executable(trie_test src/test/trie_test.cpp)
add_executable(phys_test src/test/phys_test.cpp)
add_executable(scene_test src/test/scene_test.cpp)

add_dependencies(chat_server rsa_key)
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
add_custom_command(OUTPUT bin/char_server.tar
	COMMAND docker build -t pof_chat_server -f ./dockerfiles/chat_server/Dockerfile .
	COMMAND docker save pof_chat_server -o bin/chat_server.tar
)
add_dependencies(docker instance server rsa_key)
