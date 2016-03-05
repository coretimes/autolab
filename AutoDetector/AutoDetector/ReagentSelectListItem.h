//*****************************************************************************
// �ļ��� : ReagentSelectListItem.h
// 
// ����   : �Լ�ѡ���б����ݶ��󣬱��б�λ�ڡ�ѡ���Լ��������Ի�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-1        ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "listitemdata.h"
#include "ReagentTable.h"
namespace ReagentSelectList
{
	enum ListColumn
	{
		colProject = 0,					// �����Ŀ
		colBatchNum,					// �Լ�����
		colProductDate,					// ��������
		ColumnCount
	};
}
using namespace ReagentSelectList;

class CReagentSelectListItem :	public CListItemData
{
public:
	CReagentSelectListItem(	const Reagent_Key & key,
							const CString & strBatchNum,
							const ProjectFullName & ProjectName,
							const COleDateTime & dtProductDate);
	~CReagentSelectListItem();

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
		return (int)ReagentSelectList::ColumnCount;
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
	//     ȡ����Ŀ����
	// ---------------------------------------------------------------------------
	ProjectFullName GetProjectName() const
	{
		return m_ProjectName;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ������
	// ---------------------------------------------------------------------------
	CString GetBatchName() const
	{
		return m_strBatchNum;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ���Լ�ID
	// ---------------------------------------------------------------------------
	Reagent_Key GetReagentKey()
	{
		return m_ReagentKey;
	}

private:
	Reagent_Key m_ReagentKey;
	CString m_strBatchNum;
	ProjectFullName m_ProjectName;
	COleDateTime m_dtProductDate;

private:
	static ListColumn SortColumn;		// ��������
	static bool IsAsc;					// �Ƿ������ţ�true�ǣ�falseΪ����
};
