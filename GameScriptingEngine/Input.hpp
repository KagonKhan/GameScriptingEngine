#pragma once

#include <functional>

#include <Windows.h>
#include <WinUser.h>

#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_switch.hpp>
#include <spdlog/spdlog.h>

#include <thread>


class WinApiInputParser {
public:
    WinApiInputParser() {
        spdlog::info("{} constructor...", "WinApiInputParser");
        HINSTANCE hInstance = GetModuleHandle(NULL);
        if (!hInstance) {
            spdlog::critical("Could not get instance handle!");
            exit(1);
        }
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
        //mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

        if (!keyboardHook) {
            spdlog::critical("Failed to set hooks");
            exit(1);
        }


            //MSG msg;
            //spdlog::debug("before loop");
            //while (GetMessage(&msg, NULL, 0, 0) !=0 && run) {

            //    spdlog::debug("inside loop");
            //    if (msg.message == WM_QUIT) {
            //        break;
            //    }
            //    TranslateMessage(&msg);
            //    DispatchMessage(&msg); 

            //}


        spdlog::info("WinApiInputParser constructor");
        
    }

    ~WinApiInputParser() {
        spdlog::info("WinApiInputParser destructor...");
        PostQuitMessage(0);

        UnhookWindowsHookEx(keyboardHook);
        UnhookWindowsHookEx(mouseHook);

        spdlog::info("WinApiInputParser destructor");
    }

    void subscribeKeyPress(std::function<void(int keyCode, int action)> subscriber) {
        keyboardSubscriber = std::move(subscriber);
    }

private:
    HHOOK keyboardHook;
    HHOOK mouseHook;
    inline static std::function<void(int keyCode, int action)> keyboardSubscriber;

    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            auto keyboardData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            
            // TODO: maybe skip callbacks for key held - repeats?
            keyboardSubscriber(static_cast<int>(keyboardData->vkCode), wParam);
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    /*static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            auto* mouseData = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
            switch (wParam) {
            case WM_MOUSEMOVE:

                spdlog::warn("Mouse moved: {}, {}", mouseData->pt.x, mouseData->pt.y);
                break;
            case WM_LBUTTONDOWN:
                spdlog::warn("LMB pressed");
                break;
            case WM_LBUTTONUP:
                spdlog::warn("LMB released");
                break;
            }
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }*/

};


class Input {
public:


    inline static class Mouse {
    public:
        enum class Button : short int {
            LEFT = VK_LBUTTON,
            RIGHT = VK_RBUTTON,
            MIDDLE = VK_MBUTTON,
        };

        static ImVec2 GetPosition() {
            POINT point;
            GetCursorPos(&point);
            return { (float)point.x, (float)point.y};
        }

        static bool IsButtonPressed(Button button) {
            // TODO: this does not handle if MB left and right is swapped.
            return GetAsyncKeyState(magic_enum::enum_integer(button)) & 0x8000;
        }

        static bool Click() {
            INPUT inputs[2] = {};
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;


            UINT uSent = SendInput(2, inputs, sizeof(INPUT));

            if (uSent != 2) {
                spdlog::critical("Failed to click the mouse!");
            }

            return uSent != 2;
        }
        static bool Click(ImVec2 mousePosition) {
            return false;
        }
    } mouse;


    inline static class Keyboard {
    public:
        Keyboard() {
            parser.subscribeKeyPress(parseCallback);
        }
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

        static std::string keyName(Key pressedButton) {
            if (specialVKtoName.contains(pressedButton)) {
                return specialVKtoName.at(pressedButton);
            }
            else {
                return { static_cast<char>(pressedButton) };
            }

        }

        inline static std::unordered_map <Key, std::function<void()>> keybinds;

    private:
        inline static WinApiInputParser parser;

        inline static const std::unordered_map<Key, std::string> specialVKtoName{
            {Key::F1, "F1"}, {Key::F2, "F2"}, {Key::F3, "F3"}, {Key::F4, "F4"},
            {Key::F5, "F5"}, {Key::F6, "F6"}, {Key::F7, "F7"}, {Key::F8, "F8"},
            {Key::F9, "F9"},
        };


        static void parseCallback(int keyCode, int action) {
            const Key pressed_button = static_cast<Key>(keyCode);

            const std::string button_name = keyName(pressed_button);
            

            spdlog::debug("Received key: {}", button_name);
            if (keybinds.contains(pressed_button) && action == WM_KEYUP) {
                spdlog::info("Callback for button {} called", button_name);
                std::invoke(keybinds[pressed_button]);
            }
        }
    } keyboard;



private:
};
