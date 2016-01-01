// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page1.h"
#include "MySystem.h"
#include "tlhelp32.h"
#include "shlwapi.h"
#include "g:\PSAPI.H"
#pragma comment( lib, "g:\\PSAPI.LIB" )
// CPage1 �Ի���

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
	, dwProcessId(0)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProListCtrl);
	DDX_Control(pDX, IDC_LIST2, m_DllListCtrl);
	DDX_Control(pDX, IDC_LIST3, m_ThreaList);
	DDX_Control(pDX, IDC_STATIC_PRO, m_ProState);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDOK, &CPage1::OnBnClickedOk)
    ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPage1::OnNMRclickList1)
	ON_COMMAND(ID_MENU_32771, &CPage1::OnMenuCopyProcessName)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST2, &CPage1::OnLvnItemActivateList2)//û�õ�
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1, &CPage1::OnLvnItemActivateList1)//��Ӧ��ģ�顢�߳���Ϣ
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CPage1::OnNMRclickList2)
	ON_COMMAND(ID_MENU_32773, &CPage1::OnMenuUnmapViewOfModule)
	ON_COMMAND(ID_MENU_32772, &CPage1::OnMenuKillPro)
	ON_COMMAND(ID_MENU_32774, &CPage1::OnMenuFreeModuleByPid)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPage1::OnNMRclickList3)
	ON_COMMAND(ID_MENU_32785, &CPage1::OnMenuForceKillThread)
	ON_COMMAND(ID_MENU_32786, &CPage1::OnMenuForceKillProcess)
	ON_COMMAND(ID_MENU_32789, &CPage1::OnMenuKillProcessByTer)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage1::OnBnClickedListProcess)
	ON_COMMAND(ID_MENU_DD, &CPage1::OnMenuListProcessInfo)
	ON_COMMAND(ID_MENU_32784, &CPage1::OnMenuKillThreadByTerminateThread)
END_MESSAGE_MAP()



BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();
    CBrush          m_brushBlue; 
	 m_brushBlue.CreateSolidBrush(RGB(0,0,0));  
	HICON g_icon;
	TCHAR g_path[MAX_PATH]=_T("C:\\KByS.exe");
	SHFILEINFO sfi;
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
//  WCHAR ErrCode[100];
	ZeroMemory(&sfi,sizeof(SHFILEINFO));
	SHGetFileInfo(g_path,0,&sfi,sizeof(SHFILEINFO),SHGFI_ICON);
	g_icon = sfi.hIcon;


	m_ProImageList.Create ( 20, 20, ILC_COLOR16 | ILC_MASK, 4, 1 );//big or small 

	HICON hKenny = reinterpret_cast<HICON>(
		::LoadImage ( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR ));
    //�����Ϸ����б��
	m_ProListCtrl.SetImageList ( &m_ProImageList, LVSIL_SMALL );
	m_ProListCtrl.InsertColumn(0,L"������",LVCFMT_CENTER,80,-1);
	m_ProListCtrl.InsertColumn(1,L"����ID",LVCFMT_CENTER,80,-1);
	m_ProListCtrl.InsertColumn(2,L"����·��",LVCFMT_LEFT ,80,-1);
	m_ProListCtrl.InsertColumn(3,L"������Ϣ",LVCFMT_LEFT ,80,-1);

	m_ProListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_ProListCtrl.SetExtendedStyle (  LVS_EX_FULLROWSELECT |LVS_EX_SUBITEMIMAGES  );
	m_ProListCtrl.SetColumnWidth(0,   100);//�����п�
	m_ProListCtrl.SetColumnWidth(2,   300);//�����п�
	m_ProListCtrl.SetColumnWidth(3,   160);//�����п�
	m_ProListCtrl.Invalidate();
    //�����·���dll�б��
	m_DllListCtrl.SetImageList ( &m_ProImageList, LVSIL_SMALL );
	m_DllListCtrl.InsertColumn(0,L"ģ���ַ",LVCFMT_LEFT,80,-1);
	m_DllListCtrl.InsertColumn(1,L"ģ���С",LVCFMT_LEFT,80,-1);
	m_DllListCtrl.InsertColumn(2,L"ģ��ӳ��·��",LVCFMT_LEFT ,80,-1);
	m_DllListCtrl.InsertColumn(3,L"ģ��ӳ����Ϣ",LVCFMT_LEFT ,80,-1);

	m_DllListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_DllListCtrl.SetExtendedStyle (  LVS_EX_FULLROWSELECT |LVS_EX_SUBITEMIMAGES  );
	m_DllListCtrl.SetColumnWidth(2,   250);//�����п�
	m_DllListCtrl.SetColumnWidth(3,   250);//�����п�
	m_DllListCtrl.Invalidate();
	//�����·����߳��б��
	m_ThreaList.SetImageList ( &m_ProImageList, LVSIL_SMALL );
	m_ThreaList.InsertColumn(0,L"�߳�Cid",LVCFMT_LEFT,80,-1);
	m_ThreaList.InsertColumn(1,L"�߳�EThread",LVCFMT_LEFT,80,-1);
	m_ThreaList.InsertColumn(2,L"�߳���ʼ��ַ�������ο���",LVCFMT_LEFT ,80,-1);
	m_ThreaList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_ThreaList.SetExtendedStyle (  LVS_EX_FULLROWSELECT |LVS_EX_SUBITEMIMAGES  );
	m_ThreaList.SetColumnWidth(2,   450);//�����п�
	m_ThreaList.Invalidate();
