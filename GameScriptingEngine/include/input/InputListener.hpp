#pragma once
#include "Utils.hpp"
#include "ImGui/imgui.h"

#include <Windows.h>
#include <functional>

// TODO: this does not really have to be a class - maybe something else.
class InputListener : NonMovable {
private:
    inline static constexpr char const* const TAG{"[InputListener]"};

#define WITH_MOUSE_HOOK false

public:
    static void Initialize();
    static void Release();

    static bool SubscribeKeyPress(std::function<void(int keyCode, int action)> subscriber);
#if WITH_MOUSE_HOOK
    static bool SubscribeMouseMove(std::function<void(ImVec2)> subscriber);
#endif

    // TODO: move to enums for encapsulation
    enum class KeyAction : std::uint16_t {
        DOWN = WM_KEYDOWN,
        UP   = WM_KEYUP,
    };

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    inline static HHOOK                                        keyboardHook;
    inline static HHOOK                                        mouseHook;
    inline static std::function<void(ImVec2)>                  mouseSubscriber;
    inline static std::function<void(int keyCode, int action)> keyboardSubscriber;
};
