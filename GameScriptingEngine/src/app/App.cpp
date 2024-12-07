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


int STEP = 0;


// TODO: make this proper
struct MouseScroller {
    inline static int                    times{0};
    inline static Mouse::ScrollDirection direction;

    void operator()() {
        if (times-- > 0) {
            Mouse::Scroll(direction);
        }
    }
} scroller;

void delay(int time = 1000) { std::this_thread::sleep_for(std::chrono::milliseconds{time}); }

void toggleMap() {
    Keyboard::send(Keyboard::KEY::M);
    delay();
}

void focusMiddle() {
    Mouse::Click(ImVec2{2560, 1440} / 2.0f);
    delay();
}

void scrollToWorld5() {
    for (int i = 0; i < 5; ++i) {
        Mouse::Scroll(Mouse::ScrollDirection::UP);
        delay(50);
    }
    Mouse::Scroll(Mouse::ScrollDirection::DOWN);
}

void fightBoss() {
    ImVec2 auto_button_pos{1175, 1346};
    ImVec2 boss_map_pos{640, 767};
    ImVec2 boss_cavern_pos{158, 991};
    ImVec2 enter_cavern_pos{1157, 398};
    ImVec2 redo_boss_fight_pos{594, 191};
    ImVec2 wurm_map_pos{1189, 1054};
    ImVec2 quick_ref_button{1552, 299};
    ImVec2 alchemy_button{1243, 742};

    if (STEP == 3)
        STEP++;

    if (STEP == 4) {
        Mouse::Click(boss_map_pos);
        delay(100);
        Mouse::Click(boss_map_pos);
        delay(200);
        STEP++;
        return;
    }
    if (STEP == 5) {
        delay();
        STEP++;
        return;
    }
    if (STEP == 6) {
        Mouse::Click(boss_cavern_pos);
        delay(2000);
        STEP++;
        return;
    }
    if (STEP == 7) {
        Mouse::Click(enter_cavern_pos);
        delay();
        STEP++;
        return;
    }
    if (STEP == 8) {
        Mouse::Click(auto_button_pos);
        delay();
        STEP++;
        return;
    }
    if (STEP == 9) {
        for (static int i = 0; i < 0; i++) {
            Mouse::Click(redo_boss_fight_pos);
            delay(11000);
            delay();
            return;
        }
        STEP++;
        return;
    }
    if (STEP == 10) {
        toggleMap();
        STEP++;
        return;
    }
    if (STEP == 11) {
        Mouse::Click(wurm_map_pos);
        delay(50);
        Mouse::Click(wurm_map_pos);
        delay(2000);
        STEP++;
        return;
    }
    if (STEP == 12) {
        Keyboard::send(Keyboard::KEY::C);
        delay();
        Mouse::Click(quick_ref_button);
        delay();
        Mouse::Click(alchemy_button);
        delay();
        STEP++;
        return;
    }
}
struct Bubble {
    ImVec2 pos, upgrade;
};
void doAlchemy() {
    std::vector<Bubble> red_bubbles{
        Bubble{.pos = {389, 755}, .upgrade = {1330, 1062}}, // ROID RAGING
        Bubble{.pos = {275, 657}, .upgrade = {1202, 999}},  // WARRIORS RULE
        Bubble{.pos = {279, 451}, .upgrade = {1209, 786}},  // WYOMING BLOOD
        // SCROLL ONCE
        Bubble{.pos = {319, 405}, .upgrade = {1230, 747}}, // FMJ
        // SCROLL MAX
        Bubble{.pos = {374, 268}, .upgrade = {1283, 675}}, // CROP CHAPTER SPAM
    };


    std::vector<Bubble> green_bubbles{
        Bubble{.pos = {892, 742}, .upgrade = {1791, 1052}},
        Bubble{.pos = {746, 629}, .upgrade = {1633, 969}},
        // SCROLL ONCE
        Bubble{.pos = {795, 572}, .upgrade = {1713, 901}},
        Bubble{.pos = {801, 403}, .upgrade = {1725, 751}},
        // SCROLL TWICE
        Bubble{.pos = {863, 572}, .upgrade = {1800, 900}},
    };


    std::vector<Bubble> purple_bubbles{
        // SCROLL ONCE
        Bubble{.pos = {1415, 694}, .upgrade = {1232, 1020}},
        // SCROLL MAX
        Bubble{.pos = {1323, 822}, .upgrade = {1107, 1076}}, // SPAM

    };


    std::vector<Bubble> yellow_bubbles{
        Bubble{.pos = {1675, 716}, .upgrade = {1482, 1053}},
        Bubble{.pos = {1052, 629}, .upgrade = {1661, 959}},
        Bubble{.pos = {1719, 467}, .upgrade = {1512, 837}},
        // SCROLL ONCE
        Bubble{.pos = {1768, 453}, .upgrade = {1577, 785}},

    };
}


