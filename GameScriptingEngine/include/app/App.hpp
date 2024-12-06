#pragma once
#include "Window.hpp"
#include "components/AppMode.hpp"
#include "components/MonitorPixelReader.hpp"
#include "components/SmoothedFPSCounter.hpp"
#include "components/interaction/AutoClicker.hpp"
#include "components/interaction/RectangleMarker.hpp"

// TODO: convert from static class
// instead of using static `flip mode`,
// maybe use event dispatch
class App {
private:
    static constexpr char const* const TAG{"[App]"};

public:
    static AppMode::State GetMode() { return appMode.get(); }
    static void           Start();

private:
    static void Render();
    static void templateMatcherWidget();
    static void RenderComponents();
    static void TemporaryRender();

private:
    inline static bool          isRunning{true};
    inline static bool          visible{true};
    const inline static AppMode appMode{};
    inline static ImRect        appArea;
    inline static EventListener eventListener;


    inline static Window             window;
    inline static AutoClicker        clicker;
    inline static RectangleMarker    areaMarker;
    inline static MonitorPixelReader reader;
    inline static SmoothedFPSCounter   fpsCounter{10};
};