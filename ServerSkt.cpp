// ServerSkt.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang.h"
#include "ServerSkt.h"
#include "GobangDlg.h"


// CServerSkt

CServerSkt::CServerSkt()
{
}

CServerSkt::CServerSkt(CString ip, int port)
{

}

CServerSkt::CServerSkt(CGobangDlg* ChessDlg)
{
	m_pChessDlg = ChessDlg;
}

CServerSkt::~CServerSkt()
{
}

void CServerSkt::OnAccept(int nErrorCode)
{
	CAsyncSocket::OnAccept(nErrorCode);
	m_pChessDlg->SktAccept();
}

void CServerSkt::OnReceive(int nErrorCode)
{
	CAsyncSocket::OnReceive(nErrorCode);
	m_pChessDlg->SktReceive();
}

void CServerSkt::OnSend(int nErrorCode)
{
	CAsyncSocket::OnSend(nErrorCode);
}

void CServerSkt::OnConnect(int nErrorCode)
{
	if (0 != nErrorCode)
	{

	}
	CAsyncSocket::OnConnect(nErrorCode);

}

// CServerSkt 成员函数
