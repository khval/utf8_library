
#define __USE_INLINE__

#define DefineLib( a ) struct Library * a ## _base; 

#define CloseLIB( name ) \
	if ( name  ## _base) CloseLibrary( name ## _base); \

#define libIsOpen(name) (name ## _base)

#AmigaOS3 does have private memory.

#define MEMF_PRIVATE MEMF_FAST

#Becouse AmigaOS3 don't have private memory, it can only hint at shared memory, using Public.

#define MEMF_SHARED MEMF_PUBLIC | MEMF_FAST

