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

/****** UTF8/main/Encode ******************************************
*
*   NAME
*      Encode -- Description
*
*   SYNOPSIS
*       BOOL _UTF8_ModRight( unsigned char **UTF8, int _len, ULONG flags )
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


BOOL _UTF8_ModRight(struct UTF8IFace *Self,
		unsigned char **UTF8, int _len, ULONG flags )
{
	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int pos =0;
	int size;
	BOOL ret = FALSE;
	unsigned char *temp;
	int _max_len = Self -> Length( *UTF8 ) ;
	int _start = _max_len  - _len; 

	if (_start<0)
	{
		 _start = 0;
		_len = _max_len;
	}

	pos = Self->GetOffsetSize( *UTF8, _start);
	if (pos == -1) return FALSE;

	size = Self->GetOffsetSize( *UTF8+pos,_len) ;
	if (size == -1) return FALSE;

	temp = (unsigned char *) libBase -> IExec -> AllocVecTags( size+1 , AVT_Type, flags, TAG_END );
	if (temp)
	{
		libBase -> IExec -> CopyMem( *UTF8 + pos, temp, size );
		libBase -> IExec -> FreeVec( *UTF8 );
		temp[size] = 0;
		*UTF8 =temp;
		ret = TRUE;
	}

	return ret;
}

