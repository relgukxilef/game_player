cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game_player)

add_executable(game_player main.cpp)

set_property(TARGET game_player PROPERTY CXX_STANDARD 20)