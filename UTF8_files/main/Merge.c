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
#include <proto/dos.h>
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
*      unsigned char * Merge(struct UTF8IFace *Self,ULONG mem_flag, ...);
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



unsigned char * VARARGS68K _UTF8_Merge(struct UTF8IFace *Self,ULONG mem_flag, ...)
{
	va_list list;
	va_startlinear(list, mem_flag);
	unsigned char **args = va_getlinearva(list, unsigned char **);
	unsigned char *tmp;
	int size,pos,n;

	size = 1;
	for (n=0; args[n]; n++)
	{
		size += _UTF8_GetSize( Self, args[n] ) -1;
	}

	tmp = (unsigned char *) AllocVec( size, mem_flag | MEMF_CLEAR );
	if (tmp)
	{
		pos = 0;
		for (n=0; args[n]; n++)
		{
			size = _UTF8_GetSize( Self, args[n] );
			if (size == -1) break;

			CopyMem(  args[n], tmp+pos  , size);
			pos += size - 1; // remove overwrite /0
		}
	}
	va_end(list);

	return tmp;
}