_Again:
    CMySystem:: LoadDriver(L"KsBinSword");
    CMySystem::hMyHandle = CMySystem::OpenDevice(L"\\\\.\\KsBinSword");

	if ((HANDLE)-1==CMySystem::hMyHandle||(HANDLE)NULL==CMySystem::hMyHandle||(HANDLE)0xCCCCCCCC==CMySystem::hMyHandle)
	{
		TRACE("hMyHandle:%x,%d",CMySystem::hMyHandle,GetLastError());
		if(IDYES==::MessageBox(0, L"ϵͳ��æ�����ؽ�������ʧ��,��Ҫ���¼�����\n��������أ�ĳЩ���ܽ��޷�ʹ�ã�",L"����",MB_YESNO))
		{
			CMySystem:: UnloadDriver(L"KsBinSword");
			CMySystem:: LoadDriver(L"KsBinSword");
			goto _Again;
		}

		return false;

	}


	DWORD * addr=(DWORD *)(1+(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"NtCreateProcess"));
	ZeroMemory(CMySystem::outputbuff,256);
	CMySystem::controlbuff[0]=addr[0];
	CMySystem::controlbuff[1]=(DWORD)&CMySystem::outputbuff[0];
	::CreateThread(0,0,(LPTHREAD_START_ROUTINE)CMySystem::ThreadMon,0,0,&CMySystem::dw);
	return true;
}

// CPage1 ��Ϣ�������
HBRUSH CPage1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
void CPage1::ListProcessByDrive()
{

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	typedef  BOOL (WINAPI *PFNVerQueryValueW)( 
		const LPVOID pBlock,
		LPWSTR lpSubBlock,
		LPVOID * lplpBuffer,
		PUINT puLen
		);
	typedef  BOOL (WINAPI *PFNGetFileVersionInfoW)( 
		LPWSTR lptstrFilename, 
		DWORD dwHandle,        
		DWORD dwLen,         
		LPVOID lpData
		);
	typedef  DWORD (WINAPI *PFNGetFileVersionInfoSizeW)( 
		LPWSTR lptstrFilename, /* Filename of version stamped file */
		LPDWORD lpdwHandle
		);
	HMODULE hModule = LoadLibrary (L"version.dll" ) ;
	PFNVerQueryValueW VerQueryValueW = (PFNVerQueryValueW)GetProcAddress ( hModule, "VerQueryValueW" ) ;
	PFNGetFileVersionInfoW GetFileVersionInfoW = (PFNGetFileVersionInfoW)GetProcAddress ( hModule, "GetFileVersionInfoW" ) ;
	PFNGetFileVersionInfoSizeW GetFileVersionInfoSizeW = (PFNGetFileVersionInfoSizeW)GetProcAddress ( hModule, "GetFileVersionInfoSizeW" ) ;
	if (!VerQueryValueW||!GetFileVersionInfoW||!GetFileVersionInfoSizeW)
	{
		return ;
	}
	WCHAR*   lpBuffer;   
	WCHAR   szSubBlock[256];  
	INT   nVersionLen;
	struct   LANGANDCODEPAGE   {   
		WORD   wLanguage;   
		WORD   wCodePage;   
	}   *lpTranslate;  

	WCHAR*   pBuffer;
	UINT   cbTranslate;
	UINT dwBytes;

	int ImageListCount=0;
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	HICON g_icon;
	SHFILEINFO sfi;
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	WCHAR PROID[MAX_PATH];
	int i=0;
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	DWORD_PTR SHret;
	WCHAR szPath[256]={'0'};
	HANDLE hMyPro;
	DWORD ret;
	DWORD dwOldProtect;
	int nLen =0;
	HICON hKenny = reinterpret_cast<HICON>(
		::LoadImage ( AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_BLANK),
		IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR ));

	//���
	m_ProListCtrl.DeleteAllItems();
	m_ThreaList.DeleteAllItems();

	m_DllListCtrl.DeleteAllItems();
	ImageListCount= m_ProImageList.GetImageCount();
	for(int i=0;i<ImageListCount;i++)   
	{   
		m_ProImageList.Remove(0);   

	}   

	int ProNum=0,nTryNum=0;	
	ProcessInfo ProTem;	
	void* ProN;
	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		
		AfxMessageBox(L"�����豸����ʧ�ܣ�");
		return ;
	}
