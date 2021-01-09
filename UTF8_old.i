
#define UTF8_MBYTE 0xC0
#define UTF8_EXTRABYTE 0x80
#define NOT_ASC7 0x80

int UTF8_size_check(char *ptr,int txtlen)
{
	unsigned char bit;
	unsigned int len = 0;
	int bytes;
	int n;

	for(n=0;n<txtlen;n++)
	{
		switch (ptr[n] & UTF8_MBYTE) 
		{
			case UTF8_MBYTE:

				len++;
				bytes = 0;
				for (bit=0x40; (ptr[n] & bit) > 0; bit=(bit>>1) )
				{
					len++;
					bytes ++;

					if (bit==0) 
					{
						printf("bit over run\n");
						return 0;
					}
				}

				if (bytes==0) return 0;

				while(bytes>0)
				{
					n++;
					if ( (ptr[n] & UTF8_MBYTE) != UTF8_EXTRABYTE) 
					{
//						printf(" Excpected extra byte not found  %x\n", ptr[n] & UTF8_MBYTE);
						return 0;
					}
					bytes--;
				}

				break;

			case UTF8_EXTRABYTE:

				// Unexpected extra byte
				return 0;	// they are removed
			
			default:

				len++;	// 7 bit ASCII
				break;
		}
	}
	return len;
}

int UTF8_len(char *ptr,int txtlen)
{
	int n,len = 0;

	for(n=0;n<txtlen;n++)
	{
		switch (ptr[n] & UTF8_MBYTE)
		{
			case UTF8_MBYTE:

				len++;	// UTF8 multi byte
				break;

			case UTF8_EXTRABYTE:	

				// not a char
				break;

			default:

				len++;	// 7 bit ASCII
		}
	}
	return len;
}

int _strlen(char *txt)
{
	int n,l = 0;
	for (n=0;txt[n]!=0;n++) l++;
	return l;
}


int is_UTF8(char *txt,int txtlen)
{
	return ( txtlen == UTF8_size_check(txt,txtlen) );
}

int is_ASCII_8(char *txt, int txtlen)
{
	int n = 0;
	int c = 0;

	for (n=0;n<txtlen;n++)
	{
		if (txt[n] & 0x80) c=c++;
	}

	return c;
}

int is_ASCII(char *txt,int txtlen)
{
	int utf_size;

	utf_size = UTF8_size_check(txt,txtlen);

	return (( txtlen != utf_size ) || (utf_size == 0) || (is_ASCII_8(txt,txtlen)==0) );
}



void display_ASCII_numbers(char *ptr,int txtlen)
{
	int n;
	printf("ASCII: %s\n",ptr);

	for(n=0;n<txtlen;n++)
	{
			printf( "%c (%x) ", ptr[n],ptr[n]);
	}

	printf("\n");

}

void display_UTF8_numbers(char *ptr,int txtlen)
{
	unsigned char bit;
	unsigned int len = 0;
	int n, num, bytes;

	printf("UTF8: ");
	printf("%s\n",ptr);


	for(n=0;n<txtlen;n++)
	{
		if (ptr[n] & UTF8_MBYTE)
		{
			bytes = 0;

			for (bit=0x40; (ptr[n] & bit) > 0; bit=(bit>>1) )
			{
				bytes++;
				if (bit==0) return;
			}
			num = ptr[n] & 0x1F;

			while(bytes>0)
			{
				n++;
				if ( (ptr[n] & UTF8_MBYTE ) != UTF8_EXTRABYTE) 
				{
					printf("\nFAILED: Not UTF8  \n");

					return;	// not UTF-8 Format
				}

				num = (num << 6) + (ptr[n] & 0x3F);
				bytes--;
			}

			printf( "%c (%x) ", num,num);
		}
		else
		{
			printf( "%c ", ptr[n]);
		}
	}

	printf("\n");


}

char *ASCII_UTF8(char *txt, int txtlen)
{
	int n,nn;
	int nsize;
	char *ntxt;

	nsize = txtlen + is_ASCII_8(txt,txtlen) + 1;

	if (ntxt = (void *) IExec -> AllocVec(nsize, MEMF_ANY))
	{
		nn = 0;

		for (n=0;n<txtlen;n++)
		{
			if (txt[n] & NOT_ASC7)
			{
				ntxt[nn] = (txt[n] >> 6) | UTF8_MBYTE;
				nn++;

				ntxt[nn] = (txt[n] & 0x3F) | UTF8_EXTRABYTE;
				nn++;
			}
			else
			{
				ntxt[nn] = txt[n];
				nn++;
			}
		}
		ntxt[nsize-1] = 0;
	} 
	return ntxt;
}

void get_info(char *txt)
{
	int txtlen = _strlen(txt);

	printf("\nReal text size %i\n",txtlen);

	printf("UTF8 size %i\n",UTF8_size_check(txt,txtlen) );
	printf("UTF8 len %i\n\n",UTF8_len(txt,txtlen) );

	printf(" UTF-8: %s\n", is_UTF8(txt,txtlen) ? "YES" : "NO" );
	printf(" ASCII: %s\n", is_ASCII(txt,txtlen) ? "YES" : "NO" );
	printf(" ASCII: %s\n", is_ASCII_8(txt,txtlen) > 0 ? "8 bit" : "7 bit" );

	printf(" Found %i 8bit char\n\n", is_ASCII_8(txt,txtlen) );

	if (is_UTF8(txt,txtlen) )
	{
		display_UTF8_numbers(txt,txtlen);
	}
	else
	{
		display_ASCII_numbers(txt,txtlen);
	}

}
