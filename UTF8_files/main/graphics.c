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
#include <proto/dos.h>
#include <stdarg.h>
#include "../libbase.h"


BOOL glyph_size(struct UTF8IFace *Self, struct OutlineFont *ofont,float font_size, uint32 long_char_1, int *glyph_width, int *glyph_height, int *glyph_min )
{
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	struct GlyphMap *glyph;
	int error = 0;
	ULONG XDPI, YDPI; 

	XDPI = 100;
	YDPI = 100;

	*glyph_min = 0;
	*glyph_width = 0;
	*glyph_height = 0;

	if (long_char_1 == 0x20) 
	{
		*glyph_height = 0;
		*glyph_width = (ULONG) (font_size * 0.6f);
		*glyph_min = 0; 
		return TRUE;
	}
 
	if (!(error = libBase -> IDiskfont -> ESetInfo( &ofont -> olf_EEngine,   
			OT_DeviceDPI, (XDPI << 16) | YDPI,
			OT_PointHeight, 0x000010000 * (ULONG) font_size,
		    	OT_GlyphCode, long_char_1, 	TAG_END)))
	{
		if( (error = libBase -> IDiskfont -> EObtainInfo( &ofont -> olf_EEngine, OT_GlyphMap, &glyph, TAG_END)) == 0)
		{
			if (glyph)
			{
				if (glyph->glm_BitMap)
				{
					*glyph_width = glyph->glm_BlackWidth; 
					*glyph_height = glyph->glm_BlackHeight; 
					*glyph_min = glyph->glm_Y0;
				}
			}

			libBase -> IDiskfont -> EReleaseInfo( &ofont -> olf_EEngine, OT_GlyphMap, glyph,	TAG_END);
		}
	}

	if (*glyph_width>0) return TRUE;
	return FALSE;
}

int glyph_draw(struct UTF8IFace *Self,struct RastPort *rp, struct OutlineFont *ofont, float font_size, uint32 long_char_1 , uint32 x, uint32 y)
{
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	struct GlyphMap *glyph;
	int error = 0;
	ULONG XDPI, YDPI;
	int glyph_width = 16;

	XDPI = 100;
	YDPI = 100;

	if (long_char_1 == 0x20) return (ULONG) (font_size * 0.6f);
 
	    if (!(error = libBase -> IDiskfont -> ESetInfo( &ofont -> olf_EEngine,   
			OT_DeviceDPI, (XDPI << 16) | YDPI,
			OT_PointHeight, 0x000010000 * (ULONG) font_size,
		    	OT_GlyphCode, long_char_1, 	TAG_END)))
	{

		if( (error = libBase -> IDiskfont -> EObtainInfo( &ofont -> olf_EEngine, OT_GlyphMap, &glyph, TAG_END)) == 0)
		{
			if (glyph)
			{
				if ((rp)&&(glyph->glm_BitMap))
				{
					libBase -> IGraphics -> BltBitMapTags(
						BLITA_SrcX, glyph->glm_BlackLeft,
						BLITA_SrcY, glyph->glm_BlackTop,
						BLITA_DestX, x - glyph->glm_X0 + glyph->glm_BlackLeft,
						BLITA_DestY, y - glyph->glm_Y0 + glyph->glm_BlackTop,

						BLITA_Width, glyph->glm_BlackWidth,
						BLITA_Height, glyph->glm_BlackHeight,

						BLITA_Source, glyph->glm_BitMap,
						BLITA_SrcType, BLITT_TEMPLATE,
						BLITA_Dest, rp,
						BLITA_DestType, BLITT_RASTPORT,
						BLITA_SrcBytesPerRow, glyph->glm_BMModulo,
						TAG_DONE);

					glyph_width = glyph->glm_BlackWidth; 
				}
			}

			libBase -> IDiskfont -> EReleaseInfo( &ofont -> olf_EEngine, OT_GlyphMap, glyph,	TAG_END);
		}
	}

	return glyph_width;
}


void _UTF8_TextUTF8Extent( struct UTF8IFace *Self,struct OutlineFont *ofont, float font_size, unsigned char *UTF8, struct TextExtent *ext )
{
	ULONG glyph, pos;
	int len;
	int width, height, y0;

	ext -> te_Width = 0;
	ext -> te_Height = 0;

	for ( pos = 0; (glyph = _UTF8_GetGlyph( UTF8 + pos , &len )); )
	{
		glyph_size( Self, ofont, font_size, glyph, &width, &height, &y0 );

		if (height > ext -> te_Height )
		{
			ext -> te_Height =height;
			ext -> te_Extent.MinY = -y0;
		}

		ext -> te_Width +=width + 2;
		pos += len;
	}
}

