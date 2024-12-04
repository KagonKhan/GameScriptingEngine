#pragma once
#include "ImGui/imgui_internal.h"
#include "app/GLImage.hpp"

#include <Windows.h>
#include <memory>
#include <opencv2/core/core.hpp>


// TODO: idk about this name, iffy
class MonitorPixelReader {
private:
    inline static constexpr char const* const TAG{"[MonitorPixelReader]"};

public:
    MonitorPixelReader() = default;
    explicit MonitorPixelReader(ImRect area);
    ~MonitorPixelReader();

    void    updateImage();
    void    updateRegion(ImRect new_area);
    void    updateRender();
    void    render(ImVec2 size = {0, 0}) const;
    cv::Mat getImage();

private:
    void resize(ImRect new_area);

    ImRect                     region;
    std::unique_ptr<RGBQUAD[]> pixels;

    HWND       hDesktopWnd = GetDesktopWindow();
    HDC        hDesktopDC  = GetDC(hDesktopWnd);
    HDC        hCaptureDC  = CreateCompatibleDC(hDesktopDC);
    HBITMAP    hCaptureBitmap;
    BITMAPINFO bmi{};
    GLImage    image{};
};
