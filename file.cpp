#pragma warning( disable:4996 )
#include <wtypes.h>
#include <stdio.h>
#include <shlobj.h>
#include <tchar.h>
#include <Shlwapi.h>
#include "file.h"

#pragma comment( lib, "shlwapi.lib")
#pragma comment( lib, "shell32.lib")


//生成临时文件的全路径
char* make_temp_filename(char* lpszBuffer, int cbBufSize )
{
	if (lpszBuffer == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if( FAILED(GetTempPath( cbBufSize, lpszBuffer )))
		return NULL;

	char strTempFileName[260] = {0};
	if( FAILED( GetTempFileName( lpszBuffer, NULL, 0, strTempFileName )))
		return NULL;

	strncpy( lpszBuffer, strTempFileName, cbBufSize - 1 );
	return lpszBuffer;
}