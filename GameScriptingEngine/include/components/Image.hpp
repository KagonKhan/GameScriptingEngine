#pragma once
#include "ImGui/imgui.h"


// why u no work. when moved to monitor pixel reader cpp it works/ GLAD?
class Image {
public:
    Image();
    ~Image();

    void setData(const int* data) const;
    void resize(ImVec2 newSize);

    unsigned int getID() const { return texture; }
    ImVec2       getSize() const { return size; }

private:
    unsigned int texture;
    ImVec2       size;
};