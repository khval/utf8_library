
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define __USE_INLINE__

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
	unsigned char n;

	Locale_base = OpenLibrary("locale.library", 53);
	if (Locale_base) ILocale = (struct LocaleIFace *)  GetInterface( Locale_base, "main", 1, TAG_END );

	Diskfont_base = OpenLibrary("diskfont.library", 53);
	if (Diskfont_base) IDiskfont = (struct DiskfontIFace *)  GetInterface( Diskfont_base, "main", 1, TAG_END );

	UTF8_base = OpenLibrary("UTF8.library", VER);
	if (UTF8_base) IUTF8 = (struct UTF8IFace *)  GetInterface( UTF8_base, "main", 1, TAG_END );

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

	return 0;
}


void utf8_dump(unsigned char *utf8)
{
	int pos = 0;
	int len,ret;
	do
	{
		ret = UTF8GetGlyph( utf8 + pos, &len) ;
		pos += len;

		printf("%d\n",ret);

	} while ( (len>0) && (ret > 0) );
}


int ami_main(int nargs,char **args)
{

	unsigned char *utf8;
	unsigned char *txt;
	int fd;
	char org_txt[] = "dette er en test זרו ֶ״ֵ ₪!!!";


	printf(" %08x, %08x \n",org_txt , CHAR_CODES );

	if (utf8 = UTF8Encode( CHAR_CODES, org_txt, MEMF_PRIVATE ))
	{
		printf("its encoded\n");

		utf8_dump(utf8);
		printf("txt length %d\n",strlen(org_txt));

		printf("utf8 length %d, utf8 size %d\n",UTF8Length( utf8 ),UTF8GetSize( utf8 ) );

		if (txt = UTF8Decode(  CHAR_CODES, utf8, MEMF_PRIVATE ))
		{
			printf("%s\n",txt);
			FreeVec(txt);
		}

		if (fd = open( "ram:utf8.txt", O_CREAT | O_RDWR ))
		{
			write(fd,utf8,strlen(utf8) );
			close(fd);
		}

		FreeVec(utf8);
	}
}


