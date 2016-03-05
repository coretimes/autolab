#include "StdAfx.h"
#include "TestConfirmListItem.h"
#include "SampleAreaData.h"
#include "StandardTable.h"
#include "ReagentAreaData.h"

ListColumn CTestConfirmListItem::SortColumn = colProject;		// 排序列名
bool CTestConfirmListItem::IsAsc = true;						// 是否升序排，true是，false为降序

CTestConfirmListItem::CTestConfirmListItem(const Sample_Pos & SamplePos,const Reagent_Pos &ReagentPos)
{
	m_SamplePos = SamplePos;
	m_ReagentPos = ReagentPos;
}

CTestConfirmListItem::~CTestConfirmListItem(void)
{
}

bool CTestConfirmListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CTestConfirmListItem * pData = static_cast<const CTestConfirmListItem*>(pItemData);

	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colType:					// 样品类型
	case colProject:				// 检测项目
	case colSampleNo:				// 样品编号
		{
			// 根据m_SamplePos取得样本配置节点
			CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
			SampleConfigNode * pThisSampleNode = pSampleDatas->GetNode(m_SamplePos);
			SampleConfigNode * pThatSampleNode = pSampleDatas->GetNode(pData->GetSamplePosNum());

			// 如果两条数据属于同一条标准品，则合并
			if(pThisSampleNode->IsStandard && pThisSampleNode->RecordID == pThatSampleNode->RecordID)
				return true;
		}

	case colRepeatTimes:			// 重复次数
	case colConcPoint:				// 浓度点
	case colSamplePos:				// 样本位
		{
			return false;
		}
	default:
		break;
	}
	return false;
}

UINT CTestConfirmListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 根据m_SamplePos取得样本配置节点
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pSampleNode = pSampleDatas->GetNode(m_SamplePos);

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
	case colType:					// 样品类型
		{
			if (pSampleDatas->GetNode(m_SamplePos)->IsStandard)
				contex = _T("标准品");
			else
				contex = _T("样本");
			return 0;
		}

	case colSampleNo:				// 样品编号
		{
			// 根据样本配置节点的样本ID取得样本记录或标准品记录
			if(pSampleNode->IsStandard)
			{
				StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(pSampleNode->RecordID);
				contex.Format(_T("%s"),record.BatchNum);
			}
			else
			{
				SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(pSampleNode->RecordID);
				contex = record.SampleNo;
			}
			return 0;
		}

	case colConcPoint:				// 浓度点
		{
			// 根据样本配置节点的样本ID取得样本记录或标准品记录
			if(pSampleNode->IsStandard)
			{
				contex.Format(_T("%c"),pSampleNode->StdPointIndex + 65);
			}
			else
			{
				contex = _T("-");
			}

			return 0;
		}

	case colSamplePos:				// 样本位
		{
			contex = pSampleDatas->GetCharCoordByNum(m_SamplePos);
			return 0;
		}

	case colProject:				// 检测项目
		{
			contex.Format(_T("%d. %s(%s)"),m_ReagentPos, recReagent.ShortName,recReagent.ProjectName);
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}

	case colRepeatTimes:			// 重复次数
		{
			if(pTestProjectInfo != NULL)
				contex.Format(_T("%d"), pTestProjectInfo->Repetitions);
		}

	default:
		break;
	}

	return 0;
}

bool CTestConfirmListItem::IsBeforThan(const CListItemData * pItemData)
{
	// 如果被比数据为空，则返回false（不需要交换顺序）
	if(pItemData == NULL)
		return false;
	const CTestConfirmListItem * pData = static_cast<const CTestConfirmListItem*>(pItemData);
	// 根据m_SamplePos取得样本配置节点
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pThisSampleNode = pSampleDatas->GetNode(m_SamplePos);
	SampleConfigNode * pThatSampleNode = pSampleDatas->GetNode(pData->GetSamplePosNum());

	// 正常升序方式
	bool NormalAscOrder;
	switch(SortColumn)
	{
		// 按样本类型排序
	case colType:
		{
			// 如果类型相同，则按样本位升序排列
			if(pThisSampleNode->IsStandard == pThatSampleNode->IsStandard)
			{
				if(m_SamplePos < pData->GetSamplePosNum())
					return true;
				else 
					return false;
			}

			if(pThisSampleNode->IsStandard == true && pThatSampleNode->IsStandard == false)
				return true;
			else
				return false;
		}

		// 按样本位排序
	case colSamplePos:
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
			// 如果试剂位置相等，则标准品排在前边
			if(pThisSampleNode->IsStandard == true && pThatSampleNode->IsStandard == false)
				return true;

			if(pThisSampleNode->IsStandard == false && pThatSampleNode->IsStandard == true)
				return false;

			// 同标准品或样本，小样本位号排在前边
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

	// 如果是升序方式，则放回正常排序方式
	if(IsAsc)
		return NormalAscOrder;
	// 否则返回倒序
	else
		return (!NormalAscOrder);
}	

bool CTestConfirmListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colType:
	case colProject:
	case colSamplePos:
		break;
	default:
		return false;
	}

	// 如果排序列与原排序列一样，则改变顺序倒序方式
	if(CTestConfirmListItem::SortColumn == colIndex)
	{

		// 排序方式改变：如果原来时顺序，则改为倒序
		if (CTestConfirmListItem::IsAsc == true)
			CTestConfirmListItem::IsAsc = false;
		else
			CTestConfirmListItem::IsAsc = true;
	}
	// 如果排序列与原排序列不同，则改变排序列
	else
	{
		CTestConfirmListItem::SortColumn = colIndex;
	}
	return true;
}