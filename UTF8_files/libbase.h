

#ifdef __amigaos4__
#include <proto/diskfont.h>
#include <proto/graphics.h>
#endif

extern struct Library *DOSBase;
extern struct Library *NewLibBase;
extern struct Library *GraphicsBase;
extern struct Library *DiskfontBase;

#ifdef _MSC_VER

#define sys_alloc(size,type) free(size,type)
#define sys_alloc_clear(size,type) malloc(size)
#define sys_alloc(size) malloc(size)

#endif

#ifdef __amigaos3__

#define sys_alloc(size,type) AllocVec(size,type)
#define sys_alloc_clear(size,type) AllocVec(size, MEMF_CLEAR|type)

#endif

#ifdef __amigaos4__

extern struct ExecIFace *IExec;
extern struct DOSIFace *IDOS;
extern struct NewLibIFace *INewlib;
extern struct GraphicsIFace *IGraphics;
extern struct DiskfontIFace *IDiskfont;

#define sys_alloc(size,type) AllocVecTags(size,AVT_Type, type, TAG_END)
#define sys_alloc_clear(size,type) AllocVecTags(size,AVT_Type,type,AVT_ClearWithValue,0,TAG_END)

#endif

struct _Library
{
	struct Library libNode;
	BPTR segList;
    // If you need more data fields, add them here 
	int MainCnt;
};


struct u8l
{
	unsigned char len;		// expected or current length
	unsigned char value;	// the value of data, in the package, first byte has variable length, 2en,3rd,4th byte etc. is 6 bits.
};

extern struct u8l u8_b1[256];
extern struct u8l u8_b2[256];

