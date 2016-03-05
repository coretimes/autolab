//*****************************************************************************
// 文件名 : SampleAreaManDlg.h
// 
// 类名   : 样本区配置对话框
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "SampleAreaCtrl.h"
#include "BaseDialog.h"
#include "CustomListCtrl.h"

// 列表滚动延迟时间计时器
// 说明：当鼠标在“样本区控件”移动时，焦点一直在灵活变化。列表控件上焦点也相应在变化。
//       此时，反复移动滚动条，会明显闪烁。所以，滚动条移动延迟300毫秒钟。
#define  TID_ListScroll_Delay			00001

class CSampleAreaManDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSampleAreaManDlg)

public:
	// --------------------------------------------------------------
	// 构造函数：
	//	   父对话框不能使用默认值为NULL，因为只有为非空构造时，在CBaseDialo构造函数中
	//     才能设置标题栏高度为0
	// --------------------------------------------------------------
	CSampleAreaManDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CSampleAreaManDlg();

	enum { IDD = IDD_Test_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// --------------------------------------------------
	// 摘要：
	//	   “样本区数据状态”发生变化响应函数
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaStateChange(WPARAM wParam, LPARAM lParam);

	// --------------------------------------------------
	// 摘要：
	//	   “样本区数据值”发生变化响应函数
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CSampleAreaCtrl m_ctrlSampleArea;					// 样本区控件
	CCustomListCtrl m_ctrlSampleList;					// 样本区数据列表控件
	
	Sample_Pos m_UpdatePos;								// 发生改变的样本位
};
