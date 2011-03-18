#include "quicklookjson.h"

#define HTML_HEADER "<!DOCTYPE html><html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8'><style>body{font-family:sans-serif;}.prop{font-weight:700;}.null{color:red;}.bool,.num{color:blue;}.string{color:green;white-space:pre-wrap;}#error{-moz-border-radius:8px;border:1px solid #970000;background-color:#F7E8E8;margin:.5em;padding:.5em;}.errormessage{font-family:monospace;}#json{white-space:pre-wrap;font-family:monospace;font-size:1.1em;}h1{font-size:1.2em;}.callback{font-family:monospace;color:#A52A2A;}.folded > .content{display:none;}.folded.array:after{content:'[\002026 ]';/* [...] */}.folded.obj:after{content:'{\002026 }';/* {...} */}.folded{cursor:se-resize;}.unfolded{cursor:nw-resize;}</style><script></script></head><body _onload='init()'><script id='json' type='application/json'>"
#define HTML_FOOTER "</script></body></html>"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: quicklookjson <json file>\n");
    return 1;
  }
  CFStringRef path = CFStringCreateWithCString(NULL, argv[1], kCFStringEncodingUTF8);
  CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
  CFRelease(path);
  CFDataRef text = createDataFromURL(url);
  CFRelease(url);
  if (!text) {
    printf("File not found.\n");
    return 2;
  }
  CFDataRef result = text;
  CFRelease(text);
  printf("%s", CFDataGetBytePtr(result));
  return 0;
}

CFDataRef createHTMLPreview(CFURLRef url)
{
  CFDataRef text = createDataFromURL(url);
  if (!text) return NULL;
  CFDataRef result = createHTMLData(text);
  CFRelease(text);
  return result;
}

CFDataRef createDataFromURL(CFURLRef url)
{
  SInt32 errorCode = 0;

  CFDataRef fileContent;
  CFDictionaryRef dict;
  CFArrayRef arr = CFArrayCreate(NULL, NULL, 0, NULL);

  Boolean success = CFURLCreateDataAndPropertiesFromResource (NULL,
                                url,
                                &fileContent,
                                &dict,
                                arr,
                                &errorCode);
  CFRelease(arr);
  CFRelease(dict);

  if (!success) {
    return NULL;
  }

  return fileContent;
}


CFDataRef createHTMLData(CFDataRef json)
{
  CFMutableDataRef html = CFDataCreateMutable(NULL, 0);

  CFDataAppendBytes(html, (const UInt8*) HTML_HEADER, sizeof(HTML_HEADER));
  CFDataAppendBytes(html, CFDataGetBytePtr(json), CFDataGetLength(json));
  CFDataAppendBytes(html, (const UInt8*) HTML_FOOTER, sizeof(HTML_FOOTER));

  return html;
}
