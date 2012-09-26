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
BOOL GetTmpPathName( LPSTR lpszBuffer, int cbBufSize )
{
	if( IsBadWritePtr( lpszBuffer, cbBufSize ) )
		return FALSE;

	if( FAILED(GetTempPath( cbBufSize, lpszBuffer )))
		return FALSE;

	char strTempFileName[260] = {0};
	if( FAILED( GetTempFileName( lpszBuffer, NULL, 0, strTempFileName )))
		return FALSE;

	strncpy( lpszBuffer, strTempFileName, cbBufSize - 1 );
	return TRUE;
}
stat()