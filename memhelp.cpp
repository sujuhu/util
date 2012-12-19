#include <stdio.h>
#include <memory.h>
#include "memhelp.h"


//ÅÐ¶ÏÄÚ´æ¿éÊý¾ÝÊÇ·ñÈ«Áã
bool is_zero_memory(const char* data, size_t size )
{
	for ( unsigned long i=0; i < size; i++ ) {
		if( *(data + i) != 0x0)
			return false;
	}
	return true;
}

/*
Description:	ÄÚ´æ×Ö·û´®ËÑË÷
Parameter:		haystack		ÄÚ´æ
hs			ÄÚ´æ³¤¶È
needle			ËùÒªËÑË÷µÄ×Ö·û´®
ns			×Ö·û´®³¤¶È
Return:			NULL			Î´ÕÒµ½
NOT NULL		³öÏÖµÄÎ»ÖÃ
*/
const char* memstr( const char *haystack, int hs, const char *needle, int ns )
{
	const char *pt, *hay;
	int n;

	if(hs < ns)
		return 0;

	if(haystack == needle)
		return haystack;

	if(!memcmp(haystack, needle, ns))
		return haystack;

	pt = hay = haystack;
	n = hs;

	//²éÕÒ×Ó´®µÄÎ»ÖÃÖ¸Õë
	while((pt = (char*)memchr( hay, needle[0], n )) != 0)  {
		n -= (int) (pt - hay);
		if(n < ns)
			break;

		if(!memcmp(pt, needle, ns)) {
			/*
			char tem1[14], tem2[14];
			memcpy(tem1, needle, 13);
			memcpy(tem2, pt, 13);
			*/
			return pt;
		}

		if( hay == pt ) {
			n--;
			hay++;
		} else  {
			hay = pt;
		}
	}

	return 0;
}