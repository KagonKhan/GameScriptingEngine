#include "app/backend/WindowContext.hpp"

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/implot.h"
#include "app/Exceptions.hpp"

#include <spdlog/spdlog.h>

namespace {
void glfw_error_callback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}\n", error, description);
}
} // namespace


WindowContext::WindowContext() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        throw ContextInitializationError("{} Could not initialize GLFW!", TAG);
    }

    setupGLFWWindow();
    loadOpenGL();
    setupImGui();
}

WindowContext::~WindowContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void WindowContext::setupGLFWWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);


    const auto video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(video_mode->width + 1, video_mode->height + 1, "GameScriptingEngine", nullptr, nullptr);
    if (window == nullptr) {
        throw ContextInitializationError("{} Could not create the GLFW window!", TAG);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void WindowContext::loadOpenGL() const {
    const int version = gladLoadGL(glfwGetProcAddress);

    if (!version) {
        throw ContextInitializationError("{} Failed to initialize OpenGL Context!", TAG);
    }

    // Successfully loaded OpenGL
    spdlog::debug("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}

void WindowContext::setupImGui() const {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void WindowContext::startFrame() const {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void WindowContext::endFrame() const {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

bool WindowContext::shouldClose() const { return glfwWindowShouldClose(window); }