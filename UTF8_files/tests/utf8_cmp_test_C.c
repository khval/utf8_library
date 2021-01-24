
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
	unsigned char *UTF8;
	ULONG ret;
	char *txt;
	unsigned char *UTF8_str[4];
	unsigned char **PTR;
	unsigned char *UTF8_alphabet;

	// load it from a text in UTF8 format its better, if you have the wrong ISO (in Locale prefs) you see wrong symbols.
	UTF8_alphabet =  UTF8Encode( CHAR_CODES, (char *) "abcdefghijklmnopqrstuvwxyz���ABCDEFGHIJKLMNOPQRSTUVWXYZ���", MEMF_PRIVATE ); 

	UTF8_str[0] = UTF8Encode(CHAR_CODES,"�",MEMF_PRIVATE);
	UTF8_str[1] = UTF8Encode(CHAR_CODES,"�",MEMF_PRIVATE);
	UTF8_str[2] = UTF8Encode(CHAR_CODES,"�",MEMF_PRIVATE);
	UTF8_str[3] = NULL;

	printf("\n 7BIT ASCII / UTF8 \n");

	debug_Cmp(UTF8_alphabet,"alfa","beta");
	debug_Cmp(UTF8_alphabet,"beta","alfa");
	debug_Cmp(UTF8_alphabet,"alfa","alfa");
	debug_Cmp(UTF8_alphabet,"ze","zeta");
	debug_Cmp(UTF8_alphabet,"zeta","ze");

	printf("\n NOT UTF8 \n");

	debug_Cmp(UTF8_alphabet,"�","�");
	debug_Cmp(UTF8_alphabet,"�","�");
	debug_Cmp(UTF8_alphabet,"�","a");

	printf("\n UTF8 \n");

	debug_Cmp(UTF8_alphabet,"z",UTF8_str[0]);  // cmp('z','�')
	debug_Cmp(UTF8_alphabet,UTF8_str[0],UTF8_str[1]);  // cmp('�','�')
	debug_Cmp(UTF8_alphabet,UTF8_str[1],UTF8_str[2]);  // cmp('�','�')
	debug_Cmp(UTF8_alphabet,UTF8_str[2],"a");  // cmp('�','�')

	printf("\n UTF8 lowcase vs uppercase \n");

	debug_Cmp(UTF8_alphabet,"a","A");  
	debug_Cmp(UTF8_alphabet,"Z","z");  

	printf("\n UTF8 casecmp lowcase vs uppercase \n");

	debug_CaseCmp(UTF8_alphabet,"a","A");  
	debug_CaseCmp(UTF8_alphabet,"Z","z");  
	debug_CaseCmp(UTF8_alphabet,"a","b");  
	debug_CaseCmp(UTF8_alphabet,"b","a");  
	debug_CaseCmp(UTF8_alphabet,"A","B");  
	debug_CaseCmp(UTF8_alphabet,"B","A");  

	for (PTR=UTF8_str;*PTR;PTR++)
	{
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

