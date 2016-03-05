#include "StdAfx.h"
#include "CurveDetailListItem.h"

CCurveDetailListItem::CCurveDetailListItem(int nConcIndex, 
										   double dTheoryConc,
										   CString strUnit,
										   double dTestRlu,
										   double dAverageRlu,
										   double dCalConc,
										   double dFitError,
										   double dCV,
										   double dInhibitionRate)
{
	m_nConcIndex = nConcIndex;
	m_dTheoryConc = dTheoryConc;
	m_strUint = strUnit;
	m_dTestRlu = dTestRlu;
	m_dAverageRlu = dAverageRlu;
	m_dCalConc = dCalConc; 
	m_dFitError = dFitError;
	m_dCV = dCV;
	m_dInhibitionRate = dInhibitionRate;
}

CCurveDetailListItem::~CCurveDetailListItem(void)
{
}

UINT CCurveDetailListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	switch(ColumnIndex)
	{
	case colIndex:									// 标准点索引
		contex.Format(_T("%c"),m_nConcIndex + 64);
		break;

	case colTheoryConc:								// 理论浓度
		contex.Format(_T("%g%s"),m_dTheoryConc,m_strUint);
		break;

	case colTestRlu:								// 测量发光值
		contex.Format(_T("%g"),m_dTestRlu);
		break;

	case colAverageRlu:								// 平均发光值
		contex.Format(_T("%g"),m_dAverageRlu);
		break;

	case colCalConc:								// 计算浓度
		contex.Format(_T("%g%s"),m_dCalConc,m_strUint);
		break;

	case colFitError:								// 拟合误差
		contex.Format(_T("%g"),m_dFitError);
		break;

	case colCV:										// 变异系数
		contex.Format(_T("%g"),m_dCV);				
		break;
	case colInhibitionRate:								// 抑制率
		contex.Format(_T("%g"),m_dInhibitionRate);				
		break;

	default:
		break;
	}
	return 0;
}