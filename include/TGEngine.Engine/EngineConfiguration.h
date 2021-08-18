#pragma once

#include "Platform/WindowStyle.h"

#include "InputModule.h"
#include "TimeModule.h"

TG_NAMESPACE_BEGIN

struct EngineConfiguration final
{
    WindowStyle windowStyle;
    InputMode inputMode;
    VideoMode videoMode;
};

}
