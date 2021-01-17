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
	    FPTR IUTF8_UTF8CaseCmp
	    FPTR IUTF8_UTF8Cmp
	    FPTR IUTF8_UTF8Decode
	    FPTR IUTF8_UTF8DecodeLen
	    FPTR IUTF8_UTF8Encode
	    FPTR IUTF8_UTF8EstimateByteSize
	    FPTR IUTF8_UTF8FindGlyph
	    FPTR IUTF8_UTF8GetGlyph
	    FPTR IUTF8_UTF8GetGlyphAt
	    FPTR IUTF8_UTF8GetOffsetSize
	    FPTR IUTF8_UTF8GetSize
	    FPTR IUTF8_UTF8IndexOf
	    FPTR IUTF8_UTF8Insert
	    FPTR IUTF8_UTF8Left
	    FPTR IUTF8_UTF8Length
	    FPTR IUTF8_UTF8Merge
	    FPTR IUTF8_UTF8ModInsert
	    FPTR IUTF8_UTF8ModLeft
	    FPTR IUTF8_UTF8ModRemove
	    FPTR IUTF8_UTF8ModRight
	    FPTR IUTF8_UTF8ModSubString
	    FPTR IUTF8_UTF8ModToLower
	    FPTR IUTF8_UTF8ModToUpper
	    FPTR IUTF8_UTF8ModTrim
	    FPTR IUTF8_UTF8Printf
	    FPTR IUTF8_UTF8Remove
	    FPTR IUTF8_UTF8Right
	    FPTR IUTF8_UTF8SetGlyph
	    FPTR IUTF8_UTF8SubString
	    FPTR IUTF8_UTF8ToLower
	    FPTR IUTF8_UTF8ToUpper
	    FPTR IUTF8_UTF8Trim
	    FPTR IUTF8_UTF8TextExtent
	    FPTR IUTF8_UTF8TextExtentN
	    FPTR IUTF8_UTF8TextLength
	    FPTR IUTF8_UTF8TextLengthN
	    FPTR IUTF8_UTF8Text
	    FPTR IUTF8_UTF8TextN
	    FPTR IUTF8_UTF32Text
	LABEL UTF8IFace_SIZE

#endif
