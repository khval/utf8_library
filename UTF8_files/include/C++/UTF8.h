

class UTF8Lib
{
		bool is_copy;

	public:

		struct Library *base;
		struct UTF8IFace *interface;

		UTF8Lib()
		{
			is_copy = false;
			base = IExec -> OpenLibrary("UTF8.library", 3);
			if (base) interface = (struct UTF8IFace *)  IExec -> GetInterface( base, "main", 1, TAG_END );
		}
	
		~UTF8Lib()
		{
			if (is_copy == false)
			{
				if ( interface) IExec -> DropInterface( (struct Interface *) interface ); 
				if ( base) IExec -> CloseLibrary( base); 
			}
		}

		UTF8Lib( UTF8Lib &other )
		{
			is_copy = true;
			base =other.base;
			interface = other.interface;	
		}	

		unsigned char * Copy ( unsigned char *UTF8, ULONG mem_flags )
		{
			return interface->Copy(UTF8, mem_flags );
		}

		char * Decode (ULONG *codeset_page, unsigned char * UTF8)
		{
			return interface -> Decode( codeset_page, UTF8, MEMF_PRIVATE);
		}

		unsigned char *  Encode ( ULONG *codeset_page, char * ascii )
		{
			return interface -> Encode ( codeset_page, ascii, MEMF_PRIVATE );
		}

		int  EstimateByteSize ( ULONG glyph)
		{
			return interface -> EstimateByteSize( glyph );
		}

		ULONG  GetGlyph ( unsigned char *data, int *len)
		{
			return interface -> GetGlyph ( data, len );
		}

		ULONG  GetGlyphAt ( unsigned char *data, int pos, int *len)
		{
			return interface -> GetGlyphAt ( data, pos, len );
		}

		int  GetSize ( unsigned char *data)
		{
			return interface -> GetSize( data );
		}

		int IndexOf ( unsigned char *UTF8, unsigned char *UTF8_find )
		{
			return interface -> IndexOf ( UTF8, UTF8_find );
		}

		unsigned char *Insert (unsigned char *UTF8, int _pos, unsigned char *UTF8_Insert , ULONG flags )
		{
			return interface -> Insert ( UTF8, _pos, UTF8_Insert, flags );
		}

		unsigned char *Left( unsigned char *UTF8, int _len, ULONG mem_flags )
		{
			return interface -> Left ( UTF8,_len, mem_flags );
		}

		int Length ( unsigned char * UTF8)
		{
			return interface -> Length (UTF8);
		}

		unsigned char *Remove (unsigned char *UTF8, int _start, int _len, ULONG flags )
		{
			return interface -> Remove ( UTF8, _start, _len, flags );
		}

		unsigned char *Right ( unsigned char *UTF8, int _len, ULONG mem_flags )
		{
			return interface -> Right(UTF8, _len, mem_flags );
		}

		int SetGlyph ( ULONG glyph, unsigned char * data)
		{
			return interface -> SetGlyph ( glyph, data);
		}

		unsigned char * SubString ( unsigned char *UTF8, int _start, int _len, ULONG mem_flags )
		{
			return interface -> SubString (UTF8, _start, _len, mem_flags );
		}

		unsigned char *ToLower ( unsigned char *UTF8, unsigned char *alphabet_UTF8, ULONG mem_flags )
		{
			return interface -> ToLower ( UTF8, alphabet_UTF8, mem_flags );
		}

		unsigned char *ToUpper ( unsigned char *UTF8, unsigned char *alphabet_UTF8, ULONG mem_flags )
		{
			return interface -> ToUpper( UTF8, alphabet_UTF8, mem_flags );
		}

		BOOL ModInsert (unsigned char **UTF8, int _pos, unsigned char *UTF8_Insert , ULONG flags )
		{
			return interface -> ModInsert ( UTF8, _pos, UTF8_Insert, flags );
		}

		BOOL ModLeft ( unsigned char **UTF8, int _len, ULONG mem_flags )
		{
			return interface -> ModLeft(UTF8, _len, mem_flags );
		}

		BOOL ModRemove (unsigned char **UTF8, int _start, int _len, ULONG flags )
		{
			return interface -> ModRemove ( UTF8, _start, _len, flags );
		}

		BOOL ModRight ( unsigned char **UTF8, int _len, ULONG mem_flags )
		{
			return interface -> ModRight(UTF8, _len, mem_flags );
		}

