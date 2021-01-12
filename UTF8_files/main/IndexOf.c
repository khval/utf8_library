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

/****** Self/main/Length ******************************************
*
*   NAME
*      Length -- Description
*
*   SYNOPSIS
*      int IndexOf(unsigned char * UTF8);
*
*   FUNCTION
*
*   INPUTS
*       Self - 
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

int _UTF8_IndexOf(struct UTF8IFace *Self, unsigned char * UTF8,unsigned char *find)
{
	int cnt = 0;
	int at = -1;
	int pos = 0;
	int lena,lenb = 0;
	int valuea,valueb;

	ULONG Self_len = _UTF8_Length(UTF8);
	ULONG find_len = _UTF8_Length(find);

	if ((find_len == 0)||(Self_len == 0)) return -1;

	for (pos = 0; pos<Self_len; pos++)
	{
		valuea = _UTF8_GetGlyphAt(UTF8,pos,&lena);
		valueb = _UTF8_GetGlyphAt(find,cnt,&lenb);

		if ((lena==0)||(lenb==0)) return -1;

		if (valuea == valueb)
		{
			if (at==-1) 
			{
				at = pos;
			}
			cnt++;
		}
		else
		{
			at = -1;
			cnt = 0;
		}

		if (cnt == find_len) break;
	}
	if (cnt != find_len)
	{
		at = -1;
	}

	return at;
}

