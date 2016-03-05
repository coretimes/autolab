#include "StdAfx.h"
#include "StandardConfigListItem.h"
#include "SampleAreaData.h"

CStandardConfigListItem::CStandardConfigListItem(void)
{
}

CStandardConfigListItem::~CStandardConfigListItem(void)
{
}

void CStandardConfigListItem::SetPosNum(const Sample_Pos &PosNum)
{
	m_PosNum = PosNum;							// ����λ���
}

UINT CStandardConfigListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	CSampleAreaData * pSampleAreaData = CSampleAreaData::GetInstance();
	switch(colIndex)
	{
	case colPos:					// ����λ
		contex = pSampleAreaData->GetCharCoordByNum(m_PosNum);
		break;
	case colProject:				// ��Ŀ����
		contex = m_strProject;
		break;
	case colBatchNum:				// ����
		contex = m_strBatchNum;
		break;
	case colConcIndex:				// Ũ�ȵ�����
		if(m_ConcIndex >= 0 && m_ConcIndex < (int)m_OptionsOfConcIndex.size())		// ���ڿ����ж�����������ýڵ㣬�����е��б������û������ֵ���������п���Ϊ��ʼֵ0
			contex = m_OptionsOfConcIndex[m_ConcIndex];							// m_ConcIndex�Ǵ�0��ʼ�ģ������������0��ʼ
		else
			contex = _T("");
		break;
	case colRepeat:					// �ظ�����
		if(m_Repeat > 0)
			contex.Format(_T("%d"), m_Repeat);
		else 
			contex = _T("");
		break;
	default:
		break;
	}
	return 0;
}

ColumnProperties CStandardConfigListItem::GetColumnProperties(const int &ColumnIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	if(colIndex ==  colPos)					// ����λ
		return DeletePrompt;

	if(m_strBatchNum == _T(""))
		return CanNotChange;

	switch(colIndex)
	{
	case colProject:				// �����Ŀ
		return CanNotChange;
	case colBatchNum:				// ����
		return CanNotChange;
	case colConcIndex:				// Ũ�ȵ�����
		return Optional;
	case colRepeat:					// �ظ�����
		return VariableSize;
	default:
		break;
	}
	return CanNotChange;
}

bool CStandardConfigListItem::GetComboxProperties(const int &ColumnIndex,
												   vector<CString>&strOptions,
												   int &nDefaultSelectedIndex,
												   bool &bIsEditable)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// Ũ�ȵ�������
	if(colIndex == colConcIndex)
	{
		for(int i=0; i < (int)m_OptionsOfConcIndex.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfConcIndex[i]);
		}
		// Ũ�ȵ�������0��ʼ
		nDefaultSelectedIndex = m_ConcIndex;
		bIsEditable = false;
		return true;
	}
	return false;
}

void CStandardConfigListItem::OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// Ũ�ȵ�������0��ʼ
	if(colIndex == colConcIndex)
	{
		m_ConcIndex = newSelectedIndex;
	}
}

void CStandardConfigListItem::OnEnChange(const int &ColumnIndex,const CString & newContex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// Ũ�ȵ�������1��ʼ������Ͽ�������0��ʼ
	if(colIndex == colRepeat)
	{
		m_Repeat = _tstoi(newContex);

		if(m_Repeat > 20)
			m_Repeat = 20;

		if(m_Repeat < 1)
			m_Repeat = 1;
	}
}

bool CStandardConfigListItem::GetSpinBtnProperties(const int &ColumnIndex,
													double &initial,
													double &lower, 
													double & upper,
													double &increament)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// �ظ������У�����20������1������1
	if(colIndex == colRepeat)
	{
		initial = static_cast<double> (m_Repeat);
		lower = 1;									
		upper = 20;
		increament = 1;
		return true;
	}
	return false;
}
