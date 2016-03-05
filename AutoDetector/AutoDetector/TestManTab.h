//*****************************************************************************
// �ļ��� : TestManTab.h
// 
// ����   : ���Թ���Ի����ϵı�ǩ�ؼ�
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-1       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
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
	// ժҪ��
	//	   ����뿪�¼���Ӧ
	// --------------------------------------------------
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

private:
	int	m_TabHeight;									// ��ǩ�߶�
	int m_TabWidth;										// ��ǩ���
	int m_nHoverItem;									// ��ǰ����ѡ���ǩ�ı���ʾΪ�»��ߣ�
	int m_nLastHover;									// ԭ����ѡ��

	bool m_bMouseTrack;									// ��ʶ�Ƿ�׷�ٵ�ǰ���״̬��֮����Ҫ��������
														// ��Ҫ��������Ѿ��ڴ���֮��ʱ��
														// һ�ƶ����Ͳ����ظ����� WM_MOUSEHOVER
};


