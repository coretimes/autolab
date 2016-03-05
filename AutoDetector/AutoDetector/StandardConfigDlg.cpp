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
	// �������ýڵ������ʼ��
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

	// ���ؼ���ʼ��

	//��׼Ʒ��Ϣ�ؼ���ʼ��
	_InitSampleInfoControls();

	// �����Ŀ��Ͽ��ʼ��
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_mapRegentConfigNodes.begin();
	int index = 0;
	while(iterReagent != m_mapRegentConfigNodes.end())
	{
		// ȡ�ø��Լ�ID���Լ���ϸ��Ϣ
		ReagentRecord regentInfo;
		regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
		
		// ����Ͽ�ĩβ��Ӹ��Լ������Ŀ����
		CString temp;
		temp.Format(_T("%d .%s(%s)"),iterReagent->first,regentInfo.ShortName,regentInfo.ProjectName);
		m_ComboxProjectName.InsertString(index,temp);
		
		// ��Ͽ��Item������ָ���Լ���λ��
		m_ComboxProjectName.SetItemData(index,static_cast<DWORD_PTR>(iterReagent->second->pos));
		
		// ������ǡ�����ӡ��Ĳ��ԣ����ǡ��޸ġ��Ĳ���
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

	// ���������б��ʼ��
	m_ctrlConfigList.InsertColumn(colPos,_T("����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colBatchNum,_T("��׼Ʒ����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colConcIndex,_T("��׼��"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(colRepeat,_T("������"),LVCFMT_CENTER);

	m_ctrlConfigList.SetColumnWidth(colPos,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlConfigList.SetColumnWidth(colProject,130);
	m_ctrlConfigList.SetColumnWidth(colBatchNum,110);
	m_ctrlConfigList.SetColumnWidth(colConcIndex,LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlConfigList.SetColumnWidth(colRepeat,LVSCW_AUTOSIZE_USEHEADER);

	m_ctrlConfigList.SetItemHeight(24);
	m_ctrlConfigList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlConfigList.m_SelectedBackColor = RGB(230, 241, 255);

	// �ظ������ؼ�
	m_SpinRepeation.SetBuddy((CWnd*)&m_EditRepeation);
	
	return TRUE;
}

void CStandardConfigDlg::_InitSampleInfoControls()
{
	// ��׼Ʒ����ֵ��ʾ�ؼ�
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

	// ��׼��Ũ�ȿؼ���ʼ��
	for(int i=0; i<10; i++)
	{
		m_ctrlStdPointValue[i].SetWindowText(_T("-"));
	}

	// ��Ч�ڵ������Ϊ0
	m_nNumOfValidConfigNode = 0;

	// ��׼Ʒ�������ñ��������
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
	// �����б�ÿһ��
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
// �������ܣ�
//    �������Ŀ����Ͽ����ݱ仯��Ӧ����
// ---------------------------------------------------------
void CStandardConfigDlg::OnCbnSelchangeComboxTestproject()
{
	// �����Ŀ��Ͽ�ǰѡ��������
	int indexCurrentRegent = m_ComboxProjectName.GetCurSel();
	// �ҳ���Ӧ���Լ�λ�ñ�ź��Լ�������Ϣ
	int ReagentPos = static_cast<int> (m_ComboxProjectName.GetItemData(indexCurrentRegent));
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_mapRegentConfigNodes.find(ReagentPos);

	// �����Լ����������Լ���Ϣ
	ReagentRecord regentInfo;
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);

	// �����Լ���Ϣ����Ŀ���Ʋ��Ҹ����������еı�׼Ʒ
	CString strProjectName = regentInfo.ProjectName;
	CStandardTable * pStandardTable = CStandardTable::GetInstance();
	m_vctStandardRecords.clear();
	m_ComboxBatchNum.ResetContent();
	if( pStandardTable->GetStandardsByProjecName(m_vctStandardRecords, strProjectName) == false)
	{
		// ���û�в�ѯ����׼Ʒ������ʾ����
		MessageBox(_T("���ݿ���û�д���Ŀ��׼Ʒ�����ݿ����"),_T("���ݴ���"),MB_ICONERROR|MB_OK);
		_InitSampleInfoControls();
		return;
	}

	CStandardTable::SortByBatchNumAsc(m_vctStandardRecords);

	// ����Ͽ�����ʾ���б�׼Ʒ����
	// Ĭ����������
	int DefaultIndex = -1;
	for(int i=0; i<(int)m_vctStandardRecords.size(); i++)
	{
		// ���������ӵĲ��ԣ��ұ�׼Ʒ���ŵ����Լ����ţ������������ΪĬ��
		if(m_IsAddStanderds == true && m_vctStandardRecords[i].BatchNum == regentInfo.BatchNum)
			DefaultIndex = i;

		// ������޸ĵĲ��ԣ��ұ�׼ƷID���ڱ�ID�������������ΪĬ��
		if(m_IsAddStanderds == false && m_vctStandardRecords[i].ID == m_mapSampleConfigNodes.begin()->second->RecordID)
			DefaultIndex = i;

		m_ComboxBatchNum.InsertString(i,m_vctStandardRecords[i].BatchNum);
	}

	// ���û�е����Լ����ŵı�׼Ʒ���ţ��������������ΪĬ������
	if(DefaultIndex < 0)
		DefaultIndex = (int)m_vctStandardRecords.size() - 1;
	m_ComboxBatchNum.SetCurSel(DefaultIndex);
	OnCbnSelchangeComboxBatchnum();
}

// ---------------------------------------------------------
// �������ܣ�
//    �����š���Ͽ����ݱ仯��Ӧ����
// ---------------------------------------------------------
void CStandardConfigDlg::OnCbnSelchangeComboxBatchnum()
{
	_InitSampleInfoControls();
	// ȡ�õ�ǰѡ����������
	int indexCurrenBatchNum = m_ComboxBatchNum.GetCurSel();
	// ȡ�õ�ǰ���ű�׼Ʒ��¼
	_DisplayCurrentStanderd(m_vctStandardRecords[indexCurrenBatchNum]);
	if(m_IsAddStanderds == true)
	{
		m_EditRepeation.SetWindowText(_T("1"));
		OnEnChangeEditRepeation();
	}
	// ����һ�γ�ʼ����Ϻ󣬰Ѵ˱�־����Ϊ����Ӳ��ԣ��Ժ�ĳ�ʼ��������Ӳ��Գ�ʼ����ȫ��ͬ
	m_IsAddStanderds = true;
}

void CStandardConfigDlg::_DisplayCurrentStanderd(const StandardRecord &record)
{
	CString strTemp;
	COleDateTime tempDate;
	CWnd * pWnd;
	// ��������
	pWnd = GetDlgItem(IDC_Value_ProductDate);
	tempDate = record.ProductionDate;
	strTemp = tempDate.Format(_T("%Y-%m-%d"));
	pWnd->SetWindowText(strTemp);

	// ʧЧ����
	pWnd = GetDlgItem(IDC_Value_ExpirationDate);
	tempDate = record.ExpirationDate;
	strTemp = tempDate.Format(_T("%Y-%m-%d"));
	pWnd->SetWindowText(strTemp);

	// �����
	pWnd = GetDlgItem(IDC_Value_RatedTestTimes);
	strTemp.Format(_T("%d"),record.RatedTestTimes);
	pWnd->SetWindowText(strTemp);

	// ʣ�����
	pWnd = GetDlgItem(IDC_Value_RemainTimes);
	strTemp.Format(_T("%d"),record.RemainTimes);
	pWnd->SetWindowText(strTemp);

	// Ũ�ȵ�λ
	pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(record.Unit);

	// ��׼�����
	pWnd = GetDlgItem(IDC_Value_CountOfStdPoints);
	strTemp.Format(_T("%d"),record.CountOfStdPoints);
	pWnd->SetWindowText(strTemp);

	// �����ر�ע��5~10
	for (int i=0; i<10; i++)
	{
		m_ctrlStdPointTitle[i].ShowWindow(SW_HIDE);
		m_ctrlStdPointValue[i].ShowWindow(SW_HIDE);;
	}

	// ��׼��
	for(int i = 0; i < record.CountOfStdPoints; i++)
	{
		int indexConc = i + 1;
		strTemp.Format(_T("%g"),record.StdConc[i]);
		m_ctrlStdPointValue[i].SetWindowText(strTemp);
		m_ctrlStdPointTitle[i].ShowWindow(SW_SHOW);
		m_ctrlStdPointValue[i].ShowWindow(SW_SHOW);
	}

	// �ӡ���׼��������롰ѡ������ýڵ��������ѡ���С�ĸ�����ˢ���б�
	if(record.CountOfStdPoints < m_nCountOfConfigNode)
		m_nNumOfValidConfigNode = record.CountOfStdPoints;
	else 
		m_nNumOfValidConfigNode = m_nCountOfConfigNode;

	// Ũ�ȵ�����ѡ���
	vector<CString> arrOptionsConcIndex;
	for(int i=0; i < record.CountOfStdPoints; i++)
	{
		// ��������    Ũ�ȵ�����      �ַ�
		// 0        ->      1      ->    A
		// 1        ->      2      ->    B
		// ... ...
		CString strTemp;
		strTemp.Format(_T("%c"),(i+1) + 64);		// ����������0��ʼ����Ũ�ȵ�������1��ʼ������Ӧ����i+1
		arrOptionsConcIndex.push_back(strTemp);
	}

	// �����б����ݸ���
	for(int i = 0; i < m_nNumOfValidConfigNode; i++)
	{
		// ��������    Ũ�ȵ����� 
		// 0        ->      1     
		// 1        ->      2     
		// ... ...

		CString strTemp;
		// �����Ŀ
		m_ComboxProjectName.GetWindowText(strTemp);
		m_vctListItemData[i]->SetProject(strTemp);

		// ��¼ID������
		m_ComboxBatchNum.GetWindowText(strTemp);
		m_vctListItemData[i]->SetBatchNum(strTemp);

		// Ũ�ȵ�������ֵ
		if(m_IsAddStanderds == true)
			m_vctListItemData[i]->SetConcIndex(i);
		// Ũ�ȵ�����ѡ��
		m_vctListItemData[i]->SetOptionsConcIndex(arrOptionsConcIndex);
	}
	m_ctrlConfigList.RedrawAllItems();
}

// ---------------------------------------------------------
// �������ܣ�
//    ��ת��ťλ�ñ仯��Ϣ��Ӧ����
// ---------------------------------------------------------
void CStandardConfigDlg::OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString strRepeations;
	m_EditRepeation.GetWindowText(strRepeations);

	int nTimes;
	nTimes = _ttoi(strRepeations);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		nTimes++;
	}
	else if(pNMUpDown->iDelta == 1)		// �����ֵΪ1,�������Spin�����¼�ͷʱ
	{
		nTimes--;
	}

	strRepeations.Format(_T("%d"),nTimes);
	m_EditRepeation.SetWindowText(strRepeations);
	*pResult = 0;
}

// ---------------------------------------------------------
// �������ܣ�
//    �ظ������༭��ֵ�����仯ʱ����Ӧ����
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
	// �����Ŀ��Ͽ�����ŵ�ǰѡ��������
	int indexCurrentRegent = m_ComboxProjectName.GetCurSel();
	if (indexCurrentRegent < 0)
		return;
	int indexBatchNum = m_ComboxBatchNum.GetCurSel();
	// �ҳ���Ӧ���Լ�λ�ñ��
	Reagent_Pos ReagentPos = static_cast<Reagent_Pos> (m_ComboxProjectName.GetItemData(indexCurrentRegent));

	// ��׼Ʒ��Ϣ
	pNode->RecordID = m_vctStandardRecords[indexBatchNum].ID;			// ��׼ƷID
	pNode->IsSelected = false;											// ��ѡ״̬ȡ��
	pNode->IsSetingSample = true;										// ����Ϊ���Ѿ�����������Ϣ��
	pNode->IsStandard = true;											// ����Ϊ���Ǳ�׼Ʒ��
	pNode->StdPointIndex = pItem->GetConcIndex();						// ��׼������

	// �����Ŀ��Ϣ
	pNode->TestProjArr.clear();											// ��׼Ʒֻ������һ����Ŀ
	SampleConfigNode::TestProjInfo newproj;								// ������Ŀ��Ϣ
	newproj.ReagentPos = ReagentPos;									// �Լ����Լ�λ�ñ��
	newproj.Repetitions = pItem->GetRepeat();							// �ظ�����
	newproj.TestUintKeys = 0;											// ��δ��ʼ���ԣ���ֵ0
	pNode->TestProjArr.insert(pair<Reagent_Pos,SampleConfigNode::TestProjInfo>(ReagentPos,newproj));
}

void CStandardConfigDlg::_ConfigToList(const SampleConfigNode *pNode,CStandardConfigListItem * pItem)
{
	pItem->SetPosNum(pNode->PosNum);
	// ���������±�׼Ʒ��Ϣ����������������Ϣ
	if(m_IsAddStanderds)
		return;

	pItem->SetRepeat(pNode->TestProjArr.begin()->second.Repetitions);
	pItem->SetConcIndex(pNode->StdPointIndex);
}

BOOL CStandardConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// ȡ���س�����esc���Ĺ���
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
