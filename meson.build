project('Cellular-Automata-Generator', 'cpp', default_options : 'cpp_std=c++17')

# Internal libraries
subdir('libs/imgui')

src_files = [
    './src/Application.cpp',
    './src/Elementary.cpp',
    './src/GameOfLife.cpp',
    './src/Grid.cpp',
    './src/Main.cpp'
]

include_dirs = [
    './includes',
    './libs/imgui',
    './libs/imgui/backends'
]

lib_names = [
    'glfw3',
    'glew',
    'opengl'
]

deps = []


foreach lib : lib_names
    temp = dependency(lib, required : false)
    if not temp.found()
        temp = cpp_compiler.find_library(lib)
    endif

    deps += temp
endforeach

executable(
    'cellular-automata-generator',
    sources : src_files,
    dependencies : deps,
    include_directories : include_dirs,
    link_with : imgui_lib
)