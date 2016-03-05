#include "StdAfx.h"
#include "SampleConfigListItem.h"
#include "SampleAreaData.h"

ListColumn CSampleConfigListItem::SortColumn = colPos;	// ��������
bool CSampleConfigListItem::IsAsc = false;				// �Ƿ������ţ�true�ǣ�falseΪ����

CSampleConfigListItem::CSampleConfigListItem(void)
{
	m_Repeat = -1;
	m_dTheoryConc = - 1;
	m_idxTestProject = -1;
}

CSampleConfigListItem::~CSampleConfigListItem(void)
{
}

bool CSampleConfigListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CSampleConfigListItem * pData = static_cast<const CSampleConfigListItem*>(pItemData);
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colPos:					// ����λ
	case colSampleNo:				// �������
	case colType:					// ��������
	case colDilution:				// ϡ�ͱ���
	case colDept:					// �ͼ쵥λ����
	case colApplicant:				// �ͼ���
	case colApplyDate:				// �ͼ�����
		{
			if(m_PosNum == pData->GetPosNum())
				return true;
		}
		break;
	case colProjecName:				// �����Ŀ����
	case colRepeat:					// �ظ����Դ���
	case colTheoryConc:				// ����Ũ��
	default:
		break;
	}
	return false;
}

void CSampleConfigListItem::SetPosNum(const Sample_Pos &PosNum)
{
	m_PosNum = PosNum;							// ����λ���
}

UINT CSampleConfigListItem::GetCellContex(const int &ColumnIndex,
										  CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	CSampleAreaData * pSampleAreaData = CSampleAreaData::GetInstance();
	COleDateTime tempDate;
	switch(colIndex)
	{
	case colPos:								// ����λ
		contex = pSampleAreaData->GetCharCoordByNum(m_PosNum);
		break;
	case colSampleNo:							// �������
		contex = m_SampleInfo.SampleNo;
		break;
	case colType:								// ��������
		contex = m_SampleInfo.Type;
		break;
	case colDilution:							// ϡ�Ͷ�
		contex.Format(_T("%g"),m_SampleInfo.Dilution);
		break;
	case colDept:								// �ͼ쵥λ
		contex = m_SampleInfo.Dept;
		break;
	case colApplicant:							// �ͼ���
		contex = m_SampleInfo.Applicant;
		break;
	case colApplyDate:							// �ͼ�����
		tempDate = m_SampleInfo.ApplyDate;
		contex = tempDate.Format(_T("%Y-%m-%d"));
		break;
	case colProjecName:							// ��Ŀ����
		if(m_idxTestProject >= 0)
		{
			contex = m_OptionsOfTestProjects[m_idxTestProject];
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}
		break;
	case colRepeat:								// �ظ�����
		if(m_Repeat >= 0)
			contex.Format(_T("%d"), m_Repeat);
		break;
	case colTheoryConc:							// ����Ũ��
		if(m_dTheoryConc >=0)
			contex.Format(_T("%g"),m_dTheoryConc);
		break;
	default:
		break;
	}
	return 0;
}

ColumnProperties CSampleConfigListItem::GetColumnProperties(const int &ColumnIndex)
{
	if(m_SampleInfo.SampleNo == _T(""))
		return CanNotChange;
	
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	switch(colIndex)
	{
	case colPos:								// ����λ
		return CanNotChange;
	case colSampleNo:							// �������
		return DiretEditable;
	case colType:								// ��������
		return Optional;
	case colDilution:							// ϡ�Ͷ�
		return VariableSize;
	case colDept:								// �ͼ쵥λ
		return DiretEditable;
	case colApplicant:							// �ͼ���
		return DiretEditable;
	case colApplyDate:							// �ͼ�����
		return VariableDate;
	case colProjecName:							// ��Ŀ����
		return Optional;
	case colRepeat:								// �ظ�����
		if(m_idxTestProject < 0)				// �����δ������Ŀ���ƣ��򲻿ɱ༭����
			return CanNotChange;
		return VariableSize;
	case colTheoryConc:							// ����Ũ��
		if(m_idxTestProject < 0)				// �����δ������Ŀ���ƣ��򲻿ɱ༭����
			return CanNotChange;
		return VariableSize;
	default:
		break;
	}
	return CanNotChange;
}

