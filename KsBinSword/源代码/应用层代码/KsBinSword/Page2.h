#pragma once
#include "afxwin.h"
#include "EditEx.h"

// CPage2 �Ի���

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool IsPMon;//���̼���
	bool IsRMon;//ע������
	bool IsMMon;//ģ�����

	CButton m_PMon;

	CButton m_RMon;

	CButton m_Mmon;

	afx_msg void OnBnClickedOk();
	BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedExeUSBDiskCheck();

	afx_msg void OnBnClickedCheckASP();
};
