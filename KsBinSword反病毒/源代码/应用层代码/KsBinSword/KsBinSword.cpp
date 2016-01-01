// KsBinSword.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "KsBinSwordDlg.h"

#include"Include\\USkin.h"
#pragma comment(lib, "LIB\\USkin.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKsBinSwordApp

BEGIN_MESSAGE_MAP(CKsBinSwordApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKsBinSwordApp ����

CKsBinSwordApp::CKsBinSwordApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CKsBinSwordApp ����

CKsBinSwordApp theApp;


// CKsBinSwordApp ��ʼ��

BOOL CKsBinSwordApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	hModuleUSkin = LoadLibrary(L"USkin.dll" ) ;
	USkinInit = (PFNUSkinInit)GetProcAddress ( hModuleUSkin, "USkinInit" ) ;
	USkinExit = (PFNUSkinExit)GetProcAddress ( hModuleUSkin, "USkinExit" ) ;
	USkinLoadSkin = (PFNUSkinLoadSkin)GetProcAddress ( hModuleUSkin, "USkinLoadSkin" ) ;
	

	USkinInit(NULL,NULL,"Skins\\1.u3");


	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CKsBinSwordDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	USkinExit();
	//AfxMessageBox(L"�˳���");

	DeviceIoControl(CMySystem::hMyHandle,
		IOCTL_KSBINSWORD_PMOFF,
		NULL,
		NULL,
		NULL,
		NULL,
		&CMySystem::dw,
		0);


	DeviceIoControl(CMySystem::hMyHandle,
		IOCTL_KSBINSWORD_RMOFF,
		NULL,
		NULL,
		NULL,
		NULL,
		&CMySystem::dw,
		0);


	DeviceIoControl(CMySystem::hMyHandle,
		IOCTL_KSBINSWORD_MMOFF,
		NULL,
		NULL,
		NULL,
		NULL,
		&CMySystem::dw,
		0);

	CloseHandle(CMySystem::hMyHandle);

	CMySystem::UnloadDriver(L"KsBinSword");
	CMySystem::UnloadDriver(L"Explorer");
	CMySystem::UnloadDriver(L"DrvFltIp");
	return FALSE;
}
