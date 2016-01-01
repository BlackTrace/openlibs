// KsBinSwordDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"
#include "page5.h"
#include "page6.h"
#include "page7.h"
#include "page8.h"
#include "page9.h"
#include "TrayIcon.h"
#include "afxwin.h"
// CKsBinSwordDlg �Ի���
#define PageNum 9

class CKsBinSwordDlg : public CDialog
{
// ����
public:
	CKsBinSwordDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KSBINSWORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab1;
    CTrayIcon m_TrayIcon;
	CPage1 m_Page1;
	CPage2 m_Page2;
	CPage3 m_Page3;
	CPage4 m_Page4;
	CPage5 m_Page5;
	CPage6 m_Page6;
	CPage7 m_Page7;
	CPage8 m_Page8;
	CPage9 m_Page9;
	CDialog* PageArr[PageNum];
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void ShowPageX(int Index);
public:
	afx_msg void OnBnClickedCancel();
	void OnShowPage();
public:
	afx_msg void OnMenuShowMain();
public:
	afx_msg void OnMenuExit();
public:
	CComboBox m_ComboSkin;
public:
	afx_msg void OnCbnSelchangeSkin();
};
