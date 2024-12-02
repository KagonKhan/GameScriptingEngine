#include "components/RectangleMarker.hpp"

#include "app/App.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "input/Mouse.hpp"

#include <spdlog/spdlog.h>

namespace {
using done_marking = bool;
}

void RectangleMarker::render() {
    ImGui::Text("Marked area: (%d, %d),(%d, %d)", static_cast<int>(markedArea.Min.x),
                static_cast<int>(markedArea.Min.y), static_cast<int>(markedArea.Max.x),
                static_cast<int>(markedArea.Max.y));

    ImGui::SameLine();
    if (showMarked) {
        ImGui::GetForegroundDrawList()->AddRectFilled(markedArea.Min, markedArea.Max,
                                                      ImGui::ColorConvertFloat4ToU32({0.5, 0.5, 0.5, 0.5}));
    }


    if (ImGui::Button("Mark area") || !isDoneMarking) {
        isDoneMarking = markArea();
    }
    if (static_cast<int>(markedArea.GetArea()) <= 0) {
        ImGui::SameLine();
        if (ImGui::Button(showMarked ? "Hide marked area" : "Show marked area")) {
            showMarked = !showMarked;
        }
    }
}

// TODO: cleaner version? Return value - isMarkingInProcess
done_marking RectangleMarker::markArea() {
    // TODO: the early returns are needed because what if the user holds the mouse button... idk how to make this nicer
    // for now.
    if (App::IsOverlayMode()) {
        spdlog::debug("{} Overlay was turned on, turning off for marking", TAG);
        App::FlipMode();
        return done_marking{false};
    }


    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (!rectMin.has_value()) {
        if (Input::Mouse::IsButtonPressed(Input::Mouse::BUTTON::LEFT)) {
            rectMin = Input::Mouse::GetPosition();
        }
        return done_marking{false};
    }

    ImGui::GetForegroundDrawList()->AddRectFilled(rectMin.value(), Input::Mouse::GetPosition(),
                                                  ImGui::ColorConvertFloat4ToU32({0.5, 0.5, 0.5, 0.5}));

    if (Input::Mouse::IsButtonPressed(Input::Mouse::BUTTON::LEFT)) {
        markedArea.Min = rectMin.value();
        markedArea.Max = Input::Mouse::GetPosition();

        if (markedArea.IsInverted()) {
            std::swap(markedArea.Min, markedArea.Max);
        }

        rectMin.reset();
        return done_marking{true};
    }

    return done_marking{false};
}