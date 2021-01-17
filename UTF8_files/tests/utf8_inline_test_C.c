
#define __USE_INLINE__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>

#include <inline4/utf8.h>

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
	if ( I##name) DropInterface( (struct Interface *) I##name ); \
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
	unsigned char *UTF8,*NUTF8;
	unsigned char *ALPHABET_UTF8;

	ULONG ret;
	char *txt;

	ALPHABET_UTF8 =  Encode( CHAR_CODES, (char *) "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ", MEMF_PRIVATE ); 

	// Just a test string in Norwegian ISO-8859-15
	if (UTF8 = UTF8Trim( (unsigned char *) "  AAAAA ", MEMF_PRIVATE ))
	{
		UTF8Printf(CHAR_CODES,(unsigned char *) "Original: -%s-\n",UTF8);

		if (NUTF8 = UTF8Remove(UTF8,1,3, MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Remove: -%s-\n",UTF8);

		if (NUTF8 = UTF8Insert(UTF8,1,"MIG", MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Insert: -%s-\n",UTF8);

		if (NUTF8 = UTF8Merge(MEMF_PRIVATE, UTF8," ","CAN"," ","DO"," ","IT",0))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Merge: -%s-\n",UTF8);

		if (NUTF8 =UTF8Right(UTF8,5,MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		if (NUTF8 = UTF8Merge(MEMF_PRIVATE, "AMIGA JUST ",UTF8,0))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Right+Merge: -%s-\n",UTF8);

		if (NUTF8 =UTF8Left(UTF8,5,MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}

		UTF8Printf(CHAR_CODES,(unsigned char *) "Left: -%s-\n",UTF8);

		if (NUTF8 = UTF8ToLower( UTF8,ALPHABET_UTF8, MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}
		UTF8Printf(CHAR_CODES,(unsigned char *) "ToLower: -%s-\n",UTF8);

		if (NUTF8 = UTF8ToUpper( UTF8,ALPHABET_UTF8, MEMF_PRIVATE))
		{
			FreeVec(UTF8);
			UTF8 = NUTF8;
		}
		UTF8Printf(CHAR_CODES,(unsigned char *) "ToUpper: -%s-\n",UTF8);


		FreeVec(UTF8);
	}

	return 0;
}

