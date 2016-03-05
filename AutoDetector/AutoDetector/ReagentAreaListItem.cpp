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
	// ������Լ�λ�����Թܻ�û�������Լ�
	if(pNode->IsThereTube == false || pNode->IsSetingReagent == false)
	{	
		// ����λ�ñ���⣬����ȫ����"-"	
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
	case colPos:								// �Լ�λ�ñ��
		contex.Format(_T("%d"),m_ReagentPos);
		break;
	case colProject:							// �����Ŀ
		contex.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		break;
	case colBatchNum:							// ����
		contex.Format(_T("%s"),record.BatchNum);
		break;
	case colSampleAmount:						// ��������
		contex.Format(_T("%g %s"),record.SampleAmount,record.Unit);
		break;
	case colMagnetciAmount:						// �ű�����
		contex.Format(_T("%g %s"),record.MagneticAmount,record.Unit);
		break;
	case colHRPAmount:							// ø������
		contex.Format(_T("%g %s"),record.HRPAmount,record.Unit);
		break;
	case colDilution:							// ϡ�ͱ���
		contex.Format(_T("%g ��"),record.Dilution);
		break;
	case colWashTimes:							// ��ϴ����
		contex.Format(_T("%g ��"),record.WashTimes);
		break;
	case colIncuTime:							// ����ʱ��
		contex.Format(_T("%g ����"),record.IncubationTime);
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