Again_:
	ProNum=CMySystem::GetProNum(CMySystem:: hMyHandle);

	printf("ProNum:%d\n",ProNum);

	ProN=(void*)malloc(ProNum*sizeof(ProcessInfo));

	if (!ProN)
	{

		CMySystem::ClearPro(CMySystem:: hMyHandle);
		AfxMessageBox(L"ProN malloc fail");
		return;
	}
	memset(ProN,0,ProNum*sizeof(ProcessInfo));
	VirtualProtect( ProN,ProNum*sizeof(ProcessInfo),PAGE_READWRITE,&dwOldProtect );
	DWORD ErrCode=CMySystem::GetPro( CMySystem::hMyHandle,ProN,(2*ProNum)*sizeof(ProcessInfo));
	if (998==ErrCode)
	{ 
		nTryNum++;
		if (nTryNum>4)
		{
		   AfxMessageBox(L"ϵͳ��æ�������޷���ȡ��ȷ�Ľ����б� !");
		   return;
		}
        AfxMessageBox(L"ϵͳ��æ�������ٴγ��������оٽ��� !");
		Sleep(2000);
        goto Again_;
	}
    //CString csErrCode;csErrCode.Format(L"%x",ErrCode);
    //AfxMessageBox(csErrCode);
	for (int i=0;i<ProNum-1;i++)
	{
		ProTem=*((ProcessInfo*)(ProN)+i);
		printf("%s,%x,%d\n",ProTem.name,ProTem.addr,ProTem.pid);
		//hMyPro=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,false,ProTem.pid);
		hMyPro=
			CMySystem::My_OpenProcess (
			PROCESS_ALL_ACCESS,
			0,
			ProTem.pid,
			true,
			(LPDWORD)0
			);
		ret =GetModuleFileNameEx(
			hMyPro,
			NULL,
			szPath,
			256
			);

		ZeroMemory(&sfi,sizeof(SHFILEINFO));
		SHret=SHGetFileInfoW(szPath,0,&sfi,sizeof(SHFILEINFO),SHGFI_ICON);//mInfo.szExePath��ȫ·��
		if (0==SHret)
		{
			m_ProImageList.Add ( hKenny );
		}
		else
		{
			g_icon = sfi.hIcon;
			m_ProImageList.Add ( g_icon );
		}


		if (!ret)
		{
			StrCpyW(szPath,L"·����ȡʧ��");
		}
		wsprintf(PROID,L"%S",ProTem.name);
		m_ProListCtrl.InsertItem(i,PROID,i); 

		ZeroMemory(&sfi,sizeof(MAX_PATH));
		wsprintf(PROID,L"%d",ProTem.pid);
		m_ProListCtrl.SetItemText(i,1,PROID); 
		m_ProListCtrl.SetItemText(i,2,szPath);//�����һ�� 
		nVersionLen =   GetFileVersionInfoSizeW(   szPath,   NULL   );   
		if   (   nVersionLen   <=   0   )   
		{   
			continue   ;   
		}   

		pBuffer =   new   WCHAR[   nVersionLen   ];  
		if   (   !GetFileVersionInfo(   szPath,   NULL,   nVersionLen,   pBuffer   )   )   
		{   
			continue   ;   
		}  
		VerQueryValueW(   pBuffer,   L"\\VarFileInfo\\Translation",   
			(LPVOID*)&lpTranslate,   &cbTranslate   );  
		wsprintf(   szSubBlock,L"\\StringFileInfo\\%04x%04x\\CompanyName",   
			lpTranslate[0].wLanguage,   lpTranslate[0].wCodePage   ); 
		if(   VerQueryValueW(   pBuffer,   szSubBlock,   (   LPVOID*   )&lpBuffer,   &dwBytes   )   )   
		{   
			m_ProListCtrl.SetItemText(i,3,lpBuffer);  
		}  
		else{
			m_ProListCtrl.SetItemText(i,3,L"�õ��ļ���Ϣʧ��");  
		}

		CloseHandle(hMyPro);
		//printf("%s\n",path);

	}
	free(ProN);

}
void CPage1::OnBnClickedOk()//�н���
{

	if(IDYES==::MessageBox(0, L"�������оٽ��̣���һ��Σ�գ�ȷ��Ҫ���У�",L"����",MB_YESNO))
	{

		ListProcessByDrive();
		
	}
	

}

