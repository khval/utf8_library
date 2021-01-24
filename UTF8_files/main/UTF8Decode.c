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
#include "../libbase.h"
#include "../UTF8_vectors.h"

/****** UTF8/main/UTF8Decode ******************************************
*
*   NAME
*      Decode -- Description
*
*   SYNOPSIS
*      char * UTF8Decode(unsigned char * UTF8, ULONG codeset_page);
*
*   FUNCTION
*
*   INPUTS
*       UTF8 - 
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


char * _UTF8_UTF8Decode(struct UTF8IFace *Self, ULONG *codeset_page, unsigned char * UTF8, ULONG flags )
{
	char *c;
	char *new_str = NULL;
	int pos,n=0,len;
	ULONG ret;
	int size;

	if (!UTF8) return NULL;

	size =  _UTF8_UTF8GetSize( Self, UTF8 );

	DebugPrintF(" _UTF8_UTF8Decode UTF8 string len = %d \n", size);  

	new_str =  sys_alloc_clear(size, flags);
	if (new_str)
	{
		c = new_str;
		pos = 0;

		if (!codeset_page)
		{
			do
			{
				ret = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, &len ) ;
				pos += len;
				*c = ret < 256 ? (char) ret : '#';
				c++;
			} while ( (len>0) && (ret > 0) );
		}
		else
		{
			do
			{
				ret = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, &len ) ;
				pos += len;

				*c = '#';	// default value if glyph is not found.
				for (n = 0; n<256;n++)
				{
					if (codeset_page[n] == ret )
					{ *c = n; break; }
				}
				c++;
			} while ( (len>0) && (ret > 0) );
		}
	}
	return new_str;
}

char * _UTF8_UTF8DecodeLen(struct UTF8IFace *Self, ULONG *codeset_page, unsigned char * UTF8, ULONG bytes, ULONG flags )
{
	char *c;
	char *new_str = NULL;
	int pos,n=0,len;
	ULONG ret;
	int size;

	if (!UTF8) return NULL;

	size =  _UTF8_UTF8GetSize( Self, UTF8 ) ;

	DebugPrintF(" _UTF8_Decode UTF8 string size = %d \n", size);  

	new_str =  sys_alloc(size,flags);	

	if (new_str)
	{
		c = new_str;
		pos = 0;

		if (!codeset_page)
		{
			do
			{
				ret = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, &len ) ;
				pos += len;
				bytes -= len;
				*c = ret < 256 ? (char) ret : '#';
				c++;
			} while ( (len>0) && (ret > 0) && (bytes>0) );
		}
		else
		{
			do
			{
				ret = _UTF8_UTF8GetGlyph( Self, UTF8 + pos, &len ) ;
				pos += len;
				bytes -= len;

				*c = '#';	// default value if glyph is not found.
				for (n = 0; n<256;n++)
				{
					if (codeset_page[n] == ret )
					{ *c = n; break; }
				}

				c++;
			} while ( (len>0) && (ret > 0) && (bytes>0) );
		}
	}
	return new_str;
}

