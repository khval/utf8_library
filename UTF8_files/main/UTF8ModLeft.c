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

/****** UTF8/main/UTF8ModLeft ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*       BOOL _UTF8_UTF8ModLeft( unsigned char **UTF8, int _len, ULONG flags )
*
*   FUNCTION
*
*   INPUTS
*
*       unsigned char **UTF8, 
*
*		The UTF8 string to cut from.
*
*	int _len, 
*
*		length of string from left side.
*
*	ULONG flags
*
*		memory type.
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


BOOL _UTF8_UTF8ModLeft(struct UTF8IFace *Self,	unsigned char **UTF8, int _len, ULONG flags )
{
//	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int size;
	BOOL ret = FALSE;
	unsigned char *temp;
	int _max_len = _UTF8_UTF8Length( Self, *UTF8 ) ;

	if (_len>_max_len) _len = _max_len;

	size = _UTF8_UTF8GetOffsetSize( Self, *UTF8,_len) ;
	if (size == -1) return FALSE;

	temp = (unsigned char *) sys_alloc( size+1 , flags );
	if (temp)
	{
		CopyMem( *UTF8, temp, size);
		FreeVec( *UTF8 );
		temp[size]=0;
		*UTF8 =temp;
		ret = TRUE;
	}

	return ret;
}

