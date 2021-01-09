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
*       unsigned char * _UTF8_Copy( unsigned char *UTF8, ULONG mem_flags )
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


unsigned char * _UTF8_Copy(struct UTF8IFace *Self,unsigned char *UTF8, ULONG mem_flags )
{
	struct _Library *libBase = (struct _Library *) Self -> Data.LibBase;
	int size;
	unsigned char *ret;

	size = Self->GetSize( UTF8 ) ;
	if (size == -1) return NULL;

	ret = (unsigned char *) libBase -> IExec -> AllocVecTags( size ,AVT_Type,  mem_flags, TAG_END );
	if (ret)
	{
		libBase->IExec->CopyMem(UTF8,ret,size);
	}

	return ret;
}

