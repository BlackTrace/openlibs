
#include "common.h"


//���Shadow SSDT��ַ
PULONG getAddressOfShadowTable()
{
	PULONG p;

	//����XP��2003��WIN7
	//nt!KeAddSystemServiceTable+0x1a:
	//83de0022 8d8840dbdb83    lea     ecx,nt!KeServiceDescriptorTableShadow (83dbdb40)[eax]
	//83de0028 833900          cmp     dword ptr [ecx],0
	//83de002b 7546            jne     nt!KeAddSystemServiceTable+0x6b (83de0073)

	//8d88�����ֽڣ�����+2
	p = (PULONG)((ULONG)KeAddSystemServiceTable+0x1a+2);

	return (PULONG)(*p);
}

//��õ�������δ�ĵ��������ĵ�ַ
PVOID GetExportedFuncAddress(LPTSTR lpFuncName)
{
   PVOID pAddress;
   UNICODE_STRING ustrFuncName;

   __try
   {
	   RtlInitUnicodeString(&ustrFuncName, lpFuncName);

	   pAddress = MmGetSystemRoutineAddress(&ustrFuncName);

	   return pAddress;
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
	   return NULL;
   }
}


//��ý��̾�������
NTSTATUS GetProcessImageNameByProcessHandle(HANDLE ProcessHandle,LPTSTR lpFilePath){
	NTSTATUS status;
	DWORD dwRet;
	PVOID pBuffer;
    PEPROCESS pEpr;
	HANDLE hProcess;
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK ioStatus;
    PFILE_OBJECT pFileObject;
    UNICODE_STRING ustrDosName;
    UNICODE_STRING ustrPathName;

	__try
	{
		status = PsLookupProcessByProcessId(ProcessHandle,&pEpr); 

		if(!NT_SUCCESS(status)) 
		{ 
			DbgPrint("GetProcessFileNameByProcessHandle PsLookupProcessByProcessId error!");
			return STATUS_UNSUCCESSFUL; 
		} 

		status = ObOpenObjectByPointer( 
			pEpr,
			0,
			NULL,
			PROCESS_ALL_ACCESS, 
			NULL, 
			KernelMode,//PreviousMode, 
			&hProcess 
			);
	
		if(!NT_SUCCESS(status)) 
		{ 
			DbgPrint("GetProcessFileNameByProcessHandle ObOpenObjectByPointer error!");
			return STATUS_UNSUCCESSFUL; 
		} 

		status = ZwQueryInformationProcess(hProcess, 
			ProcessImageFileName,
			NULL,
			0,
			&dwRet);

		if (status != STATUS_INFO_LENGTH_MISMATCH) {
			DbgPrint("GetProcessFileNameByProcessHandle ZwQueryInformationProcess error!");
			return STATUS_UNSUCCESSFUL;
		}

		pBuffer = ExAllocatePool(NonPagedPool,dwRet);

		if (pBuffer == NULL) {
			DbgPrint("GetProcessFileNameByProcessHandle ExAllocatePool error!");
			return STATUS_UNSUCCESSFUL ;        
		}

		status = ZwQueryInformationProcess(hProcess, 
			ProcessImageFileName,
			pBuffer,
			dwRet,
			&dwRet);

		if (!NT_SUCCESS(status)) {
			DbgPrint("GetProcessFileNameByProcessHandle ZwQueryInformationProcess2 error!");
			return STATUS_UNSUCCESSFUL;
		}

		//��ʼ��
		InitializeObjectAttributes(&ObjectAttributes,(PUNICODE_STRING)pBuffer,OBJ_CASE_INSENSITIVE,NULL,NULL);

        //��Ŀ���ļ�
		status = ZwOpenFile(&hFile,GENERIC_READ,&ObjectAttributes,&ioStatus,FILE_SHARE_READ,NULL);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("GetProcessFileNameByProcessHandle ZwOpenFile error!");
			return STATUS_UNSUCCESSFUL;
		}

		status = ObReferenceObjectByHandle(hFile,
			NULL,
			*IoFileObjectType,
			KernelMode,
			&pFileObject,
			NULL);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("GetProcessFileNameByProcessHandle ObReferenceObjectByHandle error!");
			return STATUS_UNSUCCESSFUL;
		}

		ustrPathName.Buffer = ExAllocatePool(PagedPool,256*sizeof(WCHAR));
		ustrPathName.MaximumLength = 256*sizeof(WCHAR); 

		status = IoVolumeDeviceToDosName(pFileObject->DeviceObject,&ustrDosName);

		if (!NT_SUCCESS(status))
		{
			ObDereferenceObject(pFileObject);
			return STATUS_UNSUCCESSFUL;
		}

		RtlCopyUnicodeString(&ustrPathName,&ustrDosName); 
		RtlAppendUnicodeStringToString(&ustrPathName,&pFileObject-> FileName); 

		ObDereferenceObject(pFileObject);

		wcscpy(lpFilePath,ustrPathName.Buffer);

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("GetProcessFileNameByProcessHandle EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//ͨ�����̾������ý���ID
ULONG GetProcessIDByProcessHandle(HANDLE ProcessHandle)
{
	PVOID pAddress;
	ULONG ulRet;
    NTSTATUS status;

	__try
	{
		PPROCESS_BASIC_INFORMATION pProcessBasicInformation = (PPROCESS_BASIC_INFORMATION)ExAllocatePool(PagedPool,sizeof(PROCESS_BASIC_INFORMATION));

		if (pProcessBasicInformation == NULL)
		{
			DbgPrint("pProcessBasicInformation����ʧ�ܣ�");
			//error
			return -1;
		}

		status = ZwQueryInformationProcess(ProcessHandle,ProcessBasicInformation,pProcessBasicInformation,sizeof(PROCESS_BASIC_INFORMATION),&ulRet);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("ZwQueryInformationProcessִ��ʧ�ܣ�");
			//error
			ExFreePool(pProcessBasicInformation);
			return -1;
		}

		//��ʾ���ص�ֵ
		DbgPrint("pProcessBasicInformation->UniqueProcessId��%d",pProcessBasicInformation->UniqueProcessId);

		ExFreePool(pProcessBasicInformation);

		return (ULONG)pProcessBasicInformation->UniqueProcessId;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}
}

