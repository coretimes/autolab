// MainTab.cpp : 实现文件
//

#include "stdafx.h"
#include "MainTab.h"
#include "MemDC.h"
#include "FontLib.h"
#include "MainTabSkin.h"

IMPLEMENT_DYNAMIC(CMainTab, CTabCtrl)

CMainTab::CMainTab()
{
	m_pSkin = new CMainTabSkin(this);
}

CMainTab::~CMainTab()
{
	delete m_pSkin;
}


BEGIN_MESSAGE_MAP(CMainTab, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CMainTab::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMainTab::OnPaint()
{
	m_pSkin->DrawSkin(m_rcClient);
}

void CMainTab::PreSubclassWindow()
{
	// 设置皮肤ID
	nHeaderHight = 33;
	SetMinTabWidth(60);								//设置最小选项卡的宽度
	SetItemSize(CSize(30,30));
	CTabCtrl::PreSubclassWindow();
}

void CMainTab::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);
	GetClientRect(m_rcClient);
}
void CMainTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	(static_cast<CMainTabSkin *> (m_pSkin))->DrawItem(lpDrawItemStruct);
}