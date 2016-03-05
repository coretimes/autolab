#include "StdAfx.h"
#include "StandardConfigListItem.h"
#include "SampleAreaData.h"

CStandardConfigListItem::CStandardConfigListItem(void)
{
}

CStandardConfigListItem::~CStandardConfigListItem(void)
{
}

void CStandardConfigListItem::SetPosNum(const Sample_Pos &PosNum)
{
	m_PosNum = PosNum;							// 样本位编号
}

UINT CStandardConfigListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	CSampleAreaData * pSampleAreaData = CSampleAreaData::GetInstance();
	switch(colIndex)
	{
	case colPos:					// 样本位
		contex = pSampleAreaData->GetCharCoordByNum(m_PosNum);
		break;
	case colProject:				// 项目名称
		contex = m_strProject;
		break;
	case colBatchNum:				// 批号
		contex = m_strBatchNum;
		break;
	case colConcIndex:				// 浓度点索引
		if(m_ConcIndex >= 0 && m_ConcIndex < (int)m_OptionsOfConcIndex.size())		// 由于可能有多余的样本配置节点，所以有的列表项可能没有数据值，此索引有可能为初始值0
			contex = m_OptionsOfConcIndex[m_ConcIndex];							// m_ConcIndex是从0起始的，而数组坐标从0起始
		else
			contex = _T("");
		break;
	case colRepeat:					// 重复次数
		if(m_Repeat > 0)
			contex.Format(_T("%d"), m_Repeat);
		else 
			contex = _T("");
		break;
	default:
		break;
	}
	return 0;
}

ColumnProperties CStandardConfigListItem::GetColumnProperties(const int &ColumnIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	if(colIndex ==  colPos)					// 样本位
		return DeletePrompt;

	if(m_strBatchNum == _T(""))
		return CanNotChange;

	switch(colIndex)
	{
	case colProject:				// 检测项目
		return CanNotChange;
	case colBatchNum:				// 批号
		return CanNotChange;
	case colConcIndex:				// 浓度点索引
		return Optional;
	case colRepeat:					// 重复次数
		return VariableSize;
	default:
		break;
	}
	return CanNotChange;
}

bool CStandardConfigListItem::GetComboxProperties(const int &ColumnIndex,
												   vector<CString>&strOptions,
												   int &nDefaultSelectedIndex,
												   bool &bIsEditable)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// 浓度点索引列
	if(colIndex == colConcIndex)
	{
		for(int i=0; i < (int)m_OptionsOfConcIndex.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfConcIndex[i]);
		}
		// 浓度点索引从0起始
		nDefaultSelectedIndex = m_ConcIndex;
		bIsEditable = false;
		return true;
	}
	return false;
}

void CStandardConfigListItem::OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// 浓度点索引从0起始
	if(colIndex == colConcIndex)
	{
		m_ConcIndex = newSelectedIndex;
	}
}

void CStandardConfigListItem::OnEnChange(const int &ColumnIndex,const CString & newContex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// 浓度点索引从1起始，而组合框索引从0起始
	if(colIndex == colRepeat)
	{
		m_Repeat = _tstoi(newContex);

		if(m_Repeat > 20)
			m_Repeat = 20;

		if(m_Repeat < 1)
			m_Repeat = 1;
	}
}

bool CStandardConfigListItem::GetSpinBtnProperties(const int &ColumnIndex,
													double &initial,
													double &lower, 
													double & upper,
													double &increament)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// 重复次数列：上限20、下限1、增量1
	if(colIndex == colRepeat)
	{
		initial = static_cast<double> (m_Repeat);
		lower = 1;									
		upper = 20;
		increament = 1;
		return true;
	}
	return false;
}
