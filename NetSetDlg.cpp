// NetSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang.h"
#include "NetSetDlg.h"
#include "afxdialogex.h"


// NetSetDlg 对话框

IMPLEMENT_DYNAMIC(NetSetDlg, CDialogEx)

NetSetDlg::NetSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Setting, pParent)
	, m_iPort(8000)
{

}

NetSetDlg::~NetSetDlg()
{
}

void NetSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PortEdit, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 0, 8888);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipCtrl);
	DDX_Control(pDX, IDC_Host, m_radioHost);
}


BEGIN_MESSAGE_MAP(NetSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NetSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// NetSetDlg 消息处理程序


void NetSetDlg::OnBnClickedOk()
{
	BYTE field0, field1, field2, field3;
	m_ipCtrl.GetAddress(field0, field1, field2, field3);

	m_strIP.Format(_T("%d.%d.%d.%d"), field0, field1, field2, field3);

	if (m_radioHost.GetCheck())
	{
		m_bHost = true;
	}
	else
	{
		m_bHost = false;
	}

	//AfxMessageBox(ip);
	CDialogEx::OnOK();
}




BOOL NetSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_radioHost.SetCheck(TRUE);
	
	m_ipCtrl.SetAddress(0x7F, 0x00, 0x00, 0x01);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
