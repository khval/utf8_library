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

/****** UTF8/main/Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*     BOOL UTF8ModToUpper(ULONG *alphabet_UTF8, unsigned char * UTF8, ULONG mem_flags );
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


BOOL _UTF8_UTF8ModToUpper(struct UTF8IFace *Self,
	unsigned char *alphabet_UTF8,unsigned char **UTF8,  ULONG mem_flags )
{
//	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int alen = _UTF8_UTF8Length( Self, alphabet_UTF8 );
	int halen = alen /2;
	int len, n,a,pos = 0;

	if ((alen&1)||(alen==0)) return FALSE;

	int tlen =  _UTF8_UTF8Length( Self, *UTF8 );
	int size;
	unsigned char *new_utf8;
	ULONG glyph;

	// we don't really know the size of new string.
	// because string is dynamic, so need to use UTF32.

	// its alloced as stack, so don't need to free it, 
	// it should also be faster.

	ULONG *tempUTF32 = (ULONG *) alloca( sizeof(ULONG) * (tlen + 1) );
	if (!tempUTF32) return FALSE;

	for (n=0;n<tlen;n++)
	{
		glyph = _UTF8_UTF8GetGlyph( Self, *UTF8 + pos, &len );
		pos += len;

		for (a=0;a<halen;a++)
		{
			if (glyph == _UTF8_UTF8GetGlyphAt( Self, alphabet_UTF8, a, &len))
			{
				glyph = _UTF8_UTF8GetGlyphAt( Self, alphabet_UTF8, halen+a, &len);
			}
		}

		tempUTF32[n] = glyph;
	}

	size = 1;
	for (n=0;n<tlen;n++)
	{
		len = _UTF8_UTF8EstimateByteSize( Self, tempUTF32[n] );
		size += len;
	}

	new_utf8 = (unsigned char *) sys_alloc(size, mem_flags );
	if (new_utf8)
	{
		pos = 0;
		for (n=0;n<tlen;n++)
		{
			pos += _UTF8_UTF8SetGlyph( Self, tempUTF32[n], new_utf8 + pos );
		}
		new_utf8[pos] = 0;

		FreeVec(*UTF8);
		*UTF8 = new_utf8;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

