// KsBinSwordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "KsBinSwordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_ICON_NOTIFY  WM_USER+10
#define WM_ICON_LBUTTONDBLCLK  WM_USER+11

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()


};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

	
END_MESSAGE_MAP()


// CKsBinSwordDlg �Ի���




CKsBinSwordDlg::CKsBinSwordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKsBinSwordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKsBinSwordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
	DDX_Control(pDX, IDC_COMBO2, m_ComboSkin);
}

BEGIN_MESSAGE_MAP(CKsBinSwordDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CKsBinSwordDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CKsBinSwordDlg::OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CKsBinSwordDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDTOSYSTRAY, &CKsBinSwordDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(WM_ICON_LBUTTONDBLCLK, OnShowPage)
	ON_COMMAND(ID_MENU_11, &CKsBinSwordDlg::OnMenuShowMain)
	ON_COMMAND(ID_MENU_1222, &CKsBinSwordDlg::OnMenuExit)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CKsBinSwordDlg::OnCbnSelchangeSkin)
END_MESSAGE_MAP()


// CKsBinSwordDlg ��Ϣ�������

BOOL CKsBinSwordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    PageArr[0]=&m_Page1;
	PageArr[1]=&m_Page2;
	PageArr[2]=&m_Page3;
	PageArr[3]=&m_Page4;
	PageArr[4]=&m_Page5;
	PageArr[5]=&m_Page6;
    PageArr[6]=&m_Page7;
	PageArr[7]=&m_Page8;
	PageArr[8]=&m_Page9;

	m_tab1.InsertItem(0, _T(" ���̹���"));
	PageArr[0]->Create(IDD_DIALOG1, &m_tab1);

	m_tab1.InsertItem(1, _T(" �������"));
    PageArr[1]->Create(IDD_DIALOG2, &m_tab1);

	m_tab1.InsertItem(2, _T(" ����ģ��ö��"));
    PageArr[2]->Create(IDD_DIALOG3, &m_tab1);

	m_tab1.InsertItem(3, _T("�����Ϣ"));
    PageArr[3]->Create(IDD_DIALOG4, &m_tab1);

	m_tab1.InsertItem(4, _T(" SSDT"));
    PageArr[4]->Create(IDD_DIALOG5, &m_tab1);

	m_tab1.InsertItem(5, _T(" �ļ�������"));
	PageArr[5]->Create(IDD_DIALOG6, &m_tab1);

	m_tab1.InsertItem(6, _T(" ���̱༭��"));
	PageArr[6]->Create(IDD_DIALOG7, &m_tab1);

	m_tab1.InsertItem(7, _T(" �������ǽ"));
	PageArr[7]->Create(IDD_DIALOG8, &m_tab1);

	m_tab1.InsertItem(8, _T(" PE�ļ���Ϣ�鿴"));
	PageArr[8]->Create(IDD_DIALOG9, &m_tab1);

    m_tab1.SetCurSel(0);
	CRect rc;
	m_tab1.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 10;
	rc.right -= 10;
    for (int i=0;i<PageNum;i++)
	{
		PageArr[i]->MoveWindow(&rc);
	}

	PageArr[0]->ShowWindow(SW_SHOW);

    m_ComboSkin.SetCurSel(0);
    //m_ComboSkin.SetWindowText(L"Ƥ��1");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKsBinSwordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKsBinSwordDlg::OnPaint()
{
	CPaintDC    dc(this);   
	CRect    rect;     
	GetClientRect(rect);     
	dc.FillSolidRect(rect,RGB(250, 249, 250));     

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CKsBinSwordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKsBinSwordDlg::OnBnClickedOk()
{

	WinExec("explorer �����ĵ�.htm",0);
}

void CKsBinSwordDlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	
	*pResult = 0;
}

void CKsBinSwordDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel; CRect rc;
	CurSel=m_tab1.GetCurSel(); 
    ShowPageX(CurSel);
    *pResult = 0;


}

void CKsBinSwordDlg::ShowPageX(int Index)
{
	for (int i=0;i<PageNum;i++)
	{
		PageArr[i]->ShowWindow(FALSE);
	}
	PageArr[Index]->ShowWindow(TRUE);
}


void CKsBinSwordDlg::OnBnClickedCancel()//��С������ϵͳ����
{
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,L"ϵͳ�������KsBinSword",m_hIcon,IDR_MENU5); //����
	ShowWindow(SW_HIDE); //���ش���
	//OnOK();


}
LRESULT CKsBinSwordDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}

void CKsBinSwordDlg::OnMenuShowMain()
{
	OnShowPage();
}
void CKsBinSwordDlg::OnShowPage()
{
	m_TrayIcon.RemoveIcon();
	ShowWindow(SW_SHOW);
}
void CKsBinSwordDlg::OnMenuExit()
{
	OnCancel();
}

void CKsBinSwordDlg::OnCbnSelchangeSkin()
{


	CString csSkin;
	//theApp.USkinExit();
	m_ComboSkin.GetWindowText(csSkin);
	if (csSkin==L"Ƥ��1")
	{
		theApp.USkinLoadSkin("Skins\\1.u3");
		return;
	}
	if (csSkin==L"Ƥ��2")
	{
		theApp.USkinLoadSkin("Skins\\2.u3");
		return;
	}
	if (csSkin==L"Ƥ��3")
	{
		theApp.USkinLoadSkin("Skins\\3.u3");
		return;
	}
	if (csSkin==L"Ƥ��4")
	{
		theApp.USkinLoadSkin("Skins\\4.u3");
		return;
	}
	if (csSkin==L"Ƥ��5")
	{
		theApp.USkinLoadSkin("Skins\\5.u3");
		return;
	}
	if (csSkin==L"Ƥ��6")
	{
		theApp.USkinLoadSkin("Skins\\6.u3");
		return;
	}
	if (csSkin==L"Ƥ��7")
	{
		theApp.USkinLoadSkin("Skins\\7.u3");
		return;
	}
	if (csSkin==L"Ƥ��8")
	{
		theApp.USkinLoadSkin("Skins\\8.u3");
		return;
	}
	if (csSkin==L"Ƥ��9")
	{
		theApp.USkinLoadSkin("Skins\\9.u3");
		return;
	}
	if (csSkin==L"Ƥ��10")
	{
		theApp.USkinLoadSkin("Skins\\10.u3");
		return;
	}
	if (csSkin==L"Ƥ��11")
	{
		theApp.USkinLoadSkin("Skins\\11.u3");
		return;
	}

}
