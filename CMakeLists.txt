cmake_minimum_required(VERSION 3.22)

project(main)

add_executable(main src/main.cpp)

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
