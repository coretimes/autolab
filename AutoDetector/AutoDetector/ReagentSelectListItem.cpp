#include "StdAfx.h"
#include "ReagentSelectListItem.h"

ListColumn CReagentSelectListItem::SortColumn = colProject;	// ��������
bool CReagentSelectListItem::IsAsc = false;					// �Ƿ������ţ�true�ǣ�falseΪ����

CReagentSelectListItem::CReagentSelectListItem(	const Reagent_Key & key,
												const CString & strBatchNum,
						 					    const ProjectFullName & ProjectName,
												const COleDateTime & dtProductDate)
{
	m_ReagentKey = key;
	m_strBatchNum = strBatchNum;
	m_ProjectName = ProjectName;
	m_dtProductDate = dtProductDate;
}

CReagentSelectListItem::~CReagentSelectListItem(void)
{
}

UINT CReagentSelectListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	switch(ColumnIndex)
	{
	case colBatchNum:								// �Լ�����
		contex = m_strBatchNum;
		break;
	case colProject:									// ��Ŀ����
		contex.Format(_T("%s(%s)"),m_ProjectName.ShortName,m_ProjectName.ProjectName);				
		break;
	case colProductDate:								// ��Ŀ����
		{
			COleDateTime tempDate = m_dtProductDate;
			contex = tempDate.Format(_T("%Y-%m-%d"));
		}
		break;
	default:
		break;
	}
	return 0;
}

bool CReagentSelectListItem::IsBeforThan(const CListItemData * pItemData)
{
	// �����������Ϊ�գ��򷵻�false������Ҫ����˳��
	if(pItemData == NULL)
		return false;
	const CReagentSelectListItem * pData = static_cast<const CReagentSelectListItem*>(pItemData);

	// ��������ʽ
	bool NormalAscOrder;
	switch(SortColumn)
	{
	case colProject:
		if (m_ProjectName.ShortName ==  pData->GetProjectName().ShortName)
		{
			// �������λ����ȣ����Լ�λ����������
			if(m_strBatchNum < pData->GetBatchName())
				return true;
			else
				return false;
		}

		else if(m_ProjectName.ShortName >  pData->GetProjectName().ShortName)
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

bool CReagentSelectListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colProject:
		break;
	default:
		return false;
	}

	// �����������ԭ������һ������ı�˳����ʽ
	if(CReagentSelectListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CReagentSelectListItem::IsAsc == true)
			CReagentSelectListItem::IsAsc = false;
		else
			CReagentSelectListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
	else
	{
		CReagentSelectListItem::SortColumn = colIndex;
	}
	return true;
}
