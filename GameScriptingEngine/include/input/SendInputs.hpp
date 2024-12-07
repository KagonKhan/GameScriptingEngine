#pragma once
#include <Windows.h>
#include <cstdint>

namespace INPUTS {
struct LMB_DOWN {
    LMB_DOWN() {
        ZeroMemory(&data, sizeof(data));
        data.type       = INPUT_MOUSE;
        data.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    }

    INPUT data;
};

struct LMB_UP {
    LMB_UP() {
        ZeroMemory(&data, sizeof(data));
        data.type       = INPUT_MOUSE;
        data.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }

    INPUT data;
};

struct LMB_CLICK {
    LMB_CLICK() {
        ZeroMemory(data, sizeof(data));
        data[0].type       = INPUT_MOUSE;
        data[1].type       = INPUT_MOUSE;
        data[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        data[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }

    INPUT data[2];
};

struct RMB_DOWN {
    RMB_DOWN() {
        ZeroMemory(&data, sizeof(data));
        data.type       = INPUT_MOUSE;
        data.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    }

    INPUT data;
};

struct RMB_UP {
    RMB_UP() {
        ZeroMemory(&data, sizeof(data));
        data.type       = INPUT_MOUSE;
        data.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }

    INPUT data;
};

struct RMB_CLICK {
    RMB_CLICK() {
        ZeroMemory(data, sizeof(data));
        data[0].type       = INPUT_MOUSE;
        data[1].type       = INPUT_MOUSE;
        data[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        data[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }

    INPUT data[2];
};

struct SCROLL_UP {
    SCROLL_UP() {
        ZeroMemory(&data, sizeof(data));
        data.type         = INPUT_MOUSE;
        data.mi.dwFlags   = MOUSEEVENTF_WHEEL;
        data.mi.mouseData = 10;
    }

    INPUT data;
};

struct SCROLL_DOWN {
    SCROLL_DOWN() {
        ZeroMemory(&data, sizeof(data));
        data.type         = INPUT_MOUSE;
        data.mi.dwFlags   = MOUSEEVENTF_WHEEL;
        data.mi.mouseData = -10;
    }

    INPUT data;
};
}; // namespace INPUTS