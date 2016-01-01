#include "stdafx.h"
#include "shadow_ssdt.h"


//����Ϊstaticֻ�ܱ�һ��cpp����
//SSDT���ڱ��������Ϣ
PSSDTSaveTable pSSDTSaveTable;
//SSDT��������
ULONG ulSSDTTotal;
//
PSYSTEM_MODULE_LIST pSystem_Module_List;


//���SSDT��������
ULONG GetSSDTFuncIndex(LPSTR lpName)
{
 //��ú�����ַ
 FARPROC pAddress1 = GetProcAddress(GetModuleHandle(L"ntdll.dll"),lpName);

 //ת����PULONG
 PULONG pAddress2 = (PULONG)pAddress1;

 if (*(PBYTE)pAddress1 == 0xB8)
 {
	  //kd> u ntdll!NtCreateFile
      //ntdll!NtCreateFile:
      //772555c8 b842000000      mov     eax,42h
      //772555cd ba0003fe7f      mov     edx,offset SharedUserData!SystemCallStub (7ffe0300)
      //772555d2 ff12            call    dword ptr [edx]
      //772555d4 c22c00          ret     2Ch
      //772555d7 90              nop

      //(ULONG)pAddress2+1��b8���ָ��Ϊһ���ֽ�����Ҫ��1��Ȼ������ת����ַָ��
      pAddress2 = (PULONG)((ULONG)pAddress2+1);

      //*pAddress2��ʾ��ô˵�ַ�����ݣ����ݳ���Ϊ4���ֽ�
      return *pAddress2;
 }

 return -1;
}

//ͨ����ַ�õ�ģ����
BOOL GetModuleNameByAddr(ULONG ulAddr,PSYSTEM_MODULE_LIST pSystem_module_list,LPSTR lpBuffer)
{
	ULONG i;

	for (i=0;i<pSystem_module_list->ulCount; i++)
	{
		//�õ�����ģ���ַ
		ULONG ulBase = (ULONG)pSystem_module_list->smi[i].Base;
		//�õ�����ģ���С
		ULONG ulMax  = ulBase + pSystem_module_list->smi[i].Size;

		//���ulAddr����ģ���ַ֮��Ļ���˵�����Ǵ�ģ������ĺ���
		if (ulBase<=ulAddr && ulAddr<=ulMax)
		{
			//���Ƶ�������
			lstrcpynA(lpBuffer,pSystem_module_list->smi[i].ImageName,sizeof(pSystem_module_list->smi[i].ImageName));
			//������
			return TRUE;
		}
	}

	//û���ҵ��Ļ����ؼ�
	return FALSE;
}

//���ݷ���Ž�SSDT���������
VOID SSDTSTOrderByServiceNum()
{
	ULONG i, j;

	//ð������
	for(i = ulSSDTTotal - 1; i > 0; i --)
	{
		for(j = 0; j < i; j ++)
		{
			if(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)))->ulServiceNumber > ((PSSDTSaveTable)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)))->ulServiceNumber)
			{
				//����
				SSDTSaveTable SSDTSTTemp;
				memcpy((LPVOID)&SSDTSTTemp,(LPVOID)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)),sizeof(SSDTSaveTable));
				memcpy((LPVOID)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)),(LPVOID)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)),sizeof(SSDTSaveTable));
				memcpy((LPVOID)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)),(LPVOID)&SSDTSTTemp,sizeof(SSDTSaveTable));
			}
		}
	}
}

