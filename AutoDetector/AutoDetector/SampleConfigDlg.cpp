// SampleConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleConfigDlg.h"
#include "SampleTable.h"

// CSampleConfigDlg dialog

IMPLEMENT_DYNAMIC(CSampleConfigDlg, CDialog)

CSampleConfigDlg::CSampleConfigDlg(const bool &IsAddSamples,
								   map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
								   map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
								   CWnd* pParent/* = NULL*/)
								   :
								   m_IsAddSamples(IsAddSamples),
								   m_SampleConfigNodes(SampleConfigNodes),
								   m_RegentConfigNodes(RegentConfigNodes),
								   CDialog(CSampleConfigDlg::IDD, pParent)
{
	// 样本配置节点个数初始化
	m_nCountOfConfigNode = (int)m_SampleConfigNodes.size();
}

CSampleConfigDlg::~CSampleConfigDlg()
{
}

void CSampleConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ConfigList,m_ctrlConfigList);
	DDX_Control(pDX, IDC_Listbox_Project,m_ctrlListProject);
	DDX_Control(pDX, IDC_Value_NumPrefix,m_ctrlEditNumPrefix);
	DDX_Control(pDX, IDC_Combox_Serials,m_ctrlCombSerials);
	DDX_Control(pDX, IDC_Combox_Type,m_ctrlCombTypes);
	DDX_Control(pDX, IDC_SPIN_Dilution,m_ctrlSpinBtnDilution);
	DDX_Control(pDX, IDC_Value_Dilution,m_ctrlEditDilution);
	DDX_Control(pDX, IDC_Value_Dept,m_ctrlEditDept);
	DDX_Control(pDX, IDC_Value_Applicant,m_ctrlEditApplicant);
	DDX_Control(pDX, IDC_Date_ApplyDate,m_ctrlDateApply);
	DDX_Control(pDX, IDC_Checkbox_SetDefault,m_CheckSetDefault);
	DDX_Control(pDX, IDC_Edit_Repeation,m_ctrlEditRepeations);
	DDX_Control(pDX, IDC_Spin_Repeation,m_ctrlSpinBtnRepeations);
	DDX_Control(pDX, IDC_Checkbox_SetTheoryConc,m_CheckSetThoeryConc);
	DDX_Control(pDX, IDC_Edit_StartConc,m_ctrlEditStartConc);
	DDX_Control(pDX, IDC_Spin_StartConc,m_ctrlSpinBtnStartConc);
	DDX_Control(pDX, IDC_Edit_Increament,m_ctrlEditIncreament);
	DDX_Control(pDX, IDC_Spin_Increament,m_ctrlSpinBtnIncreament);
}

BEGIN_MESSAGE_MAP(CSampleConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &CSampleConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_Cancle, &CSampleConfigDlg::OnBnClickedCancle)
	ON_EN_CHANGE(IDC_Value_NumPrefix, &CSampleConfigDlg::OnEnChangeValueNumprefix)
	ON_CBN_SELCHANGE(IDC_Combox_Serials, &CSampleConfigDlg::OnCbnSelchangeComboxSerials)
	ON_CBN_SELCHANGE(IDC_Combox_Type, &CSampleConfigDlg::OnCbnSelchangeComboxType)
	ON_EN_CHANGE(IDC_Value_Dilution, &CSampleConfigDlg::OnEnChangeValueDilution)
	ON_EN_CHANGE(IDC_Value_Dept, &CSampleConfigDlg::OnEnChangeValueDept)
	ON_EN_CHANGE(IDC_Value_Applicant, &CSampleConfigDlg::OnEnChangeValueApplicant)
	ON_LBN_SELCHANGE(IDC_Listbox_Project, &CSampleConfigDlg::OnLbnSelchangeListboxProject)
	ON_CBN_EDITCHANGE(IDC_Combox_Type, &CSampleConfigDlg::OnCbnEditchangeComboxType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Dilution, &CSampleConfigDlg::OnDeltaposSpinDilution)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ApplyDate, &CSampleConfigDlg::OnDtnDatetimechangeDateApplydate)
	ON_CONTROL(CLBN_CHKCHANGE, IDC_Listbox_Project, OnProjectChkChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_Repeation, &CSampleConfigDlg::OnDeltaposSpinRepeation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_StartConc, &CSampleConfigDlg::OnDeltaposSpinStartconc)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_Increament, &CSampleConfigDlg::OnDeltaposSpinIncreament)
	ON_EN_CHANGE(IDC_Edit_StartConc, &CSampleConfigDlg::OnEnChangeEditStartconc)
	ON_EN_CHANGE(IDC_Edit_Increament, &CSampleConfigDlg::OnEnChangeEditIncreament)
	ON_BN_CLICKED(IDC_Checkbox_SetTheoryConc, &CSampleConfigDlg::OnBnClickedCheckboxSettheoryconc)
	ON_EN_CHANGE(IDC_Edit_Repeation, &CSampleConfigDlg::OnEnChangeEditRepeation)
