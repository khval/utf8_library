# utf8_library

This library is for AmigaOS4.1 FE

I have open sourced it, so more eyes can look at code, make sure its bug free,
and contribute to it, if there anything might be useful.

It’s under LGPL so if your making changes your supposed to contribute your changes back,
you can distribute library. LGPL was choosen so you distribute with your commercial or game or program.

It can be rewritten for AROS, MorphOS, older AmigaOS 3.x.

I’m rewetting some parts to make it more portable, 
but I don’t wont waste time on ports to other operating systems,
so if anyone is interested the last mile you need to take.

The aim of this library is to provide similar features to standard functions in graphic library,
but this one draws UTF8 text, the library allows you step glyphs one by one, merge and strings etc.

The library also adopts the idea that strings might be shared or private, this is not common for strings.
(It basically allows you to memory protect strings, but this only works for single treaded applications.
(MorphOS and AmigaOS3.x can’t do that.))

Don’t mix malloc strings with utf8 strings using AllocVecTags(...).
