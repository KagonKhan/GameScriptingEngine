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
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',


        // Numpad
        NUMLOCK          = 0x90,
        NUMPAD_0         = 0x60,
        NUMPAD_1         = 0x61,
        NUMPAD_2         = 0x62,
        NUMPAD_3         = 0x63,
        NUMPAD_4         = 0x64,
        NUMPAD_5         = 0x65,
        NUMPAD_6         = 0x66,
        NUMPAD_7         = 0x67,
        NUMPAD_8         = 0x68,
        NUMPAD_9         = 0x69,
        NUMPAD_MULTIPLY  = 0x6A,
        NUMPAD_ADD       = 0x6B,
        NUMPAD_SEPARATOR = 0x6C,
        NUMPAD_SUBTRACT  = 0x6D,
        NUMPAD_DELETE    = 0x6E,
        NUMPAD_DIVIDE    = 0x6F,


        // FUNCTION KEYS
        F1  = 0x70,
        F2  = 0x71,
        F3  = 0x72,
        F4  = 0x73,
        F5  = 0x74,
        F6  = 0x75,
        F7  = 0x76,
        F8  = 0x77,
        F9  = 0x78,
        F10 = 0x79,
        F11 = 0x7A,
        F12 = 0x7B,

        // SPECIAL KEYS
        LEFT_CONTROL  = 0xA2,
        RIGHT_CONTROL = 0xA3,
        LEFT_ALT      = 0xA4,
        RIGHT_ALT     = 0xA5,
        LEFT_SHIFT    = 0xA0,
        RIGHT_SHIT    = 0xA1,

        BACKSPACE = 0x08,
        TAB       = 0x09,
        ENTER     = 0x0D,
        ESCAPE    = 0x1B,

    };

    [[nodiscard]] static bool        AddKeybind(KEY key, std::function<void()>&& action);
    [[nodiscard]] static bool        RemoveKeybind(KEY key);
    [[nodiscard]] static std::string KeyName(KEY pressed_button);
    [[nodiscard]] static bool        send(KEY key);

private:
    static void KeyPressedCallback(int key_code, int action);


private:
    inline static std::unordered_map<KEY, std::function<void()>> keybinds{magic_enum::enum_count<KEY>()};


    // TODO: how to avoid this
    inline static bool initialized = [] { return InputListener::SubscribeKeyPress(KeyPressedCallback); }();
};
