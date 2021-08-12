#include "PrecompiledHeader.h"

#include "MacOSEngine.h"

#include "../Application.h"

@implementation AppDelegate
-(id)initWithEngine:(std::unique_ptr<tg::Engine>)engine
{
    self = [super init];
    if (self != nil)
    {
        _engine = std::move(engine);
    }

    return self;
}

-(void)applicationWillFinishLaunching:(NSNotification*)aNotification
{
    tg::Application::GetInstance().Initialize(std::move(_engine));
}

-(void)applicationWillTerminate:(NSNotification*)aNotification
{
}

-(void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
    tg::Application::GetInstance().MessageLoop();
}

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}
@end
