
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>

#include <proto/utf8.h>
#include <c++/utf8.h>

#undef Length

struct Locale *locale;
uint32 codeset;

#define DefineLib( a ) struct Library * a ## _base; struct a ## IFace *I ## a;

#define CloseLIB( name ) \
	if ( I##name) IExec -> DropInterface( (struct Interface *) I##name ); \
	if ( name  ## _base) IExec -> CloseLibrary( name ## _base); \

// ------------------------------------------------------------------------------
//   Create UTF8 object whit its commands, this is all you need!
// ------------------------------------------------------------------------------
UTF8Lib UTF8;

// OLD C stuff
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

	if ((ILocale)&&(IDiskfont))
	{
		locale = ILocale->OpenLocale(NULL);
		CHAR_CODES = (ULONG *) IDiskfont -> ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		ILocale->CloseLocale(locale);
	}
	else
	{
		printf("some libs did not open\n");
	}

	CloseLIB( Diskfont );
	CloseLIB( Locale );


	return 0;
}


void utf8_dump(unsigned char *utf8)
{
	int pos = 0;
	int len,ret;
	do
	{
		ret = UTF8.GetGlyph( utf8 + pos, &len) ;
		pos += len;

		printf("%d\n",ret);

	} while ( (len>0) && (ret > 0) );
}


int ami_main(int nargs,char **args)
{
	unsigned char *utf8;
	char *txt;
	int fd;
	char org_txt[] = "dette er en test æøå ÆØÅ ¤!!!";

	printf(" %08x, %08x \n",org_txt , CHAR_CODES );

	if (utf8 = UTF8.Encode( CHAR_CODES, org_txt ))
	{
		printf("its encoded\n");

		utf8_dump(utf8);

		if (txt = UTF8.Decode( CHAR_CODES, utf8 ))
		{
			printf("%s\n",txt);
			IExec -> FreeVec(txt);
		}

		if (fd = open( "ram:utf8.txt", O_CREAT | O_RDWR ))
		{
			write(fd,utf8,strlen( (const char *) utf8) );
			close(fd);
		}

		IExec -> FreeVec(utf8);
	}
}


