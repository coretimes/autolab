#include "StdAfx.h"
#include "StandardsSelectListItem.h"

ListColumn CStandardsSelectListItem::SortColumn = colProject;	// 排序列名
bool CStandardsSelectListItem::IsAsc = false;					// 是否升序排，true是，false为降序

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
	case colBatchNum:								// 试剂批号
		contex = m_strBatchNum;
		break;
	case colProject:									// 项目名称
		contex.Format(_T("%s(%s)"),m_ProjectName.ShortName,m_ProjectName.ProjectName);				
		break;
	case colProductDate:								// 项目名称
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
	// 如果被比数据为空，则返回false（不需要交换顺序）
	if(pItemData == NULL)
		return false;
	const CStandardsSelectListItem * pData = static_cast<const CStandardsSelectListItem*>(pItemData);

	// 正常升序方式
	bool NormalAscOrder;
	switch(SortColumn)
	{
	case colProject:
		if (m_ProjectName.ShortName ==  pData->GetProjectName().ShortName)
		{
			// 如果样本位置相等，则按试剂位置升序排序
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

	// 如果是顺序方式，则放回正常排序方式
	if(IsAsc)
		return NormalAscOrder;
	// 否则返回倒序
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

	// 如果排序列与原排序列一样，则改变顺序倒序方式
	if(CStandardsSelectListItem::SortColumn == colIndex)
	{

		// 排序方式改变：如果原来时顺序，则改为倒序
		if (CStandardsSelectListItem::IsAsc == true)
			CStandardsSelectListItem::IsAsc = false;
		else
			CStandardsSelectListItem::IsAsc = true;
	}
	// 如果排序列与原排序列不同，则改变排序列
	else
	{
		CStandardsSelectListItem::SortColumn = colIndex;
	}
	return true;
}
