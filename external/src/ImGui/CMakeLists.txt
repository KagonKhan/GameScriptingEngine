add_library(ImGui
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)



find_package(GLFW3 REQUIRED)
target_link_libraries(ImGui glfw)
target_include_directories(ImGui
 PUBLIC ../../include/
 PRIVATE ../../include/ImGui
 )