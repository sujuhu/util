#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

void ReadIniString(
	const char*  ini_file, 
	const char*  app_name, 
	const char*  key, 
	const char*  default_value, 
	OUT char* value, 
	int value_len )
{
	char ini_path[512] = {0};
	GetApplicationDirectory( ini_path, sizeof( ini_path ) );
	PathAddBackslash( ini_path );
	PathAppend( ini_path, ini_file );
	memset(value, 0, value_len);
	GetPrivateProfileString( app_name, key, default_value, 
				 value, value_len - 1, ini_path );
}

int ReadIniInt( 
	const char* ini_file, 
	const char* app_name, 
	const char* key, 
	int default_value )
{
	char value[64] = {0};
	ReadIniString(ini_file, app_name, key, "", value, sizeof(value));
	if( strlen(value) <= 0) 
		return default_value;
	else 
		return atol( value );
}

void WriteIniString(
	const char* ini_file, 
	const char* app, 
	const char* key, 
	const char* format, ... )
{
	/*
	ASSERT( lpszFileName != NULL );
	ASSERT( lpszAppName != NULL );
	ASSERT( lpszKeyName != NULL );
	*/
	char ini_path[512] = {0};
	GetApplicationDirectory(ini_path, sizeof(ini_path));
	PathAddBackslash(ini_path);
	strncat(ini_path, ini_file, sizeof(ini_path) - strlen(ini_path) - 1);

	char buffer[1024] = {0};
	va_list arglist;
	va_start( arglist, format);
	vsprintf(&buffer[strlen(buffer)], format, arglist);
	va_end(arglist);
	buffer[sizeof(buffer) - 1] = '\0';

	WritePrivateProfileString(app, key, buffer, ini_path);	
}