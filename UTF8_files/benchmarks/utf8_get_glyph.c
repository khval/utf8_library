
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define __USE_INLINE__

#include <proto/exec.h>
#include <proto/dos.h>
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
	*base = OpenLibrary( name , ver);
	if (*base) *interface = GetInterface( *base,  iname , iver, TAG_END );
}

#define OpenLib( a, ver, iname, iver ) \
	open_lib( #a ".Library",  ver ,  iname, iver, & a ## _base  , (struct a ## IFace *) &I ## a )

#define CloseLIB( name ) \
	if ( I##name)  DropInterface( (struct Interface *) I##name ); \
	if ( name  ## _base) CloseLibrary( name ## _base); \


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
	open_lib( "UTF8.library",  VER , "main", 1, &UTF8_base, (struct Interface **) &IUTF8 );

	if ((ILocale)&&(IDiskfont)&&(IUTF8))
	{
		locale = OpenLocale(NULL);
		CHAR_CODES = (ULONG *) ObtainCharsetInfo(DFCS_NUMBER, (ULONG) locale -> loc_CodeSet , DFCS_MAPTABLE);
		ret = ami_main(nargs,args);
		CloseLocale(locale);
	}
	else
	{
		if (!ILocale)	printf("Failed to open Local %s %d\n",  "locale.library", 53 );
		if (!IDiskfont)	printf("Failed to open Local %s %d\n",  "diskfont.library", 53 );
		if (!IUTF8)		printf("Failed to open Local %s %d\n",  "UF8.library", VER );
	}

	CloseLIB( Diskfont );
	CloseLIB( Locale );
	CloseLIB( UTF8 );

	return 0;
}


uint32 benchmark_values[]={1,128,2048,65536,2097152,67108864,0};



struct u8l
{
	unsigned char len;		// expected or current length
	unsigned char value;	// the value of data, in the package, first byte has variable length, 2en,3rd,4th byte etc. is 6 bits.
};

struct u8l u8_b1[256];
struct u8l u8_b2[256];

void dump_bin(const char *txt,int v)
{
	int n;
	printf("%s ",txt);
	for (n=7;n>=0;n--) printf("%c", v & (1u<<n) ? '1' : '0');
	printf("\n");
}

void init_table()
{
	int n,nn,prefix,cnt;
	int v;
	bzero( u8_b1, sizeof(u8_b1) );
	bzero( u8_b2, sizeof(u8_b2) );

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

ULONG _GetGlyph(unsigned char *b, unsigned int *len2)
{
	if (!(*b & 0xC0))
	{
		*len2=1;
		 return *b;
	}
	else
	{
		register unsigned int value;
		register unsigned int len = 1;
		struct u8l *xbyte;
		struct u8l *header = u8_b1 + (*b);
		register int hlen = header -> len;

		if (hlen)
		{
			value = header -> value;

			while (--hlen)
			{
				b++;	// get next byte..
				xbyte= u8_b2 + (*b);

				if (xbyte -> len == 1)	// branch prediction expected
				{
					value = (value << 6)  | xbyte -> value;
					len++;
//					continue;
				}
				else	// not expected
				{
					*len2=0;
					return 0;	
				}
			}
	
			*len2 = len;
			return value;
		}
		else
		{
			*len2=0;
			return 0;
		}
	}
}

#define loops 1u<<30


//define loops 1


ULONG (*__GetGlyph) (unsigned char *b, unsigned int *len2);

int ami_main(int nargs,char **args)
{
	unsigned char buffer[10];
	struct timeval tstart,tend;
	struct timezone tzp;

	int n1,n2,len,len2;
	int first_sec = 0,sec;

	init_table();


	__GetGlyph = _GetGlyph;	// use function pointer for more realistic benchmark

	for (n1=0;benchmark_values[n1];n1++)
	{
		printf("run test %d\n",n1);
		len = UTF8SetGlyph( benchmark_values[n1], buffer );
		buffer[len] = 0;

		gettimeofday(&tstart, &tzp);

		for (n2=0;n2<loops;n2++)
		{
			_GetGlyph(buffer, &len2);
		}

		gettimeofday(&tend, &tzp);

		sec = tend.tv_sec - tstart.tv_sec;
		if (n1==0) first_sec = tend.tv_sec - tstart.tv_sec;

		printf("glyph length %d time: %d seconds, loops %u, efficiency %d%%\n", len2, sec , loops, first_sec * 100 / sec);


	}


	return 0;
}

