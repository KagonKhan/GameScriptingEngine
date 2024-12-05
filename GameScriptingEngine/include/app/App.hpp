#pragma once
#include "ImGui/imgui.h"
#include "SmoothFPSCounter.hpp"
#include "Window.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/interaction/AutoClicker.hpp"
#include "components/interaction/RectangleMarker.hpp"

// TODO: convert from static class
// instead of using static `flip mode`,
// maybe use event dispatch
class App {
public:
    static void Start();

private:
    static void Render();
    static void templateMatcherWidget();
    static void RenderComponents();
    static void TemporaryRender();

private:
    inline static bool isRunning{true};
    // OVERLAY MODE = INTERACTIONS - CANNOT DRAW. INPUT MODE = CAN DRAW ON THE SCREEN.
    inline static bool   overlayEnabled{true};
    inline static ImRect appArea;


    inline static Window             window;
    inline static AutoClicker        clicker;
    inline static RectangleMarker    areaMarker;
    inline static MonitorPixelReader reader;
    inline static SmoothFPSCounter   fpsCounter{10};
};