
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
	open_lib( "UTF8.library",  VER , "main", 1, &UTF8_base, (struct Interface **) &IUTF8 );

	if ((ILocale)&&(IDiskfont)&&(IUTF8))
	{
		locale = OpenLocale(NULL);
		CHAR_CODES = (ULONG *) ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		CloseLocale(locale);
	}
	else
	{
		if (!ILocale)	printf("Failed to open Local %s %d\n",  "locale.library", 53 );
		if (!IDiskfont)	printf("Failed to open Local %s %d\n",  "diskfont.library", 53 );
		if (!IUTF8)		printf("Failed to open Local %s %d\n",  "UF8.library", VER );
	}

	CloseLIB( Diskfont );
	CloseLIB( Locale );
	CloseLIB( UTF8 );

	return 0;
}



int ami_main(int nargs,char **args)
{
	unsigned char *buffer;
	ULONG ret;
	int n,pos,len;
	int size;
	char *c,*ascii;

	printf("\nCreate utf8 string whit some test values\n");

	size = 0;
	for (n=0;n<31;n++)
	{
		len = UTF8EstimateByteSize( 1<<n );
		size += len;
	}
	printf("size needed %d\n", size);

	buffer = (unsigned char *) AllocVec(size+1, MEMF_CLEAR );

	// create a bad utf8 string
	pos = 0;
	for (n=0;n<31;n++)
	{
		pos += UTF8SetGlyph( 1<<n, buffer + pos );
	}
	buffer[pos] = 0;

	for ( pos = 0; pos < UTF8Length( buffer )  ; pos++)
	{
		ret = UTF8GetGlyphAt( buffer, pos, &len) ;
		printf("%02x: %d\n",len, ret);
		if ((ret ==0) || (len == 0)) break;
	} 

	printf("UTF8 string %d long\n", UTF8Length(buffer) );

	printf("ASCII hex dump\n");
	n = 0;
	ascii = (char *) UTF8Decode( CHAR_CODES,  buffer,  MEMF_PRIVATE );
	if (ascii)
	{
		for (c = ascii;*c;c++)
		{
			n++;
			printf("%02X ",*c);
		}
		printf("\n");
		FreeVec(ascii);
	}

	printf("char string is %d long\n", n);

	FreeVec(buffer);

}

