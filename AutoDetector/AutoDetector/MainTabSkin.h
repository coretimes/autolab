//*****************************************************************************
// �ļ��� : MainTabSkin.h
// 
// ����   : ����ǩƤ��������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "skinobject.h"
#include <atlimage.h>
#include "MainTab.h"

class CMainTabSkin : public CSkinObject
{
public:
	CMainTabSkin(CMainTab * pTabCtrl);
	~CMainTabSkin(void);
	virtual void DrawSkin(const CRect &rcClipArea);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	CImage m_BannerLeft;				// ������������
	CImage m_BannerRight;				// ���������Һ��
	CImage m_BannerMiddle;				// ���������м���
	CMainTab * m_pTabCtrl;			// �Ի���
};
