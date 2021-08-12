#include "PrecompiledHeader.h"

#include <AppKit/NSAlert.h>

#include "../Application.h"
#include "../Engine.h"

namespace tg
{
namespace
{
    
constexpr NSAlertStyle ConvertMessageBoxIconToNative(MessageBoxIcon messageBoxIcon) noexcept
{
    constexpr NSAlertStyle nativeMessageBoxIcons[] = {
        NSAlertStyle(-1),
        NSAlertStyleInformational,
        NSAlertStyleCritical,
    };
    
    return nativeMessageBoxIcons[static_cast<int32_t>(messageBoxIcon)];
}

void OnHandleMessage(NSEvent* event)
{
    NSEventType eventType = [event type];
    switch (eventType)
    {
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp:
        break;
        
    default:
        [NSApp sendEvent:event];
        break;
    }
}

}

void Application::ShowMessageBox(const char8_t* title, const char8_t* message, MessageBoxIcon messageBoxIcon)
{
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:reinterpret_cast<const char*>(title)]];
    [alert setInformativeText:[NSString stringWithUTF8String:reinterpret_cast<const char*>(message)]];
    
    if (messageBoxIcon != MessageBoxIcon::No)
    {
        [alert setAlertStyle:ConvertMessageBoxIconToNative(messageBoxIcon)];
    }

    [alert runModal];
}
    
void Application::MessageLoop()
{
    NSEvent* event = nil;
    while (true)
    {
        while((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                          untilDate:nil
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:YES]) != nil)
        {
            OnHandleMessage(event);
        }
        
        m_engine->Update();
    }
}

}
