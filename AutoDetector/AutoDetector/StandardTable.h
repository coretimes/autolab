//*****************************************************************************
// �ļ��� : StandardTable.h
// 
// ��Ҫ   : ��׼Ʒ����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

// �궨�壺��׼��Ũ��ȡֵ
struct StandardRecord
{
	Sample_Key ID;				// key
	CString BatchNum;			// ����
	CString ProjectName;		// ��Ŀ����
	CString ShortName;			// ��Ŀ���
	DATE ProductionDate;		// ��������
	DATE ExpirationDate;		// ʧЧ����
	int RatedTestTimes;			// �ʵ�����
	int RemainTimes;			// ʣ�����
	DATE FirstLoadTime;			// �״�װ��ʱ��
	DATE FirstUseTime;			// �״�ʹ��ʱ��
	CString Unit;				// Ũ�ȵ�λ
	int CountOfStdPoints;		// ��׼�����
	double StdConc[10];			// ��׼��Ũ������,��׼��1Ũ�ȱ�����StdConc[0]�С���׼��2Ũ�ȱ�����StdConcStdConc[1]��......
	StandardRecord()
	{
		ID = 0;
		ProductionDate = 0;
		ExpirationDate = 0;
		RatedTestTimes = 0;
		RemainTimes = 0;
		FirstLoadTime = 0;
		FirstUseTime = 0;
		CountOfStdPoints = 0;
		for(int i=0; i<10; i++)
			StdConc[i] = -1;
	}
};

// *******************************************************
// ��׼Ʒ��
// *******************************************************
class CStandardTable : public CDBTableTemplate<StandardRecord>
{
private:
	CStandardTable(void);

public:
	static CStandardTable * GetInstance()											// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CStandardTable();
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

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ����ָ��������ѯ��׼Ʒ��Ϣ
	// ������
	//      1.StandardList - ���ر�׼Ʒ�洢�б�
	// 		2 IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	//		3.ID - ָ��key
	//		4.strBatchNum - ����
	//		5.strProjectName - ��Ŀ����
	//		6.strShortName - ��Ŀ���
	//		7.dtProductionDateUpper - ������������
	//		8.dtProductionDateLower - ������������
	//		9.dtExpirationDateUpper - ʧЧ��������
	//		10.dtExpirationDateLower - ʧЧ��������
	// ---------------------------------------------------------------------------
	bool GetStandards(
		vector<StandardRecord> & StandardList,
		const bool &IsFuzzyQuery = true,
		const uint32 &ID = 0,
		const CString &strBatchNum = _T(""),	
		const CString &strProjectName = _T(""),
		const CString &strShortName = _T(""),
		const DATE &dtProductionDateUpper = 0, 
		const DATE &dtProductionDateLower = 0,
		const DATE &dtExpirationDateUpper = 0,
		const DATE &dtExpirationDateLower = 0
		);	          

	~CStandardTable(void);
	
	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ��ȡָ�����Ƶ����б�׼Ʒ��Ϣ
	// ������
	//      1.StandardList - ���ر�׼Ʒ�洢�б�
	//		2.strProjectName - ��Ŀ����
	// 		3 IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	// ---------------------------------------------------------------------------
	bool GetStandardsByProjecName( vector<StandardRecord>& StandardList,
									const CString &strProjectName,
									const bool &IsFuzzyQuery = false)
	{
		if( GetStandards(/*vector<StandardRecord> & = */ StandardList,
						/*bool IsFuzzyQuery = */		 IsFuzzyQuery,
						/*ID = */						 0,
						/*strBatchNum = */				_T(""),	
						/*strProjectName = */			strProjectName
					      )== true)
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

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ȡ�ø��ֲ�ͬ�����š�
	// ������
	//      1.BatchNumsList - ������Ŀ���ƵĴ洢�б�
	// ---------------------------------------------------------------------------
	void GetDistinctBatchNums(vector<CString>& BatchNumsList);					

	static void SortByBatchNumAsc(vector<StandardRecord>& samplelist)				// ��������������
	{ sort(samplelist.begin(),samplelist.end(),_LessByBatchNum);}

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ���ݼ���ж� V1 > V2 �Ƿ����
	// ---------------------------------------------------------------------------
	static bool _LessByShortName(ProjectFullName & v1,ProjectFullName & v2)	
	{ return v1.ShortName < v2.ShortName; }

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// ����ĳָ��keyֵ�ļ�¼�����ݿ�
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
	static bool _LessByBatchNum(StandardRecord & v1,StandardRecord & v2)			// ����key �ж� V1 > V2 �Ƿ����
	{ return v1.BatchNum < v2.BatchNum; }
																					// �����ݿ��ж�ȡ��¼
private:
	static CStandardTable * m_pInstance;
};
