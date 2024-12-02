#include "input/Mouse.hpp"

#include <Windows.h>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>
using namespace Input;

namespace {
namespace EVENTS {
const MOUSEINPUT LMB_DOWN{.dwFlags = MOUSEEVENTF_LEFTDOWN};
const MOUSEINPUT LMB_UP{.dwFlags = MOUSEEVENTF_LEFTUP};

const MOUSEINPUT RMB_DOWN{.dwFlags = MOUSEEVENTF_RIGHTDOWN};
const MOUSEINPUT RMB_UP{.dwFlags = MOUSEEVENTF_RIGHTUP};
} // namespace EVENTS

namespace INPUTS {
const INPUT LMB_PRESS    = {.type = INPUT_MOUSE, .mi = EVENTS::LMB_DOWN};
const INPUT LMB_RELEASE  = {.type = INPUT_MOUSE, .mi = EVENTS::LMB_UP};
INPUT       LMB_CLICK[2] = {LMB_PRESS, LMB_RELEASE};

// will it break do to re-use of the structures?
const INPUT RMB_PRESS    = {.type = INPUT_MOUSE, .mi = EVENTS::RMB_DOWN};
const INPUT RMB_RELEASE  = {.type = INPUT_MOUSE, .mi = EVENTS::RMB_UP};
INPUT       RMB_CLICK[2] = {RMB_PRESS, RMB_RELEASE};
} // namespace INPUTS


inline std::unordered_map<Mouse::BUTTON, bool> WAS_BUTTON_PRESSED{
    {Mouse::BUTTON::LEFT, false},
    {Mouse::BUTTON::RIGHT, false},
    {Mouse::BUTTON::MIDDLE, false},
};
} // namespace


ImVec2 Mouse::GetPosition() {
    POINT point;
    GetCursorPos(&point);
    return {static_cast<float>(point.x), static_cast<float>(point.y)};
}

bool Mouse::IsButtonPressed(const BUTTON button) {
    const bool is_on  = GetAsyncKeyState(magic_enum::enum_integer(button)) & 0x8000;
    const bool was_on = std::exchange(WAS_BUTTON_PRESSED[button], is_on);

    return is_on && !was_on;
}

bool Mouse::Click() {
    spdlog::debug("{} Clicking mouse", TAG);

    UINT uSent = SendInput(2, INPUTS::LMB_CLICK, sizeof(INPUT));

    if (uSent != 2) {
        spdlog::critical("{} Failed to click the mouse!", TAG);
    }

    return uSent != 2;
}

bool Mouse::Click(const ImVec2 mouse_position) { return false; }
