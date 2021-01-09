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
*      ULONG GetGlyph(unsigned char data, int * len);
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

ULONG _UTF8_GetGlyph(struct UTF8IFace *Self,  unsigned char *data, int * len)
{
	unsigned char *c;
	ULONG ret = 0;
	int bytes;
	int bits;
	int n;
	int mask;

	bytes = 1;
	c =data;

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

