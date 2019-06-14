
// GobangDlg.h : 头文件
//

#pragma once

#include "NetSetDlg.h"
#include "ServerSkt.h"
#include "afxwin.h"


const int CHESS_BOARD_SIZE = 15;  //15X15棋盘
const int CHESS_BOARD_LEN = 560;  //棋盘长宽

typedef struct tagMsg
{
	BYTE msgType;	//消息类型， 0x1-落子 0x2-获胜 0x8-闲聊
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
// CGobangDlg 对话框
class CGobangDlg : public CDialogEx
{
// 构造
public:
	CGobangDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 棋盘所有点位
	CPoint m_allChessPoint[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
	// 棋盘中所有落子情况 0-空 1-黑子 2-白子
	int m_iAllChessMan[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
	// 网络Socket
	CServerSkt* m_pNetServerSkt;
	CServerSkt* m_pSktItem;
	// 是否我的回合
	bool m_bIsMyTurn;
	bool m_bIsGameOver;
	
private:
	void DrawChessBoard();
	// 根据坐标画出棋子
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
