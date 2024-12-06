#include "components/MonitorPixelReader.hpp"

#include "Utils.hpp"

#include <spdlog/spdlog.h>


MonitorPixelReader::MonitorPixelReader(ImRect area) { resize(area); }

void MonitorPixelReader::resize(const ImRect new_area) {
    if (region.ToVec4() != new_area.ToVec4()) {
        // TODO: oh boy is any of this leaking? fucking windows api
        region = new_area;
        DeleteObject(hCaptureBitmap);
        hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, region.GetWidth(), region.GetHeight());
        SelectObject(hCaptureDC, hCaptureBitmap);
        pixels = std::make_unique<RGBQUAD[]>(static_cast<int>(region.GetArea()));

        image.resize(region.GetSize());


        bmi.bmiHeader.biSize        = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth       = region.GetWidth();
        bmi.bmiHeader.biHeight      = region.GetHeight();
        bmi.bmiHeader.biPlanes      = 1;
        bmi.bmiHeader.biBitCount    = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
    }
}

MonitorPixelReader::~MonitorPixelReader() {
    ReleaseDC(hDesktopWnd, hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}
void MonitorPixelReader::updateRegion(const ImRect new_area) { resize(new_area); }

void MonitorPixelReader::updateRender() const { image.setData(reinterpret_cast<int*>(pixels.get())); }

void MonitorPixelReader::updateImage() {
    BitBlt(hCaptureDC, 0, 0, static_cast<int>(region.GetWidth()), static_cast<int>(region.GetHeight()), hDesktopDC,
           static_cast<int>(region.Min.x), static_cast<int>(region.Min.y), SRCCOPY | CAPTUREBLT);

    GetDIBits(hCaptureDC, hCaptureBitmap, 0, static_cast<unsigned int>(region.GetHeight()), pixels.get(), &bmi,
              DIB_RGB_COLORS);
}

void MonitorPixelReader::render(const ImVec2 size) const {
    const ImVec2 render_size = (static_cast<int>(size.x) * static_cast<int>(size.y)) == 0 ? image.getSize() : size;

    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), render_size, ImVec2(0, 1),
                 ImVec2(1, 0));
}

// TODO: stopped working?
// TODO: is it possible to avoid flipping the image? windows reads bottom->top, cv expects top->bottom
cv::Mat MonitorPixelReader::getImage() {
    auto t1 = Utils::now();

    const int y = static_cast<int>(image.getSize().y);
    const int x = static_cast<int>(image.getSize().x);
    cv::Mat   img(y, x, CV_8UC4);

    int rowSize = img.step; // Step (row size in bytes)

    // Copy rows in reverse order
    for (int i = 0; i < y; ++i) {
        const auto* sourceRow = reinterpret_cast<uchar*>(pixels.get()) + (y - 1 - i) * rowSize; // Start from the bottom
        uchar*      destRow   = img.ptr<uchar>(i);                                              // Row in the Mat
        std::memcpy(destRow, sourceRow, rowSize);
    }

    // std::memcpy(img.data, pixels.get(), y * x);

    // GetDIBits(hCaptureDC, hCaptureBitmap, 0, static_cast<unsigned int>(region.GetHeight()), img.data, &bmi,
    // DIB_RGB_COLORS);

    // cv::Mat fixed_orientation(y, x, CV_8UC4);
    // cv::flip(img, fixed_orientation, 0);

    auto t2 = Utils::now();
    spdlog::critical("Image retrieval: {}", Utils::duration_us(t1, t2));

    return img;
}

// ~11 ms for flip method
// ~3ms for memcpy