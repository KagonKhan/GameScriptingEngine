#pragma once

#include "InputListener.hpp"

#include <ImGui/imgui.h>
#include <Windows.h>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <unordered_map>

class Mouse {
private:
    static constexpr char const* const TAG{"[Mouse]"};

public:
    enum class Button : std::uint8_t {
        LEFT   = VK_LBUTTON,
        RIGHT  = VK_RBUTTON,
        MIDDLE = VK_MBUTTON,
    };
    enum class ScrollDirection : std::uint8_t {
        UP,
        DOWN,
    };

    [[nodiscard]] static ImVec2 GetPosition();
    [[nodiscard]] static bool   SetPosition(ImVec2 position);

    [[nodiscard]] static bool IsButtonPressed(Button button);

    [[nodiscard]] static bool Click();
    [[nodiscard]] static bool Click(const ImVec2 mouse_position);
    [[nodiscard]] static bool Scroll(ScrollDirection direction);

private:
    static void MouseMoveCallback(ImVec2 position);
};
