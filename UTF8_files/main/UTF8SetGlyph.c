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

/****** UTF8/main/UTF8SetGlyph ******************************************
*
*   NAME
*      SetGlyph -- Description
*
*   SYNOPSIS
*      int UTF8SetGlyph(ULONG glyph, unsigned char * data);
*
*   FUNCTION
*
*   INPUTS
*       glyph - 
*       data - 
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

ULONG _UTF8_UTF8SetGlyph(struct UTF8IFace *Self,
       ULONG glyph,
       unsigned char * data)
{
 	unsigned char prefix[] = {0x0,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};
	int n;

	int bytes = _UTF8_UTF8EstimateByteSize( Self, glyph );

	for ( n = bytes-1; n>0; n-- )
	{
		data[n] = 0x80 | (glyph & 0x3F);
		glyph = glyph >> 6;
	}
	data[0] = prefix[bytes-1] | glyph; 

	return bytes;
}

