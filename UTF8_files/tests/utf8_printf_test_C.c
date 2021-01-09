
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <diskfont/diskfonttag.h>

#include <proto/utf8.h>

#undef Length

#include "../libbase.h"

struct Locale *locale;
uint32 codeset;

#define DefineLib( a ) struct Library * a ## _base; struct a ## IFace *I ## a;

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
	unsigned char n;

	Locale_base = IExec -> OpenLibrary("locale.library", 53);
	if (Locale_base) ILocale = (struct LocaleIFace *)  IExec -> GetInterface( Locale_base, "main", 1, TAG_END );

	Diskfont_base = IExec -> OpenLibrary("diskfont.library", 53);
	if (Diskfont_base) IDiskfont = (struct DiskfontIFace *)  IExec -> GetInterface( Diskfont_base, "main", 1, TAG_END );

	UTF8_base = IExec -> OpenLibrary("UTF8.library", VER);
	if (UTF8_base) IUTF8 = (struct UTF8IFace *)  IExec -> GetInterface( UTF8_base, "main", 1, TAG_END );

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

	return 0;
}


void utf8_dump(unsigned char *utf8)
{
	int pos = 0;
	int len,ret;
	do
	{
		ret = IUTF8->GetGlyph( utf8 + pos, &len) ;
		pos += len;

		printf("%c: %d\n",ret < 256 ? (char) ret : '?' , ret );

	} while ( (len>0) && (ret > 0) );
}

int ami_main(int nargs,char **args)
{
	unsigned char *utf8;
	char *txt;
	int fd;
	char org_txt[] = "dette er en test æøå ÆØÅ ¤!!!";

	if (utf8 = IUTF8->Encode( CHAR_CODES, org_txt, MEMF_PRIVATE ))
	{
		IUTF8->Printf(CHAR_CODES, (unsigned char *) "%s\n", utf8 );

		IExec -> FreeVec(utf8);
	}
}


