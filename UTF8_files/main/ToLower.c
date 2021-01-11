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

/****** UTF8/main/Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*      unsigned char * ToLower(unsigned char *alphabet_UTF8,unsigned char *UTF8);
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


unsigned char *_UTF8_ToLower(struct UTF8IFace *Self,	unsigned char *alphabet_UTF8, unsigned char *UTF8 )
{
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	int alen = _UTF8_Length( alphabet_UTF8 );
	int halen = alen /2;
	int len, n,a,pos = 0;

	if ((alen&1)||(alen==0)) return NULL;

	int tlen =  _UTF8_Length( UTF8 );
	int size;
	unsigned char *new_utf8;
	ULONG glyph;

	ULONG *temp = (ULONG *) AllocVecTags( sizeof( ULONG ) * tlen, AVT_Type, MEMF_SHARED, TAG_END );
	if (!temp) return FALSE;

	for (n=0;n<tlen;n++)
	{
		glyph = _UTF8_GetGlyph( UTF8 + pos, &len );
		pos += len;

		for (a=0;a<halen;a++)
		{
			if (glyph == _UTF8_GetGlyphAt( alphabet_UTF8, halen+a, &len))
			{
				glyph = _UTF8_GetGlyphAt( alphabet_UTF8, a, &len);
			}
		}

		temp[n] = glyph;
	}

	size = 1;
	for (n=0;n<tlen;n++)
	{
		len = _UTF8_EstimateByteSize( temp[n] );
		size += len;
	}

	new_utf8 = (unsigned char *) AllocVecTags(size+100, AVT_Type, MEMF_SHARED, TAG_END );
	if (new_utf8)
	{
		pos = 0;
		for (n=0;n<tlen;n++)
		{
			pos += _UTF8_SetGlyph( temp[n], new_utf8 + pos );
		}
		new_utf8[pos] = 0;

		FreeVec(temp);
	}
	else
	{
		FreeVec(temp);
		return NULL;
	}

	return new_utf8;
}

