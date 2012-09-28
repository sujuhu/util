#include <stdint.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include "inject.h"

#pragma warning(disable:4996)

HANDLE WINAPI OsCreateRemoteThread2(
	HANDLE hProcess,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId);

//Զ���̵߳Ĳ���
typedef struct _THREAD_PARAM
{
	char	szDllName[1024];
	DWORD	dwLoadLibraryA;
	//	DWORD   dwOutPutDubegString;
}THREAD_PARAM, *PTHREAD_PARAM;

//��Ȩ
VOID EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName)
{
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;

	LookupPrivilegeValue(NULL,szPrivName,&tp.Privileges[0].Luid);
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);

	//return((GetLastError() == ERROR_SUCCESS));
}

DWORD __stdcall ThreadProc(PTHREAD_PARAM param)
{

	FARPROC lpLoadLibraryA	= (FARPROC) param->dwLoadLibraryA;

	if((DWORD)lpLoadLibraryA<100000) 
	{
		//_asm int 3;
		return 0;
	}
	if(*(DWORD*)lpLoadLibraryA==0)
	{
		return 0;	//��������ַ����Ĵ���ȫ��0Ҳ������
	}
	((HMODULE(__stdcall *)(LPCSTR))param->dwLoadLibraryA)(param->szDllName);
	return 0;
}

PVOID GxAlloc(HANDLE hTargetProc,DWORD dwSize)
{
	return VirtualAllocEx(
		hTargetProc, 
		NULL, 
		dwSize, 
		MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE); 
}

bool x_CreateRemoteThread(HANDLE hTargetProc,LPVOID pCode,LPVOID pParam)
{
	if(CreateRemoteThread(hTargetProc,0,0,(DWORD (__stdcall *)(void *))pCode ,pParam,0,NULL)==NULL) 
		return false;
	return true;
}

bool x_CreateRemoteThread2(HANDLE hTargetProc,LPVOID pCode,LPVOID pParam)
{
	if(OsCreateRemoteThread2(hTargetProc,0,0,(DWORD (__stdcall *)(void *))pCode ,pParam,0,NULL)==NULL) return FALSE;
	return true;
}

DWORD GetPidByNameEx(LPCTSTR szName)
{
	DWORD dwRet=0;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe32={0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hProcessSnap == INVALID_HANDLE_VALUE)return 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(hProcessSnap, &pe32)==FALSE) return 0;

	do {
		int nLen1 = strlen(szName); 
		if(nLen1>=15) nLen1=15;

		if( strnicmp(szName,pe32.szExeFile, nLen1 ) == 0 ) {
			dwRet=pe32.th32ProcessID;
			break;
		}
	}while (Process32Next(hProcessSnap,&pe32));


	if(hProcessSnap !=INVALID_HANDLE_VALUE)CloseHandle(hProcessSnap);

	return dwRet;

}

bool StartInject(
	const char* lpszProcName,
	const char* lpszDllName,
	uint32_t dwInjectProcID)
{
	HANDLE hTargetProc = NULL;
	LPVOID pParam = NULL;				//�����Ĵ�ŵ�ַ
	THREAD_PARAM localParam;			//��ű��ز����ı���	
	LPVOID pCode = NULL;				//����Ĵ�ŵ�ַ
	DWORD dwTargetProcID = dwInjectProcID;

	if(dwTargetProcID==-1)
		dwTargetProcID = GetPidByNameEx(lpszProcName);
	DWORD  dwCodeSize = 1000; //����Ĵ�С
	HANDLE hThread  = NULL;	 //Զ���߳̾��
	HANDLE hToken   = NULL;

	DWORD dwTemp=0;
	DWORD dwTempThreadID =0;



	//���û�л�ȡ�������˳�
	if(dwTargetProcID==0) 	
	{		
		return false;	
	}

	//��ʼ׼����Զ���̲߳���
	memset( localParam.szDllName, 0, sizeof(localParam.szDllName) );
	strncpy( localParam.szDllName, lpszDllName, sizeof(localParam.szDllName)-1);

	//	localParam.dwOutPutDubegString = (DWORD)GetProcAddress(hModule, "OutputDebugStringA");
	localParam.dwLoadLibraryA = (DWORD)LoadLibraryA;

	bool bRet = false;

	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
	EnablePrivilege(hToken,SE_DEBUG_NAME);

	do {
		hTargetProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwTargetProcID);
		if(NULL == hTargetProc)	{
			break;
		}

		//��Ŀ������з����Ų����Ŀռ䣬������ָ�뱣�浽 pParam
		pParam = GxAlloc(hTargetProc, sizeof(THREAD_PARAM));

		if(NULL == pParam) 
		{	
			break;
		}


		//��Ŀ�����д��������
		if(!WriteProcessMemory(hTargetProc, pParam, &localParam, sizeof(THREAD_PARAM),0)) 
		{
			break;
		}

		//��������ŵĿռ�	
		pCode = GxAlloc(hTargetProc,dwCodeSize);

		if(NULL == pCode)
		{
			break;
		}

		//��Ŀ�����д����
		if(!WriteProcessMemory(hTargetProc, pCode, ThreadProc, dwCodeSize,0)) 
		{		
			break;
		}

		bRet = x_CreateRemoteThread(hTargetProc, pCode, pParam);
		if (bRet == false)
		{
			bRet = x_CreateRemoteThread2(hTargetProc, pCode, pParam);
		}		
	} while (false);


	if(hToken)
	{
		CloseHandle(hToken);
	}
	if(hTargetProc)
	{
		CloseHandle(hTargetProc);
	}

	return bRet;
}



