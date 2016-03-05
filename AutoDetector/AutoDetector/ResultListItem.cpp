#include "StdAfx.h"
#include "ResultListItem.h"
#include "SampleTable.h"
#include "ReagentTable.h"
#include "GeneralFuction.h"

CResultListItem::CResultListItem(Result_Key ResultID)
{
	CResultTable * pResultTable = CResultTable::GetInstance();
	m_ResultRecord = pResultTable->GetRecordInfo(ResultID);
}

CResultListItem::~CResultListItem(void)
{
}

bool CResultListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CResultListItem * pData = static_cast<const CResultListItem*>(pItemData);
	ResultListColumn colIndex = static_cast<ResultListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colSampleNo:				// �������
	case colType:					// ��������
	case colDilution:				// ϡ�ͱ���
	case colDept:					// �ͼ쵥λ����
	case colApplicant:				// �ͼ���
	case colApplyDate:				// �ͼ�����
	case colUpdateDate:				// ����¼������
		{
			const ResultRecord & record= pData->GetResultInfo();
			if(record.SampleID == m_ResultRecord.SampleID)
				return true;
		}
	case colProjectName:			// �����Ŀ����
	case colReagentBatchNum:		// �Լ�����
	case colCurveID:				// ��׼����ID
	case colRluString:				// ����ֵ�ַ���
	case colCV:						// ����ֵ����ϵ��
	case colTheoryConc:				// ����Ũ��
	case colFitConc:				// ���Ũ��
	case colUnit:					// Ũ�ȵ�λ
	case colSurveyor:				// ����Ա
	case colTestDate:				// ��������
	default:
		break;
	}
	return false;
}

const ResultRecord & CResultListItem::GetResultInfo() const
{
	return m_ResultRecord;
}

UINT CResultListItem::GetCellContex(const int &ColumnIndex,
									CString & contex)
{
	ResultListColumn colIndex = static_cast<ResultListColumn> (ColumnIndex);
	SampleRecord sampleinfo = CSampleTable::GetInstance()->GetRecordInfo(m_ResultRecord.SampleID);
	ReagentRecord reagentinfo = CReagentTable::GetInstance()->GetRecordInfo(m_ResultRecord.ReagentID);
	COleDateTime date;
	CGeneralFuction * pFuntion = CGeneralFuction::GetInstance();

	switch(colIndex)
	{
	case colSampleNo:				// �������
		contex = sampleinfo.SampleNo;
		break;
	case colType:					// ��������
		contex = sampleinfo.Type;
		break;
	case colDilution:				// ϡ�ͱ���
		contex.Format(_T("%.2f"),sampleinfo.Dilution);
		break;
	case colDept:					// �ͼ쵥λ����
		contex = sampleinfo.Dept;
		break;
	case colApplicant:				// �ͼ���
		contex = sampleinfo.Applicant;
		break;
	case colApplyDate:				// �ͼ�����
		date = sampleinfo.ApplyDate;
		contex = date.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	case colUpdateDate:				// ����¼������
		date = sampleinfo.UpdateDate;
		contex = date.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	case colProjectName:			// �����Ŀ����
		contex = reagentinfo.ProjectName;
		break;
	case colReagentBatchNum:		// �Լ�����
		contex = reagentinfo.BatchNum;
		break;
	case colCurveID:				// ��׼����ID
		contex.Format(_T("%d"),m_ResultRecord.CurveID);
		break;
	case colRluString:				// ����ֵ�ַ���
		contex = m_ResultRecord.RluString;
		break;
	case colCV:						// ����ֵ����ϵ��
		{
			vector<double> rlulist;
			pFuntion->ParseRluString(m_ResultRecord.RluString,rlulist);
			double cv = pFuntion->GetCV(rlulist);
			contex.Format(_T("%.2f"),cv);
		}
		break;
	case colTheoryConc:				// ����Ũ��
		contex.Format(_T("%.2f"),m_ResultRecord.TheoryConc);
		break;
	case colFitConc:				// ���Ũ��
		contex.Format(_T("%.2f"),m_ResultRecord.FitConc);
		break;
	case colUnit:					// Ũ�ȵ�λ
		contex = m_ResultRecord.Unit;
		break;
	case colSurveyor:				// ����Ա
		contex = m_ResultRecord.Surveyor;
		break;
	case colTestDate:				// ��������
		date = m_ResultRecord.TestDate;
		contex = date.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	default:
		break;
	}
	return 0;
}

ColumnProperties CResultListItem::GetColumnProperties(const int &ColumnIndex)
{
	ResultListColumn colIndex = static_cast<ResultListColumn> (ColumnIndex);
	switch(colIndex)
	{
	case colSampleNo:				// �������
		return DeletePrompt;
	case colType:					// ��������
		return Optional;
	case colDilution:				// ϡ�ͱ���
		return VariableSize;
	case colDept:					// �ͼ쵥λ����
		return PromptEditable;
	case colApplicant:				// �ͼ���
		return PromptEditable;
	case colApplyDate:				// �ͼ�����
		return VariableDate;
	case colUpdateDate:				// ����¼������
		return VariableDate;
	case colProjectName:			// �����Ŀ����
		return PromptEditable;
	case colReagentBatchNum:		// �Լ�����
		return Optional;
	case colCurveID:				// ��׼����ID
		return Optional;
	case colRluString:				// ����ֵ�ַ���
		break;
	case colCV:						// ����ֵ����ϵ��
		break;
	case colTheoryConc:				// ����Ũ��
		return VariableSize;
	case colFitConc:				// ���Ũ��
		return VariableSize;
	case colUnit:					// Ũ�ȵ�λ
		return PromptEditable;
	case colSurveyor:				// ����Ա
		return PromptEditable;
	case colTestDate:				// ��������
		return VariableDate;
	default:
		break;
	}
	return CanNotChange;
}

Result_Key CResultListItem::GetKey()
{
	return m_ResultRecord.ID;
}

int CResultListItem::GetSubItemCount()
{
	return (int)ColumnCount;
}
