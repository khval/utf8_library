
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#if _MSC_VER
#include "not_amiga.h"
#include "../UTF8_vectors.h"
#endif // _MSC_VER


#ifdef __amigaos4__
#include "common4aos4.h"
#endif

#ifdef __amigaos3__
#include "common4aos3.h"
#endif

#ifdef __amigaos__
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>
#include <proto/utf8.h>
#endif

struct Locale *locale;
uint32 codeset;

DefineLib( UTF8 );
DefineLib( Locale );
DefineLib( Diskfont );

ULONG *CHAR_CODES;

int ami_main(int nargs,char **args);

#ifndef _MSC_VER

int main(int nargs,char **args)
{
	int ret;

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

#else

// windows, test 

int main(int nargs, char **args)
{
	int ret;
	ret = ami_main(nargs, args);
	return ret;
}

#endif

void debug_Cmp(unsigned char *UTF8_alphabet,unsigned char *UTF8_one,  unsigned char *UTF8_two)
{
	int ret;
	ret = UTF8Cmp( UTF8_alphabet, UTF8_one,  UTF8_two);
	UTF8Printf(CHAR_CODES, "CMP(%s,%s)==%d\n", UTF8_one , UTF8_two, ret);	
}

void debug_CaseCmp(unsigned char *UTF8_alphabet,unsigned char *UTF8_one,  unsigned char *UTF8_two)
{
	int ret;
	ret = UTF8CaseCmp( UTF8_alphabet, UTF8_one,  UTF8_two);
	UTF8Printf(CHAR_CODES, "CaseCmp(%s,%s)==%d\n", UTF8_one , UTF8_two, ret);	
}

int ami_main(int nargs,char **args)
{
//	unsigned char *UTF8;
//	ULONG ret;
//	char *txt;
	unsigned char *UTF8_strings[4];
	unsigned char **PTR;
	unsigned char *UTF8_alphabet;

	// load it from a text in UTF8 format its better, if you have the wrong ISO (in Locale prefs) you see wrong symbols.
	UTF8_alphabet =  UTF8Encode( CHAR_CODES, (char *) "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ", MEMF_PRIVATE ); 

	UTF8_strings[0] = UTF8Encode(CHAR_CODES,"æ",MEMF_PRIVATE);
	UTF8_strings[1] = UTF8Encode(CHAR_CODES,"ø",MEMF_PRIVATE);
	UTF8_strings[2] = UTF8Encode(CHAR_CODES,"å",MEMF_PRIVATE);
	UTF8_strings[3] = NULL;


#if 1
	printf("\n 7BIT ASCII / UTF8 \n");

	debug_Cmp(UTF8_alphabet,(unsigned char *) "alfa",(unsigned char *) "beta");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "beta",(unsigned char *) "alfa");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "alfa",(unsigned char *) "alfa");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "ze",(unsigned char *) "zeta");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "zeta",(unsigned char *) "ze");

#endif

	printf("\n NOT UTF8 \n");

	debug_Cmp(UTF8_alphabet,(unsigned char *) "æ",(unsigned char *) "ø");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "ø",(unsigned char *) "å");
	debug_Cmp(UTF8_alphabet,(unsigned char *) "å",(unsigned char *) "a");

	printf("\n UTF8 \n");

	debug_Cmp(UTF8_alphabet,(unsigned char *) "z",UTF8_strings[0]);  // cmp('z','æ')
	debug_Cmp(UTF8_alphabet,UTF8_strings[0],UTF8_strings[1]);  // cmp('æ','ø')
	debug_Cmp(UTF8_alphabet,UTF8_strings[1],UTF8_strings[2]);  // cmp('ø','å')
	debug_Cmp(UTF8_alphabet,UTF8_strings[2],(unsigned char *) "a");  // cmp('å','æ')

	printf("\n UTF8 lowcase vs uppercase \n");

	debug_Cmp(UTF8_alphabet,(unsigned char *) "a",(unsigned char *) "A");  
	debug_Cmp(UTF8_alphabet,(unsigned char *) "Z",(unsigned char *) "z");  

	printf("\n UTF8 casecmp lowcase vs uppercase \n");

	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "a",(unsigned char *) "A");  
	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "Z",(unsigned char *) "z");  
	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "a",(unsigned char *) "b");  
	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "b",(unsigned char *) "a");  
	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "A",(unsigned char *) "B");  
	debug_CaseCmp(UTF8_alphabet, (unsigned char *) "B",(unsigned char *) "A");  

	printf("\n Free UTF8 strings \n");

	for (PTR=UTF8_strings;*PTR;PTR++)
	{
		printf(" FreeVec(%08x)\n", (unsigned int) *PTR);
		FreeVec(*PTR);
		*PTR=NULL;
	}
	

	if (UTF8_alphabet) 
	{
		FreeVec(UTF8_alphabet);
		UTF8_alphabet = NULL;
	}

	return 0;
}

