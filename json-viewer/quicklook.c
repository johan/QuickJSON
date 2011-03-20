#include "quicklookjson.h"

#define HTML_HEADER __HTML_HEADER__
#define HTML_FOOTER __HTML_FOOTER__

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
