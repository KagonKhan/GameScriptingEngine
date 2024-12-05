#include "input/InputListener.hpp"

#include <Windows.h>
#include <functional>
#include <spdlog/spdlog.h>


InputListener::InputListener() {
    spdlog::debug("{} Constructor", TAG);
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) {
        spdlog::critical("Could not get instance handle!");
        exit(1);
    }
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
    if (WITH_MOUSE_HOOK)
        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

    if (!keyboardHook || (WITH_MOUSE_HOOK && !mouseHook)) {
        spdlog::critical("Failed to set hooks");
        exit(1);
    }
}

InputListener::~InputListener() {
    spdlog::debug("{} Destructor", TAG);
    UnhookWindowsHookEx(keyboardHook);
    if (WITH_MOUSE_HOOK)
        UnhookWindowsHookEx(mouseHook);
}

bool InputListener::SubscribeKeyPress(std::function<void(int keyCode, int action)> subscriber) {
    keyboardSubscriber = std::move(subscriber);
    return true;
}

#if WITH_MOUSE_HOOK
bool InputListener::SubscribeMouseMove(std::function<void(ImVec2)> subscriber) {
    spdlog::critical("Adding mouse subscriber");
    mouseSubscriber = std::move(subscriber);
    return true;
}
#endif

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


LRESULT CALLBACK InputListener::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    spdlog::critical("callback");
    if (mouseHook) {
        if (nCode == HC_ACTION) {
            const auto mouse_data  = reinterpret_cast<MOUSEHOOKSTRUCT*>(lParam);
            const auto mouse_point = mouse_data->pt;

            mouseSubscriber(ImVec2{static_cast<float>(mouse_point.x), static_cast<float>(mouse_point.y)});
        }
    } else {
        spdlog::warn("{} No input listeners!", TAG);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
