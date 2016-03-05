//*****************************************************************************
// 文件名 : ReagentSelectDlg.h
// 
// 类名   : 试剂选择对话框类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-01      崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "CustomListCtrl.h"
#include "ReagentTable.h"

class CReagentSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CReagentSelectDlg)

public:

	// ---------------------------------------------------------
	// 摘要：
	//		构造函数
	// 参数：
	//		1.KeyBuffer - 用来输出最后选择的试剂ID
	// ---------------------------------------------------------
	CReagentSelectDlg(Reagent_Key & KeyBuffer);   
	virtual ~CReagentSelectDlg();

	enum { IDD = IDD_ReagentSelect_Dlg };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// ---------------------------------------------------------
	// 摘要：
	//     “取消”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCancel();

	// ---------------------------------------------------------
	// 摘要：
	//     “确认”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedOk();

	// ---------------------------------------------------------
	// 摘要：
	//     “检测项目”组合框选项发生变化
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboPojectname();

	// ---------------------------------------------------------
	// 摘要：
	//     “生产日期起始日期”控件数据发生变化
	// ---------------------------------------------------------
	afx_msg void OnDtnDateChProductDateFrom(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “生产日期终止日期”控件数据发生变化
	// ---------------------------------------------------------
	afx_msg void OnDtnDateChProductDateTo(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 摘要：
	//     “隐藏过期试剂”复选框单击事件响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCheckHideexpired();

	// ---------------------------------------------------------
	// 摘要：
	//     “试剂列表”控件项发生变化消息响应函数
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListReagents(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
private:
	// ---------------------------------------------------------
	// 摘要：
	//     初始化“项目检测名称”组合框
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“试剂列表”，根据新条件重新搜索
	// ---------------------------------------------------------
	void _RefreshReagentsList();

	// ---------------------------------------------------------
	// 摘要：
	//     刷新“详细信息”区
	// ---------------------------------------------------------
	void _RefreshDetails();

private:
	CCustomListCtrl m_ctrlReagentList;								// “测试配置”列表控件

	CComboBox m_ctrlProjectName;									// “检测项目”组合框
	vector<ProjectFullName> m_AllProjectNameList;							// “所有项目名称”列表
	CDateTimeCtrl m_ctrlProductDateFrom;							// “生产日期”起始限制日期
	CDateTimeCtrl m_ctrlProductDateTo;								// “生产日期”终止限制日期
	Reagent_Key & m_nCurrentSelectedReagent;						//  当前选中试剂ID
};
