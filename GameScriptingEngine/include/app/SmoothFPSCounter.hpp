#pragma once
#include <chrono>
#include <valarray>

 class SmoothFPSCounter {
public:
     explicit SmoothFPSCounter(int count);

    [[nodiscard]] float fps() const;
     void                measure();


private:
    int                                   current_index;
    int                                   count;
    std::valarray<float>                  measurements;
    std::chrono::steady_clock::time_point lastTime;
};