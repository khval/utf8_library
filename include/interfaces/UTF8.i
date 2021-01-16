#ifndef UTF8_INTERFACE_DEF_H
#define UTF8_INTERFACE_DEF_H
/*
** This file is machine generated from idltool
** Do not edit
*/ 

#include <exec/types.i>
#include <exec/exec.i>
#include <exec/interfaces.i>

STRUCTURE UTF8IFace, InterfaceData_SIZE
	    FPTR IUTF8_Obtain
	    FPTR IUTF8_Release
	    FPTR UTF8IFace_Expunge_UNIMPLEMENTED
	    FPTR UTF8IFace_Clone_UNIMPLEMENTED
	    FPTR IUTF8_CaseCmp
	    FPTR IUTF8_Cmp
	    FPTR IUTF8_Decode
	    FPTR IUTF8_DecodeLen
	    FPTR IUTF8_Encode
	    FPTR IUTF8_EstimateByteSize
	    FPTR IUTF8_FindGlyph
	    FPTR IUTF8_GetGlyph
	    FPTR IUTF8_GetGlyphAt
	    FPTR IUTF8_GetOffsetSize
	    FPTR IUTF8_GetSize
	    FPTR IUTF8_IndexOf
	    FPTR IUTF8_Insert
	    FPTR IUTF8_Left
	    FPTR IUTF8_Length
	    FPTR IUTF8_Merge
	    FPTR IUTF8_ModInsert
	    FPTR IUTF8_ModLeft
	    FPTR IUTF8_ModRemove
	    FPTR IUTF8_ModRight
	    FPTR IUTF8_ModSubString
	    FPTR IUTF8_ModToLower
	    FPTR IUTF8_ModToUpper
	    FPTR IUTF8_ModTrim
	    FPTR IUTF8_Printf
	    FPTR IUTF8_Remove
	    FPTR IUTF8_Right
	    FPTR IUTF8_SetGlyph
	    FPTR IUTF8_SubString
	    FPTR IUTF8_ToLower
	    FPTR IUTF8_ToUpper
	    FPTR IUTF8_Trim
	    FPTR IUTF8_TextUTF8Extent
	    FPTR IUTF8_TextUTF8ExtentN
	    FPTR IUTF8_TextUTF8Length
	    FPTR IUTF8_TextUTF8LengthN
	    FPTR IUTF8_TextUTF8
	    FPTR IUTF8_TextUTF8N
	    FPTR IUTF8_TextUTF32
	LABEL UTF8IFace_SIZE

#endif
