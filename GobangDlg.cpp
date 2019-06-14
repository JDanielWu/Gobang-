
// GobangDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang.h"
#include "GobangDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGobangDlg 对话框



CGobangDlg::CGobangDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GOBANG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bBlack = true;
	m_bIsMyTurn = false;
	m_bIsGameOver = false;

	m_basePoint.x = 100;
	m_basePoint.y = 50;

	m_iChessGap = CHESS_BOARD_LEN / (CHESS_BOARD_SIZE - 1);

	for (int i = 0; i < CHESS_BOARD_SIZE; ++i)
	{
		for (int j = 0; j < CHESS_BOARD_SIZE; ++j)
		{
			m_allChessPoint[i][j].x = m_basePoint.x + m_iChessGap*i;
			m_allChessPoint[i][j].y = m_basePoint.y + m_iChessGap*j;

			// 还未落子
			m_iAllChessMan[i][j] = 0;
		}
	}

	m_pNetServerSkt = new CServerSkt(this);
	m_pSktItem = new CServerSkt(this);
}

void CGobangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChessPic, m_PlayerPic);
	DDX_Control(pDX, IDC_Player, m_strPlayer);
}

BEGIN_MESSAGE_MAP(CGobangDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_SETTING_NEWGAME, &CGobangDlg::OnSettingNewgame)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CGobangDlg 消息处理程序

BOOL CGobangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	MoveWindow(100, 100, 800, 800);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGobangDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGobangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//DrawChessBoard();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGobangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGobangDlg::OnSettingNewgame()
{
	//m_netSetDlg.Create(IDD_Setting, this);
	//m_netSetDlg.ShowWindow(SW_SHOW);

	int nRes = m_netSetDlg.DoModal();

	//画棋盘
	if (IDOK == nRes)
	{
		if (m_netSetDlg.m_bHost)
		{
			m_bBlack = true;
			m_bIsMyTurn = true;
			InitServer(m_netSetDlg.m_iPort, m_netSetDlg.m_strIP);
		}
		else
		{
			m_bBlack = false;
			m_bIsMyTurn = false;
			InitClient(m_netSetDlg.m_iPort, m_netSetDlg.m_strIP);
			m_strPlayer.SetWindowText(_T("Client"));
		}
		DrawChessBoard();
		DrawPlayer();
	}
}

void CGobangDlg::InitServer(UINT port, CString ip)
{
	m_pNetServerSkt->Create(port, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, ip);
	m_pNetServerSkt->Listen();
}

void CGobangDlg::InitClient(UINT port, CString ip)
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("WindowsSocket initial failed!"));
		return;
	}
	if (!m_pNetServerSkt->Create())
	{
		AfxMessageBox(_T("Socket Create Fail!"));
	}

	m_pNetServerSkt->Connect(ip, port);
}

void CGobangDlg::DrawPlayer()
{
	CBitmap BlackPic; 
	CBitmap WhitePic;
	HBITMAP hBmp;   


	BlackPic.LoadBitmap(IDB_BlackChess);
	WhitePic.LoadBitmap(IDB_WhiteChess);

	if (m_bIsMyTurn)
	{
		if (m_bBlack)
		{
			hBmp = (HBITMAP)BlackPic.GetSafeHandle();
		}
		else
		{
			hBmp = (HBITMAP)WhitePic.GetSafeHandle();
		}
	}
	else
	{
		if (m_bBlack)
		{
			hBmp = (HBITMAP)WhitePic.GetSafeHandle();
		}
		else
		{
			hBmp = (HBITMAP)BlackPic.GetSafeHandle();
		}
	}
	 
	m_PlayerPic.SetBitmap(hBmp);
}

bool CGobangDlg::WinGame(int xIndex, int yIndex)
{
	auto GetScoreX = [this](int x_pos, int y_pos)
	{
		int iScoreX = 0;
		for (int i = x_pos - 1; i >= 0; --i)
		{
			if (m_iAllChessMan[i][y_pos] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreX++;
			}
			else
			{
				break;
			}
		}

		for (int i = x_pos + 1; i<CHESS_BOARD_SIZE; ++i)
		{
			if (m_iAllChessMan[i][y_pos] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreX++;
			}
			else
			{
				break;
			}
		}
		return iScoreX;
	};

	auto GetScoreY = [this](int x_pos, int y_pos)
	{
		int iScoreY = 0;
		for (int i = y_pos - 1; i >= 0; --i)
		{
			if (m_iAllChessMan[x_pos][i] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreY++;
			}
			else
			{
				break;
			}
		}

		for (int i = y_pos + 1; i<CHESS_BOARD_SIZE; ++i)
		{
			if (m_iAllChessMan[x_pos][i] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreY++;
			}
			else
			{
				break;
			}
		}

		return iScoreY;
	};

	auto GetScoreL = [this](int x_pos, int y_pos)
	{
		int iScoreL = 0;
		//左上方向
		for (int i = x_pos - 1, j = y_pos + 1; i >= 0, j<CHESS_BOARD_SIZE; --i, ++j)
		{
			if (m_iAllChessMan[i][j] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreL++;
			}
			else
			{
				break;
			}
		}
		//右下方向
		for (int i = x_pos + 1, j = y_pos - 1; i<CHESS_BOARD_SIZE, j >= 0; ++i, --j)
		{
			if (m_iAllChessMan[i][j] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreL++;
			}
			else
			{
				break;
			}
		}
		return iScoreL;
	};

	auto GetScoreR = [this](int x_pos, int y_pos)
	{
		int iScoreR = 0;
		//左下方向
		for (int i = x_pos - 1, j = y_pos - 1; i >= 0, j >= 0; --i, --j)
		{
			if (m_iAllChessMan[i][j] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreR++;
			}
			else
			{
				break;
			}
		}
		//右上方向
		for (int i = x_pos + 1, j = y_pos + 1; i<CHESS_BOARD_SIZE, j<CHESS_BOARD_SIZE; ++i, ++j)
		{
			if (m_iAllChessMan[i][j] == m_iAllChessMan[x_pos][y_pos])
			{
				iScoreR++;
			}
			else
			{
				break;
			}
		}
		return iScoreR;
	};

	if (4 == GetScoreX(xIndex, yIndex))
		return true;

	if (4 == GetScoreY(xIndex, yIndex))
		return true;

	if (4 == GetScoreL(xIndex, yIndex))
		return true;

	if (4 == GetScoreR(xIndex, yIndex))
		return true;

	return false;
}

