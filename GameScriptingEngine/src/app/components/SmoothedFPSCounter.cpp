#include "app/components/SmoothedFPSCounter.hpp"

#include "ImGui/implot.h"
#include "Utils.hpp"

#include <chrono>

// utility structure for realtime plot


void Demo_RealtimePlots(float fps) {}

void SmoothedFPSCounter::measure() {
    lastTime                            = std::exchange(currTime, Utils::now());
    dtMeasurements[currentIndex++ % 10] = static_cast<float>(Utils::duration_ms(currTime, lastTime));
    averageFPS                          = measurementsCount / dtMeasurements.sum();
}
void SmoothedFPSCounter::widget() const { ImGui::Text("Current fps %.3f", averageFPS); }


void SmoothedFPSCounter::plot() const {
    static ScrollingBuffer sdata2;
    static float           t = 0;

    t += ImGui::GetIO().DeltaTime;

    sdata2.AddPoint(t, fps);

    static float history = 10.0f;


    if (ImPlot::BeginPlot("FPS graph", ImVec2(-1, 150))) {
        ImPlot::SetupAxes("time", "f ps");
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, averageFPS * 0.5f, averageFPS * 2.0f);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

        ImPlot::PlotLine("FPS", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset,
                         2 * sizeof(float));
        ImPlot::EndPlot();
    }
}