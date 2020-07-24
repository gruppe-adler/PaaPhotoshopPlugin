#import "AboutController.h"

AboutController *gAboutController = NULL;

@implementation AboutController

+ (AboutController *) aboutController
{
    return gAboutController;
}


- (id) init
{
    self = [super init];
	
    NSBundle * plugin = [NSBundle bundleForClass:[self class]];
	
    if (![plugin loadNibNamed:@"AboutDialog" owner:self topLevelObjects:nil])
    {
        return 0;
    }
    
    gAboutController = self;
    
    // Set links
    [self updateControl:hpLinkID withHyperlink:@"https://www.gruppe-adler.de"];
    [self updateControl:ghLinkID withHyperlink:@"https://github.com/gruppe-adler/PaaPhotoshopPlugin/issues"];
    
    return self;
}

- (int) showWindow
{
    [aboutWindow makeKeyAndOrderFront:nil];
    int b = [[NSApplication sharedApplication] runModalForWindow:aboutWindow];
    [aboutWindow orderOut:self];
    return b;
}

-(void)windowWillClose:(NSNotification *)notification
{
    [[NSApplication sharedApplication] stopModalWithCode:0];
}

-(void)updateControl:(NSTextField*)control withHyperlink:(NSString*)strURL
{
    [control setAllowsEditingTextAttributes: YES];
    [control setSelectable: YES];
    
    NSURL* url = [NSURL URLWithString:strURL];
    
    NSAttributedString* attrString = [control attributedStringValue];
    
    NSMutableAttributedString* attr = [[NSMutableAttributedString alloc] initWithAttributedString:attrString];
    NSRange range = NSMakeRange(0, [attr length]);
    
    [attr addAttribute:NSLinkAttributeName value:url range:range];
    [attr addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
    [attr addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSUnderlineStyleSingle] range:range];
    
    [control setAttributedStringValue:attr];
}
@end

/* Carbon entry point and C-callable wrapper functions*/
OSStatus initializeCocoaAbout()
{
    [[AboutController alloc] init];
    return noErr;
}

OSStatus orderWindowFrontAbout(void)
{
    int okPressed = [[AboutController aboutController] showWindow];
    return okPressed;
}
