//*****************************************************************************
// �ļ��� : SampleTable.h
// 
// ��Ҫ   : ��������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-7       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

// *******************************************************
// �������¼�ṹ
// *******************************************************
struct SampleRecord
{
	Sample_Key ID;				// key
	CString SampleNo;			// �������,�˹�������Զ�����
	CString Type;				// ��������
	double Dilution;			// ϡ�ͱ���
	CString Dept;				// �ͼ쵥λ����
	CString Applicant;			// �ͼ���
	DATE ApplyDate;				// �ͼ�����
	DATE UpdateDate;			// ����¼������
	SampleRecord()
	{
		ID = 0;
		Dilution = 0;			// ϡ�ͱ���
		ApplyDate = 0;			// �ͼ�����
		UpdateDate = 0;			// ����¼������
	}
};

// *******************************************************
// ��Ʒ����
// *******************************************************
class CSampleTable : public CDBTableTemplate<SampleRecord>
{
private:
	CSampleTable(void);

public:
	static CSampleTable * GetInstance() 									// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSampleTable();
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

	~CSampleTable(void);

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ����ָ��������ѯ������Ϣ
	// ������
	//      1.SampleList - ���������洢�б�
	// 		2 IsFuzzyQuery - �Ƿ�ģ����ѯ,ֻ���ַ�������������Ч
	//		3.ID - ָ��key
	//		4.strSampleNo - �걾��
	//		5.strType - ��������
	//		6.dDilutionUpper - ϡ�ͱ�������
	//		7.dDilutionLower - ϡ�ͱ���������
	//		8.strDept - �ͼ쵥λ����
	//		9.strApplicant - �ͼ���
	//		10.ApplyDateUpper - �ͼ�ʱ����
	//      11.ApplyDateLower - �ͼ�ʱ������
	//		12.UpdateDateUpper - ����ʱ������
	//		13.UpdateDateLower - ����ʱ������
	// ---------------------------------------------------------------------------
	void GetSamples(
		vector<SampleRecord> & SampleList,
		const bool &IsFuzzyQuery = true,
		const uint32 &ID = 0,					          
		const CString &strSampleNo = _T(""),			         
		const CString &strType = _T(""),			            
		const double &dDilutionUpper = 0,                  
		const double &dDilutionLower = 0,
		const CString &strDept = _T(""),				      
		const CString &strApplicant = _T(""),
		const DATE &ApplyDateUpper = 0,				     
		const DATE &ApplyDateLower = 0,
		const DATE &UpdateDateUpper = 0,             
		const DATE &UpdateDateLower = 0
		);	          

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//     ����ָ�������������ڲ�ѯ������Ϣ
	// ---------------------------------------------------------------------------
	void GetSamplesByUpdateDate(vector<SampleRecord>& SampleList,const DATE &date)
	{
		GetSamples(	/*SampleList = */		SampleList,
					/* IsFuzzyQuery = */    false,
					/*ID = */				0,					          
					/*strSampleNo = */		_T(""),			         
					/*&strType = */			_T(""),			            
					/*dDilutionUpper = */	0,                  
					/*dDilutionLower = */	0,
					/*strDept = */			_T(""),				      
					/*strApplicant = */		_T(""),
					/*ApplyDateUpper = */	0,				     
					/*ApplyDateLower = */	0,
					/*UpdateDateUpper = */	date,             
					/*UpdateDateLower = */	date
				   );
	}

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ��ȡ����������Ϣ
	// ---------------------------------------------------------------------------
	void GetAllSamples(vector<SampleRecord>& SampleList)					
	{
		GetSamples(SampleList);
	}

	// ---------------------------------------------------------------------------
	// �������ܣ�
	//	   ȡ�ø��ֲ�ͬ���������͡�����
	// ������
	//      1.TypeList - ���������������ƵĴ洢�б�
	// ---------------------------------------------------------------------------
	void GetDistinctType(vector<CString>& TypeList);					

	static void SortByIDDesc(vector<SampleRecord>& samplelist)				// ��keyֵ��������
	{ sort(samplelist.begin(),samplelist.end(),_GreaterByID);}

	static void SortByIDAsc(vector<SampleRecord>& samplelist) 				// ��keyֵ��������
	{ sort(samplelist.begin(),samplelist.end(),_LessByID);}

	static void SortBySampleNoDesc(vector<SampleRecord>& samplelist) 		// �������Ž�������
	{ sort(samplelist.begin(),samplelist.end(),_GreaterBySampleNo);}

	static void SortBySampleNoAsc(vector<SampleRecord>& samplelist) 		// ����������������
	{ sort(samplelist.begin(),samplelist.end(),_LessBySampleNo);}

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);					// ����ĳָ��keyֵ�ļ�¼�����ݿ�
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																			// �����ݿ��ж�ȡ��¼

	// ---------------------------------------------------------------------------
	// �����ñȽϺ���
	// ---------------------------------------------------------------------------
	static bool _GreaterByID(SampleRecord & v1,SampleRecord & v2)			// ����key �ж� V1 > V2 �Ƿ����
	{ return v1.ID > v2.ID; }
	static bool _LessByID(SampleRecord & v1,SampleRecord & v2)				// ����key �ж� V1 < V2 �Ƿ����
	{ return v1.ID < v2.ID; }
	static bool _GreaterBySampleNo(SampleRecord & v1,SampleRecord & v2)		// ���������� �ж� V1 > V2 �Ƿ����
	{ return v1.SampleNo > v2.SampleNo; }
	static bool _LessBySampleNo(SampleRecord & v1,SampleRecord & v2)		// ���������� �ж� V1 < V2 �Ƿ����
	{ return v1.SampleNo < v2.SampleNo; }
private:
	static CSampleTable * m_pInstance;
};
