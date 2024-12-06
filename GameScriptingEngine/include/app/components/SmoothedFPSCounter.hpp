#pragma once

#include "ImGui/imgui.h"
#include "Utils.hpp"

#include <chrono>
#include <valarray>


class SmoothedFPSCounter {
public:
    void measure();
    void widget() const;
    void plot() const ;

private:
    float                averageFPS{0.0f};
    int                  currentIndex{0};
    int                  count{20};
    std::valarray<float> dtMeasurements = std::valarray<float>(count);

    Utils::timepoint lastTime{Utils::now()}, currTime{Utils::now()};

    struct PlotBuffer {
        int   count{600}; // fps * history
        int   currentIndex{0};
        float t{0.0f};

        std::vector<ImVec2> data = std::vector<ImVec2>(count);
        Utils::timepoint    startTime{Utils::now()};

        void add(float fps) {
            t                  = Utils::duration_ms(startTime, Utils::now()) / 1000.0f;
            data[currentIndex] = {t, fps};
            currentIndex       = (currentIndex + 1) % count;
        }
    } plotBuffer;
};