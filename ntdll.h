//ntdll.h
/*
	1.0		Build
  */

#ifndef _NTDLL_H
#define _NTDLL_H

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  // _MSC_VER 

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef ANYSIZE_ARRAY
#define ANYSIZE_ARRAY 1       
#endif

#if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif

#if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#define DECLSPEC_EXPORT __declspec(dllexport)
#else
#define DECLSPEC_EXPORT
#endif

/*
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define NTAPI
#endif
*/
	
#if !defined(_NTSYSTEM_)
#define NTSYSAPI DECLSPEC_IMPORT
#else
#define NTSYSAPI DECLSPEC_EXPORT
#endif

#ifndef _CRTIMP
#define _CRTIMP NTSYSAPI 
#endif 

#ifndef NTSTATUS
typedef long NTSTATUS;
#endif

typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation, // 0 Y N
		SystemProcessorInformation, // 1 Y N
		SystemPerformanceInformation, // 2 Y N
		SystemTimeOfDayInformation, // 3 Y N
		SystemNotImplemented1, // 4 Y N
		SystemProcessesAndThreadsInformation, // 5 Y N
		SystemCallCounts, // 6 Y N
		SystemConfigurationInformation, // 7 Y N
		SystemProcessorTimes, // 8 Y N
		SystemGlobalFlag, // 9 Y Y
		SystemNotImplemented2, // 10 Y N
		SystemModuleInformation, // 11 Y
		SystemLockInformation, // 12 Y N
		SystemNotImplemented3, // 13 Y N
		SystemNotImplemented4, // 14 Y N
		SystemNotImplemented5, // 15 Y N
		SystemHandleInformation, // 16 Y N
		SystemObjectInformation, // 17 Y N
		SystemPagefileInformation, // 18 Y N
		SystemInstructionEmulationCounts, // 19 Y N
		SystemInvalidInfoClass1, // 20
		SystemCacheInformation, // 21 Y Y
		SystemPoolTagInformation, // 22 Y N
		SystemProcessorStatistics, // 23 Y N
		SystemDpcInformation, // 24 Y Y
		SystemNotImplemented6, // 25 Y N
		SystemLoadImage, // 26 N Y
		SystemUnloadImage, // 27 N Y
		SystemTimeAdjustment, // 28 Y Y
		SystemNotImplemented7, // 29 Y N
		SystemNotImplemented8, // 30 Y N
		SystemNotImplemented9, // 31 Y N
		SystemCrashDumpInformation, // 32 Y N
		SystemExceptionInformation, // 33 Y N
		SystemCrashDumpStateInformation, // 34 Y Y/N
		SystemKernelDebuggerInformation, // 35 Y N
		SystemContextSwitchInformation, // 36 Y N
		SystemRegistryQuotaInformation, // 37 Y Y
		SystemLoadAndCallImage, // 38 N Y
		SystemPrioritySeparation, // 39 N Y
		SystemNotImplemented10, // 40 Y N
		SystemNotImplemented11, // 41 Y N
		SystemInvalidInfoClass2, // 42
		SystemInvalidInfoClass3, // 43
		SystemTimeZoneInformation, // 44 Y N
		SystemLookasideInformation, // 45 Y N
		SystemSetTimeSlipEvent, // 46 N Y
		SystemCreateSession, // 47 N Y
		SystemDeleteSession, // 48 N Y
		SystemInvalidInfoClass4, // 49
		SystemRangeStartInformation, // 50 Y N
		SystemVerifierInformation, // 51 Y Y
		SystemAddVerifier, // 52 N Y
		SystemSessionProcessesInformation // 53 Y N
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_BASIC_INFORMATION { // Information Class 0
	ULONG Unknown;
	ULONG MaximumIncrement;
	ULONG PhysicalPageSize;
	ULONG NumberOfPhysicalPages;
	ULONG LowestPhysicalPage;
	ULONG HighestPhysicalPage;
	ULONG AllocationGranularity;
	ULONG LowestUserAddress;
	ULONG HighestUserAddress;
	ULONG ActiveProcessors;
	UCHAR NumberProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION { // Information Class 2
	LARGE_INTEGER IdleTime;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
	ULONG ReadOperationCount;
	ULONG WriteOperationCount;
	ULONG OtherOperationCount;
	ULONG AvailablePages;
	ULONG TotalCommittedPages;
	ULONG TotalCommitLimit;
	ULONG PeakCommitment;
	ULONG PageFaults;
	ULONG WriteCopyFaults;
	ULONG TransitionFaults;
	ULONG Reserved1;
	ULONG DemandZeroFaults;
	ULONG PagesRead;
	ULONG PageReadIos;
	ULONG Reserved2[2];
	ULONG PagefilePagesWritten;
	ULONG PagefilePageWriteIos;
	ULONG MappedFilePagesWritten;
	ULONG MappedFilePageWriteIos;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
	ULONG PagedPoolAllocs;
	ULONG PagedPoolFrees;
	ULONG NonPagedPoolAllocs;
	ULONG NonPagedPoolFrees;
	ULONG TotalFreeSystemPtes;
	ULONG SystemCodePage;
	ULONG TotalSystemDriverPages;
	ULONG TotalSystemCodePages;
	ULONG SmallNonPagedLookasideListAllocateHits;
	ULONG SmallPagedLookasideListAllocateHits;
	ULONG Reserved3;
	ULONG MmSystemCachePage;
	ULONG PagedPoolPage;
	ULONG SystemDriverPage;
	ULONG FastReadNoWait;
	ULONG FastReadWait;
	ULONG FastReadResourceMiss;
	ULONG FastReadNotPossible;
	ULONG FastMdlReadNoWait;
	ULONG FastMdlReadWait;
	ULONG FastMdlReadResourceMiss;
	ULONG FastMdlReadNotPossible;
	ULONG MapDataNoWait;
	ULONG MapDataWait;
	ULONG MapDataNoWaitMiss;
	ULONG MapDataWaitMiss;
	ULONG PinMappedDataCount;
	ULONG PinReadNoWait;
	ULONG PinReadWait;
	ULONG PinReadNoWaitMiss;
	ULONG PinReadWaitMiss;
	ULONG CopyReadNoWait;
	ULONG CopyReadWait;
	ULONG CopyReadNoWaitMiss;
	ULONG CopyReadWaitMiss;
	ULONG MdlReadNoWait;
	ULONG MdlReadWait;
	ULONG MdlReadNoWaitMiss;
	ULONG MdlReadWaitMiss;
	ULONG ReadAheadIos;
	ULONG LazyWriteIos;
	ULONG LazyWritePages;
	ULONG DataFlushes;
	ULONG DataPages;
	ULONG ContextSwitches;
	ULONG FirstLevelTbFills;
	ULONG SecondLevelTbFills;
	ULONG SystemCalls;
} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION { // Information Class 3
	LARGE_INTEGER BootTime;
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER TimeZoneBias;
	ULONG CurrentTimeZoneId;
} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

typedef struct _SYSTEM_MODULE_INFORMATION { // Information Class 11
	ULONG	Reserved[2];
	PVOID	Base;
	ULONG	Size;
	ULONG	Flags;
	USHORT	LoadOrder;
	USHORT	Unknown;
	USHORT	LoadCount;
	USHORT	ModuleNameOffset;
	CHAR	ImagePath[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


//SYSTEM_HANDLE_INFORMATION Structure
typedef struct _SYSTEM_HANDLE_INFORMATION { // Information Class 16
	ULONG ProcessId;
	UCHAR ObjectTypeNumber;
	UCHAR Flags; // 0x01 = PROTECT_FROM_CLOSE, 0x02 = INHERIT
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

//
// ClientId
//
typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

//
// Thread priority
//
typedef LONG KPRIORITY;

//
// Wait reasons
//

typedef enum _KWAIT_REASON {
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    Spare2,
    Spare3,
    Spare4,
    Spare5,
    Spare6,
    WrKernel,
    MaximumWaitReason
    } KWAIT_REASON;

//
// Process Virtual Memory Counters
//  NtQueryInformationProcess using ProcessVmCounters
//

typedef struct _VM_COUNTERS {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} VM_COUNTERS;
typedef VM_COUNTERS *PVM_COUNTERS;

typedef enum {
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWait,
	StateTransition,
	StateUnknown
} THREAD_STATE;



//
// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
//#define UNICODE_NULL ((WCHAR)0) // winnt

//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_VALID_ATTRIBUTES    0x000003F2L

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

//
// Object Attributes structure
//

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

//++
//
// VOID
// InitializeObjectAttributes(
//     OUT POBJECT_ATTRIBUTES p,
//     IN PUNICODE_STRING n,
//     IN ULONG a,
//     IN HANDLE r,
//     IN PSECURITY_DESCRIPTOR s
//     )
//
//--

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }


typedef enum _OBJECT_INFORMATION_CLASS {	//	  Query		Set
	ObjectBasicInformation,					// 0	Y		N
	ObjectNameInformation,					// 1	Y		N
	ObjectTypeInformation,					// 2	Y		N
	ObjectAllTypesInformation,				// 3	Y		N
	ObjectHandleInformation					// 4	Y		Y
} OBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_NAME_INFORMATION {               
    UNICODE_STRING Name;                                
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION; 

typedef enum _KPROFILE_SOURCE { 
    ProfileTime, 
    ProfileAlignmentFixup, 
    ProfileTotalIssues, 
    ProfilePipelineDry, 
    ProfileLoadInstructions, 
    ProfilePipelineFrozen, 
    ProfileBranchInstructions, 
    ProfileTotalNonissues, 
    ProfileDcacheMisses, 
    ProfileIcacheMisses, 
    ProfileCacheMisses, 
    ProfileBranchMispredictions, 
    ProfileStoreInstructions, 
    ProfileFpInstructions, 
    ProfileIntegerInstructions, 
    Profile2Issue, 
    Profile3Issue, 
    Profile4Issue, 
    ProfileSpecialInstructions, 
    ProfileTotalCycles, 
    ProfileIcacheIssues, 
    ProfileDcacheAccesses, 
    ProfileMemoryBarrierCycles, 
    ProfileLoadLinkedIssues, 
    ProfileMaximum 
} KPROFILE_SOURCE, *PKPROFILE_SOURCE; 

//
// Pool Allocation routines (in pool.c)
//

typedef enum _POOL_TYPE {
    NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed,
	DontUseThisType,
	NonPagedPoolCacheAligned,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS,
	MaxPoolType
	
	// end_wdm
	,
	//
	// Note these per session types are carefully chosen so that the appropriate
	// masking still applies as well as MaxPoolType above.
	//
		
	NonPagedPoolSession = 32,
	PagedPoolSession = NonPagedPoolSession + 1,
	NonPagedPoolMustSucceedSession = PagedPoolSession + 1,
	DontUseThisTypeSession = NonPagedPoolMustSucceedSession + 1,
	NonPagedPoolCacheAlignedSession = DontUseThisTypeSession + 1,
	PagedPoolCacheAlignedSession = NonPagedPoolCacheAlignedSession + 1,
	NonPagedPoolCacheAlignedMustSSession = PagedPoolCacheAlignedSession + 1,
		
	// begin_wdm
		
} POOL_TYPE;




typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
	ULONG ContextSwitchCount;
	THREAD_STATE State;
	KWAIT_REASON WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES { // Information Class 5
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters; // Windows 2000 only
	SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

typedef struct _SYSTEM_OBJECT_TYPE_INFORMATION { // Information Class 17
	ULONG NextEntryOffset;
	ULONG ObjectCount;
	ULONG HandleCount;
	ULONG TypeNumber;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK ValidAccessMask;
	POOL_TYPE PoolType;
	UCHAR Unknown;
	UNICODE_STRING Name;
} SYSTEM_OBJECT_TYPE_INFORMATION, *PSYSTEM_OBJECT_TYPE_INFORMATION;

typedef struct _SYSTEM_OBJECT_INFORMATION {
	ULONG NextEntryOffset;
	PVOID Object;
	ULONG CreatorProcessId;
	USHORT Unknown;
	USHORT Flags;
	ULONG PointerCount;
	ULONG HandleCount;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
	ULONG ExclusiveProcessId;
	PSECURITY_DESCRIPTOR SecurityDescriptor;
	UNICODE_STRING Name;
} SYSTEM_OBJECT_INFORMATION, *PSYSTEM_OBJECT_INFORMATION;

#define STATUS_INFO_LENGTH_MISMATCH				((NTSTATUS)0xC0000004L)
#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth

//
// MessageId: STATUS_BUFFER_OVERFLOW
//
// MessageText:
//
//  {Buffer Overflow}
//  The data was too large to fit into the specified buffer.
//
#define STATUS_BUFFER_OVERFLOW           ((NTSTATUS)0x80000005L)
	
NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
IN OUT PVOID SystemInformation,
IN ULONG SystemInformationLength,
OUT PULONG ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSection(
OUT PHANDLE SectionHandle,
IN ACCESS_MASK DesiredAccess,
IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS 
NTAPI 
NtQueryObject(
IN HANDLE ObjectHandle, 
IN DWORD ObjectInformationClass, 
OUT PVOID ObjectInformation,
IN DWORD Length, 
OUT PDWORD ResultLength 
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
OUT PHANDLE FileHandle,
IN	ACCESS_MASK DesiredAccess,
IN	POBJECT_ATTRIBUTES ObjectAttributes,
OUT PIO_STATUS_BLOCK IoStatusBlock,
IN	ULONG ShareAccess,
IN	ULONG OpenOptions
);

NTSYSAPI
VOID
NTAPI
RtlInitUnicodeString(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );



typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );

//
// use the Win32 API instead
//     DeviceIoControl
//
NTSTATUS
NtDeviceIoControlFile (
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
    );

typedef enum _FILE_INFORMATION_CLASS { 
  FileDirectoryInformation                 = 1,
  FileFullDirectoryInformation,
  FileBothDirectoryInformation,
  FileBasicInformation,
  FileStandardInformation,
  FileInternalInformation,
  FileEaInformation,
  FileAccessInformation,
  FileNameInformation,
  FileRenameInformation,
  FileLinkInformation,
  FileNamesInformation,
  FileDispositionInformation,
  FilePositionInformation,
  FileFullEaInformation,
  FileModeInformation,
  FileAlignmentInformation,
  FileAllInformation,
  FileAllocationInformation,
  FileEndOfFileInformation,
  FileAlternateNameInformation,
  FileStreamInformation,
  FilePipeInformation,
  FilePipeLocalInformation,
  FilePipeRemoteInformation,
  FileMailslotQueryInformation,
  FileMailslotSetInformation,
  FileCompressionInformation,
  FileObjectIdInformation,
  FileCompletionInformation,
  FileMoveClusterInformation,
  FileQuotaInformation,
  FileReparsePointInformation,
  FileNetworkOpenInformation,
  FileAttributeTagInformation,
  FileTrackingInformation,
  FileIdBothDirectoryInformation,
  FileIdFullDirectoryInformation,
  FileValidDataLengthInformation,
  FileShortNameInformation,
  FileIoCompletionNotificationInformation,
  FileIoStatusBlockRangeInformation,
  FileIoPriorityHintInformation,
  FileSfioReserveInformation,
  FileSfioVolumeInformation,
  FileHardLinkInformation,
  FileProcessIdsUsingFileInformation,
  FileNormalizedNameInformation,
  FileNetworkPhysicalNameInformation,
  FileIdGlobalTxDirectoryInformation,
  FileIsRemoteDeviceInformation,
  FileAttributeCacheInformation,
  FileNumaNodeInformation,
  FileStandardLinkInformation,
  FileRemoteProtocolInformation,
  FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


#ifdef __cplusplus
}
#endif

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  // _MSC_VER 

#endif //_NTDLL_H
