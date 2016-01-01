#include "common.h"

//
char* common_TCHARToCHAR(TCHAR* pwstr)
{
	//
	int len = 2 * wcslen(pwstr);
	char* pstr_ = new char[len + 1];
	//
	wcstombs(pstr_,pwstr,len + 1);
	return pstr_;
}

//
TCHAR* common_CHARToTCHAR(char* pstr_)
{
	//
	int len = strlen(pstr_);
	wchar_t* pwstr_ = new wchar_t[len + 1];
	//
	mbstowcs(pwstr_,pstr_,len + 1);
	return pwstr_;
}

//UTF-8��GBK��ת��
char* common_UTFToGBK(const char* pstr_)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, pstr_, -1, NULL, 0);
	wchar_t* pwstr = new wchar_t[len + 1];
	ZeroMemory(pwstr,len + 1);

	MultiByteToWideChar(CP_UTF8, 0, pstr_, -1, pwstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, pwstr, -1, NULL, 0, NULL, NULL);
	char* pstr = new char[len + 1];
	ZeroMemory(pstr,len + 1);

	WideCharToMultiByte(CP_ACP, 0, pwstr, -1, pstr, len, NULL, NULL);

	if(pwstr) delete[] pwstr;

	return pstr;
}

//GBK��UTF-8��ת��
char* common_GBKToUTF(const char* pstr_)
{
	int len = MultiByteToWideChar(CP_ACP, 0, pstr_, -1, NULL, 0);
	wchar_t* pwstr = new wchar_t[len + 1];
	ZeroMemory(pwstr,len + 1);

	MultiByteToWideChar(CP_ACP, 0, pstr_, -1, pwstr, len);

	len = WideCharToMultiByte(CP_UTF8, 0, pwstr, -1, NULL, 0, NULL, NULL);
	char* pstr = new char[len + 1];
	ZeroMemory(pstr,len + 1);

	WideCharToMultiByte(CP_UTF8, 0, pwstr, -1, pstr, len, NULL, NULL);

	if(pwstr) delete[] pwstr;

	return pstr;
}

//�ػ�����
//bReboot:��־�ǹػ���������
//����:true:�ɹ���false:ʧ��
bool common_shutdown(bool bReboot)
{
	HMODULE hNtdll;
	NTSTATUS status;

	PNtSetSystemPowerState NtSetSystemPowerState = NULL;
	PNtShutdownSystem NtShutdownSystem = NULL;

	hNtdll = LoadLibrary(L"ntdll.dll");

	//�������ʧ��
	if (!hNtdll)
	{
		return false;
	}

	//���������������
	if (bReboot)
	{
		NtShutdownSystem = (PNtShutdownSystem)GetProcAddress(hNtdll, "NtShutdownSystem");

		//�����ַ���ʧ��
		if (!NtShutdownSystem) 
		{
			return false;
		}

		status = NtShutdownSystem(ShutdownReboot);

		if (!NT_SUCCESS(status))
		{
			return false;
		}

		return true;
	}
	else
	{
		//��ȡ��ַ
		NtSetSystemPowerState = (PNtSetSystemPowerState)GetProcAddress(hNtdll, "NtSetSystemPowerState");

		//�����ַ���ʧ��
		if (NtSetSystemPowerState==NULL) 
		{
			return false;
		}

		//���õ�Դ״̬
		status = NtSetSystemPowerState((POWER_ACTION)PowerSystemShutdown,(SYSTEM_POWER_STATE)PowerActionShutdown,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);

		if (!NT_SUCCESS(status))
		{
			return false;
		}

		return true;
	}
}

//��������Ȩ��
//hToken:��������
//lpszPrivilege:Ȩ�ޱ���
//bEnablePrivilege:ʹ��
//����:true:�ɹ���false:ʧ��
bool common_setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
	TOKEN_PRIVILEGES tp;

	tp.PrivilegeCount = 1;

	if (!LookupPrivilegeValue( 
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&tp.Privileges[0].Luid ))        // receives LUID of privilege
	{
		return false; 
	}

	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		NULL, 
		NULL))
	{ 
		return false; 
	} 

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return false;
	} 

	return true;
}

//��������Ȩ��
//lpszPrivilege:Ȩ�ޱ���
//bEnablePrivilege:ʹ��
//����:true:�ɹ���false:ʧ��
bool common_setPrivilege(ULONG ulPID, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
	HANDLE hProcess;
	HANDLE hToken;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ulPID);

	//����򿪽���ʧ��
	if (!hProcess)
	{
		return false;
	}

	//��������Ȩ��
	if(!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken))
	{
		return false;
	}

	if (!common_setPrivilege(hToken,lpszPrivilege,bEnablePrivilege))
	{
		return false;
	}		

	return true;
}

