cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")
add_executable(MyProject ${SOURCES})

target_link_libraries(MyProject ${OPENGL_LIBRARIES})
