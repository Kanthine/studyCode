#import "AAPLAppDelegate.h"

@implementation AAPLAppDelegate

#if TARGET_MACOS
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}
#endif

@end
