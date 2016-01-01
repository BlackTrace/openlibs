#include "sysmodule.h"


PVOID IopOpenRegistryKey;
PVOID IopLoadUnloadDriver;

//ö������ģ��
NTSTATUS sysmodule_enumDrivers(PDRIVER_OBJECT pDriverObj)
{
	NTSTATUS status;
	PLIST_ENTRY pListHead = NULL;
	PLDR_DATA_TABLE_ENTRY pLdr = NULL;
	PSYSTEM_MODULE_INFO pSystemModuleInfo_tmp = NULL;

	__try
	{
		//pSystemModuleInfoHead��¼ͷ��
		pSystemModuleInfoHead = pSystemModuleInfo = (PSYSTEM_MODULE_INFO)ExAllocatePool(NonPagedPool,sizeof(SYSTEM_MODULE_INFO));

		//
		pSystemModuleInfo->next = NULL;


		//��Ϊ����ͷ��
		pListHead = ((PLIST_ENTRY)pDriverObj->DriverSection)->Flink;

		//ѭ����������
		do 
		{
			pLdr = CONTAINING_RECORD(pListHead,
				LDR_DATA_TABLE_ENTRY,
				InLoadOrderLinks);

			if (pLdr->EntryPoint && pLdr->FullDllName.Buffer) 
			{
				pSystemModuleInfo_tmp = (PSYSTEM_MODULE_INFO)ExAllocatePool(NonPagedPool,sizeof(SYSTEM_MODULE_INFO));

				RtlZeroMemory(pSystemModuleInfo_tmp,sizeof(SYSTEM_MODULE_INFO));

				wcscpy(pSystemModuleInfo_tmp->BaseName,pLdr->BaseDllName.Buffer);
                wcscpy(pSystemModuleInfo_tmp->FullName,pLdr->FullDllName.Buffer);

				//��ַ
				pSystemModuleInfo_tmp->ulBaseAddress = pLdr->DllBase;
				//��С
				pSystemModuleInfo_tmp->ulSize = pLdr->SizeOfImage;
				//
				pSystemModuleInfo_tmp->next = NULL;


				pSystemModuleInfo->next = pSystemModuleInfo_tmp;

				pSystemModuleInfo = pSystemModuleInfo_tmp;
			}

			//ָ������
			pListHead = pListHead->Flink;
		} while (pListHead != ((PLIST_ENTRY)pDriverObj->DriverSection)->Flink);

		//���°�ָ���Ƶ�ͷ��
		pSystemModuleInfo = pSystemModuleInfoHead->next;

		//���سɹ�
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_enumDrivers EXCEPTION_EXECUTE_HANDLER error!");

		//����ʧ��
		return STATUS_UNSUCCESSFUL;
	}
}

