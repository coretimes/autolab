//*****************************************************************************
// 文件名 : TestDlg.h
// 
// 类名   : 测试管理对话框
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
#include "TestManTab.h"

class CTestDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CTestDlg();
	enum { IDD = IDD_Test_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// ---------------------------------------------------------
	// 摘要：
	//     “标签项”选择变化响应
	// ---------------------------------------------------------
	afx_msg void OnTcnSelchangeTestmanTab(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CTestManTab m_TestManTab;
	map<TabView,CDialog *>	m_mapChildViews;	// 子对话框指针
};
