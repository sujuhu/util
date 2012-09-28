#ifndef		__MEMORY_H_ZXK_20100410__
#define		__MEMORY_H_ZXK_20100410__

#ifdef __cplusplus
extern "C" {
#endif

//判断内存块数据是否全零
bool IsZeroMemory(char* pData, size_t dwDataSize );

/*
//内存复制（汇编实现）
ULONG	asmemcpy(void* Dst, void *Src, unsigned long Len);

//内存设置（汇编实现）
VOID*	asmemset(void *Dst, int cSet, unsigned long Len);

//内存比较（汇编实现）
INT	asmemcmp(const void *Src, const void *Dst, unsigned long len);

//内存中查找指定的字符（汇编实现）
VOID*	asmemchr(const void *Src, int c, unsigned long len);
*/

//内存搜索
const char* memstr(const char *haystack, int hs, const char *needle, int ns);

#ifdef __cplusplus
}
#endif

#endif			//__MEMORY_H_ZXK_20100410__