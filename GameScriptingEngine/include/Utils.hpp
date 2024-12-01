#pragma once
#include <algorithm>
#include <chrono>

static void clamped(int &value, const int low, const int high) { value = std::clamp(value, low, high); }

// TODO: assert alignment works
struct Interval {
    int minutes = {0};
    int seconds = {0};
    int milli   = {0};
    int micro   = {0};


    [[nodiscard]] std::chrono::microseconds asMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::minutes{minutes}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds{seconds}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds{milli}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds{micro});
    }
};
