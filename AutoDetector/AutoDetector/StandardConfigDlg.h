//*****************************************************************************
// 文件名 : StandardConfigDlg.h
// 
// 类名   : 标准品测试配置对话框类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "CustomListCtrl.h"
#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "StandardTable.h"
#include "StandardConfigListItem.h"

using namespace std;

class CStandardConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CStandardConfigDlg)

public:
	// ---------------------------------------------------------
	// 函数介绍：
	//     根据指定的“样本配置节点”数组构造函数
	// 参数：
	//     1.IsAddStanderds - 是否添加新标准品测试标志：
	//                      ture添加新标准品,false是修改标准品测试
	//     2.SampleConfigNodes - 传入的样本节点指针数组
	//     3.ReagentConfigNode - 传入的试剂节点指针数组
	// ---------------------------------------------------------
	CStandardConfigDlg(	const bool &IsAddStanderds,
						map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
						map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
						CWnd* pParent = NULL);

	virtual ~CStandardConfigDlg();
	enum { IDD = IDD_StandardConfig_Dlg };
	virtual BOOL OnInitDialog();

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

	// ---------------------------------------------------------
	// 函数介绍：
	//     “确认”按钮单击响应函数
	// ---------------------------------------------------------
	afx_msg void OnBnClickedOk();

	// ---------------------------------------------------------
	// 函数介绍：
	//    “ 取消”按钮单击响应函数
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCancel();

	// ---------------------------------------------------------
	// 函数介绍：
	//    “检测项目”组合框内容变化响应函数
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxTestproject();

	// ---------------------------------------------------------
	// 函数介绍：
	//    “批号”组合框内容变化响应函数
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxBatchnum();

	// ---------------------------------------------------------
	// 函数介绍：
	//    旋转按钮位置变化消息响应函数
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//    重复次数编辑框值发生变化时的响应函数
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditRepeation();

	// ---------------------------------------------------------
	// 函数介绍：
	//    初始化标准品信息控件
	// ---------------------------------------------------------
	void _InitSampleInfoControls();

	// ---------------------------------------------------------
	// 函数介绍：
	//    设置当前标准品信息显示
	// ---------------------------------------------------------
	void _DisplayCurrentStanderd(const StandardRecord &record);

	// ---------------------------------------------------------
	// 函数介绍：
	//     数据由列表ListItem流向样本区配置节点ConfigNode
	// ---------------------------------------------------------
	void _ListToConfig(const CStandardConfigListItem *pItem,SampleConfigNode *pNode);

	// ---------------------------------------------------------
	// 函数介绍：
	//     数据由样本区配置节点ConfigNode流向列表ListItem
	// ---------------------------------------------------------
	void _ConfigToList(const SampleConfigNode *pNode,CStandardConfigListItem * pItem);

private:
	CCustomListCtrl m_ctrlConfigList;								// 测试配置列表控件
	CComboBox m_ComboxProjectName;									// “检测项目”组合框控件
	CComboBox m_ComboxBatchNum;										// “批号”组合框控件
	CEdit m_EditRepeation;											// “重复次数”编辑框控件
	CSpinButtonCtrl m_SpinRepeation;								// “重复次数”微调按钮控件
	CStatic m_ctrlStdPointTitle[10];								// “标准点”索引标题
	CStatic m_ctrlStdPointValue[10];								// “标准点”浓度值

	map<Reagent_Pos,const ReagentConfigNode *> m_mapRegentConfigNodes;	// 试剂区试剂配置节点信息，uint16是试剂区位置编号
	map<Sample_Pos,SampleConfigNode *> &m_mapSampleConfigNodes;			// 样本区样本配置节点信息，uint16是样本区位置编号
	bool m_IsAddStanderds;												// 是否添加新标准品测试标志，ture：

	vector<CStandardConfigListItem*> m_vctListItemData;					// 测试配置列表项数据数组,动态数组
	int m_nCountOfConfigNode;											// 样本配置节点的个数
	int m_nNumOfValidConfigNode;										// 有效样本配置节点的个数：“标准点个数”与“选择的配置节点个数”之间较小的数值

	vector<StandardRecord> m_vctStandardRecords;						// 当前检测项目名称下，所有标准品记录信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
