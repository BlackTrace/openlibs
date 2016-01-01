// Page5.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page5.h"
#include "MySystem.h"

// CPage5 �Ի���

IMPLEMENT_DYNAMIC(CPage5, CDialog)

CPage5::CPage5(CWnd* pParent /*=NULL*/)
	: CDialog(CPage5::IDD, pParent)
{

}

CPage5::~CPage5()
{
}

void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_List);
}


BEGIN_MESSAGE_MAP(CPage5, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPage5::OnReSSDTAndThrowSpilth)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage5::OnReShowSSDT)
	ON_BN_CLICKED(IDCANCEL, &CPage5::OnReSSDT)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST3, &CPage5::OnDrawColorForMyList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPage5::OnNMRclickList3)
	ON_COMMAND(ID_MENU_32791, &CPage5::OnMenuRusSSDT)
END_MESSAGE_MAP()


// CPage5 ��Ϣ�������
HBRUSH CPage5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
BOOL CPage5::OnInitDialog()
{
	CDialog::OnInitDialog();
	if( hDriver == INVALID_HANDLE_VALUE )
	{
		//��ȡ�ļ����ʧ��
		// Do Nothing
	}
	//�����
	m_List.InsertColumn( 0, L"�����", LVCFMT_CENTER, 50, -1 );
	m_List.InsertColumn( 1, L"��ǰ��ַ", LVCFMT_CENTER, 100, -1 );
	m_List.InsertColumn( 2, L"ԭʼ��ַ", LVCFMT_CENTER, 100, -1 );
	m_List.InsertColumn( 3, L"��������", LVCFMT_LEFT, 250, -1 );
	m_List.InsertColumn( 4, L"ģ����", LVCFMT_LEFT, 300, -1 );
	//ʹ���ѡ��һ��
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	hDriver=CMySystem::hMyHandle;
	ShowSSDT();
	return true;

}
void CPage5::OnReSSDTAndThrowSpilth()
{
	// �ָ�SSDT��ɾ����ϵͳ����
	
	if( IDYES == MessageBox( L"�ò�����һ����Σ����,�Ƿ����?", L"�ָ�SSDT��������ʾ", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//�ָ�SSDT,��ɾ����ϵͳSSDT����
		if( CMySystem::ReSSDTAndThrowSpilth( hDriver ) )
		{
			MessageBox( L"�ָ�SSDT��ɾ����ϵͳSSDT�����ɹ�!", L"�ɹ�", MB_OK );
		}
		else
		{
			MessageBox( L"�ָ�SSDT��ɾ����ϵͳSSDT����ʧ��!", L"ʧ��", MB_OK );
		}
		//�ػ�
		OnReShowSSDT();
	}
}

void CPage5::ShowSSDT(void)
{
	//�������
	WCHAR szIndex[10]={'\0'};
	WCHAR szCurAddr[18]={'\0'};
	WCHAR szOrgAddr[18]={'\0'};
	WCHAR szTemp[0x200]={'\0'};
	if( !CMySystem::EnumSSDT( hDriver ) )
	{

		//MessageBox( L"ö��SSDTʧ��!", L"����", MB_OK );
		return;		//�����˳�����
	}

  
	for(ULONG i = 0; i < CMySystem::TotalSSDTCount; i ++ )
	{
		wsprintf( szIndex, L"0x%04X", 0);
		wsprintf( szIndex, L"0x%04X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber & 0xFFFF \
			);
		wsprintf( szCurAddr, L"0x%08X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			);
		wsprintf( szOrgAddr, L"0x%08X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			);
		m_List.InsertItem( m_List.GetItemCount(), szIndex );
		m_List.SetItemText( m_List.GetItemCount()-1, 1, szCurAddr );
		m_List.SetItemText( m_List.GetItemCount()-1, 2, szOrgAddr );
        CMySystem::CharToWCHAR(szTemp,((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ServiceFunctionName);
		//wsprintf(szTemp,L"%s",);
		m_List.SetItemText( m_List.GetItemCount()-1, 3, \
			szTemp \
			);
		CMySystem::CharToWCHAR(szTemp,((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName);
		m_List.SetItemText( m_List.GetItemCount()-1, 4, \
			szTemp \
			);
	}
}

void CPage5::OnReShowSSDT()
{
	m_List.DeleteAllItems();
	ShowSSDT();
}

void CPage5::OnReSSDT()
{
	//ȷ��
	if( IDYES == MessageBox( L"�ò�����һ����Σ����,�Ƿ����?", L"�ָ�SSDT��ʾ", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//�ָ�SSDT
		if( CMySystem::ReSSDT( hDriver ) )
		{
			MessageBox( L"�ָ�SSDT�ɹ�!", L"�ɹ�", MB_OK );
		}
		else
		{
			MessageBox( L"�ָ�SSDTʧ��!", L"ʧ��", MB_OK );
		}
		//�ػ�
		OnReShowSSDT();
	}
}

void CPage5::OnDrawColorForMyList(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		//�����ǰSSDT��ַ��ԭʼ��ַ��һ��,����ʾ����Ϊ��ɫ,����Ϊ��ɫ
		if( \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			!= ((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			)		//����Ϊ��ɫ
		{
			clrNewTextColor = RGB( 255, 0, 0 );
		}
		else		//����Ϊ��ɫ
		{
			clrNewTextColor = RGB( 0, 0, 0 );
		}

		//���ñ���ɫ
		if( nItem%2 ==0 )
		{
			clrNewBkColor = RGB( 240, 240, 240 );	//ż���б���ɫΪ��ɫ
		}
		else
		{
			clrNewBkColor = RGB( 255, 255, 255 );	//�����б���ɫΪ��ɫ
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}

}

void CPage5::OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult)//�����Ҽ��˵�
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU6);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	}
	*pResult = 0;
}

void CPage5::OnMenuRusSSDT()
{
	CString str;
	int CurSel=m_List.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_List.GetItemText(CurSel,0); // 0=<col<m_list������
	DWORD dwIndexId=CMySystem::HexToBigInt(str);

	TRACE("OnMenuRusSSDT:%x",dwIndexId);
	if( IDYES == MessageBox( L"�ò�����һ����Σ����,�Ƿ����?", L"�ָ�SSDT��ʾ", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//�ָ�SSDT
 		if( CMySystem::SetHook( hDriver, \
 			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + dwIndexId * sizeof(SSDTSaveTable)))->ulServiceNumber, \
 			&(((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + dwIndexId * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress) \
 			) )
		{
			MessageBox( L"�ָ�SSDT�ɹ�!", L"�ɹ�", MB_OK );
		}
		else
		{
			MessageBox( L"�ָ�SSDTʧ��!", L"ʧ��", MB_OK );
		}
		//�ػ�
		OnReShowSSDT();
	}
}
