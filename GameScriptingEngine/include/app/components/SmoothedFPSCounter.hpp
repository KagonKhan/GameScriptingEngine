#pragma once

#include "Utils.hpp"

#include <chrono>
#include <valarray>

class SmoothedFPSCounter {
public:
    void measure();
    void widget() const;
    void plot() const;


private:
    float averageFPS{0.0f};


    Utils::timepoint lastTime{Utils::now()};
    Utils::timepoint currTime{Utils::now()};

    class Buffer {
        float                count{120};
        int                  currentIndex{0};
        std::valarray<float> time{count}, value{count};

    public:
        void add(float fps) {
            
        }
    } buffer;
};