//��������
//lpFileName:�����ļ�·��
//lpLinkName:��������
//����:�ɹ��򷵻��������,ʧ���򷵻�NULL
HANDLE common_loadDriver(LPCTSTR lpFileName,LPTSTR lpLinkName){
	HANDLE hDriver;

	// ��SCM������
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(hSCM == NULL)
	{
		return NULL;
	}

	// ������򿪷���
	SC_HANDLE hService = CreateService(hSCM, lpLinkName, lpLinkName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, lpFileName, NULL, 0, NULL, NULL, NULL);

	if(hService == NULL)
	{
		int nError = GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			hService = OpenService(hSCM, lpLinkName, SERVICE_ALL_ACCESS);
		}
	}

	if(hService == NULL)
	{
		return NULL;
	}

	// ��������
	// �������DriverEntry����
	if(!StartService(hService, 0, NULL))  
	{
		int nError = GetLastError();
		if(nError != ERROR_SERVICE_ALREADY_RUNNING)
		{	
			return NULL;
		}
	}

	// �򿪵����������������豸�ľ��
	TCHAR sz[256];
	wsprintf(sz, L"\\\\.\\%s", lpLinkName);

	//���豸
	hDriver = CreateFile(sz, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!hDriver)
	{	
		return NULL;
	}

	return hDriver;
}

//ж������
//hDriver:����ģ����
//lpLinkName:��������
//����:true:�ɹ���false:ʧ��
bool common_unloadDriver(HANDLE hDriver,LPTSTR lpLinkName){
	SERVICE_STATUS ss;

	//�����ͷ��������
	CloseHandle(hDriver);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (!hSCManager)
	{
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, lpLinkName, DELETE | SERVICE_STOP);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);
		return false;
	}

	ControlService(hService, SERVICE_CONTROL_STOP, &ss);
	DeleteService(hService );

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return true;
}

//���ļ����ԶԻ���
//lpFilePath:Ŀ���ļ�·��
//����:��
void common_showFileProperty(LPTSTR lpFilePath)
{
	SHELLEXECUTEINFO sei;
	sei.fMask=SEE_MASK_INVOKEIDLIST|SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI; 
	sei.hwnd=NULL; 
	sei.hInstApp=0; 
	sei.lpVerb=L"properties"; 
	sei.lpFile=lpFilePath; 
	sei.lpParameters=NULL; 
	sei.lpDirectory=NULL; 
	sei.nShow=1; 
	sei.lpIDList=0; 
	sei.cbSize=sizeof(sei); 
	ShellExecuteEx(&sei);
} 

//��λ�ļ�
//lpFilePath:Ŀ���ļ�·��
//����:��
void common_explorerFile(LPSTR lpFilePath)
{
	//��ֹ�ļ�·�����������޷�����
	CHAR buffer[512]={"explorer.exe   /select,"};
	strcat_s(buffer,lpFilePath);
	WinExec(buffer,SW_SHOW); 
} 

//��λע���
//lpRegPath:ע���·��
//����:true:�ɹ���false:ʧ��
bool common_explorerReg(LPTSTR lpRegPath)
{
	HKEY hKey;

	//�����ע���
	if (RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit",&hKey))
	{
		return false;
	}

	//д�����һ�δ򿪵�λ��
	if (RegSetValueEx(hKey,L"lastkey",0,REG_SZ,(LPBYTE)lpRegPath,MAX_PATH))
	{
		return false;
	}

	//���д��ɹ�����ر�
	RegCloseKey(hKey);

    return true;
} 

//������ԴID�������ļ� 
//resId:��ԴID 
//lpResType:��Դ����
//lpFileName:Ŀ���ļ�·�� 
//����:true:�ɹ���false:ʧ��
bool common_createFileFromResource(int resId,LPTSTR lpResType,LPTSTR lpFileName) 
{ 
	//����ָ��ID���͵���Դ
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(resId),lpResType); 

    //���û�в��ҵ������Դ
	if (!hRsrc) 
	{ 
		return false; 
	} 

	//�õ���Դ�Ĵ�С
	DWORD dwSize = SizeofResource(NULL, hRsrc);  
	//�����ȡ��Դ��Сʧ��
	if (dwSize == 0) 
	{ 
		return false; 
	} 

	//װ����Դ
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
    //���װ����Դʧ��
	if (!hGlobal) 
	{ 
		return false; 
	} 

    //������Դ
	LPVOID lpBuffer = LockResource(hGlobal); 
	//���������Դʧ��
	if (!lpBuffer) 
	{ 
		//�ͷż��ص���Դ
		FreeResource(hGlobal); 
		return false; 
	} 

	//�����ļ�
	FILE* lpFile;
	_wfopen_s(&lpFile,lpFileName,L"wb"); 

	if (!lpFile)
	{
		//�ͷż��ص���Դ
		FreeResource(hGlobal); 
		return false;
	}

    //��������ļ��ɹ�
	//����Դд���ļ�
	if (dwSize == fwrite(lpBuffer, sizeof(byte), dwSize, lpFile)) 
	{
		fclose(lpFile); //�ͷ��ļ���� 
	}

	//�ͷż��ص���Դ
	FreeResource(hGlobal); 
	return true;
}


