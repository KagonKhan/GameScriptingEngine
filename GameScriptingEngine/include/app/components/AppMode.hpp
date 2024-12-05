#pragma once
#include "ImGui/imgui_internal.h"
#include "Utils.hpp"
#include "app/events/EventListener.hpp"
#include "input/Mouse.hpp"

#include <spdlog/spdlog.h>


class AppMode : NonMovable {
private:
    static constexpr char const* const TAG{"[AppMode]"};

public:
    enum class State : std::uint8_t {
        OVERLAY     = 0,
        INTERACTIVE = 1,
    };

    AppMode();
    ~AppMode();


    [[nodiscard]] State get() const {
        return State::INTERACTIVE;
        lockedState.value_or(checkState());
    }

private:
    [[nodiscard]] State checkState() const {
        // TODO: in case this ever grows = have a library of names?
        if (ImGuiWindow* main_window = ImGui::FindWindowByName("###MAIN_WINDOW"); main_window != nullptr) {
            return static_cast<State>(main_window->Rect().Contains(Mouse::GetPosition()));
        }

        spdlog::critical("{} Cannot locate main window", TAG);
        return State::INTERACTIVE;
    }

    // TODO: convert to static?
    // TODO: not very satisfied with the locking. Any other ideas?
    EventListener        eventListener;
    std::optional<State> lockedState{std::nullopt};
};