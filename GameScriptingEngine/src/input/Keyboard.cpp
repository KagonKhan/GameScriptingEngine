#include "input/Keyboard.hpp"

#include <Windows.h>
#include <functional>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>
#include <unordered_map>


namespace {
namespace EVENTS {} // namespace EVENTS

namespace INPUTS {

INPUT* MakeKeyEvent(Keyboard::KEY key) {
    static INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type   = INPUT_KEYBOARD;
    inputs[0].ki.wVk = static_cast<unsigned short>(key);

    inputs[1].type       = INPUT_KEYBOARD;
    inputs[1].ki.wVk     = static_cast<unsigned short>(key);
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    return inputs;
}


} // namespace INPUTS
} // namespace


namespace {
inline const std::unordered_map<Keyboard::KEY, std::string> SPECIAL_VK_TO_NAME{
    {Keyboard::KEY::F1, "F1"},
    {Keyboard::KEY::F2, "F2"},
    {Keyboard::KEY::F3, "F3"},
    {Keyboard::KEY::F4, "F4"},
    {Keyboard::KEY::F5, "F5"},
    {Keyboard::KEY::F6, "F6"},
    {Keyboard::KEY::F7, "F7"},
    {Keyboard::KEY::F8, "F8"},
    {Keyboard::KEY::F9, "F9"},

    {Keyboard::KEY::NUMPAD_0, "Numpad 0"},
    {Keyboard::KEY::NUMPAD_1, "Numpad 1"},
    {Keyboard::KEY::NUMPAD_2, "Numpad 2"},
    {Keyboard::KEY::NUMPAD_3, "Numpad 3"},
    {Keyboard::KEY::NUMPAD_4, "Numpad 4"},
    {Keyboard::KEY::NUMPAD_5, "Numpad 5"},
    {Keyboard::KEY::NUMPAD_6, "Numpad 6"},
    {Keyboard::KEY::NUMPAD_7, "Numpad 7"},
    {Keyboard::KEY::NUMPAD_8, "Numpad 8"},
    {Keyboard::KEY::NUMPAD_9, "Numpad 9"},

    {Keyboard::KEY::NUMPAD_MULTIPLY, "Numpad Multiply"},
    {Keyboard::KEY::NUMPAD_ADD, "Numpad Add"},
    {Keyboard::KEY::NUMPAD_SEPARATOR, "Numpad Separator"},
    {Keyboard::KEY::NUMPAD_SUBTRACT, "Numpad Subtract"},
    {Keyboard::KEY::NUMPAD_DELETE, "Numpad Delete"},
    {Keyboard::KEY::NUMPAD_DIVIDE, "Numpad Divide"},


    {Keyboard::KEY::LEFT_CONTROL, "Left Control"},
    {Keyboard::KEY::RIGHT_CONTROL, "Right Control"},
    {Keyboard::KEY::LEFT_ALT, "Left Alt"},
    {Keyboard::KEY::RIGHT_ALT, "Right Alt"},
    {Keyboard::KEY::LEFT_SHIFT, "Left Shift"},
    {Keyboard::KEY::RIGHT_SHIT, "Right Shift"},


    {Keyboard::KEY::BACKSPACE, "Backspace"},
    {Keyboard::KEY::TAB, "Tab"},
    {Keyboard::KEY::ENTER, "Enter"},

};


} // namespace


std::string Keyboard::KeyName(KEY pressed_button) {
    if (SPECIAL_VK_TO_NAME.contains(pressed_button)) {
        return SPECIAL_VK_TO_NAME.at(pressed_button);
    } else {
        return {static_cast<char>(pressed_button)};
    }
}

bool Keyboard::send(KEY key) {
    spdlog::debug("{} Sending key: {}", TAG, KeyName(key));

    const UINT sent = SendInput(2, INPUTS::MakeKeyEvent(key), sizeof(INPUT));

    if (sent != 2) {
        spdlog::critical("{} Failed to send!", TAG);
    }

    return sent == 2;
}


bool Keyboard::AddKeybind(KEY key, std::function<void()>&& action) {
    if (keybinds.contains(key)) {
        return false;
    }
    keybinds.emplace(key, std::move(action));
    return true;
}

bool Keyboard::RemoveKeybind(const KEY key) {
    if (!keybinds.contains(key))
        return false;

    keybinds.erase(key);
    return true;
}

void Keyboard::KeyPressedCallback(const int key_code, const int action) {
    const auto        pressed_button = static_cast<Keyboard::KEY>(key_code);
    const std::string button_name    = Keyboard::KeyName(pressed_button);

    if (keybinds.contains(pressed_button) && action == WM_KEYUP) {
        spdlog::debug("Callback for button {} called", button_name);
        std::invoke(keybinds[pressed_button]);
    } else if (action == WM_KEYUP) {
        spdlog::info("{} Received key: {}, without keybinds", TAG, button_name);
    }
}