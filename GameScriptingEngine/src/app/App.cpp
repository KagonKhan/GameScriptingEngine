#include "app/App.hpp"

#include "ImGui/implot.h"
#include "Random.hpp"
#include "app/AssetManager.hpp"
#include "app/events/Events.hpp"
#include "components/interaction/ScreenImageSaver.hpp"
#include "components/search/TemplateMatcher.hpp"
#include "input/Mouse.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <spdlog/spdlog.h>

// utility structure for realtime plot
struct ScrollingBuffer {
    int              MaxSize;
    int              Offset;
    float            maxValue{0.0f};
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        maxValue = std::max(maxValue, y);
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset       = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }

    float maxVal() { return maxValue; }
};

void Demo_RealtimePlots(float fps) {
    static ScrollingBuffer sdata2;
    static float           t = 0;

    t += ImGui::GetIO().DeltaTime;

    sdata2.AddPoint(t, fps);

    static float history = 10.0f;

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
        ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 120);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

        ImPlot::PlotLine("FPS", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset,
                         2 * sizeof(float));
        ImPlot::EndPlot();
    }
}


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

    return appMode.get() == AppMode::State::INTERACTIVE ? interactive_name : overlay_name;
}
} // namespace
void App::Start() {
    eventListener.listen([](Events::SetWindowVisibility const& event) { visible = event.isVisible; });

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

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, visible ? 1.0f : 0.0f);
    ImGui::Begin(WindowName(appMode), &isRunning);
    {
        auto fps = fpsCounter.fps();
        ImGui::Text("Current fps %.3f", fps);
        Demo_RealtimePlots(fps);

        RenderComponents();
        TemporaryRender();

        static GLImage image;
        static cv::Mat mat = AssetManager::Retrieve("6.png");
        if (static bool ranOnce = false; !ranOnce) {
            image.resize({static_cast<float>(mat.cols), static_cast<float>(mat.rows)});
            cv::Mat fmat;
            cv::flip(mat, fmat, 0);
            image.setData(fmat);
            ranOnce = true;
        }
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), {800, 600}, ImVec2(0, 1),
                     ImVec2(1, 0));

        ImGui::Separator();
        ImGui::Checkbox("Demo", &demo);
        if (demo)
            ImGui::ShowDemoWindow();
    }
    ImGui::End();
    ImGui::PopStyleVar();
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