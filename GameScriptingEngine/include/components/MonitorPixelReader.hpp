#pragma once
#include "components/Image.hpp"
#include "ImGui/imgui_internal.h"

#include <Windows.h>
#include <memory>


// TODO: idk about this name, iffy
class MonitorPixelReader {
public:
    MonitorPixelReader() = default;
    explicit MonitorPixelReader(ImRect area);
    ~MonitorPixelReader();

    void updateImage();
    void updateRegion(ImRect new_area);
    void render(ImVec2 size = {0, 0}) const;

private:
    void resize(ImRect new_area);

    ImRect                     region;
    std::unique_ptr<RGBQUAD[]> pixels;

    HWND       hDesktopWnd = GetDesktopWindow();
    HDC        hDesktopDC  = GetDC(hDesktopWnd);
    HDC        hCaptureDC  = CreateCompatibleDC(hDesktopDC);
    HBITMAP    hCaptureBitmap;
    BITMAPINFO bmi {};
    Image      image {};
};

