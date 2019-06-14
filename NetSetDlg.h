#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// NetSetDlg 对话框

class NetSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetSetDlg)

public:
	NetSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Setting };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 网络端口
	int m_iPort;
	CIPAddressCtrl m_ipCtrl;
	CString m_strIP;
	bool m_bHost;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_radioHost;
};
