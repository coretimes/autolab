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
	case colIndex:									// ��׼������
		contex.Format(_T("%c"),m_nConcIndex + 64);
		break;

	case colTheoryConc:								// ����Ũ��
		contex.Format(_T("%g%s"),m_dTheoryConc,m_strUint);
		break;

	case colTestRlu:								// ��������ֵ
		contex.Format(_T("%g"),m_dTestRlu);
		break;

	case colAverageRlu:								// ƽ������ֵ
		contex.Format(_T("%g"),m_dAverageRlu);
		break;

	case colCalConc:								// ����Ũ��
		contex.Format(_T("%g%s"),m_dCalConc,m_strUint);
		break;

	case colFitError:								// ������
		contex.Format(_T("%g"),m_dFitError);
		break;

	case colCV:										// ����ϵ��
		contex.Format(_T("%g"),m_dCV);				
		break;
	case colInhibitionRate:								// ������
		contex.Format(_T("%g"),m_dInhibitionRate);				
		break;

	default:
		break;
	}
	return 0;
}