#include "shadow.h"


//���win32k��ַ
ULONG GetWin32kBase()
{
	ULONG i;
    ULONG ulAddress;
  
	ULONG ulSize; 
	NTSTATUS status; 
	PSYSTEM_MODULE_LIST pSystem_module_list; 

	//��һ������ʱ��ѯ�����ֽ��� 
	ZwQuerySystemInformation(SystemModuleInformation,NULL,0, &ulSize); 

	//ͨ�����ص��ֽ����������ڴ� 
	pSystem_module_list = (PSYSTEM_MODULE_LIST)ExAllocatePool(PagedPool,ulSize); 
	//��������ڴ�ʧ��
	if(!pSystem_module_list) 
	{ 
		//���������Ϣ
		DbgPrint("shadow.c GetWin32kBase() pSystem_module_list �����ڴ�ʧ�ܣ�");
		return 0; 
	} 

	//�ٴλ�� 
	status = ZwQuerySystemInformation(SystemModuleInformation,pSystem_module_list,ulSize,&ulSize); 
	//��ȡʧ�� 
	if(!NT_SUCCESS(status)) 
	{ 
		//���������Ϣ
		DbgPrint("shadow.c GetWin32kBase() ZwQuerySystemInformation ʧ�ܣ�");
		//�ͷ�������ڴ�
		ExFreePool(pSystem_module_list); 
		return 0; 
	} 

	//��������
	for (i=0; i < pSystem_module_list->ulCount; i++)
	{
		//ƥ���ַ���
		if(strcmp(pSystem_module_list->smi[i].ImageName,"\\SystemRoot\\System32\\win32k.sys") == 0)
		{
			//���������Ϣ
			DbgPrint("shadow.c GetWin32kBase() pSystem_module_list->smi[i].ImageName:%s",pSystem_module_list->smi[i].ImageName);
			//��ַ
			ulAddress = (ULONG)pSystem_module_list->smi[i].Base;
			break;
		}

	}

	//���������Ϣ
	DbgPrint("shadow.c GetWin32kBase() ulAddress:%08X",ulAddress);

	//�ͷ��ڴ�
	ExFreePool(pSystem_module_list);
	//���ص�ַ
	return ulAddress;
}

//���ݷ���Ż��ԭʼ������ַ
ULONG GetOldShadowSSDTAddress(ULONG ulIndex)
{
	NTSTATUS status;
    HANDLE hFile;

	ULONG ulWin32kBase;
	ULONG ulShadowBase;
	//���ƫ����
	ULONG ulShadowRVA;

	UNICODE_STRING ustrFileName;
	OBJECT_ATTRIBUTES  objectAttributes;
	IO_STATUS_BLOCK io_status = {0};

	//��ȡ��λ��
	ULONG ulLocation;
	LARGE_INTEGER offset;

	//���صĺ�����ַ
	ULONG ulFuncAddress;

	//���Win32k��ַ
	ulWin32kBase = GetWin32kBase();

	//���������Ϣ
	DbgPrint("shadow.c GetOldSSDTAddress() ulWin32kBase:%08X",ulWin32kBase);

	//����ڴ��е�Shadow SSDT��ַ
	ulShadowBase = KeServiceDescriptorTableShadow[1].ServiceTableBase;

	//���������Ϣ
	DbgPrint("shadow.c GetOldSSDTAddress() ulShadowBase:%08X",ulShadowBase);

	//�õ�Shadow���ƫ����
	ulShadowRVA = ulShadowBase - ulWin32kBase;

	//���������Ϣ
	DbgPrint("shadow.c GetOldSSDTAddress() ulShadowRVA:%08X",ulShadowRVA);

	//��ȡ��λ��
	ulLocation = ulShadowRVA + ulIndex * 4;
	offset.QuadPart = ulLocation;

	//����ZwReadFile��ȡ�ļ�
	//��ʼ��OBJECT_ATTRIBUTES�ṹ
	RtlInitUnicodeString(&ustrFileName, L"\\SystemRoot\\system32\\win32k.sys");

	//��ʼ���ṹ��
	InitializeObjectAttributes( 
		&objectAttributes,
		&ustrFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);

	//���ļ�
	status = ZwCreateFile(
		&hFile,
		FILE_EXECUTE | SYNCHRONIZE,
		&objectAttributes,
		&io_status,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE |
		FILE_RANDOM_ACCESS |
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if(!NT_SUCCESS(status))
	{
		//���������Ϣ
		DbgPrint("shadow.c GetOldSSDTAddress() ZwCreateFile error!");
		ZwClose(hFile);
		return NULL;
	}

	//���ļ�
	status = ZwReadFile(
		hFile,
		NULL,
		NULL,
		NULL,
		NULL,
		&ulFuncAddress,
		sizeof(ULONG),
		&offset,
		NULL);

	if(!NT_SUCCESS(status))
	{
		//���������Ϣ
		DbgPrint("shadow.c GetOldSSDTAddress() ZwReadFile error!");
		ZwClose(hFile);
		return NULL;
	}

	//�ر�
	ZwClose(hFile);

	//���������Ϣ
	DbgPrint("shadow.c GetOldSSDTAddress() ulFuncAddress:%d",ulFuncAddress);

	return ulFuncAddress;
}