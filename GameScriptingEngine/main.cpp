#include "app/App.hpp"

#include <spdlog/spdlog.h>

static void initialize() { spdlog::set_level(spdlog::level::debug); }

int main() {
    initialize();

    App app;
    app.Start();
}