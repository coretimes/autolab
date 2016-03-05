//*****************************************************************************
// 文件名 : CIncuAreaCtrl.h
// 
// 类名   : 试剂区示意图控件
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "SkinObject.h"

class CMainTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMainTab)

public:
	CMainTab();
	virtual ~CMainTab();

public:
	CRect GetClienRect(){return m_rcClient;}
	int GetHeaderHeight(){return nHeaderHight;}
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PreSubclassWindow();

protected:
	CRect m_rcClient;
	int nHeaderHight;
	CSkinObject * m_pSkin;
};


