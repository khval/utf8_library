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


/****** UTF8/main/GetGlyph ******************************************
*
*   NAME
*      GetGlyph -- Description
*
*   SYNOPSIS
*      BOOL _UTF8_ModInsert(unsigned char *UTF8, int _start, unsigned char *UTF8_insert , ULONG flags )
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

BOOL _UTF8_ModInsert(struct UTF8IFace *Self, unsigned char **UTF8, int _start, unsigned char *UTF8_insert , ULONG flags )
{
//	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	BOOL ret = FALSE;
	int size, size_before, size_insert;
	unsigned char *temp;


	size = _UTF8_GetSize(Self, *UTF8);
	size_insert = _UTF8_GetSize(Self, UTF8_insert) - 1; // we don't want the extra 0 in the size.

	size_before = _UTF8_GetOffsetSize(Self, *UTF8, _start);
	if (size_before==-1) return FALSE;

	temp = (unsigned char *) sys_alloc(size + size_insert, flags );
	if (temp)
	{
		CopyMem( UTF8, temp, size_before );
		CopyMem( UTF8_insert, temp+size_before, size_insert );
		CopyMem( UTF8+size_before, temp+size_before+size_insert, size - size_before );
		temp[ size + size_insert - 1 ] = 0;	

		*UTF8 = temp;
		ret = TRUE;
	}

	return ret;
}

