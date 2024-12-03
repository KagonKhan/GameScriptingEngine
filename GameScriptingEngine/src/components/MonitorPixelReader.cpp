#include "components/MonitorPixelReader.hpp"
#include <spdlog/spdlog.h>

//static class Image {
//public:
//    Image(){
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        glBindTexture(GL_TEXTURE_2D, 0);
//        // TODO: make this work...
//        // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, 2560, 1440);
//    }
//    ~Image() { glDeleteTextures(1, &texture);
//    }
//    void setData(const int* data) {
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//
//    void resize(ImVec2 newSize) {
//        if (size == newSize)
//            return;
//
//        size = newSize;
//
//        // TODO: later some optimizations possible on GL side?
//    }
//
//    unsigned int getID() const { return texture; }
//    ImVec2       getSize() const { return size; }
//
//private:
//    unsigned int texture;
//    ImVec2       size;
//} image{};


MonitorPixelReader::MonitorPixelReader(ImRect area) { resize(area); }

void MonitorPixelReader::resize(ImRect newArea) {
    if (region.ToVec4() != newArea.ToVec4()) {
        // TODO: oh boy is any of this leaking? fucking windows api
        region = newArea;
        DeleteObject(hCaptureBitmap);
        hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, region.GetWidth(), region.GetHeight());
        SelectObject(hCaptureDC, hCaptureBitmap);
        pixels         = std::make_unique<RGBQUAD[]>((int) region.GetArea());

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
void MonitorPixelReader::updateRegion(ImRect newArea) { resize(newArea); }

void MonitorPixelReader::updateImage() {
    BitBlt(hCaptureDC, 0, 0, region.GetWidth(), region.GetHeight(), hDesktopDC, region.Min.x, region.Min.y,
           SRCCOPY | CAPTUREBLT);


    GetDIBits(hCaptureDC, hCaptureBitmap, 0, region.GetHeight(), pixels.get(), &bmi, DIB_RGB_COLORS);


    image.setData(reinterpret_cast<int*>(pixels.get()));
}

void MonitorPixelReader::render(ImVec2 size) {
    ImVec2 render_size = ((int) size.x * (int) size.y) == 0 ? image.getSize() : size;
    ImGui::Image((ImTextureID) (intptr_t) image.getID(), render_size, ImVec2(0, 1), ImVec2(1, 0));
}
