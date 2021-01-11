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

/****** UTF8/main/Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*      unsigned char * Encode( ULONG *codeset_page, char * ascii, ULONG mem_flags );
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


unsigned char * _UTF8_Encode(struct UTF8IFace *Self, ULONG *codeset_page, char * ascii, ULONG mem_flags)
{
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;

	unsigned char *c;
	unsigned char *utf8 = NULL;
	int size,pos;

	if ( ! libBase ) return NULL;
	if ( ! libBase -> IExec ) return NULL;
	if ( ! ascii ) return NULL;
	if ( ! codeset_page ) 
	{
		size = 0;
		for (c = (unsigned char *) ascii; *c ; c++ )
			size += _UTF8_EstimateByteSize( *c );

		utf8 = AllocVecTags(size+2, AVT_Type, mem_flags, TAG_END );
		if (utf8)
		{
			pos = 0;
			for (c = (unsigned char *) ascii; *c ; c++ )
				pos += _UTF8_ SetGlyph( *c, utf8 + pos );
			utf8[pos] = 0;
		}
	}
	else
	{
		size = 0;
		for (c = (unsigned char *) ascii; *c ; c++ )
			size += _UTF8_EstimateByteSize( codeset_page[  *c ] );

		utf8 = AllocVecTags(size+2, AVT_Type, mem_flags, TAG_END );
		if (utf8)
		{
			pos = 0;
			for (c = (unsigned char *) ascii; *c ; c++ )
				pos += _UTF8_ SetGlyph( codeset_page[  *c ], utf8 + pos );
			utf8[pos] = 0;
		}
	}

	return utf8;
}

