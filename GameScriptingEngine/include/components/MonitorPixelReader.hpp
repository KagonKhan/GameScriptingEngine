#pragma once
#include "components/Image.hpp"
#include "ImGui/imgui_internal.h"

#include <Windows.h>
#include <memory>


class MonitorPixelReader {
public:
    MonitorPixelReader() = default;
    MonitorPixelReader(ImRect area);
    ~MonitorPixelReader();

    void updateImage();
    void updateRegion(ImRect newArea);
    void render(ImVec2 size = {0, 0});

private:
    void resize(ImRect newArea);

    ImRect                     region;
    std::unique_ptr<RGBQUAD[]> pixels;

    HWND       hDesktopWnd = GetDesktopWindow();
    HDC        hDesktopDC  = GetDC(hDesktopWnd);
    HDC        hCaptureDC  = CreateCompatibleDC(hDesktopDC);
    HBITMAP    hCaptureBitmap;
    BITMAPINFO bmi {0};
    Image      image {};
};

// TODO: idk about this name, iffy
