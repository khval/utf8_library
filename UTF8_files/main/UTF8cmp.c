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
*       int _UTF8_UTF8Cmp( unsigned char *UTF8_alphabet, unsigned char *UTF8_one, unsigned char *UTF8_two )
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


int _UTF8_UTF8Cmp(struct UTF8IFace *Self, unsigned char *UTF8_alphabet, unsigned char *UTF8_one,  unsigned char *UTF8_two)
{
	int pos_one =0, pos_two = 0, len;
	ULONG value_one,value_two,t,alen,halen;

	alen = _UTF8_UTF8Length( Self, UTF8_alphabet );
	halen = alen /2;

	do
	{
		value_one = _UTF8_UTF8GetGlyph( Self, UTF8_one + pos_one , &len );
		if (len==0) return 2;
		pos_one += len;
		value_two = _UTF8_UTF8GetGlyph( Self, UTF8_two + pos_two , &len );
		if (len==0) return 2;
		pos_two +=len;

		t=_UTF8_UTF8FindGlyph( Self, UTF8_alphabet, value_one );
		if (t!=0xFFFFFFFF) 	value_one = ((t%halen)<<1) + (t/halen)+1;

		t=_UTF8_UTF8FindGlyph( Self, UTF8_alphabet, value_two );
		if (t!=0xFFFFFFFF)	value_two = ((t%halen)<<1) + (t/halen)+1;

		if (value_one>value_two) return 1;
		if (value_one<value_two) return -1;

	} while ((value_one)&&(value_two));

	return 0;
}

