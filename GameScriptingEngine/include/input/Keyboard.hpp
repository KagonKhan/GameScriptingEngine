#pragma once

#include "InputListener.hpp"

#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <string>

class Keyboard {
private:
    inline static constexpr char const* const TAG{"[Keyboard]"};

public:
    // TODO: a way to retrieve unused keybinds, and a descriptor where a keybind is assigned?
    // A screen / window for all keybinds? Like in games?
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

    [[nodiscard]] static bool        AddKeybind(KEY key, std::function<void()>&& action);
    [[nodiscard]] static bool        RemoveKeybind(KEY key);
    [[nodiscard]] static std::string KeyName(KEY pressed_button);

private:
    static void KeyPressedCallback(const int key_code, const int action);

private:
    inline static std::unordered_map<KEY, std::function<void()>> keybinds{magic_enum::enum_count<KEY>()};

    // TODO: how to avoid this
    inline static bool subscribed = [] {
        return InputListener::SubscribeKeyPress(KeyPressedCallback);
    }();
};
