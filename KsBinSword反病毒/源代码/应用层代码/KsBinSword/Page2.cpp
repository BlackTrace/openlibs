// Page2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page2.h"

#include "MySystem.h"
// CPage2 �Ի���

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CPage2::IDD, pParent)
	, IsPMon(false)
{

}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_PMon);
	DDX_Control(pDX, IDCANCEL, m_RMon);
	DDX_Control(pDX, IDC_BUTTON1, m_Mmon);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPage2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPage2::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage2::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK2, &CPage2::OnBnClickedExeUSBDiskCheck)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage2::OnBnClickedCheckASP)
END_MESSAGE_MAP()
BOOL CPage2::OnInitDialog()
{
	CDialog::OnInitDialog();
	IsPMon=false;//���̼���
	IsRMon=false;//ע������
	IsMMon=false;//ģ�����
	return true;
}
HBRUSH CPage2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
// CPage2 ��Ϣ�������

void CPage2::OnBnClickedOk()
{
	if (CMySystem::hMyHandle==(HANDLE)-1)
    {
		AfxMessageBox(L"��δ�����豸���ӣ�");
		return;
    }
	if (false==IsPMon)
	{
		IsPMon=true;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_PMON,
			CMySystem::controlbuff,
			256,
			CMySystem::controlbuff,
			256,
			&CMySystem::dw,
			0);
        m_PMon.SetWindowText(L"�رս��̼��");
	}else{
		IsPMon=false;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_PMOFF,
			NULL,
			NULL,
			NULL,
			NULL,
			&CMySystem::dw,
			0);
		m_PMon.SetWindowText(L"��ʼ���̼��");
	}
}

void CPage2::OnBnClickedCancel()
{
	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		AfxMessageBox(L"��δ�����豸���ӣ�");
		return;
	}
	if (false==IsRMon)
	{
		IsRMon=true;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_RMON,
			CMySystem::controlbuff,
			256,
			CMySystem::controlbuff,
			256,
			&CMySystem::dw,
			0);
		m_RMon.SetWindowText(L"�ر�ע�����");
	}else{
		IsRMon=false;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_RMOFF,
			NULL,
			NULL,
			NULL,
			NULL,
			&CMySystem::dw,
			0);
		m_RMon.SetWindowText(L"��ʼע�����");
	}
}

void CPage2::OnBnClickedButton1()
{
	if (CMySystem::hMyHandle==(HANDLE)-1)
	{
		AfxMessageBox(L"��δ�����豸���ӣ�");
		return;
	}
	if (false==IsMMon)
	{
		IsMMon=true;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_MMON,
			CMySystem::controlbuff,
			256,
			CMySystem::controlbuff,
			256,
			&CMySystem::dw,
			0);
		m_Mmon.SetWindowText(L"�ر�ģ����");
	}else{
		IsMMon=false;
		DeviceIoControl(CMySystem::hMyHandle,
			IOCTL_KSBINSWORD_MMOFF,
			NULL,
			NULL,
			NULL,
			NULL,
			&CMySystem::dw,
			0);
		m_Mmon.SetWindowText(L"��ʼģ����");
	}
}

void CPage2::OnBnClickedExeUSBDiskCheck()
{
	CHAR szComm[20]="U�ָ̻�����.exe";
	WinExec(szComm,0);
}

void CPage2::OnBnClickedCheckASP()
{
	CHAR szComm[20]="�ű�ľ���ɱ.exe";
	WinExec(szComm,0);
}
