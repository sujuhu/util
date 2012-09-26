#include <stdint.h>
#include <stdio.h>

/*
	Description:	递归创建目录
	Parameter:	lpszDir		目录全路径	
	Return:		TRUE		目录创建成功
				false		目录创建失败， GetLastError()
 */
errno_t makedirs(char* dirname)
{
	TCHAR szTemp[_MAX_PATH];
	TCHAR szDirName[_MAX_PATH];
	LPTSTR lpszDirName1 = szDirName;
	_tcscpy(szDirName,dirname);
	LPTSTR p=_tcschr(lpszDirName1,_T('\\'));
	if( p==NULL ) 
		return 0;

	if( strlen( dirname ) == 0 ) 
		return EINVAL;

	PathAddBackslash(szDirName); //增加一个"\\"

	memcpy( szTemp, lpszDirName1,(char*)p-(char*)lpszDirName1 );
	szTemp[p-lpszDirName1] = _T('\0');	

	while( _tcslen( szTemp ) > 0 ) {
		if( !PathFileExists( szTemp ) )
			if(!::CreateDirectory( szTemp, NULL ) )
				return false;

		p=_tcschr(p+1,_T('\\'));
		if(!p)
			break;
		memcpy(szTemp,lpszDirName1,(char*)p-(char*)lpszDirName1);
		szTemp[p-lpszDirName1]=L'\0';
	}
	return 0;
}


BOOL RemoveDirectoryCallback( LPCTSTR lpszDirectory, LPVOID lpParam );

BOOL RemoveFileCallback( LPCTSTR lpszFile, LPVOID lpParam );

/*
	Description:	递归删除目录
	Parameter:	dir		目录全路径	
	Return:		0		目录删除成功
				!0		目录删除失败，返回错误码
 */
bool rmdirs( const char* dir )
{
	//枚举系统目录下的所有文件
	char szWindowDirectory[512] = {0};
	strncpy( szWindowDirectory, dir, sizeof( szWindowDirectory ) - 3 );
	PathAddBackslash( szWindowDirectory );
	strncat( szWindowDirectory, "*", 1 );

	WIN32_FIND_DATA FindFileData = {0};
	CHFile hFind( HT_FILE_FIND );
	hFind = FindFirstFile( szWindowDirectory, &FindFileData );
	if( hFind.GetHandle() == INVALID_HANDLE_VALUE )	{
		return false;
	}

	do {
		if( 0 == strncmp( FindFileData.cFileName, "..", 2 )
			|| 0 == strncmp( FindFileData.cFileName, ".", 1 ) )
			continue;

		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			//是一个目录， 递归
			char szDirectory[512] = {0};
			strncpy( szDirectory, dir, 
					sizeof( szDirectory ) - 1 );
			PathAddBackslash( szDirectory );
			strncat( szDirectory, FindFileData.cFileName, 
				 sizeof( szDirectory) - strlen( szDirectory ) - 1 );
			
			//删除目录
			if( !RecursiveRemoveDirectory( szDirectory  ) ) {
				return false;
			}
		} else {
			//是一个文件, 进行删除文件

			//获取文件全路径
			char szPathName[512] = {0};
			strncpy( szPathName, dir, 
					sizeof( szPathName) - 1 );
			PathAddBackslash( szPathName );
			strncat( szPathName, FindFileData.cFileName, 
				 sizeof( szPathName ) - strlen( szPathName ) - 1 );
			if( !DeleteFile( szPathName ) ) {
				//获取文件属性, 如果是隐藏属性, 那么
				//删除隐藏属性后再删除
				ULONG attr = GetFileAttributes( szPathName );
				if( attr & FILE_ATTRIBUTE_HIDDEN ) {
					attr &= ~FILE_ATTRIBUTE_HIDDEN;
				} 

				if( attr & FILE_ATTRIBUTE_SYSTEM ) {
					attr &= ~FILE_ATTRIBUTE_SYSTEM;
				}

				if( attr & FILE_ATTRIBUTE_READONLY ) {
					attr &= ~FILE_ATTRIBUTE_READONLY;
				}
				
				SetFileAttributes( szPathName, attr );
				if( !DeleteFile( szPathName ) ) {
					return false;
				}
			} 
		}
	}while( FindNextFile( hFind.GetHandle(), &FindFileData ) != 0 );

	hFind.Close();
	if( !RemoveDirectory(dir) ) {
		return false;
	}

	return 0;
}

char* split_dir( const char* filename, 	OUT char* dir, 	int max_cch)
{
	char drive[_MAX_DRIVE];
	char _dir[_MAX_DIR];
	_splitpath(filename, drive,_dir, NULL,NULL);
	sprintf(dir,"%s%s", drive, _dir);
	return 0;
}

/*
Description:	清空指定的目录， 但不删除目录本身
Parameter:	lpszDir		目录全路径	
Return:		TRUE		目录清空成功
		false		目录清空失败， GetLastError()
 */
