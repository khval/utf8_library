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

/****** UTF8/main/Length ******************************************
*
*   NAME
*      Length -- Description
*
*   SYNOPSIS
*      ULONG GetOffsetSize(unsigned char * UTF8, int _len);
*
*   FUNCTION
*
*   INPUTS
*       UTF8 - 
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

ULONG _UTF8_GetOffsetSize(struct UTF8IFace *Self,  unsigned char * UTF8, int to_pos)
{
	int offset =0;
	int n,len;
	ULONG value;

	offset = 0;
	for (n=0;n<to_pos;n++)
	{
		value = Self -> GetGlyph( UTF8 + offset, &len );
		offset += len;
		if (value == 0) return -1;
	}
	return offset;
}

