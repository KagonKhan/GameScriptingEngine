#include "App.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <spdlog/spdlog.h>


namespace {
auto fixMonitorDpiDifferences = [windowDPI = 0.0f]() mutable {
    if (windowDPI == 0.0f) {
        windowDPI = ImGui::GetWindowDpiScale();
    }

    if (float current_dpi = ImGui::GetWindowDpiScale(); current_dpi != windowDPI) {
        spdlog::debug("DPI changed, resizing the window");
        auto size = ImGui::GetWindowSize();
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
    static bool demo = false;
    if (ImGui::Begin("GameScriptingEngine", &isRunning)) {
        fixMonitorDpiDifferences();

        ImGui::Checkbox("AutoClicker", &clicker.isVisible);

        if (clicker.isVisible) {
            clicker.render();
        }

        ImGui::Checkbox("Demo", &demo);
        if (demo)
            ImGui::ShowDemoWindow();

    } else {
        spdlog::critical("Cannot write to main window!");
    }
    ImGui::End();
}