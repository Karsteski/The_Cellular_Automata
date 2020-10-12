cmake_minimum_required(VERSION 3.17)

project(imgui)


add_library(imgui
  imgui_widgets.cpp
  imgui.cpp
  imgui_impl_glfw.cpp
  imgui_impl_opengl3.cpp
  imgui_demo.cpp
  imgui_widgets.cpp
  imgui_draw.cpp
  GL/gl3w.c
  misc/cpp/imgui_stdlib.cpp)
target_include_directories(imgui PUBLIC "./misc/cpp/" "./")

find_package(OpenGL)
find_package(glfw3)
target_link_libraries(imgui PRIVATE glfw ${CMAKE_DL_LIBS})

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# I don't know what linking is necessary on windows, mac
if(LINUX)
  target_link_libraries(imgui PRIVATE  OpenGL::GLX)
endif()

add_library(imgui::imgui ALIAS imgui)
