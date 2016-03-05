#include "StdAfx.h"
#include "ResultTable.h"

CResultTable * CResultTable::m_pInstance = NULL;

CResultTable::CResultTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("Result");
	m_nMaxID = _SelectMaxIDFromDB();
}

CResultTable::~CResultTable(void)
{
}

BOOL CResultTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// ����ĳָ��keyֵ�ļ�¼�����ݿ�
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 result_key = KeyList[i];
		// -----------------------------------------------
		//  �����ݿ��в�ѯ��keyֵ�ļ�¼
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM Result WHERE ID = %d"), result_key);
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
		map<UINT,ResultRecord>::iterator iter = m_BufferList.find(result_key);	// �ҵ���������Ӧ������
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}
		const ResultRecord& result = iter->second;

		m_rs.SetFieldValue(_T("ID"), (long&)result.ID);
		m_rs.SetFieldValue(_T("SampleID"), (long&)result.SampleID);	
		m_rs.SetFieldValue(_T("ReagentID"), (long&)result.ReagentID);
		m_rs.SetFieldValue(_T("CurveID"), (long&)result.CurveID);
		m_rs.SetFieldValue(_T("RluString"), result.RluString);
		m_rs.SetFieldValue(_T("TheoryConc"), result.TheoryConc);
		m_rs.SetFieldValue(_T("FitConc"), result.FitConc);
		m_rs.SetFieldValue(_T("Unit"), result.Unit);
		m_rs.SetFieldValue(_T("Surveyor"), result.Surveyor);
		m_rs.SetFieldValue(_T("TestDate"), result.TestDate);

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
// 1.����ָ��sql�Ĳ�ѯ���ݿ�
// 2.�����������û�в�ѯ���ļ�¼������뻺����
// 3.������������Ѿ��в�ѯ���ļ�¼���򲻼��뻺����
// 4.�������в�ѯ���ļ�¼��keyֵ�������ٴ��ڻ������в���
// �������
// 1.strSQL:	���룬��ѯ��SQL
// 2.keyList:	�������ѯ���ļ�¼keyֵ�б�
// ********************************************************************************
BOOL CResultTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// ���������������10000������Ҫ��������
		_ClearList();

	if (!m_rs.Open(strSQL, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		ResultRecord tempRecordBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempRecordBuffer.ID))
			break;

		map<UINT,ResultRecord>::iterator iter = m_BufferList.find(tempRecordBuffer.ID);

		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("SampleID"), (long&)tempRecordBuffer.SampleID);
			m_rs.GetFieldValue(_T("ReagentID"), (long&)tempRecordBuffer.ReagentID);
			m_rs.GetFieldValue(_T("CurveID"), (long&)tempRecordBuffer.CurveID);
			m_rs.GetFieldValue(_T("RluString"), tempRecordBuffer.RluString);
			m_rs.GetFieldValue(_T("TheoryConc"), tempRecordBuffer.TheoryConc);
			m_rs.GetFieldValue(_T("FitConc"), tempRecordBuffer.FitConc);
			m_rs.GetFieldValue(_T("Unit"), tempRecordBuffer.Unit);
			m_rs.GetFieldValue(_T("Surveyor"), tempRecordBuffer.Surveyor);

			COleDateTime tempDate;
			m_rs.GetFieldValue(_T("TestDate"),tempDate);
			tempRecordBuffer.TestDate = tempDate;

			m_BufferList.insert(map<UINT,ResultRecord>::value_type(tempRecordBuffer.ID, tempRecordBuffer));
		}
		keyList.push_back(tempRecordBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}
