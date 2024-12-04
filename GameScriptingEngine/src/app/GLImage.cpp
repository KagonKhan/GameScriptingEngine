#include "app/GLImage.hpp"

#include <GLAD/GLAD.h>

GLImage::GLImage() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    // TODO: make this work finally... performance maybe?
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, 2560, 1440);
}

GLImage::~GLImage() { glDeleteTextures(1, &texture); }

// TODO: issues with packing and alignment
/* for openCV mats
    glPixelStorei(GL_UNPACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, image.step / image.elemSize());
 */
void GLImage::setData(const int* data, cv::Mat image) const {
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, image.step / image.elemSize());


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLImage::resize(ImVec2 newSize) {
    if (size == newSize)
        return;

    size = newSize;

    // TODO: later some optimizations possible on GL side?
}