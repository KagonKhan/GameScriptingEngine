#pragma once
#include "Utils.hpp"
#include "input/Keyboard.hpp"

#include <atomic>
#include <thread>


// Should this have multiple instances?
class AutoClicker {
private:
    inline static constexpr char const* const TAG{"[AutoClicker]"};

public:
    AutoClicker();
    ~AutoClicker();

    void render();
    void keybindWidget();
    void intervalWidget();

public:
    bool isVisible = true;

private:
    // CV to pause the thread instead of stopping it?
    void startClicking();
    void stopClicking();

private:
    std::thread          clicker;
    std::atomic<bool>    isRunning;
    Input::Keyboard::Key keybind{Input::Keyboard::Key::Q};
    Interval             interval;
};
