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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#include <stdarg.h>

/****** UTF8/main/GetGlyph ******************************************
*
*   NAME
*      GetGlyph -- Description
*
*   SYNOPSIS
*      ULONG _UTF8_FindGlyph(unsigned char *UTF8_alphabet, unsigned char glyph )
*
*   FUNCTION
*
*   INPUTS
*       UTF8_alphabet - 
*       glyph - 
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

ULONG _UTF8_FindGlyph(struct UTF8IFace *Self,unsigned char *UTF8_alphabet, ULONG glyph )
{
	int len, a, alen;
	
	alen = _UTF8_ Length( UTF8_alphabet );

	for (a=0;a<alen;a++)
	{
		if (glyph == _UTF8_GetGlyphAt( UTF8_alphabet, a, &len))
		{
			return a;
		}
	}
	return 0xFFFFFFFF;
}
