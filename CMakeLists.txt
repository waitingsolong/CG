cmake_minimum_required(VERSION 3.22)

project(main)

set(CMAKE_CXX_STANDARD 20)

add_executable(main "src/main.cpp" "src/managers/shadermanager.h" "src/managers/shadermanager.cpp" "src/managers/texturemanager.h" "src/managers/texturemanager.cpp" "src/movingcamera.h" "src/movingcamera.cpp" "src/globjects/GLObject.h" "src/managers/texture.h" "src/managers/texture.cpp" "src/managers/shader.h" "src/managers/shader.cpp" "src/globjects/GLCube.h" "src/globjects/GLCube.cpp"  "src/globjects/GLSphere.h" "src/globjects/GLSphere.cpp" "src/globjects/GLSkybox.h" "src/globjects/GLSkybox.cpp")
target_compile_definitions(main PUBLIC PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(main
  PRIVATE
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${OPENGL_LIBRARIES})

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/libs/stb)
