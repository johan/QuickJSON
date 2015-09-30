QuickJSON
=========

A QuickLook plugin to pretty-print JSON in the terse comma-first style, e g:

```json
{ "name": "flux"
, "description": "An application architecture based on a unidirectional data flow"
, "version": "2.1.1"
, "homepage": "http://facebook.github.io/flux/"
, "repository":
  { "type": "git"
  , "url": "https://github.com/facebook/flux.git"
  }
, "authors":
  [ "Facebook"
  , "Jing Chen <jingc@fb.com>"
  , "Bill Fisher <fisherwebdev@gmail.com>"
  , "Paul O'Shannessy <paul@oshanessy.com>"
  ]
}
```

### Installing

To install a pre-built version for osx 10.6+,
download and unzip
[QuickJSON.qlgenerator.zip](https://github.com/downloads/johan/QuickJSON/QuickJSON.qlgenerator.zip)
into `/Library/QuickLook/` (to install system-globally),
or into `~/Library/QuickLook/` (to install for yourself only).
You may need to create this directory, if it does not exist.

To get sweet copy/paste functionality while in QuickLook, also run:

    defaults write com.apple.finder QLEnableTextSelection -bool true

from a Terminal window,
and to refresh installed plugins afterwards:

    qlmanage -r