//��ø�����ID
LONG GetParentProcessIDByProcessHandle(HANDLE ProcessHandle)
{
	PVOID pAddress;
	ULONG ulRet;
	NTSTATUS status;

	__try
	{
		PPROCESS_BASIC_INFORMATION pProcessBasicInformation = (PPROCESS_BASIC_INFORMATION)ExAllocatePool(PagedPool,sizeof(PROCESS_BASIC_INFORMATION));

		if (pProcessBasicInformation == NULL)
		{
			DbgPrint("pProcessBasicInformation����ʧ�ܣ�");
			//error
			return -1;
		}

		status = ZwQueryInformationProcess(ProcessHandle,ProcessBasicInformation,pProcessBasicInformation,sizeof(PROCESS_BASIC_INFORMATION),&ulRet);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("ZwQueryInformationProcessִ��ʧ�ܣ�");
			//error
			ExFreePool(pProcessBasicInformation);
			return -1;
		}

		//��ʾ���ص�ֵ
		DbgPrint("pProcessBasicInformation->InheritedFromUniqueProcessId��%d",pProcessBasicInformation->InheritedFromUniqueProcessId);

		ExFreePool(pProcessBasicInformation);

		return (ULONG)pProcessBasicInformation->InheritedFromUniqueProcessId;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}
}

