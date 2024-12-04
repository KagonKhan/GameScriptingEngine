#include "app/App.hpp"
#include "app/AssetManager.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>
static void initialize() { spdlog::set_level(spdlog::level::debug); }
int         dummy_error_handler(int status, char const* func_name, char const* err_msg, char const* file_name, int line,
                                void* userdata) {
    // Do nothing -- will suppress console output
    return 0; // Return value is not used
}


// TODO: optimize conan file especially opencv module
int main() {
    cv::redirectError(dummy_error_handler);
    AssetManager::ReadAll();

    initialize();

    App app;
    app.Start();
}