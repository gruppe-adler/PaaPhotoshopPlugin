//
//  messagebox.m
//  PaaFormat
//
//  Created by toor on 12.07.20.
//

#include "../common/MessageBox.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

void DisplayMessage(std::string title, std::string message) {
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
