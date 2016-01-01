#include "stdafx.h"
#include "Driver.h"

//��������
HANDLE LoadDriver(LPCTSTR lpFileName)
{
	TCHAR szLinkName[] = L"dosProcessMonitor";

	HANDLE hDriver = INVALID_HANDLE_VALUE;
	// ��SCM������
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCM == NULL)
	{
		return NULL;
	}

	// ������򿪷���
	SC_HANDLE hService = CreateService(hSCM, szLinkName, szLinkName, SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
		lpFileName, NULL, 0, NULL, NULL, NULL);
	if(hService == NULL)
	{
		int nError = GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			hService = OpenService(hSCM, szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	if(hService == NULL)
	{
		return NULL;
	}
	// ��������
	if(!StartService(hService, 0, NULL))  // �������DriverEntry����
	{
		int nError = GetLastError();
		if(nError != ERROR_SERVICE_ALREADY_RUNNING)
		{	
			return NULL;
		}
	}
	// �򿪵����������������豸�ľ��
	TCHAR sz[256];
	wsprintf(sz, L"\\\\.\\%s", szLinkName);
	hDriver = CreateFile(sz, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hDriver == INVALID_HANDLE_VALUE)
	{	
		return NULL;
	}
	return hDriver;
}
//ж������
void UnloadDriver(HANDLE hDriver)
{
	TCHAR szLinkName[] = L"dosDriverDeianeira";

	CloseHandle( hDriver );

	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL,
		SC_MANAGER_CREATE_SERVICE );
	if (NULL != hSCManager)
	{
		SC_HANDLE hService = OpenService( hSCManager, szLinkName, DELETE | SERVICE_STOP );
		if (NULL != hService)
		{
			SERVICE_STATUS ss;
			ControlService( hService, SERVICE_CONTROL_STOP, &ss );
			DeleteService( hService );
			CloseServiceHandle( hService );
		}
		CloseServiceHandle( hSCManager );
	}
}