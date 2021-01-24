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
#include "../libbase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/UTF8Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*      unsigned char * UTF8Encode( ULONG *codeset_page, char * ascii, ULONG mem_flags );
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


unsigned char * _UTF8_UTF8Encode(struct UTF8IFace *Self, ULONG *codeset_page, char * ascii, ULONG mem_flags)
{
	unsigned char *c;
	unsigned char *utf8 = NULL;
	int size,pos;

	if ( ! ascii ) return NULL;
	if ( ! codeset_page ) 
	{
		size = 1;
		for (c = (unsigned char *) ascii; *c ; c++ )
			size += _UTF8_UTF8EstimateByteSize( Self, *c );

		utf8 = sys_alloc(size,  mem_flags );
		if (utf8)
		{
			pos = 0;
			for (c = (unsigned char *) ascii; *c ; c++ )
				pos += _UTF8_UTF8SetGlyph( Self, *c, utf8 + pos );
			utf8[pos] = 0;
		}
	}
	else
	{
		size = 1;
		for (c = (unsigned char *) ascii; *c ; c++ )
			size += _UTF8_UTF8EstimateByteSize( Self, codeset_page[  *c ] );

		utf8 = sys_alloc(size, mem_flags );
		if (utf8)
		{
			pos = 0;
			for (c = (unsigned char *) ascii; *c ; c++ )
				pos += _UTF8_UTF8SetGlyph( Self, codeset_page[  *c ], utf8 + pos );
			utf8[pos] = 0;
		}
	}

	return utf8;
}

