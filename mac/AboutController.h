#ifndef __AboutController_H__
#define __AboutController_H__

#import <Cocoa/Cocoa.h>

OSStatus orderWindowFrontAbout(void);
OSStatus initializeCocoaAbout(void);

@interface AboutController : NSWindowController<NSWindowDelegate>
{
    IBOutlet NSWindow *aboutWindow;

    IBOutlet NSTextField * hpLinkID;
    IBOutlet NSTextField* ghLinkID;
}

- (int) showWindow;
+ (AboutController *) aboutController;
@end

#endif //__AboutController_H__
