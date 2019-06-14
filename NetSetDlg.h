#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// NetSetDlg �Ի���

class NetSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetSetDlg)

public:
	NetSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NetSetDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Setting };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����˿�
	int m_iPort;
	CIPAddressCtrl m_ipCtrl;
	CString m_strIP;
	bool m_bHost;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_radioHost;
};
