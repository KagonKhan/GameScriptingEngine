#include "app/App.hpp"

#include "app/AssetManager.hpp"
#include "app/events/Events.hpp"
#include "components/interaction/ScreenImageSaver.hpp"
#include "components/search/TemplateMatcher.hpp"
#include "sink.hpp"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>


void App::Start() {
    (void)Keyboard::AddKeybind(Keyboard::KEY::E, [] { visible = !visible; });
    (void)Keyboard::AddKeybind(Keyboard::KEY::LEFT_CONTROL, [] {
        static bool toggler = true;

        GlobalEventBus::Add(Events::ForceMode{.mode = toggler ? AppMode::State::INTERACTIVE : AppMode::State::OVERLAY});
        toggler = !toggler;
    });

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

// TODO: style and docking is becoming VERY important
// https://github.com/ocornut/imgui/issues/4430#:~:text=and%20snippet%20I%20call%20right%20after%20setting%20imgui%20frame%3A
void App::Render() {
    static bool demo = false;
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, visible ? 1.0f : 0.0f);

    ImGui::Begin(WindowName(), &isRunning);
    {

            const auto& messages = ImGuiLogSink::instance->messages();
            ImGui::BeginChild("LogRegion", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            for (const auto& message : messages) {
                ImGui::TextUnformatted(message.c_str());
            }
            ImGui::EndChild();
        

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

        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), {800, 600}, ImVec2(0, 1),
                     ImVec2(1, 0));
    }

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