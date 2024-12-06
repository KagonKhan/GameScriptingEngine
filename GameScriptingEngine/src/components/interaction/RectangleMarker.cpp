#include "components/interaction/RectangleMarker.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "app/App.hpp"
#include "app/events/Events.hpp"
#include "input/Mouse.hpp"

#include <spdlog/spdlog.h>


namespace {
using done_marking = bool;
}


// TODO: add escape keybind to stop the current action?
// TODO: make sure it works with inverted areas (negative)
void RectangleMarker::render() {
    ImGui::Text("Marked area: (%d, %d),(%d, %d)", static_cast<int>(markedArea.Min.x),
                static_cast<int>(markedArea.Min.y), static_cast<int>(markedArea.Max.x),
                static_cast<int>(markedArea.Max.y));

    if (showMarked) {
        ImGui::GetForegroundDrawList()->AddRectFilled(markedArea.Min, markedArea.Max,
                                                      ImGui::ColorConvertFloat4ToU32({0.5, 0.5, 0.5, 0.5}));
    }


    if (ImGui::Button("Mark area") || !isDoneMarking) {
        isDoneMarking = markArea();
    }

    if (!isDoneMarking) {
        ImGui::GetBackgroundDrawList()->AddRectFilled({0, 0}, {2560, 1440},
                                                      ImGui::ColorConvertFloat4ToU32({0, 0, 0, 0.7f}));
    }

    if (static_cast<int>(markedArea.GetArea()) > 0 && (ImGui::SameLine(), ImGui ::Button("Reset"))) {
        reset();
    }
    if (static_cast<int>(markedArea.GetArea()) > 0) {
        ImGui::SameLine();
        if (ImGui::Button(showMarked ? "Hide marked area" : "Show marked area")) {
            showMarked = !showMarked;
        }
    }
}


// TODO: cleaner version?
done_marking RectangleMarker::markArea() {
    // TODO: the early returns are needed because what if the user holds the mouse button... idk how to make this nicer
    // for now.
    if (AppMode::get() == AppMode::State::OVERLAY) {
        GlobalEventBus::Add(Events::ForceMode{.mode =AppMode::State::INTERACTIVE});
        return done_marking{false};
    }
    GlobalEventBus::Add(Events::SetWindowVisibility{.isVisible = false});


    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (!rectMin.has_value()) {
        if (Mouse::IsButtonPressed(Mouse::Button::LEFT)) {
            rectMin = Mouse::GetPosition();
        }
        return done_marking{false};
    }

    ImGui::GetForegroundDrawList()->AddRectFilled(rectMin.value(), Mouse::GetPosition(),
                                                  ImGui::ColorConvertFloat4ToU32({0.5, 0.5, 0.5, 0.5}));

    if (Mouse::IsButtonPressed(Mouse::Button::LEFT)) {
        markedArea.Max = Mouse::GetPosition();
        markedArea.Min = rectMin.value();

        if (markedArea.Min.x > markedArea.Max.x)
            std::swap(markedArea.Min.x, markedArea.Max.x);
        if (markedArea.Min.y > markedArea.Max.y)
            std::swap(markedArea.Min.y, markedArea.Max.y);


        GlobalEventBus::Add(Events::ReleaseModeEnforcement{});
        GlobalEventBus::Add(Events::SetWindowVisibility{.isVisible = true});
        rectMin.reset();
        return done_marking{true};
    }

    return done_marking{false};
}
