#ifndef		__MEMORY_H_ZXK_20100410__
#define		__MEMORY_H_ZXK_20100410__

#ifdef __cplusplus
extern "C" {
#endif

//�ж��ڴ�������Ƿ�ȫ��
bool IsZeroMemory(char* pData, size_t dwDataSize );

/*
//�ڴ渴�ƣ����ʵ�֣�
ULONG	asmemcpy(void* Dst, void *Src, unsigned long Len);

//�ڴ����ã����ʵ�֣�
VOID*	asmemset(void *Dst, int cSet, unsigned long Len);

//�ڴ�Ƚϣ����ʵ�֣�
INT	asmemcmp(const void *Src, const void *Dst, unsigned long len);

//�ڴ��в���ָ�����ַ������ʵ�֣�
VOID*	asmemchr(const void *Src, int c, unsigned long len);
*/

//�ڴ�����
const char* memstr(const char *haystack, int hs, const char *needle, int ns);

#ifdef __cplusplus
}
#endif

#endif			//__MEMORY_H_ZXK_20100410__