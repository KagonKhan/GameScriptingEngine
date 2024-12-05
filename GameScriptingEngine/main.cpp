#include "app/App.hpp"
#include "app/AssetManager.hpp"

#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
static void initialize() { spdlog::set_level(spdlog::level::debug); }
int         dummy_error_handler(int status, char const* func_name, char const* err_msg, char const* file_name, int line,
                                void* userdata) {
    // Do nothing -- will suppress console output
    return 0; // Return value is not used
}

// TODO: event library - threading? Will use event bus for now.
// TODO: optimize conan file especially opencv module
int main() {
    cv::redirectError(dummy_error_handler);
    AssetManager::ReadAll();

    initialize();

    try {
        // Necessary to unconditionally delete windows hooks,
        // otherwise the entire system is unstable during exception.
        InputListener listener;

        App app;
        app.Start(); // TODO: does not have a ctor for now.

    } catch (std::exception e) {
        spdlog::critical("[main] Exception occured {}", e.what());
        throw;
    }
}