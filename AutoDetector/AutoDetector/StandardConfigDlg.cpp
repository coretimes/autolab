// StandardConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "StandardConfigDlg.h"
#include "StandardConfigListItem.h"
#include "ReagentTable.h"

// CStandardConfigDlg dialog

IMPLEMENT_DYNAMIC(CStandardConfigDlg, CDialog)

CStandardConfigDlg::CStandardConfigDlg( const bool &IsAddStanderds,
									    map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
										map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
										CWnd* pParent /*=NULL*/)
										:
										m_IsAddStanderds(IsAddStanderds),
										m_mapSampleConfigNodes(SampleConfigNodes),
										m_mapRegentConfigNodes(RegentConfigNodes),
										CDialog(CStandardConfigDlg::IDD, pParent)
{
	// 样本配置节点个数初始化
	m_nCountOfConfigNode = (int)m_mapSampleConfigNodes.size();
}

CStandardConfigDlg::~CStandardConfigDlg()
{
}

void CStandardConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_List_TestConfig,m_ctrlConfigList);
	DDX_Control(pDX, IDC_ComBox_TestProject,m_ComboxProjectName);
	DDX_Control(pDX, IDC_ComBox_BatchNum,m_ComboxBatchNum);
	DDX_Control(pDX, IDC_Edit_Repeation,m_EditRepeation);
	DDX_Control(pDX, IDC_Spin_Repeation,m_SpinRepeation);

	DDX_Control(pDX, IDC_Title_PointA,m_ctrlStdPointTitle[0]);
	DDX_Control(pDX, IDC_Title_PointB,m_ctrlStdPointTitle[1]);
	DDX_Control(pDX, IDC_Title_PointC,m_ctrlStdPointTitle[2]);
	DDX_Control(pDX, IDC_Title_PointD,m_ctrlStdPointTitle[3]);
	DDX_Control(pDX, IDC_Title_PointE,m_ctrlStdPointTitle[4]);
	DDX_Control(pDX, IDC_Title_PointF,m_ctrlStdPointTitle[5]);
	DDX_Control(pDX, IDC_Title_PointG,m_ctrlStdPointTitle[6]);
	DDX_Control(pDX, IDC_Title_PointH,m_ctrlStdPointTitle[7]);
	DDX_Control(pDX, IDC_Title_PointI,m_ctrlStdPointTitle[8]);
	DDX_Control(pDX, IDC_Title_PointJ,m_ctrlStdPointTitle[9]);

	DDX_Control(pDX, IDC_Value_PointA,m_ctrlStdPointValue[0]);
	DDX_Control(pDX, IDC_Value_PointB,m_ctrlStdPointValue[1]);
	DDX_Control(pDX, IDC_Value_PointC,m_ctrlStdPointValue[2]);
	DDX_Control(pDX, IDC_Value_PointD,m_ctrlStdPointValue[3]);
	DDX_Control(pDX, IDC_Value_PointE,m_ctrlStdPointValue[4]);
	DDX_Control(pDX, IDC_Value_PointF,m_ctrlStdPointValue[5]);
	DDX_Control(pDX, IDC_Value_PointG,m_ctrlStdPointValue[6]);
	DDX_Control(pDX, IDC_Value_PointH,m_ctrlStdPointValue[7]);
	DDX_Control(pDX, IDC_Value_PointI,m_ctrlStdPointValue[8]);
	DDX_Control(pDX, IDC_Value_PointJ,m_ctrlStdPointValue[9]);
}

BEGIN_MESSAGE_MAP(CStandardConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &CStandardConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CStandardConfigDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_ComBox_TestProject, &CStandardConfigDlg::OnCbnSelchangeComboxTestproject)
	ON_CBN_SELCHANGE(IDC_ComBox_BatchNum, &CStandardConfigDlg::OnCbnSelchangeComboxBatchnum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_Repeation, &CStandardConfigDlg::OnDeltaposSpinRepeation)
	ON_EN_CHANGE(IDC_Edit_Repeation, &CStandardConfigDlg::OnEnChangeEditRepeation)
