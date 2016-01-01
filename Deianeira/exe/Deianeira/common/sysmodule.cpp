#include "sysmodule.h"

//���ϵͳģ���б� 
PSYSTEM_MODULE_LIST GetSystemModuleList() 
{ 
	//����ָ�붨�� 
	PNtQuerySystemInformation NtQuerySystemInformation; 
	HMODULE hModule;  
	ULONG ulSize; 
	NTSTATUS status; 
	PSYSTEM_MODULE_LIST pSystemModuleInfo; 

	//���ģ���� 
	if(!(hModule=LoadLibrary(L"ntdll.dll"))) 
	{ 
		return NULL; 
	} 

	//��ú�����ַ 
	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation"); 
	//������ַ��ȡʧ�� 
	if(!NtQuerySystemInformation) 
	{ 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//��һ������ʱ��ѯ�����ֽ��� 
	NtQuerySystemInformation(SystemModuleInformation,NULL,0, &ulSize); 
	//ͨ�����ص��ֽ����������ڴ� 
	pSystemModuleInfo = (PSYSTEM_MODULE_LIST)malloc(ulSize); 
	//��������ڴ�ʧ��
	if(!pSystemModuleInfo) 
	{ 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//�ٴλ�� 
	status = NtQuerySystemInformation(SystemModuleInformation,pSystemModuleInfo,ulSize,&ulSize); 
	//��ȡʧ�� 
	if(!NT_SUCCESS(status)) 
	{ 
		//�ͷ�������ڴ�
		free(pSystemModuleInfo); 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//�ͷ��ڴ� 
	FreeLibrary(hModule); 
	//����ɹ�
	return pSystemModuleInfo; 
} 