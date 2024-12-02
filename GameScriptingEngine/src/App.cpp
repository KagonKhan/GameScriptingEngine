#include "App.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <spdlog/spdlog.h>


namespace {
auto fix_monitor_dpi_differences = [windowDPI = 0.0f]() mutable {
    if (windowDPI == 0.0f) {
        windowDPI = ImGui::GetWindowDpiScale();
    }

    if (const float current_dpi = ImGui::GetWindowDpiScale(); current_dpi != windowDPI) {
        spdlog::debug("DPI changed, resizing the window");
        const ImVec2 size = ImGui::GetWindowSize();
        ImGui::SetWindowSize(size + ImVec2{1.f, 1.f});
        ImGui::SetWindowSize(size - ImVec2{1.f, 1.f});
        windowDPI = ImGui::GetWindowDpiScale();
    }
};
} // namespace


void App::start() {
    while (!glfwWindowShouldClose(window) && isRunning) {
        glfwPollEvents();

        window.startFrame();

        render();

        window.endFrame();
    }
}

void App::render() {
    /*ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoInputs;
    ImGui::GetWindowViewport()->Flags &= ~(ImGuiWindowFlags_NoInputs);*/
    static bool demo = false;

    if (ImGui::Begin("GameScriptingEngine", &isRunning)) {
        fix_monitor_dpi_differences();

        ImGui::Checkbox("AutoClicker", &clicker.isVisible);

        if (clicker.isVisible) {
            clicker.render();
        }

        // TODO: a way to achieve overlay is to change  mouse click through on a keybind, or maybe while holding ctrl?
        // This would allow me to draw on the screen easily.

        ImGui::Checkbox("Demo", &demo);
        if (demo)
            ImGui::ShowDemoWindow();


    } else {
        spdlog::critical("Cannot write to main window!");
    }
    ImGui::End();
}