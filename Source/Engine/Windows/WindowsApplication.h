#pragma once

#include "Core/NonCopyable.h"

typedef struct tagMSG MSG;

namespace tg
{

class WindowsApplication :
    private NonCopyable
{
/**@section Type */
public:
    using MessageCallback = void(*)(const MSG&);

/**@section Constructor */
public:
    WindowsApplication();

/**@section Method */
public:
    void SetCustomMessageCallback(MessageCallback callback);

/**@section Variable */
protected:
    MessageCallback m_messageCallback{};
};

using PlatformApplication = WindowsApplication;

}                               
