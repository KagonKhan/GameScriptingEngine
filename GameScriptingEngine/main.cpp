

#include "app/App.hpp"
#include "sink.hpp"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/spdlog.h>

// TODO: event library - threading? Will use event bus for now.
// TODO: optimize conan file especially opencv module

int main() {
    ImGuiLogSink::instance = std::make_shared<ImGuiLogSink>();

    // auto callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>(
    //     [](spdlog::details::log_msg const& message) { spdlog::critical("Callback: {}", message.payload);
    //     });


    spdlog::set_level(spdlog::level::debug);
    spdlog::default_logger()->sinks().push_back(ImGuiLogSink::instance);

    App app;
    app.Start(); // TODO: does not need a ctor for now.
}

// TODO: scratch-language like widget for scripting? is that even possible? plugins?
// TODO: remove windows console, and log to imgui window


void idleonNightly() {
    /* Send M
     * Scroll to world 5
     * Teleport to boss, kill it 300 times
     * Teleport to wurms,
     * Auto
     * Bubbles.
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     */
}