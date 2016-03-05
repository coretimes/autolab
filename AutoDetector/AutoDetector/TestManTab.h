//*****************************************************************************
// 文件名 : TestManTab.h
// 
// 类名   : 测试管理对话框上的标签控件
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-1       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
// CTestManTab
enum TabView
{
	ViewReagentArea = 0,
	ViewSampleArea,
	ViewTestConfirm,
	ViewMonitor,
	NumTabs
};

class CTestManTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CTestManTab)

public:
	CTestManTab();
	virtual ~CTestManTab();
	int GetHeaderWidth()
	{
		return m_TabWidth;
	}
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	   鼠标离开事件响应
	// --------------------------------------------------
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

private:
	int	m_TabHeight;									// 标签高度
	int m_TabWidth;										// 标签宽度
	int m_nHoverItem;									// 当前焦点选项（标签文本显示为下划线）
	int m_nLastHover;									// 原焦点选项

	bool m_bMouseTrack;									// 标识是否追踪当前鼠标状态，之所以要这样定义
														// 是要避免鼠标已经在窗体之上时，
														// 一移动鼠标就不断重复产生 WM_MOUSEHOVER
};


