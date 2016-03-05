//*****************************************************************************
// �ļ��� : SampleConfigListItem.h
// 
// ����   : �������������б�������,���б�λ�ڡ��������á������Ի�����
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
#include "listitemdata.h"
#include "SampleTable.h"

namespace SampleConfigList
{
	enum ListColumn
	{
		colPos = 0,					// ����λ
		colSampleNo,				// �������
		colType,					// ��������
		colDilution,				// ϡ�ͱ���
		colDept,					// �ͼ쵥λ����
		colApplicant,				// �ͼ���
		colApplyDate,				// �ͼ�����
		colProjecName,				// �����Ŀ����
		colRepeat,					// �ظ����Դ���
		colTheoryConc,				// ����Ũ��
		ColumnCount
	};
}

using namespace SampleConfigList;

class CSampleConfigListItem :	public CListItemData
{
public:
	CSampleConfigListItem(void);
	~CSampleConfigListItem(void);
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
	void SetPosNum(const Sample_Pos &PosNum);

	Sample_Pos GetPosNum() const
	{
		return m_PosNum;
	}

	void SetSampleNo(const CString &strSampleNo)
	{
		m_SampleInfo.SampleNo = strSampleNo;
	}

	void SetSampleType(const CString &strSampleType)
	{
		m_SampleInfo.Type = strSampleType;
	}
	void SetSampleDilution(const double &dDilution)
	{
		m_SampleInfo.Dilution = dDilution;
	}

	void SetSampleDept(const CString &strSampleDept)
	{
		m_SampleInfo.Dept = strSampleDept;
	}

	void SetSampleApplicant(const CString &strApplicant)
	{
		m_SampleInfo.Applicant = strApplicant;
	}

	void SetRepeat(const int &repeat)
	{
		m_Repeat = repeat;
	}

	int GetRepeat() const
	{
		return m_Repeat;
	}

	void SetApplyDate(COleDateTime date)
	{
		m_SampleInfo.ApplyDate = date;
	}

	void SetTypeOptions(const vector<CString> & options)
	{
		for(int i=0; i<(int)options.size(); i++)
		{
			m_OptionsOfSampleTypes.push_back(options[i]);
		}
	}

	void SetProjectOptions(const vector<CString> & options)
	{
		for(int i=0; i< (int)options.size(); i++)
		{
			m_OptionsOfTestProjects.push_back(options[i]);
		}
	}

	void SetProjectIndex(const int & indexOfProject)
	{
		m_idxTestProject = indexOfProject;
	}

	int GetProjectIndex() const
	{
		return m_idxTestProject;
	}
	
	void SetThoeryConc(const double & conc)
	{
		m_dTheoryConc = conc;
	}

	double GetThoeryConc() const
	{
		return m_dTheoryConc;
	}

	void GetSampleInfo(SampleRecord & record) const
	{
		record = m_SampleInfo;
	}

	void SetSampleInfo(const SampleRecord & record)
	{
		m_SampleInfo = record;
	}
	// ---------------------------------------------------------------------------
	// ժҪ��
	//     �жϱ���ĳ�е�Ԫ���Ƿ�������ĳ������ͬ�е�Ԫ��ȼۣ�
	//     ���ȼ�ʱ�����Ʊ��ʱ��Ҫ�ϲ�Ϊһ����Ԫ��
	// ������
	//     1.ColumnIndex - ������
	//     2.pItemData - ���б�������ָ��
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData);

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
	//     �༭�ؼ����ݸ�����Ӧ����
	// ������
	//     1.ColumnIndex - ������
	//     2.contex - �ı�����
	// ---------------------------------------------------------------------------
	virtual void OnEnChange(const int &ColumnIndex,const CString & newContex);

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
		return (int)ColumnCount;
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
	//     ��Ͽ�ؼ�ѡ�������Ӧ����
	// ������
	//     1.ColumnIndex - ������
	//     2.newSelectedIndex - ��ѡ������
	// ---------------------------------------------------------------------------
	virtual void OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex);

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
	//     ȡ��ʱ��ؼ�������
	//     1.ColumnIndex - ��������������в���DateCtrl���򷵻�false
	//     2.fInitial - ��ʼֵ
	// ---------------------------------------------------------------------------
	virtual bool GetDateCtrlProperties(const int &ColumnIndex,COleDateTime &initial);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ��Ͽ�ؼ�ѡ�������Ӧ����
	// ������
	//     1.ColumnIndex - ������
	//     2.newDate - ��ѡ������
	// ---------------------------------------------------------------------------
	virtual void OnDtnchange(const int &ColumnIndex,const COleDateTime & newDate);

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

private:
	Sample_Pos m_PosNum;					 // ������λ�ñ��
	SampleRecord m_SampleInfo;				 // ������¼
	vector<CString> m_OptionsOfTestProjects; // ��Ŀ����ѡ��
	int m_idxTestProject;					 // �����Ŀ��m_OptionsOfTestProjects�е�����
	int m_Repeat;							 // �ظ�����
	double m_dTheoryConc;					 // ����Ũ��
	vector<CString> m_OptionsOfSampleTypes;  // ��������ѡ��

private:
	static ListColumn SortColumn;			// ��������
	static bool IsAsc;						// �Ƿ������ţ�true�ǣ�falseΪ����
};
