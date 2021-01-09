
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
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
	*base = IExec -> OpenLibrary( name , ver);
	if (*base) *interface = IExec -> GetInterface( *base,  iname , iver, TAG_END );
}

#define OpenLib( a, ver, iname, iver ) \
	open_lib( #a ".Library",  ver ,  iname, iver, & a ## _base  , (struct a ## IFace *) &I ## a )

#define CloseLIB( name ) \
	if ( I##name) IExec -> DropInterface( (struct Interface *) I##name ); \
	if ( name  ## _base) IExec -> CloseLibrary( name ## _base); \


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
		locale = ILocale->OpenLocale(NULL);
		CHAR_CODES = (ULONG *) IDiskfont -> ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		ILocale->CloseLocale(locale);
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
	unsigned char *UTF8_ret;
	char *txt;

	// Just a test string in Norwegian ISO-8859-15
	UTF8 = IUTF8 -> Encode( CHAR_CODES, (char *) "Hello World זרו", MEMF_PRIVATE ); 

	if (UTF8_ret=IUTF8 -> Left( UTF8, 5, MEMF_PRIVATE ))
	{
		IUTF8->Printf( CHAR_CODES, (unsigned char *) "%s\n", UTF8_ret  );
		IExec->FreeVec(UTF8_ret);
	}

	if (UTF8_ret=IUTF8 -> Right( UTF8, 3, MEMF_PRIVATE ))
	{
		IUTF8->Printf( CHAR_CODES, (unsigned char *) "%s\n", UTF8_ret  );
		IExec->FreeVec(UTF8_ret);
	}

	// bad length, test.

	if (UTF8_ret=IUTF8 -> Left( UTF8, 1000, MEMF_PRIVATE ))
	{
		IUTF8->Printf( CHAR_CODES, (unsigned char *) "%s\n", UTF8_ret  );
		IExec->FreeVec(UTF8_ret);
	}

	if (UTF8_ret=IUTF8 -> Right( UTF8, 1000, MEMF_PRIVATE ))
	{
		IUTF8->Printf( CHAR_CODES, (unsigned char *) "%s\n", UTF8_ret  );
		IExec->FreeVec(UTF8_ret);
	}

	if (UTF8) IExec->FreeVec(UTF8);

	return 0;
}

