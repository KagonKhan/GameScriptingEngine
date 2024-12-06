#pragma once

#include "app/backend/WindowContext.hpp"


class Window {
public:
    void startFrame() const { context.startFrame(); }
    void endFrame() const { context.endFrame(); }

    [[nodiscard]] bool shouldClose() const { return context.shouldClose(); }

    void fixScaling();

private:
    WindowContext context;
    float         windowDPI{0.0f};
};
