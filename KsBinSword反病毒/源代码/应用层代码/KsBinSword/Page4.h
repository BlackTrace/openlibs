#pragma once
#include "afxwin.h"
#include "EditEx.h"

// CPage4 �Ի���
#define MaxMonNum 50
#define MaxLenNum 200
class CPage4 : public CDialog
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage4();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEditEx m_MonInfoEdit;
public:
	CHAR g_OutMonInfoArr[MaxMonNum][MaxLenNum];
	void            PrintMonInfo(HANDLE hMyHandle);
	afx_msg void    OnBnClickedResMonInfo();
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBnClickedClearInfo();
};
