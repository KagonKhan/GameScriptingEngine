#include "input/InputListener.hpp"

#include <Windows.h>
#include <functional>
#include <spdlog/spdlog.h>


InputListener::InputListener() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) {
        spdlog::critical("Could not get instance handle!");
        exit(1);
    }
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);

    if (!keyboardHook) {
        spdlog::critical("Failed to set hooks");
        exit(1);
    }
}

InputListener::~InputListener() {
    UnhookWindowsHookEx(keyboardHook);
}

void InputListener::SubscribeKeyPress(std::function<void(int keyCode, int action)> subscriber) {
    keyboardSubscriber = std::move(subscriber);
}


LRESULT CALLBACK InputListener::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        const auto keyboardData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // TODO: maybe skip callbacks for key held - repeats?
        if (keyboardSubscriber)
            keyboardSubscriber(static_cast<int>(keyboardData->vkCode), wParam);
        else
            spdlog::warn("{} No input listeners!", TAG);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
