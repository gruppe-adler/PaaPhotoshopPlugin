#include <Cocoa/Cocoa.h>
#include "PIDefines.h"
#include "AboutController.h"
#include <string>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

void DoMessageUI(std::string title, std::string message) {
    NSAlert *alert = [[NSAlert alloc]init];
    
    [alert setMessageText: [NSString stringWithCString:title.c_str()
                                              encoding:[NSString defaultCStringEncoding]]];
    [alert setInformativeText:[NSString stringWithCString:message.c_str()
                                                 encoding:[NSString defaultCStringEncoding]]];
    
    [alert setAlertStyle:NSAlertStyleCritical];
    [alert addButtonWithTitle:@"OK"];
    
    [alert runModal];
    
    [alert release];
}

Boolean DoAboutUI ()
{
    OSStatus err = initializeCocoaAbout();
    
    if (!err)
        err = orderWindowFrontAbout();
    else
        err = 0;
    
    return err;
}
