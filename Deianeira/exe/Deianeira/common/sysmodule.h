//sysmodule.h
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
	_SystemModuleInfo *next;
}SYSTEM_MODULE_INFO, *PSYSTEM_MODULE_INFO; 

//���ϵͳģ���б� 
PSYSTEM_MODULE_LIST GetSystemModuleList();

#endif //_SYSMODULE_