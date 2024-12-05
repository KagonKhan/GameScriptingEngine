
# =============================    ImGui    =============================
add_library(ImGui
    "ImGui/imgui.cpp"
    "ImGui/imgui_demo.cpp"
    "ImGui/imgui_draw.cpp"
    "ImGui/imgui_impl_glfw.cpp"
    "ImGui/imgui_impl_opengl3.cpp"
    "ImGui/imgui_tables.cpp"
    "ImGui/imgui_widgets.cpp"
    "ImGui/imgui_stdlib.cpp"
    
    "ImGui/implot.cpp"
    "ImGui/implot_demo.cpp"
    "ImGui/implot_items.cpp"
)



find_package(GLFW3 REQUIRED)
target_link_libraries(ImGui glfw)

target_include_directories(ImGui
    PUBLIC ../include/
    PRIVATE ../include/ImGui
)


# =============================    GLAD    =============================

add_library(GLAD
    GLAD/GLAD.c
)

target_include_directories(GLAD
    PUBLIC ../include/
    PRIVATE ../include/GLAD
)



# =============================    EventBus    =============================

add_library(EventBus
    "EventBus/EventBus.cpp"
    "EventBus/PassPerk.cpp"
    "EventBus/PerkEventBus.cpp"
    "EventBus/TagPerk.cpp"
    "EventBus/WaitPerk.cpp"
)

target_include_directories(EventBus
    PUBLIC ../include/Listener.hpp
    PUBLIC ../include/EventBus.hpp
    PUBLIC ../include/Bus.hpp

    PRIVATE ../include/EventBus
    PRIVATE ../include/EventBus/dexode
)