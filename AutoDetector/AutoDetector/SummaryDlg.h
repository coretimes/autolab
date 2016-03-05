//*****************************************************************************
// 文件名 : SummaryDlg.h
// 
// 类名   : 概要对话框
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
// 对话框上显示样本区控件、孵育区控件、试剂区控件等控件；
// 显示仪器各区域主要参数信息
//*****************************************************************************
#pragma once

#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "BaseDialog.h"
#include "IncuAreaCtrl.h"

class CSummaryDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSummaryDlg)

public:
	// --------------------------------------------------------------
	// 构造函数：
	//	   父对话框不能使用默认值为NULL，因为只有为非空构造时，在CBaseDialo构造函数中
	//     才能设置标题栏高度为0
	// --------------------------------------------------------------
	CSummaryDlg(CWnd* pParent);
	virtual ~CSummaryDlg();

	enum { IDD = IDD_Summary_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	// --------------------------------------------------
	// 摘要：
	//	   “样本区数据值”发生变化响应函数
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CSampleAreaCtrl m_ctrlSampleArea;			// 样本区控件
	CReagentAreaCtrl m_ctrlReagentArea;			// 试剂区控件
	CIncuAreaCtrl m_ctrlIncuArea;				// 试剂区控件
};
