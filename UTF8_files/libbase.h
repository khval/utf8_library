struct _Library
{
	struct Library libNode;
	BPTR segList;
    // If you need more data fields, add them here 
	struct Library *DOS_base;
	struct Library *Graphics_base;
	struct Library *Diskfont_base;
	struct ExecIFace *IExec;
	struct DOSIFace *IDOS;
	struct GraphicsIFace *IGraphics;
	struct DiskfontIFace *IDiskfont;
	int MainCnt;
};

