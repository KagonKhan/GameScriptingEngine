#pragma once
#include "app/backend/GLImage.hpp"

#include <ImGui/imgui_internal.h>
#include <Windows.h>
#include <memory>


class MonitorPixelReader {
private:
    static constexpr char const* const TAG{"[MonitorPixelReader]"};

public:
    MonitorPixelReader() = default;
    explicit MonitorPixelReader(ImRect area);
    ~MonitorPixelReader();

    MonitorPixelReader(MonitorPixelReader const&)           = delete;
    MonitorPixelReader operator=(MonitorPixelReader const&) = delete;
    MonitorPixelReader(MonitorPixelReader&&)                = delete;
    MonitorPixelReader operator=(MonitorPixelReader&&)      = delete;

public:
    void    updateImage();
    void    updateRegion(ImRect new_area);
    void    updateRender() const;
    void    render(ImVec2 size = {0, 0}) const;
    [[nodiscard]] cv::Mat getImage();

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
