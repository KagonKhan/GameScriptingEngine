#pragma once
#include "ImGui/imgui.h"
#include "SmoothFPSCounter.hpp"
#include "Window.hpp"
#include "components/AutoClicker.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/RectangleMarker.hpp"


class App {
public:
    static void Start();
    // OVERLAY MODE = INTERACTIONS - CANNOT DRAW. INPUT MODE = CAN DRAW ON THE SCREEN.
    static bool IsOverlayMode() { return overlayEnabled; }
    static void FlipMode() { overlayEnabled = !overlayEnabled; }

    static void DrawRect(ImVec2 min, ImVec2 max) {
        mmin = min;
        mmax = max;
        drawRect = true;
    }

private:
    static void Render();
    static void RenderComponents();
    static void TemporaryRender();

private:
    inline static bool isRunning{true};
    inline static bool overlayEnabled{false};


    inline static Window             window;
    inline static AutoClicker        clicker;
    inline static RectangleMarker    areaMarker;
    inline static MonitorPixelReader reader;
    inline static SmoothFPSCounter   fpsCounter{10};

    inline static ImVec2 mmin, mmax;
    inline static bool   drawRect{false};
};