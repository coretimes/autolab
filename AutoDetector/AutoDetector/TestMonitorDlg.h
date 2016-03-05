//*****************************************************************************
// 文件名 : TestMonitorDlg.h
// 
// 类名   : 测试监控对话框
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-02       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
//	   实时监控测试状态
//*****************************************************************************
#pragma once

#include "BaseDialog.h"
#include "CustomListCtrl.h"
#include <map>

class CTestMonitorDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CTestMonitorDlg)

public:
	// --------------------------------------------------------------
	// 构造函数：
	//	   父对话框不能使用默认值为NULL，因为只有为非空构造时，在CBaseDialo构造函数中
	//     才能设置标题栏高度为0
	// --------------------------------------------------------------
	CTestMonitorDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CTestMonitorDlg();
	enum { IDD = IDD_TestMonitor_Dlg };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------
	// 摘要：
	//	   “测试缓冲区”数据发生变化响应函数
	// --------------------------------------------------
	afx_msg LRESULT OnTestBufferUpdate(WPARAM wParam, LPARAM lParam);
	
	// --------------------------------------------------
	// 摘要：
	//	   刷新查找表
	// --------------------------------------------------
	void RefreshSearchMap();

private:
	CCustomListCtrl m_ctrlMornitorList;							// 测试监控列表
	multimap<TestUnit_ID,int> m_SearchMap;						// 根据测试单元ID，定位列表项的查找表<测试单元ID，列表项索引>
};
