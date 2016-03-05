//*****************************************************************************
// �ļ��� : CurveTable.h
// 
// ��Ҫ   : ���߱���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct CurveRecord
{
	Curve_Key ID;				// key
	Standard_Key StandardID;	// ��׼ƷID
	Reagent_Key ReagentID;		// �Լ�ID
	CurveType DefaultFitType;	// Ĭ�����ģ��
	CString RluString[10];		// ����ֵ�ַ���
	CString Surveyor;			// ����Ա
	DATE TestDate;				// ��������
};

// *******************************************************
// �������ݿ����
// *******************************************************
class CCurveTable : public CDBTableTemplate<CurveRecord>
{
private:
	CCurveTable(void);

public:
	static CCurveTable * GetInstance();												// ȡ��ʵ��ָ��

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

	~CCurveTable(void);

	void GetResultsByDate(DATE date,vector<CurveRecord>& resultlist);				// ��ȡָ���������ڵ����н����Ϣ

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ����ָ��������ѯ������Ϣ
	// ������
	//      1.CurveList - ���ر�׼Ʒ�洢�б�
	// 		2 IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	//		3.strBatchNum - ����
	//		4.strProjectName - ��Ŀ����
	//		5.strShortName - ��Ŀ���
	//		6.dtTestDateUpper - ������������
	//		7.dtTestDateLower - ������������
	//		8.strSurveyor - ����Ա
	// ---------------------------------------------------------------------------
	bool GetCurves(
		vector<CurveRecord> & CurveList,
		const bool &IsFuzzyQuery = true,
		const CString &strBatchNum = _T(""),	
		const CString &strProjectName = _T(""),
		const CString &strShortName = _T(""),
		const DATE &dtTestDateUpper = 0, 
		const DATE &dtTestDateLower = 0,
		const CString &strSurveyor = _T(""));

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// ����ĳָ��keyֵ�ļ�¼�����ݿ�
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																					// �����ݿ��ж�ȡ��¼
private:
	static CCurveTable * m_pInstance;
};
