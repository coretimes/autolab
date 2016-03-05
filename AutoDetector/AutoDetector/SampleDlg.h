//*****************************************************************************
// 文件名 : SampleDlg.h
// 
// 类名   : 样本管理对话框
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
#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CSampleDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSampleDlg)

public:
	CSampleDlg(CWnd* pParent = NULL);					// 标准构造函数
	virtual ~CSampleDlg();

	enum { IDD = IDD_Sample_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStatic1();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	CCustomListCtrl m_ctrlReagentList;								// “测试配置”列表控件
	HBRUSH m_hCtlBrush;												// 控件背景画刷	

};
