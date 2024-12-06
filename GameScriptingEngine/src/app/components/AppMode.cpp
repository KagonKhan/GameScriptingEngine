#include "app/components/AppMode.hpp"

#include "app/events/Events.hpp"

AppMode::State AppMode::checkState() {
    // TODO: in case this ever grows = have a library of names?
    if (ImGuiWindow const * const main_window = ImGui::FindWindowByName("###MAIN_WINDOW"); main_window != nullptr) {
        return static_cast<State>(main_window->Rect().Contains(Mouse::GetPosition()));
    }

    spdlog::critical("{} Cannot locate main window", TAG);
    return State::INTERACTIVE;
}

bool AppMode::initializer() {
    eventListener.listen([](Events::ForceMode const& event) { lockedState.emplace(event.mode); });
    eventListener.listen([](Events::ReleaseModeEnforcement const&) { lockedState.reset(); });

    return true;
}