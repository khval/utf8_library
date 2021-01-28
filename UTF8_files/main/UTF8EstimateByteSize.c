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

/****** UTF8/main/UTF8EstimateByteSize ******************************************
*
*   NAME
*      EstimateByteSize -- Description
*
*   SYNOPSIS
*      int UTF8EstimateByteSize(ULONG glyph);
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

#define b1_value (1u<<7)	// ASCII
#define b2_value (1u<<11)	// 5bit + 6bit			// 2 bits = 2 bytes, 1 bit space , 3 bit used.
#define b3_value (1u<<16)	// 4bit + 6bit + 6bit 
#define b4_value (1u<<21)	// 3bit + 6bit + 6bit + 6bit
#define b5_value (1u<<26)	// 2bit + 6bit + 6bit + 6bit + 6bit
#define b6_value (1u<<31)	// 1bit + 6bit + 6bit + 6bit + 6bit + 6bit

int _UTF8_UTF8EstimateByteSize(struct UTF8IFace *Self, ULONG glyph)
{
	if (glyph < b1_value ) return 1;
	if (glyph < b2_value ) return 2;
	if (glyph < b3_value ) return 3;
	if (glyph < b4_value ) return 4;
	if (glyph < b5_value ) return 5;
	if (glyph < b6_value ) return 6;

	return 0;
}

