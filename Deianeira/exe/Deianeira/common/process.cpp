#include "process.h"


//��ȡ�̵߳�״̬
//ulPID:Ŀ�����ID
//ulTID:Ŀ���߳�ID
//����:0:��ʾ�����쳣,1:��ʾ���̴��ڹ���״̬,2:��ʾ����û�б�����
ULONG process_getThreadState(ULONG ulPID,ULONG ulTID)
{
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return 0;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return 0;
	}

	//ö�ٵõ����н���
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//���������б�
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			for(UINT32 i=0;i<p->ThreadCount;i++)
			{
				SYSTEM_THREADS systemThread=p->Threads[i];
				if(systemThread.ClientId.ulTID == ulTID) //�ҵ��߳�              
				{
					if(systemThread.dwState == StateWait && systemThread.dwWaitReason == Suspended) //�̱߳�����
					{
						delete[] sp; 
						FreeLibrary(hModule);
						return 1;
					}
					else
					{
						delete[] sp; 
						FreeLibrary(hModule);
						return 2;
					}
				}
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return 0;
}

//�ں˹����߳�
//hDriver:�������
//ulTID:Ŀ���߳�ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_SUSPEND,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ں˻ָ��߳�
//hDriver:�������
//ulTID:Ŀ���߳�ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_RESUME,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ں˽����߳�
//hDriver:�������
//ulTID:Ŀ���߳�ID
//����:true:��ʾɾ���ɹ�,false:��ʾɾ��ʧ��
bool process_killThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILLTHREAD,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//��ȡ���̵ĸ�����ID
//ulTID:Ŀ���߳�ID
//����:-1:ʧ�ܣ�����ɹ�
ULONG process_getProcessParentPID(ULONG ulPID)
{
	ULONG ulPPID = -1;
	PPROCESS_BASIC_INFORMATION pbi = new PROCESS_BASIC_INFORMATION();
	
	PNtQueryInformationProcess NtQueryInformationProcess;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return -1;
	}

	NtQueryInformationProcess=(PNtQueryInformationProcess)GetProcAddress(hModule, "NtQueryInformationProcess");
	if (NtQueryInformationProcess==NULL)
	{
		FreeLibrary(hModule);
		return -1;
	}

	//�򿪽��̻�ý��̾��
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,ulPID);

	//����򿪽���ʧ��
	if (hProcess==NULL)
	{
		return -1;
	}

	//��ѯ������Ϣ
	NTSTATUS status = NtQueryInformationProcess(hProcess,ProcessBasicInformation,
		pbi,sizeof(PROCESS_BASIC_INFORMATION),NULL);

	//�����ý�����Ϣ�ɹ�
	if (NT_SUCCESS(status))
	{
		//������ID
		ulPPID = pbi->InheritedFromUniqueProcessId;
	}

	//�ͷŽ��̾��
	CloseHandle (hProcess);

	delete[] pbi;

	//���ظ�����ID
	return ulPPID;
}

//��ȡ���̵�״̬
//ulPID:Ŀ�����ID
//����:0:��ʾ�����쳣,1:��ʾ���̴��ڹ���״̬,2:��ʾ����û�б�����
ULONG process_getProcessState(ULONG ulPID)
{
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return 0;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return 0;
	}

	//ö�ٵõ����н���
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//���������б�
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason==5)
			{
				delete[] sp; 
				FreeLibrary(hModule);
				//���̴��ڹ���״̬
				return 1;
			}
			else
			{
				delete[] sp; 
				FreeLibrary(hModule);
				//����û�б�����
				return 2;
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return 0;
}

//��������
//ulPID:Ŀ�����ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendProcess(ULONG ulPID)
{
	bool bRet = false;

	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return false;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return false;
	}

	//ö�ٵõ����н���
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//���������б�
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason!=5)
			{
				HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,systemThread.ClientId.ulTID); 
				if(!SuspendThread(hThread))
				{
					bRet = true;
				}
				else
				{
					bRet = false;
				}
				CloseHandle(hThread); 
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return bRet;
}

//�ں���������
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_SUSPEND,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ָ�����
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeProcess(ULONG ulPID)
{
	bool bRet = false;

	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return false;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return false;
	}

	//ö�ٵõ����н���
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//���������б�
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason==5)
			{
				//��������н��̽����Ĵ���
				HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,systemThread.ClientId.ulTID); 
				if(ResumeThread(hThread))
				{
					//�ָ��ɹ�
					bRet = true;
				}
				else
				{
					//�ָ�ʧ��
					bRet = false;
				}
				CloseHandle(hThread);
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return bRet;
}