//�����Ҽ��˵�
void CPage1::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	}
	*pResult = 0;

}

void CPage1::OnMenuCopyProcessName()
{
	//�õ�������
	int CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
    CString str=m_ProListCtrl.GetItemText(CurSel,2); // 0=<col<m_list������

	DWORD dwLength = str.GetLength(); // Ҫ���Ƶ��ִ�����
	LPBYTE lpWcharBuf=(LPBYTE)malloc(dwLength*sizeof(WCHAR) + 2);
	
	HANDLE hGlobalMemory = GlobalAlloc(GHND, dwLength*2 + 2); // �����ڴ�
	LPBYTE lpGlobalMemory = (LPBYTE)GlobalLock(hGlobalMemory); // �����ڴ�
	memcpy(lpWcharBuf,str.GetBuffer(),dwLength*sizeof(WCHAR)+2);
	CMySystem::WCHARToChar((char *)lpGlobalMemory,(WCHAR *)lpWcharBuf);
	GlobalUnlock(hGlobalMemory); // �����ڴ�����
	HWND hWnd = GetSafeHwnd(); // ��ȡ��ȫ���ھ��
	::OpenClipboard(hWnd); // �򿪼�����
	::EmptyClipboard(); // ��ռ�����
	::SetClipboardData(CF_TEXT, hGlobalMemory); // ���ڴ��е����ݷ��õ�������
	::CloseClipboard(); // �رռ�����
    free(lpWcharBuf);
	AfxMessageBox(str);
}

void CPage1::OnLvnItemActivateList2(NMHDR *pNMHDR, LRESULT *pResult)//û�õ�
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    AfxMessageBox(L"??");
	*pResult = 0;
}

void CPage1::OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult)//��Ӧ��ģ�顢�߳���Ϣ
{
	
    ListProcessInfo();
	*pResult = 0;
}

