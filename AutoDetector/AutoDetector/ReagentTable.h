//*****************************************************************************
// �ļ��� : ReagentTable.h
// 
// ��Ҫ   : �Լ�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>
#include "Types.h"

struct ReagentRecord
{
	Reagent_Key ID;				// key
	CString BatchNum;			// ����
	CString ProjectName;		// ��Ŀ����
	CString ShortName;			// ��Ŀ���
	DATE ProductionDate;		// ��������
	DATE ExpirationDate;		// ʧЧ����
	int RatedTestTimes;			// �ʵ�����
	int RemainTimes;			// ʣ�����
	DATE FirstLoadTime;			// �״�װ��ʱ��
	DATE FirstUseTime;			// �״�ʹ��ʱ��
	CString Unit;				// ������λ
	double UpperLimit;			// �������
	double LowerLimit;			// �������
	double SampleAmount;		// ��������
	double MagneticAmount;		// �ű�����
	double HRPAmount;			// ø������
	double Dilution;			// ϡ�ͱ���
	double WashTimes;			// ��ϴ����
	double IncubationTime;		// ����ʱ��
};

// *******************************************************
// �Լ�����
// *******************************************************
class CReagentTable : public CDBTableTemplate<ReagentRecord>
{
private:
	CReagentTable(void);

public:
	static CReagentTable * GetInstance() 											// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CReagentTable();
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

	~CReagentTable(void);

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ����ָ��������ѯ�����Ϣ
	// ������
	//      1.ReagentList - �����Լ��б�
	// 		2 IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	//		3.ID - ָ��key
	//		4.BatchNum - ����
	//		5.ProjectName - ��Ŀ����
	//		6.ShortName - ��Ŀ���
	//		7.ProductionDateUpper - ������������
	//      8.ProductionDateLower - ������������
	//		9.ExpirationDateUpper - ʧЧ��������
	//      10.ExpirationDateLower - ʧЧ��������
	//      11.FirstLoadTime - �״�װ��ʱ��
	//      12.FirstUseTime  -�״�ʹ��ʱ��
	//      13.UpperLimit   - �������
	//      14.LowerLimit  - �������
	// ---------------------------------------------------------------------------
	bool GetReagents(
		vector<ReagentRecord> & ReagentList,
		const bool &IsFuzzyQuery = true,
		const Reagent_Key &ID = 0,					          
		const CString &strBatchNum = _T(""),			         
		const CString &strProjectName =_T(""),			            
		const CString &strShortName = _T(""),                  
		const DATE &ProductionDateUpper = 0,
		const DATE &ProductionDateLower = 0,
		const DATE &ExpirationDateUpper =0,
		const DATE &ExpirationDateLower =0,
		const DATE   &FirstLoadTime=0,
		const DATE   &FirstUseTime =0,
		const double &UpperLimit = 0,			
		const double &LowerLimit = 0			
		);

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ��ȡָ���������ڵ����н����Ϣ
	// ������
	// 		1 date - ָ��������
	//      2.resultlist - ���ؽ���б�
	// ---------------------------------------------------------------------------
	bool GetReagentsByDate(const DATE &date,vector<ReagentRecord>& resultlist)
	{
		if( GetReagents(/*resultlist=*/          resultlist,
			/*IsFuzzyQuery=*/                         false,
			/*ID=*/                                       0,
			/*strBatchNum=*/                         _T(""),
			/*strProjectName=*/                      _T(""),
			/*strShortName=*/                        _T(""),
			/*ProductionDateUpper=*/                   date,
			/*ProductionDateLower=*/                   date,
			/*ExpirationDateUpper=*/                      0,
			/*ExpirationDateLower=*/                      0,
			/*FirstLoadTime=*/                            0,
			/*FirstUseTime*/                              0,
			/*UpperLimit*/                                0,
			/*LowerLimit*/                                0
			) == true)
			return true;
		else
			return false;
	}

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ��ȡָ����Ŀ���Ƶ������Լ������Ϣ
	// ������
	//      1.resultlist - ���ؽ���б�
	// 		2.strProjectName - ָ������Ŀ����
	// 		3.IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	// ---------------------------------------------------------------------------
	bool GetReagentsByProjectName(vector<ReagentRecord>& resultlist,const CString &strProjectName,const bool &IsFuzzyQuery = false)
	{
		if( GetReagents(/*resultlist=*/          resultlist,
			/*IsFuzzyQuery=*/                  IsFuzzyQuery,
			/*ID=*/                                       0,
			/*strBatchNum=*/                         _T(""),
			/*strProjectName=*/              strProjectName,
			/*strShortName=*/                        _T(""),
			/*ProductionDateUpper=*/                      0,
			/*ProductionDateLower=*/                      0,
			/*ExpirationDateUpper=*/                      0,
			/*ExpirationDateLower=*/                      0,
			/*FirstLoadTime=*/                            0,
			/*FirstUseTime*/                              0,
			/*UpperLimit*/                                0,
			/*LowerLimit*/                                0
			) == true)
			return true;
		else
			return false;
	}

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ȡ�ø��ֲ�ͬ����Ŀ���ơ�
	// ������
	//      1.NameList - ������Ŀ���ƵĴ洢�б�
	// ---------------------------------------------------------------------------
	void GetDistinctProjectNames(vector<ProjectFullName>& NameList);					

	static void SortByBatchNumAsc(vector<ReagentRecord>& resultlist)				// ��������������
	{ sort(resultlist.begin(),resultlist.end(),_LessByBatchNum);}

private:
	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ����ĳָ��keyֵ�ļ�¼�����ݿ�
	// ������
	//      1.KeyList - ָ����keyֵ�б�
	// ---------------------------------------------------------------------------
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);

	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ����key �ж� V1 > V2 �Ƿ����
	// ---------------------------------------------------------------------------
	static bool _LessByBatchNum(ReagentRecord & v1,ReagentRecord & v2)	
	{ return v1.BatchNum < v2.BatchNum; }

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ����key �ж� V1 > V2 �Ƿ����
	// ---------------------------------------------------------------------------
	static bool _LessByShortName(ProjectFullName & v1,ProjectFullName & v2)	
	{ return v1.ShortName < v2.ShortName; }

private:
	static CReagentTable * m_pInstance;
};