typedef LONG  NTSTATUS;

typedef struct _LSA_UNICODE_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
}LSA_UNICODE_STRING, *PLSA_UNICODE_STRING; 

typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;


typedef struct _OBJECT_ATTRIBUTES 
{
	ULONG Length;
	HANDLE RootDirectory;
	UNICODE_STRING *ObjectName;
	ULONG Attributes;
	PSECURITY_DESCRIPTOR SecurityDescriptor;
	PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
}OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;


typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

typedef struct _INITIAL_TEB{ 
	
	PVOID OldStabckBase; 
	
	PVOID OldStackLimit; 
	
	PVOID StackBase; 
	
	PVOID StackLimit; 
	
	PVOID StackAllocationBase; 
	
}INITIAL_TEB, *PINITIAL_TEB;


typedef NTSTATUS (__stdcall *ZwAllocateVirtualMemory)(  
						IN HANDLE               ProcessHandle,
						IN OUT PVOID            *BaseAddress,
						IN ULONG                ZeroBits,
						IN OUT PULONG           RegionSize,
						IN ULONG                AllocationType,
						IN ULONG                Protect);


 
typedef NTSTATUS (__stdcall *ZwWriteVirtualMemory)( 
					 IN HANDLE               ProcessHandle,
					 IN PVOID                BaseAddress,
					 IN PVOID                Buffer,
					 IN ULONG                NumberOfBytesToWrite,
					 OUT PULONG              NumberOfBytesWritten OPTIONAL );


typedef NTSTATUS (__stdcall *ZwProtectVirtualMemory)(  
					   IN HANDLE               ProcessHandle,
					   IN OUT PVOID            *BaseAddress,
					   IN OUT PULONG           NumberOfBytesToProtect,
					   IN ULONG                NewAccessProtection,
					   OUT PULONG              OldAccessProtection );


typedef NTSTATUS (__stdcall *ZwGetContextThread)(IN HANDLE ThreadHandle, OUT PCONTEXT pContext);




typedef NTSTATUS (__stdcall *ZwCreateThread)(
			   OUT PHANDLE             ThreadHandle,
			   IN ACCESS_MASK          DesiredAccess,
			   IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
			   IN HANDLE               ProcessHandle,
			   OUT PCLIENT_ID          ClientId,
			   IN PCONTEXT             ThreadContext,
			   IN PINITIAL_TEB         InitialTeb,
			   IN BOOLEAN              CreateSuspended );


typedef NTSTATUS (__stdcall *ZwResumeThread)(IN HANDLE ThreadHandle, OUT PULONG SuspendCount OPTIONAL );


ZwResumeThread pfnZwResumeThread;
ZwCreateThread pfnZwCreateThread;
ZwGetContextThread pfnZwGetContextThread;
ZwProtectVirtualMemory pfnZwProtectVirtualMemory; 
ZwWriteVirtualMemory pfnZwWriteVirtualMemory;
ZwAllocateVirtualMemory pfnZwAllocateVirtualMemory;


