#include "StdAfx.h"
#include "SampleTable.h"
#include "DBConnect.h"

CSampleTable * CSampleTable::m_pInstance = NULL;

CSampleTable::CSampleTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("Sample");
	m_nMaxID = _SelectMaxIDFromDB();
}

CSampleTable::~CSampleTable(void)
{
}

void CSampleTable::GetDistinctType(vector<CString>& TypeList)
{
	CString strSQL;

	// Ϊ��ʹ��_SelectRecordsFromDB������sql��д��������ʽ������ʹ��distinct
	strSQL.Format(_T("select * from %s where ID in ( select Max(ID) from %s group by Type) "),m_strTableName,m_strTableName);

	vector<UINT> keylist;
	_SelectRecordsFromDB(strSQL,keylist);
	for(int i = 0;i < (int)keylist.size(); i++)
	{
		TypeList.push_back(m_BufferList.find(keylist[i])->second.Type);
	}
}

void CSampleTable::GetSamples(vector<SampleRecord>& samplelist,
							  const bool &IsFuzzyQuery,
							  const uint32 &ID, 
							  const CString &SampleNo,
							  const CString &Type,
							  const double &DilutionUp,                  
							  const double &DilutionLow,
							  const CString &Dept,				      
							  const CString &Applicant,
							  const DATE &ApplyDateUp,				     
							  const DATE &ApplyDateLow,
							  const DATE &UpdateDateUp,             
							  const DATE &UpdateDateLow)
{
	CString strSQL;
	strSQL.Format(_T("select * from %s where 1=1"),m_strTableName);

	CString strTemp;

	if(ID)
	{
		strSQL.Format(_T(" and ID= %d"),ID);
	}
	if(SampleNo!=_T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and SampleNo like '%%%s%%'"),SampleNo);
		else
			strTemp.Format(_T(" and SampleNo = '%s'"),SampleNo);
		strSQL += strTemp;
	}
	if(Type != _T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and Type like '%%%s%%'"),Type);
		else		
			strTemp.Format(_T(" and Type = '%s'"),Type);
		strSQL += strTemp;
	}
	if(DilutionUp)
	{
		strTemp.Format(_T(" and Dilution <= %f "),DilutionUp);
		strSQL += strTemp;
	}
	if(DilutionLow)
	{
		strTemp.Format(_T(" and Dilution >=%f"),DilutionLow);
		strSQL += strTemp;
	}
	if(Dept!=_T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and Dept like '%%%s%%'"),Dept);	
		else
			strTemp.Format(_T(" and Dept = '%s'"),Dept);
		strSQL += strTemp;
	}
	if(Applicant!=_T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and Applicant like '%%%s%%'"),Applicant);
		else	
			strTemp.Format(_T(" and Applicant = '%s'"),Applicant);
		strSQL += strTemp;
	}
	if(ApplyDateUp)
	{
		COleDateTime dtTemp(ApplyDateUp); 
		strTemp.Format(_T(" and ApplyDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;
	}
	if(ApplyDateLow)
	{
		COleDateTime dtTemp(ApplyDateLow);
		strTemp.Format(_T(" and ApplyDate >=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}
	if(UpdateDateUp)
	{
		COleDateTime dtTemp(UpdateDateUp);
		strTemp.Format(_T(" and UpdateDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}
	if(UpdateDateLow)
	{
		COleDateTime dtTemp(UpdateDateLow);
		strTemp.Format(_T(" and UpdateDate =>#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}

	vector<UINT> keylist;
	_SelectRecordsFromDB(strSQL,keylist);
	for(int i = 0;i < (int)keylist.size(); i++)
	{
		samplelist.push_back(m_BufferList.find(keylist[i])->second);
	}
}

BOOL CSampleTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// ����ĳָ��keyֵ�ļ�¼�����ݿ�
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 sample_key = KeyList[i];
		// -----------------------------------------------
		//  �����ݿ��в�ѯ��keyֵ�ļ�¼
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM %s WHERE ID = %d"),m_strTableName, sample_key);
		if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
		{
			ASSERT(0);
			return FALSE;
		}

		// ------------------------------------------------
		//  �����ѯ������£����Ϊ��ѯ��������¼�¼
		// -----------------------------------------------
		size_t nCount = m_rs.GetRecordCount();

		if (nCount == 0)														// ���������û�д˼�¼�������
		{
			if (!m_rs.AddNew())
				break;
		}

		map<UINT,SampleRecord>::iterator iter = m_BufferList.find(sample_key);	// �ҵ���������Ӧ������
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}
		const SampleRecord& sample = iter->second;

		m_rs.SetFieldValue(_T("ID"), (long&)sample.ID);
		m_rs.SetFieldValue(_T("SampleNo"), sample.SampleNo);	
		m_rs.SetFieldValue(_T("TYPE"), sample.Type);
		m_rs.SetFieldValue(_T("Dilution"), sample.Dilution);
		m_rs.SetFieldValue(_T("Dept"), sample.Dept);
		m_rs.SetFieldValue(_T("Applicant"), sample.Applicant);
		m_rs.SetFieldValue(_T("ApplyDate"), sample.ApplyDate);
		m_rs.SetFieldValue(_T("UpdateDate"), sample.UpdateDate);

		if (!m_rs.Update())
		{
			m_rs.CancelUpdate();
		}
		m_rs.Close();
	}
	return TRUE;
}

// ********************************************************************************
// �������ܣ�
//     1.����ָ��sql�Ĳ�ѯ���ݿ�
//     2.�����������û�в�ѯ���ļ�¼������뻺����
//     3.������������Ѿ��в�ѯ���ļ�¼���򲻼��뻺����
//     4.�������в�ѯ���ļ�¼��keyֵ�������ٴ��ڻ������в���
// �������
//     1.strSQL:	���룬��ѯ��SQL
//     2.keyList:	�������ѯ���ļ�¼keyֵ�б�
// ********************************************************************************
BOOL CSampleTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// ���������������10000������Ҫ��������
		_ClearList();

	if (!m_rs.Open(strSQL,  CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		SampleRecord tempSampleBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempSampleBuffer.ID))
			break;

		map<UINT,SampleRecord>::iterator iter = m_BufferList.find(tempSampleBuffer.ID);
		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("SampleNo"), tempSampleBuffer.SampleNo);
			m_rs.GetFieldValue(_T("Type"), tempSampleBuffer.Type);
			m_rs.GetFieldValue(_T("Dilution"), tempSampleBuffer.Dilution);
			m_rs.GetFieldValue(_T("Dept"), tempSampleBuffer.Dept);
			m_rs.GetFieldValue(_T("Applicant"), tempSampleBuffer.Applicant);

			COleDateTime tempDate;
			m_rs.GetFieldValue(_T("ApplyDate"),tempDate);
			tempSampleBuffer.ApplyDate = tempDate;

			m_rs.GetFieldValue(_T("UpdateDate"),tempDate);
			tempSampleBuffer.UpdateDate = tempDate;

			m_BufferList.insert(map<UINT,SampleRecord>::value_type(tempSampleBuffer.ID, tempSampleBuffer));
		}
		keyList.push_back(tempSampleBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}