//�ͷ��ڴ�
VOID sysmodule_freeDriverModuleInfo()
{
	PSYSTEM_MODULE_INFO p = NULL;

	__try
	{
		while(pSystemModuleInfo != NULL)
		{
			p = pSystemModuleInfo->next;
			ExFreePool(pSystemModuleInfo);
			pSystemModuleInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_freeDriverModuleInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��ô�С
ULONG sysmodule_getDriverModuleCount()
{
	__try
	{
		PSYSTEM_MODULE_INFO p=NULL;
		ULONG ulCount=0;

		//��ô�С
		for (p=pSystemModuleInfo;p;p=p->next)
		{
			ulCount++;
		}

		DbgPrint("sysmodule_getDriverModuleCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}	
}

//�������ݵ�ring3
NTSTATUS sysmodule_getDriverModuleInfo(PSYSTEM_MODULE_INFO pBuffer)
{
	PSYSTEM_MODULE_INFO p=NULL;
    ULONG ulIndex = 0;

	__try
	{
		//����
		for (p=pSystemModuleInfo;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(SYSTEM_MODULE_INFO));
			ulIndex++;
		}

		//�ͷ��ڴ�
		sysmodule_freeDriverModuleInfo();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getDriverModuleInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//�õ�IopLoadUnloadDriver�ĵ�ַ
ULONG sysmodule_getIopLoadUnloadDriverAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtLoadDriver,ulIopLoadUnloadDriver;

	__try
	{
		//���NtLoadDriver��ַ
		ulNtLoadDriver = GetSSDTFuncAddress(ZwLoadDriver);

		//���IopLoadUnloadDriver��ַ
		for (cPtr = (PUCHAR)ulNtLoadDriver; cPtr < (PUCHAR)ulNtLoadDriver + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==7)
				{
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulIopLoadUnloadDriver = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("sysmodule_getIopLoadUnloadDriverAddress address :%X, ulIopLoadUnloadDriver :%X",cPtr,ulIopLoadUnloadDriver);
					break;
				}

				//
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		return ulIopLoadUnloadDriver;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getIopLoadUnloadDriverAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//�õ�IopOpenRegistryKey�ĵ�ַ
ULONG sysmodule_getIopOpenRegistryKeyAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulIopLoadUnloadDriver,ulIopOpenRegistryKey;

	__try
	{
		ulIopLoadUnloadDriver = sysmodule_getIopLoadUnloadDriverAddress();

		if (ulIopLoadUnloadDriver ==0 || ulIopLoadUnloadDriver<=0x80000000)
		{
			DbgPrint("sysmodule_getIopOpenRegistryKeyAddress ulIopLoadUnloadDriver error!");
			return NULL;
		}

		//���IopLoadDriver��ַ
		for (cPtr = (PUCHAR)ulIopLoadUnloadDriver; cPtr < (PUCHAR)ulIopLoadUnloadDriver + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==1)
				{
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulIopOpenRegistryKey = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("sysmodule_getIopOpenRegistryKeyAddress ulIopOpenRegistryKey :%X",ulIopOpenRegistryKey);
					break;		
				}
				
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//���ص�ַ
		return ulIopOpenRegistryKey;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getIopOpenRegistryKeyAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//
NTSTATUS NewIopOpenRegistryKey(
							   OUT PHANDLE Handle,
							   IN HANDLE BaseHandle OPTIONAL,
							   IN PUNICODE_STRING KeyName,
							   IN ACCESS_MASK DesiredAccess,
							   IN BOOLEAN Create
							   )
{
	return STATUS_UNSUCCESSFUL;
}

//������InlineHook,��ֹ��������
BOOLEAN sysmodule_inlineHook_disableLoadDriver()
{
	__try
	{
		//���ԭ�е�ַ
		ULONG ulIopOpenRegistryKey,ulIopLoadUnloadDriver;

		ulIopLoadUnloadDriver = sysmodule_getIopLoadUnloadDriverAddress();

		if (ulIopLoadUnloadDriver==0 && ulIopLoadUnloadDriver<=0x80000000)
		{
			DbgPrint("sysmodule_inlineHook_disableLoadDriver ulIopLoadUnloadDriver error!");
			return FALSE;
		}

		IopLoadUnloadDriver = (PVOID)ulIopLoadUnloadDriver;

		ulIopOpenRegistryKey = sysmodule_getIopOpenRegistryKeyAddress();

		if (ulIopOpenRegistryKey==0 && ulIopOpenRegistryKey<=0x80000000)
		{
			DbgPrint("sysmodule_inlineHook_disableLoadDriver ulIopOpenRegistryKey error!");
			return FALSE;
		}

		IopOpenRegistryKey = (PVOID)ulIopOpenRegistryKey;

		common_callAddrHook(IopLoadUnloadDriver,IopOpenRegistryKey,NewIopOpenRegistryKey,PAGE_SIZE);

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_inlineHook_disableLoadDriver EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//ֹͣ��ֹ����
VOID sysmodule_inlineHook_disableLoadDriver_stop()
{
	__try
	{
		common_callAddrHook(IopLoadUnloadDriver,NewIopOpenRegistryKey,IopOpenRegistryKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_inlineHook_disableLoadDriver_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}