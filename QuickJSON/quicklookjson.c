#include "quicklookjson.h"

#define HTML_HEADER "<!DOCTYPE html><html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8'><style>body{font-family:sans-serif;}.prop{font-weight:700;}.null{color:red;}.bool,.num{color:blue;}.string{color:green;white-space:pre-wrap;}#error{-moz-border-radius:8px;border:1px solid #970000;background-color:#F7E8E8;margin:.5em;padding:.5em;}.errormessage{font-family:monospace;}#json{white-space:pre-wrap;font-family:monospace;font-size:1.1em;}h1{font-size:1.2em;}.callback{font-family:monospace;color:#A52A2A;}.folded > .content{display:none;}.folded.array:after{content:'[\002026 ]';/* [...] */}.folded.obj:after{content:'{\002026 }';/* {...} */}.folded{cursor:se-resize;}.unfolded{cursor:nw-resize;}</style><script>var JSONFormatter=(function(){var toString=Object.prototype.toString,re=/^[\\s\\u200B\\uFEFF]*([\\w$\\[\\]\\.]+)[\\s\\u200B\\uFEFF]*\\([\\s\\u200B\\uFEFF]*([\\[{][\\s\\S]*[\\]}])[\\s\\u200B\\uFEFF]*\\)([\\s\\u200B\\uFEFF;]*)$/m;function detectJSONP(s){var js=s,cb='',se='',match;if((match=re.exec(s))&&4===match.length){cb=match[1];js=match[2];se=match[3].replace(/[^;]+/g,'')}try{return wrapJSONP(JSON.parse(js),cb,se)}catch(e){return error(e,s)}}function wrapJSONP(val,callback,semicolon){var output='<span id=json>'+value(val,callback?'':null,callback&&'<br\\n/>')+'</span>';if(callback)output='<span class=callback>'+callback+'(</span>'+output+'<span class=callback>)'+semicolon+'</span>';return output}function isArray(obj){return'[object Array]'===toString.call(obj)}function span(value,className){return'<span class=\\''+className+'\\'>'+html(value)+'</span>'}function error(e,data){return'<div id=error>Error parsing JSON: '+e+'</div><h1>Content:</h1>'+'<span id=json>'+html(data)+'</span>'}function html(s,isAttribute){if(s==null)return'';s=(s+'').replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');return isAttribute?s.replace(/'/g,'&apos;'):s}var js=JSON.stringify('\\b\\f\\n\\r\\t').length===12?function saneJSEscaper(s,noQuotes){s=html(JSON.stringify(s).slice(1,-1));return noQuotes?s:'&quot;'+s+'&quot;'}:function insaneEscaper(s,noQuotes){var had={'\\b':'b','\\f':'f','\\r':'r','\\n':'n','\\t':'t'},ws;for(ws in had)if(-1===s.indexOf(ws))delete had[ws];s=JSON.stringify(s).slice(1,-1);for(ws in had)s=s.replace(new RegExp('\\\\\\\\u000'+(ws.charCodeAt().toString(16)),'ig'),'\\\\'+had[ws]);s=html(s);return noQuotes?s:'&quot;'+s+'&quot;'};function value(v,indent,nl){var output;switch(typeof v){case'boolean':output=span(v,'bool');break;case'number':output=span(v,'num');break;case'string':if(/^(\\w+):\\/\\/[^\\s]+$/i.test(v)){output='&quot;<a href=\\''+html(v,!!'attribute')+'\\'>'+js(v,1)+'</a>&quot;'}else{output='<span class=string>'+js(v)+'</span>'}break;case'object':if(null===v){output=span('null','null')}else{indent=indent==null?'':indent+'&nbsp; ';if(isArray(v)){output=array(v,indent,nl)}else{output=object(v,indent,nl)}}break}return output}function object(obj,indent,nl){var output='';for(var key in obj){if(output)output+='<br\\n/>'+indent+', ';output+='<span class=prop>'+js(key)+'</span>: '+value(obj[key],indent,'<br\\n/>')}if(!output)return'{}';return'<span class=\\'unfolded obj\\'><span class=content>'+(nl?nl+indent:'')+'{ '+output+'<br\\n/>'+indent+'}</span></span>'}function array(a,indent,nl){for(var i=0,output='';i<a.length;i++){if(output)output+='<br\\n/>'+indent+', ';output+=value(a[i],indent,'')}if(!output)return'[]';return'<span class=\\'unfolded array\\'><span class=content>'+(nl?nl+indent:'')+'[ '+output+'<br\\n/>'+indent+']</span></span>'}return function JSONFormatter(s,url){return detectJSONP(s,url)}})();function init(){var node=document.getElementById('json'),json=node.textContent;document.body.innerHTML=JSONFormatter(json);document.addEventListener('click',function folding(e){var elem=e.target,is;do{if(/^a$/i.test(elem.nodeName))return;is=elem.className||''}while(!/\\b(un)?folded /.test(is)&&(elem=elem.parentNode));if(elem)elem.className=/unfolded /.test(is)?is.replace('unfolded ','folded '):is.replace('folded ','unfolded ')},false)}</script></head><body onload='init()'><script id='json' type='application/json'>"
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