FARPROC WINAPI MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    PIMAGE_DOS_HEADER pDOSHeader;
    PIMAGE_NT_HEADERS pNTHeader;
    PIMAGE_EXPORT_DIRECTORY pExportDir;
    LPCSTR *pFunctionName;
    LPDWORD pFunction;
    LPWORD pIndex;
	
    if (NULL == hModule && NULL == lpProcName)
        return NULL;
	
    pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
    if (IsBadReadPtr(pDOSHeader, sizeof(IMAGE_DOS_HEADER)))
        return NULL;
		/*
		if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL; //������Ч�� DOS �ļ�ͷ��־
	*/
	
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDOSHeader + (DWORD)pDOSHeader->e_lfanew);
	/*
    if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	return NULL; //������Ч�� PE �ļ�ͷ��־*/
	
    
    pExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pDOSHeader + \
		(DWORD)pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    if ((DWORD)pExportDir == (DWORD)pDOSHeader)
        return NULL; //û�������
	
    pFunctionName = (LPCSTR *)((DWORD)pExportDir->AddressOfNames + (DWORD)pDOSHeader);
    pFunction = (LPDWORD)((DWORD)pExportDir->AddressOfFunctions + (DWORD)pDOSHeader);
    pIndex = (LPWORD)((DWORD)pExportDir->AddressOfNameOrdinals + (DWORD)pDOSHeader);
    while ((LPCSTR)((DWORD)*pFunctionName + (DWORD)pDOSHeader)!= NULL)
    {
        if (strcmp((LPCSTR)((DWORD)*pFunctionName + (DWORD)pDOSHeader), lpProcName) == 0)
        {
            return (FARPROC)(pFunction[*pIndex] + (DWORD)pDOSHeader);
        }
        pFunctionName++;
        pIndex++;
    }
	
    return NULL;
}

