#pragma once
#include "app/components/AppMode.hpp"

namespace Events {

struct ForceMode {
    AppMode::State mode;
};
struct ReleaseModeEnforcement {};

struct SetWindowVisibility {
    bool isVisible;
};
} // namespace Events
