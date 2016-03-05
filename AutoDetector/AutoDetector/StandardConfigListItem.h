//*****************************************************************************
// �ļ��� : StandardConfigListItem.h
// 
// ����   : ��׼Ʒ���������б������ݣ����б�λ�ڡ���׼Ʒ���á������Ի�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-10       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "ListItemData.h"

namespace StandardConfigList
{
	enum ListColumn
	{
		colPos = 0,					// ����λ
		colProject,					// �����Ŀ
		colBatchNum,				// ��׼Ʒ����
		colConcIndex,				// Ũ�ȵ�����
		colRepeat,					// �ظ����Դ���
		ColumnCount
	};
}

using namespace StandardConfigList;

class CStandardConfigListItem : public CListItemData
{
public:
	CStandardConfigListItem();
	~CStandardConfigListItem(void);
	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ��������
	// ������
	//     1.pos - ����λ
	//     2.nStandardRecordID - ��׼Ʒ��¼ID
	//     3.BatchNum - ����
	//     4.ConcIndex - Ũ�ȵ�����
	//     5.repeat - �ظ�����
	// ---------------------------------------------------------------------------
	void SetPosNum(const uint16 &PosNum = 0);

	uint16 GetPosNum()
	{
		return m_PosNum;
	}

	void SetBatchNum(const CString &BatchNum)
	{
		m_strBatchNum = BatchNum;
	}

	CString GetBatchNum() const
	{
		return m_strBatchNum;
	}

	void SetProject(const CString &Project)
	{
		m_strProject = Project;
	}

	void SetConcIndex(const int &ConcIndex)
	{
		m_ConcIndex = ConcIndex;
	}

	int GetConcIndex() const
	{
		return m_ConcIndex;
	}

	void SetRepeat(const int &repeat)
	{
		m_Repeat = repeat;
	}

	int GetRepeat() const
	{
		return m_Repeat;
	}

	void SetOptionsConcIndex(const vector<CString> & arrOptions)
	{
		m_OptionsOfConcIndex.clear();
		for(int i = 0; i < (int)arrOptions.size(); i++)
		{
			m_OptionsOfConcIndex.push_back(arrOptions[i]);
		}
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �жϱ���ĳ�е�Ԫ���Ƿ�������ĳ������ͬ�е�Ԫ��ȼۣ�
	//     ���ȼ�ʱ�����Ʊ��ʱ��Ҫ�ϲ�Ϊһ����Ԫ��
	// ������
	//     1.ColumnIndex - ������
	//     2.pItemData - ���б�������ָ��
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
	{
		// ������û�кϲ���Ԫ������ȫ������false
		return false;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ��ĳ��Ԫ������
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
	virtual ColumnProperties GetColumnProperties(const int &ColumnIndex);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ������Ŀ����
	// ---------------------------------------------------------------------------
	virtual int GetSubItemCount()
	{
		return (int)StandardConfigList::ColumnCount;
	}

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ����Ͽ�����,���ĳ�е�������Optional������ô˺��������Ͽ�ѡ��
	// ������
	//     1.ColumnIndex - ��������������в���Optional���򷵻�false
	//     2.strOptions - ��������ʾѡ������
	//     3.nDefaultSelectedIndex - Ĭ��ѡ������
	// ---------------------------------------------------------------------------
	virtual bool GetComboxProperties(const int &ColumnIndex,
									 vector<CString>&strOptions,
									 int &nDefaultSelectedIndex,
									 bool &bIsEditable);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ��΢����ť������
	// ������
	//     1.ColumnIndex - ��������������в���Spin���򷵻�false
	//     2.fInitial - ��ʼֵ
	//     3.lower - ȡֵ��Χ����
	//     4.upper - ȡֵ��Χ����
	//     5.increament - ��ť��������
	virtual bool GetSpinBtnProperties(const int &ColumnIndex,
									  double &initial,
									  double &lower, 
									  double &upper,
									  double &increament);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ��Ͽ�ؼ�ѡ�������Ӧ����
	// ������
	//     1.ColumnIndex - ������
	//     2.newSelectedIndex - ��ѡ������
	// ---------------------------------------------------------------------------
	virtual void OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �༭�ؼ����ݸ�����Ӧ����
	// ������
	//     1.ColumnIndex - ������
	//     2.contex - �ı�����
	// ---------------------------------------------------------------------------
	virtual void OnEnChange(const int &ColumnIndex,const CString & newContex);

private:
	uint16 m_PosNum;						// ����λ
	CString m_strProject;					// �����Ŀ
	CString m_strBatchNum;					// ����
	int m_ConcIndex;						// Ũ�ȵ�����
	int m_Repeat;							// �ظ�����
	vector<CString> m_OptionsOfConcIndex;	// Ũ�ȵ�����ѡ��
};
