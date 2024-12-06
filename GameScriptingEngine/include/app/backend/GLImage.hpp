#pragma once
#include <GLAD/GLAD.h>
#include <ImGui/imgui.h>
#include <opencv2/opencv.hpp>


class GLImage {
public:
    GLImage();
    ~GLImage();

    void setData(const int* data) const;
    void setData(cv::Mat const& image) const;
    void resize(ImVec2 newSize);

    GLuint getID() const { return texture; }
    ImVec2 getSize() const { return size; }

private:
    GLuint texture;
    ImVec2 size;
};