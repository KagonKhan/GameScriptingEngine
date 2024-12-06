#pragma once
#include "ImGui/imgui_internal.h"
#include "Utils.hpp"
#include "app/events/EventListener.hpp"
#include "input/Mouse.hpp"

#include <spdlog/spdlog.h>


class AppMode {
private:
    static constexpr char const* const TAG{"[AppMode]"};

public:
    enum class State : std::uint8_t {
        OVERLAY     = 0,
        INTERACTIVE = 1,
    };

    [[nodiscard]] static State get() { return lockedState.value_or(checkState()); }

private:
    [[nodiscard]] static State checkState();
    [[nodiscard]] static bool initializer();

    // TODO: convert to static?
    inline static EventListener        eventListener;
    inline static std::optional<State> lockedState{std::nullopt};
    inline static bool                 initialized = initializer();

};