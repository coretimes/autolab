//*****************************************************************************
// 文件名 : ReagentAreaManDlg.h
// 
// 类名   : 试剂区配置对话框
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

#include "BaseDialog.h"
#include "ReagentAreaCtrl.h"
#include "CustomListCtrl.h"

class CReagentAreaManDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CReagentAreaManDlg)

public:
	// --------------------------------------------------------------
	// 构造函数：
	//	   父对话框不能使用默认值为NULL，因为只有为非空构造时，在CBaseDialo构造函数中
	//     才能设置标题栏高度为0
	// --------------------------------------------------------------
	CReagentAreaManDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CReagentAreaManDlg();
	enum { IDD = IDD_ReagentAreaMan_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnReagentAreaDataUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	CCustomListCtrl m_ctrlReagentList;								// 测试配置列表控件
	CReagentAreaCtrl m_ctrlReagentArea;
};
