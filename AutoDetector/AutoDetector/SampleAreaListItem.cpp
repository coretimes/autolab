#include "StdAfx.h"
#include "SampleAreaListItem.h"
#include "SampleAreaData.h"
#include "ReagentAreaData.h"
#include "StandardTable.h"

ListColumn CSampleAreaListItem::SortColumn = colPos;	// ��������
bool CSampleAreaListItem::IsAsc = true;					// �Ƿ������ţ�true�ǣ�falseΪ����

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

	// ����m_SamplePosȡ���������ýڵ�
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pSampleNode = pSampleDatas->GetNode(m_SamplePos);
	switch(colIndex)
	{
	case colPos:								// ����λ�ñ��
		{
			contex = pSampleDatas->GetCharCoordByNum(m_SamplePos);
			return 0;
		}
	case colType:								// ��������
		{
			if(pSampleNode->IsSetingSample == false)
				contex = _T("");
			else if (pSampleNode->IsStandard)
				contex = _T("��׼Ʒ");
			else
				contex = _T("����");
			return 0;
		}

	case colSampleNo:
		if(pSampleNode->IsSetingSample == false)
			contex = _T("");
		// �����������ýڵ������IDȡ��������¼���׼Ʒ��¼
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

	// ����m_ReagentPosȡ���������ýڵ�ļ����Ŀ��Ϣ
	if(m_ReagentPos > 0)
	{
		SampleConfigNode::TestProjInfo * pTestProjectInfo = NULL;
		map<Sample_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pSampleNode->TestProjArr.find(m_ReagentPos);
		if(iter != pSampleNode->TestProjArr.end())
			pTestProjectInfo  = &(iter->second);

		// ����m_ReagentPosȡ���Լ�ID
		CReagentAreaData  * pReagentDatas = CReagentAreaData::GetInstance();
		ReagentConfigNode * pReagentNode = pReagentDatas->GetNode(m_ReagentPos);

		// �����Լ�IDȡ���Լ���¼
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
				contex.Format(_T("%s"), _T("δ��ʼ"));
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
	case colPos:					// ����λ
	case colType:					// ��������
	case colSampleNo:				// �������
		{
			if(m_SamplePos == pData->GetSamplePosNum())
				return true;
		}
		break;
	case colProject:				// �����Ŀ����
	case colRepeats:				// �ظ����Դ���
	default:
		break;
	}
	return false;
}

bool CSampleAreaListItem::IsBeforThan(const CListItemData * pItemData)
{
	// �����������Ϊ�գ��򷵻�false������Ҫ����˳��
	if(pItemData == NULL)
		return false;
	const CSampleAreaListItem * pData = static_cast<const CSampleAreaListItem*>(pItemData);

	// ��������ʽ
	bool NormalAscOrder;
	switch(SortColumn)
	{
	// ������λ����
	case colPos:
		if (m_SamplePos ==  pData->GetSamplePosNum())
		{
			// �������λ����ȣ����Լ�λ����������
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

	// �������Ŀ����
	case colProject:
		if (m_ReagentPos ==  pData->GetReagentPosNum())
		{
			// ����Լ�λ����ȣ�������λ����������
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

	// �����˳��ʽ����Ż���������ʽ
	if(IsAsc)
		return NormalAscOrder;
	// ���򷵻ص���
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

	// �����������ԭ������һ������ı�˳����ʽ
	if(CSampleAreaListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CSampleAreaListItem::IsAsc == true)
			CSampleAreaListItem::IsAsc = false;
		else
			CSampleAreaListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
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
