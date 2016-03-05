//*****************************************************************************
// 文件名 : MainTabSkin.h
// 
// 类名   : 主标签皮肤绘制类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
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
	CImage m_BannerLeft;				// 标题区域左横幅
	CImage m_BannerRight;				// 标题区域右横幅
	CImage m_BannerMiddle;				// 标题区域中间横幅
	CMainTab * m_pTabCtrl;			// 对话框
};
