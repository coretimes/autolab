//*****************************************************************************
// �ļ��� : CIncuAreaCtrl.h
// 
// ����   : �Լ���ʾ��ͼ�ؼ�
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-10       ��־��       �½�
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