void CPage1::OnMenuUnmapViewOfModule()
{
	// ģ��ж��
	ULONG    ret;int lpBaseAddr=0;
	WCHAR ErrCode[200];
	typedef ULONG (WINAPI *PFNNtUnmapViewOfSection)(  HANDLE ProcessHandle, PVOID BaseAddress );
	int CurSel=m_DllListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	CString str=m_DllListCtrl.GetItemText(CurSel,2); 

    CString CSBaseAddress=m_DllListCtrl.GetItemText(CurSel,0);
	CSBaseAddress=CSBaseAddress.Right(CSBaseAddress.GetLength()-2);

    CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // �õ�Pid

	WCHAR* wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);

	lpBaseAddr=CMySystem::HexToInt(CSBaseAddress);
	if (lpBaseAddr==0)
	{
		AfxMessageBox(L"ģ���ַ����");
		return;
	}
	HMODULE hModule = GetModuleHandle ( L"ntdll.dll" ) ;
	if ( hModule == NULL )
		hModule = LoadLibrary ( L"ntdll.dll" ) ;
    
	PFNNtUnmapViewOfSection pfnNtUnmapViewOfSection = (PFNNtUnmapViewOfSection)GetProcAddress ( hModule, "NtUnmapViewOfSection" ) ;

	HANDLE hProcess = OpenProcess ( PROCESS_ALL_ACCESS, TRUE, dwProcessId ) ;
	ret = pfnNtUnmapViewOfSection ( hProcess, (PVOID)lpBaseAddr ) ;

	CloseHandle ( hProcess ) ;
	if (ret)
	{
       wsprintf(ErrCode,L"ж��ģ����������룺%x",ret);
	   AfxMessageBox(ErrCode);
	}
	ListProcessInfo();

}
void CPage1::ListProDllByQueryVirMem(void)
{

//  ��Ӧ�о�ģ�鹦��
//  #define MemorySectionName 2
//  #define STR_LENGTH 512
//  #define	LC_ALL 0
	typedef  BOOL (WINAPI *PFNVerQueryValueW)( 
		const LPVOID pBlock,
		LPWSTR lpSubBlock,
		LPVOID * lplpBuffer,
		PUINT puLen
		);
	typedef  BOOL (WINAPI *PFNGetFileVersionInfoA)( 
		LPSTR lptstrFilename, 
		DWORD dwHandle,        
		DWORD dwLen,         
		LPVOID lpData
		);
	typedef  DWORD (WINAPI *PFNGetFileVersionInfoSizeA)( 
		LPSTR lptstrFilename, /* Filename of version stamped file */
		LPDWORD lpdwHandle
		);
	HMODULE hModule = LoadLibrary (L"version.dll" ) ;
	PFNVerQueryValueW VerQueryValueW = (PFNVerQueryValueW)GetProcAddress ( hModule, "VerQueryValueW" ) ;
	PFNGetFileVersionInfoA GetFileVersionInfoA = (PFNGetFileVersionInfoA)GetProcAddress ( hModule, "GetFileVersionInfoA" ) ;
	PFNGetFileVersionInfoSizeA GetFileVersionInfoSizeA = (PFNGetFileVersionInfoSizeA)GetProcAddress ( hModule, "GetFileVersionInfoSizeA" ) ;
	if (!VerQueryValueW||!GetFileVersionInfoA||!GetFileVersionInfoSizeA)
	{
		return ;
	}
	WCHAR*   lpBuffer;   
	WCHAR   szSubBlock[256];  
	INT   nVersionLen;
	struct   LANGANDCODEPAGE   {   
		WORD   wLanguage;   
		WORD   wCodePage;   
	}   *lpTranslate;  

	WCHAR*   pBuffer;
	UINT   cbTranslate;
	UINT dwBytes;

	char szModPath[MAX_PATH]={'\0'}; 
	WCHAR wcModPath[MAX_PATH]={'\0'}; 
	WCHAR PROID[MAX_PATH];
    HANDLE hFilesys=NULL;
	int retLength;
	CString str;//ģ���ַCString str
	PMEMORY_SECTION_NAME out_data=(PMEMORY_SECTION_NAME)	malloc(0x200u);
	int	dwProcessId=0;
	wchar_t wstr[256];
    m_DllListCtrl.DeleteAllItems();
	int CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // 0=<col<m_list������

	WCHAR* wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);
	hModule=0;
	hModule=(HMODULE)GetModuleHandleA("ntdll.dll");
	ZwQueryVirtualMemory_ ZwQueryVirtualMemory=(ZwQueryVirtualMemory_)GetProcAddress(hModule,"ZwQueryVirtualMemory");
	//HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);
	HANDLE hProcess= CMySystem::My_OpenProcess (
		PROCESS_ALL_ACCESS,
		0,
		dwProcessId,
		true,
		(LPDWORD)0
		);
	if (!hProcess)
	{
		
		return ;
	}
	//���
    int ii=0; 

	for (unsigned int i=0;i<0x7fffffff;i=i+0x10000)
	{
		ZwQueryVirtualMemory((DWORD)hProcess,(DWORD)i,MemorySectionName,(DWORD)out_data,STR_LENGTH,(DWORD)&retLength);
		if(!IsBadReadPtr((BYTE*)out_data->SectionFileName.Buffer,1))
			if(((BYTE*)out_data->SectionFileName.Buffer)[0]==0x5c)
			{
				if(wcscmp(wstr, out_data->SectionFileName.Buffer))
				{
					_wsetlocale(LC_ALL,L"chs");    
					//printf("0x%08x   %ws\n",i,out_data->SectionFileName.Buffer); 


					wsprintf(PROID,L"0x%x",i);

					m_DllListCtrl.InsertItem(ii,PROID,0); 
					wsprintf(PROID,L"%s",out_data->SectionFileName.Buffer);
					CMySystem::TrimPathW(PROID);
					m_DllListCtrl.SetItemText(ii,2,PROID);
					CMySystem::WCHARToChar(szModPath,out_data->SectionFileName.Buffer);  
					CMySystem::TrimPath(szModPath);
                    //--�õ�ģ��汾��Ϣ
					nVersionLen =   GetFileVersionInfoSizeA(   szModPath,   NULL   );   
					if   (   nVersionLen   <=   0   )   
					{   
						goto Here    ;   
					}   

					pBuffer =   new   WCHAR[   nVersionLen   ];  
					if   (   !GetFileVersionInfoA(   szModPath,   NULL,   nVersionLen,   pBuffer   )   )   
					{   
						goto Here   ;   
					}  
					VerQueryValueW(   pBuffer,   L"\\VarFileInfo\\Translation",   
						(LPVOID*)&lpTranslate,   &cbTranslate   );  
					wsprintf(   szSubBlock,L"\\StringFileInfo\\%04x%04x\\CompanyName",   
						lpTranslate[0].wLanguage,   lpTranslate[0].wCodePage   ); 
					if(   VerQueryValueW(   pBuffer,   szSubBlock,   (   LPVOID*   )&lpBuffer,   &dwBytes   )   )   
					{   
						m_DllListCtrl.SetItemText(ii,3,lpBuffer);  
					}  
					else{
						m_DllListCtrl.SetItemText(ii,3,L"�õ��ļ���Ϣʧ��");  
					}
					//--
Here:
                    DWORD qwFileSize;
					hFilesys = CreateFileA(szModPath, GENERIC_READ ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					DWORD dwFileSizeHigh;
					if (hFilesys!=INVALID_HANDLE_VALUE)
					{
						 qwFileSize = GetFileSize(hFilesys, &dwFileSizeHigh);
						 CloseHandle(hFilesys);
						 wsprintf(PROID,L"0x%x",qwFileSize);
					}
					else
					{
						wsprintf(PROID,L"��ȡģ���Сʧ��,ģ�����ڱ�ʹ��");
					}

	                m_DllListCtrl.SetItemText(ii,1,PROID); 
					ii++;

				}

				wcscpy_s(wstr,   out_data->SectionFileName.Buffer);

			}

	}
	CloseHandle(hProcess);
}

