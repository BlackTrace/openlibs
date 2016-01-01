#include "protection.h"


//���ڱ�����һ��NtUserGetForegroundWindow�Ľ��
HWND hWndLastForeground;

PVOID pNtOpenProcess;
PVOID pNtOpenThread;


//NtOpenThread
NTSTATUS NewObOpenObjectByPointer_forNtOpenThread(IN PVOID  Object,IN ULONG  HandleAttributes,
												  IN PACCESS_STATE  PassedAccessState  OPTIONAL,
												  IN ACCESS_MASK  DesiredAccess  OPTIONAL,
												  IN POBJECT_TYPE  ObjectType  OPTIONAL,
												  IN KPROCESSOR_MODE  AccessMode,OUT PHANDLE  Handle)
{
	//��������Լ������Լ��Ļ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		//����򿪵�Ŀ���߳����ڵĽ�����r3Ӧ�ó���Ļ�
		if (IoThreadToProcess(Object) == pEProcess)
		{
			return STATUS_ACCESS_DENIED;
		}
	}

	return ObOpenObjectByPointer(Object,
		HandleAttributes,
		PassedAccessState,
		DesiredAccess,
		ObjectType,
		AccessMode,
		Handle
		);
}

//NtOpenProcess
NTSTATUS NewObOpenObjectByPointer_forNtOpenProcess(IN PVOID  Object,IN ULONG  HandleAttributes,
												   IN PACCESS_STATE  PassedAccessState  OPTIONAL,
												   IN ACCESS_MASK  DesiredAccess  OPTIONAL,
												   IN POBJECT_TYPE  ObjectType  OPTIONAL,
												   IN KPROCESSOR_MODE  AccessMode,OUT PHANDLE  Handle
												   )
{
	//��������Լ������Լ��Ļ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		//����򿪵�Ŀ�������r3Ӧ�ó���Ļ�
		if (Object == pEProcess)
		{
			return STATUS_ACCESS_DENIED;
		}
	}

	return ObOpenObjectByPointer(Object,
		HandleAttributes,
		PassedAccessState,
		DesiredAccess,
		ObjectType,
		AccessMode,
		Handle
		);
}


//shadow ssdt hook
NTSTATUS NewNtUserBuildHwndList(
								IN HDESK hdesk,
								IN HWND hwndNext,
								IN BOOL fEnumChildren,
								IN DWORD idThread,
								IN UINT cHwndMax,
								OUT HWND *phwndFirst,
								OUT PUINT pcHwndNeeded)
{
	NTSTATUS status;

	if (PsGetCurrentProcessId()!= ulProcessID)
	{
		ULONG ProcessID;

		if (!fEnumChildren)
		{
			//�õ�����ID
			ProcessID = OldNtUserQueryWindow(hwndNext, 0);

			//������ڱ���Ӧ�ó��򣬷���ʧ��
			if (ProcessID == ulProcessID)
			{
				return STATUS_UNSUCCESSFUL;
			}
		}

		status = OldNtUserBuildHwndList(hdesk,hwndNext,fEnumChildren,idThread,cHwndMax,phwndFirst,pcHwndNeeded);

		if (NT_SUCCESS(status))
		{
			ULONG i=0;
			ULONG j;

			while (i < *pcHwndNeeded)
			{
				//���Ҷ�Ӧ����Ľ���ID
				ProcessID=OldNtUserQueryWindow(phwndFirst[i],0);

				//�����Ҫ�����Ľ���ID
				if (ProcessID == ulProcessID)
				{
					//���б�����ȥ��
					for (j = i; j < (*pcHwndNeeded) - 1; j++)
					{
						phwndFirst[j] = phwndFirst[j+1]; 
					}

					//���һ����ֵΪ0��Ϊ�˱���͵����ڶ����ظ�
					phwndFirst[*pcHwndNeeded-1] = 0; 
					
					//��Ӧ�ļ�1
					(*pcHwndNeeded)--;

					//���������������壬��Ϊһ��������ܲ�ֹһ������
					continue; 
				}

				i++;
			}
		}

		return status;
	}

	return OldNtUserBuildHwndList(hdesk,hwndNext,fEnumChildren,idThread,cHwndMax,phwndFirst,pcHwndNeeded);
}

BOOL NewNtUserDestroyWindow(
							IN HWND hwnd)
{
	//��������Լ������Լ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//��ѯָ������Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//���ָ������Ľ���ID��Ҫ�����Ľ���ID
		if (ulPID==ulProcessID)
		{
			return FALSE;
		}
	}

	//������Լ������Լ��Ļ���ֱ�ӷ��ؽ��
	return OldNtUserDestroyWindow(hwnd);
}


