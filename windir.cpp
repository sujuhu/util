#pragma warning( disable:4996 )

//显示文件浏览对话框， 允许其选择一个目录
bool SelectDirectory( UINT uFlag, OUT LPSTR lpszDirectory,  int cbBufSize )
{
	char   pszBuffer[260];   
	BROWSEINFO   bi;     LPITEMIDLIST   pidl;   
	bi.hwndOwner   =   NULL;   
	bi.pidlRoot   =   NULL;   
	bi.pszDisplayName   =   pszBuffer;   
	bi.lpszTitle   =   "Select Directory";   
	bi.ulFlags   =  BIF_RETURNONLYFSDIRS;   
	bi.lpfn   =   NULL;   
	bi.lParam   =   0;   
	if((pidl   =   SHBrowseForFolder(&bi))   !=   NULL) {   
		if( SHGetPathFromIDList(pidl,   pszBuffer) ) {   
			if( strlen( pszBuffer) <= 0 )
				return false; 
			strncpy( lpszDirectory, pszBuffer, cbBufSize - 1 );
			return true;
		}   
	} 
	return false;
}