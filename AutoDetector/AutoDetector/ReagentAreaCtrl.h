//*****************************************************************************
// 文件名 : ReagentAreaCtrl.h
// 
// 类名   : 试剂区示意图控件
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 1.构成：
//	 试剂区示意图控件由标题区、节点区（15个节点）及其他区域组成，每个节点的属性是一个uint32类型试剂ID；
//   节点属性值保存于m_arrReagents[15]中，对应索引是(pos - 1)
//*****************************************************************************

#pragma once
#include "ReagentTable.h"
#include "ReagentAreaData.h"
#include "GeneralFuction.h"

//*******************************************************
// 控件类型
// 在“概况”窗口的控件，作用仅仅是为了显示，不能用来配置
// 在“测试”窗口的控件，作用可用来配置
//*******************************************************
enum RAControlType
{
	raForConfig = 0,
	raForShowing
};

//*********************************
// 试剂区示意图控件类
//*********************************
class CReagentAreaCtrl : public CStatic
{
	DECLARE_DYNAMIC(CReagentAreaCtrl)

public:
	CReagentAreaCtrl(const RAControlType & ctType);
	virtual ~CReagentAreaCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
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
	//	   鼠标右键按下事件响应
	// --------------------------------------------------
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	   窗口大小发生改变消息响应
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// 摘要：
	//	   CToolTipCtrl 控件的的TTN_NEEDTEX消息响应，取得提示文本
	// --------------------------------------------------
	afx_msg BOOL OnToolTipNotify(UINT id,NMHDR * pTTTStruct,LRESULT * pResult);

	// --------------------------------------------------
	// 摘要：
	//	   “修改试剂配置”菜单事件响应
	// --------------------------------------------------
	afx_msg void OnClickMenuReagentSelect();

	// --------------------------------------------------
	// 摘要：
	//	   “删除试剂”菜单事件响应
	// --------------------------------------------------
	afx_msg void OnClickMenuReagentDelete();

	// --------------------------------------------------
	// 摘要：
	//	   绘制控件主体图
	// --------------------------------------------------
	void _DrawFrame(CDC *pDC);

	// --------------------------------------------------
	// 摘要：
	//	   绘制各节点
	// 参数：
	//     1.pos - 试剂位置编号
	//     2.pDC - 设备环境
	// --------------------------------------------------
	void _DrawTubes(const Reagent_Pos pos,CDC *pDC);

	// --------------------------------------------------
	// 摘要：
	//	   绘制小试管
	// 参数：
	//     1.rect - 绘制区域
	//     2.pDC - 设备环境
	// --------------------------------------------------
	void _DrawSmallTube(const CRect &rect,CDC *pDC);

	// --------------------------------------------------
	// 摘要：
	//	   绘制大试管
	// 参数：
	//     1.rect - 绘制区域
	//     2.pDC - 设备环境
	// --------------------------------------------------
	void _DrawBigTube(const CRect &rect,CDC *pDC);

	// --------------------------------------------------
	// 摘要：
	//	   取得试剂位矩形
	// 参数：
	//     1.pos - 试剂位置编号
	// --------------------------------------------------
	CRect _GetPosRect(const Reagent_Pos &pos) const;

	// --------------------------------------------------
	// 摘要：
	//	   使某指定试剂位和对应标题节点显示无效，并重新绘制
	// 参数：
	//     1.pos - 试剂位置编号
	// --------------------------------------------------
	void _InvalidPos(const Reagent_Pos &pos);

	// --------------------------------------------------
	// 摘要：
	//	   根据像素坐标判断位置坐标 
	// 参数：
	//     1.pt - 像素坐标点
	//     2.pos - 位置编号
	// --------------------------------------------------
	void _WhichNode(const CPoint & pt, Reagent_Pos &pos) const;

	// --------------------------------------------------
	// 摘要：
	//	   取得试剂位标题矩形
	// 参数：
	//     1.pos - 试剂位置编号
	// --------------------------------------------------
	CRect _GetHeaderRect(const Reagent_Pos &pos) const;

	// --------------------------------------------------
	// 摘要：
	//	   更新指定节点的标准品提示文本
	// --------------------------------------------------
	BOOL _UpdateReagentTipText(const ReagentConfigNode * pNode);

	// --------------------------------------------------
	// 摘要：
	//	   显示菜单函数
	// --------------------------------------------------
	void _DisplayMenu();

private:
	CRect m_rcClient;									// 控件节点区域矩形
	CRect m_rcNodesRegion;								// 控件节点区域矩形
	CRect m_rcTitleRegion;								// 控件标题区域矩形

	Reagent_Pos m_HoverPos;								// 原焦点位置

	bool m_bMouseTrack;									// 标识是否追踪当前鼠标状态，之所以要这样定义
														// 是要避免鼠标已经在窗体之上时，
														// 一移动鼠标就不断重复产生 WM_MOUSEHOVER
	HICON m_hIconPlus;									// 加号图片
	HICON m_hIconPlusDisable;							// 加号灰色图片
	CImage m_PlusPress;									// 加号按下时的图片（PNG类型）
	BOOL m_bIsLButtonDown;								// 鼠标左键是否按下
	CToolTipCtrl * m_pToolTip;							// 动态提示控件
	map<Reagent_Pos,CString> m_mapToolTipTexts;			// 保存每个节点的提示文本
	CMenu * m_pMenu;									// 菜单指针

private:
	RAControlType m_ControlType;						// 在“概况”窗口的“样本区控件”只起到提示作用，不允许选择和修改等
														// 在“测试”窗口中的“样本区控件”允许选择修改

private:
	CReagentAreaData * m_pReagentData;
};
