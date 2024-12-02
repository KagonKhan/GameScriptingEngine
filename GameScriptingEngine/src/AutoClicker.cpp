#include "AutoClicker.hpp"

#include "Utils.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"

#include <ImGui/imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <string>


AutoClicker::AutoClicker() {
    Input::Keyboard::AddKeybind(keybind, [this] { isRunning ? stopClicking() : startClicking(); });
}

AutoClicker::~AutoClicker() { stopClicking(); }

void AutoClicker::render() {
    ImGui::Separator();
    // TODO: make this prettier without so much stupid code - youtube tutorials on imgui style or something

    keybindWidget();
    intervalWidget();


    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));
    ImGui::Text("Current period in ms: {%llu}", interval.asMicroseconds().count());

    ImGui::Separator();
}

void AutoClicker::keybindWidget() {
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));

    std::string current_preview = Input::Keyboard::KeyName(keybind);
    if (ImGui::BeginCombo("Keybind", current_preview.c_str(), ImGuiComboFlags_WidthFitPreview)) {
        for (const auto& [value, name] : magic_enum::enum_entries<Input::Keyboard::Key>()) {
            bool is_selected = value == keybind;
            if (ImGui::Selectable(Input::Keyboard::KeyName(value).c_str(), is_selected)) {
                Input::Keyboard::RemoveKeybind(keybind);
                keybind = value;
                Input::Keyboard::AddKeybind(keybind, [this] { isRunning ? stopClicking() : startClicking(); });
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
            Input::Mouse::Click();
            std::this_thread::sleep_for(interval.asMicroseconds());
        }
    });
};

void AutoClicker::stopClicking() {
    spdlog::debug("{} stopped clicking.", TAG);
    isRunning = false;
    if(clicker.joinable()) clicker.join();
}