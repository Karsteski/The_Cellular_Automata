imgui_sources = [
    './imgui_demo.cpp',
    './imgui_draw.cpp',
    './imgui_tables.cpp',
    './imgui_widgets.cpp',
    './imgui.cpp'
]

imgui_backends = ['backends/imgui_impl_glfw.cpp', 'backends/imgui_impl_opengl3.cpp']

imgui_includes = [
    '.',
    'backends'
]

imgui_lib = static_library('imgui',
    sources : [imgui_sources, imgui_backends],
    include_directories : imgui_includes
)