#include "app/App.hpp"

#include "components/search/TemplateMatcher.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <spdlog/spdlog.h>


namespace {
auto fix_monitor_dpi_differences = [windowDPI = 0.0f]() mutable {
    if (windowDPI == 0.0f) {
        windowDPI = ImGui::GetWindowDpiScale();
    }

    if (const float current_dpi = ImGui::GetWindowDpiScale(); current_dpi != windowDPI) {
        spdlog::debug("DPI changed, resizing the window");
        const ImVec2 size = ImGui::GetWindowSize();
        ImGui::SetWindowSize(size + ImVec2{1.f, 1.f});
        ImGui::SetWindowSize(size - ImVec2{1.f, 1.f});
        windowDPI = ImGui::GetWindowDpiScale();
    }
};

const char* windowName(bool isOverlayMode) {
    constexpr static const char* const overlay_name{"GameScriptingEngine OVERLAY mode ###MAIN_WINDOW"};
    constexpr static const char* const input_name{"GameScriptingEngine INPUT mode ###MAIN_WINDOW"};

    return isOverlayMode ? overlay_name : input_name;
}
} // namespace

void App::Start() {
    Input::Keyboard::AddKeybind(Input::Keyboard::KEY::LEFT_CONTROL, [&] { overlayEnabled = !overlayEnabled; });
    Random::Init();

    // TODO: frame limiter? without image processing its ~1000 i dont want to use vsync
    while (isRunning && !glfwWindowShouldClose(window)) {
        fpsCounter.measure();


        glfwPollEvents();

        // TODO: convert it to be automatic, save the window rect, and detect mouse position. If inside - non overlay
        if (overlayEnabled) {
            ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoInputs;
        }
        fix_monitor_dpi_differences();

        window.startFrame();

        Render();

        window.endFrame();
        auto t2 = std::chrono::steady_clock::now();
    }

    Input::Keyboard::RemoveKeybind(Input::Keyboard::KEY::Q);
}


void App::Render() {
    static bool demo = false;

    ImGui::Begin(windowName(overlayEnabled), &isRunning);
    ImGui::Text("Current fps %.3f", fpsCounter.fps());

    RenderComponents();
    TemporaryRender();


    ImGui::Separator();
    ImGui::Checkbox("Demo", &demo);
    if (demo)
        ImGui::ShowDemoWindow();


    ImGui::End();
}

void App::RenderComponents() {
    ImGui::Separator();
    areaMarker.render();
    ImGui::Separator();

    if (drawRect) {
        ImGui::GetForegroundDrawList()->AddRectFilled(areaMarker.get().GetTL() + mmin, areaMarker.get().GetTL() +mmax,
                                                      ImGui::ColorConvertFloat4ToU32({1, 0, 1, 1}));
   
    }

    if (auto area = areaMarker.get(); area.GetArea() > 0) {
        reader.updateRegion(area);
        reader.updateImage();
        reader.updateRender();
        reader.render();



        if (ImGui::Button("Match")) {
            auto                   t1 = std::chrono::high_resolution_clock::now();
            static TemplateMatcher matcher{};
            auto                   screen = reader.getImage();
            static cv::Mat         templ  = cv::imread("C:\\Users\\DogeDen\\Downloads\\templ.png");
            
            matcher.templateMatch(screen, templ);


            
            auto t2 = std::chrono::high_resolution_clock::now();
            spdlog::critical("MEASUREMENT: {} ms",
                             std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
        }
    }


    ImGui::Checkbox("AutoClicker", &clicker.isVisible);

    if (clicker.isVisible) {
        clicker.render();
    }
}
void App::TemporaryRender() {}
