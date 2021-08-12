#pragma once

#include "Platform/WindowStyle.h"

#include "InputModule.h"
#include "TimeModule.h"

namespace tg
{

struct EngineConfiguration final
{
    WindowStyle windowStyle;
    InputMode inputMode;
    VideoMode videoMode;
};

}