void CPage1::ListThread(void)
{
	THREAD_INFO Thread;
	int ThreadNum=0;
	void* ThrN;
	int dwProcessTid;
	CString str;


	WCHAR PROID[MAX_PATH];
	//printf("Thread\n");
	m_ThreaList.DeleteAllItems();
	int CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // 0=<col<m_list������
	WCHAR* wTem=str.GetBuffer(str.GetLength());
	dwProcessTid=_wtoi(wTem);
	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		AfxMessageBox(L"�����豸����ʧ�ܣ�");
		return ;
	}
	ThreadNum=CMySystem::GetThrNum(CMySystem::hMyHandle,dwProcessTid);
	//printf("ThreadNum:%d\n",ThreadNum);
    wsprintf(PROID,L"%d",ThreadNum);
	//AfxMessageBox(PROID);
	if (ThreadNum==0)
	{
		AfxMessageBox(L"�õ��߳�ʧ�ܣ�\n�������ܵõ������̵��߳�");
		return;
	}
	ThrN=(void*)malloc(ThreadNum*sizeof(THREAD_INFO));
	if (!ThrN)
	{
		CMySystem::ClearThr(CMySystem::hMyHandle);
	}
	CMySystem::GetThr( CMySystem::hMyHandle,ThrN,(2*ThreadNum)*sizeof(THREAD_INFO));
	for (int i=0;i<ThreadNum;i++)
	{
		Thread=*((PTHREAD_INFO)(ThrN)+i);
		//printf("%x,%x\n",Thread.dwThreadId,Thread.pEThread); 
		wsprintf(PROID,L"%d",Thread.dwThreadId);
		m_ThreaList.InsertItem(i,PROID,0); 
		wsprintf(PROID,L"0x%x",Thread.pEThread);
		m_ThreaList.SetItemText(i,1,PROID);
		wsprintf(PROID,L"0x%x",Thread.StartAddress);
		m_ThreaList.SetItemText(i,2,PROID); 

	}
}
void CPage1::ListProcessInfo(void)//��Ӧ��ģ�顢�߳���Ϣ
{


   ListProDllByQueryVirMem();
   ListThread();

}

void CPage1::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����Ҽ��˵�
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU2);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	}
	*pResult = 0;
}

void CPage1::OnMenuKillPro()//��ͨ���㷨ɱ����
{
	CString str;
	int CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // 0=<col<m_list������


	WCHAR* wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);
	CMySystem::KillProcess( dwProcessId);
	AfxMessageBox(L"����������ϣ������оٽ��̣�");

}

void CPage1::OnMenuFreeModuleByPid()//ж��ģ�飬��ͨ����
{
	CString CSPid,CSName;
	int CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	CSPid=m_ProListCtrl.GetItemText(CurSel,1); // 0=<col<m_list������
    WCHAR* wTem=CSPid.GetBuffer(CSPid.GetLength());
	DWORD dwPid=_wtoi(wTem);

    CurSel=m_DllListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	CSName=m_DllListCtrl.GetItemText(CurSel,2); // 0=<col<m_list������ 

	CSName=CSName.Right(CSName.GetLength()-CSName.ReverseFind( '\\' )-1);
	TCHAR * szModuleName = CSName.GetBuffer();//_T("plk.dll");
    //dwPid = 1780;
	CMySystem::FreeModuleByPid(szModuleName,dwPid);
	ListProDllByQueryVirMem();

}




