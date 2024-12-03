#include "components/Image.hpp"

#include <GLAD/GLAD.h>

Image::Image() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    // TODO: make this work finally... performance maybe?
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, 2560, 1440);
}

Image::~Image() { glDeleteTextures(1, &texture); }

void Image::setData(const int* data) const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::resize(ImVec2 newSize) {
    if (size == newSize)
        return;

    size = newSize;

    // TODO: later some optimizations possible on GL side?
}