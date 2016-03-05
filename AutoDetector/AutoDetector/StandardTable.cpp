#include "StdAfx.h"
#include "StandardTable.h"

CStandardTable * CStandardTable::m_pInstance = NULL;

CStandardTable::CStandardTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("Standard");
	m_nMaxID = _SelectMaxIDFromDB();
}

CStandardTable::~CStandardTable(void)
{
}

bool CStandardTable::GetStandards(
									vector<StandardRecord> & StandardList,
									const bool &IsFuzzyQuery,
									const uint32 &ID,
									const CString &strBatchNum,	
									const CString &strProjectName,
									const CString &strShortName,
									const DATE &dtProductionDateUpper, 
									const DATE &dtProductionDateLower,
									const DATE &dtExpirationDateUpper,
									const DATE &dtExpirationDateLower
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
			strTemp.Format(_T(" and BatchNum =  '%s'"),strBatchNum);

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

	if(dtProductionDateUpper)
	{
		COleDateTime dtTemp(dtProductionDateUpper); 
		strTemp.Format(_T(" and ProductionDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;
	}
	if(dtProductionDateLower)
	{
		COleDateTime dtTemp(dtProductionDateLower);
		strTemp.Format(_T(" and ProductionDate >=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}
	if(dtExpirationDateUpper)
	{
		COleDateTime dtTemp(dtExpirationDateUpper);
		strTemp.Format(_T(" and ExpirationDateUpper <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}
	if(dtExpirationDateLower)
	{
		COleDateTime dtTemp(dtExpirationDateLower);
		strTemp.Format(_T(" and ExpirationDateLower =>#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}

	vector<UINT> keylist;
	if(_SelectRecordsFromDB(strSQL,keylist) == false)
		return false;

	if(keylist.size() < 1)
		return false;
	
	for(int i = 0;i < (int)keylist.size(); i++)
	{
		StandardList.push_back(m_BufferList.find(keylist[i])->second);
	}
	return true;
}

BOOL CStandardTable::_UpdateRecordsToDB(vector<UINT> KeyList)							// 更新某指定key值的记录至数据库
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 Standard_key = KeyList[i];
		// -----------------------------------------------
		//  从数据库中查询此key值的记录
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM %s WHERE ID = %d"),m_strTableName, Standard_key);
		if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
		{
			ASSERT(0);
			return FALSE;
		}

		// ------------------------------------------------
		//  如果查询到则更新，如果为查询到则插入新纪录
		// -----------------------------------------------
		size_t nCount = m_rs.GetRecordCount();
		if (nCount == 0)																// 如果数据中没有此记录，则添加
		{
			if (!m_rs.AddNew())
				break;
		}
		map<UINT,StandardRecord>::iterator iter = m_BufferList.find(Standard_key);	// 找到缓存区对应的数据
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}
		const StandardRecord& Standard = iter->second;

		m_rs.SetFieldValue(_T("ID"), (long&)Standard.ID);
		m_rs.SetFieldValue(_T("BatchNum"), Standard.BatchNum);	
		m_rs.SetFieldValue(_T("ProjectName"), Standard.ProjectName);
		m_rs.SetFieldValue(_T("ShortName"), Standard.ShortName);
		m_rs.SetFieldValue(_T("ProductionDate"), Standard.ProductionDate);
		m_rs.SetFieldValue(_T("ExpirationDate"), Standard.ExpirationDate);
		m_rs.SetFieldValue(_T("RatedTestTimes"), Standard.RatedTestTimes);
		m_rs.SetFieldValue(_T("RemainTimes"), Standard.RemainTimes);
		m_rs.SetFieldValue(_T("FirstLoadTime"), Standard.FirstLoadTime);
		m_rs.SetFieldValue(_T("FirstUseTime"), Standard.FirstUseTime);
		m_rs.SetFieldValue(_T("CountOfStdPoints"), (long&)Standard.CountOfStdPoints);
		m_rs.SetFieldValue(_T("Conc1"), Standard.StdConc[0]);
		m_rs.SetFieldValue(_T("Conc2"), Standard.StdConc[1]);
		m_rs.SetFieldValue(_T("Conc3"), Standard.StdConc[2]);
		m_rs.SetFieldValue(_T("Conc4"), Standard.StdConc[3]);
		m_rs.SetFieldValue(_T("Conc5"), Standard.StdConc[4]);
		m_rs.SetFieldValue(_T("Conc6"), Standard.StdConc[5]);
		m_rs.SetFieldValue(_T("Conc7"), Standard.StdConc[6]);
		m_rs.SetFieldValue(_T("Conc8"), Standard.StdConc[7]);
		m_rs.SetFieldValue(_T("Conc9"), Standard.StdConc[8]);
		m_rs.SetFieldValue(_T("Conc10"),Standard.StdConc[9]);
		if (!m_rs.Update())
		{
			m_rs.CancelUpdate();
		}
		m_rs.Close();

	}
	return TRUE;
}

// ********************************************************************************
// 函数介绍：
// 1.根据指定sql文查询数据库
// 2.如果缓冲区内没有查询到的记录，则加入缓冲区
// 3.如果缓冲区内已经有查询到的记录，则不加入缓冲区
// 4.保存所有查询到的记录的key值，便于再此在缓冲区中查找
// 参数类表：
// 1.strSQL:	输入，查询用SQL
// 2.keyList:	输出，查询到的记录key值列表
// ********************************************************************************
BOOL CStandardTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// 如果数据条数大于10000，则需要清理缓冲区
		_ClearList();

	if (!m_rs.Open(strSQL, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		StandardRecord tempRecordBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempRecordBuffer.ID))
			break;

		map<UINT,StandardRecord>::iterator iter = m_BufferList.find(tempRecordBuffer.ID);

		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("BatchNum"), tempRecordBuffer.BatchNum);
			m_rs.GetFieldValue(_T("ProjectName"), tempRecordBuffer.ProjectName);
			m_rs.GetFieldValue(_T("ShortName"), tempRecordBuffer.ShortName);
			m_rs.GetFieldValue(_T("Unit"), tempRecordBuffer.Unit);

			COleDateTime tempDate;
			m_rs.GetFieldValue(_T("ProductionDate"), tempDate);
			tempRecordBuffer.ProductionDate = tempDate;

			m_rs.GetFieldValue(_T("ExpirationDate"), tempDate);
			tempRecordBuffer.ExpirationDate = tempDate;

			m_rs.GetFieldValue(_T("RatedTestTimes"), tempRecordBuffer.RatedTestTimes);
			m_rs.GetFieldValue(_T("RemainTimes"), tempRecordBuffer.RemainTimes);

			m_rs.GetFieldValue(_T("FirstLoadTime"), tempDate);
			tempRecordBuffer.FirstLoadTime = tempDate;

			m_rs.GetFieldValue(_T("FirstUseTime"), tempDate);
			tempRecordBuffer.FirstUseTime = tempDate;

			m_rs.GetFieldValue(_T("CountOfStdPoints"), tempRecordBuffer.CountOfStdPoints);
			m_rs.GetFieldValue(_T("Conc1"), tempRecordBuffer.StdConc[0]);
			m_rs.GetFieldValue(_T("Conc2"), tempRecordBuffer.StdConc[1]);
			m_rs.GetFieldValue(_T("Conc3"), tempRecordBuffer.StdConc[2]);
			m_rs.GetFieldValue(_T("Conc4"), tempRecordBuffer.StdConc[3]);
			m_rs.GetFieldValue(_T("Conc5"), tempRecordBuffer.StdConc[4]);
			m_rs.GetFieldValue(_T("Conc6"), tempRecordBuffer.StdConc[5]);
			m_rs.GetFieldValue(_T("Conc7"), tempRecordBuffer.StdConc[6]);
			m_rs.GetFieldValue(_T("Conc8"), tempRecordBuffer.StdConc[7]);
			m_rs.GetFieldValue(_T("Conc9"), tempRecordBuffer.StdConc[8]);
			m_rs.GetFieldValue(_T("Conc10"), tempRecordBuffer.StdConc[9]);
			m_BufferList.insert(map<UINT,StandardRecord>::value_type(tempRecordBuffer.ID, tempRecordBuffer));
		}

		keyList.push_back(tempRecordBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}

void CStandardTable::GetDistinctProjectNames(vector<ProjectFullName>& NameList)
{
	CString strSQL;

	// 为了使用_SelectRecordsFromDB函数，sql文写成如下形式，而不使用distinct
	strSQL.Format(_T("select *					\
					 from %s					\
					 where ID in				\
						( select Max(ID)		\
						  from %s				\
						  group by ProjectName  \
						 )						\
					 order by ShortName "		\
					 ),m_strTableName,m_strTableName);

	vector<UINT> keylist;
	_SelectRecordsFromDB(strSQL,keylist);
	for(int i = 0;i < (int)keylist.size(); i++)
	{
		StandardRecord & record = m_BufferList.find(keylist[i])->second;

		// 返回名称格式为“项目名称（项目简称）”
		ProjectFullName duName;
		duName.ProjectName = record.ProjectName;
		duName.ShortName = record.ShortName;
		NameList.push_back(duName);
	}
}

void CStandardTable::GetDistinctBatchNums(vector<CString>& BatchNumsList)
{
	CString strSQL;

	// 为了使用_SelectRecordsFromDB函数，sql文写成如下形式，而不使用distinct
	strSQL.Format(_T("select * \
					 from %s \
					 where ID in \
						( select Max(ID) \
						from %s \
						group by BatchNum \
						) \
					order by ShortName") \
					,m_strTableName,m_strTableName);

	vector<UINT> keylist;
	_SelectRecordsFromDB(strSQL,keylist);

	for(int i = 0;i < (int)keylist.size(); i++)
	{
		StandardRecord & record = m_BufferList.find(keylist[i])->second;
		BatchNumsList.push_back(record.BatchNum);
	}
}