//��ӷ���
//lpName:��������
//lpDisplayName:�Ѻ���ʾ����
//ulStartMode:��������
//lpPath:Ӧ�ó���·��
//����:true:�ɹ���false:ʧ��
bool common_addService(LPTSTR lpName,LPTSTR lpDisplayName,ULONG ulStartMode,LPTSTR lpPath)
{
	SC_HANDLE hManager;
	SC_HANDLE hService;

	if((hManager=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		//����򿪷�����ƹ�����ʧ��
		return false;
	}

	//��������
	hService=CreateService(
		hManager,lpName,lpDisplayName, 
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, 
		ulStartMode, SERVICE_ERROR_NORMAL,
		lpPath, 0, 0, 0, 0, 0 );

	//�����������ʧ��
	if(!hService)
	{
		CloseServiceHandle(hManager);
		return false;
	}

	//����ɹ�
	CloseServiceHandle(hService);
	CloseServiceHandle(hManager);

	return true;
}

//ɾ������
//lpName:��������
//����:true:�ɹ���false:ʧ��
bool common_delService(LPTSTR lpName)
{
	SC_HANDLE hManager;
	SC_HANDLE hService;
	SERVICE_STATUS status;

	if((hManager=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		//����򿪷�����ƹ�����ʧ��
		return false;
	}

	//���Ѵ��ڷ���
	hService=OpenService(hManager,lpName,SERVICE_ALL_ACCESS|DELETE);

	//�����ʧ��
	if (!hService)
	{
		CloseServiceHandle(hManager);
		return false;
	}

	//��ѯ����״̬
	BOOL blResult=QueryServiceStatus(hService,&status);

	//���ʧ��
	if (!blResult)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hManager);
		return false;
	}

	//�жϷ���״̬
	if (status.dwCurrentState!=SERVICE_STOPPED)
	{
		//�������û�б����ɾ��,�ͷ��ַ�����ƴ���
		blResult=ControlService(hService,SERVICE_CONTROL_STOP,&status);
		//���͹��ܴ���ʧ��
		if (!blResult)
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hManager);
			return false;
		}
	}
	
	//����ֹͣ��ɾ���˷���
	blResult=DeleteService(hService);

	//ɾ������ʧ��
	if (!blResult)
	{
		CloseServiceHandle(hService );
		CloseServiceHandle(hManager);
		return false;
	}

	CloseServiceHandle(hService );
	CloseServiceHandle(hManager);

	return true;
}

//���ô���
//lpIP:IP��ַ
//lpPort:�˿�
//����:true:�ɹ���false:ʧ��
bool common_setProxy(LPTSTR lpIP,LPTSTR lpPort)
{		 
	INTERNET_PER_CONN_OPTION_LIST list;
	BOOL    bRet;

	//���ṹ��
	list.dwSize = sizeof(list);
	// NULL����Lan
	list.pszConnection = NULL;
	// ��������ѡ��
	list.dwOptionCount = 3;
	list.pOptions = new INTERNET_PER_CONN_OPTION[3];

	// ȷ���ڴ汻����
	if(!list.pOptions)
	{
		return false;
	}

	//���ñ�־
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_PROXY;
	//���ô�������
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	TCHAR proxyAddress[MAX_PATH];
	//����
	memset(proxyAddress,0,MAX_PATH);
	//�����ַ���
	lstrcat(proxyAddress,L"http://");
	lstrcat(proxyAddress,lpIP);
	lstrcat(proxyAddress,L":");
	lstrcat(proxyAddress,lpPort);

	//���������ַ���
	list.pOptions[1].Value.pszValue =proxyAddress;
	//��д����
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = L"local";

	//����ѡ��
	bRet = InternetSetOption(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, sizeof(list));

	//�ͷ��ڴ�
	delete [] list.pOptions;

	if (!bRet)
	{
		return false;
	}

	return true;		
}

