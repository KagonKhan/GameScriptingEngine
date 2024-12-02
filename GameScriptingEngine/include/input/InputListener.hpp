#pragma once
#include <Windows.h>
#include <functional>

// TODO: this does not really have to be a class - maybe something else.
class InputListener {
private:
    inline static constexpr char const* const TAG{"[InputListener]"};

public:
    InputListener();
    ~InputListener();

    static void SubscribeKeyPress(std::function<void(int keyCode, int action)> subscriber);

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    inline static HHOOK                                        keyboardHook;
    inline static HHOOK                                        mouseHook;
    inline static std::function<void(int keyCode, int action)> keyboardSubscriber;
};
