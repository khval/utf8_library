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
*       BOOL _UTF8_ModTrim( unsigned char **UTF8, ULONG mem_flags )
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


BOOL _UTF8_ModTrim(struct UTF8IFace *Self, unsigned char **UTF8, ULONG mem_flags )
{
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	BOOL ret = FALSE;
	int _start =0;
	int _end = -1;
	int _len;

	unsigned char *temp;
	int n;
	unsigned char c;

	for (n=0;*UTF8[n];n++)
	{
		c  = *UTF8[n];

		if ((c==0x20)&&(_end==-1)) _start=n+1;
		if (c!=0x20) _end = n;
	}
	_len =_end-_start+1;

	temp = (unsigned char *) AllocVecTags( _len + 1, AVT_Type, mem_flags, TAG_END );
	if (temp)
	{
		libBase->IExec -> CopyMem( *UTF8+_start, temp, _len );
		FreeVec( *UTF8 );
		temp[_len] = 0;
		*UTF8 =temp;
		ret = TRUE;
	}

	return ret;
}

