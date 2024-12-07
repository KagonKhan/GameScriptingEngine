#include "app/components/SmoothedFPSCounter.hpp"

#include "Utils.hpp"

#include <ImGui/implot.h>

void SmoothedFPSCounter::measure() {
    lastTime                       = std::exchange(currTime, Utils::now());
    dtMeasurements[currentIndex++] = static_cast<float>(Utils::duration_us(lastTime, currTime));
    if (currentIndex >= count) {
        averageFPS     = static_cast<float>(count) * (1'000'000.f / dtMeasurements.sum());
        dtMeasurements = static_cast<float>(currentIndex = 0);
    }
    plotBuffer.add(averageFPS);
}

void SmoothedFPSCounter::render() const {
    ImGui::Text("Current fps %.3f", averageFPS);
    ImGui::Spacing();
    plot();
}


void SmoothedFPSCounter::plot() const {
    ImPlot::PushStyleColor(ImPlotCol_FrameBg, {0, 0, 0, 0});

    if (ImPlot::BeginPlot("##Scrolling", plotSize, 63)) {
        ImPlot::SetupAxes(nullptr, nullptr, 49423, 49410);

        ImPlot::SetupAxisLimits(ImAxis_X1, plotBuffer.t - 5.0f, plotBuffer.t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 1.0f, 150.0f, ImGuiCond_Always);

        ImPlot::PlotLine("##FPS", &plotBuffer.data[0].x, &plotBuffer.data[0].y, plotBuffer.count, 0,
                         plotBuffer.currentIndex, 2 * sizeof(float));
        ImPlot::EndPlot();
    }

    ImPlot::PopStyleColor();
}
