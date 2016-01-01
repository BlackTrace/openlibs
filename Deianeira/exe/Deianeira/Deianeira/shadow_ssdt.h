//��ֹ�ض���
#ifndef _SHADOW_SSDT_
#define _SHADOW_SSDT_

#include "..\common\common.h"
#include "..\common\sysmodule.h"


#define RVATOVA(base,offset) ((PVOID)((DWORD)(base)+(DWORD)(offset)))

typedef struct { 
	WORD    offset:12;
	WORD    type:4;
}IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;


//SDT�ṹ��
typedef struct ServiceDescriptorTable {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTable;
	unsigned int NumberOfServices;
	unsigned int *ParamTableBase;
}ServiceDescriptorTable,*PServiceDescriptorTable;


//����SSDT��Ϣ
typedef struct SSDTSaveTable		
{
	ULONG ulServiceNumber;
	ULONG ulCurrentFuncAddress;	
	ULONG ulOriginalFuncAddress;
	char ServiceFunctionName[MAX_PATH];
	char ModuleName[MAX_PATH];
} SSDTSaveTable, *PSSDTSaveTable;


//����Ϊstaticֻ�ܱ�һ��cpp����
//SSDT���ڱ��������Ϣ
extern PSSDTSaveTable pSSDTSaveTable;
//SSDT��������
extern ULONG ulSSDTTotal;
//
extern PSYSTEM_MODULE_LIST pSystem_Module_List;

//���SSDT��������
ULONG GetSSDTFuncIndex(LPSTR lpName);

//ͨ����ַ�õ�ģ����
BOOL GetModuleNameByAddr(ULONG ulAddr,PSYSTEM_MODULE_LIST pSystem_module_list,LPSTR lpBuffer);

//���ݷ���Ž�SSDT���������
VOID SSDTSTOrderByServiceNum();

//ö��SSDT
BOOL GetCurrentSSDTFunc(HANDLE hDriver);

VOID GetOldSSDTAddress();

#endif //_SHADOW_SSDT_