END_MESSAGE_MAP()

BOOL CStandardConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 各控件初始化

	//标准品信息控件初始化
	_InitSampleInfoControls();

	// 检测项目组合框初始化
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_mapRegentConfigNodes.begin();
	int index = 0;
	while(iterReagent != m_mapRegentConfigNodes.end())
	{
		// 取得该试剂ID的试剂详细信息
		ReagentRecord regentInfo;
		regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
		
		// 在组合框末尾添加该试剂检测项目名称
		CString temp;
		temp.Format(_T("%d .%s(%s)"),iterReagent->first,regentInfo.ShortName,regentInfo.ProjectName);
		m_ComboxProjectName.InsertString(index,temp);
		
		// 组合框该Item的数据指向试剂区位置
		m_ComboxProjectName.SetItemData(index,static_cast<DWORD_PTR>(iterReagent->second->pos));
		
		// 如果不是“新添加”的测试，即是“修改”的测试
		if(m_IsAddStanderds == false)
		{
			SampleConfigNode * pSampleConfigNode = m_mapSampleConfigNodes.begin()->second;
			SampleConfigNode::TestProjInfo pProjInfo = pSampleConfigNode->TestProjArr.begin()->second;
			if(pProjInfo.ReagentPos == iterReagent->second->pos)
			{
				m_ComboxProjectName.SetCurSel(index);
				OnCbnSelchangeComboxTestproject();
			}
		}
		index++;
		iterReagent++;
	}

	// 测试配置列表初始化
	m_ctrlConfigList.InsertColumn(colPos,_T("坐标"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colBatchNum,_T("标准品批号"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colConcIndex,_T("标准点"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colRepeat,_T("测光次数"),LVCFMT_CENTER);

	m_ctrlConfigList.SetColumnWidth(colPos,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlConfigList.SetColumnWidth(colProject,130);
	m_ctrlConfigList.SetColumnWidth(colBatchNum,110);
	m_ctrlConfigList.SetColumnWidth(colConcIndex,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlConfigList.SetColumnWidth(colRepeat,LVSCW_AUTOSIZE_USEHEADER);

	m_ctrlConfigList.SetItemHeight(24);
	m_ctrlConfigList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlConfigList.m_SelectedBackColor = RGB(230, 241, 255);

	// 重复次数控件
	m_SpinRepeation.SetBuddy((CWnd*)&m_EditRepeation);
	
	return TRUE;
}

void CStandardConfigDlg::_InitSampleInfoControls()
{
	// 标准品属性值显示控件
	CWnd * pWnd;
	pWnd = GetDlgItem(IDC_Value_ProductDate);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_ExpirationDate);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_RatedTestTimes);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_RemainTimes);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_CountOfStdPoints);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(_T(""));

	// 标准点浓度控件初始化
	for(int i=0; i<10; i++)
	{
		m_ctrlStdPointValue[i].SetWindowText(_T("-"));
	}

	// 有效节点个数设为0
	m_nNumOfValidConfigNode = 0;

	// 标准品测试配置表表项数据
	m_ctrlConfigList.DeleteAllItemsAndDatas();
	map<Sample_Pos,SampleConfigNode *>::iterator iter = m_mapSampleConfigNodes.begin();
	int i = 0;
	m_vctListItemData.resize(m_mapSampleConfigNodes.size());
	while(iter != m_mapSampleConfigNodes.end())
	{
		CStandardConfigListItem * newItem = new CStandardConfigListItem();
		_ConfigToList(iter->second,newItem);
		m_vctListItemData[i] = newItem;
		m_ctrlConfigList.AddItem(i,static_cast<CListItemData *>(newItem));
		i++;
		iter++;
	}
}

