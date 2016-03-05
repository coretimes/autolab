#include "StdAfx.h"
#include "SampleAreaListItem.h"
#include "SampleAreaData.h"
#include "ReagentAreaData.h"
#include "StandardTable.h"

ListColumn CSampleAreaListItem::SortColumn = colPos;	// 排序列名
bool CSampleAreaListItem::IsAsc = true;					// 是否升序排，true是，false为降序

CSampleAreaListItem::CSampleAreaListItem(const Sample_Pos & SamplePos,const Reagent_Pos &ReagentPos)
{
	m_SamplePos = SamplePos;
	m_ReagentPos = ReagentPos;
}

CSampleAreaListItem::~CSampleAreaListItem(void)
{
}

UINT CSampleAreaListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 根据m_SamplePos取得样本配置节点
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pSampleNode = pSampleDatas->GetNode(m_SamplePos);
	switch(colIndex)
	{
	case colPos:								// 样本位置编号
		{
			contex = pSampleDatas->GetCharCoordByNum(m_SamplePos);
			return 0;
		}
	case colType:								// 样本类型
		{
			if(pSampleNode->IsSetingSample == false)
				contex = _T("");
			else if (pSampleNode->IsStandard)
				contex = _T("标准品");
			else
				contex = _T("样本");
			return 0;
		}

	case colSampleNo:
		if(pSampleNode->IsSetingSample == false)
			contex = _T("");
		// 根据样本配置节点的样本ID取得样本记录或标准品记录
		else if(pSampleNode->IsStandard)
		{
			StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(pSampleNode->RecordID);
			contex.Format(_T("%s-%c"),record.BatchNum,pSampleNode->StdPointIndex + 65);
		}
		else
		{
			SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(pSampleNode->RecordID);
			contex = record.SampleNo;
		}
		return 0;
	default:
		break;
	}

	// 根据m_ReagentPos取得样本配置节点的检测项目信息
	if(m_ReagentPos > 0)
	{
		SampleConfigNode::TestProjInfo * pTestProjectInfo = NULL;
		map<Sample_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pSampleNode->TestProjArr.find(m_ReagentPos);
		if(iter != pSampleNode->TestProjArr.end())
			pTestProjectInfo  = &(iter->second);

		// 根据m_ReagentPos取得试剂ID
		CReagentAreaData  * pReagentDatas = CReagentAreaData::GetInstance();
		ReagentConfigNode * pReagentNode = pReagentDatas->GetNode(m_ReagentPos);

		// 根据试剂ID取得试剂记录
		ReagentRecord recReagent = CReagentTable::GetInstance()->GetRecordInfo(pReagentNode->ReagentID);

		switch(colIndex)
		{
		case colProject:
			contex.Format(_T("%d. %s(%s)"),m_ReagentPos, recReagent.ShortName,recReagent.ProjectName);
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		case colRepeats:
			if(pTestProjectInfo != NULL)
				contex.Format(_T("%d"), pTestProjectInfo->Repetitions);
			break;
		case colTestState:
				contex.Format(_T("%s"), _T("未开始"));
			break;

		default:
			break;
		}
	}
	return 0;
}

bool CSampleAreaListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CSampleAreaListItem * pData = static_cast<const CSampleAreaListItem*>(pItemData);

	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colPos:					// 样本位
	case colType:					// 样本类型
	case colSampleNo:				// 样本编号
		{
			if(m_SamplePos == pData->GetSamplePosNum())
				return true;
		}
		break;
	case colProject:				// 检测项目名称
	case colRepeats:				// 重复测试次数
	default:
		break;
	}
	return false;
}

bool CSampleAreaListItem::IsBeforThan(const CListItemData * pItemData)
{
	// 如果被比数据为空，则返回false（不需要交换顺序）
	if(pItemData == NULL)
		return false;
	const CSampleAreaListItem * pData = static_cast<const CSampleAreaListItem*>(pItemData);

	// 正常升序方式
	bool NormalAscOrder;
	switch(SortColumn)
	{
	// 按样本位排序
	case colPos:
		if (m_SamplePos ==  pData->GetSamplePosNum())
		{
			// 如果样本位置相等，则按试剂位置升序排序
			if(m_ReagentPos < pData->GetReagentPosNum())
				return true;
			else
				return false;
		}

		else if(m_SamplePos <  pData->GetSamplePosNum())
			NormalAscOrder = true;
		else 
			NormalAscOrder = false;
		break;

	// 按检测项目排序
	case colProject:
		if (m_ReagentPos ==  pData->GetReagentPosNum())
		{
			// 如果试剂位置相等，则按样本位置升序排序
			if(m_SamplePos <  pData->GetSamplePosNum())
				return true;
			else
				return false;
		}
		
		else if(m_ReagentPos <  pData->GetReagentPosNum())
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

bool CSampleAreaListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colPos:
	case colProject:
		break;
	default:
		return false;
	}

	// 如果排序列与原排序列一样，则改变顺序倒序方式
	if(CSampleAreaListItem::SortColumn == colIndex)
	{

		// 排序方式改变：如果原来时顺序，则改为倒序
		if (CSampleAreaListItem::IsAsc == true)
			CSampleAreaListItem::IsAsc = false;
		else
			CSampleAreaListItem::IsAsc = true;
	}
	// 如果排序列与原排序列不同，则改变排序列
	else
	{
		CSampleAreaListItem::SortColumn = colIndex;
	}
	return true;
}

bool CSampleAreaListItem::IsHighlight()
{
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	if(m_SamplePos == pSampleDatas->GetHoverPos())
		return true;
	else
		return false;
}
