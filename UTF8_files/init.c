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
#include <libraries/UTF8.h>
#include <proto/UTF8.h>
#include <stdarg.h>

#include "UTF8.library_rev.h"

/* Version Tag */
//include "UTF8.library_rev.h"
STATIC CONST UBYTE USED verstag[] = VERSTAG;

#include "libbase.h"

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
	if (libBase->IDOS) libBase -> IExec -> DropInterface( (struct Interface *) libBase-> IDOS);
	if (libBase -> DOS_base) libBase -> IExec-> CloseLibrary( libBase -> DOS_base);

	// Close Diskfont library
	if (libBase -> IDiskfont) libBase -> IExec -> DropInterface( (struct Interface *) libBase-> IDiskfont);
	if (libBase -> Diskfont_base) libBase -> IExec-> CloseLibrary( libBase -> Diskfont_base);

	// Close Graphics library
	if (libBase->IGraphics) libBase -> IExec -> DropInterface( (struct Interface *) libBase-> IGraphics);
	if (libBase -> Graphics_base) libBase -> IExec-> CloseLibrary( libBase -> Graphics_base);
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


/* The ROMTAG Init Function */
STATIC struct Library *libInit(struct Library *LibraryBase, APTR seglist, struct Interface *exec)
{
    struct _Library *libBase = (struct _Library *)LibraryBase;
    struct ExecIFace *IExec UNUSED = (struct ExecIFace *)exec;

    libBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    libBase->libNode.lib_Node.ln_Pri  = 0;
    libBase->libNode.lib_Node.ln_Name = "UTF8.library";
    libBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    libBase->libNode.lib_Version      = VERSION;
    libBase->libNode.lib_Revision     = REVISION;
    libBase->libNode.lib_IdString     = VSTRING;

	libBase -> MainCnt = 0;

	libBase->segList = (BPTR)seglist;

	libBase -> IExec = IExec;

	libBase->IDOS = NULL;
	libBase->IDiskfont = NULL;
	libBase->IGraphics = NULL;

	libBase-> DOS_base = IExec->OpenLibrary("dos.library", 53);
	libBase-> Diskfont_base = IExec->OpenLibrary("diskfont.library", 53);
	libBase-> Graphics_base = IExec->OpenLibrary("graphics.library", 54);

	if (libBase->DOS_base)
	{
		libBase -> IExec -> DebugPrintF("UTF8.library: opened DOS.library version 53\n");
		libBase->IDOS = (struct DOSIFace *) IExec->GetInterface(libBase-> DOS_base,"main", 1, NULL);
	} 

	if (libBase->Diskfont_base)
	{
		libBase -> IExec -> DebugPrintF("UTF8.library: opened Diskfont.library version 53\n");
		libBase->IDiskfont = (struct DiskfontIFace *) IExec->GetInterface(libBase-> Diskfont_base,"main", 1, NULL);
	} 

	if (libBase->Graphics_base)
	{
		libBase -> IExec -> DebugPrintF("UTF8.library: opened Graphics.library version 53\n");
		libBase->IGraphics = (struct GraphicsIFace *) IExec->GetInterface(libBase-> Graphics_base,"main", 1, NULL);
	} 


	if (!( (libBase->DOS_base) && (libBase->Diskfont_base) && (libBase->Graphics_base) ) )
	{
		if (!libBase->IDOS) libBase -> IExec -> DebugPrintF("UTF8.library: can't open DOS.library version 53\n");
		if (!libBase->IDiskfont) libBase -> IExec -> DebugPrintF("UTF8.library: can't open Diskfont.library version 54\n");
		if (!libBase->IGraphics) libBase -> IExec -> DebugPrintF("UTF8.library: can't open Graphics.library version 53\n");

		close_libs( libBase );
		return NULL;
	}


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
	{ MIT_VectorTable,	(Tag)main_vectors	},
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

