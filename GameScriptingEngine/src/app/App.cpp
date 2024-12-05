#include "app/App.hpp"

#include "Random.hpp"
#include "app/AssetManager.hpp"
#include "components/interaction/ScreenImageSaver.hpp"
#include "components/search/TemplateMatcher.hpp"
#include "input/Mouse.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <opencv2/imgcodecs.hpp>
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

const char* WindowName(AppMode const& appMode) {
    constexpr static const char* const overlay_name{"GameScriptingEngine OVERLAY mode ###MAIN_WINDOW"};
    constexpr static const char* const interactive_name{"GameScriptingEngine INTERACTIVE mode ###MAIN_WINDOW"};

    return appMode.get() == AppMode::State::INTERACTIVE? interactive_name : overlay_name;
}
} // namespace

void App::Start() {


    // TODO: frame limiter? without image processing its ~1000 i dont want to use vsync
    while (isRunning && !glfwWindowShouldClose(window)) {
        fpsCounter.measure();


        glfwPollEvents();
        GlobalEventBus::Process();

        // TODO: convert it to be automatic, save the window rect,
        // and detect mouse position. If inside - non overlay
        if (appMode.get() == AppMode::State::OVERLAY) {
            ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoInputs;
        }
        fix_monitor_dpi_differences();

        window.startFrame();

        Render();

        window.endFrame();
        auto t2 = std::chrono::steady_clock::now();
    }

}


void App::Render() {
    static bool demo = false;


    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::Begin(WindowName(appMode), &isRunning);

    // TODO: any way to make a callback instead of writing this each frame
    //appArea = ImGui::GetCurrentWindow()->Rect();
    

    ImGui::Text("Current fps %.3f", fpsCounter.fps());

    RenderComponents();
    TemporaryRender();

    static GLImage image;
    static cv::Mat mat = AssetManager::Retrieve("6.png");
    if (static bool ranOnce = false; !ranOnce) {
        image.resize({static_cast<float>(mat.cols), static_cast<float>(mat.rows)});
        cv::Mat fmat;
        cv::flip(mat, fmat, 0);
        image.setData(reinterpret_cast<const int*>(fmat.data), fmat);
        ranOnce = true;
    }
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), {800, 600}, ImVec2(0, 1),
                 ImVec2(1, 0));

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


    static ScreenImageSaver saver;
    if (auto area = areaMarker.get(); area.GetArea() > 0) {
        reader.updateRegion(area);
        reader.updateImage();
        reader.updateRender();
        reader.render();


        saver.save(reader.getImage());
    }

    templateMatcherWidget();


    ImGui::Checkbox("AutoClicker", &clicker.isVisible);

    if (clicker.isVisible) {
        clicker.render();
    }
}
void App::TemporaryRender() {}


void App::templateMatcherWidget() {
    static bool   draw = false;
    static ImVec2 drawBegin, drawSize;

    if (auto area = areaMarker.get(); area.GetArea() > 0) {
        if (ImGui::Button("Find template")) {
            static cv::Mat templateImage = cv::imread("C:\\Users\\DogeDen\\Downloads\\templ.png");
            auto           result        = TemplateMatcher::templateMatch(reader.getImage(), templateImage);
            if (result.has_value()) {
                draw      = true;
                drawBegin = result.value().position + area.GetTL();
                drawSize  = result.value().size;
            }
        }
    }

    if (draw) {
        ImGui::GetForegroundDrawList()->AddRectFilled(drawBegin, drawBegin + drawSize,
                                                      ImGui::ColorConvertFloat4ToU32({1, 0, 1, 1}));
    }

    if (areaMarker.get().GetArea() < 1.0f) {
        draw = false;
    }
}