#pragma once
#include "magic_enum/magic_enum.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <input/InputListener.hpp>

namespace Input {
class Keyboard {
public:
    enum class KEY : std::uint8_t {
        W = 'W',
        A = 'A',
        S = 'S',
        D = 'D',
        E = 'E',
        Q = 'Q',

        // Special Keys
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,

        LEFT_CONTROL = 0xA2,
    };

    static std::string KeyName(KEY pressed_button);
    static bool        AddKeybind(KEY key, std::function<void()>&& action);
    static bool        RemoveKeybind(KEY key);

private:
    static void KeyPressedCallback(int key_code, int action);

private:
    inline static std::unordered_map<KEY, std::function<void()>> keybinds{magic_enum::enum_count<KEY>()};

    // TODO: clunky af, what cleaner way can I auto-initialize this?
    inline static bool subscribed = []{
        static InputListener input_listener;
        InputListener::SubscribeKeyPress(KeyPressedCallback);
        return true;
    }();
};
} // namespace Input