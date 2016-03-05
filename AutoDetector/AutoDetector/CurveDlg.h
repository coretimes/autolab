//*****************************************************************************
// 文件名 : CurveDlg.h
// 
// 类名   : 标准曲线管理对话框
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-2       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CCurveDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurveDlg();

// 对话框数据
	enum { IDD = IDD_Curve_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ---------------------------------------------------------
	// 摘要：
	//     “项目检测名称”组合框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboPojectname();

	// ---------------------------------------------------------
	// 摘要：
	//     “批号”组合框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboBatchnum();

	// ---------------------------------------------------------
	// 摘要：
	//     “定标起始日期”内容发生变化
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateTestdatefrom(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “定标终止日期”内容发生变化
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateTestdate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “曲线列表选项”发生变化
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListCurve(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	// ---------------------------------------------------------
	// 摘要：
	//     初始化“项目检测名称”组合框
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// 摘要：
	//     初始化“批号”组合框
	// ---------------------------------------------------------
	void _InitComboBatchNums();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“曲线列表”，根据新条件重新搜索
	// ---------------------------------------------------------
	void _RefreshCurveList();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“曲线详细信息”
	// ---------------------------------------------------------
	void _RefreshDetails();

protected:
	CCustomListCtrl m_ctrlCurveList;								// “曲线列表”控件
	CCustomListCtrl m_ctrlDetailsList;								// “曲线详细信息”控件

	CComboBox m_ctrlProjectName;									// “检测项目”组合框
	CComboBox m_ctrlBatchNum;										// “批号”组合框
	vector<ProjectFullName> m_AllProjectNameList;					// “所有项目名称”列表
	vector<CString> m_AllBatchNumList;								// “所有批号”列表	
	CDateTimeCtrl m_ctrlTestDateFrom;								// “定标日期”起始限制日期
	CDateTimeCtrl m_ctrlTestDateTo;									// “定标日期”终止限制日期
	Curve_Key m_nCurrentSelectedCurve;								//  当前选中曲线ID

	HBRUSH m_hCtlBrush;												// 控件背景画刷	
public:
};
