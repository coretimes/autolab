//*****************************************************************************
// �ļ��� : ResultTable.h
// 
// ��Ҫ   : �����������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct ResultRecord
{
	uint32 ID;					// key
	uint32 SampleID;			// ����ID
	uint32 ReagentID;			// �Լ�ID
	uint32 CurveID;				// ��׼����ID
	CString RluString;			// ����ֵ�ַ���
	double TheoryConc;			// ����Ũ��
	double FitConc;				// ���Ũ��
	CString Unit;				// Ũ�ȵ�λ
	CString Surveyor;			// ����Ա
	DATE TestDate;				// ��������
};

// *******************************************************
// �������
// *******************************************************
class CResultTable : public CDBTableTemplate<ResultRecord>
{
private:
	CResultTable(void);

public:
	static CResultTable * GetInstance() 											// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CResultTable();
		return m_pInstance;
	}
	// --------------------------------------------------
	// ժҪ��
	//	   ɾ��ʵ��
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CResultTable(void);

	void GetResultsBySampleID(uint32 ReagentID,vector<ResultRecord>& resultlist);	// ��ȡָ������ID�����н����Ϣ
	void GetResultsByReagentID(uint32 ReagentID,vector<ResultRecord>& resultlist);	// ��ȡָ�����ID�����н����Ϣ
	void GetResultsByDate(DATE date,vector<ResultRecord>& resultlist);				// ��ȡָ���������ڵ����н����Ϣ

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// ����ĳָ��keyֵ�ļ�¼�����ݿ�
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																					// �����ݿ��ж�ȡ��¼
private:
	static CResultTable * m_pInstance;
};
