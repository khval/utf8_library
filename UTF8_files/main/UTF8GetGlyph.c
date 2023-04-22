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

#ifdef __amigaos4__
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#endif

#if _MSC_VER
#include "not_amiga.h"
#endif // _MSC_VER

#include <stdarg.h>
#include "../libBase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/UTF8GetGlyph ******************************************
*
*   NAME
*      GetGlyph -- Description
*
*   SYNOPSIS
*      ULONG UTF8GetGlyph(unsigned char *utf8, int * len);
*
*   FUNCTION
*
*   INPUTS
*       data - 
*       len - 
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

ULONG _UTF8_UTF8GetGlyph(struct UTF8IFace *Self,  unsigned char *utf8, int * len2)
{
	if (!(*utf8 & 0xC0))
	{
		*len2=1;
		 return (ULONG) *utf8;
	}
	else
	{
		register unsigned int value;
		register unsigned int len = 1;
		struct u8l *xbyte;
		struct u8l *header = u8_b1 + (*utf8);
		register int hlen = header -> len;

		if (hlen)
		{
			value = header -> value;

			while (--hlen)
			{
				utf8++;	// get next byte..
				xbyte= u8_b2 + (*utf8);

				if (xbyte -> len == 1)	// branch prediction expected
				{
					value = (value << 6)  | xbyte -> value;
					len++;
					continue;
				}
				else	// not expected
				{
					*len2=0;
					return 0;	
				}
			}
	
			*len2 = len;
			return value;
		}
		else
		{
			*len2=0;
			return 0;
		}
	}
}


/*
ULONG _UTF8_UTF8GetGlyph(struct UTF8IFace *Self,  unsigned char *utf8, int * len)
{
	unsigned char *c;
	ULONG ret = 0;
	int bytes;
	int bits;
	int n;
	int mask;

	bytes = 1;
	c =utf8;

	if ( ! (*c & 0x80))
	{
		ret = *c;
		*len = 1;
	}
	else		// muli chars;
	{
		bits =6;
		if  ( (*c & 0xC0) == 0x80 ) 
		{
			 *len =0; return 0;
		}
		
		for ( n = 6; (*c&(1<<n)) && (n>0)  ; n--)
		{
			bytes++;	 bits--;
		}

		mask = (1<<bits)-1 ;
		ret = *c & mask;

		for (n=1; n<bytes;n++)
		{
			c++;
			if ( (*c&0xC0) != 0x80) { *len = 0; return 0; }
			ret = (ret << 6) | (*c & 0x3F);
			if (*c == 0) { *len = 0; return 0; }
		}
		if (ret<128) 
		{
		 	ret = 0x20;
		}
	}

	*len = bytes;

	return ret;
}
*/
