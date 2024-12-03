
# =============================    ImGui    =============================
add_library(ImGui
    src/ImGui/imgui.cpp
    src/ImGui/imgui_demo.cpp
    src/ImGui/imgui_draw.cpp
    src/ImGui/imgui_impl_glfw.cpp
    src/ImGui/imgui_impl_opengl3.cpp
    src/ImGui/imgui_tables.cpp
    src/ImGui/imgui_widgets.cpp
)



find_package(GLFW3 REQUIRED)
target_link_libraries(ImGui glfw)
target_include_directories(ImGui
    PUBLIC ../../include/
    PRIVATE ../../include/ImGui
)


# =============================    GLAD    =============================

add_library(GLAD
    src/GLAD/GLAD.c
)

target_include_directories(GLAD
    PUBLIC ../../include/
    PRIVATE ../../include/GLAD
)