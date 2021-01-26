/*
    UTF8.library
    Copyright (C) 2014-2020  Kjetil Hvalstrand

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

#ifdef __amigaos4__
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#include <proto/dos.h>
#endif

#if _MSC_VER
#include "not_amiga.h"
#endif // _MSC_VER

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../libbase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/UTF8Printf ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*      unsigned char * UTF8Printf(ULONG *codeset_page, unsigned char *utf8_fmt, ...)
*
*   INPUTS
*       ULONG *codeset_page
*
*		To print a text into a ASCII console, we need to do reverse lookup to find ascii value.
*		codeset is used for this.
*
*       unsigned char *utf8_fmt
*
*		The formating string similar to normal printf fmt, only difference it take UTF8 string.
*
*	...
*
*		a list of variables you like to print.
*
*   RESULT
*       The result ...
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/


int my_strcmp( char *txta, char *txtb )
{
	int cnta=0,cntb=0;
	char *t,*a,*b;

	for (t=txta;*t;t++) cnta++;
	for (t=txtb;*t;t++) cntb++;

	if (cnta!=cntb) return 1;

	b=txtb;
	for (a=txta;*a;a++)
	{
		if (*a!=*b) return 1;
		b++;
	}

	return 0;
}

int my_find_item(const char **list,char *txt)
{
	int nr = 0;
	const char **item;

	for (item = list; *item; item++ )
	{
		if (my_strcmp( *((char **) item),txt) == 0) return nr;
		nr++;
	}
	return -1;
}

enum {
	sw_str,
	sw_uint32,
	sw_des32,
	sw_int32,
	sw_int64,
	sw_float,
	sw_short
};

int get_format(  char *text,  char *buffer )
{
	int n;
	 char *c;
	 char *d;

	c = text;
	d = buffer;
	
	// copy '%'
	*d = *c;
	d++;c++;
	n = 1;

	for (; ((*c>='a')&&(n<4)) ; c++ )
	{
		*d = *c;
		d++;
		n++;
	}
	*d = 0;	// null term.	

	return n;	
}

void VARARGS68K _UTF8_UTF8Printf(struct UTF8IFace *Self,ULONG *codeset_page, unsigned char *utf8_fmt, ...)
{
	va_list list;



	va_startlinear(list, utf8_fmt);

	
	//	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	const char *fmt_list[]={"%s","%u","%d","%i","%lld","%f","%h", NULL};
	char *fmt;
	char fmt_buffer[6];
	char *buffer = NULL;
	char *b,*f;
	char *arg_str;
	unsigned char *tmp;

	fmt = _UTF8_UTF8Decode( Self, codeset_page, utf8_fmt, MEMF_PRIVATE );
	if (fmt)
	{
		buffer = (char *) sys_alloc_clear( _UTF8_UTF8GetSize ( Self, utf8_fmt) ,MEMF_PRIVATE );
	}

	if ((fmt)&&(buffer))
	{
		b = buffer;
		f = fmt;

		while (*f)
		{
			switch (*f)
			{
				case '%': 
					*b = 0;		// null term.
					b = buffer;	// reset buffer cnt.

					Printf("%s",buffer);

					f += get_format( f, fmt_buffer );

					switch (my_find_item(fmt_list,fmt_buffer))
					{
						case sw_str:

							tmp =  va_arg(list,unsigned char *);

							arg_str = _UTF8_UTF8Decode( Self, codeset_page, tmp , MEMF_PRIVATE );
							if (arg_str)
							{
								Printf("%s",arg_str);
								FreeVec(arg_str);
							}

					 		DebugPrintF("We are done decoding the string \n");
							break;
						case sw_uint32:
							Printf("%lu", va_arg(list,ULONG)); break;
						case sw_des32:
							Printf("%ld", va_arg(list,int)); break;
						case sw_int32:
							Printf("%ld", va_arg(list,int)); break;
						case sw_int64:
							Printf(fmt_buffer, va_arg(list,long long int)); break;
						case sw_short:
							Printf(fmt_buffer, va_arg(list,int)); break;
						default:
							Printf(fmt_buffer, va_arg(list,ULONG)); break;
					}

					break;
				default :

					*b = *f;	
					f++;
					b++;
			}
		}
		*b = 0;		// null term.
	}

	Printf("%s",buffer);

	if (buffer) FreeVec(buffer);
	if (fmt) FreeVec(fmt);

	va_end(list);
}

