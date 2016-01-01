#include "common.h"
#include "sysmodule.h"


typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG_PTR RefCnt:3;
		ULONG_PTR Value;
	};
} EX_FAST_REF, *PEX_FAST_REF;


typedef struct _EX_CALLBACK_ROUTINE_BLOCK
{
	EX_RUNDOWN_REF RundownProtect;
	PEX_CALLBACK_FUNCTION Function;
	PVOID Context;
} EX_CALLBACK_ROUTINE_BLOCK, *PEX_CALLBACK_ROUTINE_BLOCK;


typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

NTSYSAPI     
NTSTATUS    
NTAPI
ZwOpenDirectoryObject( 
					  __out PHANDLE DirectoryHandle, 
					  __in ACCESS_MASK DesiredAccess,
					  __in POBJECT_ATTRIBUTES ObjectAttributes 
					  );

NTSYSAPI     
NTSTATUS    
NTAPI 
ZwQueryDirectoryObject(
					   IN HANDLE DirectoryHandle,
					   OUT PVOID Buffer,
					   IN ULONG BufferLength,
					   IN BOOLEAN ReturnSingleEntry,
					   IN BOOLEAN RestartScan,
					   IN OUT PULONG Context,
					   OUT PULONG ReturnLength OPTIONAL
					   );


//��⵽��ϵͳ�ص������е���Ϣ
typedef struct _KernelSystemCallbackInfo{
	//�ص�����
	ULONG ulType;
	//����·��
	WCHAR PathName[256];
	//
	ULONG ulEntryAddress;
	//�ص�������ַ
	ULONG ulCallBackAddress;
	//������Ϣ
	ULONG ulAttatch;

	//ָ����һ��
    struct KERNEL_SYSTEM_CALLBACK_INFO* next;
}KERNEL_SYSTEM_CALLBACK_INFO,*PKERNEL_SYSTEM_CALLBACK_INFO;

//��⵽�Ĺ�����������Ϣ
typedef struct _KernelFilterDriverInfo{
	ULONG ulObjType;
	WCHAR FileName[64];
	WCHAR FilePath[256];
	ULONG ulAttachDevice;
	//����
	WCHAR HostFileName[64];
	//ָ����һ��
	struct KERNEL_FILTERDRIVER_INFO* next;
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

//ȫ��
PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo,pKernelSystemCallbackInfoHead;

//ȫ��
PKERNEL_FILTERDRIVER_INFO pKernelFilterDriverInfo,pKernelFilterDriverInfoHead;


//���ϵͳ�Ļص�����
NTSTATUS kernel_systemCallbackEnum();

//��ô�С
ULONG kernel_systemCallbackGetCount();

//�������ݵ�ring3
NTSTATUS kernel_systemCallbackGetInfo(PKERNEL_SYSTEM_CALLBACK_INFO pBuffer);

//ж��
NTSTATUS kernel_systemCallbackKill(ULONG ulEntryAddress,ULONG ulFuncAddr);

//ö�ٹ�������
NTSTATUS kernel_filterDriverEnum();

//��ô�С
ULONG kernel_filterDriverGetCount();

//�������ݵ�ring3
NTSTATUS kernel_filterDriverGetInfo(PKERNEL_FILTERDRIVER_INFO pBuffer);

//ժ����������
NTSTATUS kernel_filterDriverKill(PDEVICE_OBJECT pAttachObj);