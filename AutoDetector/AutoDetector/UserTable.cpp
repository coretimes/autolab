#include "StdAfx.h"
#include "UserTable.h"

CUserTable * CUserTable::m_pInstance = NULL;
CUserTable * CUserTable::GetInstance(void)
{
	if(m_pInstance == NULL)
		m_pInstance = new CUserTable();
	return m_pInstance;
}

CUserTable::CUserTable(void)
{
	CAdoRecordSet m_rs(&CDBConnect::GetInstance()->m_db);
	m_strTableName = _T("User");
	m_nMaxID = _SelectMaxIDFromDB();
}

CUserTable::~CUserTable(void)
{
}

BOOL CUserTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// 更新某指定key值的记录至数据库
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 User_key = KeyList[i];
		// -----------------------------------------------
		//  从数据库中查询此key值的记录
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM User WHERE ID = %d"), User_key);
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
		map<UINT,UserRecord>::iterator iter = m_BufferList.find(User_key);	// 找到缓存区对应的数据
		if(iter == m_BufferList.end())
		{
			ASSERT(0);
			break;
		}
		const UserRecord& User = iter->second;

		m_rs.SetFieldValue(_T("ID"), (long&)User.ID);
		m_rs.SetFieldValue(_T("Name"), User.Name);	
		m_rs.SetFieldValue(_T("Password"), User.Password);
		m_rs.SetFieldValue(_T("Alias"), User.Alias);
		m_rs.SetFieldValue(_T("Type"), (long&)User.Type);

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
BOOL CUserTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// 如果数据条数大于10000，则需要清理缓冲区
		_ClearList();

	if (!m_rs.Open(strSQL, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	while(!m_rs.IsEOF())
	{
		UserRecord tempRecordBuffer;

		if (!m_rs.GetFieldValue(_T("ID"), (long&)tempRecordBuffer.ID))
			break;

		map<UINT,UserRecord>::iterator iter = m_BufferList.find(tempRecordBuffer.ID);

		if(iter == m_BufferList.end())				
		{
			m_rs.GetFieldValue(_T("Name"), tempRecordBuffer.Name);
			m_rs.GetFieldValue(_T("Password"), tempRecordBuffer.Password);
			m_rs.GetFieldValue(_T("Alias"), tempRecordBuffer.Alias);
			m_rs.GetFieldValue(_T("Type"), (long&)tempRecordBuffer.Type);
			m_BufferList.insert(map<UINT,UserRecord>::value_type(tempRecordBuffer.ID, tempRecordBuffer));
		}
		keyList.push_back(tempRecordBuffer.ID);
		m_rs.MoveNext();
	}

	m_rs.Close();
	return TRUE;
}
