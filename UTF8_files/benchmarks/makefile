
VER=-DVER=6

files = utf8_get_glyph.exe

inc = -I../include -I./

all:		$(files)

%.exe:	%.c
		gcc  -Wall -O3  $(INC) $(DEBUG) $(VER)  -D__USE_INLINE__ -D__no_stdlib__ $(@:.exe=.c) -o $@

clean:
		delete #?.exe
