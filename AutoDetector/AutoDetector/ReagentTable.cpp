#include "StdAfx.h"
#include "ReagentTable.h"

CReagentTable * CReagentTable::m_pInstance = NULL;

CReagentTable::CReagentTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("Reagent");
	m_nMaxID = _SelectMaxIDFromDB();
}

CReagentTable::~CReagentTable(void)
{
}

void CReagentTable::GetDistinctProjectNames(vector<ProjectFullName>& NameList)
{
	CString strSQL;

	// Ϊ��ʹ��_SelectRecordsFromDB������sql��д��������ʽ������ʹ��distinct
	strSQL.Format(_T("select * from %s where ID in ( select Max(ID) from %s group by ProjectName) "),m_strTableName,m_strTableName);

	vector<UINT> keylist;
	_SelectRecordsFromDB(strSQL,keylist);
	for(int i = 0;i < (int)keylist.size(); i++)
	{
		ReagentRecord & record = m_BufferList.find(keylist[i])->second;
		
		// �������Ƹ�ʽΪ����Ŀ���ƣ���Ŀ��ƣ���
		ProjectFullName duName;
		duName.ProjectName = record.ProjectName;
		duName.ShortName = record.ShortName;
		NameList.push_back(duName);
	}

	// ����Ŀ�������
	sort(NameList.begin(),NameList.end(),_LessByShortName);
}

BOOL CReagentTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// ����ĳָ��keyֵ�ļ�¼�����ݿ�
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 reagent_key = KeyList[i];
		// -----------------------------------------------
		//  �����ݿ��в�ѯ��keyֵ�ļ�¼
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM Reagent WHERE ID = %d"), reagent_key);
		if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
		{
			ASSERT(0);
			return FALSE;
		}

		// ------------------------------------------------
		//  �����ѯ������£����Ϊ��ѯ��������¼�¼
		// -----------------------------------------------
		size_t nCount = m_rs.GetRecordCount();
		if (nCount == 0)															// ���������û�д˼�¼�������
		{
			if (!m_rs.AddNew())
				break;
		}
		map<UINT,ReagentRecord>::iterator iter = m_BufferList.find(reagent_key);	// �ҵ���������Ӧ������
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}

		const ReagentRecord& reagent = iter->second;
		m_rs.SetFieldValue(_T("ID"), (long&)reagent.ID);	
		m_rs.SetFieldValue(_T("BatchNum"), reagent.BatchNum);
		m_rs.SetFieldValue(_T("ProjectName"), reagent.ProjectName);
		m_rs.SetFieldValue(_T("ShortName"), reagent.ShortName);
		m_rs.SetFieldValue(_T("ProductionDate"), reagent.ProductionDate);
		m_rs.SetFieldValue(_T("ExpirationDate"), reagent.ExpirationDate);
		m_rs.SetFieldValue(_T("RatedTestTimes"), reagent.RatedTestTimes);
		m_rs.SetFieldValue(_T("RemainTimes"), reagent.RemainTimes);
		m_rs.SetFieldValue(_T("FirstLoadTime"), reagent.FirstLoadTime);
		m_rs.SetFieldValue(_T("FirstUseTime"), reagent.FirstUseTime);
		m_rs.SetFieldValue(_T("Unit"), reagent.Unit);
		m_rs.SetFieldValue(_T("UpperLimit"), reagent.UpperLimit);
		m_rs.SetFieldValue(_T("LowerLimit"), reagent.LowerLimit);
		m_rs.SetFieldValue(_T("SampleAmount"), reagent.SampleAmount);
		m_rs.SetFieldValue(_T("MagneticAmount"), reagent.MagneticAmount);
		m_rs.SetFieldValue(_T("HRPAmount"), reagent.HRPAmount);
		m_rs.SetFieldValue(_T("Dilution"), reagent.Dilution);
		m_rs.SetFieldValue(_T("WashTimes"), reagent.WashTimes);
		m_rs.SetFieldValue(_T("IncubationTime"), reagent.IncubationTime);
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
BOOL CReagentTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// ���������������10000������Ҫ��������
		_ClearList();

	if (!m_rs.Open(strSQL, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		ReagentRecord tempRecordBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempRecordBuffer.ID))
			break;

		map<UINT,ReagentRecord>::iterator iter = m_BufferList.find(tempRecordBuffer.ID);

		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("BatchNum"), tempRecordBuffer.BatchNum);
			m_rs.GetFieldValue(_T("ProjectName"), tempRecordBuffer.ProjectName);
			m_rs.GetFieldValue(_T("ShortName"), tempRecordBuffer.ShortName);

			COleDateTime tempDate;
			m_rs.GetFieldValue(_T("ProductionDate"),tempDate);
			tempRecordBuffer.ProductionDate = tempDate;

			m_rs.GetFieldValue(_T("ExpirationDate"),tempDate);
			tempRecordBuffer.ExpirationDate = tempDate;

			m_rs.GetFieldValue(_T("RatedTestTimes"), tempRecordBuffer.RatedTestTimes);
			m_rs.GetFieldValue(_T("RemainTimes"), tempRecordBuffer.RemainTimes);

			m_rs.GetFieldValue(_T("FirstLoadTime"),tempDate);
			tempRecordBuffer.FirstLoadTime = tempDate;

			m_rs.GetFieldValue(_T("FirstUseTime"), tempDate);
			tempRecordBuffer.FirstUseTime = tempDate;

			m_rs.GetFieldValue(_T("Unit"), tempRecordBuffer.Unit);
			m_rs.GetFieldValue(_T("UpperLimit"), tempRecordBuffer.UpperLimit);
			m_rs.GetFieldValue(_T("LowerLimit"), tempRecordBuffer.LowerLimit);
			m_rs.GetFieldValue(_T("SampleAmount"), tempRecordBuffer.SampleAmount);
			m_rs.GetFieldValue(_T("MagneticAmount"), tempRecordBuffer.MagneticAmount);
			m_rs.GetFieldValue(_T("HRPAmount"), tempRecordBuffer.HRPAmount);
			m_rs.GetFieldValue(_T("Dilution"), tempRecordBuffer.Dilution);
			m_rs.GetFieldValue(_T("WashTimes"), tempRecordBuffer.WashTimes);
			m_rs.GetFieldValue(_T("IncubationTime"), tempRecordBuffer.IncubationTime);

			m_BufferList.insert(map<UINT,ReagentRecord>::value_type(tempRecordBuffer.ID, tempRecordBuffer));
		}
		keyList.push_back(tempRecordBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}

