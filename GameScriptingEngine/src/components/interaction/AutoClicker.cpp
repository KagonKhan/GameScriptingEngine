#include "components/interaction/AutoClicker.hpp"

#include "Utils.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"

#include <ranges>
#include <ImGui/imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <string>


AutoClicker::AutoClicker() {
    Keyboard::AddKeybind(keybind, [this] { isRunning ? stopClicking() : startClicking(); });
}

AutoClicker::~AutoClicker() { stopClicking(); }

void AutoClicker::render() {
    // TODO: make this prettier without so much stupid code - YouTube tutorials on DearImGui style or something


    ImGui::Checkbox("AutoClicker", &isVisible);

    if (!isVisible)
        return;

    keybindWidget();
    intervalWidget();


    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));
    ImGui::Text("Current period in ms: {%lld}", interval.asMicroseconds().count());
}

void AutoClicker::keybindWidget() {
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));

    const std::string current_preview = Keyboard::KeyName(keybind);
    if (ImGui::BeginCombo("Keybind", current_preview.c_str(), ImGuiComboFlags_WidthFitPreview)) {
        for (const auto& key : magic_enum::enum_entries<Keyboard::KEY>() | std::views::keys) {
            const bool is_selected = key == keybind;
            if (ImGui::Selectable(Keyboard::KeyName(key).c_str(), is_selected)) {
                Keyboard::RemoveKeybind(keybind);
                keybind = key;
                Keyboard::AddKeybind(keybind, [this] { isRunning ? stopClicking() : startClicking(); });
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }


        ImGui::EndCombo();
    }
    ImGui::Spacing();
}

void AutoClicker::intervalWidget() {
    ImGui::Text("Interval: ");
    ImGui::PushItemWidth(25);
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(40, 0));

    ImGui::InputInt("min", &interval.minutes, 0);
    clamped(interval.minutes, 0, 59);
    ImGui::SameLine();
    ImGui::InputInt("sec", &interval.seconds, 0);
    clamped(interval.seconds, 0, 59);
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(45);
    ImGui::InputInt("milli", &interval.milli, 0);
    clamped(interval.milli, 0, 1000);
    ImGui::SameLine();
    ImGui::InputInt("micro", &interval.micro, 0);
    clamped(interval.micro, 0, 1000);
    ImGui::PopItemWidth();
}

void AutoClicker::startClicking() {
    if (interval.asMicroseconds().count() == 0){
        spdlog::warn("{} Click period is 0, skipping!", TAG);
        return;
        }

    isRunning = true;
    spdlog::debug("{} started clicking.", TAG);
    clicker = std::thread([this] {
        while (isRunning) {
            Mouse::Click();
            std::this_thread::sleep_for(interval.asMicroseconds());
        }
    });
};

void AutoClicker::stopClicking() {
    if (!isRunning)
        return;

    spdlog::debug("{} stopped clicking.", TAG);
    isRunning = false;
    if(clicker.joinable()) clicker.join();
}