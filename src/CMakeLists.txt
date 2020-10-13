cmake_minimum_required(VERSION 3.17)

project(life
  DESCRIPTION "Conway's game of life"
  VERSION 0.0.0
  LANGUAGES CXX)

add_subdirectory(imgui)

add_executable(life main.cpp
  Application.cpp
  GameOfLife.cpp
  Elementary.cpp
  Grid.cpp)
target_link_libraries(life PRIVATE imgui::imgui)
target_compile_features(life PRIVATE cxx_std_17)