bool CReagentTable::GetReagents(vector<ReagentRecord> & resultlist,
								const bool &IsFuzzyQuery,
								const uint32 &ID,					          
								const CString &strBatchNum,			         
								const CString &strProjectName,			            
								const CString &strShortName,                  
								const DATE &ProductionDateUpper,
								const DATE &ProductionDateLower,
								const DATE &ExpirationDateUpper,
								const DATE &ExpirationDateLower,
								const DATE   &FirstLoadTime,
								const DATE   &FirstUseTime,
								const double &UpperLimit, 
								const double &LowerLimit

								)
{
	CString strSQL;
	strSQL.Format(_T("select * from %s where 1=1"),m_strTableName);

	CString strTemp;

	if(ID)
	{
		strSQL.Format(_T(" and ID= %d"),ID);
	}

	if(strBatchNum!=_T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and BatchNum like '%%%s%%'"),strBatchNum);
		else
			strTemp.Format(_T(" and BatchNum = '%s'"),strBatchNum);
		strSQL += strTemp;
	}
	if(strProjectName != _T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and ProjectName like '%%%s%%'"),strProjectName);
		else		
			strTemp.Format(_T(" and ProjectName = '%s'"),strProjectName);
		strSQL += strTemp;
	}
	if(strShortName != _T(""))
	{
		if(IsFuzzyQuery)
			strTemp.Format(_T(" and ShortName like '%%%s%%'"),strShortName);
		else		
			strTemp.Format(_T(" and ShortName = '%s'"),strShortName);
		strSQL += strTemp;
	}
	if(ProductionDateUpper)
	{
		COleDateTime dtTemp(ProductionDateUpper); 
		strTemp.Format(_T(" and ProductionDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;
	}
	if(ProductionDateLower)
	{
		COleDateTime dtTemp(ProductionDateLower); 
		strTemp.Format(_T(" and ProductionDate >=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;;
	}
	if(ExpirationDateUpper)
	{
		COleDateTime dtTemp(ExpirationDateUpper); 
		strTemp.Format(_T(" and ExpirationDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;
	}
	if(ExpirationDateLower)
	{
		COleDateTime dtTemp(ExpirationDateLower); 
		strTemp.Format(_T(" and ExpirationDate >=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;;
	}
	if(FirstLoadTime)
	{
		COleDateTime dtTemp(FirstLoadTime); 
		strTemp.Format(_T(" and FirstLoadTime =#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;;
	}
	if(FirstUseTime)
	{
		COleDateTime dtTemp(FirstUseTime); 
		strTemp.Format(_T(" and FirstUseTime =#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;;
	}
	if(UpperLimit)
	{		
		strTemp.Format(_T(" and UpperLimit = %f"),UpperLimit);
		strSQL += strTemp;
	}
	if(LowerLimit)
	{
		strTemp.Format(_T(" and LowerLimit =%f"),LowerLimit);
		strSQL+=strTemp;
	}

	vector<UINT> keylist;
	if(_SelectRecordsFromDB(strSQL,keylist) == FALSE)
		return false;
	if(keylist.size() < 1)
		return false;

	for(int i = 0;i < (int)keylist.size(); i++)
	{
		resultlist.push_back(m_BufferList.find(keylist[i])->second);
	}
	return true;
}

