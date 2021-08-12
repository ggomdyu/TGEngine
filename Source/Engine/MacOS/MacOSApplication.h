#pragma once

#include "Core/NonCopyable.h"

namespace tg
{

class MacOSApplication :
    private NonCopyable
{
};

using PlatformApplication = MacOSApplication;

}
