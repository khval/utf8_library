/*
    UTF8.library
    Copyright (C) 2020  Kjetil Hvalstrand

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>


struct Locale *locale;
uint32 codeset;

struct Library *locale_base;
struct LocaleIFace *ILocale;

struct Library *diskfont_base;
struct DiskfontIFace *IDiskFont;

ULONG *CHAR_CODES;

#include "UTF8.i"

int ami_main(int nargs,char **args);

int main(int nargs,char **args)
{
	int ret;
	unsigned char n;

	locale_base = IExec -> OpenLibrary("locale.library", 53);
	if (locale_base) ILocale = (struct LocaleIFace *)  IExec -> GetInterface( locale_base, "main", 1, TAG_END );

	diskfont_base = IExec -> OpenLibrary("diskfont.library", 53);
	if (diskfont_base) IDiskFont = (struct DiskfontIFace *)  IExec -> GetInterface( diskfont_base, "main", 1, TAG_END );

	if ((ILocale)&&(IDiskFont))
	{
		locale = ILocale->OpenLocale(NULL);

		CHAR_CODES = (ULONG *) IDiskFont -> ObtainCharsetInfo(DFCS_NUMBER, locale -> loc_CodeSet , DFCS_MAPTABLE);

/*
		if (CHAR_CODES)
		{
			for ( n=0;n<255;n++)
			{
				if (n != CHAR_CODES[n]  )
				{
					printf("%d: '%c' %d\n",n,n, CHAR_CODES[n] );
				}
			}
		}
*/

		ret = ami_main(nargs,args);

		ILocale->CloseLocale(locale);
	}
	else
	{
		printf("some libs did not open\n");
	}


	if (IDiskFont) IExec -> DropInterface( (struct Interface *) IDiskFont );
	if (diskfont_base) IExec -> CloseLibrary(diskfont_base);

	if (ILocale) IExec -> DropInterface( (struct Interface *) ILocale );
	if (locale_base) IExec -> CloseLibrary(locale_base);

	return 0;
}


void utf8_dump(unsigned char *utf8)
{
	int pos = 0;
	int len,ret;
	do
	{
		ret = utf8_get_glyph( utf8 + pos, &len) ;
		pos += len;

		printf("%d\n",ret);

	} while ( (len>0) && (ret > 0) );
}


int ami_main(int nargs,char **args)
{

	unsigned char *utf8;
	unsigned char *txt;
	int fd;
	char org_txt[] = "dette er en test æøå ÆØÅ ¤!!!";

	if (utf8 = utf8_encode( org_txt , CHAR_CODES ))
	{
		printf("its encoded\n");

		utf8_dump(utf8);
		printf("txt length %d\n",strlen(org_txt));
		printf("utf8 length %d\n",utf8_length( utf8 ));


		if (txt = utf8_decode( utf8, CHAR_CODES ))
		{
			printf("%s\n",txt);
			IExec -> FreeVec(txt);
		}

		if (fd = open( "ram:utf8.txt", O_CREAT | O_RDWR ))
		{
			write(fd,utf8,strlen(utf8) );
			close(fd);
		}

		IExec -> FreeVec(utf8);
	}
}


int ami_main_test(int nargs,char **args)
{
	char *buffer;
	ULONG ret;
	int n,pos,len;
	int size;

	size = 0;
	for (n=0;n<31;n++)
	{
		len = utf8_estimate_bytesize( 1<<n );
		size += len;
	}
	printf("size needed %d\n", size);

	buffer = IExec -> AllocVec(size+1, MEMF_CLEAR );

	// create a bad utf8 string
	pos = 0;
	for (n=0;n<31;n++)
	{
		pos += utf8_set_glyph( 1<<n, buffer + pos );
	}
	buffer[pos] = 0;

	pos = 0;
	do
	{
		ret = utf8_get_glyph( buffer + pos, &len) ;
		pos += len;

		printf("%d\n",ret);

	} while ( (len>0) && (ret > 0) );

	printf("buffer size used %d of %d\n",pos,size+1);

	IExec -> FreeVec(buffer);
}


