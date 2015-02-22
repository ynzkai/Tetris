
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "Tetris.h"
#include "ChildView.h"
#include "Tetris_public.h"
#include "Mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_TIMER 1

// CChildView

CChildView::CChildView()
	: sound(TRUE)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	draw_tetris(&dc, rect);

	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	init_area();
	SetTimer(IDC_TIMER, speed, NULL);

	return 0;
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nChar)
	{
	case VK_LEFT:
		if(game_state == STATE_RUN)
		{
			move_block(MOVE_DIRECTION_LEFT);
			Invalidate();
		}
		break;
	case VK_RIGHT:
		if(game_state == STATE_RUN)
		{
			move_block(MOVE_DIRECTION_RIGHT);
			Invalidate();
		}
		break;
	case VK_SPACE:
		if(game_state == STATE_RUN)
		{
			rotate_block(ROTATE_DIRECTION_RIGHT);
			Invalidate();
			UpdateWindow();
		}
		break;
	case VK_DOWN:
		if(game_state == STATE_RUN && speed_state == SPEED_STATE_NORMAL)
		{
			SetTimer(IDC_TIMER, SPEED_FAST, NULL);
			speed_state = SPEED_STATE_FAST;
		}
		break;
	case VK_F1:
		sound = !sound;
		break;
	case VK_F2:
		init_area();
		SetTimer(IDC_TIMER, speed, NULL);
		game_state = STATE_RUN;
		break;
	}

	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(game_state == STATE_RUN && !move_block(MOVE_DIRECTION_DOWN))
	{
		if(sound)
		{
			PlaySound(MAKEINTRESOURCE(IDR_DING), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		}
		if(check() > 0 && sound)
		{
			PlaySound(MAKEINTRESOURCE(IDR_SCORE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		}
		create_block();
	}
	Invalidate();

	//CWnd::OnTimer(nIDEvent);
}


void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nChar)
	{
	case 'p':
	case 'P':
		if(game_state == STATE_PAUSE)
		{
			game_state = STATE_RUN;
		}
		else if(game_state == STATE_RUN)
		{
			game_state = STATE_PAUSE;
		}
		break;
	}

	//CWnd::OnChar(nChar, nRepCnt, nFlags);
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nChar)
	{
	case VK_DOWN:
		if(speed_state == SPEED_STATE_FAST)
		{
			SetTimer(IDC_TIMER, speed, NULL);
			speed_state = SPEED_STATE_NORMAL;
		}
		break;
	}

	//CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