void CPage1::OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU4);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	}
	*pResult = 0;
}

void CPage1::OnMenuForceKillThread()//������ǿ�ƽ����߳�
{
	if(IDNO==::MessageBox(0, L"�����������̣߳���һ��Σ�գ�ȷ��Ҫ���У�",L"����",MB_YESNO))
	{
		return;

	}
	CString str;
	int CurSel=0;
	WCHAR*wTem={'\0'};
	DWORD dwProcessId=0;
	DWORD  dwCid=0;
	CurSel=m_ThreaList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ThreaList.GetItemText(CurSel,0); // �߳�Cid
    wTem=str.GetBuffer(str.GetLength());
	dwCid=_wtoi(wTem);

	CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // ����Pid
	wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);

	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		AfxMessageBox(L"�����豸����ʧ�ܣ�");
		return ;
	}
	AfxMessageBox(L"�����߳���ϣ��������оٽ��̣�����Ҫ�ٴ��оٵ�ǰ���̵��̣߳����������������");
	CMySystem::ForceKillThread( CMySystem::hMyHandle, dwProcessId, dwCid);
	
}


void CPage1::OnMenuForceKillProcess()
{

  if(IDYES==::MessageBox(0, L"�����н������̣���һ��Σ�գ�ȷ��Ҫ���У�",L"����",MB_YESNO))
 { 
	
	CString str;
	int CurSel=0;
	WCHAR*wTem={'\0'};
	DWORD dwProcessId=0;
	CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // ����Pid
	wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);
	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		AfxMessageBox(L"�����豸����ʧ�ܣ�");
		return ;
	}
	CMySystem::ForceKillProcess( CMySystem::hMyHandle, dwProcessId);
	AfxMessageBox(L"����������ϣ��������оٽ��̣�");
 }

}

void CPage1::OnMenuKillProcessByTer()
{
	CString str;
	int CurSel=0;
	WCHAR*wTem={'\0'};
	DWORD dwProcessId=0;
	CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // ����Pid
	wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);
	HANDLE hProcess= CMySystem::My_OpenProcess (
		PROCESS_ALL_ACCESS,
		0,
		dwProcessId,
		true,
		(LPDWORD)0
		);
	AfxMessageBox(L"����������ϣ��������оٽ��̣�");
	TerminateProcess(hProcess,0);
	
}

void CPage1::OnBnClickedListProcess()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	typedef  BOOL (WINAPI *PFNVerQueryValueW)( 
		const LPVOID pBlock,
		LPWSTR lpSubBlock,
		LPVOID * lplpBuffer,
		PUINT puLen
		);
	typedef  BOOL (WINAPI *PFNGetFileVersionInfoW)( 
		LPWSTR lptstrFilename, 
		DWORD dwHandle,        
		DWORD dwLen,         
		LPVOID lpData
		);
	typedef  DWORD (WINAPI *PFNGetFileVersionInfoSizeW)( 
		LPWSTR lptstrFilename, /* Filename of version stamped file */
		LPDWORD lpdwHandle
		);
	HMODULE hModule = LoadLibrary (L"version.dll" ) ;
	PFNVerQueryValueW VerQueryValueW = (PFNVerQueryValueW)GetProcAddress ( hModule, "VerQueryValueW" ) ;
	PFNGetFileVersionInfoW GetFileVersionInfoW = (PFNGetFileVersionInfoW)GetProcAddress ( hModule, "GetFileVersionInfoW" ) ;
	PFNGetFileVersionInfoSizeW GetFileVersionInfoSizeW = (PFNGetFileVersionInfoSizeW)GetProcAddress ( hModule, "GetFileVersionInfoSizeW" ) ;
    if (!VerQueryValueW||!GetFileVersionInfoW||!GetFileVersionInfoSizeW)
	{
		return ;
	}
	WCHAR*   lpBuffer;   
	WCHAR   szSubBlock[256];  
	INT   nVersionLen;
	struct   LANGANDCODEPAGE   {   
		WORD   wLanguage;   
		WORD   wCodePage;   
	}   *lpTranslate;  

	WCHAR*   pBuffer;
	UINT   cbTranslate; 

	int ImageListCount=0;
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	HICON g_icon;
	SHFILEINFO sfi;
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	WCHAR PROID[MAX_PATH];
	int i=0;
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	DWORD_PTR SHret;
	WCHAR szPath[256]={'0'};
	HANDLE hSnapshotInner;
	HANDLE hMyPro;
	DWORD ret;
	UINT   dwBytes;   
