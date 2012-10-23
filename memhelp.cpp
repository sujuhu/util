#include <stdio.h>
#include <memory.h>
#include "memhelp.h"


//ÅÐ¶ÏÄÚ´æ¿éÊý¾ÝÊÇ·ñÈ«Áã
bool IsZeroMemory( char* data, size_t size )
{
	for ( unsigned long i=0; i < size; i++ ) {
		if( *(data + i) != 0x0)
			return false;
	}
	return true;
}

/*
unsigned long asmemcpy(void* Dst, void *Src, unsigned long Len)
{
	__asm
	{
		push    ecx;
		push    esi;
		push    edi;
		mov     ecx, Len;
		mov     esi, Src;
		mov     edi, Dst;
		mov     eax, ecx;
		shr     ecx, 2;
		repne movsd;
		mov     cl, al;
		and     cl, 3;
		repne movsb;
		pop     edi;
		pop     esi;
		pop     ecx;
	}
	return Len; // ·µ»Ø¸´ÖÆµÄ×Ö·û¸öÊý
}


void *asmemset(void *Dst, int cSet, unsigned long Len)
{
	__asm
	{
		pushad;
		mov     ecx, Len;
		mov     edi, Dst;
		mov     eax, cSet;
		mov     edx, ecx;
		shr     ecx, 2;
		rep stosd;
		mov     cl, dl;
		and     cl, 3;
		rep stosb;
		popad;
	}
	return Dst; // ·µ»ØDstµÄÖµ
}

int asmemcmp(const void *Src, const void *Dst, unsigned long len)
{
	int r = 0;
	__asm
	{
		push    ecx;
		push    esi;
		push    edi;
		mov     ecx, len;
		mov     esi, Src;
		mov     edi, Dst;
		mov     eax, ecx;
		shr     ecx, 2;
		repne cmpsd;
		jnz     _NotSame;
		mov     cl, al;
		and     cl, 3;
		repne cmpsb;
		jz      _End;
_NotSame:
		ja       _Big;
		mov      eax, -1;
		mov      r, eax;
		jmp      _End;
_Big:
		mov      eax, 1;
		mov      r, eax;
_End:
		pop     edi;
		pop     esi;
		pop     ecx;
	}
	return r;
}

// ÔÚ¶þ½øÖÆ´®ÖÐËÑË÷Ò»¸ö×Ö·û£¬·µ»Ø¸Ã×Ö·ûµÄµØÖ·£¬ËÑË÷²»µ½·µ»ØNULL
void *asmemchr(const void *Src, int c, unsigned long len)
{
	void *r = 0;
	__asm
	{
		push    ecx;
		push    esi;
		push    edi;
		mov     ecx, len;
		mov     edi, Src;
		mov     eax, c;
		repne scasb;
		jnz      _End;
		mov      eax, edi;
		dec      eax;
		mov      r, eax;
_End:
		pop     edi;
		pop     esi;
		pop     ecx;
	}
	return r;
}
*/


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