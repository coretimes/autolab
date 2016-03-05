//*****************************************************************************
// �ļ��� : CurveListItem.h
// 
// ����   : ����ѡ���б����ݶ��󣬱��б�λ�ڡ��������Ի�����
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
namespace CurveList
{
	enum ListColumn
	{
		colProject = 0,				// �����Ŀ
		colBatchNum,				// ��׼����
		colTestDate,				// ��������
		colDefaultFitType,			// Ĭ�����ģ��
		colSurveyor,				// ����Ա
		ColumnCount
	};
}

using namespace CurveList;

class CCurveListItem : public CListItemData
{
public:
	CCurveListItem(const CurveRecord & curve);
public:
	~CCurveListItem(void);

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
	//     �жϱ����Ƿ�λ������ĳ������֮ǰ,����ǣ�����Ҫ�ѱ�������pItemData����˳��
	// ������
	//     1.pItemData - �Ƚ���ָ��
	// ---------------------------------------------------------------------------
	virtual bool IsBeforThan(const CListItemData * pItemData);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �ı�����ʽ����
	// ---------------------------------------------------------------------------
	virtual bool ChangeSortMode(const int & col);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ������Ŀ����
	// ---------------------------------------------------------------------------
	virtual inline int GetSubItemCount()
	{
		return (int)CurveList::ColumnCount;
	}
	
	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ�ò���ʱ��
	// ---------------------------------------------------------------------------
	COleDateTime GetTestTime() const
	{
		return m_CurveRecord.TestDate;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ������key
	// ---------------------------------------------------------------------------
	Curve_Key GetCurveKey() const
	{
		return m_CurveRecord.ID;
	}

private:
	CurveRecord m_CurveRecord;
	CString m_strProjectName;
	CString m_strBatchNum;

private:
	static ListColumn SortColumn;		// ��������
	static bool IsAsc;					// �Ƿ������ţ�true�ǣ�falseΪ����
};
