cmake_minimum_required(VERSION 3.0.0)

project(Automata
  DESCRIPTION "Cellular Automata Generator"
  VERSION 1.0.0
  LANGUAGES C CXX)  

# Add source files
file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files.
file(GLOB_RECURSE HEADER_FILES
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files.
link_directories(${CMAKE_SOURCE_DIR}src/imgui)

# These are the only global variables that should be set.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Main Project.
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    "src"
    "src/imgui" 
    "src/imgui/GLFW" 
    "src/imgui/lib-vc2019"
    "src/imgui/misc/cpp")

# GLFW: Static linkage, no dependecies. Note the use of a .lib in this situation.
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/imgui/lib-vc2019/glfw3.lib)

# GL3W: Static linkage, no dependencies.
add_library(gl3w STATIC)
target_include_directories(gl3w PUBLIC "src/imgui/")
target_sources(gl3w PRIVATE src/imgui/GL/gl3w.c)
target_link_libraries(${PROJECT_NAME} gl3w)

# OpenGL, comes w/ Windows.
find_package(OpenGL)
target_link_libraries(${PROJECT_NAME} OpenGL::GL gl3w)

# Dear ImGui: Static linkage, no dependencies. 
add_library(imgui STATIC)
target_include_directories(imgui PUBLIC "src/imgui")
target_sources(
    imgui 
    PRIVATE
    "src/imgui/imgui_widgets.cpp"
    "src/imgui/imgui.cpp"
    "src/imgui/imgui_impl_glfw.cpp"
    "src/imgui/imgui_impl_opengl3.cpp"
    "src/imgui/imgui_demo.cpp"
    "src/imgui/imgui_widgets.cpp"
    "src/imgui/imgui_draw.cpp"
    "src/imgui/misc/cpp/imgui_stdlib.cpp")
target_link_libraries(${PROJECT_NAME} imgui)

# Catch2 Testing.
# Need to figure out a way to exclude Application.cpp, or else I get a linker error because of main() redef.
add_executable(
    tests 
    tests/tests.cpp 
    tests/Catch2.hpp 
    src/Elementary.cpp
    src/GameOfLife.cpp
    src/Grid.cpp)

target_include_directories(
    tests 
    PUBLIC 
    "src"
    "src/imgui" 
    "src/imgui/GLFW" 
    "src/imgui/lib-vc2019"
    "src/imgui/misc/cpp")

# Necessary linkages for the tests.exe to properly compile.
target_link_libraries(tests gl3w)
target_link_libraries(tests OpenGL::GL gl3w)
target_link_libraries(tests imgui)

# To nicely support folders in IDEs. 
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

# Set executable target destination.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
