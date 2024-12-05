#include "app/components/AppMode.hpp"

#include "app/events/Events.hpp"


AppMode::AppMode() {
    eventListener.listen([this](Events::ForceMode const& event) { lockedState.emplace(event.mode); });
    eventListener.listen([this](Events::ReleaseModeEnforcement const&) { lockedState.reset(); });
}

AppMode::~AppMode() { eventListener.unlistenAll(); }