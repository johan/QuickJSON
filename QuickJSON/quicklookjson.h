#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

CFDataRef createHTMLPreview(CFURLRef url);
CFDataRef createDataFromURL(CFURLRef url);
CFDataRef createNFOString(CFDataRef text);
CFDataRef createHTMLData(CFDataRef nfoString);
