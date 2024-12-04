#include "components/MonitorPixelReader.hpp"
#include <spdlog/spdlog.h>

MonitorPixelReader::MonitorPixelReader(ImRect area) { resize(area); }

void MonitorPixelReader::resize(const ImRect new_area) {
    if (region.ToVec4() != new_area.ToVec4()) {
        // TODO: oh boy is any of this leaking? fucking windows api
        region = new_area;
        DeleteObject(hCaptureBitmap);
        hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, region.GetWidth(), region.GetHeight());
        SelectObject(hCaptureDC, hCaptureBitmap);
        pixels         = std::make_unique<RGBQUAD[]>(static_cast<int>(region.GetArea()));

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

void MonitorPixelReader::updateRender() { image.setData(reinterpret_cast<int*>(pixels.get())); }

void MonitorPixelReader::updateImage() {
    BitBlt(hCaptureDC, 0, 0, region.GetWidth(), region.GetHeight(), hDesktopDC, region.Min.x, region.Min.y,
           SRCCOPY | CAPTUREBLT);

    GetDIBits(hCaptureDC, hCaptureBitmap, 0, region.GetHeight(), pixels.get(), &bmi, DIB_RGB_COLORS);
}

void MonitorPixelReader::render(const ImVec2 size) const {
    const ImVec2 render_size = (static_cast<int>(size.x) * static_cast<int>(size.y)) == 0 ? image.getSize() : size;
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(image.getID())), render_size, ImVec2(0, 1), ImVec2(1, 0));
}

cv::Mat MonitorPixelReader::getImage() {
    cv::Mat img(image.getSize().y, image.getSize().x, CV_8UC4);
    GetDIBits(hCaptureDC, hCaptureBitmap, 0, region.GetHeight(), img.data, &bmi, DIB_RGB_COLORS);

    cv::Mat fixed_orientation(image.getSize().y, image.getSize().x, CV_8UC4);
    cv::flip(img, fixed_orientation, 0);
    return fixed_orientation;
}
