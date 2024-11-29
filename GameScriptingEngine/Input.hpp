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
        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

        if (!keyboardHook || !mouseHook) {
            spdlog::critical("Failed to set hooks");
            exit(1);
        }

        run = true;

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
        run = false;
        PostQuitMessage(0);
        spdlog::info("joining poller");
        poller.join();
        spdlog::info("poller joined");

        UnhookWindowsHookEx(keyboardHook);
        UnhookWindowsHookEx(mouseHook);

        spdlog::info("WinApiInputParser destructor");
    }

private:
    HHOOK keyboardHook;
    HHOOK mouseHook;
    bool run{ false };
    std::thread poller;

    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            auto* keyboardData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            switch (wParam) {
            case WM_KEYDOWN:
                spdlog::warn("Key pressed: {}", (char)keyboardData->vkCode);
                break;
            case WM_KEYUP:
                spdlog::warn("Key released: {}", (char)keyboardData->vkCode);
                break;
            }
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
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
    }

};


class Input {
public:


    class Mouse {
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

        
    };


    class Keyboard {
    public:
        enum class Key : short int {
            W = 'W',
            A = 'A',
            S = 'S',
            D = 'D',
            E = 'E',
            Q = 'Q',
        };

        inline static std::unordered_map <Key, std::function<void()>> keybinds;

        static void parseCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            spdlog::info("Callback called");
            Key pressed_button = static_cast<Key>(key);

            if (keybinds.contains(pressed_button) && action == GLFW_PRESS) {
                std::invoke(keybinds[pressed_button]);
            }

        }


    private:
        inline static WinApiInputParser parser;

    };

private:
};
