#include "input/Keyboard.hpp"

#include <Windows.h>
#include <functional>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>
#include <unordered_map>

using namespace Input;

namespace {
inline static const std::unordered_map<Keyboard::Key, std::string> specialVKtoName{
    {Keyboard::Key::F1, "F1"}, {Keyboard::Key::F2, "F2"}, {Keyboard::Key::F3, "F3"},
    {Keyboard::Key::F4, "F4"}, {Keyboard::Key::F5, "F5"}, {Keyboard::Key::F6, "F6"},
    {Keyboard::Key::F7, "F7"}, {Keyboard::Key::F8, "F8"}, {Keyboard::Key::F9, "F9"},
};
} // namespace

std::string Keyboard::KeyName(Key pressedButton) {
    if (specialVKtoName.contains(pressedButton)) {
        return specialVKtoName.at(pressedButton);
    } else {
        return {static_cast<char>(pressedButton)};
    }
}

bool Keyboard::AddKeybind(Key key, std::function<void()>&& action) {
    if (keybinds.contains(key))
        return false;

    keybinds.emplace(key, std::move(action));
    return true;
}

bool Keyboard::RemoveKeybind(Key key) {
    if (!keybinds.contains(key))
        return false;

    keybinds.erase(key);
    return true;
}

void Keyboard::KeyPressedCallback(int keyCode, int action) {
    const Key         pressed_button = static_cast<Key>(keyCode);
    const std::string button_name    = KeyName(pressed_button);

    if (keybinds.contains(pressed_button) && action == WM_KEYUP) {
        spdlog::debug("Callback for button {} called", button_name);
        std::invoke(keybinds[pressed_button]);
    }
}