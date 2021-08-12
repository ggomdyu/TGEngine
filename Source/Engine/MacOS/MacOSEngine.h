#pragma once

#include <AppKit/NSApplication.h>
#include <objc/NSObject.h>

#include "../Application.h"

namespace tg
{

class Engine;

}

@interface AppDelegate : NSObject<NSApplicationDelegate>
{
    std::unique_ptr<tg::Engine> _engine;
}
-(id)initWithEngine:(std::unique_ptr<tg::Engine>)engine;
@end

#define TGON_ENGINE(className)\
    int main(int argc, const char* argv[])\
    {\
        @autoreleasepool\
        {\
            id sharedApplication = [NSApplication sharedApplication];\
            id appDelegate = [[AppDelegate alloc] initWithEngine:std::make_unique<className>()];\
            [sharedApplication setDelegate:appDelegate];\
            [sharedApplication setActivationPolicy:NSApplicationActivationPolicyRegular];\
            [sharedApplication activateIgnoringOtherApps:YES];\
            [sharedApplication run];\
        }\
        return 0;\
    }
