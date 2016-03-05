#include "StdAfx.h"
#include "SampleConfigListItem.h"
#include "SampleAreaData.h"

ListColumn CSampleConfigListItem::SortColumn = colPos;	// 排序列名
bool CSampleConfigListItem::IsAsc = false;				// 是否升序排，true是，false为降序

CSampleConfigListItem::CSampleConfigListItem(void)
{
	m_Repeat = -1;
	m_dTheoryConc = - 1;
	m_idxTestProject = -1;
}

CSampleConfigListItem::~CSampleConfigListItem(void)
{
}

bool CSampleConfigListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CSampleConfigListItem * pData = static_cast<const CSampleConfigListItem*>(pItemData);
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colPos:					// 样本位
	case colSampleNo:				// 样本编号
	case colType:					// 样本类型
	case colDilution:				// 稀释倍数
	case colDept:					// 送检单位名称
	case colApplicant:				// 送检人
	case colApplyDate:				// 送检日期
		{
			if(m_PosNum == pData->GetPosNum())
				return true;
		}
		break;
	case colProjecName:				// 检测项目名称
	case colRepeat:					// 重复测试次数
	case colTheoryConc:				// 理论浓度
	default:
		break;
	}
	return false;
}

void CSampleConfigListItem::SetPosNum(const Sample_Pos &PosNum)
{
	m_PosNum = PosNum;							// 样本位编号
}

UINT CSampleConfigListItem::GetCellContex(const int &ColumnIndex,
										  CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	CSampleAreaData * pSampleAreaData = CSampleAreaData::GetInstance();
	COleDateTime tempDate;
	switch(colIndex)
	{
	case colPos:								// 样本位
		contex = pSampleAreaData->GetCharCoordByNum(m_PosNum);
		break;
	case colSampleNo:							// 样本编号
		contex = m_SampleInfo.SampleNo;
		break;
	case colType:								// 样本类型
		contex = m_SampleInfo.Type;
		break;
	case colDilution:							// 稀释度
		contex.Format(_T("%g"),m_SampleInfo.Dilution);
		break;
	case colDept:								// 送检单位
		contex = m_SampleInfo.Dept;
		break;
	case colApplicant:							// 送检人
		contex = m_SampleInfo.Applicant;
		break;
	case colApplyDate:							// 送检日期
		tempDate = m_SampleInfo.ApplyDate;
		contex = tempDate.Format(_T("%Y-%m-%d"));
		break;
	case colProjecName:							// 项目名称
		if(m_idxTestProject >= 0)
		{
			contex = m_OptionsOfTestProjects[m_idxTestProject];
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}
		break;
	case colRepeat:								// 重复次数
		if(m_Repeat >= 0)
			contex.Format(_T("%d"), m_Repeat);
		break;
	case colTheoryConc:							// 理论浓度
		if(m_dTheoryConc >=0)
			contex.Format(_T("%g"),m_dTheoryConc);
		break;
	default:
		break;
	}
	return 0;
}

ColumnProperties CSampleConfigListItem::GetColumnProperties(const int &ColumnIndex)
{
	if(m_SampleInfo.SampleNo == _T(""))
		return CanNotChange;
	
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	switch(colIndex)
	{
	case colPos:								// 样本位
		return CanNotChange;
	case colSampleNo:							// 样本编号
		return DiretEditable;
	case colType:								// 样本类型
		return Optional;
	case colDilution:							// 稀释度
		return VariableSize;
	case colDept:								// 送检单位
		return DiretEditable;
	case colApplicant:							// 送检人
		return DiretEditable;
	case colApplyDate:							// 送检日期
		return VariableDate;
	case colProjecName:							// 项目名称
		return Optional;
	case colRepeat:								// 重复次数
		if(m_idxTestProject < 0)				// 如果还未设置项目名称，则不可编辑此项
			return CanNotChange;
		return VariableSize;
	case colTheoryConc:							// 理论浓度
		if(m_idxTestProject < 0)				// 如果还未设置项目名称，则不可编辑此项
			return CanNotChange;
		return VariableSize;
	default:
		break;
	}
	return CanNotChange;
}

