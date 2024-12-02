#pragma once
#include "AutoClicker.hpp"
#include "Window.hpp"
#include "components/RectangleMarker.hpp"
#include "ImGui/imgui.h"


class App {
public:
    static void Start();
    static bool IsOverlayMode() { return enableOverlay; }
    static void FlipMode() { enableOverlay = !enableOverlay; }

private:
    static void Render();

private:
    inline static bool isRunning{true};
    inline static bool enableOverlay{false};

    inline static Window      window;
    inline static AutoClicker clicker;
    inline static RectangleMarker           areaMarker;
};