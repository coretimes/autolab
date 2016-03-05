#include "StdAfx.h"
#include "TestConfirmListItem.h"
#include "SampleAreaData.h"
#include "StandardTable.h"
#include "ReagentAreaData.h"

ListColumn CTestConfirmListItem::SortColumn = colProject;		// ��������
bool CTestConfirmListItem::IsAsc = true;						// �Ƿ������ţ�true�ǣ�falseΪ����

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
	case colType:					// ��Ʒ����
	case colProject:				// �����Ŀ
	case colSampleNo:				// ��Ʒ���
		{
			// ����m_SamplePosȡ���������ýڵ�
			CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
			SampleConfigNode * pThisSampleNode = pSampleDatas->GetNode(m_SamplePos);
			SampleConfigNode * pThatSampleNode = pSampleDatas->GetNode(pData->GetSamplePosNum());

			// ���������������ͬһ����׼Ʒ����ϲ�
			if(pThisSampleNode->IsStandard && pThisSampleNode->RecordID == pThatSampleNode->RecordID)
				return true;
		}

	case colRepeatTimes:			// �ظ�����
	case colConcPoint:				// Ũ�ȵ�
	case colSamplePos:				// ����λ
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

	// ����m_SamplePosȡ���������ýڵ�
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pSampleNode = pSampleDatas->GetNode(m_SamplePos);

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
	case colType:					// ��Ʒ����
		{
			if (pSampleDatas->GetNode(m_SamplePos)->IsStandard)
				contex = _T("��׼Ʒ");
			else
				contex = _T("����");
			return 0;
		}

	case colSampleNo:				// ��Ʒ���
		{
			// �����������ýڵ������IDȡ��������¼���׼Ʒ��¼
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

	case colConcPoint:				// Ũ�ȵ�
		{
			// �����������ýڵ������IDȡ��������¼���׼Ʒ��¼
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

	case colSamplePos:				// ����λ
		{
			contex = pSampleDatas->GetCharCoordByNum(m_SamplePos);
			return 0;
		}

	case colProject:				// �����Ŀ
		{
			contex.Format(_T("%d. %s(%s)"),m_ReagentPos, recReagent.ShortName,recReagent.ProjectName);
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}

	case colRepeatTimes:			// �ظ�����
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
	// �����������Ϊ�գ��򷵻�false������Ҫ����˳��
	if(pItemData == NULL)
		return false;
	const CTestConfirmListItem * pData = static_cast<const CTestConfirmListItem*>(pItemData);
	// ����m_SamplePosȡ���������ýڵ�
	CSampleAreaData *pSampleDatas = CSampleAreaData::GetInstance();
	SampleConfigNode * pThisSampleNode = pSampleDatas->GetNode(m_SamplePos);
	SampleConfigNode * pThatSampleNode = pSampleDatas->GetNode(pData->GetSamplePosNum());

	// ��������ʽ
	bool NormalAscOrder;
	switch(SortColumn)
	{
		// ��������������
	case colType:
		{
			// ���������ͬ��������λ��������
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

		// ������λ����
	case colSamplePos:
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
			// ����Լ�λ����ȣ����׼Ʒ����ǰ��
			if(pThisSampleNode->IsStandard == true && pThatSampleNode->IsStandard == false)
				return true;

			if(pThisSampleNode->IsStandard == false && pThatSampleNode->IsStandard == true)
				return false;

			// ͬ��׼Ʒ��������С����λ������ǰ��
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

	// ���������ʽ����Ż���������ʽ
	if(IsAsc)
		return NormalAscOrder;
	// ���򷵻ص���
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

	// �����������ԭ������һ������ı�˳����ʽ
	if(CTestConfirmListItem::SortColumn == colIndex)
	{

		// ����ʽ�ı䣺���ԭ��ʱ˳�����Ϊ����
		if (CTestConfirmListItem::IsAsc == true)
			CTestConfirmListItem::IsAsc = false;
		else
			CTestConfirmListItem::IsAsc = true;
	}
	// �����������ԭ�����в�ͬ����ı�������
	else
	{
		CTestConfirmListItem::SortColumn = colIndex;
	}
	return true;
}