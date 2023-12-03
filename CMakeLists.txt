cmake_minimum_required(VERSION 3.10)
project(SnakeYou)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(dependencies/raylib)
add_executable(SnakeYou main.cpp)
target_link_libraries(SnakeYou PUBLIC raylib)
