#include "input/Mouse.hpp"

#include "input/SendInputs.hpp"

#include <Windows.h>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace {


} // namespace

ImVec2 Mouse::GetPosition() {
    POINT point;
    GetCursorPos(&point);
    return {static_cast<float>(point.x), static_cast<float>(point.y)};
}

bool Mouse::SetPosition(ImVec2 position) { return SetCursorPos(position.x, position.y); }

bool Mouse::IsButtonPressed(const Button button) {
    static std::unordered_map<Mouse::Button, bool> WAS_BUTTON_PRESSED{
        {Button::LEFT, false},
        {Button::RIGHT, false},
        {Button::MIDDLE, false},
    };

    const bool is_on  = GetAsyncKeyState(magic_enum::enum_integer(button)) & 0x8000;
    const bool was_on = std::exchange(WAS_BUTTON_PRESSED[button], is_on);

    return is_on && !was_on;
}

bool Mouse::Click() {
    spdlog::debug("{} Clicking mouse", TAG);

    const UINT sent = SendInput(2, INPUTS::LMB_CLICK().data, sizeof(INPUT));

    if (sent != 2) {
        spdlog::critical("{} Failed to click the mouse!", TAG);
    }

    return sent == 2;
}

bool Mouse::Click(const ImVec2 mouse_position) {
    auto current_mouse_position = GetPosition();
    bool success{true};
    success &= SetPosition(mouse_position);
    success &= Click();
    //success &= SetPosition(current_mouse_position);

    return success;
}

bool Mouse::Scroll(ScrollDirection direction) {
    spdlog::debug("{} Scrolling mouse", TAG);

    INPUT      input = (direction == ScrollDirection::DOWN) ? INPUTS::SCROLL_DOWN().data : INPUTS::SCROLL_UP().data;
    const UINT sent  = SendInput(1, &input, sizeof(INPUT));

    if (sent != 1) {
        spdlog::critical("{} Failed to scroll the mouse!", TAG);
    }

    return sent == 1;
}

void Mouse::MouseMoveCallback(ImVec2 position) { spdlog::critical("Mouse position ({}, {})", position.x, position.y); }
