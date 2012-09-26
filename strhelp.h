#ifndef __STRING_H_ZXK_20100410
#define __STRING_H_ZXK_20100410

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned short  Length;
	unsigned short  MaximumLength;
	wchar_t*  Buffer;
}UNICODE_STRING, *PUNICODE_STRING;

/*
Description:		�ָ��ַ���
Parameter:			lpszSrc	 ��Ҫ�ָ���ַ���
chSplit  �ָ��ַ�
aString	 �ָ������ַ��������ָ��
cbMaxInRow	ÿһ���ַ���һ��������ַ���
pnMaxRow	�ָ��˶�����
Return:				TRUE	 �ָ�ɹ�
FALSE	 �ָ�ʧ��
*/
bool	SplitString(
		const char* lpszSrc, 
		char chSplit, 
		OUT char* aString, 
		int cbMaxInRow, 
		int* pnMaxRow );

//ɾ���ַ������Ŀո�
void	TrimLeftString(char* lpsz);

//ɾ���ַ����Ҳ�Ŀո�
void	TrimRightString(char* lpsz);

int	CombinString(
	char chSplit, 
	OUT char* aString, 
	int cbMaxInRow, 
	int nRow, 
	OUT char* lpszDst, 
	int nLenDst);

char* MakeMacString(uint8_t mac[6], char* str, int max_cch);

char* MakeIPString(uint8_t ip[4], char* str, int max_cch);

char* MakeTimeString(
	SYSTEMTIME* tmSystem, 
	OUT LPSTR lpszDatatime, 
	DWORD cbBufferSize);

bool MakeHexString(uint8_t* buffer, DWORD size, OUT char* hex, DWORD max_cch);

int	MakeHexBinary(const char* hex_str, IN uint8_t* buffer, int max_cch);


#ifdef __cplusplus
}
#endif

#endif //__STRING_H_ZXK_20100410


