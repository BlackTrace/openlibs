#pragma once


// CRuleDlg �Ի���

class CRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CRuleDlg)

public:
	CRuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRuleDlg();

	unsigned long srcIp;
	unsigned long srcMask;
	unsigned short srcPort;

	unsigned long dstIp;
	unsigned long dstMask;
	unsigned short dstPort;

	unsigned int protocol;

	int cAction;
	CString	m_ipsource;
	UINT	m_portsource;
	CString	m_ipdestination;
	UINT	m_portDestination;
	CString	m_action;
	CString	m_protocol;
	CString	m_srcMask;
	CString	m_dstMask;
// �Ի�������
	enum { IDD = IDD_ADDRULEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

public:
	afx_msg void OnBnClickedOk();
};
