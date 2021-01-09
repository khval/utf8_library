
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/locale.h>
#include <proto/diskfont.h>
#include <diskfont/diskfonttag.h>

#include <proto/utf8.h>

#undef Length

struct Locale *locale;
uint32 codeset;

#define DefineLib( a ) struct Library * a ## _base; struct a ## IFace *I ## a;

void open_lib( const char *name, int ver , const char *iname, int iver, struct Library **base, struct Interface **interface)
{
	*base = IExec -> OpenLibrary( name , ver);
	if (*base) *interface = IExec -> GetInterface( *base,  iname , iver, TAG_END );
}

#define OpenLib( a, ver, iname, iver ) \
	open_lib( #a ".Library",  ver ,  iname, iver, & a ## _base  , (struct a ## IFace *) &I ## a )

#define CloseLIB( name ) \
	if ( I##name) IExec -> DropInterface( (struct Interface *) I##name ); \
	if ( name  ## _base) IExec -> CloseLibrary( name ## _base); \


DefineLib( UTF8 );
DefineLib( Locale );
DefineLib( Diskfont );

ULONG *CHAR_CODES;

int ami_main(int nargs,char **args);

int main(int nargs,char **args)
{
	int ret;

	open_lib( "locale.library", 53 , "main", 1, &Locale_base, (struct Interface **) &ILocale  );
	open_lib( "diskfont.library", 53 , "main", 1, &Diskfont_base, (struct Interface **) &IDiskfont );
	open_lib( "UTF8.library", VER , "main", 1, &UTF8_base, (struct Interface **) &IUTF8 );

	if ((ILocale)&&(IDiskfont)&&(IUTF8))
	{
		locale = ILocale->OpenLocale(NULL);
		CHAR_CODES = (ULONG *) IDiskfont -> ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		ILocale->CloseLocale(locale);
	}
	else
	{
		if (!ILocale)	printf("Failed to open %s v %d\n",  "locale.library", 53 );
		if (!IDiskfont)	printf("Failed to open %s v %d\n",  "diskfont.library", 53 );
		if (!IUTF8)		printf("Failed to open %s v %d\n",  "UF8.library", VER );
	}

	CloseLIB( Diskfont );
	CloseLIB( Locale );
	CloseLIB( UTF8 );

	return ret;
}


void debug_Cmp(unsigned char *UTF8_alphabet,unsigned char *UTF8_one,  unsigned char *UTF8_two)
{
	int ret;
	ret = IUTF8->Cmp( UTF8_alphabet, UTF8_one,  UTF8_two);
	IUTF8->Printf(CHAR_CODES, "CMP(%s,%s)==%d\n", UTF8_one , UTF8_two, ret);	
}

void debug_CaseCmp(unsigned char *UTF8_alphabet,unsigned char *UTF8_one,  unsigned char *UTF8_two)
{
	int ret;
	ret = IUTF8->CaseCmp( UTF8_alphabet, UTF8_one,  UTF8_two);
	IUTF8->Printf(CHAR_CODES, "CaseCmp(%s,%s)==%d\n", UTF8_one , UTF8_two, ret);	
}

int ami_main(int nargs,char **args)
{
	unsigned char *UTF8;
	ULONG ret;
	char *txt;
	unsigned char *UTF8_str[4];
	unsigned char **PTR;
	unsigned char *UTF8_alphabet;

	// load it from a text in UTF8 format its better, if you have the wrong ISO (in Locale prefs) you see wrong symbols.
	UTF8_alphabet =  IUTF8 -> Encode( CHAR_CODES, (char *) "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ", MEMF_PRIVATE ); 

	UTF8_str[0] = IUTF8->Encode(CHAR_CODES,"æ",MEMF_PRIVATE);
	UTF8_str[1] = IUTF8->Encode(CHAR_CODES,"ø",MEMF_PRIVATE);
	UTF8_str[2] = IUTF8->Encode(CHAR_CODES,"å",MEMF_PRIVATE);
	UTF8_str[3] = NULL;

	printf("\n 7BIT ASCII / UTF8 \n");

	debug_Cmp(UTF8_alphabet,"alfa","beta");
	debug_Cmp(UTF8_alphabet,"beta","alfa");
	debug_Cmp(UTF8_alphabet,"alfa","alfa");
	debug_Cmp(UTF8_alphabet,"ze","zeta");
	debug_Cmp(UTF8_alphabet,"zeta","ze");

	printf("\n NOT UTF8 \n");

	debug_Cmp(UTF8_alphabet,"æ","ø");
	debug_Cmp(UTF8_alphabet,"ø","å");
	debug_Cmp(UTF8_alphabet,"å","a");

	printf("\n UTF8 \n");

	debug_Cmp(UTF8_alphabet,"z",UTF8_str[0]);  // cmp('z','æ')
	debug_Cmp(UTF8_alphabet,UTF8_str[0],UTF8_str[1]);  // cmp('æ','ø')
	debug_Cmp(UTF8_alphabet,UTF8_str[1],UTF8_str[2]);  // cmp('ø','å')
	debug_Cmp(UTF8_alphabet,UTF8_str[2],"a");  // cmp('å','æ')

	printf("\n UTF8 lowcase vs uppercase \n");

	debug_Cmp(UTF8_alphabet,"a","A");  
	debug_Cmp(UTF8_alphabet,"Z","z");  

	printf("\n UTF8 casecmp lowcase vs uppercase \n");

	debug_CaseCmp(UTF8_alphabet,"a","A");  
	debug_CaseCmp(UTF8_alphabet,"Z","z");  
	debug_CaseCmp(UTF8_alphabet,"a","b");  
	debug_CaseCmp(UTF8_alphabet,"b","a");  
	debug_CaseCmp(UTF8_alphabet,"A","B");  
	debug_CaseCmp(UTF8_alphabet,"B","A");  

	for (PTR=UTF8_str;*PTR;PTR++)
	{
		IExec->FreeVec(*PTR);
		*PTR=NULL;
	}
	
	return 0;
}