void CStandardConfigDlg::OnBnClickedOk()
{
	// 遍历列表每一项
	for (int i=0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CStandardConfigListItem * pItem = reinterpret_cast<CStandardConfigListItem *> (m_ctrlConfigList.GetItemData(i));
		map<Sample_Pos,SampleConfigNode *>::iterator iterSampleConfigNode = m_mapSampleConfigNodes.find(pItem->GetPosNum());
		_ListToConfig(pItem,iterSampleConfigNode->second);
	}

	return OnOK();
}

void CStandardConfigDlg::OnBnClickedCancel()
{
	return OnCancel();
}

// ---------------------------------------------------------
// 函数介绍：
//    “检测项目”组合框内容变化响应函数
// ---------------------------------------------------------
void CStandardConfigDlg::OnCbnSelchangeComboxTestproject()
{
	// 检测项目组合框当前选择项索引
	int indexCurrentRegent = m_ComboxProjectName.GetCurSel();
	// 找出对应的试剂位置编号和试剂配置信息
	int ReagentPos = static_cast<int> (m_ComboxProjectName.GetItemData(indexCurrentRegent));
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_mapRegentConfigNodes.find(ReagentPos);

	// 利用试剂索引查找试剂信息
	ReagentRecord regentInfo;
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);

	// 利用试剂信息的项目名称查找该名称下所有的标准品
	CString strProjectName = regentInfo.ProjectName;
	CStandardTable * pStandardTable = CStandardTable::GetInstance();
	m_vctStandardRecords.clear();
	m_ComboxBatchNum.ResetContent();
	if( pStandardTable->GetStandardsByProjecName(m_vctStandardRecords, strProjectName) == false)
	{
		// 如果没有查询到标准品，则显示错误
		MessageBox(_T("数据库中没有此项目标准品或数据库错误！"),_T("数据错误"),MB_ICONERROR|MB_OK);
		_InitSampleInfoControls();
		return;
	}

	CStandardTable::SortByBatchNumAsc(m_vctStandardRecords);

	// 在组合框里显示所有标准品批号
	// 默认批号索引
	int DefaultIndex = -1;
	for(int i=0; i<(int)m_vctStandardRecords.size(); i++)
	{
		// 如果是新添加的测试，且标准品批号等于试剂批号，则该索引设置为默认
		if(m_IsAddStanderds == true && m_vctStandardRecords[i].BatchNum == regentInfo.BatchNum)
			DefaultIndex = i;

		// 如果是修改的测试，且标准品ID等于本ID，则该索引设置为默认
		if(m_IsAddStanderds == false && m_vctStandardRecords[i].ID == m_mapSampleConfigNodes.begin()->second->RecordID)
			DefaultIndex = i;

		m_ComboxBatchNum.InsertString(i,m_vctStandardRecords[i].BatchNum);
	}

	// 如果没有等于试剂批号的标准品批号，则最大批号设置为默认批号
	if(DefaultIndex < 0)
		DefaultIndex = (int)m_vctStandardRecords.size() - 1;
	m_ComboxBatchNum.SetCurSel(DefaultIndex);
	OnCbnSelchangeComboxBatchnum();
}

// ---------------------------------------------------------
// 函数介绍：
//    “批号”组合框内容变化响应函数
// ---------------------------------------------------------
void CStandardConfigDlg::OnCbnSelchangeComboxBatchnum()
{
	_InitSampleInfoControls();
	// 取得当前选择批号索引
	int indexCurrenBatchNum = m_ComboxBatchNum.GetCurSel();
	// 取得当前批号标准品记录
	_DisplayCurrentStanderd(m_vctStandardRecords[indexCurrenBatchNum]);
	if(m_IsAddStanderds == true)
	{
		m_EditRepeation.SetWindowText(_T("1"));
		OnEnChangeEditRepeation();
	}
	// 当第一次初始化完毕后，把此标志设置为新添加测试，以后的初始化与新添加测试初始化完全相同
	m_IsAddStanderds = true;
}