bool  clsdir( LPCTSTR lpszDirName )
{
	//枚举系统目录下的所有文件
	char szWindowDirectory[512] = {0};
	strncpy( szWindowDirectory, lpszDirName, sizeof( szWindowDirectory ) - 3 );
	PathAddBackslash( szWindowDirectory );
	strncat( szWindowDirectory, "*", 1 );

	WIN32_FIND_DATA FindFileData = {0};
	CHFile hFind( HT_FILE_FIND );
	hFind = FindFirstFile( szWindowDirectory, &FindFileData );
	if( hFind.GetHandle() == INVALID_HANDLE_VALUE )	{
		printf( "FindFirstFile Failed" );
		return false;
	}

	do {
		if( 0 == strncmp( FindFileData.cFileName, "..", 2 )
			|| 0 == strncmp( FindFileData.cFileName, ".", 1 ) )
			continue;

		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			//是一个目录， 递归
			char szDirectory[512] = {0};
			strncpy( szDirectory, lpszDirName, 
				sizeof( szDirectory ) - 1 );
			PathAddBackslash( szDirectory );
			strncat( szDirectory, FindFileData.cFileName, 
				sizeof( szDirectory) - strlen( szDirectory ) - 1 );

			//删除目录
			if( !RecursiveRemoveDirectory( szDirectory  ) ) {
				printf( "RecursiveRemoveDirectory Failed" );
				return false;
			}
		} else {
			//是一个文件, 进行删除文件

			//获取文件全路径
			char szPathName[512] = {0};
			strncpy( szPathName, lpszDirName, 
				sizeof( szPathName) - 1 );
			PathAddBackslash( szPathName );
			strncat( szPathName, FindFileData.cFileName, 
				sizeof( szPathName ) - strlen( szPathName ) - 1 );
			if( !DeleteFile( szPathName ) ) {
				//用户不再枚举
				printf( "DeleteFile Failed" );
				return false;
			} 
		}
	}while( FindNextFile( hFind.GetHandle(), &FindFileData ) != 0 );

	hFind.Close();

	SetLastError( ERROR_SUCCESS );
	return TRUE;
}

/*
Description:	枚举文件
Parameter:	lpszDirectrory		所要枚举的目录
		pfnDirectory		枚举目录回调函数
		lpDirectoryParam	枚举到目录时传给回调函数的参数
		pfnFile			枚举到文件时的回调函数
		lpFileParam		枚举到目录时传给回调函数的参数
Return:		TRUE:			枚举结束
		false:			ERROR_CANCELLED	用户取消枚举
		Other:			参见windows定义
*/
bool walkdir( LPCTSTR lpszDirectrory, 
	fnEnumDirectoryCallback pfnDirectory, 
	LPVOID lpDirectoryParam, 
	fnEnumFileCallback pfnFile, 
	LPVOID lpFileParam  )
{
	if( lpszDirectrory == NULL || IsBadStringPtr( lpszDirectrory, -1 ) ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return false;
	}

	if( pfnDirectory != NULL && IsBadCodePtr( (FARPROC)pfnDirectory ) ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return false;
	}

	if( pfnFile != NULL && IsBadCodePtr( (FARPROC)pfnFile  ) ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return false;
	}

	//枚举系统目录下的所有文件
	char szWindowDirectory[512] = {0};
	strncpy( szWindowDirectory, lpszDirectrory, sizeof( szWindowDirectory ) - 3 );
	PathAddBackslash( szWindowDirectory );
	strncat( szWindowDirectory, "*", 1 );

	WIN32_FIND_DATA FindFileData = {0};
	CHFile hFind( HT_FILE_FIND );
	hFind = FindFirstFile( szWindowDirectory, &FindFileData );
	if( hFind.GetHandle() == INVALID_HANDLE_VALUE )	{
		return false;
	}

	do {
		if( 0 == strncmp( FindFileData.cFileName, "..", 2 )
			|| 0 == strncmp( FindFileData.cFileName, ".", 1 ) )
			continue;

		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			//是一个目录， 递归
			char szDirectory[512] = {0};
			strncpy( szDirectory, lpszDirectrory, sizeof( szDirectory ) - 1 );
			PathAddBackslashA( szDirectory );
			strncat( szDirectory, FindFileData.cFileName, sizeof( szDirectory) - strlen( szDirectory ) - 1 );

			if( pfnDirectory != NULL ) {
				if( !pfnDirectory( szDirectory, lpDirectoryParam ) ) {
					//用户不再枚举
					SetLastError( ERROR_CANCELLED );
					return false;
				}
			}

			if( !walkdir( szDirectory, pfnDirectory, lpDirectoryParam, pfnFile, lpFileParam ) ) {
				if( GetLastError() == ERROR_CANCELLED ) { 
					SetLastError( ERROR_CANCELLED );
					return false;
				} else { 
					continue;
				}
			}
		} else {
			//是一个文件
			if( pfnFile != NULL ) {
				//获取文件全路径
				char szPathName[512] = {0};
				strncpy( szPathName, lpszDirectrory, sizeof( szPathName) - 1 );
				PathAddBackslashA( szPathName );
				strncat( szPathName, FindFileData.cFileName, sizeof( szPathName ) - strlen( szPathName ) - 1 );
				if( !pfnFile( szPathName, lpFileParam ) ) {
					//用户不再枚举
					SetLastError( ERROR_CANCELLED );
					return false;
				} 
			}
		}
	}while( FindNextFile( hFind.GetHandle(), &FindFileData ) != 0 );

	SetLastError( ERROR_SUCCESS );
	return true;
}
