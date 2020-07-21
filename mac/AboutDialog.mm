//
//  AboutDialog.cpp
//  PaaFormat
//
//  Created by toor on 21.07.20.
//

#include "AboutDialog.h"

void DoAboutBox() {
    NSAlert* const alert = [[NSAlert alloc] init];
    
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"www.gruppe-adler.de"];
    
    [alert setMessageText:@"About PAA Format Plugin"];
    [alert setInformativeText:@"PAA Photoshop Plugin by Gruppe Adler"];
    [alert setAlertStyle:NSAlertStyleInformational];
    
    [alert setIcon:[NSImage imageNamed:@"Adler"]];
    
    const NSModalResponse buttonPressed = [alert runModal];
    
    if(buttonPressed == NSAlertFirstButtonReturn) {
        //PSFileList->BrowseUrl("www.gruppe-adler.de");
    }
    
    [alert release];
}
