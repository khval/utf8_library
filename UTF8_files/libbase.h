
#include <proto/diskfont.h>
#include <proto/graphics.h>

struct Library *DOSBase;
struct Library *NewLibBase;
struct Library *GraphicsBase;
struct Library *DiskfontBase;


#ifdef __amigaos3__

#define sys_alloc(size,type) AllocVec(size,type)
#define sys_alloc_clear(size,type) AllocVec(size, MEMF_CLEAR|type)

#endif

#ifdef __amigaos4__

struct ExecIFace *IExec;
struct DOSIFace *IDOS;
struct NewLibIFace *INewlib;
struct GraphicsIFace *IGraphics;
struct DiskfontIFace *IDiskfont;

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

