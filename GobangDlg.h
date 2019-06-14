
// GobangDlg.h : ͷ�ļ�
//

#pragma once

#include "NetSetDlg.h"
#include "ServerSkt.h"
#include "afxwin.h"


const int CHESS_BOARD_SIZE = 15;  //15X15����
const int CHESS_BOARD_LEN = 560;  //���̳���

typedef struct tagMsg
{
	BYTE msgType;	//��Ϣ���ͣ� 0x1-���� 0x2-��ʤ 0x8-����
	BYTE xPos;
	BYTE yPos;
	char info[256];
}Msg;
enum ChessManColor
{
	BlankChess=0,
	BlackChess,
	WhiteChess
};
// CGobangDlg �Ի���
class CGobangDlg : public CDialogEx
{
// ����
public:
	CGobangDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSettingNewgame();

public:
	NetSetDlg m_netSetDlg;
	CPoint m_basePoint;

private:
	bool m_bBlack;
	int m_iChessGap;
	// �������е�λ
	CPoint m_allChessPoint[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
	// ����������������� 0-�� 1-���� 2-����
	int m_iAllChessMan[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
	// ����Socket
	CServerSkt* m_pNetServerSkt;
	CServerSkt* m_pSktItem;
	// �Ƿ��ҵĻغ�
	bool m_bIsMyTurn;
	bool m_bIsGameOver;
	
private:
	void DrawChessBoard();
	// �������껭������
	void DrawChessMan(int xPos, int yPos, bool bBlack=true);
	bool ValidPos(CPoint pos, CPoint &drawPos, int& xIndex, int& yIndex);
	void InitServer(UINT port, CString ip);
	void InitClient(UINT port, CString ip);
	void DrawPlayer();
	bool WinGame(int xIndex, int yIndex);

public:
	void SktAccept();
	void SktReceive();
	void SktConnect();
	void SktSend(int msgType, int xIndex, int yIndex);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_PlayerPic;
	CStatic m_strPlayer;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
