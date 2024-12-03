#pragma once
#include "components/AutoClicker.hpp"
#include "Window.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/RectangleMarker.hpp"
#include "ImGui/imgui.h"


class App {
public:
    static void Start();
    static bool IsOverlayMode() { return overlayEnabled; }
    static void FlipMode() { overlayEnabled = !overlayEnabled; }

private:
    static void Render();
    static void TemporaryRender();

private:
    inline static bool isRunning{true};
    inline static bool overlayEnabled{false};

    inline static Window      window;
    inline static AutoClicker clicker;
    inline static RectangleMarker           areaMarker;
    inline static MonitorPixelReader reader;
};