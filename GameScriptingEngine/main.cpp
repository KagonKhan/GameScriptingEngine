#include "app/App.hpp"
#include "app/AssetManager.hpp"

#include <spdlog/spdlog.h>


// TODO: event library - threading? Will use event bus for now.
// TODO: optimize conan file especially opencv module

int main() {
    spdlog::set_level(spdlog::level::debug);

    App app;
    app.Start(); // TODO: does not need a ctor for now.
}

// TODO: scratch-language like widget for scripting? is that even possible? plugins?