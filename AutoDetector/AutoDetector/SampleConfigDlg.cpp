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
	// �������ýڵ������ʼ��
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
	// ���ؼ���ʼ��

	// ����š���Ͽ�͡����ǰ׺���༭�����ݳ�ʼ��
	m_ctrlCombSerials.InsertString(0,_T("����(1��2��3)"));
	m_ctrlCombSerials.InsertString(1,_T("����(A1��B2)"));
	m_ctrlCombSerials.InsertString(2,_T("����+����"));
	m_ctrlCombSerials.SetCurSel(0);
	m_ctrlEditNumPrefix.SetWindowText(_T("����"));

	// ���������͡���Ͽ����ݳ�ʼ��
	CSampleTable * pSampleTable = CSampleTable::GetInstance();
	vector<CString> TypeList;
	pSampleTable->GetDistinctType(TypeList);
	for(int i=0; i< (int) TypeList.size(); i++)
	{
		m_ctrlCombTypes.InsertString(i,TypeList[i]);
	}

	// ��ϡ�ͱ������༭���ʼ��
	m_ctrlEditDilution.SetWindowText(_T("1"));

	// �����Ŀ��Ͽ��ʼ��
	CReagentTable * pReagentTable = CReagentTable::GetInstance();

	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_RegentConfigNodes.begin();
	vector<CString> ProjectList;
	while(iterReagent != m_RegentConfigNodes.end())
	{
		// ȡ�ø��Լ�ID���Լ���ϸ��Ϣ
		ReagentRecord regentInfo;
		regentInfo = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
		int nCountOfComboxItem = m_ctrlListProject.GetCount();

		// ����Ͽ�ĩβ��Ӹ��Լ������Ŀ����
		CString temp;
		temp.Format(_T("%d. %s"),iterReagent->first,regentInfo.ProjectName);
		m_ctrlListProject.InsertString(nCountOfComboxItem,temp);

		// ��Ͽ��Item������ָ���Լ���λ��
		m_ctrlListProject.SetItemData(nCountOfComboxItem,static_cast<DWORD_PTR>(iterReagent->second->pos));

		// ����Ŀ���Ʊ�����������Ϊitemdata����Ͽ�ѡ��
		ProjectList.push_back(temp);
		iterReagent++;
	}

	// ��ʼһ����������
	m_ctrlConfigList.InsertColumn(0,_T("����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(1,_T("�������"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(2,_T("��������"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(3,_T("ϡ�ͱ���"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(4,_T("�ͼ쵥λ����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(5,_T("�ͼ���"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(6,_T("�ͼ�����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(7,_T("�����Ŀ����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(8,_T("�ظ�����"),LVCFMT_CENTER);
	m_ctrlConfigList.InsertColumn(9,_T("����Ũ��"),LVCFMT_CENTER);
	m_ctrlConfigList.SetItemHeight(24);
	
	m_ctrlConfigList.m_GridFrameColor = RGB(169,199,240);
	m_ctrlConfigList.m_SelectedBackColor = RGB(230, 241, 255);

	for(int i=0; i<m_ctrlConfigList.GetColumnCount(); i++)
	{
		m_ctrlConfigList.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	}

	// ��ʼһ��������
	map<Sample_Pos,SampleConfigNode *>::iterator iter = m_SampleConfigNodes.begin();
	int idxItem = 0;
	while(iter != m_SampleConfigNodes.end())
	{
		vector<CSampleConfigListItem *> vectItem;

		// ����confignode��������ת��ΪCSampleConfigListItem�������ݺ���
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
		// ���ظ���������ʼ��
		m_ctrlEditRepeations.SetWindowText(_T("1"));
	}
	
	// ����Ũ�ȿؼ�
	CWnd * pWnd = GetDlgItem(IDC_Title_StartConc);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_Title_Increament);
	pWnd->EnableWindow(FALSE);
	m_ctrlEditStartConc.EnableWindow(FALSE);
	m_ctrlEditIncreament.EnableWindow(FALSE);
	m_ctrlSpinBtnStartConc.EnableWindow(FALSE);
	m_ctrlSpinBtnIncreament.EnableWindow(FALSE);

	// Ũ�ȵ�λ�ؼ���ʾ
	pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(_T(""));
	pWnd = GetDlgItem(IDC_Value_Unit2);
	pWnd->SetWindowText(_T(""));
	return TRUE;
}

void CSampleConfigDlg::OnBnClickedOk()
{
	// ��������ѡ�е��������ýڵ�
	map<Sample_Pos,SampleConfigNode *>::iterator iterSampleConfigNode = m_SampleConfigNodes.begin();	
	while(iterSampleConfigNode != m_SampleConfigNodes.end())
	{
		// ����ÿ�������ڵ�ġ������Ŀ��Ϣ�����飬ɾ������δ��ʼ�ļ����Ŀ��Ϣ�������ظ������ۼ�
		map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iterTestProj = iterSampleConfigNode->second->TestProjArr.begin();
		// ��Ҫɾ����key�������������������ɾ��
		vector<Reagent_Pos> DeleteNodeKey;
		while(iterTestProj != iterSampleConfigNode->second->TestProjArr.end())
		{
			if(iterTestProj->second.TestUintKeys < 1)
				DeleteNodeKey.push_back(iterTestProj->first);	
			iterTestProj++;
		}
		// ɾ�������б����keyֵ��Ӧ��Ԫ��
		for (int i=0; i<(int)DeleteNodeKey.size(); i++)
		{
			iterSampleConfigNode->second->TestProjArr.erase(DeleteNodeKey[i]);
		}
		iterSampleConfigNode++;
	}

	for (int i=0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		// ȡ���б�ListItem����
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *> (m_ctrlConfigList.GetItemData(i));

		// ȡ���������ýڵ�ָ��
		map<Sample_Pos,SampleConfigNode *>::iterator iterSampleConfigNode = m_SampleConfigNodes.find(pItem->GetPosNum());

		// ʹ������ListItem����ConfigNode
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
	// ��ʼ���������
	CString strPrefix,strSerial;
	m_ctrlEditNumPrefix.GetWindowText(strPrefix);

	// �����Ͽ�ǰѡ������
	int indexSerials = m_ctrlCombSerials.GetCurSel();

	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(i));

		// �ж������Ͽ�ǰѡ������
		switch(indexSerials)
		{
		case 0:		// ����(1��2��3)
			strSerial.Format(_T("%d"),i + 1);
			break;
		case 1:		// ����(A1��B2)
			pItem->GetCellContex(colPos,strSerial);
			break;
		case 2:		// ����+����
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
	// �����͡���Ͽ�ѡ����仯��Ӧ
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
	// �����͡���Ͽ����ݷ����仯��Ӧ
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
	//��ϡ�ͱ������༭�����ݷ����仯��Ӧ
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
	//��ϡ�ͱ�����΢����ť������Ӧ����
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strDilution;
	m_ctrlEditDilution.GetWindowText(strDilution);

	double dDilution;
	dDilution = _tstof(strDilution);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		dDilution += 1.0;
	}
	else if(pNMUpDown->iDelta == 1)		// �����ֵΪ1,�������Spin�����¼�ͷʱ
	{
		dDilution -= 1.0;
	}

	strDilution.Format(_T("%g"),dDilution);
	m_ctrlEditDilution.SetWindowText(strDilution);
	*pResult = 0;
}

void CSampleConfigDlg::OnEnChangeValueDept()
{
	//���ͼ쵥λ�������仯��Ӧ����
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
	//���ͼ��ˡ������仯��Ӧ����
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
	// ��Ŀ�б�ѡ��״̬�ı���Ϣ��Ӧ��ѡ��ָ����Ŀ�������б�����
	int idxProject = m_ctrlListProject.GetCurSel();
	multimap<int,int>::iterator iterSearch = m_mapSearchList.find(idxProject);

	// ��ȡ������ԭlistitem��ѡ��״̬
	for(int i = 0; i < m_ctrlConfigList.GetItemCount(); i++)
	{
 		m_ctrlConfigList.SetItemState(i,0, LVIS_SELECTED|LVIS_FOCUSED);
	}

	// ���ݲ�ѯ�����������Ŀ������listitem
	while(iterSearch != m_mapSearchList.end() && iterSearch->first == idxProject)
	{		
		m_ctrlConfigList.SetItemState(iterSearch->second,LVIS_SELECTED, LVIS_SELECTED);
		iterSearch ++;
	}

	// Ũ�ȵ�λ��ʾ����
	CString strUnit = _GetUnitConc(idxProject);
	CWnd * pWnd = GetDlgItem(IDC_Value_Unit);
	pWnd->SetWindowText(strUnit);
	pWnd = GetDlgItem(IDC_Value_Unit2);
	pWnd->SetWindowText(strUnit);
}

void CSampleConfigDlg::OnProjectChkChange()
{
	// ��Ŀ�б�check״̬�ı���Ϣ��Ӧ
	int idxProject = m_ctrlListProject.GetCurSel();
	int checkState = m_ctrlListProject.GetCheck(idxProject);
	switch(checkState)
	{
	case 0:	// �������δѡ��ʱ����ʾҪɾ������Ŀ
		{
			_DeleteProject(idxProject);
			break;
		}

	case 1:	// �������ѡ��ʱ����ʾҪ��Ӵ���Ŀ
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
	//���ͼ����ڡ��仯��Ӧ����
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
	//����������Ũ�ȸ�ѡ�򡱵�����Ӧ����
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
	// ���ظ��������༭��仯��Ӧ����
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
	//���ظ�������΢����ť������Ӧ����

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strRepeations;
	m_ctrlEditRepeations.GetWindowText(strRepeations);

	int nRepeations;
	nRepeations = _tstoi(strRepeations);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		nRepeations += 1;
	}
	else if(pNMUpDown->iDelta == 1)		// �����ֵΪ1,�������Spin�����¼�ͷʱ
	{
		nRepeations -= 1;
	}

	strRepeations.Format(_T("%d"),nRepeations);
	m_ctrlEditRepeations.SetWindowText(strRepeations);
	*pResult = 0;
}

void CSampleConfigDlg::OnDeltaposSpinStartconc(NMHDR *pNMHDR, LRESULT *pResult)
{
	//����ʼŨ�ȡ�΢����ť������Ӧ����

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strStartConc;
	m_ctrlEditStartConc.GetWindowText(strStartConc);

	double dStartConc;
	dStartConc = _tstof(strStartConc);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		dStartConc += 0.1;
	}
	else if(pNMUpDown->iDelta == 1)		// �����ֵΪ1,�������Spin�����¼�ͷʱ
	{
		dStartConc -= 0.1;
	}

	strStartConc.Format(_T("%g"),dStartConc);
	m_ctrlEditStartConc.SetWindowText(strStartConc);
	*pResult = 0;
}

void CSampleConfigDlg::OnDeltaposSpinIncreament(NMHDR *pNMHDR, LRESULT *pResult)
{
	//��Ũ��������΢����ť������Ӧ����

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString strIncreament;
	m_ctrlEditIncreament.GetWindowText(strIncreament);

	double dIncreament;
	dIncreament = _tstof(strIncreament);

	CString strValue;
	if(pNMUpDown->iDelta == -1)			// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		dIncreament += 0.01;
	}
	else if(pNMUpDown->iDelta == 1)		// �����ֵΪ1,�������Spin�����¼�ͷʱ
	{
		dIncreament -= 0.01;
	}

	strIncreament.Format(_T("%g"),dIncreament);
	m_ctrlEditIncreament.SetWindowText(strIncreament);
	*pResult = 0;
}

void CSampleConfigDlg::OnEnChangeEditStartconc()
{
	//������Ũ���������༭�����ݷ����仯��Ӧ
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
	//������Ũ���������༭�����ݷ����仯��Ӧ
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
	// ɾ��ĳ��Ŀ

	// ��������ѡ��״̬�������ýڵ�
	map<Sample_Pos,SampleConfigNode *>::iterator iterSample = m_SampleConfigNodes.begin();
	while(iterSample != m_SampleConfigNodes.end())
	{
		// �ҵ�����ͬ������б�����
		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterListItemData = m_mapListItemData.find(iterSample->first);
		// �Ƿ���Ҫ��ӱ�־
		bool IsNeedAdd = true;
		// ��������б�����ָ�룬�Ա��������б�����ʱ��������������Ϣ
		CSampleConfigListItem * pSourceData = iterListItemData->second;
		// �����ҵ�����ͬ����λ��������б�����
		while(iterListItemData != m_mapListItemData.end() && iterListItemData->first == iterSample->first)
		{
			CSampleConfigListItem * pItem = iterListItemData->second;

			// ���ĳ������δ�����κ���Ŀ��Ϣ����ֱ��������Ŀ��Ϣ
			if(pItem->GetProjectIndex() < 0)
			{
				// ������Ŀ����
				pItem->SetProjectIndex(newProjecIndex);

				// �����ظ�����
				CString strRepeat;
				m_ctrlEditRepeations.GetWindowText(strRepeat);
				int repeat = _tstoi(strRepeat);
				pItem->SetRepeat(repeat);
				IsNeedAdd = false;
				break;
			}

			// ����������Ѿ�����˸���Ŀ�����˳���ѭ��
			if(pItem->GetProjectIndex() == newProjecIndex)
			{
				IsNeedAdd = false;
				break;
			}

			iterListItemData++;
		}
		if( IsNeedAdd)
		{
			// �������б����ݣ���ԭ���ݿ���
			CSampleConfigListItem * pNewItem = new CSampleConfigListItem(*(pSourceData));
			pNewItem->SetProjectIndex(newProjecIndex);

			// �����ظ�����
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
	// ɾ��ĳ��Ŀ
	multimap<int/*idxProjec*/,int/*idxListItem*/>::iterator iterSearch = m_mapSearchList.find(idxDelProject);

	vector<int> vctDeleteRows;
	while(iterSearch != m_mapSearchList.end() && iterSearch->first == idxDelProject)
	{
		CSampleConfigListItem * pItem = reinterpret_cast<CSampleConfigListItem *>(m_ctrlConfigList.GetItemData(iterSearch->second));
		Sample_Pos pos = pItem->GetPosNum();

		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterLow = m_mapListItemData.lower_bound(pos);
		multimap<Sample_Pos,CSampleConfigListItem*>::iterator iterUp = m_mapListItemData.upper_bound(pos);

		// ���������ֻ��һ�������Ŀ,����Ŀ����������Ũ�ȡ����ظ���������Ϊ����
		if(iterLow == (--iterUp))
		{
			CSampleConfigListItem* pItemData = iterLow->second;
			pItemData->SetProjectIndex(-1);
			pItemData->SetThoeryConc(-1);
			pItemData->SetRepeat(-1);
		}
		// ����ֱ��ɾ����������
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
	// ������Զ��������Ũ��
	if(IsAdd)
	{
		// ȡ���Զ�������ʼֵ������ֵ
		CString strStartConc,strIncreament;
		m_ctrlEditStartConc.GetWindowText(strStartConc);
		m_ctrlEditIncreament.GetWindowText(strIncreament);
		double dStartConc = _tstof(strStartConc);
		double dIncreament = _tstof(strIncreament);

		// ѭ���Զ�����б�ѡ����Ŀ������Ũ��
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
	// �����ɾ������Ũ��
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
	// ȡ��ָ����������Ŀ��Ũ�ȵ�λ
	// 1.������Ŀ������ȡ���Լ�λ��
	Reagent_Pos pos = static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(idxProject));
	// 2.�����Լ�λ�ã�ȡ���Լ�ID
	map<Reagent_Pos,const ReagentConfigNode *>::iterator iterReagent = m_RegentConfigNodes.find(pos);
	// 3.�����Լ�ID��ȡ���Լ���¼��Ϣ
	CReagentTable * pReagentTable = CReagentTable::GetInstance();
	ReagentRecord record = pReagentTable->GetRecordInfo(iterReagent->second->ReagentID);
	return record.Unit;
}

void CSampleConfigDlg::_ListToConfig(const CSampleConfigListItem *pItem,SampleConfigNode *pNode)
{
	// �������б�ListItem����ConfigNode

	SampleRecord recSample;
	pItem->GetSampleInfo(recSample);
	CSampleTable *pSampleTable = CSampleTable::GetInstance();

	// �������IDС��1��˵����δ����������Ϣ����Ҫ�����ݿ��в���������
	if(pNode->RecordID < 1)
	{
		// ���б�������ȡ��������Ϣ
		// �����ݿ�����¼�¼��������ID
		pSampleTable->AddNewRecord(recSample);
		pNode->RecordID = recSample.ID;		// ����ID
		pNode->IsSelected = false;			// ����Ϊ���Ѿ�����������Ϣ��
		pNode->IsSetingSample = true;		// ��ѡ״̬ȡ��
		pNode->IsStandard = false;			// ����Ϊ���Ǳ�׼Ʒ��
		pNode->StdPointIndex = -1;			// ��׼������-1
	}
	else
		// ����������ݿ�
		pSampleTable->UpdateRecord(recSample);

	// �����Ŀ����
	int idxProject = pItem->GetProjectIndex();
	// ���û�м����Ŀ����������Ŀ��Ϣ
	if(idxProject < 0)
		return;

	Reagent_Pos pos = static_cast<Reagent_Pos>(m_ctrlListProject.GetItemData(idxProject));
	// ������ʱ���ñ����������Ŀ���飩����Ϊ����̫��
	map<Reagent_Pos,SampleConfigNode::TestProjInfo> & TestProjecList = pNode->TestProjArr;
	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iteProjInfo = TestProjecList.find(pos);

	// ����¼����Ŀ�����ڣ�������Ȼ���ڣ������Ѿ��ڲ��Խ����У��򴴽��¼����Ŀ�ڵ�
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
	// ����ϲ���һ����Ŀ��Ϣ�ڵ���
	else
	{
		// �ظ������ۼ�
		iteProjInfo->second.Repetitions += pItem->GetRepeat();
		// ����Ũ�Ȳ��޸�
	}

}

void CSampleConfigDlg::_ConfigToList(const SampleConfigNode *pNode,vector <CSampleConfigListItem*> & ItemList)
{
	// ���������������ýڵ�ConfigNode�����б�ListItem

	// ȡ�����ýڵ�����Ŀ����,������ռ�
	int nCountOfProject = (int)pNode->TestProjArr.size();
	if(nCountOfProject <= 1)
		ItemList.resize(1);
	else
		ItemList.resize(nCountOfProject);

	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::const_iterator iterTestProjArr = pNode->TestProjArr.begin();
	for(int iItem = 0; iItem < (int) ItemList.size(); iItem++)
	{
		ItemList[iItem] = new CSampleConfigListItem();
		// ����λ������
		ItemList[iItem]->SetPosNum(pNode->PosNum);
		
		if(pNode->IsSetingSample == false)
			break;;

		// ������¼��Ϣ
		CSampleTable *pSampleTable = CSampleTable::GetInstance();
		SampleRecord record = pSampleTable->GetRecordInfo(pNode->RecordID);
		ItemList[iItem]->SetSampleInfo(record);
		
		// �����Ŀ��Ϣ
		if(iterTestProjArr != pNode->TestProjArr.end())
		{
			// �����Լ�λ�������ҵ��������Ŀ���б���ж�Ӧ����
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