bool CSampleConfigListItem::GetComboxProperties(const int &ColumnIndex,
												vector<CString>&strOptions,
												int &nDefaultSelectedIndex,
												bool &bIsEditable)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 样本类型列
	if(colIndex == colType)
	{
		for(int i=0; i < (int)m_OptionsOfSampleTypes.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfSampleTypes[i]);

			if(m_OptionsOfSampleTypes[i] == m_SampleInfo.Type)
				nDefaultSelectedIndex = i;
			bIsEditable = true;
		}
		return true;
	}

	// 项目名称列
	if(colIndex == colProjecName)
	{
		for(int i=0; i < (int)m_OptionsOfTestProjects.size(); i++)	
		{
			strOptions.push_back(m_OptionsOfTestProjects[i]);
			nDefaultSelectedIndex = m_idxTestProject;
		}
		bIsEditable = false;
		return true;
	}

	return false;
}

void CSampleConfigListItem::OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 样本类型列
	if(colIndex == colType)
	{
		m_SampleInfo.Type = m_OptionsOfSampleTypes[newSelectedIndex];
	}

	// 项目名称列
	if(colIndex == colProjecName)
	{
		m_idxTestProject = newSelectedIndex;
	}
}

bool CSampleConfigListItem::GetSpinBtnProperties(const int &ColumnIndex,
													double &initial,
													double &lower, 
													double & upper,
													double &increament)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	// 稀释倍数列：上限100000、下限1、增量2
	if(colIndex == colDilution)
	{
		initial = static_cast<double> (m_SampleInfo.Dilution);
		lower = 1;									
		upper = 100000;
		increament = 2 ;	
		return true;
	}

	// 重复次数列：上限20、下限0、增量1
	if(colIndex == colRepeat)
	{
		initial = static_cast<double> (m_Repeat);
		lower = 0;									
		upper = 20;
		increament = 1;
		return true;
	}

	// 理论浓度列：上限100、下限0、增量01
	if(colIndex == colTheoryConc)
	{
		initial = static_cast<double> (m_dTheoryConc);
		lower = 0;									
		upper = 100;
		increament = 0.1;
		return true;
	}
	return false;
}

bool CSampleConfigListItem::GetDateCtrlProperties(const int &ColumnIndex,COleDateTime &initial)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 送检时间
	if(colIndex == colApplyDate)
	{
		initial = m_SampleInfo.ApplyDate;
		return true;
	}
	return false;
}

void CSampleConfigListItem::OnDtnchange(const int &ColumnIndex,const COleDateTime & newDate)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 送检时间
	if(colIndex == colApplyDate)
	{
		m_SampleInfo.ApplyDate = newDate;
		return;
	}
}

void CSampleConfigListItem::OnEnChange(const int &ColumnIndex,const CString & newContex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	// 样本编号
	if(colIndex == colSampleNo)
	{
		m_SampleInfo.SampleNo = newContex;
	}

	// 送检人
	if(colIndex == colApplicant)
	{
		m_SampleInfo.Applicant = newContex;
	}

	// 送检单位
	if(colIndex == colDept)
	{
		m_SampleInfo.Dept = newContex;
	}

	// 稀释倍数
	if(colIndex == colDilution)
	{
		m_SampleInfo.Dilution = _tstoi(newContex);
	}

	// 重复次数
	if(colIndex == colRepeat)
	{
		m_Repeat = _tstoi(newContex);

		if(m_Repeat > 20)
			m_Repeat = 20;

		if(m_Repeat < 0)
			m_Repeat = 0;
	}

	// 理论浓度
	if(colIndex == colTheoryConc)
	{
		m_dTheoryConc = _tstof(newContex);
	}
}

bool CSampleConfigListItem::IsBeforThan(const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CSampleConfigListItem * pData = static_cast<const CSampleConfigListItem*>(pItemData);

	// 正常升序方式
	bool NormalAscOrder;
	switch(SortColumn)
	{
	case colPos:
		if (m_PosNum ==  pData->GetPosNum())
		{
			// 如果样本位相等，则按检测项目升序排序
			if(m_idxTestProject < pData->GetProjectIndex())
				return true;
			else
				return false;
		}
		else if(m_PosNum >  pData->GetPosNum())
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

bool CSampleConfigListItem::ChangeSortMode(const int & col)
{
	ListColumn colIndex = static_cast<ListColumn> (col);
	switch(colIndex)
	{
	case colPos:
		break;
	default:
		return false;
	}

	// 如果排序列与原排序列一样，则改变顺序倒序方式
	if(CSampleConfigListItem::SortColumn == colIndex)
	{

		// 排序方式改变：如果原来时顺序，则改为倒序
		if (CSampleConfigListItem::IsAsc == true)
			CSampleConfigListItem::IsAsc = false;
		else
			CSampleConfigListItem::IsAsc = true;
	}
	// 如果排序列与原排序列不同，则改变排序列
	else
	{
		CSampleConfigListItem::SortColumn = colIndex;
	}
	return true;
}
