//*****************************************************************************
// 文件名 : SampleConfigDlg.h
// 
// 类名   : 样本测试配置对话框类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-11       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************
#include "CustomListCtrl.h"
#include "SampleConfigListItem.h"
#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "Types.h"

#pragma once
class CSampleConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CSampleConfigDlg)

public:
	// ---------------------------------------------------------
	// 函数介绍：
	//     构造函数
	// 参数：
	//     1.IsAddSamples - 是否添加新样本测试标志：
	//                      ture添加新样本,false是修改样本测试
	//     2.SampleConfigNodes - 传入的样本节点指针数组
	//     3.ReagentConfigNode - 传入的试剂节点指针数组
	// ---------------------------------------------------------
	CSampleConfigDlg(const bool &IsAddSamples,
					 map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
					 map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
					 CWnd* pParent = NULL);

	virtual ~CSampleConfigDlg();

	enum { IDD = IDD_SampleConfig_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	// ---------------------------------------------------------
	// 函数介绍：
	//     “确认”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedOk();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “取消”按钮单击响应
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCancle();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “编号前缀”编辑框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueNumprefix();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “序号”组合框选项发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxSerials();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “类型”组合框选项和内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxType();
	afx_msg void OnCbnEditchangeComboxType();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “稀释倍数”编辑框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueDilution();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “稀释倍数”微调按钮单击响应函数
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinDilution(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “送检单位”编辑框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueDept();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “送检人”编辑框内容发生变化响应
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueApplicant();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “项目列表”单击响应函数
	// ---------------------------------------------------------
	afx_msg void OnLbnSelchangeListboxProject();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “项目列表”check变化响应函数
	// ---------------------------------------------------------
	afx_msg void OnProjectChkChange();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “送检日期”变化响应函数
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateApplydate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “设置理论浓度”复选框响应函数
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCheckboxSettheoryconc();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “重复次数”编辑框响应函数
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditRepeation();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “重复次数”微调按钮响应函数
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “起始浓度”微调按钮响应函数
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinStartconc(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “浓度增量”微调按钮响应函数
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinIncreament(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “起始浓度”编辑框响应函数
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditStartconc();

	// ---------------------------------------------------------
	// 函数介绍：
	//     “起浓度增量”编辑框响应函数
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditIncreament();

private:
	// ---------------------------------------------------------
	// 函数介绍：
	//     初始化样本编号函数
	// ---------------------------------------------------------
	void _InitSampleNo();

	// ---------------------------------------------------------
	// 函数介绍：
	//     添加新项目
	// ---------------------------------------------------------
	void _AddNewProject(const int &newProjecIndex);

	// ---------------------------------------------------------
	// 函数介绍：
	//     删除项目
	// ---------------------------------------------------------
	void _DeleteProject(const int &newProjecIndex);

	// ---------------------------------------------------------
	// 函数介绍：
	//     刷新查询列表
	// ---------------------------------------------------------
	void _RefreshSearchList();

	// ---------------------------------------------------------
	// 函数介绍：
	//     自动填充或删除理论浓度
	// 参数：
	//     IsAdd-如果是true，则自动填充；如果是false则删除理论浓度
	// ---------------------------------------------------------
	void _AutoFillThoeryConc(bool IsAdd = true);

	// ---------------------------------------------------------
	// 函数介绍：
	//     取得指定索引的项目的浓度单位
	// ---------------------------------------------------------
	CString _GetUnitConc(int idxProject);

	// ---------------------------------------------------------
	// 函数介绍：
	//     数据由列表ListItem流向样本区配置节点ConfigNode
	// ---------------------------------------------------------
	void _ListToConfig(const CSampleConfigListItem *pItem,SampleConfigNode *pNode);

	// ---------------------------------------------------------
	// 函数介绍：
	//     数据由样本区配置节点ConfigNode流向列表ListItem
	// ---------------------------------------------------------
	void _ConfigToList(const SampleConfigNode *pNode,vector <CSampleConfigListItem*> & ItemList);
private:
	CCustomListCtrl m_ctrlConfigList;								// “测试配置”列表控件
	CEdit m_ctrlEditNumPrefix;										// “编号前缀”编辑框
	CComboBox m_ctrlCombSerials;									// “序号”组合框
	CComboBox m_ctrlCombTypes;										// “样本类型”组合框
	CEdit m_ctrlEditDilution;										// “稀释度”编辑框
	CSpinButtonCtrl m_ctrlSpinBtnDilution;							// “稀释度”微调按钮
	CEdit m_ctrlEditDept;											// “送检单位”编辑框
	CEdit m_ctrlEditApplicant;										// “送检人”编辑框
	CDateTimeCtrl m_ctrlDateApply;									// “送检日期”日期选择控件
	CButton m_CheckSetDefault;										// “设置为默认值”复选框按钮
	CCheckListBox m_ctrlListProject;								// “检测项目”组合框
	CEdit m_ctrlEditRepeations;										// “重复次数”编辑框
	CSpinButtonCtrl m_ctrlSpinBtnRepeations;						// “重复次数”微调按钮
	CButton m_CheckSetThoeryConc;									// “设置理论浓度”复选框按钮
	CEdit m_ctrlEditStartConc;										// “起始浓度”编辑框
	CSpinButtonCtrl m_ctrlSpinBtnStartConc;							// “起始浓度”微调按钮
	CEdit m_ctrlEditIncreament;										// “增量”编辑框
	CSpinButtonCtrl m_ctrlSpinBtnIncreament;						// “增量”微调按钮

	map<Reagent_Pos,const ReagentConfigNode *> &m_RegentConfigNodes;// 试剂区试剂配置节点信息，Reagent_Pos是试剂区位置编号ReagentConfigNode*指向试剂区数据
	map<Sample_Pos,SampleConfigNode *> &m_SampleConfigNodes;		// 样本区样本配置节点信息，Sample_Pos是样本区位置编号
	bool m_IsAddSamples;											// 是否是添加新样本测试标志

	multimap<Sample_Pos,CSampleConfigListItem*> m_mapListItemData;	// 样本配置列表项数据数组，uint16是样本区位置编号
	multimap<int/*idxProjec*/,int/*idxListItem*/> m_mapSearchList;	// 样本配置列表项数据数组查询表，前一个int是项目组合框索引，后一个int是配置列表索引
	int m_nCountOfConfigNode;										// 样本配置节点的个数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
