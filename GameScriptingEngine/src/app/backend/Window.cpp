#include "app/backend/Window.hpp"

#include "ImGui/imgui.h"

#include <spdlog/spdlog.h>

void Window::fixScaling() {
    if (windowDPI == 0.0f) {
        windowDPI = ImGui::GetWindowDpiScale();
    }

    if (const float current_dpi = ImGui::GetWindowDpiScale(); current_dpi != windowDPI) {
        spdlog::debug("DPI changed, resizing the window");

        const ImVec2 size = ImGui::GetWindowSize();
        ImGui::SetWindowSize(size + ImVec2{1.f, 1.f});
        ImGui::SetWindowSize(size - ImVec2{1.f, 1.f});

        windowDPI = current_dpi;
    }
}