void CGobangDlg::DrawChessBoard()
{
	CClientDC dc(this);
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	dc.SelectObject(&pen);
	
	CPoint leftTop(m_basePoint);
	
	//横向
	for (int i = 0; i < CHESS_BOARD_SIZE; ++i)
	{
		dc.MoveTo(leftTop);
		dc.LineTo(leftTop.x+ CHESS_BOARD_LEN, leftTop.y);
		leftTop.y += m_iChessGap;
	}
	//纵向
	leftTop = m_basePoint;
	for (int j = 0; j < CHESS_BOARD_SIZE; ++j)
	{
		dc.MoveTo(leftTop);
		dc.LineTo(leftTop.x, leftTop.y+ CHESS_BOARD_LEN);
		leftTop.x += m_iChessGap;
	}
}

void CGobangDlg::DrawChessMan(int xPos, int yPos, bool bBlack)
{
	CClientDC dc(this);
	CBrush  brush;
	if (bBlack)
	{
		brush.CreateSolidBrush(RGB(0, 0, 0));
	}
	else
	{
		brush.CreateSolidBrush(RGB(255, 255, 255));
	}
	
	dc.SelectObject(&brush);

	int chessManSize = 15;
	dc.Ellipse(xPos- chessManSize, yPos- chessManSize, xPos + chessManSize, yPos + chessManSize);
}

bool CGobangDlg::ValidPos(CPoint pos, CPoint &drawPos, int& xIndex, int& yIndex)
{
	drawPos = pos;

	int distanse = 100;

	for (int i = 0; i < CHESS_BOARD_SIZE; ++i)
	{
		for (int j = 0; j < CHESS_BOARD_SIZE; ++j)
		{
			int dis = pow(pos.x - m_allChessPoint[i][j].x, 2) + pow(pos.y - m_allChessPoint[i][j].y, 2);
			// 距离符合且没有落子
			if ( dis <= distanse && m_iAllChessMan[i][j] == 0)
			{
				drawPos = m_allChessPoint[i][j];
				xIndex = i;
				yIndex = j;
				return true;
			}
		}
	}

	return false;
}

void CGobangDlg::SktAccept()
{
	struct sockaddr_in clientIP;
	int len = sizeof(clientIP);

	m_pNetServerSkt->Accept(*m_pSktItem, (SOCKADDR*)&clientIP, &len);
}

void CGobangDlg::SktReceive()
{
	char m_buf[1024];
	//主机
	if (m_bBlack)
	{
		m_pSktItem->Receive(m_buf, 1024);
	}
	else
	{
		m_pNetServerSkt->Receive(m_buf, 1024);
	}
	
	int msgType = m_buf[0];

	int x_index = m_buf[1];
	int y_index = m_buf[2];

	CPoint drawPos;
	CPoint point = m_allChessPoint[x_index][y_index];
	
	if (msgType&0x1)
	{
		// 通过网络发送的位置 都是对方棋子，反色
		if (!m_bIsMyTurn && ValidPos(point, drawPos, x_index, y_index))
		{
			DrawChessMan(drawPos.x, drawPos.y, !m_bBlack);
			m_iAllChessMan[x_index][y_index] = !m_bBlack ? BlackChess : WhiteChess;
			m_bIsMyTurn = true;
			DrawPlayer();
		}
		if (msgType & 0x2)
		{
			m_bIsGameOver = true;
			AfxMessageBox(_T("You lost!"));
		}
		return;
	}
	//闲聊
	if (msgType == 0x8)
	{
		//todo
		return;
	}


}

void CGobangDlg::SktConnect()
{

}

void CGobangDlg::SktSend(int msgType, int xIndex, int yIndex)
{
	//发送
	char posBuf[3];
	posBuf[0] = msgType;
	posBuf[1] = xIndex;
	posBuf[2] = yIndex;
	
	//主机
	if (m_bBlack)
	{
		m_pSktItem->Send(posBuf, 3);
	}
	else
	{
		m_pNetServerSkt->Send(posBuf, 3);
	}
}

void CGobangDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint drawPos;
	int x_index = 0;
	int y_index = 0;

	if (!m_bIsGameOver && m_bIsMyTurn && ValidPos(point, drawPos, x_index, y_index))
	{
		DrawChessMan(drawPos.x, drawPos.y, m_bBlack);
		m_iAllChessMan[x_index][y_index] = m_bBlack ? BlackChess : WhiteChess;
		m_bIsMyTurn = false;
		DrawPlayer();	
		if (WinGame(x_index, y_index))
		{
			SktSend(0x1|0x2, x_index, y_index);
			m_bIsGameOver = true;
			AfxMessageBox(_T("You win!"));
		}
		else
		{
			SktSend(0x1, x_index, y_index);
		}
		
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


HBRUSH CGobangDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_Player)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
