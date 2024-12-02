#include "App.hpp"

#include "input/Mouse.hpp"

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

void App::Start() {
    Input::Keyboard::AddKeybind(Input::Keyboard::KEY::LEFT_CONTROL, [&] { enableOverlay = !enableOverlay; });


    while (!glfwWindowShouldClose(window) && isRunning) {
        glfwPollEvents();

        if (enableOverlay) {
            ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoInputs;
        }
        fix_monitor_dpi_differences();

        window.startFrame();

        Render();

        window.endFrame();
    }

    Input::Keyboard::RemoveKeybind(Input::Keyboard::KEY::Q);
}

void App::Render() {
    static bool demo = false;

    ImGui::Begin("GameScriptingEngine", &isRunning);

    ImGui::Separator();
    areaMarker.render();
    ImGui::Separator();

    ImGui::Checkbox("AutoClicker", &clicker.isVisible);

    if (clicker.isVisible) {
        clicker.render();
    }


    ImGui::Separator();
    ImGui::Checkbox("Demo", &demo);
    if (demo)
        ImGui::ShowDemoWindow();


    ImGui::End();
}