#pragma once
#include "ImGui/imgui_internal.h"
#include "Utils.hpp"

class RectangleMarker {
private:
    static constexpr char const* const TAG{"[RectangleMarker]"};

public:
    bool isVisible{true};

    void   render();
    ImRect get() const {return markedArea;}
    void reset() { markedArea = {0, 0, 0, 0}; }

private:
    bool markArea();

private:
    ImRect                markedArea{};
    bool                  isDoneMarking{true};
    bool                  showMarked{false};
    std::optional<ImVec2> rectMin{std::nullopt};
};