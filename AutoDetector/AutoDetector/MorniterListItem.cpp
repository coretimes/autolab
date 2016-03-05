#include "StdAfx.h"
#include "MorniterListItem.h"
#include "TestManagement.h"
#include "ReagentTable.h"
#include "SampleAreaData.h"
#include "SampleTable.h"
#include "StandardTable.h"

CMorniterListItem::CMorniterListItem(Metering_ID ID)
{
	// ���ݲ��ID��ȡ�ò��ڵ�����
	CMeteringManagement * pList = CMeteringManagement::GetInstance();
	pList->GetNode(ID,m_MeteringNode);

	// ȡ�ò�ܽڵ�Ĳ��Ե�Ԫ��Ϣ
	m_pTestUnit = CTestManagement::GetInstance()->GetTestUnit(m_MeteringNode.RealatedInfoKey.ParentUnitID);

	// ���ݲ��ڵ��ڵĲ��Ե�ԪID��ȡ���������Խڵ�����
	m_pTestUnit->GetSampleNodeInfo(m_MeteringNode.RealatedInfoKey.ConcIndex,m_SampleTestNode);
}

CMorniterListItem::~CMorniterListItem(void)
{
}

bool CMorniterListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CMorniterListItem * pData = static_cast<const CMorniterListItem*>(pItemData);

	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	
	// ȡ�ò��ڵ�����
	const MeteringNode & thatNode = pData->GetMeteringNode();

	// ȡ�ò�ܽڵ�Ĳ��Ե�Ԫ��Ϣ
	const CTestUnit * pThatUnit = pData->GetTestUnit();

	switch(colIndex)
	{
	case colType:					// ��Ʒ����
	case colProject:				// �����Ŀ
	case colSampleNo:				// ��Ʒ���
	case colCurve:					// ��׼����
		{
			// �������ͬһ���Ե�Ԫ����ϲ�
			if (m_pTestUnit->GetID() == pThatUnit->GetID())
				return true;
			break;
		}

	case colConcPoint:				// Ũ�ȵ�
	case colSamplePos:				// ����λ
	case colAverageRlu:				// ƽ������ֵ
	case colCV:						// ����ϵ��
	case colTheoryConc:				// ����Ũ��
	case colCalConc:				// ����Ũ��
		{
			// ���������ͬһ���Ե�Ԫ���򲻺ϲ�
			if (m_pTestUnit->GetID() != pThatUnit->GetID())
				return false;

			// �������ͬһ���Ե�Ԫ������ͬһŨ�ȵ㣬�ϲ�
			if (m_MeteringNode.RealatedInfoKey.ConcIndex == thatNode.RealatedInfoKey.ConcIndex)
				return true;
			break;
		}

	case colMeteringIndex:			// ���ڵ�����
	case colFinishTime:				// ���ʱ��
	case colMeteringState:			// ���״̬
	case colUseState:				// ʹ��״̬
	case colRlu:					// ����ֵ
		return false;

	default:
		break;
	}
	return false;
}

UINT CMorniterListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colType:					// ��Ʒ����
		{
			if (m_SampleTestNode.IsStandard)
				contex = _T("��׼Ʒ");
			else
				contex = _T("����");
			break;
		}
	case colProject:				// �����Ŀ
		{
			CReagentTable *pReagentTable = CReagentTable::GetInstance();
			ReagentRecord reagentRecord = pReagentTable->GetRecordInfo(m_SampleTestNode.ReagentID);
			reagentRecord.ProjectName;
			contex.Format(_T("%d. %s(%s)"),m_SampleTestNode.ReagentPos, reagentRecord.ShortName,reagentRecord.ProjectName);
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}
	case colSampleNo:				// ��Ʒ���
		{

			if (m_SampleTestNode.IsStandard)
			{
				StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
				contex.Format(_T("%s"),record.BatchNum);
			}
			else
			{
				SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
				contex.Format(_T("%s"),record.SampleNo);
			}

			break;
		}

	case colConcPoint:				// Ũ�ȵ�
		{
			if (m_SampleTestNode.IsStandard)
				contex.Format(_T("%c"),m_MeteringNode.RealatedInfoKey.ConcIndex + 65);
			else
				contex = _T("-");

			break;
		}

	case colSamplePos:				// ����λ
		{
			contex = CSampleAreaData::GetInstance()->GetCharCoordByNum(m_SampleTestNode.SamplePos);
			break;
		}

	case colMeteringIndex:			// ���ڵ�����
		{
			contex.Format(_T("%d"),m_MeteringNode.MeteringIndex);
			break;
		}
	case colFinishTime:				// ���ʱ��
		{
			// ȡ�õ�ǰ��ʱ����
			CMeteringManagement * pMeteringManagement = CMeteringManagement::GetInstance();
			Tick_ID curtick = pMeteringManagement->GetCurrentTick();
	
			// ʣ��ʱ�� = ���������� - ��ǰ���ڣ� * ����ʱ�������룩
 			CTimeSpan lefttime((m_MeteringNode.end -curtick) * pMeteringManagement->GetTickInterval() / 1000);

			// ȡ�õ�ǰʱ��
			CTime cm = CTime::GetCurrentTime();

			// �������ʱ��
			CTime finishTime = cm + lefttime;
			contex = finishTime.Format(_T("%H:%M"));
			break;
		}

	case colMeteringState:			// ���״̬
		{
			switch(m_MeteringNode.state)
			{
			case MeteringNode::Unsent:				// δ����
				contex = _T("δ����");
				break;
			case MeteringNode::Sending:				// ���ݷ�����
				contex = _T("���ݷ�����");
				break;
			case MeteringNode::WaitePipetting:		// �ȴ�����
				contex = _T("�ȴ�����");
				break;
			case MeteringNode::Pipetting:			// ���ڼ���
				contex = _T("���ڼ���");
				break;
			case MeteringNode::AfterPipetting:		// �������
				contex = _T("�������");
				break;
			case MeteringNode::Incubating:			// ������
				contex = _T("������");
				break;
			case MeteringNode::WaiteCleaning:		// �ȴ���ϴ
				contex = _T("�ȴ���ϴ");
				break;
			case MeteringNode::Cleaning:			// ������ϴ
				contex = _T("������ϴ");
				break;
			case MeteringNode::WaiteMetering:		// �ȴ����
				contex = _T("�ȴ����");
				break;
			case MeteringNode::Metering:			// ���ڲ��
				contex = _T("���ڲ��");
				break;
			case MeteringNode::Over:				// ������
				contex = _T("������");
				break;
			}
			break;
		}
	case colUseState:								// ʹ��״̬
		contex = _T("ʹ��");
		break;
	case colRlu:									// ����ֵ
		break;
	case colAverageRlu:								// ƽ������ֵ
		break;
	case colCV:										// ����ϵ��
		break;
	case colTheoryConc:								// ����Ũ��
		if (m_SampleTestNode.IsStandard)
		{
			StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
			contex.Format(_T("%g%s"),record.StdConc[m_SampleTestNode.StdPointIndex],record.Unit);
		}
		else
		{
			//SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
			//contex.Format(_T("%s"),record.);
		}
		break;
	case colCalConc:								// ����Ũ��
		break;
	case colCurve:									// ��׼����
		break;
	default:
		break;
	}

	return 0;
}
