// Page9.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page9.h"
#pragma warning( disable: 4311 )
#pragma warning( disable: 4312 )
#pragma warning( disable: 4996 )


// CPage9 �Ի���

IMPLEMENT_DYNAMIC(CPage9, CDialog)

CPage9::CPage9(CWnd* pParent /*=NULL*/)
: CDialog(CPage9::IDD, pParent)
{

}

CPage9::~CPage9()
{
}

void CPage9::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PePrint);
	DDX_Control(pDX, IDC_EDIT2, m_PEPath);
}


BEGIN_MESSAGE_MAP(CPage9, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CPage9::OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH CPage9::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
// CPage9 ��Ϣ�������

BOOL CPage9::LoadFile(LPTSTR lpFilename,PMAP_FILE_STRUCT pstMapFile)
{

	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;

	memset(pstMapFile,0,sizeof(MAP_FILE_STRUCT));

	hFile=CreateFile(lpFilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,0);

	if (!hFile)       
		return FALSE;

	hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	if(!hMapping)
	{         
		CloseHandle(hFile);
		return FALSE;
	}
	ImageBase=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0);
	if(!ImageBase)
	{         
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return FALSE;
	}
	pstMapFile->hFile=hFile;
	pstMapFile->hMapping=hMapping;
	pstMapFile->ImageBase=ImageBase;
	return TRUE;
}

void CPage9::UnLoadFile(PMAP_FILE_STRUCT pstMapFile)
{
	if(pstMapFile->ImageBase)
		UnmapViewOfFile(pstMapFile->ImageBase);

	if(pstMapFile->hMapping)
		CloseHandle(pstMapFile->hMapping);

	if(pstMapFile->hFile)
		CloseHandle(pstMapFile->hFile);

}


BOOL CPage9::IsPEFile(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDH=NULL;
	PIMAGE_NT_HEADERS pNtH=NULL;

	if(!ImageBase)
		return FALSE;

	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	if(pDH->e_magic!=IMAGE_DOS_SIGNATURE)
		return FALSE;

	pNtH=(PIMAGE_NT_HEADERS32)((DWORD)pDH+pDH->e_lfanew);
	if (pNtH->Signature != IMAGE_NT_SIGNATURE )
		return FALSE;

	return TRUE;

}

PIMAGE_NT_HEADERS CPage9::GetNtHeaders(LPVOID ImageBase)
{

	if(!IsPEFile(ImageBase))
		return NULL;
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_DOS_HEADER pDH;
	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);

	return pNtH;

}

PIMAGE_FILE_HEADER   CPage9::GetFileHeader(LPVOID ImageBase)
{      
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_FILE_HEADER pFH=NULL;
	pNtH=GetNtHeaders(ImageBase);
	pFH=&pNtH->FileHeader;
	return pFH;
}

PIMAGE_OPTIONAL_HEADER CPage9::GetOptionalHeader(LPVOID ImageBase)
{
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pOH=&(GetNtHeaders(ImageBase))->OptionalHeader;
	return pOH;
}

PIMAGE_SECTION_HEADER CPage9::GetFirstSectionHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS     pNtH=NULL;
	PIMAGE_SECTION_HEADER pSH=NULL;

	pNtH=GetNtHeaders(ImageBase);
	pSH=IMAGE_FIRST_SECTION(pNtH);
	return pSH;
}

LPVOID CPage9::RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA)
{ 
	return ::ImageRvaToVa(pNtH,ImageBase,dwRVA,NULL);

}

LPVOID CPage9::GetDirectoryEntryToData(LPVOID ImageBase,USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	LPVOID pDirData=NULL;
	PIMAGE_NT_HEADERS     pNtH=NULL;
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pNtH=GetNtHeaders(ImageBase);
	if(!pNtH)
		return NULL;
	pOH=GetOptionalHeader(ImageBase);//�е����⣡
	if(!pOH)
		return NULL;
	dwDataStartRVA=pOH->DataDirectory[DirectoryEntry].VirtualAddress;
	if(!dwDataStartRVA)
		return NULL;

	pDirData=RvaToPtr(pNtH,ImageBase,dwDataStartRVA);
	if(!pDirData)
		return NULL; 
	return pDirData;
}