//ö��SSDT
BOOL GetCurrentSSDTFunc(HANDLE hDriver)
{
	ULONG i;
	ULONG ulReal=0;
	DWORD dwRet=0;
	//�ں��ļ���ַ
	ULONG ulNTBase;
	//ntdll���
	HMODULE hNtdll;

	//����ģ����Ϣ����
	pSystem_Module_List = GetSystemModuleList();

	//�������ʧ��
	if(pSystem_Module_List == NULL)
	{
		return FALSE;
	}
	//�õ�NT�ں��ļ���ַ
	ulNTBase = (ULONG)(pSystem_Module_List->smi->Base);

	//���ntdll�����ȡʧ��
	if(!(hNtdll=LoadLibrary(L"ntdll.dll")))
	{
		return FALSE;
	}

	//�õ�SSDT����
	BOOL bRet = DeviceIoControl(hDriver,IOCTL_SSDT_GET, NULL, 0, &ulSSDTTotal, sizeof(ULONG), &dwRet, NULL);
	//�����óɹ�
	if(!bRet)
	{
		return NULL;
	}

	//���
	pSSDTSaveTable  = (PSSDTSaveTable)malloc(ulSSDTTotal*sizeof(SSDTSaveTable));
	//��������ڴ�ʧ��
	if(pSSDTSaveTable == NULL)
	{
		return FALSE;
	}

	//��ʼ����
	for(i = 0; i < ulSSDTTotal; i ++ )
	{
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulServiceNumber = -1;
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulCurrentFuncAddress = 0L;
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulOriginalFuncAddress = 0L;
		//����ServiceFunctionName
		memset(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ServiceFunctionName, 0, MAX_PATH);
		//����ModuleName
		memset(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ModuleName, 0, MAX_PATH);
	}

	//ö��
	ULONG ulNtDllOffset = (ULONG)hNtdll;
	//PEͷ��
	ulNtDllOffset = ulNtDllOffset + ((PIMAGE_DOS_HEADER)hNtdll)->e_lfanew + 4;
	//����Ŀ¼
	ulNtDllOffset = ulNtDllOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) - IMAGE_NUMBEROF_DIRECTORY_ENTRIES*sizeof(IMAGE_DATA_DIRECTORY);
	//������
	ulNtDllOffset = (DWORD)hNtdll + ((PIMAGE_DATA_DIRECTORY)ulNtDllOffset)->VirtualAddress;

	//����Ŀ¼��
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ulNtDllOffset;
	//�õ���������
	ULONG ulFuncNameCount = pImage_export_directory->NumberOfNames;

	//����������ָ��
	PULONG pFuncName = (PULONG)((ULONG)hNtdll + pImage_export_directory->AddressOfNames);

	//ѭ���Һ�����
	for( i = 0; i < ulFuncNameCount; i ++ )
	{
		//������
		LPSTR pszName = (LPSTR)((ULONG)hNtdll + pFuncName[i]);

		//Nt ��ͷ�ĺ���
		if(pszName[0] == 'N' && pszName[1] == 't')	
		{
			//��ú����ķ����
			ULONG ulIndex = GetSSDTFuncIndex(pszName);

			if (ulIndex==-1)
			{
				continue;
			}

			ULONG ulAddr = 0;
			DeviceIoControl(hDriver, IOCTL_SSDT_GETFUNCADDR, &ulIndex, sizeof(ULONG), &ulAddr, sizeof(ULONG), &dwRet, NULL);

			//ͨ����ַ�õ�ģ����
			char buffer[MAX_PATH];
			memset(buffer, 0, sizeof(buffer));

			//���ģ����
			if(GetModuleNameByAddr(ulAddr, pSystem_Module_List, buffer))
			{
				memcpy(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ModuleName, buffer, sizeof(buffer));
			}

			//����SSDT��Ϣ���������
			((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ulServiceNumber = ulIndex;
			((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ulCurrentFuncAddress = ulAddr;
			memcpy(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ServiceFunctionName, pszName,sizeof(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ServiceFunctionName));

			ulReal++;
		}
	}

	return TRUE;
}




DWORD FindKiServiceTable(HMODULE hModule,DWORD ulKeServiceDescriptorTable)
{ 
	PIMAGE_FILE_HEADER    pImage_file_header;
	PIMAGE_OPTIONAL_HEADER    pImage_optional_header;
	PIMAGE_SECTION_HEADER    pImage_section_header;
	PIMAGE_BASE_RELOCATION    pbr;
	PIMAGE_FIXUP_ENTRY    pfe;

	DWORD    dwFixups=0,i,dwPointerRva,dwPointsToRva,dwKiServiceTable;
	BOOL    bFirstChunk;

	//���DOSͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)hModule;

	//������ЧPE��־
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		return FALSE;
	}

	//NTͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//������ЧPE��־
	if (pImage_nt_header->Signature!=IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	//�ļ�ͷ��
	pImage_file_header = (PIMAGE_FILE_HEADER)((ULONG)pImage_nt_header + sizeof(IMAGE_NT_SIGNATURE));

	//��ѡͷ��
	pImage_optional_header = (PIMAGE_OPTIONAL_HEADER)((ULONG)pImage_file_header + sizeof(IMAGE_FILE_HEADER));

	//��ͷ��
	pImage_section_header = (PIMAGE_SECTION_HEADER)((ULONG)pImage_optional_header + sizeof(IMAGE_OPTIONAL_HEADER));

	if ((pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) &&
		(!((pImage_file_header->Characteristics)&IMAGE_FILE_RELOCS_STRIPPED))) {

			pbr=(PIMAGE_BASE_RELOCATION)RVATOVA(pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,hModule);

			bFirstChunk=TRUE;

			while (bFirstChunk || pbr->VirtualAddress) { 
				bFirstChunk=FALSE;

				pfe=(PIMAGE_FIXUP_ENTRY)((DWORD)pbr+sizeof(IMAGE_BASE_RELOCATION));

				for (i=0;i<(pbr->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))>>1;i++,pfe++) { 
					if (pfe->type==IMAGE_REL_BASED_HIGHLOW) { 
						dwFixups++;
						dwPointerRva=pbr->VirtualAddress+pfe->offset;

						dwPointsToRva=*(PDWORD)((DWORD)hModule+dwPointerRva)-(DWORD)pImage_optional_header->ImageBase;

						if (dwPointsToRva==ulKeServiceDescriptorTable) { 

							if (*(PWORD)((DWORD)hModule+dwPointerRva-2)==0x05c7) {

								dwKiServiceTable=*(PDWORD)((DWORD)hModule+dwPointerRva+4)-pImage_optional_header->ImageBase;
								return dwKiServiceTable;
							}
						}

					} 

				}
				*(PDWORD)&pbr+=pbr->SizeOfBlock;
			}
	}    

	return 0;
}

VOID GetOldSSDTAddress()
{   
	PIMAGE_FILE_HEADER        pImage_file_header;
	PIMAGE_OPTIONAL_HEADER    pImage_optional_header;
	PIMAGE_SECTION_HEADER     pImage_section_header;

	//��û�ַ
	ULONG ulKernelBase = (ULONG)(pSystem_Module_List->smi->Base);;

	//�õ�kernel�ļ���������
	PCHAR pKernelName=pSystem_Module_List->smi->ModuleNameOffset + pSystem_Module_List->smi->ImageName;

	//����kernel
	HMODULE hKernel=LoadLibraryExA(pKernelName,0,DONT_RESOLVE_DLL_REFERENCES);

	//���ʧ��
	if (hKernel==NULL) 
	{
		return;
	}

	//���SSDT���ַ
	ULONG ulKeServiceDescriptorTable=(DWORD)GetProcAddress(hKernel,"KeServiceDescriptorTable");

	//������ʧ��
	if (ulKeServiceDescriptorTable==NULL) 
	{
		return;
	}

	//������ƫ����
	ulKeServiceDescriptorTable = ulKeServiceDescriptorTable - (ULONG)hKernel;

	//����KiServiceTable��ַ
	ULONG ulKiServiceTable = FindKiServiceTable(hKernel,ulKeServiceDescriptorTable);

	//���ʧ��
	if (ulKiServiceTable==NULL)
	{ 
		return;
	}

	//������PE�ṹ��
	//���DOSͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)hKernel;

	//������ЧPE��־
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		return;
	}

	//NTͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//������ЧPE��־
	if (pImage_nt_header->Signature!=IMAGE_NT_SIGNATURE)
	{
		return;
	}

	//�ļ�ͷ��
	pImage_file_header = (PIMAGE_FILE_HEADER)((ULONG)pImage_nt_header + sizeof(IMAGE_NT_SIGNATURE));

	//��ѡͷ��
	pImage_optional_header = (PIMAGE_OPTIONAL_HEADER)((ULONG)pImage_file_header + sizeof(IMAGE_FILE_HEADER));

	//��ͷ��
	pImage_section_header = (PIMAGE_SECTION_HEADER)((ULONG)pImage_optional_header + sizeof(IMAGE_OPTIONAL_HEADER));

	PULONG    pService;
	ULONG ulServices=0;

	for (pService=(PULONG)((ULONG)hKernel + ulKiServiceTable); *pService - pImage_optional_header->ImageBase < pImage_optional_header->SizeOfImage; pService++,ulServices++)
	{
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulServices * sizeof(SSDTSaveTable)))->ulOriginalFuncAddress = *pService - pImage_optional_header->ImageBase + ulKernelBase;
	}

	FreeLibrary(hKernel);
}