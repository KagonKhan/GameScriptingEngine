#pragma once
#include <spdlog/sinks/base_sink.h>

class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex> {
public:
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        messages_.clear();
    }

    const std::vector<std::string>& messages() const { return messages_; }
    inline static std::shared_ptr<ImGuiLogSink> instance;

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        std::lock_guard<std::mutex> lock(mutex_);
        spdlog::memory_buf_t        formatted;
        base_sink::formatter_->format(msg, formatted);
        messages_.emplace_back(fmt::to_string(formatted));
    }

    void flush_() override {
        // No flushing needed for in-memory sink
    }


private:
    mutable std::mutex       mutex_;
    std::vector<std::string> messages_;
};