HWND NewNtUserFindWindowEx(
							  IN HWND hwndParent,
							  IN HWND hwndChild,
							  IN PUNICODE_STRING pstrClassName OPTIONAL,
							  IN PUNICODE_STRING pstrWindowName OPTIONAL,
							  IN DWORD dwType)
{
	HWND hWnd;

	hWnd = OldNtUserFindWindowEx(hwndParent, hwndChild, pstrClassName, pstrWindowName, dwType);

	//��������Լ������Լ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
        //��ѯָ������Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hWnd, 0);

		//���ָ������Ľ���ID��Ҫ�����Ľ���ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//������Լ������Լ��Ļ���ֱ�ӷ��ؽ��
	return hWnd;;
}

HWND NewNtUserGetForegroundWindow(VOID)
{
	HWND hWnd;

	hWnd = OldNtUserGetForegroundWindow();   

	//��������Լ������Լ�
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		ULONG ulPID;
		//��ѯָ������Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hWnd, 0);

		if (ulPID == ulProcessID)
		{
			hWnd = hWndLastForeground;
		}
		else
		{
			hWndLastForeground = hWnd;
		}		
	}   

	//������Լ������Լ��Ļ���ֱ�ӷ��ؽ��
	return hWnd;
}

LRESULT NewNtUserMessageCall(
							 IN HWND hwnd,
							 IN UINT msg,
							 IN WPARAM wParam,
							 IN LPARAM lParam,
							 IN ULONG_PTR xParam,
							 IN DWORD xpfnProc,
							 IN BOOL bAnsi)
{
	ULONG ulPID;

	//��������Լ������Լ�
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//���Ŀ�����Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd,0);

		//�����Ҫ�����Ľ���ID
		if (ulPID == ulProcessID)
		{
			return FALSE;
		}
	}

	//���ǵĻ�
	return OldNtUserMessageCall(hwnd,msg,wParam,lParam,xParam,xpfnProc,bAnsi);
}

BOOL NewNtUserPostMessage(
						  IN HWND hwnd,
						  IN UINT msg,
						  IN WPARAM wParam,
						  IN LPARAM lParam
						  )
{
	ULONG ulPID;

	//��������Լ������Լ�
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//���Ŀ�����Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd,0);

		//�����Ҫ�����Ľ���ID
		if (ulPID == ulProcessID)
		{
			return FALSE;
		}
	}

	//���ǵĻ�
	return OldNtUserPostMessage(hwnd,msg,wParam,lParam);
}

BOOL NewNtUserPostThreadMessage(
								IN DWORD id,
								IN UINT msg,
								IN WPARAM wParam,
								IN LPARAM lParam)
{
	PETHREAD pEThread;
	NTSTATUS status;

	//��������Լ������Լ�
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//PETHREAD
		status = PsLookupThreadByThreadId((HANDLE)id,&pEThread);

		if (NT_SUCCESS(status))
		{
			if (IoThreadToProcess(pEThread) == pEProcess)
			{
				return FALSE;
			}
			else
			{
				return OldNtUserPostThreadMessage(id,msg,wParam,lParam);
			}
		}
	}
	
	return OldNtUserPostThreadMessage(id,msg,wParam,lParam);;
}

HANDLE NewNtUserQueryWindow(
							 IN HWND WindowHandle,
							 IN ULONG TypeInformation)
{
	ULONG ulPID;

	//��������Լ������Լ�
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//���Ŀ�����Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(WindowHandle,0);

		//�����Ҫ�����Ľ���ID
		if (ulPID == ulProcessID)
		{
			return 0;
		}
	}

	//���ǵĻ�
	return OldNtUserQueryWindow(WindowHandle,TypeInformation);
}

LONG NewNtUserSetWindowLong(
							IN HWND hwnd,
							IN int nIndex,
							IN LONG dwNewLong,
							IN BOOL bAnsi)
{
	//��������Լ������Լ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//��ѯָ������Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//���ָ������Ľ���ID��Ҫ�����Ľ���ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//������Լ������Լ��Ļ���ֱ�ӷ��ؽ��
	return OldNtUserSetWindowLong(hwnd,nIndex,dwNewLong,bAnsi);
}

BOOL NewNtUserShowWindow(
						 IN HWND hwnd,
						 IN int nCmdShow)
{
	//��������Լ������Լ�
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//��ѯָ������Ľ���ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//���ָ������Ľ���ID��Ҫ�����Ľ���ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//������Լ������Լ��Ļ���ֱ�ӷ��ؽ��
	return OldNtUserShowWindow(hwnd,nCmdShow);
}

