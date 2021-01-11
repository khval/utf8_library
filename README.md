# utf8_library

This library for AmigaOS4.1 FE

it can be rewritten for AROS, MorphOS, older AmigaOS 3.x,

The aim of this library is to provide similar features to standard functions in graphic library,
but this one draws UTF8 text, the library allows you step glyphs one by one, merge and strings etc.

The library also adopts the idea that strings might be shared or private, this not common for strings.
(It basically allows you memory protected strings, but this only works for single treaded applications.)
Don’t mix malloc strings with utc8 strings using allocvec.
