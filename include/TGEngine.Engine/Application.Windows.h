#pragma once

#include "Core/NonCopyable.h"

typedef struct tagMSG MSG;

TG_NAMESPACE_BEGIN

class WindowsApplication :
    private NonCopyable
{
public:
    using MessageCallback = void(*)(const MSG&);

public:
    WindowsApplication();

public:
    void SetCustomMessageCallback(MessageCallback callback);

protected:
    MessageCallback _messageCallback{};
};

using PlatformApplication = WindowsApplication;

}                               
