/* :ts=4
 *  $VER: init.c $Revision$ (06-Mar-2014)
 *
 *  This file is part of UTF8.
 *
 *  Copyright (c) 2014 Hyperion Entertainment CVBA.
 *  All Rights Reserved.
 *
 * $Id$
 *
 * $Log$
 *
 *
 */


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <newlib.h>
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#include <stdarg.h>
#include <string.h>

#include "UTF8.library_rev.h"

/* Version Tag */
//include "UTF8.library_rev.h"
STATIC CONST UBYTE USED *verstag = (const UBYTE*) VERSTAG;

#include "libbase.h"

struct Library *DOSBase = NULL;
struct Library *NewLibBase = NULL;
struct Library *GraphicsBase = NULL;
struct Library *DiskfontBase = NULL;

struct ExecIFace *IExec = NULL;
struct DOSIFace *IDOS = NULL;
struct NewLibIFace *INewlib = NULL;
struct GraphicsIFace *IGraphics = NULL;
struct DiskfontIFace *IDiskfont = NULL;

struct u8l u8_b1[256];
struct u8l u8_b2[256];

/*
 * The system (and compiler) rely on a symbol named _start which marks
 * the beginning of execution of an ELF file. To prevent others from 
 * executing this library, and to keep the compiler/linker happy, we
 * define an empty _start symbol here.
 *
 * On the classic system (pre-AmigaOS 4.x) this was usually done by
 * moveq #0,d0
 * rts
 *
 */
int32 _start(void);

int32 _start(void)
{
    /* If you feel like it, open DOS and print something to the user */
    return RETURN_FAIL;
}


/* Open the library */
STATIC struct Library *libOpen(struct LibraryManagerInterface *Self, ULONG version)
{
    struct _Library *libBase = (struct _Library *) Self->Data.LibBase; 

    if (version != VERSION)
    {
        return NULL;
    }

    /* Add any specific open code here 
       Return 0 before incrementing OpenCnt to fail opening */


    /* Add up the open count */
    libBase->libNode.lib_OpenCnt++;
    return (struct Library *)libBase;

}


/* Close the library */
STATIC APTR libClose(struct LibraryManagerInterface *Self)
{
    struct Library *libBase = (struct Library *)Self->Data.LibBase;
    /* Make sure to undo what open did */


    /* Make the close count */
    ((struct Library *)libBase)->lib_OpenCnt--;

    return 0;
}


void close_libs( struct _Library *libBase )
{
	// Close DOS library
	if (IDOS) IExec -> DropInterface( (struct Interface *) IDOS);
	if ( DOSBase) IExec-> CloseLibrary( DOSBase);

	// Close NewLib library
	if (INewlib) IExec -> DropInterface( (struct Interface *) INewlib);
	if ( NewLibBase) IExec-> CloseLibrary( NewLibBase);

	// Close Diskfont library
	if ( IDiskfont) IExec -> DropInterface( (struct Interface *)  IDiskfont);
	if ( DiskfontBase) IExec-> CloseLibrary(  DiskfontBase);

	// Close Graphics library
	if (IGraphics) IExec -> DropInterface( (struct Interface *) IGraphics);
	if (GraphicsBase) IExec-> CloseLibrary( GraphicsBase);
}

/* Expunge the library */
STATIC APTR libExpunge(struct LibraryManagerInterface *Self)
{
    /* If your library cannot be expunged, return 0 */
    struct ExecIFace *IExec
        = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
    APTR result = (APTR)0;
    struct _Library *libBase = (struct _Library *)Self->Data.LibBase;

    if (libBase->libNode.lib_OpenCnt == 0)
    {
	     result = (APTR)libBase->segList;
        /* Undo what the init code did */

        IExec->Remove((struct Node *)libBase);
        IExec->DeleteLibrary((struct Library *)libBase);

	close_libs( libBase );

    }
    else
    {
        result = (APTR)0;
        libBase->libNode.lib_Flags |= LIBF_DELEXP;
    }
    return result;
}

void _memset( char *ptr, char n, int size )
{
	for (ptr; size; size --) *ptr = n;
}


void init_table()
{
	int n,nn,prefix,cnt;
	int v;
	_memset( (char*) u8_b1, 0, sizeof(u8_b1) );
	_memset( (char*) u8_b2, 0, sizeof(u8_b2) );

	// setup first byte in UTF8...

	// 0..127	// ASCII
	for (n=0;n<128;n++) 
	{
		u8_b1[n].len = 1;
		u8_b1[n].value = n;
	}

	// valid extended ranges...
	for (n=2;n<7;n++)
	{
		prefix = ((1u<<n)-1) << (8 -n);		// bit prefix == number of bytes
		cnt = (1u<<(7-n))-1;				// range = 0...x

		for ( v= 0 ; v<= cnt ; v++ ) 
		{
			nn = prefix | v;
			u8_b1[nn].len = n;
			u8_b1[nn].value = v;
		}
	}

	// setup byte x in utf8 glyph... there is only one valid range..

	// valid extended ranges...  
	for (n=0;n<=0x3F;n++)
	{
		nn = 0x80 | n;
		u8_b2[nn].len = 1;
		u8_b2[nn].value = n;
	}
}


