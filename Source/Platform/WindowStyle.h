#pragma once

#include <cstdint>
#include <string>

namespace tg
{

struct WindowStyle final
{
    std::u8string title = u8"Hello world!";
    int32_t x = 0;
    int32_t y = 0;
    int32_t width = 838;
    int32_t height = 441;
    bool hasCaption = true;
    bool enableSystemButton = true;
    bool enableFullScreen = false;
    bool resizeable = false;
    bool topMost = false;
    bool showMiddle = true;
};

}