void CPage9::PrintFileHeader(PIMAGE_FILE_HEADER pFileHeader)
{
	tm *createTime;
	switch(pFileHeader->Machine)
	{
	case 0X14C:
		PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUΪ Intel i386\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0X162:
		PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUΪ MIPS R3000\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x166:
		PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUΪ MIPS R4000\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x184:
		PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUΪ Alpha AXP\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x1F0:
		PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUΪ Power PC\r\n");PrintEdit+=PrintEditTem;
		break;
	default:PrintEditTem.Format(L"��ִ���ļ���Ŀ��CPUδ֪\r\n");

	}
	PrintEditTem.Format(L"�ļ���������Ŀ:%d\r\n",pFileHeader->NumberOfSections);PrintEdit+=PrintEditTem;
	createTime=gmtime((time_t *)&pFileHeader->TimeDateStamp);
	WCHAR wBuf[50];
	char  cBuf[50];
	strcpy(cBuf,asctime(createTime));
	CMySystem::CharToWCHAR(wBuf,cBuf);
	PrintEditTem.Format(L"�ļ��Ĵ���ʱ��GMT is: %ws\r\n", wBuf);

	PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"IAMGE_OPTIONAL_HEADER32�ṹ��С:%d\r\n",pFileHeader->SizeOfOptionalHeader);PrintEdit+=PrintEditTem;
	if(pFileHeader->Characteristics==0x1000)
		PrintEditTem.Format(L"���ļ�Ϊϵͳ�ļ�\r\n");PrintEdit+=PrintEditTem;
	if(pFileHeader->Characteristics==0x2000)
		PrintEditTem.Format(L"���ļ�ΪDLL�ļ�\r\n");PrintEdit+=PrintEditTem;
}

void CPage9::PrintOptionAlHeader(PIMAGE_OPTIONAL_HEADER pNt)
{
	PrintEditTem.Format(L"\r\n�����������汾��:%d,�ΰ汾��:%d\r\n",pNt->MajorLinkerVersion,pNt->MinorLinkerVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"���к��д���������ܴ�С%ld\r\n",pNt->SizeOfCode);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"���г�ʼ������������ܴ�С%ld\r\n",pNt->SizeOfInitializedData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����δ��ʼ������������ܴ�С%ld\r\n",pNt->SizeOfUninitializedData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����ִ�����RAV(AddressOfEntryPoint):%X\r\n",pNt->AddressOfEntryPoint);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����������ʼRVA(BaseOfCode):%X\r\n",pNt->BaseOfCode);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����������ʼRVA(BaseOfData):%X\r\n",pNt->BaseOfData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����Ĭ��װ�����ַ(ImageBase):%X\r\n",pNt->ImageBase);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����ϵͳ���汾��%d\r\n",pNt->MajorOperatingSystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����ϵͳ�ΰ汾��%d\r\n",pNt->MinorOperatingSystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����Ҫ�ĵ���ϵͳ���汾��%d\r\n",pNt->MajorSubsystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����Ҫ�ĵ���ϵͳ�ΰ汾��%d\r\n",pNt->MinorSubsystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"ӳ��װ���ڴ����ܳߴ�(SizeOfIamge):%d\r\n",pNt->SizeOfImage);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"DOSͷ,PEͷ,������ܴ�С%d\r\n",pNt->SizeOfHeaders);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"ӳ��Ч���%ld\r\n",pNt->CheckSum);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"�ļ���ϵͳ%d\r\n",pNt->Subsystem);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"����Ŀ¼�������%d\r\n",pNt->NumberOfRvaAndSizes);PrintEdit+=PrintEditTem;

}

void CPage9::PrintSectionInfo()
{
	PIMAGE_FILE_HEADER       pFH=NULL;
	PIMAGE_SECTION_HEADER   pSH=NULL;
	WCHAR wBuf[50];
	char  cBuf[50];
	pFH=GetFileHeader(MapFile.ImageBase);
	if(!pFH)
		return;
	pSH=GetFirstSectionHeader(MapFile.ImageBase);
	PrintEditTem.Format(L"\r\nPE�ļ���������Ϣ:\r\n");PrintEdit+=PrintEditTem;
	for(int i=0;i<pFH->NumberOfSections;i++)
	{

		PrintEditTem.Format(L"����ģ��:%ws\r\n",wBuf);
		strcpy(cBuf,(char *)pSH->Name);
		CMySystem::CharToWCHAR(wBuf,cBuf);
		PrintEditTem.Format(L"����:%ws\t",wBuf);PrintEdit+=PrintEditTem;

		PrintEditTem.Format(L"VirtualSize:%x\t",pSH->Misc);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"VirtualAddress:%x\t",pSH->VirtualAddress);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"SizeOfRawData:%x\t",pSH->SizeOfRawData);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"SizeOfRawData:%x\t",pSH->SizeOfRawData);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"�ļ�����(Characteristics):%X\r\n",pSH->Characteristics);PrintEdit+=PrintEditTem;
		pSH++;
	}
}