END_MESSAGE_MAP()

BOOL CSampleConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 各控件初始化

	// “序号”组合框和“编号前缀”编辑框内容初始化
	m_ctrlCombSerials.InsertString(0,_T("数字(1、2、3)"));
	m_ctrlCombSerials.InsertString(1,_T("坐标(A1、B2)"));
	m_ctrlCombSerials.InsertString(2,_T("日期+数字"));
	m_ctrlCombSerials.SetCurSel(0);
	m_ctrlEditNumPrefix.SetWindowText(_T("样本"));

	// “样本类型”组合框内容初始化
	CSampleTable * pSampleTable = CSampleTable::GetInstance();
	vector<CString> TypeList;
	pSampleTable->GetDistinctType(TypeList);
	for(int i=0; i< (int) TypeList.size(); i++)
	{
		m_ctrlCombTypes.InsertString(i,TypeList[i]);
	}

	// “稀释倍数”编辑框初始化
	m_ctrlEditDilution.SetWindowText(_T("1"));

	// 检测项目组合框初始化
	CReagentTable * pReagentTable = CReagentTable::GetInstance();

	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_RegentConfigNodes.begin();
	vector<CString> ProjectList;
	while(iterReagent != m_RegentConfigNodes.end())
	{
		// 取得该试剂ID的试剂详细信息
		ReagentRecord regentInfo;
		regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
		int nCountOfComboxItem = m_ctrlListProject.GetCount();

		// 在组合框末尾添加该试剂检测项目名称
		CString temp;
		temp.Format(_T("%d. %s"),iterReagent->first,regentInfo.ProjectName);
		m_ctrlListProject.InsertString(nCountOfComboxItem,temp);

		// 组合框该Item的数据指向试剂区位置
		m_ctrlListProject.SetItemData(nCountOfComboxItem,static_cast<DWORD_PTR>(iterReagent->second->pos));

		// 把项目名称保存下来，作为itemdata的组合框选项
		ProjectList.push_back(temp);
		iterReagent++;
	}

	// 初始一览表列属性
	m_ctrlConfigList.InsertColumn(0,_T("坐标"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(1,_T("样本编号"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(2,_T("样本类型"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(3,_T("稀释倍数"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(4,_T("送检单位名称"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(5,_T("送检人"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(6,_T("送检日期"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(7,_T("检测项目名称"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(8,_T("重复次数"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(9,_T("理论浓度"),LVCFMT_CENTER);
	m_ctrlConfigList.SetItemHeight(24);
	
	m_ctrlConfigList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlConfigList.m_SelectedBackColor = RGB(230, 241, 255);

	for(int i=0; i<m_ctrlConfigList.GetColumnCount(); i++)
	{
		m_ctrlConfigList.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	}

	// 初始一览表数据
	map<Sample_Pos,SampleConfigNode *>::iterator iter = m_SampleConfigNodes.begin();
	int idxItem = 0;
	while(iter != m_SampleConfigNodes.end())
	{
		vector<CSampleConfigListItem *> vectItem;

		// 调用confignode类型数据转化为CSampleConfigListItem类型数据函数
		_ConfigToList(iter->second,vectItem);

		for(int i=0; i < (int)vectItem.size(); i++)
		{
			vectItem[i]->SetTypeOptions(TypeList);
			vectItem[i]->SetProjectOptions(ProjectList);
			m_mapListItemData.insert(pair<Sample_Pos,CSampleConfigListItem*>(vectItem[i]->GetPosNum(),vectItem[i]));
			m_ctrlConfigList.AddItem(idxItem++,static_cast<CListItemData *> (vectItem[i]));
		}
		iter++;
	}
	_RefreshSearchList();

	if(m_IsAddSamples)
	{
		OnEnChangeValueNumprefix();
		OnEnChangeValueDilution();
		OnDtnDatetimechangeDateApplydate(NULL,NULL);
		// “重复次数”初始化
		m_ctrlEditRepeations.SetWindowText(_T("1"));
	}
	
	// 理论浓度控件
	CWnd * pWnd = GetDlgItem(IDC_Title_StartConc);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_Title_Increament);
	pWnd->EnableWindow(FALSE);
	m_ctrlEditStartConc.EnableWindow(FALSE);
	m_ctrlEditIncreament.EnableWindow(FALSE);
	m_ctrlSpinBtnStartConc.EnableWindow(FALSE);
	m_ctrlSpinBtnIncreament.EnableWindow(FALSE);

	// 浓度单位控件显示
	pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_Unit2);
	pWnd->SetWindowText(_T(""));
	return TRUE;
}

void CSampleConfigDlg::OnBnClickedOk()
{
	// 遍历所有选中的样本配置节点
	map<Sample_Pos,SampleConfigNode *>::iterator iterSampleConfigNode = m_SampleConfigNodes.begin();	
	while(iterSampleConfigNode != m_SampleConfigNodes.end())
	{
		// 遍历每个样本节点的“检测项目信息”数组，删除掉还未开始的检测项目信息，避免重复次数累加
		map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iterTestProj = iterSampleConfigNode->second->TestProjArr.begin();
		// 把要删除的key保存下来，遍历完后再删除
		vector<Reagent_Pos> DeleteNodeKey;
		while(iterTestProj != iterSampleConfigNode->second->TestProjArr.end())
		{
			if(iterTestProj->second.TestUintKeys < 1)
				DeleteNodeKey.push_back(iterTestProj->first);	
			iterTestProj++;
		}
		// 删除数组中保存的key值对应的元素
		for (int i=0; i<(int)DeleteNodeKey.size(); i++)
		{
			iterSampleConfigNode->second->TestProjArr.erase(DeleteNodeKey[i]);
		}
		iterSampleConfigNode++;
	}

	for (int i=0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		// 取得列表ListItem数据
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *> (m_ctrlConfigList.GetItemData(i));

		// 取得样本配置节点指针
		map<Sample_Pos,SampleConfigNode *>::iterator iterSampleConfigNode = m_SampleConfigNodes.find(pItem->GetPosNum());

		// 使数据由ListItem流向ConfigNode
		_ListToConfig(pItem,iterSampleConfigNode->second);
	}
	return OnOK();
}

void CSampleConfigDlg::OnBnClickedCancle()
{
	return OnCancel();
}

void CSampleConfigDlg::OnEnChangeValueNumprefix()
{
	_InitSampleNo();
}

void CSampleConfigDlg::OnCbnSelchangeComboxSerials()
{
	_InitSampleNo();
}

void CSampleConfigDlg::_InitSampleNo()
{
	// 初始化样本编号
	CString strPrefix,strSerial;
	m_ctrlEditNumPrefix.GetWindowText(strPrefix);

	// 序号组合框当前选择索引
	int indexSerials = m_ctrlCombSerials.GetCurSel();

	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));

		// 判断序号组合框当前选择索引
		switch(indexSerials)
		{
		case 0:		// 数字(1、2、3)
			strSerial.Format(_T("%d"),i + 1);
			break;
		case 1:		// 坐标(A1、B2)
			pItem->GetCellContex(colPos,strSerial);
			break;
		case 2:		// 日期+数字
			CString strToday = COleDateTime::GetCurrentTime().Format(_T("%y%m%d"));
			strSerial.Format(_T("%s-%d"),strToday,i + 1);
			break;
		}
		pItem->SetSampleNo(strPrefix + strSerial);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnCbnSelchangeComboxType()
{
	// “类型”组合框选项发生变化响应
	CString strSampleType;
	int nIndex = m_ctrlCombTypes.GetCurSel();
	m_ctrlCombTypes.GetLBText( nIndex, strSampleType);
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetSampleType(strSampleType);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnCbnEditchangeComboxType()
{
	// “类型”组合框内容发生变化响应
	CString strSampleType;
	m_ctrlCombTypes.GetWindowText(strSampleType);
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetSampleType(strSampleType);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnEnChangeValueDilution()
{
	//“稀释倍数”编辑框内容发生变化响应
	CString strDilution;
	m_ctrlEditDilution.GetWindowText(strDilution);
	double dDilution;
	dDilution = _tstof(strDilution);

	if(dDilution > 100000)
	{
		dDilution = 100000;
		strDilution.Format(_T("%g"),dDilution);
		m_ctrlEditDilution.SetWindowText(strDilution);
	}

	if(dDilution < 1)
	{
		dDilution = 1;
		strDilution.Format(_T("%g"),dDilution);
		m_ctrlEditDilution.SetWindowText(strDilution);
	}

	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetSampleDilution(dDilution);

	}
	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnDeltaposSpinDilution(NMHDR *pNMHDR, LRESULT *pResult)
{
	//“稀释倍数”微调按钮单击响应函数
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strDilution;
	m_ctrlEditDilution.GetWindowText(strDilution);

	double dDilution;
	dDilution = _tstof(strDilution);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// 如果此值为-1,即点击了Spin的向上箭头时
	{
		dDilution += 1.0;
	}
	else if(pNMUpDown->iDelta == 1)		// 如果此值为1,即点击了Spin的向下箭头时
	{
		dDilution -= 1.0;
	}

	strDilution.Format(_T("%g"),dDilution);
	m_ctrlEditDilution.SetWindowText(strDilution);
	*pResult = 0;
}

void CSampleConfigDlg::OnEnChangeValueDept()
{
	//“送检单位”发生变化响应函数
	CString strEditDept;
	m_ctrlEditDept.GetWindowText(strEditDept);
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetSampleDept(strEditDept);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnEnChangeValueApplicant()
{
	//“送检人”发生变化响应函数
	CString strEditApplicat;
	m_ctrlEditApplicant.GetWindowText(strEditApplicat);
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetSampleApplicant(strEditApplicat);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnLbnSelchangeListboxProject()
{
	// 项目列表选中状态改变消息响应，选中指定项目的所有列表数据
	int idxProject = m_ctrlListProject.GetCurSel();
	multimap<int,int>::iterator iterSearch = m_mapSearchList.find(idxProject);

	// 先取消所有原listitem的选中状态
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
 		m_ctrlConfigList.SetItemState(i,0, LVIS_SELECTED|LVIS_FOCUSED);
	}

	// 根据查询表，查出检测此项目的所有listitem
	while(iterSearch != m_mapSearchList.end() && iterSearch->first == idxProject)
	{		
		m_ctrlConfigList.SetItemState(iterSearch->second,LVIS_SELECTED, LVIS_SELECTED);
		iterSearch ++;
	}

	// 浓度单位显示更新
	CString strUnit = _GetUnitConc(idxProject);
	CWnd * pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(strUnit);
	pWnd = GetDlgItem(IDC_Value_Unit2);
	pWnd->SetWindowText(strUnit);
}

void CSampleConfigDlg::OnProjectChkChange()
{
	// 项目列表check状态改变消息响应
	int idxProject = m_ctrlListProject.GetCurSel();
	int checkState = m_ctrlListProject.GetCheck(idxProject);
	switch(checkState)
	{
	case 0:	// 点击后变成未选中时，表示要删除此项目
		{
			_DeleteProject(idxProject);
			break;
		}

	case 1:	// 点击后变成选中时，表示要添加此项目
		{
			_AddNewProject(idxProject);
			m_ctrlListProject.SetCheck(idxProject,1);
			break;
		}
	default:
		return;
	}
	_RefreshSearchList();
	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnDtnDatetimechangeDateApplydate(NMHDR *pNMHDR, LRESULT *pResult)
{
	//“送检日期”变化响应函数
	COleDateTime time;
	m_ctrlDateApply.GetTime(time);

	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		pItem->SetApplyDate(time);
	}

	m_ctrlConfigList.RedrawAllItems();
	// 	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// 	*pResult = 0;
}

void CSampleConfigDlg::OnBnClickedCheckboxSettheoryconc()
{
	//“设置理论浓度复选框”单击响应函数
	int checkState = m_CheckSetThoeryConc.GetCheck();
	if(checkState == 1)
	{
		checkState = 1;
		CWnd * pWnd = GetDlgItem(IDC_Title_StartConc);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_Title_Increament);
		pWnd->EnableWindow(TRUE);
		m_ctrlEditStartConc.EnableWindow(TRUE);
		m_ctrlSpinBtnStartConc.EnableWindow(TRUE);
		m_ctrlEditIncreament.EnableWindow(TRUE);
		m_ctrlSpinBtnIncreament.EnableWindow(TRUE);
		_AutoFillThoeryConc(true);
	}
	else
	{
		checkState = 0;
		CWnd * pWnd = GetDlgItem(IDC_Title_StartConc);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_Title_Increament);
		pWnd->EnableWindow(FALSE);
		m_ctrlEditStartConc.EnableWindow(FALSE);
		m_ctrlSpinBtnStartConc.EnableWindow(FALSE);
		m_ctrlEditIncreament.EnableWindow(FALSE);
		m_ctrlSpinBtnIncreament.EnableWindow(FALSE);
		_AutoFillThoeryConc(false);
	}
	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnEnChangeEditRepeation()
{
	// “重复次数”编辑框变化响应函数
	CString strRepeations;
	m_ctrlEditRepeations.GetWindowText(strRepeations);
	int dRepeations;
	dRepeations = _tstoi(strRepeations);

	if(dRepeations > 20)
	{
		dRepeations = 20;
		strRepeations.Format(_T("%d"),dRepeations);
		m_ctrlEditRepeations.SetWindowText(strRepeations);
	}

	if(dRepeations < 1)
	{
		dRepeations = 1;
		strRepeations.Format(_T("%d"),dRepeations);
		m_ctrlEditRepeations.SetWindowText(strRepeations);
	}

	POSITION pos = m_ctrlConfigList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ctrlConfigList.GetNextSelectedItem(pos);
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(nItem));
		pItem->SetRepeat(dRepeations);
	}

	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult)
{
	//“重复次数”微调按钮单击响应函数

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strRepeations;
	m_ctrlEditRepeations.GetWindowText(strRepeations);

	int nRepeations;
	nRepeations = _tstoi(strRepeations);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// 如果此值为-1,即点击了Spin的向上箭头时
	{
		nRepeations += 1;
	}
	else if(pNMUpDown->iDelta == 1)		// 如果此值为1,即点击了Spin的向下箭头时
	{
		nRepeations -= 1;
	}

	strRepeations.Format(_T("%d"),nRepeations);
	m_ctrlEditRepeations.SetWindowText(strRepeations);
	*pResult = 0;
}

void CSampleConfigDlg::OnDeltaposSpinStartconc(NMHDR *pNMHDR, LRESULT *pResult)
{
	//“起始浓度”微调按钮单击响应函数

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strStartConc;
	m_ctrlEditStartConc.GetWindowText(strStartConc);

	double dStartConc;
	dStartConc = _tstof(strStartConc);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// 如果此值为-1,即点击了Spin的向上箭头时
	{
		dStartConc += 0.1;
	}
	else if(pNMUpDown->iDelta == 1)		// 如果此值为1,即点击了Spin的向下箭头时
	{
		dStartConc -= 0.1;
	}

	strStartConc.Format(_T("%g"),dStartConc);
	m_ctrlEditStartConc.SetWindowText(strStartConc);
	*pResult = 0;
}

void CSampleConfigDlg::OnDeltaposSpinIncreament(NMHDR *pNMHDR, LRESULT *pResult)
{
	//“浓度增量”微调按钮单击响应函数

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strIncreament;
	m_ctrlEditIncreament.GetWindowText(strIncreament);

	double dIncreament;
	dIncreament = _tstof(strIncreament);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// 如果此值为-1,即点击了Spin的向上箭头时
	{
		dIncreament += 0.01;
	}
	else if(pNMUpDown->iDelta == 1)		// 如果此值为1,即点击了Spin的向下箭头时
	{
		dIncreament -= 0.01;
	}

	strIncreament.Format(_T("%g"),dIncreament);
	m_ctrlEditIncreament.SetWindowText(strIncreament);
	*pResult = 0;
}

void CSampleConfigDlg::OnEnChangeEditStartconc()
{
	//“理论浓度增量”编辑框内容发生变化响应
	CString strStartconc;
	m_ctrlEditStartConc.GetWindowText(strStartconc);
	double dStartconc;
	dStartconc = _tstof(strStartconc);

	if(dStartconc > 1000)
	{
		dStartconc = 1000;
		strStartconc.Format(_T("%g"),dStartconc);
		m_ctrlEditStartConc.SetWindowText(strStartconc);
	}

	if(dStartconc < 0)
	{
		dStartconc = 0;
		strStartconc.Format(_T("%g"),dStartconc);
		m_ctrlEditStartConc.SetWindowText(strStartconc);
	}

	_AutoFillThoeryConc();
	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::OnEnChangeEditIncreament()
{
	//“理论浓度增量”编辑框内容发生变化响应
	CString strIncreament;
	m_ctrlEditIncreament.GetWindowText(strIncreament);
	double dIncreament;
	dIncreament = _tstof(strIncreament);

	if(dIncreament > 1000)
	{
		dIncreament = 1000;
		strIncreament.Format(_T("%g"),dIncreament);
		m_ctrlEditIncreament.SetWindowText(strIncreament);
	}

	if(dIncreament < -1000)
	{
		dIncreament = -1000;
		strIncreament.Format(_T("%g"),dIncreament);
		m_ctrlEditIncreament.SetWindowText(strIncreament);
	}

	_AutoFillThoeryConc();
	m_ctrlConfigList.RedrawAllItems();
}

void CSampleConfigDlg::_AddNewProject(const int &newProjecIndex)
{
	// 删除某项目

	// 遍历所有选中状态样本配置节点
	map<Sample_Pos,SampleConfigNode *>::iterator iterSample = m_SampleConfigNodes.begin();
	while(iterSample != m_SampleConfigNodes.end())
	{
		// 找到所有同坐标的列表数据
		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterListItemData = m_mapListItemData.find(iterSample->first);
		// 是否需要添加标志
		bool IsNeedAdd = true;
		// 保存此条列表数据指针，以备创建新列表数据时用来拷贝样本信息
		CSampleConfigListItem * pSourceData = iterListItemData->second;
		// 遍历找到所有同样本位置坐标的列表数据
		while(iterListItemData != m_mapListItemData.end() && iterListItemData->first == iterSample->first)
		{
			CSampleConfigListItem * pItem = iterListItemData->second;

			// 如果某样本还未设置任何项目信息，则直接设置项目信息
			if(pItem->GetProjectIndex() < 0)
			{
				// 设置项目索引
				pItem->SetProjectIndex(newProjecIndex);

				// 设置重复次数
				CString strRepeat;
				m_ctrlEditRepeations.GetWindowText(strRepeat);
				int repeat = _tstoi(strRepeat);
				pItem->SetRepeat(repeat);
				IsNeedAdd = false;
				break;
			}

			// 如果此样本已经添加了该项目，则退出此循环
			if(pItem->GetProjectIndex() == newProjecIndex)
			{
				IsNeedAdd = false;
				break;
			}

			iterListItemData++;
		}
		if( IsNeedAdd)
		{
			// 创建新列表数据，从原数据拷贝
			CSampleConfigListItem * pNewItem = new CSampleConfigListItem(*(pSourceData));
			pNewItem->SetProjectIndex(newProjecIndex);

			// 设置重复次数
			CString strRepeat;
			m_ctrlEditRepeations.GetWindowText(strRepeat);
			int repeat = _tstoi(strRepeat);
			pNewItem->SetRepeat(repeat);
			m_mapListItemData.insert(pair<Sample_Pos,CSampleConfigListItem*>(iterSample->first,pNewItem));
			m_ctrlConfigList.AddItem(m_ctrlConfigList.GetItemCount(),pNewItem);
		}
		iterSample++;
	}
	m_ctrlConfigList.Reorder();
}

void CSampleConfigDlg::_DeleteProject(const int &idxDelProject)
{
	// 删除某项目
	multimap<int/*idxProjec*/,int/*idxListItem*/>::iterator iterSearch = m_mapSearchList.find(idxDelProject);

	vector<int> vctDeleteRows;
	while(iterSearch != m_mapSearchList.end() && iterSearch->first == idxDelProject)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(iterSearch->second));
		Sample_Pos pos = pItem->GetPosNum();

		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterLow = m_mapListItemData.lower_bound(pos);
		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterUp = m_mapListItemData.upper_bound(pos);

		// 如果此样本只有一个检测项目,则项目索引、理论浓度、和重复次数均设为负数
		if(iterLow == (--iterUp))
		{
			CSampleConfigListItem* pItemData = iterLow->second;
			pItemData->SetProjectIndex(-1);
			pItemData->SetThoeryConc(-1);
			pItemData->SetRepeat(-1);
		}
		// 否则直接删除此项数据
		else
			vctDeleteRows.push_back(iterSearch->second);

		iterSearch ++;
	}

	m_ctrlConfigList.DeleteMultItems(vctDeleteRows);
}

void CSampleConfigDlg::_RefreshSearchList()
{
	m_mapListItemData.clear();
	m_mapSearchList.clear();
	for(int i=0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));
		m_mapListItemData.insert(pair<Sample_Pos,CSampleConfigListItem*>(pItem->GetPosNum(),pItem));

		int idxProject = pItem->GetProjectIndex();
		if (idxProject >= 0)
		{
			m_mapSearchList.insert(pair<int,int>(pItem->GetProjectIndex(),i));
		}
	}
}
void CSampleConfigDlg::_AutoFillThoeryConc(bool IsAdd)
{
	// 如果是自动填充理论浓度
	if(IsAdd)
	{
		// 取得自动填充的起始值和增量值
		CString strStartConc,strIncreament;
		m_ctrlEditStartConc.GetWindowText(strStartConc);
		m_ctrlEditIncreament.GetWindowText(strIncreament);
		double dStartConc = _tstof(strStartConc);
		double dIncreament = _tstof(strIncreament);

		// 循环自动填充列表选中项目的理论浓度
		double i = 0;
		POSITION pos = m_ctrlConfigList.GetFirstSelectedItemPosition();
		while (pos)
		{
			int nItem = m_ctrlConfigList.GetNextSelectedItem(pos);
			CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(nItem));
			pItem->SetThoeryConc(dStartConc + dIncreament * i);
			i += 1;
		}
	}
	// 如果是删除理论浓度
	else
	{
		POSITION pos = m_ctrlConfigList.GetFirstSelectedItemPosition();
		while (pos)
		{
			int nItem = m_ctrlConfigList.GetNextSelectedItem(pos);
			CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(nItem));
			pItem->SetThoeryConc(-1);
		}
	}
}

CString CSampleConfigDlg::_GetUnitConc(int idxProject)
{
	// 取得指定索引的项目的浓度单位
	// 1.根据项目索引，取得试剂位置
	Reagent_Pos pos = static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(idxProject));
	// 2.根据试剂位置，取得试剂ID
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_RegentConfigNodes.find(pos);
	// 3.根据试剂ID，取得试剂记录信息
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	ReagentRecord record = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
	return record.Unit;
}

void CSampleConfigDlg::_ListToConfig(const CSampleConfigListItem *pItem,SampleConfigNode *pNode)
{
	// 数据由列表ListItem流向ConfigNode

	SampleRecord recSample;
	pItem->GetSampleInfo(recSample);
	CSampleTable *pSampleTable = CSampleTable::GetInstance();

	// 如果样本ID小于1，说明还未设置样本信息，需要在数据库中插入新数据
	if(pNode->RecordID < 1)
	{
		// 从列表项数据取得样本信息
		// 往数据库插入新纪录，并返回ID
		pSampleTable->AddNewRecord(recSample);
		pNode->RecordID = recSample.ID;		// 样本ID
		pNode->IsSelected = false;			// 设置为“已经设置样本信息”
		pNode->IsSetingSample = true;		// 被选状态取消
		pNode->IsStandard = false;			// 设置为“非标准品”
		pNode->StdPointIndex = -1;			// 标准点索引-1
	}
	else
		// 否则更新数据库
		pSampleTable->UpdateRecord(recSample);

	// 检测项目索引
	int idxProject = pItem->GetProjectIndex();
	// 如果没有检测项目，则不设置项目信息
	if(idxProject < 0)
		return;

	Reagent_Pos pos = static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(idxProject));
	// 定义临时引用变量（检测项目数组），因为名字太长
	map<Reagent_Pos,SampleConfigNode::TestProjInfo> & TestProjecList = pNode->TestProjArr;
	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iteProjInfo = TestProjecList.find(pos);

	// 如果新检测项目不存在，或者虽然存在，但是已经在测试进行中，则创建新检测项目节点
	if( iteProjInfo == TestProjecList.end()
		||iteProjInfo->second.TestUintKeys > 0)
	{
		SampleConfigNode::TestProjInfo newProjecNode;
		newProjecNode.ReagentPos = pos;
		newProjecNode.Repetitions = pItem->GetRepeat();
		newProjecNode.TestUintKeys = 0;
		newProjecNode.ThoeryConc = pItem->GetThoeryConc();
		TestProjecList.insert(pair<Reagent_Pos,SampleConfigNode::TestProjInfo>(pos,newProjecNode));
	}
	// 否则合并到一个项目信息节点中
	else
	{
		// 重复次数累加
		iteProjInfo->second.Repetitions += pItem->GetRepeat();
		// 理论浓度不修改
	}

}

void CSampleConfigDlg::_ConfigToList(const SampleConfigNode *pNode,vector <CSampleConfigListItem*> & ItemList)
{
	// 数据由样本区配置节点ConfigNode流向列表ListItem

	// 取得配置节点检测项目个数,并分配空间
	int nCountOfProject = (int)pNode->TestProjArr.size();
	if(nCountOfProject <= 1)
		ItemList.resize(1);
	else
		ItemList.resize(nCountOfProject);

	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::const_iterator iterTestProjArr = pNode->TestProjArr.begin();
	for(int iItem = 0; iItem < (int) ItemList.size(); iItem++)
	{
		ItemList[iItem] = new CSampleConfigListItem();
		// 样本位置坐标
		ItemList[iItem]->SetPosNum(pNode->PosNum);
		
		if(pNode->IsSetingSample == false)
			break;;

		// 样本记录信息
		CSampleTable *pSampleTable = CSampleTable::GetInstance();
		SampleRecord record = pSampleTable->GetRecordInfo(pNode->RecordID);
		ItemList[iItem]->SetSampleInfo(record);
		
		// 检测项目信息
		if(iterTestProjArr != pNode->TestProjArr.end())
		{
			// 根据试剂位置坐标找到“检测项目”列表框中对应索引
			Reagent_Pos ReagentPos = iterTestProjArr->first;

			for(int idxProject = 0; idxProject < m_ctrlListProject.GetCount(); idxProject ++)
			{
				//Sample_Pos pos = static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(j));
				if(ReagentPos == static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(idxProject)))
				{
					ItemList[iItem]->SetProjectIndex(idxProject);
					ItemList[iItem]->SetRepeat(iterTestProjArr->second.Repetitions);
					ItemList[iItem]->SetThoeryConc(iterTestProjArr->second.ThoeryConc);
					m_ctrlListProject.SetCheck(idxProject,true);
					break;
				}
			}
			iterTestProjArr++;
		}
	}
}

BOOL CSampleConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
