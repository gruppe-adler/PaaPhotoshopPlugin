#include <Cocoa/Cocoa.h>
#include "PIDefines.h"
#include "AboutController.h"

Boolean DoAboutUI ()
{
    OSStatus err = initializeCocoaHistory();
    
    if (!err)
        err = orderWindowFrontHistory();
    else
        err = 0;
    
    return err;
}