void idleonNightly() {
    GlobalEventBus::Add(Events::ForceMode{.mode = AppMode::State::OVERLAY});
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    if (STEP == 0) {
        focusMiddle();
        toggleMap();
        STEP++;
        return;
    }
    if (STEP == 1) {
        STEP++;
        return;
    }
    if (STEP == 2) {
        scrollToWorld5();
        STEP++;
        return;
    }
    if (STEP >= 3) {
        fightBoss();
        return;
    }
    if (STEP == 13) {
        doAlchemy();
        STEP++;
        return;
    }
    GlobalEventBus::Add(Events::ReleaseModeEnforcement{});
    // Keyboard::send(Keyboard::KEY::M);
    ; /* Send M
       * Scroll to world 5
       * Teleport to boss, kill it 300 times
       * Teleport to wurms,
       * Auto
       * Bubbles.
       *
       *
       *
       *
       *
       *
       *
       *
       *
       *
       *
       *
       *
       *
       */
}


void initKeybinds() {
    //(void)Keyboard::AddKeybind(Keyboard::KEY::E, [] { visible = !visible; });
    //(void) Keyboard::AddKeybind(Keyboard::KEY::LEFT_CONTROL, [] {
    //    static bool toggler = true;

    //    GlobalEventBus::Add(Events::ForceMode{.mode = toggler ? AppMode::State::INTERACTIVE :
    //    AppMode::State::OVERLAY}); toggler = !toggler;
    //});


    //(void) Keyboard::AddKeybind(Keyboard::KEY::W, [] {
    //    MouseScroller::times     = 20;
    //    MouseScroller::direction = Mouse::ScrollDirection::UP;
    //});
    (void) Keyboard::AddKeybind(Keyboard::KEY::LEFT_CONTROL, [] {
        App::inProgress = true;
        STEP            = 0;


        GlobalEventBus::Add(Events::ForceMode{.mode = AppMode::State::OVERLAY});
        
        // auto pos = Mouse::GetPosition();
        // spdlog::critical("Mouse position ({}, {})", pos.x, pos.y);
        /*MouseScroller::times     = 20;
        MouseScroller::direction = Mouse::ScrollDirection::DOWN;*/
    });
    (void) Keyboard::AddKeybind(Keyboard::KEY::ESCAPE, [] {
        App::inProgress = false;
        STEP            = 9990;


        GlobalEventBus::Add(Events::ReleaseModeEnforcement{});
        /*MouseScroller::times     = 20;
        MouseScroller::direction = Mouse::ScrollDirection::DOWN;*/
    });
}


void App::Start() {
    initKeybinds();

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
    // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, visible ? 1.0f : 0.0f);

    ImGui::Begin(WindowName(), &isRunning);
    {

        if (inProgress)
            idleonNightly();



















        scroller();
        // Extract to separate file, also allow scrolling and add coloring;
        if (ImGui::Button("clear")) {
            ImGuiLogSink::instance->clear();
        }
        const auto& messages = ImGuiLogSink::instance->messages();
        ImGui::BeginChild("LogRegion", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (const auto& message : messages) {
            ImGui::TextUnformatted(message.c_str());
        }
        ImGui::SetScrollHereY(0.999f);
        ImGui::EndChild();

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, inProgress);
        if (ImGui::Button(inProgress ? "Script in progress..." : "Run nightly script", ImVec2(400, 200))) {
            inProgress = true;
            idleonNightly();

            GlobalEventBus::Add(Events::ReleaseModeEnforcement{});
            inProgress = false;
            //
        }
        ImGui::PopItemFlag();

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