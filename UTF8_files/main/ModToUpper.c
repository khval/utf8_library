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
*     BOOL ModToUpper(unsigned char * UTF8, ULONG *alphabet_UTF8, ULONG mem_flags );
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


BOOL _UTF8_ModToUpper(struct UTF8IFace *Self,
	unsigned char **UTF8, unsigned char *alphabet_UTF8, ULONG mem_flags )
{
//	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int alen = _UTF8_Length( Self, alphabet_UTF8 );
	int halen = alen /2;
	int len, n,a,pos = 0;

	if ((alen&1)||(alen==0)) return FALSE;

	int tlen =  _UTF8_Length( Self, *UTF8 );
	int size;
	unsigned char *new_utf8;
	ULONG glyph;

	ULONG *temp = (ULONG *) AllocVec( sizeof( ULONG ) * tlen, MEMF_CLEAR );
	if (!temp) return FALSE;

	for (n=0;n<tlen;n++)
	{
		glyph = _UTF8_GetGlyph( Self, *UTF8 + pos, &len );
		pos += len;

		for (a=0;a<halen;a++)
		{
			if (glyph == _UTF8_GetGlyphAt( Self, alphabet_UTF8, a, &len))
			{
				glyph = _UTF8_GetGlyphAt( Self, alphabet_UTF8, halen+a, &len);
			}
		}

		temp[n] = glyph;
	}

	size = 1;
	for (n=0;n<tlen;n++)
	{
		len = _UTF8_EstimateByteSize( Self, temp[n] );
		size += len;
	}

	new_utf8 = (unsigned char *) AllocVecTags(size+100, AVT_Type, mem_flags, TAG_END );
	if (new_utf8)
	{
		pos = 0;
		for (n=0;n<tlen;n++)
		{
			pos += _UTF8_SetGlyph( Self, temp[n], new_utf8 + pos );
		}
		new_utf8[pos] = 0;

		FreeVec(temp);
		FreeVec(*UTF8);
		*UTF8 = new_utf8;
	}
	else
	{
		FreeVec(temp);
		return FALSE;
	}

	return TRUE;
}

