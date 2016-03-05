//*****************************************************************************
// 文件名 : StanderdDlg.h
// 
// 类名   : 标准品管理对话框类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-02      崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "BaseDialog.h"
#include "CustomListCtrl.h"
#include "StandardTable.h"

class CStanderdDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CStanderdDlg)

public:
	CStanderdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStanderdDlg();

// 对话框数据
	enum { IDD = IDD_Standerd_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	// ---------------------------------------------------------
	// 摘要：
	//     “添加”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedAddrecordButton();

	// ---------------------------------------------------------
	// 摘要：
	//     “检测项目”组合框选项发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboPojectname();

	// ---------------------------------------------------------
	// 摘要：
	//     “生产起始日期”控件日期发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateProductfrom(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “生产终止日期”控件日期发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateProductto(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “删除”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedDeleterecordButton();

	// ---------------------------------------------------------
	// 摘要：
	//     “修改”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedModifyButton();

	// ---------------------------------------------------------
	// 摘要：
	//     “控件背景色”取得消息响应
	// ---------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// ---------------------------------------------------------
	// 摘要：
	//     初始化“项目检测名称”组合框
	// ---------------------------------------------------------
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// ---------------------------------------------------------
	// 摘要：
	//     标准品选项发生变化
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListStanderd(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     初始化“项目检测名称”组合框
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“标准品列表”，根据新条件重新搜索
	// ---------------------------------------------------------
	void _RefreshStandardsList();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“详细信息”区
	// ---------------------------------------------------------
	void _RefreshDetails();

protected:
	CCustomListCtrl m_ctrlStandardsList;							// “标准品”列表控件

	CComboBox m_ctrlProjectName;									// “检测项目”组合框
	vector<ProjectFullName> m_AllProjectNameList;					// “所有项目名称”列表
	CDateTimeCtrl m_ctrlProductDateFrom;							// “生产日期”起始限制日期
	CDateTimeCtrl m_ctrlProductDateTo;								// “生产日期”终止限制日期
	Standard_Key m_nCurrentSelectedStanderd;						//  当前选中标准品ID
	CStatic m_ctrlStdPointTitle[10];								// “标准点”索引标题
	CStatic m_ctrlStdPointValue[10];								// “标准点”浓度值

	HBRUSH m_hCtlBrush;												// 控件背景画刷	
};