//��StartAddr��ַ  ��ʼ��OldAddr  �滻ΪNewAddr��ַ   ������ SIZE 
BOOL common_callAddrHook(PVOID StartAddr,PVOID OldAddr,PVOID NewAddr,ULONG Size)
{
	PUCHAR cPtr, pOpcode;
	ULONG ulLength,ulOffset;

	__try
	{
		for (cPtr=StartAddr;(ULONG)cPtr<(ULONG)StartAddr + Size;cPtr += ulLength)
		{
			//���ָ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//ʧ��
			if (!ulLength) 
			{
				break;
			}

			//��תָ��
			if (ulLength ==5 && *cPtr==0xE8)// ��ǰ����5 �ҵ�һ�ֽ�ΪE8 
			{
				//�жϵ�ǰ�Ƿ�ΪOldAddr��CALL��Ե�ַ 
				if ((ULONG)OldAddr-(ULONG)cPtr - 5 == *(PULONG)(cPtr+1)) 
				{
					//��ΪCALL�õ������ƫ��  �������ǻ���Ҫ���м������ƫ��
					ulOffset=(ULONG)NewAddr - (ULONG)cPtr - 5;

					//ȡ���ڴ汣��
					UnProtected();

					//�滻��ת��ַ
					*(PULONG)(cPtr + 1)=ulOffset; 

					//�����ڴ汣��
					Protected();

					return TRUE;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_callAddrHook EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//ȥ���ڴ汣��
VOID UnProtected()
{
	_asm{
		push eax
			mov eax,cr0
			and eax,not 10000h
			mov cr0,eax
			pop eax
	}
};

//�ָ��ڴ汣��
VOID Protected()
{
	_asm{  
		push eax
			mov eax,cr0
			or eax,10000h
			mov cr0,eax
			pop eax
	}
};

//�����������������������
VOID common_getHandleTypeName(ULONG ulObjectTypeIndex,LPTSTR lpstr){
	switch(ulObjectTypeIndex)
	{
	case OB_TYPE_UNKNOWN:
		wcscpy(lpstr,L"Unknown");
		break;
	case OB_TYPE_TYPE:
		wcscpy(lpstr,L"Type");
		break;
	case OB_TYPE_DIRECTORY:
		wcscpy(lpstr,L"Directory");
		break;
	case OB_TYPE_SYMBOLIC_LINK:
		wcscpy(lpstr,L"SymbolicLink");
		break;
	case OB_TYPE_TOKEN:
		wcscpy(lpstr,L"Token");
		break;
	case OB_TYPE_PROCESS:
		wcscpy(lpstr,L"Process");
		break;
	case OB_TYPE_THREAD:
		wcscpy(lpstr,L"Thread");
		break;
	case OB_TYPE_JOB:
		wcscpy(lpstr,L"Job");
		break;
	case OB_TYPE_DEBUG_OBJECT:
		wcscpy(lpstr,L"Debug");
		break;
	case OB_TYPE_EVENT:
		wcscpy(lpstr,L"Event");
		break;
	case OB_TYPE_EVENT_PAIR:
		wcscpy(lpstr,L"EventPair");
		break;
	case OB_TYPE_MUTANT:
		wcscpy(lpstr,L"Mutant");
		break;
	case OB_TYPE_CALLBACK:
		wcscpy(lpstr,L"Callback");
		break;
	case OB_TYPE_SEMAPHORE:
		wcscpy(lpstr,L"Semaphore");
		break;
	case OB_TYPE_TIMER:
		wcscpy(lpstr,L"Timer");
		break;
	case OB_TYPE_PROFILE:
		wcscpy(lpstr,L"Profile");
		break;
	case OB_TYPE_KEYED_EVENT:
		wcscpy(lpstr,L"KeyedEvent");
		break;
	case OB_TYPE_WINDOWS_STATION:
		wcscpy(lpstr,L"WindowsStation");
		break;
	case OB_TYPE_DESKTOP:
		wcscpy(lpstr,L"Desktop");
		break;
	case OB_TYPE_SECTION:
		wcscpy(lpstr,L"Section");
		break;
	case OB_TYPE_KEY:
		wcscpy(lpstr,L"Key");
		break;
	case OB_TYPE_PORT:
		wcscpy(lpstr,L"Port");
		break;
	case OB_TYPE_WAITABLE_PORT:
		wcscpy(lpstr,L"WaitablePort");
		break;
	case OB_TYPE_ADAPTER:
		wcscpy(lpstr,L"Adapter");
		break;
	case OB_TYPE_CONTROLLER:
		wcscpy(lpstr,L"Controller");
		break;
	case OB_TYPE_DEVICE:
		wcscpy(lpstr,L"Device");
		break;
	case OB_TYPE_DRIVER:
		wcscpy(lpstr,L"Driver");
		break;
	case OB_TYPE_IOCOMPLETION:
		wcscpy(lpstr,L"IoCompletion");
		break;
	case OB_TYPE_FILE:
		wcscpy(lpstr,L"File");
		break;
	case OB_TYPE_WMIGUID:
		wcscpy(lpstr,L"WmiGuid");
		break;
	}
}
