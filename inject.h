#ifndef _INJECT_HEADER_H_
#define _INJECT_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

bool StartInject(
	const char* lpszProcName,
	const char* lpszDllName,
	uint32_t dwInjectProcID);


#ifdef __cplusplus
}
#endif

#endif