void _UTF8_TextUTF8ExtentN( struct UTF8IFace *Self,struct OutlineFont *ofont, float font_size, unsigned char *UTF8, int n, struct TextExtent *ext )
{
//	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	ULONG glyph, pos;
	int len;
	int width, height, y0;

	ext -> te_Width = 0;
	ext -> te_Height = 0;

	pos = 0;
	for ( ; n>0 ; n-- )
	{
		glyph = _UTF8_GetGlyph( UTF8 + pos , &len );
		glyph_size( Self, ofont, font_size, glyph, &width, &height, &y0 );

		if (height > ext -> te_Height )
		{
			ext -> te_Height =height;
			ext -> te_Extent.MinY = -y0;
		}

		ext -> te_Width +=width + 2;
		pos += len;
	}
}

int _UTF8_TextUTF8Length( struct UTF8IFace *Self,struct OutlineFont *ofont, float font_size, unsigned char *UTF8 )
{
	ULONG glyph, pos;
	int len;
	ULONG xpos = 0;
	int width, height, y0;

	for ( pos = 0; (glyph = _UTF8_GetGlyph( UTF8 + pos , &len )); )
	{
		glyph_size( Self, ofont, font_size, glyph, &width,&height,&y0 );
		xpos +=width + 2;  
		pos += len;	
	}

	return xpos;
}

int _UTF8_TextUTF8LengthN( struct UTF8IFace *Self,struct OutlineFont *ofont, float font_size, unsigned char *UTF8, int n )
{
	ULONG glyph, pos;
	int len;
	ULONG xpos = 0;
	int width, height, y0;

	for ( pos = 0; ((glyph = _UTF8_GetGlyph( UTF8 + pos , &len ))&&(n-->0)); )
	{
		glyph_size( Self, ofont, font_size, glyph, &width,&height,&y0 );
		xpos +=width + 2;  
		pos += len;
	}

	return xpos;
}

void _UTF8_TextUTF8( struct UTF8IFace *Self,struct RastPort *rp,struct OutlineFont *ofont, float font_size, int xpos, int ypos, unsigned char *UTF8 )
{
	ULONG glyph, pos;
	int len;
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	ULONG w = libBase -> IGraphics ->GetBitMapAttr( rp -> BitMap, BMA_ACTUALWIDTH );

	for ( pos = 0; (glyph = _UTF8_GetGlyph( UTF8 + pos , &len )); )
	{
		xpos += glyph_draw( Self, rp, ofont, font_size, glyph, xpos, ypos ) + 2;
		pos += len;
		if (xpos >= w) break;
	}
}

void _UTF8_TextUTF8N( struct UTF8IFace *Self,struct RastPort *rp,struct OutlineFont *ofont, float font_size, int xpos, int ypos, unsigned char *UTF8, int n )
{
	ULONG glyph, pos;
	int len;
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	ULONG w = libBase -> IGraphics ->GetBitMapAttr( rp -> BitMap, BMA_ACTUALWIDTH );

	for ( pos = 0; ((glyph = _UTF8_GetGlyph( UTF8 + pos , &len )) && (n-->0)); )
	{
		xpos += glyph_draw( Self, rp, ofont, font_size, glyph, xpos, ypos ) + 2;
		pos += len;
		if (xpos >= w) break;
	}
}

void _UTF32_TextUTF32( struct UTF8IFace *Self,struct RastPort *rp,struct OutlineFont *ofont, float font_size, int xpos, int ypos, ULONG *UTF32 )
{
	ULONG glyph, pos;
	struct _Library *libBase = (struct _Library *) _UTF8_Data.LibBase;
	ULONG w = libBase -> IGraphics ->GetBitMapAttr( rp -> BitMap, BMA_ACTUALWIDTH );

	for ( ; glyph = *UTF32; UTF32++ )
	{
		xpos += glyph_draw( Self, rp, ofont, font_size, glyph, xpos, ypos ) + 2;
		if (xpos >= w) break;
	}
}
