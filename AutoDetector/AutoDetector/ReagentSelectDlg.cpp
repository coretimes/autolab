// ReagentSelectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentSelectDlg.h"
#include "ReagentSelectListItem.h"

// CReagentSelectDlg �Ի���

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
	
	// ��Ŀ����Ͽ��ʼ��
	_InitComboProjectName();

	// ��ֹ�������ڳ�ʼ��Ϊ���죬��ʼ�������ڳ�ʼ��Ϊһ����ǰ
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);
	defaultTime.SetDate(defaultTime.GetYear() - 1,defaultTime.GetMonth(),defaultTime.GetDay());
	m_ctrlProductDateFrom.SetTime(defaultTime);

	// �Լ��б��ʼ��
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("����"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProductDate,_T("��������"),LVCFMT_CENTER);

	m_ctrlReagentList.SetColumnWidth(colBatchNum,100);
	m_ctrlReagentList.SetColumnWidth(colProject,100);
	m_ctrlReagentList.SetColumnWidth(colProductDate,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlReagentList.SetItemHeight(24);
	m_ctrlReagentList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlReagentList.m_SelectedBackColor = RGB(230, 241, 255);

	// ˢ���Լ��б�
	_RefreshReagentsList();

	// ˢ���Լ���ϸ��Ϣ
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
		// ��Ŀ�����б������ʽΪ�� ��Ŀ��ƣ���Ŀȫ�ƣ�
		CString strDuName;
		strDuName.Format(_T("%s (%s)"),m_AllProjectNameList[i].ShortName,m_AllProjectNameList[i].ProjectName);
		m_ctrlProjectName.InsertString( i + 1,strDuName);	
	}
}

void CReagentSelectDlg::_RefreshReagentsList()
{
	// ȡ����Ŀ����
	int index = m_ctrlProjectName.GetCurSel();
	CString strProjecName;
	if(index > 0)
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;

	// ȡ����������
	COleDateTime timeProductDateFrom,timeProductDateTo;
	m_ctrlProductDateFrom.GetTime(timeProductDateFrom);
	m_ctrlProductDateTo.GetTime(timeProductDateTo);

	// ȡ���Լ���ָ��
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

	// ���Լ��б��в����ѯ�����Լ���
	m_ctrlReagentList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtReagentList.size()); i++)
	{ 
		// �Լ�ID
		Reagent_Key key = vtReagentList[i].ID;

		// �Լ�����ȡ��
		CString strBatchNum = vtReagentList[i].BatchNum;

		// ��Ŀȫ�ƺͼ��ȡ��
		ProjectFullName projectname;
		projectname.ProjectName = vtReagentList[i].ProjectName;
		projectname.ShortName = vtReagentList[i].ShortName;
		
		// ��������ȡ��
		COleDateTime dtProductDate = vtReagentList[i].ProductionDate;
		// �����Լ��б���
		CReagentSelectListItem * pNewItem = new CReagentSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}
	
	// Ĭ������
	m_ctrlReagentList.Reorder();

	// ���Լ��б�����Ϊ���㴰��
	m_ctrlReagentList.SetFocus();
}

void CReagentSelectDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// �����ǰ�Լ�ID��Ч����������ϸ��Ϣ����������Ϊ��
	if(m_nCurrentSelectedReagent > 0)
	{
		CReagentTable * pTable = CReagentTable::GetInstance();
		ReagentRecord record = pTable->GetRecordInfo(m_nCurrentSelectedReagent);
		CString strTemp;
		COleDateTime dtDate;
		// ��׼Ʒ��ϸ��Ϣ���Լ�ID��
		strTemp.Format(_T("%d"),record.ID);
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���������š�
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(record.BatchNum);

		// ��׼Ʒ��ϸ��Ϣ����Ŀ���ơ�
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		strTemp.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���������ڡ�
		dtDate = record.ProductionDate;
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ��ʧЧ���ڡ�
		dtDate = record.ExpirationDate;
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���״μ���ʱ�䡱
		dtDate = record.FirstLoadTime;
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���״�ʹ��ʱ�䡱
		dtDate = record.FirstUseTime;
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���ʵ�������
		strTemp.Format(_T("%d"),record.RatedTestTimes);
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ʣ��ʹ�ô�����
		strTemp.Format(_T("%d"),record.RemainTimes);
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��������λ��
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(record.Unit);

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		strTemp.Format(_T("%g"),record.UpperLimit);
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		strTemp.Format(_T("%g"),record.LowerLimit);
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ������������
		strTemp.Format(_T("%g"),record.SampleAmount);
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���ű�������
		strTemp.Format(_T("%g"),record.MagneticAmount);
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ø��������
		strTemp.Format(_T("%g"),record.HRPAmount);
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ϡ�ͱ�����
		strTemp.Format(_T("%g"),record.Dilution);
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ����ϴ������
		strTemp.Format(_T("%g"),record.WashTimes);
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ������ʱ�䡱
		strTemp.Format(_T("%g"),record.IncubationTime);
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(strTemp);
	}
	else
	{
		// ��׼Ʒ��ϸ��Ϣ���Լ�ID��
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���������š�
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ����Ŀ���ơ�
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���������ڡ�
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ʧЧ���ڡ�
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���״μ���ʱ�䡱
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���״�ʹ��ʱ�䡱
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���ʵ�������
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ʣ��ʹ�ô�����
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��������λ��
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ������������
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���ű�������
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ø��������
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ϡ�ͱ�����
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ����ϴ������
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ������ʱ�䡱
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(_T(""));

	}
}

// CReagentSelectDlg ��Ϣ�������

void CReagentSelectDlg::OnBnClickedCancel()
{
	CDialog::EndDialog(IDCANCEL);
}

void CReagentSelectDlg::OnBnClickedOk()
{
	if(m_nCurrentSelectedReagent == 0)
	{
		if(MessageBox(_T("û��ѡ���κ��Լ���ȷ���˳��󲻻�ı�ԭ�����ã�\n�Ƿ�ȷ���˳���"),_T("��ѡ��ȷ��"),MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
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
	// �����ϸ��Ϣ
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();

	*pResult = 0;
}

void CReagentSelectDlg::OnDtnDateChProductDateFrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// �����ϸ��Ϣ
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
		// ��ǰ�Լ�ID��Ϊ��ǰѡ���������
		m_nCurrentSelectedReagent = pItem->GetReagentKey();
		_RefreshDetails();
	}

	*pResult = 0;
}