void CStandardConfigDlg::_DisplayCurrentStanderd(const StandardRecord &record)
{
	CString strTemp;
	COleDateTime tempDate;
	CWnd * pWnd;
	// 生产日期
	pWnd = GetDlgItem(IDC_Value_ProductDate);
	tempDate = record.ProductionDate;
	strTemp = tempDate.Format(_T("%Y-%m-%d"));
	pWnd->SetWindowText(strTemp);

	// 失效日期
	pWnd = GetDlgItem(IDC_Value_ExpirationDate);
	tempDate = record.ExpirationDate;
	strTemp = tempDate.Format(_T("%Y-%m-%d"));
	pWnd->SetWindowText(strTemp);

	// 额定次数
	pWnd = GetDlgItem(IDC_Value_RatedTestTimes);
	strTemp.Format(_T("%d"),record.RatedTestTimes);
	pWnd->SetWindowText(strTemp);

	// 剩余次数
	pWnd = GetDlgItem(IDC_Value_RemainTimes);
	strTemp.Format(_T("%d"),record.RemainTimes);
	pWnd->SetWindowText(strTemp);

	// 浓度单位
	pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(record.Unit);

	// 标准点个数
	pWnd = GetDlgItem(IDC_Value_CountOfStdPoints);
	strTemp.Format(_T("%d"),record.CountOfStdPoints);
	pWnd->SetWindowText(strTemp);

	// 先隐藏标注点5~10
	for (int i=0; i<10; i++)
	{
		m_ctrlStdPointTitle[i].ShowWindow(SW_HIDE);
		m_ctrlStdPointValue[i].ShowWindow(SW_HIDE);;
	}

	// 标准点
	for(int i = 0; i < record.CountOfStdPoints; i++)
	{
		int indexConc = i + 1;
		strTemp.Format(_T("%g"),record.StdConc[i]);
		m_ctrlStdPointValue[i].SetWindowText(strTemp);
		m_ctrlStdPointTitle[i].ShowWindow(SW_SHOW);
		m_ctrlStdPointValue[i].ShowWindow(SW_SHOW);
	}

	// 从“标准点个数”与“选择的配置节点个数”中选择较小的个数，刷新列表
	if(record.CountOfStdPoints < m_nCountOfConfigNode)
		m_nNumOfValidConfigNode = record.CountOfStdPoints;
	else 
		m_nNumOfValidConfigNode = m_nCountOfConfigNode;

	// 浓度点索引选项表
	vector<CString> arrOptionsConcIndex;
	for(int i=0; i < record.CountOfStdPoints; i++)
	{
		// 数组索引    浓度点索引      字符
		// 0        ->      1      ->    A
		// 1        ->      2      ->    B
		// ... ...
		CString strTemp;
		strTemp.Format(_T("%c"),(i+1) + 64);		// 数组索引从0起始，而浓度点索引从1开始，所以应该是i+1
		arrOptionsConcIndex.push_back(strTemp);
	}

	// 配置列表数据更新
	for(int i = 0; i < m_nNumOfValidConfigNode; i++)
	{
		// 数组索引    浓度点索引 
		// 0        ->      1     
		// 1        ->      2     
		// ... ...

		CString strTemp;
		// 检测项目
		m_ComboxProjectName.GetWindowText(strTemp);
		m_vctListItemData[i]->SetProject(strTemp);

		// 记录ID和批号
		m_ComboxBatchNum.GetWindowText(strTemp);
		m_vctListItemData[i]->SetBatchNum(strTemp);

		// 浓度点索引赋值
		if(m_IsAddStanderds == true)
			m_vctListItemData[i]->SetConcIndex(i);
		// 浓度点索引选项
		m_vctListItemData[i]->SetOptionsConcIndex(arrOptionsConcIndex);
	}
	m_ctrlConfigList.RedrawAllItems();
}

