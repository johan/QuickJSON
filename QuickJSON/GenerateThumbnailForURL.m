#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#include <WebKit/WebKit.h>

#include "quicklookjson.h"

#define MIN_SIZE     32
#define THUMB_W      490.0
#define THUMB_H      800.0
#define THUMB_ASPECT (THUMB_W / THUMB_H)

OSStatus
GenerateThumbnailForURL( void *thisInterface
                       , QLThumbnailRequestRef thumbnail
                       , CFURLRef url
                       , CFStringRef contentTypeUTI
                       , CFDictionaryRef options
                       , CGSize maxSize
                       )
{
  if (maxSize.width < MIN_SIZE || maxSize.height < MIN_SIZE)
    return noErr;

  NSAutoreleasePool
              *pool = [[NSAutoreleasePool alloc] init];

  NSRect renderRect = NSMakeRect(0.0, 0.0, THUMB_W, THUMB_H);
  float scale       = maxSize.height / THUMB_H;
  NSSize scaleSize  = NSMakeSize(scale, scale);
  CGSize thumbSize  = NSSizeToCGSize(NSMakeSize((maxSize.width * THUMB_ASPECT)
                                               , maxSize.height));
  CFDataRef dataRef = createHTMLPreview(url);
  NSData *data      = (id)dataRef;

  WebView* webView  = [[WebView alloc] initWithFrame:renderRect];
  [webView scaleUnitSquareToSize:scaleSize];
  [[[webView mainFrame] frameView] setAllowsScrolling:NO];

  [[webView mainFrame] loadData:data MIMEType:@"text/html"
                       textEncodingName:@"UTF-8" baseURL:nil];

  while ([webView isLoading]) {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
  }

  // Get a context to render into
  CGContextRef context =
    QLThumbnailRequestCreateContext(thumbnail, thumbSize, false, NULL);

  if (context != NULL) {
    NSGraphicsContext* nsContext =
      [NSGraphicsContext
        graphicsContextWithGraphicsPort:(void *)context
        flipped:[webView isFlipped]];

    [webView displayRectIgnoringOpacity:[webView bounds]
             inContext:nsContext];

    QLThumbnailRequestFlushContext(thumbnail, context);

    CFRelease(context);
  }

  CFRelease(data);

  [pool release];

  return noErr;
}

void CancelThumbnailGeneration( void* thisInterface
                              , QLThumbnailRequestRef thumbnail)
{
  // implement only if supported
}