HANDLE WINAPI OsCreateRemoteThread2(
									HANDLE hProcess,
									LPSECURITY_ATTRIBUTES lpThreadAttributes,
									SIZE_T dwStackSize,
									LPTHREAD_START_ROUTINE lpStartAddress,
									LPVOID lpParameter,
									DWORD dwCreationFlags,
									LPDWORD lpThreadId)
{

	HMODULE hModule = GetModuleHandle("ntdll.dll");

	pfnZwAllocateVirtualMemory = (ZwAllocateVirtualMemory)MyGetProcAddress( hModule, "ZwAllocateVirtualMemory");
	pfnZwWriteVirtualMemory = (ZwWriteVirtualMemory)MyGetProcAddress( hModule, "ZwWriteVirtualMemory");
	pfnZwProtectVirtualMemory = (ZwProtectVirtualMemory)MyGetProcAddress( hModule, "ZwProtectVirtualMemory");
	pfnZwGetContextThread = (ZwGetContextThread)MyGetProcAddress( hModule, "ZwGetContextThread");
	pfnZwCreateThread = (ZwCreateThread)MyGetProcAddress( hModule, "ZwCreateThread");
	pfnZwResumeThread = (ZwResumeThread)MyGetProcAddress( hModule, "ZwResumeThread");

	DWORD dwRtlExitUserThreadAddr = (DWORD)MyGetProcAddress(hModule, "RtlExitUserThread");


	//by 80695073(QQ) 
	//email kiss2008ufo@yahoo.com.cn
	CONTEXT    context = {CONTEXT_FULL}; 
	CLIENT_ID  cid; 
	DWORD      ret; 
	HANDLE    hThread = NULL;
	DWORD    StackReserve;
	DWORD    StackCommit = 0x1000;
	ULONG_PTR  Stack = 0;
	INITIAL_TEB InitialTeb;
	ULONG    x; 
	const CHAR myBaseThreadInitThunk[] = 
	{
			//   00830000    8BFF			 mov     edi, edi
			'\x8B','\xFF',
			//   00830002    55              push    ebp
			'\x55',
			//   00830003    8BEC            mov     ebp, esp
			'\x8B','\xEC',
			//   00830005    51              push    ecx   //ntdll.RtlExitUserThread
			'\x51',
			//   00830006    53              push    ebx   //����
			'\x53',
			//   00830007    FFD0            call    eax   //������ַ
			'\xFF','\xD0',
			//   00830009    59              pop     ecx   //�ָ�����������ַ
			'\x59',
			//   0083000A    50              push    eax   //���ղŵĽ��ѹջ
			'\x50',
			//   0083000B    FFD1            call    ecx   //����RtlExitUserThread ����
			'\xFF','\xD1',
			//  0083000D    90              nop
			'\x90'
	};
	PVOID  pBaseThreadThunk = NULL; //�����ͷ�
	
	//0�������OS�ļ��غ���
	StackReserve = 0x1000;
	ret = pfnZwAllocateVirtualMemory(hProcess, 
		/*&stack.ExpandableStackBottom*/(PVOID*)&pBaseThreadThunk, 
		0, 
		&StackReserve,
		MEM_COMMIT, 
		PAGE_EXECUTE_READWRITE); 
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwAllocateVirtualMemory0 !\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
	ret = pfnZwWriteVirtualMemory(hProcess,
		pBaseThreadThunk,
		(LPVOID)myBaseThreadInitThunk,
		sizeof(myBaseThreadInitThunk),&x);
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwAllocateVirtualMemory0 !\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
	cid.UniqueProcess = hProcess;
	
	//1��׼����ջ
	StackReserve = 0x10000;
	ret = pfnZwAllocateVirtualMemory(hProcess, 
		/*&stack.ExpandableStackBottom*/(PVOID*)&Stack, 
		0, 
		&StackReserve,
		MEM_RESERVE, 
		PAGE_READWRITE); 
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwAllocateVirtualMemory1!\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
//	printf("OK OsCreateRemoteThread2:ZwAllocateVirtualMemory 0x%08x\n",Stack);
	
	InitialTeb.StackAllocationBase = (PVOID)Stack;
    InitialTeb.StackBase = (PVOID)(Stack + StackReserve);
	
	/* Update the Stack Position */
    Stack += StackReserve - StackCommit;
	
	Stack -= 0x1000;
	StackCommit += 0x1000;
	
    /* Allocate memory for the stack */
    ret = pfnZwAllocateVirtualMemory(hProcess,
		(PVOID*)&Stack,
		0,
		&StackCommit,
		MEM_COMMIT,
        PAGE_READWRITE);
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwAllocateVirtualMemory2!\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
//	printf("OK OsCreateRemoteThread2:ZwAllocateVirtualMemory 2 0x%08x\n",Stack);
	InitialTeb.StackLimit = (PVOID)Stack;
	
	
	StackReserve = 0x1000; 
	ret = pfnZwProtectVirtualMemory(hProcess, (PVOID*)&Stack, &StackReserve, PAGE_READWRITE | PAGE_GUARD, &x); 
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwProtectVirtualMemory!\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
	/* Update the Stack Limit keeping in mind the Guard Page */
    InitialTeb.StackLimit = (PVOID)((ULONG_PTR)InitialTeb.StackLimit - 0x1000);
	//2��׼��CONTEXT
	//  CONTEXT context = {CONTEXT_FULL}; 
	ret = pfnZwGetContextThread(GetCurrentThread(),&context); 
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwGetContextThread!\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
	context.Esp = (ULONG)InitialTeb.StackBase; 
	context.Eip = (ULONG)pBaseThreadThunk; //������д��Ҫ���صĵ�ַ��������Ҫ�Լ��ս��Լ�
	context.Ebx = (ULONG)lpParameter;
	//other init
	//must
	context.Eax = (ULONG)lpStartAddress;
//	context.Ecx = 0x778B0859;/*win7*///0x77AEEC01;/*vista*/ //ntdll.RtlExitUserThread
	context.Ecx = dwRtlExitUserThreadAddr;
	context.Edx = 0x00000000; //nouse
	
	
	ret = pfnZwCreateThread(&hThread, THREAD_ALL_ACCESS, 0, hProcess, &cid, &context, &InitialTeb, TRUE); 
	if (ret >= 0x80000000)
	{
		//ʧ��
//		printf("Error IN OsCreateRemoteThread2 ZwCreateThread!\n");
		goto OsCreateRemoteThread2Ret;
		//end
	}
	if(lpThreadId)
	{
		*lpThreadId = (DWORD)cid.UniqueThread;
	}
	if (!(dwCreationFlags & CREATE_SUSPENDED))
    {
        pfnZwResumeThread(hThread, NULL);
    }
OsCreateRemoteThread2Ret:
	return hThread;
}