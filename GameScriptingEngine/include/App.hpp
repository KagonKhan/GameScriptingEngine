#pragma once
#include "AutoClicker.hpp"
#include "Window.hpp"


class App {
public:
    void start();

private:
    void render();

private:
    bool        isRunning{true};

    Window      window;
    AutoClicker clicker;

    bool enableOverlay{false};
};