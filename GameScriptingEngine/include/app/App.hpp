#pragma once
#include "ImGui/imgui.h"
#include "SmoothFPSCounter.hpp"
#include "Window.hpp"
#include "components/interaction/AutoClicker.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/interaction/RectangleMarker.hpp"


class App {
public:
    static void Start();
    // OVERLAY MODE = INTERACTIONS - CANNOT DRAW. INPUT MODE = CAN DRAW ON THE SCREEN.
    static bool IsOverlayMode() { return overlayEnabled; }
    static void FlipMode() { overlayEnabled = !overlayEnabled; }

private:
    static void Render();
    static void templateMatcherWidget();
    static void RenderComponents();
    static void TemporaryRender();

private:
    inline static bool isRunning{true};
    inline static bool overlayEnabled{true};


    inline static Window             window;
    inline static AutoClicker        clicker;
    inline static RectangleMarker    areaMarker;
    inline static MonitorPixelReader reader;
    inline static SmoothFPSCounter   fpsCounter{10};

};