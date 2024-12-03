#include "app/SmoothFPSCounter.hpp"

#include <chrono>

SmoothFPSCounter::SmoothFPSCounter(int count)
    : current_index(0), count(count), measurements(count), lastTime(std::chrono::high_resolution_clock::now()) {}

float SmoothFPSCounter::fps() const {
    float average = measurements.sum() / static_cast<float>(count);
    return 1000.f / average;
}

void SmoothFPSCounter::measure() {
    auto  now         = std::chrono::high_resolution_clock::now();
    float measurement = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 1000.f;
    measurements[current_index++ % 10] = measurement;
    lastTime                           = now;
}