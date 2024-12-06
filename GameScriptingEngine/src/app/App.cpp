#include "app/App.hpp"

#include "app/AssetManager.hpp"
#include "app/events/Events.hpp"
#include "components/interaction/ScreenImageSaver.hpp"
#include "components/search/TemplateMatcher.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <spdlog/spdlog.h>


namespace {
auto fix_monitor_dpi_differences = [windowDPI = 0.0f]() mutable {

};

} // namespace
void App::Start() {
    eventListener.listen([](Events::SetWindowVisibility const& event) { visible = event.isVisible; });
    try {
    InputListener::Initialize();
    while (isRunning && !window.shouldClose()) {

            Update();

            window.startFrame();
            window.fixScaling();
            Render();

            window.endFrame();
        } 
    } catch (std::exception const& e) {
        InputListener::Release();
        spdlog::critical("{} Error occured: {}", TAG, e.what());
        throw;
    }
}

void App::Update() {
    glfwPollEvents();
    GlobalEventBus::Process();

    if (AppMode::get() == AppMode::State::OVERLAY) {
        ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoInputs;
    }

    fpsCounter.measure();
}


void App::Render() {
    static bool demo = false;
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, visible ? 1.0f : 0.0f);

    ImGui::Begin(WindowName(), &isRunning);
    {
        fpsCounter.render();

        RenderComponents();
        TemporaryRender();


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

    static GLImage          image;
    static ScreenImageSaver saver;
    if (auto area = areaMarker.get(); area.GetArea() > 0) {
        reader.updateRegion(area);
        reader.updateImage();
        reader.updateRender();
        // reader.render();


         saver.save(reader.getImage());
        if (ImGui::Button("your mom")) {
            image.setData(reader.getImage());
        }
    }
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), {800, 600}, ImVec2(0, 1),
                 ImVec2(1, 0));

    templateMatcherWidget();

    clicker.render();

    // static GLImage image;
    // static cv::Mat mat = AssetManager::Retrieve("6.png");
    // if (static bool ranOnce = false; !ranOnce) {
    //     image.resize({static_cast<float>(mat.cols), static_cast<float>(mat.rows)});
    //     cv::Mat fmat;
    //     cv::flip(mat, fmat, 0);
    //     image.setData(fmat);
    //     ranOnce = true;
    // }
    // ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), {800, 600}, ImVec2(0, 1),
    //              ImVec2(1, 0));
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


const char* App::WindowName() {
    constexpr static const char* const overlay_name{"GameScriptingEngine OVERLAY mode ###MAIN_WINDOW"};
    constexpr static const char* const interactive_name{"GameScriptingEngine INTERACTIVE mode ###MAIN_WINDOW"};

    return AppMode::get() == AppMode::State::INTERACTIVE ? interactive_name : overlay_name;
}