//*****************************************************************************
// �ļ��� : CurveDetailListItem.h
// 
// ����   : ������ϸ��Ϣ�б������ݣ����б�λ�ڡ��������Ի�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-2        ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "listitemdata.h"
#include "CurveTable.h"

namespace CurveDetailListItem
{
	enum ListColumn
	{
		colIndex = 0,			// ��׼������
		colTheoryConc,			// ����Ũ��
		colTestRlu,				// ��������ֵ
		colAverageRlu,			// ƽ������ֵ
		colCalConc,				// ����Ũ��
		colFitError,			// ������
		colCV,					// ����ϵ��
		colInhibitionRate,		// ������
		ColumnCount
	};
}
using namespace CurveDetailListItem;

class CCurveDetailListItem : public CListItemData
{
public:
	CCurveDetailListItem(int nConcIndex, 
						 double dTheoryConc,
						 CString strUnit,
						 double dTestRlu,
						 double dAverageRlu,
						 double dCalConc,
						 double dFitError,
						 double dCV,
						 double dInhibitionRate);

	~CCurveDetailListItem(void);
	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �жϱ���ĳ�ֶ��Ƿ�������ĳ������ͬ�ֶ���ȣ�
	//     �����ʱ�����Ʊ��ʱ��Ҫ�ϲ�Ϊһ����Ԫ��
	// ������
	//     1.ColumnIndex - ������
	//     2.pItemData - ���б�������ָ��
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
	{
		// ���б�û�кϲ���Ԫ��
		return false;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ��ĳ��Ԫ���ı�����
	// ������
	//     1.ColumnIndex - ������
	//     2.contex - �ı�����
	// ����ֵ��
	//     �ı������ʽ
	// ---------------------------------------------------------------------------
	virtual UINT GetCellContex(const int &ColumnIndex,CString & contex);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ��ĳ������
	// ---------------------------------------------------------------------------
	virtual ColumnProperties GetColumnProperties(const int &ColumnIndex)
	{
		return CanNotChange;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ������Ŀ����
	// ---------------------------------------------------------------------------
	virtual inline int GetSubItemCount()
	{
		return (int)CurveDetailListItem::ColumnCount;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ��Ũ�ȵ�����
	// ---------------------------------------------------------------------------
	int GetConcIndex() const
	{
		return m_nConcIndex;
	}

private:
	int m_nConcIndex;
	double m_dTheoryConc;
	CString m_strUint;
	double m_dTestRlu;
	double m_dAverageRlu;
	double m_dCalConc;
	double m_dFitError;
	double m_dCV;
	double m_dInhibitionRate;
};