#ifndef _SYSMODULE_
#define _SYSMODULE_

#include "common.h"


typedef struct _SystemModuleInfo
{   
	//base name
	WCHAR BaseName[256];
	//full name
	WCHAR FullName[256];
	//��ַ
	ULONG ulBaseAddress;
	//ӳ���С
	ULONG ulSize;
	//�����б�ָ��
	struct SYSTEM_MODULE_INFO* next;
}SYSTEM_MODULE_INFO, *PSYSTEM_MODULE_INFO; 


//ȫ��
PSYSTEM_MODULE_INFO pSystemModuleInfo,pSystemModuleInfoHead;


//ö������ģ��
NTSTATUS sysmodule_enumDrivers(PDRIVER_OBJECT pDriverObj);

//�ͷ��ڴ�
VOID sysmodule_freeDriverModuleInfo();

//��ô�С
ULONG sysmodule_getDriverModuleCount();

//�������ݵ�ring3
NTSTATUS sysmodule_getDriverModuleInfo(PSYSTEM_MODULE_INFO pBuffer);

//������InlineHook,��ֹ��������
BOOLEAN sysmodule_inlineHook_disableLoadDriver();

//������InlineHook,��ֹ��������
VOID sysmodule_inlineHook_disableLoadDriver_stop();

#endif //_SYSMODULE_