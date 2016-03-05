// IncuAreaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "IncuAreaCtrl.h"
#include "GeneralFuction.h"
#include "MemDC.h"

IncuConfigNode CIncuAreaCtrl::staticIncuConfigList[NUM_IncuPOS];

// CIncuAreaCtrl

IMPLEMENT_DYNAMIC(CIncuAreaCtrl, CStatic)

CIncuAreaCtrl::CIncuAreaCtrl()
{

}

CIncuAreaCtrl::~CIncuAreaCtrl()
{
}


BEGIN_MESSAGE_MAP(CIncuAreaCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIncuAreaCtrl message handlers



void CIncuAreaCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
}


void CIncuAreaCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc,m_rcClient); // 默认矩形为裁剪区域
	memDC.CopyFromSrcDC();
	_DrawFrame(&memDC);
	memDC.CopyToSrcDC();
}

void CIncuAreaCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CStatic::OnMouseMove(nFlags, point);
}

void CIncuAreaCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CStatic::OnLButtonDown(nFlags, point);
}

void CIncuAreaCtrl::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect(m_rcClient);

	m_rcTitleRegion.top = m_rcClient.top;
	m_rcTitleRegion.left = m_rcClient.left;
	m_rcTitleRegion.right = m_rcClient.right;
	m_rcTitleRegion.bottom = (int)(m_rcClient.bottom * 0.08);

	m_rcNodesRegion.top = m_rcClient.top + (int)(m_rcClient.Height() * 0.1);
	m_rcNodesRegion.left = m_rcClient.left + 10;
	m_rcNodesRegion.right = m_rcClient.right - 10;
	m_rcNodesRegion.bottom = m_rcClient.bottom - 10;
}

void CIncuAreaCtrl::_DrawFrame(CDC *pDC)
{
	CPen penNodesRegionFrame(PS_SOLID,1, RGB(171,171,171));  // 灰线画笔
	CBrush brTitle(RGB(238,141,91));						 // 红色标题背景画刷
	CBrush brBackGround(RGB(255,255,255));					 // 白色背景画刷

	int nSavedDC = pDC->SaveDC();
	// 刷白色客户区域背景色
	pDC->SelectObject(brBackGround);
	pDC->FillRect(&m_rcClient,&brBackGround);

	// 绘制标题区
	pDC->SelectObject(brTitle);
	pDC->FillRect(&m_rcTitleRegion,&brTitle);
	CGeneralFuction * pFution = CGeneralFuction::GetInstance();
	pFution->DrawShadowCation(pDC,m_rcTitleRegion,_T("孵育区"));

	// 画整个控件区域边框色
	CBrush brClientEdge(RGB(210,220,235));					 // 浅灰线边框颜色
	pDC->SelectObject(brClientEdge);
	pDC->FrameRect(&m_rcClient,&brClientEdge);

	// 画节点区域外围虚线
	CPen penDotLine(PS_DOT,1,RGB(222,222,222));
	CRect rcDotLine = m_rcNodesRegion;
	rcDotLine.InflateRect(5,5,5,5);
	pDC->SelectObject(brBackGround);
	pDC->SelectObject(penDotLine);
	pDC->Rectangle(rcDotLine);

	// 画节点区域背景
	pDC->SelectObject(penNodesRegionFrame);
	pDC->Rectangle(&m_rcNodesRegion);

	//// 画节点区域框架和点
	//for(int x=0; x<=NUM_COL; x++)
	//{
	//	if(x >0)
	//	{
	//		RECT rcTop,rcBottom;
	//		_GetNodeRect(x,1,rcTop);
	//		_GetNodeRect(x,NUM_ROW + 1,rcBottom);
	//		pDC->MoveTo(CPoint(rcTop.left,rcTop.top));
	//		pDC->LineTo(CPoint(rcBottom.left,rcBottom.bottom));
	//	}

	//	for(int y=1; y <= NUM_ROW+1; y++)
	//	{
	//		_DrawNode(x,y,pDC);
	//	}
	//}
	pDC->RestoreDC(nSavedDC);
}

BOOL CIncuAreaCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
