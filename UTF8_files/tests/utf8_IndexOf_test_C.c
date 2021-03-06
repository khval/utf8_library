
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define __USE_INLINE__

#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>
#include <proto/utf8.h>

#undef Length

struct Locale *locale;
uint32 codeset;

#define DefineLib( a ) struct Library * a ## _base; struct a ## IFace *I ## a;

void open_lib( const char *name, int ver , const char *iname, int iver, struct Library **base, struct Interface **interface)
{
	*base = OpenLibrary( name , ver);
	if (*base) *interface = GetInterface( *base,  iname , iver, TAG_END );
}

#define OpenLib( a, ver, iname, iver ) \
	open_lib( #a ".Library",  ver ,  iname, iver, & a ## _base  , (struct a ## IFace *) &I ## a )

#define CloseLIB( name ) \
	if ( I##name)  DropInterface( (struct Interface *) I##name ); \
	if ( name  ## _base) CloseLibrary( name ## _base); \


DefineLib( UTF8 );
DefineLib( Locale );
DefineLib( Diskfont );

ULONG *CHAR_CODES;

int ami_main(int nargs,char **args);

int main(int nargs,char **args)
{
	int ret;

	open_lib( "locale.library", 53 , "main", 1, &Locale_base, (struct Interface **) &ILocale  );
	open_lib( "diskfont.library", 53 , "main", 1, &Diskfont_base, (struct Interface **) &IDiskfont );
	open_lib( "UTF8.library", VER , "main", 1, &UTF8_base, (struct Interface **) &IUTF8 );

	if ((ILocale)&&(IDiskfont)&&(IUTF8))
	{
		locale = OpenLocale(NULL);
		CHAR_CODES = (ULONG *) ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		CloseLocale(locale);
	}
	else
	{
		if (!ILocale)	printf("Failed to open %s v %d\n",  "locale.library", 53 );
		if (!IDiskfont)	printf("Failed to open %s v %d\n",  "diskfont.library", 53 );
		if (!IUTF8)		printf("Failed to open %s v %d\n",  "UF8.library", VER );
	}

	CloseLIB( Diskfont );
	CloseLIB( Locale );
	CloseLIB( UTF8 );

	return ret;
}


int ami_main(int nargs,char **args)
{
	unsigned char *UTF8;
	ULONG ret;
	char *txt;

	// Just a test string in Norwegian ISO-8859-15
	UTF8 = UTF8Encode( CHAR_CODES, (char *) "Hello World ���", MEMF_PRIVATE ); 

	// Remeber 7Bit ASCII == UTF8bit,  UTF8 != 7bit ASCII.

	ret =UTF8IndexOf( UTF8, (unsigned char *) "ello" );
	printf("found at %d\n",ret);

	ret =UTF8IndexOf( UTF8, (unsigned char *) "World" );
	printf("found at %d\n",ret);

	ret =UTF8IndexOf( UTF8, (unsigned char *) "not found" );
	printf("not found returns %d\n",ret);

	if (UTF8) FreeVec(UTF8);

	return 0;
}

