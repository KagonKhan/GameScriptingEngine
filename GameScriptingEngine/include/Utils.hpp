#pragma once
#include <algorithm>
#include <chrono>
#include <random>

static void clamped(int& value, const int low, const int high) { value = std::clamp(value, low, high); }

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

namespace Utils {
using timepoint = std::chrono::steady_clock::time_point;

inline timepoint now() { return std::chrono::steady_clock::now(); }

inline long long duration_ms(timepoint const& t1, timepoint const& t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}
inline long long duration_us(timepoint const& t1, timepoint const& t2) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
}
inline long long duration_ns(timepoint const& t1, timepoint const& t2) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
}
} // namespace Utils


struct NonCopyable {
    NonCopyable()                              = default;
    ~NonCopyable()                             = default;
    NonCopyable(NonCopyable const&)            = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;
    NonCopyable(NonCopyable&&)                 = default;
    NonCopyable& operator=(NonCopyable&&)      = default;
};
struct NonMovable {
    NonMovable()                             = default;
    ~NonMovable()                            = default;
    NonMovable(NonMovable const&)            = delete;
    NonMovable& operator=(NonMovable const&) = delete;
    NonMovable(NonMovable&&)                 = delete;
    NonMovable& operator=(NonMovable&&)      = delete;
};