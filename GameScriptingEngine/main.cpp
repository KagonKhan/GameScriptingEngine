#include "App.hpp"
#include <spdlog/spdlog.h>

void initialize() { 
    spdlog::set_level(spdlog::level::debug);

}

int main() {
    initialize();

	App app;
    app.start();
}