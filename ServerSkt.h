#pragma once

// CServerSkt ÃüÁîÄ¿±ê
class CGobangDlg;

class CServerSkt : public CAsyncSocket
{
public:
	CServerSkt();
	CServerSkt(CGobangDlg* ChessDlg);
	CServerSkt(CString ip, int port);
	virtual ~CServerSkt();

	void OnAccept(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);
	void OnConnect(int nErrorCode);


public:
	CGobangDlg* m_pChessDlg;
};