// ---------------------------------------------------------
// 函数介绍：
//    旋转按钮位置变化消息响应函数
// ---------------------------------------------------------
void CStandardConfigDlg::OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString strRepeations;
	m_EditRepeation.GetWindowText(strRepeations);

	int nTimes;
	nTimes = _ttoi(strRepeations);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// 如果此值为-1,即点击了Spin的向上箭头时
	{
		nTimes++;
	}
	else if(pNMUpDown->iDelta == 1)		// 如果此值为1,即点击了Spin的向下箭头时
	{
		nTimes--;
	}

	strRepeations.Format(_T("%d"),nTimes);
	m_EditRepeation.SetWindowText(strRepeations);
	*pResult = 0;
}

// ---------------------------------------------------------
// 函数介绍：
//    重复次数编辑框值发生变化时的响应函数
// ---------------------------------------------------------
void CStandardConfigDlg::OnEnChangeEditRepeation()
{
	CString strRepeations;
	m_EditRepeation.GetWindowText(strRepeations);
	int nTimes;
	nTimes = _ttoi(strRepeations);
	
	if(nTimes > 20)
	{
		nTimes = 20;
		strRepeations.Format(_T("%d"),nTimes);
		m_EditRepeation.SetWindowText(strRepeations);
	}

	if(nTimes < 1)
	{
		nTimes = 1;
		strRepeations.Format(_T("%d"),nTimes);
		m_EditRepeation.SetWindowText(strRepeations);
	}

	for(int i = 0; i < m_nNumOfValidConfigNode; i++)
	{
		m_vctListItemData[i]->SetRepeat(nTimes);
	}
	m_ctrlConfigList.RedrawAllItems();
	return;
}

void CStandardConfigDlg::_ListToConfig(const CStandardConfigListItem *pItem,SampleConfigNode *pNode)
{
	if(pItem->GetBatchNum() == _T(""))
		return;
	// 检测项目组合框和批号当前选择项索引
	int indexCurrentRegent = m_ComboxProjectName.GetCurSel();
	if (indexCurrentRegent < 0)
		return;
	int indexBatchNum = m_ComboxBatchNum.GetCurSel();
	// 找出对应的试剂位置编号
	Reagent_Pos ReagentPos = static_cast<Reagent_Pos> (m_ComboxProjectName.GetItemData(indexCurrentRegent));

	// 标准品信息
	pNode->RecordID = m_vctStandardRecords[indexBatchNum].ID;			// 标准品ID
	pNode->IsSelected = false;											// 被选状态取消
	pNode->IsSetingSample = true;										// 设置为“已经设置样本信息”
	pNode->IsStandard = true;											// 设置为“是标准品”
	pNode->StdPointIndex = pItem->GetConcIndex();						// 标准点索引

	// 检测项目信息
	pNode->TestProjArr.clear();											// 标准品只允许检测一个项目
	SampleConfigNode::TestProjInfo newproj;								// 测试项目信息
	newproj.ReagentPos = ReagentPos;									// 试剂区试剂位置编号
	newproj.Repetitions = pItem->GetRepeat();							// 重复次数
	newproj.TestUintKeys = 0;											// 还未开始测试，赋值0
	pNode->TestProjArr.insert(pair<Reagent_Pos,SampleConfigNode::TestProjInfo>(ReagentPos,newproj));
}

void CStandardConfigDlg::_ConfigToList(const SampleConfigNode *pNode,CStandardConfigListItem * pItem)
{
	pItem->SetPosNum(pNode->PosNum);
	// 如果是添加新标准品信息，则不再设置其他信息
	if(m_IsAddStanderds)
		return;

	pItem->SetRepeat(pNode->TestProjArr.begin()->second.Repetitions);
	pItem->SetConcIndex(pNode->StdPointIndex);
}

BOOL CStandardConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// 取消回车键和esc键的功能
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
