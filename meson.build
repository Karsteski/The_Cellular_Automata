project('Cellular-Automata-Generator', 'cpp', default_options : 'cpp_std=c++17')

# Internal libraries
subdir('src/imgui')

src_files = [
    'src/Application.cpp',
    'src/Elementary.cpp',
    'src/GameOfLife.cpp',
    'src/Grid.cpp'
]

include_dirs = [
    'src',
    'src/imgui',
    'src/imgui/backends'
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

executable('Cellular-Automata-Generator',
    sources : src_files,
    dependencies : deps,
    include_directories : include_dirs,
    link_with : [imgui_lib]
)