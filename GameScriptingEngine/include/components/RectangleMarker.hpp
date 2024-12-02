#pragma once
#include "ImGui/imgui_internal.h"
#include "Utils.hpp"

class RectangleMarker {
private:
    static constexpr char const* const TAG{"[RectangleMarker]"};

public:
    bool isVisible{true};

    void render();

private:
    bool markArea();

private:
    ImRect markedArea{};
    bool   isDoneMarking{true};
    bool   showMarked{false};

    std::optional<ImVec2> rectMin{std::nullopt};
};