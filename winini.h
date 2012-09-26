#ifndef _WININI_HEADER_H_
#define _WININI_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

void ReadIniString(
	const char*  ini_file, 
	const char*  app_name, 
	const char*  key, 
	const char*  default_value, 
	OUT char* value, 
	int value_len );

int ReadIniInt( 
	const char* ini_file, 
	const char* app_name, 
	const char* key, 
	int default_value );

void WriteIniString(
	const char* ini_file, 
	const char* app, 
	const char* key, 
	const char* format, ... );

#ifdef __cplusplus
};
#endif

#endif