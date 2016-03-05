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
	case colSampleNo:				// 样本编号
	case colType:					// 样本类型
	case colDilution:				// 稀释倍数
	case colDept:					// 送检单位名称
	case colApplicant:				// 送检人
	case colApplyDate:				// 送检日期
	case colUpdateDate:				// 样本录入日期
		{
			const ResultRecord & record= pData->GetResultInfo();
			if(record.SampleID == m_ResultRecord.SampleID)
				return true;
		}
	case colProjectName:			// 检测项目名称
	case colReagentBatchNum:		// 试剂批号
	case colCurveID:				// 标准曲线ID
	case colRluString:				// 发光值字符串
	case colCV:						// 发光值变异系数
	case colTheoryConc:				// 理论浓度
	case colFitConc:				// 拟合浓度
	case colUnit:					// 浓度单位
	case colSurveyor:				// 检验员
	case colTestDate:				// 检验日期
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
	case colSampleNo:				// 样本编号
		contex = sampleinfo.SampleNo;
		break;
	case colType:					// 样本类型
		contex = sampleinfo.Type;
		break;
	case colDilution:				// 稀释倍数
		contex.Format(_T("%.2f"),sampleinfo.Dilution);
		break;
	case colDept:					// 送检单位名称
		contex = sampleinfo.Dept;
		break;
	case colApplicant:				// 送检人
		contex = sampleinfo.Applicant;
		break;
	case colApplyDate:				// 送检日期
		date = sampleinfo.ApplyDate;
		contex = date.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	case colUpdateDate:				// 样本录入日期
		date = sampleinfo.UpdateDate;
		contex = date.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	case colProjectName:			// 检测项目名称
		contex = reagentinfo.ProjectName;
		break;
	case colReagentBatchNum:		// 试剂批号
		contex = reagentinfo.BatchNum;
		break;
	case colCurveID:				// 标准曲线ID
		contex.Format(_T("%d"),m_ResultRecord.CurveID);
		break;
	case colRluString:				// 发光值字符串
		contex = m_ResultRecord.RluString;
		break;
	case colCV:						// 发光值变异系数
		{
			vector<double> rlulist;
			pFuntion->ParseRluString(m_ResultRecord.RluString,rlulist);
			double cv = pFuntion->GetCV(rlulist);
			contex.Format(_T("%.2f"),cv);
		}
		break;
	case colTheoryConc:				// 理论浓度
		contex.Format(_T("%.2f"),m_ResultRecord.TheoryConc);
		break;
	case colFitConc:				// 拟合浓度
		contex.Format(_T("%.2f"),m_ResultRecord.FitConc);
		break;
	case colUnit:					// 浓度单位
		contex = m_ResultRecord.Unit;
		break;
	case colSurveyor:				// 检验员
		contex = m_ResultRecord.Surveyor;
		break;
	case colTestDate:				// 检验日期
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
	case colSampleNo:				// 样本编号
		return DeletePrompt;
	case colType:					// 样本类型
		return Optional;
	case colDilution:				// 稀释倍数
		return VariableSize;
	case colDept:					// 送检单位名称
		return PromptEditable;
	case colApplicant:				// 送检人
		return PromptEditable;
	case colApplyDate:				// 送检日期
		return VariableDate;
	case colUpdateDate:				// 样本录入日期
		return VariableDate;
	case colProjectName:			// 检测项目名称
		return PromptEditable;
	case colReagentBatchNum:		// 试剂批号
		return Optional;
	case colCurveID:				// 标准曲线ID
		return Optional;
	case colRluString:				// 发光值字符串
		break;
	case colCV:						// 发光值变异系数
		break;
	case colTheoryConc:				// 理论浓度
		return VariableSize;
	case colFitConc:				// 拟合浓度
		return VariableSize;
	case colUnit:					// 浓度单位
		return PromptEditable;
	case colSurveyor:				// 检验员
		return PromptEditable;
	case colTestDate:				// 检验日期
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