//�ں˻ָ�����
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_RESUME,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ں˽�������
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_killProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILL2,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ں˽�������,�ڴ�����
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_killProcess_memclr(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILL,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//�ں˽���ģ��ж��
//hDriver:�������
//ulPID:Ŀ�����ID
//ulDllBase:DLLģ���ַ
//����:true:��ʾж�سɹ�,false:��ʾж��ʧ��
bool process_unloadModule(HANDLE hDriver,ULONG ulPID,ULONG ulDllBase){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	UNLOADDLL_CLIENT_ID clientID;
	clientID.ulPID = ulPID;
	clientID.ulBaseAddress = ulDllBase;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_MODULE_UNLOAD,&clientID,sizeof(UNLOADDLL_CLIENT_ID),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//ע��DLL
//lpFilePath:DLL�ļ�·��
//ulPID:����ID
//����:true:ע��ɹ�,false:ע��ʧ��
bool process_injectDll(LPTSTR lpFilePath,ULONG ulPID)
{
	//��ý���Ȩ��
	HANDLE  hProcess = OpenProcess(PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, ulPID); 

	//����򿪽��̳ɹ�
	if(hProcess)
	{
		DWORD dwSize,dwWritten;
		//����·����С
		dwSize = sizeof(WCHAR)*lstrlenW(lpFilePath)+1;

		//��Ŀ����̿ռ��ڣ�����ռ����ڴ��DLL�ļ�·��
		LPVOID lpBuffer=VirtualAllocEx(hProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);

		//�������ռ�ʧ��
		if(lpBuffer==NULL)
		{
			CloseHandle(hProcess);
			return false;
		}

		//���շ���Ŀռ�д��DLL�ļ�·��
		if(WriteProcessMemory(hProcess,lpBuffer,lpFilePath,dwSize,&dwWritten))
		{
			//���ʵ��д������ݲ�����DLL·����С
			if(dwWritten!=dwSize)
			{
				//�ͷŸղŷ���Ŀռ�
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;
			} 

			//���kernel��̬���ӿ���
			HMODULE hModule=GetModuleHandle(L"Kernel32.dll");

			//������ʧ��
			if(hModule==NULL)
			{
				//�ͷŸղŷ���Ŀռ�
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;
			}

			//���ģ���к����ĵ�ַ
			LPTHREAD_START_ROUTINE pFunc=(LPTHREAD_START_ROUTINE)GetProcAddress(hModule,"LoadLibraryW");

			//��ȡ������ַʧ��
			if(pFunc==NULL)
			{
				//�ͷŸղŷ���Ŀռ�
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;           
			}

			//��Ŀ����̿ռ��ڴ����߳�
			HANDLE hThread=CreateRemoteThread(hProcess,NULL,0,pFunc,lpBuffer,0,NULL);
			//���޵ȴ�Ŀ���̵߳Ľ���
			WaitForSingleObject(hThread,INFINITE);
			//�ͷ��߳̾��
			CloseHandle(hThread);

			//�ͷŸղŷ���Ŀռ�
			VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
			CloseHandle(hProcess);

			return true;
		}

		return false;
	}

	return false;
}

//��ý��̵�ģ����Ϣ�б�
//����:true:�ɹ�,false:ʧ��
bool process_getModuleInfoTable()
{
	PPROCESS_MODULE_INFO p;
	MODULEENTRY32	me32;
	me32.dwSize	= sizeof(MODULEENTRY32);

	//��ʼ��
	pProcessModuleInfoHead = pProcessModuleInfo = new PROCESS_MODULE_INFO;

	//
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if (!hModuleSnap)
	{
		return false;
	}

	//
	BOOL bRet = Module32First(hModuleSnap, &me32);

	while(bRet)
	{
		p = new PROCESS_MODULE_INFO;
		wcscpy_s(p->FileName,me32.szModule);
		wcscpy_s(p->FilePath,me32.szExePath);

		p->next = NULL;

		p->ulBaseAddress = (DWORD)me32.modBaseAddr;
		p->ulSize = (DWORD)(me32.modBaseSize);

		pProcessModuleInfo->next = p;
		pProcessModuleInfo = p;	

		pProcessModuleInfo = pProcessModuleInfoHead->next;

		//
		bRet = Module32Next(hModuleSnap, &me32);
	}

	CloseHandle(hModuleSnap);
	return true;
}

//��ý����߳��б�
PSYSTEM_PROCESSES GetThreadList(ULONG ulPID) 
{ 
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return NULL;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return NULL;
	}

	//ö�ٵõ����н���
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//���������б�
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID) 
		{            
			FreeLibrary(hModule);
			//�����б�
			return p;
		} 
		done = p->NextEntryDelta == 0; 
	}

	return NULL;
}