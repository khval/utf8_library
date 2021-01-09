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

#define UTF8_2BYTES 0x80|0x40
#define MCHARS 0x80;

void show_bits(ULONG num)
{
	int n;

	for (n=32;n!=-1;n--) printf("%c", (1<<n) & num ? '1' : '0' );
	printf("\n");
}

ULONG utf8_get_glyph(unsigned char *data, int *len)
{
	unsigned char *c;
	ULONG ret = 0;
	int bytes;
	int bits;
	int n;
	int mask;

	bytes = 1;
	c =data;

	if ( ! (*c & 0x80))
	{
		ret = *c;
		*len = 1;
	}
	else		// muli chars;
	{
		bits =6;
		if  ( (*c & 0xC0) == 0x80 ) 
		{
			 *len =0; return 0;
		}
		
		for ( n = 6; (*c&(1<<n)) && (n>0)  ; n--)
		{
			bytes++;	 bits--;
		}

		mask = (1<<bits)-1 ;
		ret = *c & mask;

		for (n=1; n<bytes;n++)
		{
			c++;
			if ( (*c&0xC0) != 0x80) { *len = 0; return 0; }

			ret = (ret << 6) | (*c & 0x3F);

			if (*c == 0) { *len = 0; return 0; }
		}
		if (ret<128) 
		{
		 	ret = 0x20;
		}
	}

	*len = bytes;

	return ret;
}

int utf8_estimate_bytesize(ULONG glyph)
{
	int n;
	int bytes_to_bits[] = {7,11,16,21,26,31};

	for (n = 0; n<6; n++)
		if (glyph < (1<<bytes_to_bits[n]) ) return n+1;

	return 0;
}


int utf8_set_glyph( ULONG glyph, unsigned char *data )
{
	unsigned char prefix[] = {0x0,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};
	int n,pos;

	int bytes = utf8_estimate_bytesize(glyph);

	for ( n = bytes-1; n>0; n-- )
	{
		data[n] = 0x80 | (glyph & 0x3F);
		glyph = glyph >> 6;
	}
	data[0] = prefix[bytes-1] | glyph; 

	return bytes;
}


int utf8_length( unsigned char *utf8 )
{
	char *c;
	int cnt = 0;

	for (c = utf8 ; *c ;c++ )
	{
		if (*c < 128) cnt++;
		if ( (*c & 0xC0) == 0xC0 ) cnt++;
	}
	return cnt;
}

unsigned char *utf8_encode( char *ascii, ULONG *codeset_page )
{
	char *c;
	unsigned char *utf8 = NULL;
	int size,pos;

	size = 0;
	for (c = ascii; *c ; c++ )
		size += utf8_estimate_bytesize( codeset_page[ *c ] );

	printf("UTF8 buffer %d\n",size);

	if (utf8 = IExec -> AllocVec(size+1, MEMF_CLEAR ))
	{
		pos = 0;
		for (c = ascii; *c ; c++ )
			pos += utf8_set_glyph( codeset_page[ *c ], utf8 + pos );
		utf8[pos] = 0;
	}

	return utf8;
}

unsigned char *utf8_decode( unsigned char *utf8, ULONG *codeset_page )
{
	char *c;
	char *new_str = NULL;
	int pos,n,len;
	ULONG ret;
	int size =  utf8_length( utf8 );

	if (new_str = IExec -> AllocVec(size+1, MEMF_CLEAR ))
	{
		c = new_str;
		pos = 0;
		do
		{
			ret = utf8_get_glyph( utf8 + pos, &len) ;
			pos += len;

			*c = 0x20;	// default value if glyph is not found.
			for (n = 0; n<256;n++)
			{
				if (codeset_page[n] == ret )
				{ *c = n; break; }
			}
			c++;
		} while ( (len>0) && (ret > 0) );
		*c = 0;	// term string
	}

	return new_str;
}

