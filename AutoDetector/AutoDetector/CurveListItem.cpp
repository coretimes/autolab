#include "StdAfx.h"
#include "CurveListItem.h"
#include "StandardTable.h"

ListColumn CCurveListItem::SortColumn = colTestDate;		// ��������
bool CCurveListItem::IsAsc = false;							// �Ƿ������ţ�true�ǣ�falseΪ����

CCurveListItem::CCurveListItem(const CurveRecord & curve)
{
	m_CurveRecord = curve;
	StandardRecord recStandard = CStandardTable::GetInstance()->GetRecordInfo(curve.StandardID);
	m_strProjectName.Format(_T("%s(%s)"),recStandard.ShortName,recStandard.ProjectName);
	m_strBatchNum = recStandard.BatchNum;
}

CCurveListItem::~CCurveListItem(void)
{
}

UINT CCurveListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	switch(ColumnIndex)
	{
	case colProject:									// ��Ŀ����
		contex = m_strProjectName;
		break;
	case colBatchNum:									// �Լ�����
		contex = m_strBatchNum;
		break;
	case colTestDate:									// �������
		{
			COleDateTime tempDate = m_CurveRecord.TestDate;
			contex = tempDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			break;
		}
	case colDefaultFitType:
		{
			if (m_CurveRecord.DefaultFitType == Spline)
				contex = _T("��������");
			if (m_CurveRecord.DefaultFitType == FourParams)
				contex = _T("�Ĳ���");
			break;
		}
	case colSurveyor:
		{
			contex = m_CurveRecord.Surveyor;
			break;
		}

	default:
		break;
	}
	return 0;
}


bool CCurveListItem::IsBeforThan(const CListItemData * pItemData)
{
	// �����������Ϊ�գ��򷵻�false������Ҫ����˳��
	if(pItemData == NULL)
		return false;

	const CCurveListItem * pData = static_cast<const CCurveListItem*>(pItemData);

	// ��������ʽ
	bool NormalAscOrder;
	switch(SortColumn)
	{
		// ��������������
	case colTestDate:
		if (m_CurveRecord.TestDate < pData->GetTestTime())
			NormalAscOrder = true;

		else if (m_CurveRecord.TestDate == pData->GetTestTime())
			return false;
		else
			NormalAscOrder = false;

	default:
		break;
	}
	// ���������ʽ����Ż���������ʽ
	if(IsAsc)
		return NormalAscOrder;
	// ���򷵻ص���
	else
		return (!NormalAscOrder);
}

bool CCurveListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colProject:
	case colBatchNum:
	case colDefaultFitType:
	case colSurveyor:
		return false;
	case colTestDate:
		break;
	default:
		return false;
	}

	// �����������ԭ������һ������ı�˳����ʽ
	if(CCurveListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CCurveListItem::IsAsc == true)
			CCurveListItem::IsAsc = false;
		else
			CCurveListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
	else
	{
		CCurveListItem::SortColumn = colIndex;
	}
	return true;
}