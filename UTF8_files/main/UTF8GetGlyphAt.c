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
#include "../libBase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/GetGlyph ******************************************
*
*   NAME
*      GetGlyph -- Description
*
*   SYNOPSIS
*      ULONG GetGlyphAt(unsigned char data,int pos,int * len);
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

ULONG _UTF8_UTF8GetGlyphAt(struct UTF8IFace *Self,  unsigned char *UTF8, int _pos, int * len)
{
	int pos = 0;
	int bpos = 0;
	ULONG ret;
	do
	{
		ret = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, len ) ;
		if (len == 0) return 0;
		pos += *len;
		bpos++;
	} while ( (bpos<=_pos) && (ret > 0) );

	return ret;
}

