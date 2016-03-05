//*****************************************************************************
// �ļ��� : MorniterListItem.h
// 
// ����   : ���Լ���б�ؼ���������б�λ�ڡ����Խ��ȼ�ء��Ի�����
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
#include "MeteringManagement.h"
#include "TestUnit.h"

namespace MorniterListItem
{
	enum ListColumn
	{
		colType = 0,				// ��Ʒ����
		colProject,					// �����Ŀ
		colSampleNo,				// ��Ʒ���
		colConcPoint,				// Ũ�ȵ�
		colSamplePos,				// ����λ
		colMeteringIndex,			// ���ڵ�����
		colFinishTime,				// ���ʱ��
		colMeteringState,			// ���״̬
		colUseState,				// ʹ��״̬
		colRlu,						// ����ֵ
		colAverageRlu,				// ƽ������ֵ
		colCV,						// ����ϵ��
		colTheoryConc,				// ����Ũ��
		colCalConc,					// ����Ũ��
		colCurve,					// ��׼����
		ColumnCount
	};
}
using namespace MorniterListItem;

class CMorniterListItem :public CListItemData
{
public:
	CMorniterListItem(Metering_ID ID);
	~CMorniterListItem(void);

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
	//     ȡ�ò��ڵ�
	// ---------------------------------------------------------------------------
	const MeteringNode & GetMeteringNode() const
	{	return m_MeteringNode;}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ�ò��Ե�Ԫ
	// ---------------------------------------------------------------------------
	const CTestUnit * GetTestUnit() const
	{	return m_pTestUnit;}

private:
	MeteringNode m_MeteringNode;				// ��ܽڵ���Ϣ
	SampleTestNode m_SampleTestNode;			// �����ڵ���Ϣ
	CTestUnit * m_pTestUnit;					// ���Ե�Ԫ��Ϣ
};
