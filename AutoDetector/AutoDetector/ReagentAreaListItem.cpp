#include "StdAfx.h"
#include "ReagentAreaListItem.h"
#include "ReagentAreaData.h"
#include "ReagentTable.h"

CReagentAreaListItem::CReagentAreaListItem(Reagent_Pos pos)
{
	m_ReagentPos = pos;
}

CReagentAreaListItem::~CReagentAreaListItem(void)
{
}
UINT CReagentAreaListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	ReagentConfigNode * pNode = CReagentAreaData::GetInstance()->GetNode(m_ReagentPos);
	// 如果本试剂位置无试管或没有配置试剂
	if(pNode->IsThereTube == false || pNode->IsSetingReagent == false)
	{	
		// 除了位置编号外，其他全部画"-"	
		if(colIndex == colPos)
			contex.Format(_T("%d"),m_ReagentPos);
		else
			contex = _T("");
		return 0;
	}
	CReagentTable * pTable = CReagentTable::GetInstance();
	ReagentRecord record = pTable->GetRecordInfo(pNode->ReagentID);
	COleDateTime tempDate;
	switch(colIndex)
	{
	case colPos:								// 试剂位置编号
		contex.Format(_T("%d"),m_ReagentPos);
		break;
	case colProject:							// 检测项目
		contex.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		break;
	case colBatchNum:							// 批号
		contex.Format(_T("%s"),record.BatchNum);
		break;
	case colSampleAmount:						// 样本用量
		contex.Format(_T("%g %s"),record.SampleAmount,record.Unit);
		break;
	case colMagnetciAmount:						// 磁标用量
		contex.Format(_T("%g %s"),record.MagneticAmount,record.Unit);
		break;
	case colHRPAmount:							// 酶标用量
		contex.Format(_T("%g %s"),record.HRPAmount,record.Unit);
		break;
	case colDilution:							// 稀释倍数
		contex.Format(_T("%g 倍"),record.Dilution);
		break;
	case colWashTimes:							// 清洗次数
		contex.Format(_T("%g 次"),record.WashTimes);
		break;
	case colIncuTime:							// 温育时间
		contex.Format(_T("%g 分钟"),record.IncubationTime);
		break;	
	default:
		break;
	}

	return 0;
}

void CReagentAreaListItem::SetHighlightState(bool state)
{
	ReagentConfigNode * pNode = CReagentAreaData::GetInstance()->GetNode(m_ReagentPos);
	pNode->IsHover = state;
}

bool CReagentAreaListItem::IsHighlight()
{
	ReagentConfigNode * pNode = CReagentAreaData::GetInstance()->GetNode(m_ReagentPos);
	return pNode->IsHover;
}
