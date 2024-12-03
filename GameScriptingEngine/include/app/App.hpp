#pragma once
#include "ImGui/imgui.h"
#include "Window.hpp"
#include "components/AutoClicker.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/RectangleMarker.hpp"

#include "SmoothFPSCounter.hpp"


class App {
public:
    static void Start();
    static bool IsOverlayMode() { return overlayEnabled; }
    static void FlipMode() { overlayEnabled = !overlayEnabled; }

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
    inline static SmoothFPSCounter fpsCounter{10};
};