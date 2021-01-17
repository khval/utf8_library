
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
#include <inline4/utf8.h>

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

	return 0;
}

int ami_main(int nargs,char **args)
{
	unsigned char *UTF8,*NUTF8;
	unsigned char *ALPHABET_UTF8;

	ULONG ret;
	char *txt;

	ALPHABET_UTF8 =  UTF8Encode( CHAR_CODES, (char *) "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ", MEMF_PRIVATE ); 

	// Just a test string in Norwegian ISO-8859-15
	if (UTF8 = UTF8Trim( (unsigned char *) "  AAAAA ", MEMF_PRIVATE ))
	{
		UTF8Printf(CHAR_CODES,(unsigned char *) "Original: -%s-\n",UTF8);

		UTF8ModRemove( &UTF8,1,3, MEMF_PRIVATE);
		UTF8Printf(CHAR_CODES,(unsigned char *) "ModRemove: -%s-\n",UTF8);

		UTF8ModInsert( &UTF8,1,"MIG", MEMF_PRIVATE);
		UTF8Printf(CHAR_CODES,(unsigned char *) "ModInsert: -%s-\n",UTF8);

		if (NUTF8 = UTF8Merge(MEMF_PRIVATE, UTF8," ","CAN"," ","DO"," ","IT",0))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Merge: -%s-\n",UTF8);

		UTF8ModRight( &UTF8,5,MEMF_PRIVATE);

		if (NUTF8 = UTF8Merge(MEMF_PRIVATE, "AMIGA JUST ",UTF8,0))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "ModRight+Merge: -%s-\n",UTF8);

		UTF8ModLeft(&UTF8,5,MEMF_PRIVATE);
		UTF8Printf(CHAR_CODES,(unsigned char *) "ModLeft: -%s-\n",UTF8);

		UTF8ModToLower( ALPHABET_UTF8, &UTF8, MEMF_PRIVATE);
		UTF8Printf(CHAR_CODES,(unsigned char *) "ModToLower: -%s-\n",UTF8);

		UTF8ModToUpper( ALPHABET_UTF8, &UTF8, MEMF_PRIVATE);
		UTF8Printf(CHAR_CODES,(unsigned char *) "ModToUpper: -%s-\n",UTF8);


		FreeVec(UTF8);
	}

	return 0;
}
