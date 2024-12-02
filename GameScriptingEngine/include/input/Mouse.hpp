#pragma once
#include <ImGui/imgui.h>
#include <Windows.h>

namespace Input {
class Mouse {
private:
    inline static constexpr char const* const TAG{"[Mouse]"};

public:
    enum class Button : short int {
        LEFT   = VK_LBUTTON,
        RIGHT  = VK_RBUTTON,
        MIDDLE = VK_MBUTTON,
    };

    static ImVec2 GetPosition();

    static bool IsButtonPressed(Button button);

    static bool Click();
    static bool Click(ImVec2 mousePosition);
};

} // namespace Input