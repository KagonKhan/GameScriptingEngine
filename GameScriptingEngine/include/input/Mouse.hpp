#pragma once
#include <ImGui/imgui.h>
#include <Windows.h>
#include <cstdint>

namespace Input {
class Mouse {
private:
    static constexpr char const* const TAG{"[Mouse]"};

public:
    enum class BUTTON : std::uint8_t {
        LEFT   = VK_LBUTTON,
        RIGHT  = VK_RBUTTON,
        MIDDLE = VK_MBUTTON,
    };

    static ImVec2 GetPosition();

    static bool IsButtonPressed(BUTTON button);

    static bool Click();
    static bool Click(const ImVec2 mouse_position);
};

} // namespace Input