bool CSampleConfigListItem::GetComboxProperties(const int &ColumnIndex,
												vector<CString>&strOptions,
												int &nDefaultSelectedIndex,
												bool &bIsEditable)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// ����������
	if(colIndex == colType)
	{
		for(int i=0; i < (int)m_OptionsOfSampleTypes.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfSampleTypes[i]);

			if(m_OptionsOfSampleTypes[i] == m_SampleInfo.Type)
				nDefaultSelectedIndex = i;
			bIsEditable = true;
		}
		return true;
	}

	// ��Ŀ������
	if(colIndex == colProjecName)
	{
		for(int i=0; i < (int)m_OptionsOfTestProjects.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfTestProjects[i]);
			nDefaultSelectedIndex = m_idxTestProject;
		}
		bIsEditable = false;
		return true;
	}

	return false;
}

void CSampleConfigListItem::OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// ����������
	if(colIndex == colType)
	{
		m_SampleInfo.Type = m_OptionsOfSampleTypes[newSelectedIndex];
	}

	// ��Ŀ������
	if(colIndex == colProjecName)
	{
		m_idxTestProject = newSelectedIndex;
	}
}

bool CSampleConfigListItem::GetSpinBtnProperties(const int &ColumnIndex,
													double &initial,
													double &lower, 
													double & upper,
													double &increament)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// ϡ�ͱ����У�����100000������1������2
	if(colIndex == colDilution)
	{
		initial = static_cast<double> (m_SampleInfo.Dilution);
		lower = 1;									
		upper = 100000;
		increament = 2 ;	
		return true;
	}

	// �ظ������У�����20������0������1
	if(colIndex == colRepeat)
	{
		initial = static_cast<double> (m_Repeat);
		lower = 0;									
		upper = 20;
		increament = 1;
		return true;
	}

	// ����Ũ���У�����100������0������01
	if(colIndex == colTheoryConc)
	{
		initial = static_cast<double> (m_dTheoryConc);
		lower = 0;									
		upper = 100;
		increament = 0.1;
		return true;
	}
	return false;
}

bool CSampleConfigListItem::GetDateCtrlProperties(const int &ColumnIndex,COleDateTime &initial)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// �ͼ�ʱ��
	if(colIndex == colApplyDate)
	{
		initial = m_SampleInfo.ApplyDate;
		return true;
	}
	return false;
}

void CSampleConfigListItem::OnDtnchange(const int &ColumnIndex,const COleDateTime & newDate)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// �ͼ�ʱ��
	if(colIndex == colApplyDate)
	{
		m_SampleInfo.ApplyDate = newDate;
		return;
	}
}

void CSampleConfigListItem::OnEnChange(const int &ColumnIndex,const CString & newContex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// �������
	if(colIndex == colSampleNo)
	{
		m_SampleInfo.SampleNo = newContex;
	}

	// �ͼ���
	if(colIndex == colApplicant)
	{
		m_SampleInfo.Applicant = newContex;
	}

	// �ͼ쵥λ
	if(colIndex == colDept)
	{
		m_SampleInfo.Dept = newContex;
	}

	// ϡ�ͱ���
	if(colIndex == colDilution)
	{
		m_SampleInfo.Dilution = _tstoi(newContex);
	}

	// �ظ�����
	if(colIndex == colRepeat)
	{
		m_Repeat = _tstoi(newContex);

		if(m_Repeat > 20)
			m_Repeat = 20;

		if(m_Repeat < 0)
			m_Repeat = 0;
	}

	// ����Ũ��
	if(colIndex == colTheoryConc)
	{
		m_dTheoryConc = _tstof(newContex);
	}
}

bool CSampleConfigListItem::IsBeforThan(const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CSampleConfigListItem * pData = static_cast<const CSampleConfigListItem*>(pItemData);

	// ��������ʽ
	bool NormalAscOrder;
	switch(SortColumn)
	{
	case colPos:
		if (m_PosNum ==  pData->GetPosNum())
		{
			// �������λ��ȣ��򰴼����Ŀ��������
			if(m_idxTestProject < pData->GetProjectIndex())
				return true;
			else
				return false;
		}
		else if(m_PosNum >  pData->GetPosNum())
			NormalAscOrder = true;
		else 
			NormalAscOrder = false;
		break;
	default:
		NormalAscOrder = false;
	}

	// �����˳��ʽ����Ż���������ʽ
	if(IsAsc)
		return NormalAscOrder;
	// ���򷵻ص���
	else
		return (!NormalAscOrder);
}

bool CSampleConfigListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colPos:
		break;
	default:
		return false;
	}

	// �����������ԭ������һ������ı�˳����ʽ
	if(CSampleConfigListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CSampleConfigListItem::IsAsc == true)
			CSampleConfigListItem::IsAsc = false;
		else
			CSampleConfigListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
	else
	{
		CSampleConfigListItem::SortColumn = colIndex;
	}
	return true;
}
