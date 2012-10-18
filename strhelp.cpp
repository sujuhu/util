#pragma warning(disable:4996)
#include <wtypes.h>
#include "typedef.h"
#include <stdio.h>
#include "strhelp.h"

bool	SplitString(
		const char* lpszSrc, 
		char chSplit, 
		OUT char* aString, 
		int cbMaxInRow, 
		int* pnMaxRow )
{
	int nLenSrc = 0;
	int nRow = 0;
	int nColumn = 0;
	int i=0;

	if( lpszSrc == NULL || IsBadStringPtr( lpszSrc, -1 ) ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	if( pnMaxRow == NULL || IsBadWritePtr( pnMaxRow, sizeof( int ) ) ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	if( aString == NULL || IsBadWritePtr( aString, cbMaxInRow * (*pnMaxRow) ) ) {
		SetLastError( ERROR_INVALID_USER_BUFFER );
		return FALSE;
	}

	nLenSrc = (int)strlen( lpszSrc );
	if( nLenSrc == 0 ) {
		*pnMaxRow = 0;
		return TRUE;
	}

	for( ; i < nLenSrc; i++ ) {
		if( lpszSrc[i] == chSplit ) {
			//说明有新行了
			nRow++;
			nColumn = 0;
			if( nRow >= ( *pnMaxRow ) ) {
				SetLastError( ERROR_INSUFFICIENT_BUFFER );
				return FALSE;
			}
		}
		else {

			if( nColumn >= ( cbMaxInRow - 1 ) ) {
				SetLastError( ERROR_OUT_OF_STRUCTURES );
				return FALSE;
			}
			*(aString + nRow*cbMaxInRow + nColumn ) = *(((char*)lpszSrc) + i );
			nColumn++;
		}
	}

	(*pnMaxRow) = nRow + 1 ;

	return TRUE;
}

/*
Description:	删除字符串左边的空格
Parameter：		lpsz			字符串
*/
void	TrimLeftString(char* lpsz )
{
	char* pString = lpsz;
	int nBlank = 0;
	int i=0;

	if( strlen( lpsz ) <=0 ) 
		return;

	//删除前面的空格
	while( TRUE ) {
		if( pString[i] == ' ' ) {
			nBlank++;
			i++;
		} else {
			//不为空格
			break;
		}
	}

	if( nBlank != 0 ) {
		//有空格
		int nCopy = (int)strlen( lpsz ) - nBlank;
		if( nCopy == 0 ) {
			//全部是空格
			lpsz[0] = 0;
		}
		strncpy( lpsz, pString + nBlank, nCopy );
	}
}

/*
	删除字符串右边的空格
 */
void	TrimRightString(char* lpsz)
{
	char*	pString = lpsz;
	size_t	len = strlen( lpsz );
	if( len <= 0 ) 
		return;

	do{
		if( lpsz[len - 1] == 0x20 ) {
			lpsz[len-1] = 0;
		} else {
			break;
		}
	}while( len--);
}

int	CombinString(
	char chSplit, 
	OUT char* aString, 
	int cbMaxInRow, 
	int nRow, 
	OUT char* lpszDst, 
	int nLenDst)
{
	int nWritePos = 0;
	int i, j;

	if( aString == NULL || IsBadReadPtr( aString, cbMaxInRow * nRow ) ) {
		SetLastError( ERROR_INVALID_USER_BUFFER );
		return FALSE;
	}

	for(  i=0; i < nRow ; i++ ) {
		for( j=0; j < cbMaxInRow; j++ ) {
			if( *( aString + i*cbMaxInRow + j) != '\0' ) {
				if( nWritePos >= nLenDst ) {
					SetLastError( ERROR_INSUFFICIENT_BUFFER );
					return 0;
				}
				lpszDst[nWritePos] = ( *( aString + i*cbMaxInRow + j ) );
				nWritePos++;
			} else {
				//填充一个分隔字符
				if( nWritePos >= nLenDst ) {
					SetLastError( ERROR_INSUFFICIENT_BUFFER );
					return 0;
				}
				lpszDst[nWritePos] = chSplit;
				nWritePos++;
				break;
			}
		}
	}

	return nWritePos;
}

/*
Description:	获取MAC地址格式的字符串
Parameter:		pMac			MAC地址的值（6个字节)
lpszMac			MAC字符串
cbStrSize		MAC字符串缓冲区长度
Return:			LPCTSTR			返回MAC字符串	
*/
char* MakeMacString(uint8_t mac[6], char* str, int max_cch)
{
	_snprintf( str, max_cch - 1, "%02X-%02X-%02X-%02X-%02X-%02X", 
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
	return str;
}

char* MakeIPString(uint8_t ip[4], char* str, int max_cch)
{
	_snprintf(str, max_cch-1, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
	return str;
}

char* MakeTimeString(
	SYSTEMTIME* tmSystem, 
	OUT LPSTR lpszDatatime, 
	DWORD cbBufferSize)
{
	if( lpszDatatime == NULL || IsBadWritePtr( lpszDatatime, cbBufferSize ) )  
		return NULL;
	ZeroMemory( lpszDatatime, cbBufferSize );
	_snprintf( lpszDatatime, cbBufferSize - 1, "%d-%02d-%02d %02d:%02d:%02d", 
		tmSystem->wYear,  tmSystem->wMonth, tmSystem->wDay, 
		tmSystem->wHour, tmSystem->wMinute, tmSystem->wSecond );
	return lpszDatatime;
}

bool MakeHexString(uint8_t* buffer, int size, OUT char* hex, int max_cch)
{
	if( buffer == NULL || hex == NULL)
		return false;

	memset(hex, 0, max_cch);
	for (int i = 0; i < size; i++ ) {
		char tmp[3] = {0}; 
		_snprintf(tmp, sizeof(tmp), "%02X", buffer[i] );
		strncat(hex, tmp, sizeof(tmp) - 1);
	}

	return true;
}

int	MakeHexBinary(const char* hex_str, IN uint8_t* buffer, int max_cch)
{
	int nLen = (int)strlen(hex_str);
	int i = 0;
	for( i=0; i < (nLen / 2) &&  i < (int)max_cch ; i++ ) {
		char tmp[3] = {0};
		strncpy( tmp, hex_str + i*2, 2 );
		buffer[i] = (BYTE)strtoul(tmp, NULL, 16 );
	}

	return i;
}



