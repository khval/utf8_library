
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#ifdef __amigaos4__
#include "common4aos4.h"
#endif

#ifdef __amigaos3__
#include "common4aos3.h"
#endif

#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>
#include <proto/utf8.h>

#undef Length

struct Locale *locale;
uint32 codeset;

DefineLib( UTF8 );
DefineLib( Locale );
DefineLib( Diskfont );

ULONG *CHAR_CODES;

int ami_main(int nargs,char **args);

int main(int nargs,char **args)
{
	int ret;
	unsigned char n;

	Locale_base = OpenLibrary("locale.library", 53);
	Diskfont_base = OpenLibrary("diskfont.library", 53);
	UTF8_base = OpenLibrary("UTF8.library", VER);

#ifdef __amigaos4__

	if (Locale_base) ILocale = (struct LocaleIFace *)  GetInterface( Locale_base, "main", 1, TAG_END );
	if (Diskfont_base) IDiskfont = (struct DiskfontIFace *)  GetInterface( Diskfont_base, "main", 1, TAG_END );
	if (UTF8_base) IUTF8 = (struct UTF8IFace *)  GetInterface( UTF8_base, "main", 1, TAG_END );

#endif

	if (
		libIsOpen(Locale) &&
		libIsOpen(Diskfont) &&
		libIsOpen(UTF8))
	{
		locale = OpenLocale(NULL);
		CHAR_CODES = (ULONG *) ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		CloseLocale(locale);
	}
	else
	{
		if (!libIsOpen(Locale))	printf("Failed to open %s v %d\n",  "locale.library", 53 );
		if (!libIsOpen(Diskfont))	printf("Failed to open %s v %d\n",  "diskfont.library", 53 );
		if (!libIsOpen(UTF8))		printf("Failed to open %s v %d\n",  "UF8.library", VER );
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

	// Just a test string in ASCII 7bit => UTF8 (7bit) is TRUE, UTF8 (8bit) != ASCII 7BIT
	UTF8 = UTF8Remove( (unsigned char * ) "This is a test", 4, 5, MEMF_PRIVATE );
	if (UTF8)
	{
		UTF8Printf(CHAR_CODES, (unsigned char *) "%s\n",UTF8);
		FreeVec(UTF8);
	}


	return 0;
}

