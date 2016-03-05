#include "StdAfx.h"
#include "CurveListItem.h"
#include "StandardTable.h"

ListColumn CCurveListItem::SortColumn = colTestDate;		// 排序列名
bool CCurveListItem::IsAsc = false;							// 是否升序排，true是，false为降序

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
	case colProject:									// 项目名称
		contex = m_strProjectName;
		break;
	case colBatchNum:									// 试剂批号
		contex = m_strBatchNum;
		break;
	case colTestDate:									// 检测日期
		{
			COleDateTime tempDate = m_CurveRecord.TestDate;
			contex = tempDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			break;
		}
	case colDefaultFitType:
		{
			if (m_CurveRecord.DefaultFitType == Spline)
				contex = _T("三次样条");
			if (m_CurveRecord.DefaultFitType == FourParams)
				contex = _T("四参数");
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
	// 如果被比数据为空，则返回false（不需要交换顺序）
	if(pItemData == NULL)
		return false;

	const CCurveListItem * pData = static_cast<const CCurveListItem*>(pItemData);

	// 正常升序方式
	bool NormalAscOrder;
	switch(SortColumn)
	{
		// 按样本类型排序
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
	// 如果是升序方式，则放回正常排序方式
	if(IsAsc)
		return NormalAscOrder;
	// 否则返回倒序
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

	// 如果排序列与原排序列一样，则改变顺序倒序方式
	if(CCurveListItem::SortColumn == colIndex)
	{

		// 排序方式改变：如果原来时顺序，则改为倒序
		if (CCurveListItem::IsAsc == true)
			CCurveListItem::IsAsc = false;
		else
			CCurveListItem::IsAsc = true;
	}
	// 如果排序列与原排序列不同，则改变排序列
	else
	{
		CCurveListItem::SortColumn = colIndex;
	}
	return true;
}