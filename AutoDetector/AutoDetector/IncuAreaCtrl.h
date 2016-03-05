//*****************************************************************************
// 文件名 : CIncuAreaCtrl.h
// 
// 类名   : 孵育区示意图控件
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 1.构成：
//	 试剂区示意图控件由标题区、节点区（15个节点）及其他区域组成，每个节点的属性是一个uint32类型试剂ID；
//   节点属性值保存于m_arrIncus[15]中，对应索引是(pos - 1)
//*****************************************************************************
#pragma once
#include "Types.h"
#define  NUM_IncuPOS		15					// 孵育区容纳试剂个数
struct IncuConfigNode
{
	//Incu_Pos pos;								// 孵育区位置编号
	bool IsThereTube;							// 是否有试管
};

//*********************************
// 试剂区示意图控件类
//*********************************
class CIncuAreaCtrl : public CStatic
{
	DECLARE_DYNAMIC(CIncuAreaCtrl)

public:
	CIncuAreaCtrl();
	virtual ~CIncuAreaCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	// --------------------------------------------------
	// 摘要：
	//	   鼠标移动事件响应
	// --------------------------------------------------
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	   鼠标左键按下事件响应
	// --------------------------------------------------
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	   窗口大小发生改变消息响应
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// 摘要：
	//	   擦除背景消息响应
	// --------------------------------------------------
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------
	// 摘要：
	//	   绘制控件主体图
	// --------------------------------------------------
	void _DrawFrame(CDC *pDC);

private:
	CRect m_rcClient;									// 控件节点区域矩形
	CRect m_rcNodesRegion;								// 控件节点区域矩形
	CRect m_rcTitleRegion;								// 控件标题区域矩形

private: 
	static IncuConfigNode staticIncuConfigList[NUM_IncuPOS];
};
