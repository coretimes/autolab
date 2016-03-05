//*****************************************************************************
// �ļ��� : TestConfirmListItem.h
// 
// ����   : ����ȷ���б�ؼ���������б�λ�ڡ�������Ϣȷ�ϡ��Ի�����
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

namespace TestConfirmList
{
	enum ListColumn
	{
		colType = 0,				// ��Ʒ����
		colSampleNo,				// ��Ʒ���
		colConcPoint,				// Ũ�ȵ�
		colSamplePos,				// ����λ
		colProject,					// �����Ŀ
		colRepeatTimes,				// �ظ�����
		ColumnCount
	};
}
using namespace TestConfirmList;

class CTestConfirmListItem :	public CListItemData
{
public:
	CTestConfirmListItem(const Sample_Pos & SamplePos,const Reagent_Pos &ReagentPos);
public:
	~CTestConfirmListItem(void);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �жϱ���ĳ�ֶ��Ƿ�������ĳ������ͬ�ֶ���ȣ�
	//     �����ʱ�����Ʊ��ʱ��Ҫ�ϲ�Ϊһ����Ԫ��
	// ������
	//     1.ColumnIndex - ������
	//     2.pItemData - ���б�������ָ��
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData);

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
	//     ȡ������Ŀ����
	// ---------------------------------------------------------------------------
	virtual int GetSubItemCount()
	{
		return (int)ColumnCount;
	}

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
	//     �ı�����ʽ����
	// ---------------------------------------------------------------------------
	Sample_Pos GetSamplePosNum() const
	{	
		return m_SamplePos;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ���Լ�λ�ñ��
	// ---------------------------------------------------------------------------
	Reagent_Pos GetReagentPosNum() const
	{	
		return m_ReagentPos;
	}

private:
	Sample_Pos m_SamplePos;
	Reagent_Pos m_ReagentPos;

private:
	static ListColumn SortColumn;		// ��������
	static bool IsAsc;					// �Ƿ������ţ�true�ǣ�falseΪ����
};