/* The ROMTAG Init Function */
STATIC struct Library *libInit(struct Library *LibraryBase, APTR seglist, struct Interface *exec)
{
    struct _Library *libBase = (struct _Library *)LibraryBase;

    libBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    libBase->libNode.lib_Node.ln_Pri  = 0;
    libBase->libNode.lib_Node.ln_Name = (STRPTR) "UTF8.library";
    libBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    libBase->libNode.lib_Version      = VERSION;
    libBase->libNode.lib_Revision     = REVISION;
    libBase->libNode.lib_IdString     = (STRPTR) VSTRING;

    IExec = (struct ExecIFace *)exec;

	libBase -> MainCnt = 0;

	libBase->segList = (BPTR)seglist;


	INewlib = NULL;
	IDOS = NULL;
	IDiskfont = NULL;
	IGraphics = NULL;

	DOSBase = IExec->OpenLibrary("dos.library", 53);
	NewLibBase = IExec->OpenLibrary("newlib.library", 53);
	DiskfontBase = IExec->OpenLibrary("diskfont.library", 53);
	GraphicsBase = IExec->OpenLibrary("graphics.library", 54);

	if (DOSBase)
	{
		IExec ->DebugPrintF("UTF8.library: opened dos.library version 53\n");
		IDOS = (struct DOSIFace *) IExec->GetInterface( DOSBase,"main", 1, NULL);
	} 

	if (NewLibBase)
	{
		IExec ->DebugPrintF("UTF8.library: opened newlib.library version 53\n");
		INewlib = (NewLibIFace*) IExec->GetInterface( NULL /*NewLibBase*/,"main", 1, NULL);
	} 

	if (DiskfontBase)
	{
		IExec->DebugPrintF("UTF8.library: opened diskfont.library version 53\n");
		IDiskfont = (struct DiskfontIFace *) IExec->GetInterface( DiskfontBase,"main", 1, NULL);
	} 

	if (GraphicsBase)
	{
		IExec->DebugPrintF("UTF8.library: opened graphics.library version 53\n");
		IGraphics = (struct GraphicsIFace *) IExec->GetInterface( GraphicsBase,"main", 1, NULL);
	} 

	if (!( (DOSBase) && (NewLibBase) && (DiskfontBase) && (GraphicsBase) ) )
	{
		if (!IDOS) IExec->DebugPrintF("UTF8.library: can't open DOS.library version 53\n");
		if (!IDiskfont) IExec->DebugPrintF("UTF8.library: can't open Diskfont.library version 54\n");
		if (!IGraphics) IExec->DebugPrintF("UTF8.library: can't open Graphics.library version 53\n");

		close_libs( libBase );
		return NULL;
	}

	init_table();

	return (struct Library *)libBase;
}

/* ------------------- Manager Interface ------------------------ */
/* These are generic. Replace if you need more fancy stuff */
STATIC uint32 _manager_Obtain(struct LibraryManagerInterface *Self)
{
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

STATIC uint32 _manager_Release(struct LibraryManagerInterface *Self)
{
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,-1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

/* Manager interface vectors */
STATIC CONST APTR lib_manager_vectors[] =
{
	_manager_Obtain,
	_manager_Release,
	NULL,
	NULL,
	libOpen,
	libClose,
	libExpunge,
	NULL,
	(APTR)-1
};

/* "__library" interface tag list */
STATIC CONST struct TagItem lib_managerTags[] =
{
	{ MIT_Name,			(Tag)"__library"		},
	{ MIT_VectorTable,	(Tag)lib_manager_vectors},
	{ MIT_Version,		1						},
	{ TAG_DONE,			0						}
};

/* ------------------- Library Interface(s) ------------------------ */

#include "UTF8_vectors.c"

/* Uncomment this line (and see below) if your library has a 68k jump table */
/* extern APTR VecTable68K[]; */

STATIC CONST struct TagItem mainTags[] =
{
	{ MIT_Name,			(Tag)"main"			},
	{ MIT_VectorTable,	(Tag)main_v1_vectors	},
	{ MIT_Version,		1					},
	{ TAG_DONE,			0					}
};

STATIC CONST CONST_APTR libInterfaces[] =
{
	lib_managerTags,
	mainTags,
	NULL
};

STATIC CONST struct TagItem libCreateTags[] =
{
	{ CLT_DataSize,		sizeof(struct _Library)	},
	{ CLT_InitFunc,		(Tag)libInit			},
	{ CLT_Interfaces,	(Tag)libInterfaces		},
	/* Uncomment the following line if you have a 68k jump table */
	/* { CLT_Vector68K, (Tag)VecTable68K }, */
	{TAG_DONE,		 0 }
};


/* ------------------- ROM Tag ------------------------ */
STATIC CONST struct Resident lib_res USED =
{
	RTC_MATCHWORD,
	(struct Resident *)&lib_res,
	(APTR)(&lib_res + 1),
	RTF_NATIVE|RTF_AUTOINIT, /* Add RTF_COLDSTART if you want to be resident */
	VERSION,
	NT_LIBRARY, /* Make this NT_DEVICE if needed */
	0, /* PRI, usually not needed unless you're resident */
	"UTF8.library",
	VSTRING,
	(APTR)libCreateTags
};

