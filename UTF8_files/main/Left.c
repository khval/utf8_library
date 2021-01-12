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

/****** UTF8/main/Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*       unsigned char * _UTF8_Left( unsigned char *UTF8, int _len, ULONG flags )
*
*   FUNCTION
*
*   INPUTS
*       start - 
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


unsigned char * _UTF8_Left(struct UTF8IFace *Self,
		unsigned char *UTF8, int _len, ULONG flags )
{
////	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int pos =0;
	int size;
	int n,len;
	ULONG value;
	unsigned char *ret;
	int _max_len = _UTF8_Length( Self, UTF8 ) ;

	if (_len>_max_len) _len = _max_len;

	size = 0;
	for (n=0;n<_len;n++)
	{
		value = _UTF8_GetGlyph( Self, UTF8 + pos + size, &len );
		size += len;
		if (value == 0) return NULL;
	}

	ret = (unsigned char *) AllocVec( size+1 ,MEMF_CLEAR);
	CopyMem( UTF8, ret, size);

	return ret;
}