void CPage9::printIAT()
{
	//ULONG size;
	WCHAR wBuf[60];
	char  cBuf[60];
	PIMAGE_IMPORT_DESCRIPTOR pImport;
	pImport = (IMAGE_IMPORT_DESCRIPTOR *)GetDirectoryEntryToData(MapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (!pImport)
	{
		PrintEditTem.Format(L"����������ģ�飡\r\n");PrintEdit+=PrintEditTem;
		return;
	}

	//pImport=(PIMAGE_IMPORT_DESCRIPTOR)((BYTE *)MapFile.ImageBase+GetOptionalHeader(MapFile.ImageBase)->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	while(pImport->FirstThunk)
	{
		char *pszDllName=(char *)RvaToPtr(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->Name);//(char *)((BYTE *)MapFile.ImageBase+pImport->Name);
		strcpy(cBuf,pszDllName);
		CMySystem::CharToWCHAR(wBuf,cBuf);
		PrintEditTem.Format(L"����ģ��:%ws\r\n",wBuf);
		PrintEdit+=PrintEditTem;
		IMAGE_THUNK_DATA * pThunk=(IMAGE_THUNK_DATA *)(::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->OriginalFirstThunk,NULL));
		int n=0;
		if (NULL==pThunk)
		{
			return;
		}
		while(pThunk->u1.Function)
		{
			char * pszFunName=(char *)(::ImageRvaToVa((PIMAGE_NT_HEADERS)GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,(unsigned long)pThunk->u1.AddressOfData,NULL));
			PDWORD lpAddr=(DWORD*)(::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->FirstThunk,NULL))+n;//((BYTE *)MapFile.ImageBase+pImport->FirstThunk)+n;
			if (NULL==pszFunName)
			{
				break;
			}
			strcpy(cBuf,pszFunName+2);
		    CMySystem::CharToWCHAR(wBuf,cBuf);
			PrintEditTem.Format(L"�Ӵ�ģ�鵼��ĺ���:%-25ws,",wBuf);
			PrintEdit+=PrintEditTem;
			PrintEditTem.Format(L"������ַ:%X\r\n",lpAddr);PrintEdit+=PrintEditTem;
			n++;
			pThunk++;
		}
		pImport++;
	}
}


void CPage9::printET()
{
	WCHAR wBuf[60];
	char  cBuf[60];
	IMAGE_EXPORT_DIRECTORY * pExport;
	pExport = (IMAGE_EXPORT_DIRECTORY *)GetDirectoryEntryToData(MapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!pExport)
	{
		return;
	}
	strcpy(cBuf,(char *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->Name,NULL));
	CMySystem::CharToWCHAR(wBuf,cBuf);

	PrintEditTem.Format(L"ģ�����ʵ����%ws\r\n",wBuf);PrintEdit+=PrintEditTem;
	int n;
	n=pExport->NumberOfFunctions;
	PrintEditTem.Format(L"ģ���к�������:%d\r\n",n);PrintEdit+=PrintEditTem;
	DWORD *AddressF=(DWORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfFunctions,NULL);
	DWORD *AddressName=(DWORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfNames,NULL);
	WORD *AddressNameO=(WORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfNameOrdinals,NULL);
	for(int i=0;i<n;i++)
	{
		strcpy(cBuf,(char *)ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,*AddressName,NULL));
		CMySystem::CharToWCHAR(wBuf,cBuf);
		
		PrintEditTem.Format(L"����������:%-30s,��ַ:%-4x,˳�к�:%d\r\n",wBuf,*AddressF,*AddressNameO);PrintEdit+=PrintEditTem;
		AddressName++;
		AddressF++;
		AddressNameO++;
	}

}
void CPage9::OnDropFiles(HDROP hDropInfo)
{
	// ����ק���ļ����ļ���
	WCHAR szFileName[MAX_PATH + 1];
	// �õ�����ק���ļ���
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	// ���ļ�����ʾ����
	m_PEPath.SetWindowText(szFileName);

	CDialog::OnDropFiles(hDropInfo);
}
void CPage9::OnBnClickedOk()
{
	
	LPTSTR fileName=L"C:\\Documents and Settings\\Administrator\\����\\����ֿ�\\Debug\\DecodeSystemBuffer.exe";
    CString csPath;
	m_PEPath.GetWindowText(csPath);
	m_PePrint.SetWindowText(L"");
	PrintEdit.Empty();
	LoadFile(csPath.GetBuffer(),&MapFile);
	if(!IsPEFile(MapFile.ImageBase))
	{
		PrintEditTem.Format(L"PE�ļ���Ϣ��ȡʧ�ܡ���ȷ����PE�ļ������ļ�δ��ʹ�ã�\r\n");PrintEdit+=PrintEditTem;
		goto _exit ;
	}
	PrintFileHeader(GetFileHeader(MapFile.ImageBase));
	PrintOptionAlHeader(GetOptionalHeader(MapFile.ImageBase));
	PrintSectionInfo();
	printET();
	printIAT();
	getchar();
	UnLoadFile(&MapFile);

	
	
_exit:
	m_PePrint.SetWindowText(PrintEdit);
    return ;

}


