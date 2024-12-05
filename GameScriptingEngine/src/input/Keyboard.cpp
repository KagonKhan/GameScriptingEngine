#include "input/Keyboard.hpp"

#include "input/InputListener.hpp"

#include <Windows.h>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>
#include <unordered_map>


namespace {
inline const std::unordered_map<Keyboard::KEY, std::string> SPECIAL_VK_TO_NAME{
    {Keyboard::KEY::F1, "F1"}, {Keyboard::KEY::F2, "F2"},
    {Keyboard::KEY::F3, "F3"}, {Keyboard::KEY::F4, "F4"},
    {Keyboard::KEY::F5, "F5"}, {Keyboard::KEY::F6, "F6"},
    {Keyboard::KEY::F7, "F7"}, {Keyboard::KEY::F8, "F8"},
    {Keyboard::KEY::F9, "F9"}, {Keyboard::KEY::LEFT_CONTROL, "Left Control"},
};


} // namespace


std::string Keyboard::KeyName(KEY pressed_button) {
    if (SPECIAL_VK_TO_NAME.contains(pressed_button)) {
        return SPECIAL_VK_TO_NAME.at(pressed_button);
    } else {
        return {static_cast<char>(pressed_button)};
    }
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
    }
}