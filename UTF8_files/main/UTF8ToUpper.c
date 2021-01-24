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

#include <stdlib.h>

#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#include <stdarg.h>
#include "../libbase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/UTF8Upper ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*      unsigned char * UTF8Upper(ULONG *codeset_page,char * ascii);
*
*   FUNCTION
*
*   INPUTS
*       ascii - 
*       codeset_page - 
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


unsigned char * _UTF8_UTF8ToUpper(struct UTF8IFace *Self, unsigned char *alphabet_UTF8, unsigned char *UTF8, ULONG mem_flags )
{
	int alen = _UTF8_UTF8Length( Self, alphabet_UTF8 );
	int halen = alen /2;
	int len, n,a,pos = 0;

	if ((alen&1)||(alen==0)) return NULL;

	int tlen =  _UTF8_UTF8Length( Self, UTF8 );
	int size;
	unsigned char *new_utf8;
	ULONG glyph;
	ULONG *ptr32;

	ULONG *tempUTF32 = (ULONG *) alloca( sizeof(ULONG) * (tlen + 1) );
	if (!tempUTF32) return FALSE;

	for (n=0;n<tlen;n++)
	{
		glyph = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, &len );
		pos += len;

		for (a=0;a<halen;a++)
		{
			if (glyph == _UTF8_UTF8GetGlyphAt( Self,alphabet_UTF8, a, &len))
			{
				glyph = _UTF8_UTF8GetGlyphAt( Self, alphabet_UTF8, halen+a, &len);
			}
		}

		tempUTF32[n] = glyph;
	}
	tempUTF32[tlen] = 0;

	size = 1;
	for (ptr32=tempUTF32;*ptr32;ptr32++)
	{
		len = _UTF8_UTF8EstimateByteSize( Self, *ptr32 );
		size += len;
	}

	new_utf8 = (unsigned char *) sys_alloc(size, mem_flags );
	if (new_utf8)
	{
		pos = 0;
		for (ptr32=tempUTF32;*ptr32;ptr32++)
		{
			pos += _UTF8_UTF8SetGlyph( Self, *ptr32, new_utf8 + pos );
		}
		new_utf8[pos] = 0;
	}
	else
	{
		return NULL;
	}

	return new_utf8;
}