		BOOL ModSubString ( unsigned char **UTF8, int _start, int _len, ULONG mem_flags )
		{
			return interface -> ModSubString (UTF8, _start, _len, mem_flags );
		}

		BOOL ModToLower ( unsigned char **UTF8, unsigned char *alphabet_UTF8, ULONG mem_flags )
		{
			return interface -> ModToLower ( alphabet_UTF8, UTF8, mem_flags );
		}

		BOOL ModToUpper ( unsigned char **UTF8, unsigned char *alphabet_UTF8, ULONG mem_flags )
		{
			return interface -> ModToUpper( alphabet_UTF8, UTF8, mem_flags );
		}

};



class UTF8Obj
{
		unsigned char *data;
		UTF8Lib *lib;	
		bool lib_copy;
		ULONG *codeset_page;
		ULONG mem_flags;

		public:

			UTF8Obj()
			{
				lib = new UTF8Lib();
				lib_copy = false;
				codeset_page = NULL;
				mem_flags = MEMF_PRIVATE;
				data = (unsigned char *)  IExec -> AllocVec(1,MEMF_CLEAR|mem_flags);
			}

			UTF8Obj( UTF8Lib *_lib, ULONG _mem_flags )
			{
				lib = _lib;
				lib_copy = true;
				codeset_page = NULL;
				mem_flags = _mem_flags;
				data = (unsigned char *)  IExec -> AllocVec(1,MEMF_CLEAR|mem_flags);
			}

			UTF8Obj( UTF8Lib *_lib, ULONG _mem_flags, ULONG *_codeset_page )
			{
				lib = _lib;
				lib_copy = true;
				codeset_page = _codeset_page;
				mem_flags = _mem_flags;
				data = (unsigned char *)  IExec -> AllocVec(1,MEMF_CLEAR|mem_flags );
			}

			UTF8Obj( UTF8Obj &other)
			{
				unsigned char *b, *d; 
				int size = 1;
				lib = other.lib;
				lib_copy = true;
				codeset_page = other.codeset_page;
				mem_flags = other.mem_flags;
				data = lib->Copy( other.data, mem_flags );
				*d = 0;
			}

			~UTF8Obj()
			{
				if ( data) IExec -> FreeVec( data );
				if ((lib_copy==false)&&(lib)) delete lib;
			}

			void ToLower ( unsigned char *alphabet_UTF8, ULONG mem )
			{
				if ((lib)&&(alphabet_UTF8)) lib -> ModToLower ( &data, alphabet_UTF8, mem_flags );
			}
	
			void ToUpper ( unsigned char *alphabet_UTF8 )
			{
				if ((lib)&&(alphabet_UTF8)) lib -> ModToUpper( &data, alphabet_UTF8, mem_flags );
			}

			void Remove ( int _start, int _len )
			{
				if (lib) lib -> ModRemove( &data, _start, _len , mem_flags );
			}

			void Insert( int _pos, unsigned char *utf8_insert )
			{
				if (lib) lib->ModInsert( &data, _pos, utf8_insert, mem_flags );
			}

			void Left( int _len )
			{
				if (lib) lib->ModLeft( &data, _len, mem_flags );
			}

			void Right( int _len )
			{
				if (lib) lib->ModRight( &data, _len, mem_flags );
			}

			void SubString( int _start, int _len )
			{
				if (lib) lib->ModSubString( &data, _start, _len, mem_flags );
			}

			int Length() 
			{
				return lib ? lib -> Length(data) : 0;
			}

			char *ConvertToAscII()
			{
				if ((lib)&&(codeset_page))
				{
					return ( char *) lib -> Decode(codeset_page, data );
				}
				else
				{
					return NULL;
				}
			}

			void CopyUTF8( unsigned char *UTF8 )
			{
				if (data) IExec->FreeVec(data);
				data = lib->Copy( UTF8, mem_flags );
			}
			
			void operator=( const char *txt ) 
			{
				if ((lib)&&(codeset_page))
				{
					if (data) IExec->FreeVec(data);
					data = lib -> Encode(  codeset_page, (char *) txt );
				}
			};

			ULONG operator[]( int pos) const
			{
				int len;
				if (pos>=lib -> Length(data) ) return 0;
				return lib ? lib->GetGlyphAt ( data, pos, &len ) : 0 ;
			};
};