HWND NewNtUserWindowFromPoint(LONG x, LONG y)
{
	return 0;
}


//
VOID protection_startInlineHook()
{

	//����ԭ�е�ַ
	pNtOpenProcess = GetExportedFuncAddress(L"NtOpenProcess");

	//�����õ�ַʧ��
	if (pNtOpenProcess==NULL)
	{
		DbgPrint("protection_startInlineHook pNtOpenProcess error!");
		return;
	}

	pNtOpenThread = GetExportedFuncAddress(L"NtOpenThread");

	//�����õ�ַʧ��
	if (pNtOpenThread==NULL)
	{
		DbgPrint("protection_startInlineHook pNtOpenThread error!");
		return;
	}

	//inline hook
	common_callAddrHook(pNtOpenProcess,ObOpenObjectByPointer,NewObOpenObjectByPointer_forNtOpenProcess,PAGE_SIZE);
	common_callAddrHook(pNtOpenThread,ObOpenObjectByPointer,NewObOpenObjectByPointer_forNtOpenThread,PAGE_SIZE);
}

//
VOID protection_stopInlineHook()
{
	//�ѵ�ַ�滻���� 
	common_callAddrHook(pNtOpenProcess,NewObOpenObjectByPointer_forNtOpenProcess,ObOpenObjectByPointer,PAGE_SIZE);
	common_callAddrHook(pNtOpenThread,NewObOpenObjectByPointer_forNtOpenThread,ObOpenObjectByPointer,PAGE_SIZE);
}


//����shadow ssdt hook
VOID protection_startShadowSSDTHook()
{
	//���ӵ�GUI���̵�ַ�ռ�
	KeAttachProcess(pEProcess);

	_try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���ȱ���ԭ���ĵ�ַ
			OldNtUserBuildHwndList = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP];
			OldNtUserDestroyWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP];
			OldNtUserFindWindowEx = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP];						;
			OldNtUserGetForegroundWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP];
            OldNtUserMessageCall = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP];
			OldNtUserPostMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP];
			OldNtUserPostThreadMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP];
			OldNtUserQueryWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP];
			OldNtUserSetWindowLong = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP];
			OldNtUserShowWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP];
			OldNtUserWindowFromPoint = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP];		
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���ȱ���ԭ���ĵ�ַ
			OldNtUserBuildHwndList = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7];
			OldNtUserDestroyWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7];
			OldNtUserFindWindowEx = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7];						;
			OldNtUserGetForegroundWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7];
			OldNtUserMessageCall = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7];
			OldNtUserPostMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7];
			OldNtUserPostThreadMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7];
			OldNtUserQueryWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7];
			OldNtUserSetWindowLong = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7];
			OldNtUserShowWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7];
			OldNtUserWindowFromPoint = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7];
		}



		//ȥ���ڴ汣��
		UnProtected();

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���¶�λshadow SSDT��ַ
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP] = NewNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP] = NewNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP] = NewNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP] = NewNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP] = NewNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP] = NewNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP] = NewNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP] = NewNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP] = NewNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP] = NewNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP] = NewNtUserWindowFromPoint;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���¶�λshadow SSDT��ַ
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7] = NewNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7] = NewNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7] = NewNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7] = NewNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7] = NewNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7] = NewNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7] = NewNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7] = NewNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7] = NewNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7] = NewNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7] = NewNtUserWindowFromPoint;
		}
		

		//�ָ��ڴ汣��
		Protected();
	}
	__finally
	{
		KeDetachProcess();
	}
}

//ֹͣshadow ssdt hook
VOID protection_stopShadowSSDTHook()
{
	//���ӵ�GUI���̵�ַ�ռ�
	KeAttachProcess(pEProcess);

	_try
	{
		//ȥ���ڴ汣��
		UnProtected();

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���¶�λshadow SSDT��ַ
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP] = OldNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP] = OldNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP] = OldNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP] = OldNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP] = OldNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP] = OldNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP] = OldNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP] = OldNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP] = OldNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP] = OldNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP] = OldNtUserWindowFromPoint;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���¶�λshadow SSDT��ַ
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7] = OldNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7] = OldNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7] = OldNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7] = OldNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7] = OldNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7] = OldNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7] = OldNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7] = OldNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7] = OldNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7] = OldNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7] = OldNtUserWindowFromPoint;
		}

		//�ָ��ڴ汣��
		Protected();
	}
	__finally
	{
		KeDetachProcess();
	}
}