//ȡ������
//����:true:�ɹ���false:ʧ��
bool common_delProxy()
{
	HKEY hKey;
	DWORD dwValue=0;

	//��ע���
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_ALL_ACCESS, &hKey))
	{
		return false;
	}		 

	//����ע����ֵ
	if (RegSetValueEx(hKey, L"ProxyEnable", 0, REG_DWORD,(LPBYTE)&dwValue, sizeof(DWORD)))
	{
		RegCloseKey(hKey);
		return false;
	}		 

	RegCloseKey(hKey);

	//����֪ͨ
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);    
	InternetSetOption(NULL ,INTERNET_OPTION_REFRESH , NULL, 0);

	return true;
}

//�ж��ļ��Ƿ���PE�ļ�
//lpFilePath:Ŀ���ļ�
//����:true:�ǣ�false:����
bool common_isPEFile(LPTSTR lpFilePath){
	//��Ŀ���ļ�
	HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

	//��ʧ��
	if (!hFile)
	{
		return false;
	}

	//�ļ�ӳ��
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

	//ӳ��ʧ��
	if (!hMap)
	{			
		CloseHandle(hFile);
		return false;
	}

	//�ļ����ڴ��ӳ��
	PVOID pBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

	//ʧ��
	if (!pBase)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		return false;
	}

	//������PE�ṹ��
	//���DOSͷ��
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)pBase;

	//������ЧPE��־
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		UnmapViewOfFile(pBase);
		return false;
	}

	//NTͷ��
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//������ЧPE��־
	if (pImage_nt_header->Signature != IMAGE_NT_SIGNATURE)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		UnmapViewOfFile(pBase);
		return false;
	}

	CloseHandle(hMap);
	CloseHandle(hFile);
	UnmapViewOfFile(pBase);

	return true;
}

//��ÿ�ݷ�ʽ�ļ���ָ���·��
//lpwsLinkName:lnk�ļ���·��
//lpwsLinkPath:���ڴ����ָ����·���Ļ�����
//����:HRESULT
HRESULT common_getLnkPath(IN LPWSTR lpwsLinkName,OUT LPWSTR lpwsLinkPath)
{
	HRESULT hResult;
	IShellLink *pIShellLink;
	WIN32_FIND_DATA wfd;

	//��ʼ��
	CoInitialize(NULL);
	//����ʵ��
	hResult = CoCreateInstance((REFIID)CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,(REFIID)IID_IShellLink,(LPVOID *)&pIShellLink);

	//����ɹ�
	if (SUCCEEDED(hResult))
	{
		IPersistFile *pIPersistFile;
		//��ѯ�����Ϣ
		hResult = pIShellLink->QueryInterface((REFIID)IID_IPersistFile,(LPVOID *)&pIPersistFile);

		//�����ѯ�ɹ�
		if (SUCCEEDED(hResult))
		{
			//���ؿ�ݷ�ʽ�ļ�
			hResult = pIPersistFile->Load(lpwsLinkName, STGM_READ);

			//����ɹ�
			if (SUCCEEDED(hResult))
			{
				//����
				hResult = pIShellLink->Resolve(NULL,SLR_ANY_MATCH | SLR_NO_UI);

				if (SUCCEEDED(hResult))
				{
					//��ÿ�ݷ�ʽ���ƶ���·��
					hResult = pIShellLink->GetPath(lpwsLinkPath,MAX_PATH,&wfd,SLGP_SHORTPATH);
				}

			}

			pIPersistFile->Release();
		}

		pIShellLink->Release();
	}

	return hResult;
}

//����IP��ַ�ַ���
PCHAR GetIP(unsigned int ipaddr)
{
	static char pIP[20];
	unsigned int nipaddr = htonl(ipaddr);
	sprintf_s(pIP, "%d.%d.%d.%d",
		(nipaddr>>24) &0xFF,
		(nipaddr>>16) &0xFF,
		(nipaddr>>8) &0xFF,
		(nipaddr)&0xFF);
	return pIP;
}