//	

	int nLen =0;
	HICON hKenny = reinterpret_cast<HICON>(
		::LoadImage ( AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_BLANK),
		IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR ));

	//���
	m_ProListCtrl.DeleteAllItems();
    m_ThreaList.DeleteAllItems();

    m_DllListCtrl.DeleteAllItems();

	ImageListCount= m_ProImageList.GetImageCount();
	for(int i=0;i<ImageListCount;i++)   
	{   
		m_ProImageList.Remove(0);   

	}   
	for (BOOL bRet = Process32First(hSnapshot,&pInfo);bRet;bRet=Process32Next(hSnapshot,&pInfo),i++)
	{
		if (!StrCmpW (pInfo.szExeFile,L"[System Process]")||!StrCmpW (pInfo.szExeFile,L"System"))
		{
			m_ProImageList.Add ( hKenny );
		}
		else
		{
			hSnapshotInner = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pInfo.th32ProcessID);
			Module32First(hSnapshotInner,&mInfo);
			ZeroMemory(&sfi,sizeof(SHFILEINFO));
			SHret=SHGetFileInfoW(mInfo.szExePath,0,&sfi,sizeof(SHFILEINFO),SHGFI_ICON);//mInfo.szExePath��ȫ·��
				if (0==SHret||hSnapshotInner==(HANDLE)-1)
				{
					m_ProImageList.Add ( hKenny );
				}
				else
				{
					g_icon = sfi.hIcon;
					m_ProImageList.Add ( g_icon );
				}
		}
		//hMyPro=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,false,pInfo.th32ProcessID);
		hMyPro= CMySystem::My_OpenProcess (
			PROCESS_ALL_ACCESS,
			0,
			pInfo.th32ProcessID,
			true,
			(LPDWORD)0
			);
		ret=GetModuleFileNameEx(
			hMyPro,
			NULL,
			szPath,
			256
			);

		if (!ret)
		{
			StrCpyW(szPath,L"�õ�·��ʧ��");
		}


		m_ProListCtrl.InsertItem(i,pInfo.szExeFile,i); 

		ZeroMemory(&sfi,sizeof(MAX_PATH));
		wsprintf(PROID,L"%d",pInfo.th32ProcessID);
		m_ProListCtrl.SetItemText(i,1,PROID); 
		m_ProListCtrl.SetItemText(i,2,szPath);//�����һ�� 

  
		nVersionLen =   GetFileVersionInfoSizeW(   szPath,   NULL   );   
		if   (   nVersionLen   <=   0   )   
		{   
			continue   ;   
		}   

		pBuffer =   new   WCHAR[   nVersionLen   ];  
		if   (   !GetFileVersionInfo(   szPath,   NULL,   nVersionLen,   pBuffer   )   )   
		{   
			continue   ;   
		}  
		VerQueryValueW(   pBuffer,   L"\\VarFileInfo\\Translation",   
			(LPVOID*)&lpTranslate,   &cbTranslate   );  
		wsprintf(   szSubBlock,L"\\StringFileInfo\\%04x%04x\\CompanyName",   
			lpTranslate[0].wLanguage,   lpTranslate[0].wCodePage   ); 
		if(   VerQueryValueW(   pBuffer,   szSubBlock,   (   LPVOID*   )&lpBuffer,   &dwBytes   )   )   
		{   
			m_ProListCtrl.SetItemText(i,3,lpBuffer);  
		}  
		else{
            m_ProListCtrl.SetItemText(i,3,L"�õ��ļ���Ϣʧ��");  
		}

		CloseHandle(hMyPro);
	}
}

void CPage1::OnMenuListProcessInfo()
{
	ListProcessInfo();
}

void CPage1::OnMenuKillThreadByTerminateThread()
{

	CString str;
	int CurSel=0;
	WCHAR*wTem={'\0'};
	DWORD dwProcessId=0;
	DWORD  dwCid=0;
	CurSel=m_ThreaList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ThreaList.GetItemText(CurSel,0); // �߳�Cid
	wTem=str.GetBuffer(str.GetLength());
	dwCid=_wtoi(wTem);

	CurSel=m_ProListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_ProListCtrl.GetItemText(CurSel,1); // ����Pid
	wTem=str.GetBuffer(str.GetLength());
	dwProcessId=_wtoi(wTem);
	AfxMessageBox(L"�����߳���ϣ��������оٽ��̣�����Ҫ�ٴ��оٵ�ǰ���̵��̣߳����������������");
	CMySystem::KillThreadByTerminateThread( dwProcessId, dwCid);




}
