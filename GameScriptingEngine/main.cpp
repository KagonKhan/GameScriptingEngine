 #include "app/App.hpp"
 #include "app/AssetManager.hpp"

 #include <opencv2/opencv.hpp>
 #include <spdlog/spdlog.h>
 static void initialize() { spdlog::set_level(spdlog::level::debug); }
 int         dummy_error_handler(int status, char const* func_name, char const* err_msg, char const* file_name, int
 line,
                                 void* userdata) {
     // Do nothing -- will suppress console output
     return 0; // Return value is not used
 }

// TODO: event library - threading? Will use event bus for now.
// TODO: optimize conan file especially opencv module
 int main() {
     AssetManager::ReadAll();

     initialize();

     try {
         // Necessary to unconditionally delete windows hooks,
         // otherwise the entire system is unstable during exception.
         InputListener listener;

         App app;
         app.Start(); // TODO: does not have a ctor for now.

     } catch (std::exception const& e) {
         spdlog::critical("[main] Exception occured {}", e.what());
         throw;
     }
 }


// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context
// creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

//#include <stdio.h>
//#define GL_SILENCE_DEPRECATION
//#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <GLES2/gl2.h>
//#endif
//#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_glfw.h"
//#include "ImGui/imgui_impl_opengl3.h"
//
//#include <GLFW/glfw3.h> // Will drag system OpenGL headers
//
//// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and
//// compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+ requires linking with
//// legacy_stdio_definitions.lib, which we do using this pragma. Your own project should not be affected, as you are
//// likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
//#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
//#pragma comment(lib, "legacy_stdio_definitions")
//#endif
//
//// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
//#ifdef __EMSCRIPTEN__
//#include "../libs/emscripten/emscripten_mainloop_stub.h"
//#endif
//
//static void glfw_error_callback(int error, const char* description) {
//    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
//}
//
//// Main code
//int main(int, char**) {
//    glfwSetErrorCallback(glfw_error_callback);
//    if (!glfwInit())
//        return 1;
//
//
//    const char* glsl_version = "#version 150";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
//
//    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
//    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
//
//    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
//
//    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
//    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
//    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
//
//    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//
//
//    const auto video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//    // Create window with graphics context
//    GLFWwindow* window = glfwCreateWindow(video_mode->width + 1, video_mode->height + 1,
//                                          "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
//    if (window == nullptr)
//        return 1;
//
//
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1); // Enable vsync
//
//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void) io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
//    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
//    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport
//   
//
//    ImGui::StyleColorsDark();
//
//    //ImGuiStyle& style = ImGui::GetStyle();
//    //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
//    //    style.WindowRounding              = 0.0f;
//    //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//    //}
//
//
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//
//    // Our state
//    bool   show_demo_window    = true;
//    bool   show_another_window = false;
//    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//    // Main loop
//
//    while (!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
//            ImGui_ImplGlfw_Sleep(10);
//            continue;
//        }
//
//        // Start the Dear ImGui frame
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code
//        // to learn more about Dear ImGui!).
//        if (show_demo_window)
//            ImGui::ShowDemoWindow(&show_demo_window);
//
//        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//        {
//            static float f       = 0.0f;
//            static int   counter = 0;
//
//            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
//
//            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
//            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);
//
//            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
//            ImGui::ColorEdit3("clear color", (float*) &clear_color); // Edit 3 floats representing a color
//
//            if (ImGui::Button(
//                    "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
//                counter++;
//            ImGui::SameLine();
//            ImGui::Text("counter = %d", counter);
//
//            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//            ImGui::End();
//        }
//
//        // 3. Show another simple window.
//        if (show_another_window) {
//            ImGui::Begin("Another Window",
//                         &show_another_window); // Pass a pointer to our bool variable (the window will have a closing
//                                                // button that will clear the bool when clicked)
//            ImGui::Text("Hello from another window!");
//            if (ImGui::Button("Close Me"))
//                show_another_window = false;
//            ImGui::End();
//        }
//
//        // Rendering
//        ImGui::Render();
//        int display_w, display_h;
//        glfwGetFramebufferSize(window, &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//
//        //ImGuiIO& io = ImGui::GetIO();
//        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
//        //    GLFWwindow* backup_current_context = glfwGetCurrentContext();
//        //    ImGui::UpdatePlatformWindows();
//        //    ImGui::RenderPlatformWindowsDefault();
//        //    glfwMakeContextCurrent(backup_current_context);
//        //}
//
//
//        glfwSwapBuffers(window);
//    }
//
//    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}