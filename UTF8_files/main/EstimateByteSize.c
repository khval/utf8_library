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
#include "../UTF8_vectors.h"

/****** UTF8/main/EstimateByteSize ******************************************
*
*   NAME
*      EstimateByteSize -- Description
*
*   SYNOPSIS
*      int EstimateByteSize(ULONG glyph);
*
*   FUNCTION
*
*   INPUTS
*       glyph - 
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

int _UTF8_EstimateByteSize(struct UTF8IFace *Self, ULONG glyph)
{
 	int n;
	int bytes_to_bits[] = {7,11,16,21,26,31};

	for (n = 0; n<6; n++)
		if (glyph < (1<<bytes_to_bits[n]) ) return n+1;

	return 0;
}

