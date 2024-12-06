#pragma once

#include "GLAD/GLAD.h"

#include <GLFW/glfw3.h>

class WindowContext {
private:
    static constexpr char const* const TAG{"[WindowContext]"};

public:
    WindowContext();
    ~WindowContext();

    WindowContext(WindowContext const&)            = delete;
    WindowContext& operator=(WindowContext const&) = delete;
    WindowContext(WindowContext&&)                 = delete;
    WindowContext& operator=(WindowContext&&)      = delete;

public:
    explicit           operator GLFWwindow*() const { return window; }
    void               startFrame() const;
    void               endFrame() const;
    [[nodiscard]] bool shouldClose() const;

private:
    void setupGLFWWindow();
    void loadOpenGL() const;
    void setupImGui() const;

private:
    const char* glsl_version = "#version 150";
    GLFWwindow* window{nullptr};
};