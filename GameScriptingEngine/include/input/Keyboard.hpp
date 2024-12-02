#pragma once
#include "magic_enum/magic_enum.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <input/InputListener.hpp>

namespace Input {
class Keyboard {
public:
    enum class Key : short int {
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
    };

    static std::string KeyName(Key pressedButton);
    static bool        AddKeybind(Key key, std::function<void()>&& action);
    static bool        RemoveKeybind(Key key);

private:
    static void KeyPressedCallback(int keyCode, int action);

private:
    inline static std::unordered_map<Key, std::function<void()>> keybinds{magic_enum::enum_count<Key>()};

    // TODO: clunky af, what cleaner way can i auto-initialize this?
    inline static bool subscribed = []{
        static InputListener input_listener; 
        input_listener.SubscribeKeyPress(KeyPressedCallback);
        return true;
    }();
};
} // namespace Input