#pragma once

#include "Core/NonCopyable.h"

TG_NAMESPACE_BEGIN

class MacOSApplication :
    private NonCopyable
{
};

using PlatformApplication = MacOSApplication;

}
