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


#ifdef __amigaos4__
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#endif

#if _MSC_VER
#include "not_amiga.h"
#endif // _MSC_VER

#include <stdarg.h>
#include "../libBase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/Length ******************************************
*
*   NAME
*      Length -- Description
*
*   SYNOPSIS
*      ULONG UTF8GetSize(unsigned char * UTF8);
*
*   FUNCTION
*
*   INPUTS
*       UTF8 - 
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

ULONG _UTF8_UTF8GetSize(struct UTF8IFace *Self,
       unsigned char * UTF8)
{
	unsigned char *b;
	int size = 1;

	if (!UTF8) return -1;
	for (b = UTF8 ; *b ;b++ ) { size++; }
	return size;
}

