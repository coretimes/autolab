// ReagentSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentSelectDlg.h"
#include "ReagentSelectListItem.h"

// CReagentSelectDlg 对话框

IMPLEMENT_DYNAMIC(CReagentSelectDlg, CDialog)

CReagentSelectDlg::CReagentSelectDlg(Reagent_Key & KeyBuffer)
	: CDialog(CReagentSelectDlg::IDD, NULL),m_nCurrentSelectedReagent(KeyBuffer)
{
}

CReagentSelectDlg::~CReagentSelectDlg()
{
}

void CReagentSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PojectName,m_ctrlProjectName);
	DDX_Control(pDX, IDC_Date_ProductFrom,m_ctrlProductDateFrom);
	DDX_Control(pDX, IDC_Date_ProductTo,m_ctrlProductDateTo);
	DDX_Control(pDX, IDC_LIST_Reagents,m_ctrlReagentList);
}


BEGIN_MESSAGE_MAP(CReagentSelectDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CReagentSelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CReagentSelectDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PojectName, &CReagentSelectDlg::OnCbnSelchangeComboPojectname)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductTo, &CReagentSelectDlg::OnDtnDateChProductDateTo)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductFrom, &CReagentSelectDlg::OnDtnDateChProductDateFrom)
	ON_BN_CLICKED(IDC_CHECK_HideExpired, &CReagentSelectDlg::OnBnClickedCheckHideexpired)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_Reagents, &CReagentSelectDlg::OnLvnItemchangedListReagents)
END_MESSAGE_MAP()

BOOL CReagentSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// 项目名组合框初始化
	_InitComboProjectName();

	// 终止生产日期初始化为今天，起始生产日期初始化为一年以前
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);
	defaultTime.SetDate(defaultTime.GetYear() - 1,defaultTime.GetMonth(),defaultTime.GetDay());
	m_ctrlProductDateFrom.SetTime(defaultTime);

	// 试剂列表初始化
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("批号"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProductDate,_T("生产日期"),LVCFMT_CENTER);

	m_ctrlReagentList.SetColumnWidth(colBatchNum,100);
	m_ctrlReagentList.SetColumnWidth(colProject,100);
	m_ctrlReagentList.SetColumnWidth(colProductDate,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlReagentList.SetItemHeight(24);
	m_ctrlReagentList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlReagentList.m_SelectedBackColor = RGB(230, 241, 255);

	// 刷新试剂列表
	_RefreshReagentsList();

	// 刷新试剂详细信息
	_RefreshDetails();
	return TRUE;
}

void CReagentSelectDlg::_InitComboProjectName()
{
	CReagentTable * pTable = CReagentTable::GetInstance();
	pTable->GetDistinctProjectNames(m_AllProjectNameList);

	m_ctrlProjectName.InsertString(0,_T(""));	
	for(int i = 0; i < (int)(m_AllProjectNameList.size()); i++)
	{
		// 项目名称列表输出格式为： 项目简称（项目全称）
		CString strDuName;
		strDuName.Format(_T("%s (%s)"),m_AllProjectNameList[i].ShortName,m_AllProjectNameList[i].ProjectName);
		m_ctrlProjectName.InsertString( i + 1,strDuName);	
	}
}

void CReagentSelectDlg::_RefreshReagentsList()
{
	// 取得项目名称
	int index = m_ctrlProjectName.GetCurSel();
	CString strProjecName;
	if(index > 0)
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;

	// 取得生产日期
	COleDateTime timeProductDateFrom,timeProductDateTo;
	m_ctrlProductDateFrom.GetTime(timeProductDateFrom);
	m_ctrlProductDateTo.GetTime(timeProductDateTo);

	// 取得试剂表指针
	CReagentTable * pTable = CReagentTable::GetInstance();

	vector<ReagentRecord> vtReagentList;
	pTable->GetReagents(
		/*vector<ReagentRecord> & */vtReagentList,
		/*const bool &IsFuzzyQuery = */false,
		/*const Reagent_Key &ID =*/ 0,					          
		/*const CString &strBatchNum =*/ _T(""),			         
		/*const CString &strProjectName =*/strProjecName,			            
		/*const CString &strShortName = */_T(""),                  
		/*const DATE &ProductionDateUpper = */timeProductDateTo,
		/*const DATE &ProductionDateLower = */timeProductDateFrom);

	// 在试剂列表中插入查询到的试剂项
	m_ctrlReagentList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtReagentList.size()); i++)
	{ 
		// 试剂ID
		Reagent_Key key = vtReagentList[i].ID;

		// 试剂批号取得
		CString strBatchNum = vtReagentList[i].BatchNum;

		// 项目全称和简称取得
		ProjectFullName projectname;
		projectname.ProjectName = vtReagentList[i].ProjectName;
		projectname.ShortName = vtReagentList[i].ShortName;
		
		// 生产日期取得
		COleDateTime dtProductDate = vtReagentList[i].ProductionDate;
		// 构造试剂列表项
		CReagentSelectListItem * pNewItem = new CReagentSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}
	
	// 默认排序
	m_ctrlReagentList.Reorder();

	// 把试剂列表设置为焦点窗口
	m_ctrlReagentList.SetFocus();
}

void CReagentSelectDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// 如果当前试剂ID有效，则设置详细信息，否则设置为空
	if(m_nCurrentSelectedReagent > 0)
	{
		CReagentTable * pTable = CReagentTable::GetInstance();
		ReagentRecord record = pTable->GetRecordInfo(m_nCurrentSelectedReagent);
		CString strTemp;
		COleDateTime dtDate;
		// 标准品详细信息“试剂ID”
		strTemp.Format(_T("%d"),record.ID);
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“生产批号”
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(record.BatchNum);

		// 标准品详细信息“项目名称”
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		strTemp.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“生产日期”
		dtDate = record.ProductionDate;
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“失效日期”
		dtDate = record.ExpirationDate;
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“首次加载时间”
		dtDate = record.FirstLoadTime;
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“首次使用时间”
		dtDate = record.FirstUseTime;
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“额定实验次数”
		strTemp.Format(_T("%d"),record.RatedTestTimes);
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“剩余使用次数”
		strTemp.Format(_T("%d"),record.RemainTimes);
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“测量单位”
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(record.Unit);

		// 标准品详细信息“结果上限”
		strTemp.Format(_T("%g"),record.UpperLimit);
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“结果下限”
		strTemp.Format(_T("%g"),record.LowerLimit);
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“样本用量”
		strTemp.Format(_T("%g"),record.SampleAmount);
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“磁标用量”
		strTemp.Format(_T("%g"),record.MagneticAmount);
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“酶标用量”
		strTemp.Format(_T("%g"),record.HRPAmount);
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“稀释倍数”
		strTemp.Format(_T("%g"),record.Dilution);
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“清洗次数”
		strTemp.Format(_T("%g"),record.WashTimes);
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“孵育时间”
		strTemp.Format(_T("%g"),record.IncubationTime);
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(strTemp);
	}
	else
	{
		// 标准品详细信息“试剂ID”
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“生产批号”
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“项目名称”
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“生产日期”
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“失效日期”
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“首次加载时间”
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“首次使用时间”
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“额定实验次数”
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“剩余使用次数”
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“测量单位”
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“结果上限”
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“结果下限”
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“样本用量”
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“磁标用量”
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“酶标用量”
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“稀释倍数”
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“清洗次数”
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“孵育时间”
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(_T(""));

	}
}

// CReagentSelectDlg 消息处理程序

void CReagentSelectDlg::OnBnClickedCancel()
{
	CDialog::EndDialog(IDCANCEL);
}

void CReagentSelectDlg::OnBnClickedOk()
{
	if(m_nCurrentSelectedReagent == 0)
	{
		if(MessageBox(_T("没有选择任何试剂，确认退出后不会改变原有配置！\n是否确认退出？"),_T("空选择确认"),MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
			return;			
	}	
	CDialog::EndDialog(IDOK);
}

void CReagentSelectDlg::OnCbnSelchangeComboPojectname()
{
	_RefreshReagentsList();
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
}

void CReagentSelectDlg::OnDtnDateChProductDateTo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// 清空详细信息
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();

	*pResult = 0;
}

void CReagentSelectDlg::OnDtnDateChProductDateFrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// 清空详细信息
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();

	*pResult = 0;
}

void CReagentSelectDlg::OnBnClickedCheckHideexpired()
{
	_RefreshReagentsList();
}

void CReagentSelectDlg::OnLvnItemchangedListReagents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState & ODS_SELECTED)
	{
		CReagentSelectListItem * pItem = reinterpret_cast<CReagentSelectListItem *>(m_ctrlReagentList.GetItemData(pNMLV->iItem));
		// 当前试剂ID设为当前选中项的数据
		m_nCurrentSelectedReagent = pItem->GetReagentKey();
		_RefreshDetails();
	}

	*pResult = 0;
}
