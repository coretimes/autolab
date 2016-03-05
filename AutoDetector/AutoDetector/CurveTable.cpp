#include "StdAfx.h"
#include "curveTable.h"

CCurveTable * CCurveTable::m_pInstance = NULL;
CCurveTable * CCurveTable::GetInstance(void)
{
	if(m_pInstance == NULL)
		m_pInstance = new CCurveTable();
	return m_pInstance;
}

CCurveTable::CCurveTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("Curve");
	m_nMaxID = _SelectMaxIDFromDB();
}

CCurveTable::~CCurveTable(void)
{
}

bool CCurveTable::GetCurves(
				  vector<CurveRecord> & CurveList,
				  const bool &IsFuzzyQuery,
				  const CString &strBatchNum,	
				  const CString &strProjectName,
				  const CString &strShortName,
				  const DATE &dtTestDateUpper, 
				  const DATE &dtTestDateLower,
				  const CString &strSurveyor)	
{
	CString strSQL;
	strSQL.Format(_T("select * from %s where 1=1"),m_strTableName);

	CString strTemp;
	CString strConnect;
	if(strBatchNum!=_T(""))
	{
		strConnect = _T(" and StandardID in ( select ID from Standard where 1=1");
		strSQL += strConnect;

		if(IsFuzzyQuery)
			strTemp.Format(_T(" and BatchNum like '%%%s%%'"),strBatchNum);
		else
			strTemp.Format(_T(" and BatchNum =  '%s'"),strBatchNum);

		strSQL += strTemp;
	}

	if(strProjectName != _T(""))
	{
		if(strConnect == _T(""))
		{
			strConnect = _T(" and StandardID in ( select ID from Standard where 1=1");
			strSQL += strConnect;
		}

		if(IsFuzzyQuery)
			strTemp.Format(_T(" and ProjectName like '%%%s%%'"),strProjectName);
		else
			strTemp.Format(_T(" and ProjectName = '%s'"),strProjectName);
		strSQL += strTemp;
	}
	if(strShortName != _T(""))
	{
		if(strConnect == _T(""))
		{
			strConnect = _T(" and StandardID in ( select ID from Standard where 1=1");
			strSQL += strConnect;
		}

		if(IsFuzzyQuery)
			strTemp.Format(_T(" and ShortName like '%%%s%%'"),strShortName);
		else
			strTemp.Format(_T(" and ShortName = '%s'"),strShortName);
		strSQL += strTemp;
	}

	if(strConnect != _T(""))
		strSQL += _T(" )");


	if(dtTestDateUpper)
	{
		COleDateTime dtTemp(dtTestDateUpper); 
		strTemp.Format(_T(" and TestDate <=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL += strTemp;
	}
	if(dtTestDateLower)
	{
		COleDateTime dtTemp(dtTestDateLower);
		strTemp.Format(_T(" and TestDate >=#%s#"),dtTemp.Format(_T("%Y-%m-%d")));
		strSQL+=strTemp;
	}

	vector<UINT> keylist;
	if(_SelectRecordsFromDB(strSQL,keylist) == false)
		return false;

	if(keylist.size() < 1)
		return false;

	for(int i = 0;i < (int)keylist.size(); i++)
	{
		CurveList.push_back(m_BufferList.find(keylist[i])->second);
	}
	return true;
}

BOOL CCurveTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// 更新某指定key值的记录至数据库
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 curve_key = KeyList[i];
		// -----------------------------------------------
		//  从数据库中查询此key值的记录
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM Curve WHERE ID = %d"), curve_key);
		if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
		{
			ASSERT(0);
			return FALSE;
		}

		// ------------------------------------------------
		//  如果查询到则更新，如果为查询到则插入新纪录
		// -----------------------------------------------
		size_t nCount = m_rs.GetRecordCount();
		if (nCount == 0)														// 如果数据中没有此记录，则添加
		{
			if (!m_rs.AddNew())
				break;
		}
		map<UINT,CurveRecord>::iterator iter = m_BufferList.find(curve_key);	// 找到缓存区对应的数据
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}
		const CurveRecord& curve = iter->second;

		m_rs.SetFieldValue(_T("ID"), (long&)curve.ID);
		m_rs.SetFieldValue(_T("StandardID"), (long&)curve.StandardID);	
		m_rs.SetFieldValue(_T("ReagentID"), (long&)curve.ReagentID);
		m_rs.SetFieldValue(_T("DefaultFitType"), (long&)curve.DefaultFitType);
		m_rs.SetFieldValue(_T("Std1"), curve.RluString[0]);
		m_rs.SetFieldValue(_T("Std2"), curve.RluString[1]);
		m_rs.SetFieldValue(_T("Std3"), curve.RluString[2]);
		m_rs.SetFieldValue(_T("Std4"), curve.RluString[3]);
		m_rs.SetFieldValue(_T("Std5"), curve.RluString[4]);
		m_rs.SetFieldValue(_T("Std6"), curve.RluString[5]);
		m_rs.SetFieldValue(_T("Std7"), curve.RluString[6]);
		m_rs.SetFieldValue(_T("Std8"), curve.RluString[7]);
		m_rs.SetFieldValue(_T("Std9"), curve.RluString[8]);
		m_rs.SetFieldValue(_T("Std10"), curve.RluString[9]);
		m_rs.SetFieldValue(_T("Surveyor"), curve.Surveyor);
		m_rs.SetFieldValue(_T("TestDate"), curve.TestDate);

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
BOOL CCurveTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// 如果数据条数大于10000，则需要清理缓冲区
		_ClearList();

	if (!m_rs.Open(strSQL, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		CurveRecord tempRecordBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempRecordBuffer.ID))
			break;

		map<UINT,CurveRecord>::iterator iter = m_BufferList.find(tempRecordBuffer.ID);

		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("StandardID"), (long&)tempRecordBuffer.StandardID);
			m_rs.GetFieldValue(_T("ReagentID"), (long&)tempRecordBuffer.ReagentID);
			m_rs.GetFieldValue(_T("DefaultFitType"), (long&)tempRecordBuffer.DefaultFitType);
			m_rs.GetFieldValue(_T("Std1"), tempRecordBuffer.RluString[0]);
			m_rs.GetFieldValue(_T("Std2"), tempRecordBuffer.RluString[1]);
			m_rs.GetFieldValue(_T("Std3"), tempRecordBuffer.RluString[2]);
			m_rs.GetFieldValue(_T("Std4"), tempRecordBuffer.RluString[3]);
			m_rs.GetFieldValue(_T("Std5"), tempRecordBuffer.RluString[4]);
			m_rs.GetFieldValue(_T("Std6"), tempRecordBuffer.RluString[5]);
			m_rs.GetFieldValue(_T("Std7"), tempRecordBuffer.RluString[6]);
			m_rs.GetFieldValue(_T("Std8"), tempRecordBuffer.RluString[7]);
			m_rs.GetFieldValue(_T("Std9"), tempRecordBuffer.RluString[8]);
			m_rs.GetFieldValue(_T("Std10"), tempRecordBuffer.RluString[9]);
			m_rs.GetFieldValue(_T("Surveyor"), tempRecordBuffer.Surveyor);

			COleDateTime tempDate;
			m_rs.GetFieldValue(_T("TestDate"),tempDate);
			tempRecordBuffer.TestDate = tempDate;

			m_BufferList.insert(map<UINT,CurveRecord>::value_type(tempRecordBuffer.ID, tempRecordBuffer));
		}

		keyList.push_back(tempRecordBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}
