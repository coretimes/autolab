#include "StdAfx.h"
#include "StandardsSelectListItem.h"

ListColumn CStandardsSelectListItem::SortColumn = colProject;	// ��������
bool CStandardsSelectListItem::IsAsc = false;					// �Ƿ������ţ�true�ǣ�falseΪ����

CStandardsSelectListItem::CStandardsSelectListItem(	const Standard_Key & key,
											   const CString & strBatchNum,
											   const ProjectFullName & ProjectName,
											   const COleDateTime & dtProductDate)
{
	m_StandardsKey = key;
	m_strBatchNum = strBatchNum;
	m_ProjectName = ProjectName;
	m_dtProductDate = dtProductDate;
}

CStandardsSelectListItem::~CStandardsSelectListItem(void)
{
}

UINT CStandardsSelectListItem::GetCellContex(const int &ColumnIndex,CString & contex)
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

bool CStandardsSelectListItem::IsBeforThan(const CListItemData * pItemData)
{
	// �����������Ϊ�գ��򷵻�false������Ҫ����˳��
	if(pItemData == NULL)
		return false;
	const CStandardsSelectListItem * pData = static_cast<const CStandardsSelectListItem*>(pItemData);

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

bool CStandardsSelectListItem::ChangeSortMode(const int & col)
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
	if(CStandardsSelectListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CStandardsSelectListItem::IsAsc == true)
			CStandardsSelectListItem::IsAsc = false;
		else
			CStandardsSelectListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
	else
	{
		CStandardsSelectListItem::SortColumn = colIndex;
	}
	return true;
}
