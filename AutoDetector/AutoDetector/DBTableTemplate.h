//*****************************************************************************
// 文件名 : DBTableTemplate.h
// 
// 概要   : 数据库表类模板
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-7       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBConnect.h"
#include <map>
#include <vector>

using namespace std;

// *******************************************************
// 数据库表缓存管理类模板定义
// *******************************************************
template <typename RecordInfo>
class CDBTableTemplate
{
public:
	CDBTableTemplate(void);
	~CDBTableTemplate(void);

	BOOL SaveToDB(void);												// 所有数据保存至DB
	BOOL AddNewRecord(RecordInfo& info);								// 往样本表中插入新数据
	BOOL UpdateRecord(RecordInfo& info);								// 更新某条记录
	BOOL DeleteRecord(Table_Key key);										// 删除指定key值的记录
	const RecordInfo& GetRecordInfo(Table_Key key);						// 获取指定key值的记录信息

protected:
	virtual BOOL _UpdateRecordsToDB(vector<Table_Key> KeyList) = 0;		// 更新某些key值的记录至数据库

	virtual BOOL _SelectRecordsFromDB(CString & strSQL,vector<Table_Key> &KeyList) = 0;
																		// 从数据库中读取记录

	BOOL _DeleteRecordsFromDB(vector<Table_Key> KeyList);					// 从数据库中删除某指定key值的记录
	Table_Key _SelectMaxIDFromDB();										// 从数据库中查询最大Key值
	void _ClearList();													// 清除缓冲中的无须更新的数据

protected:
	map<Table_Key, RecordInfo> m_BufferList;							// 样本表,缓存需要读写DB的数据
	map<Table_Key, RecordInfo> m_UpdateList;							// 保存需要更新数据库的记录的key值和引用
	map<Table_Key, RecordInfo> m_DeleteList;							// 保存需要在数据库中删除的数据
	CString m_strTableName;												// 表名字
	Table_Key m_nMaxID;													// 保存当前DB中最大的key值
	CAdoRecordSet m_rs;
	RecordInfo m_NullRecord;											// 由于多数函数返回引用类型，此变量表示未查到合适结果的“空引用”
};

// *******************************************************
// 构造函数
// *******************************************************
template <typename RecordInfo>
CDBTableTemplate<RecordInfo>::CDBTableTemplate(void)
{
	m_rs.Attach(&CDBConnect::GetInstance()->m_db);
}

// *******************************************************
// 析构函数
// *******************************************************
template <typename RecordInfo>
CDBTableTemplate<RecordInfo>::~CDBTableTemplate(void)
{
}

// *******************************************************
// 保存缓冲区的数据至DB
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::SaveToDB(void)							
{
	vector<Table_Key> keylist;

	// 1.取得m_UpdateList表中的key值
	map<Table_Key, RecordInfo> ::iterator iter = m_UpdateList.begin();
	while(iter!= m_UpdateList.end())
	{
		keylist.push_back(iter->first);
		iter++;
	}

	// 2.更新数据
	_UpdateRecordsToDB(keylist);

	// 3.取得m_DeleteList表中的key值
	keylist.clear();
	iter = m_DeleteList.begin();
	while(iter!= m_DeleteList.end())
	{
		keylist.push_back(iter->first);
		iter++;
	}

	// 4.删除数据
	_DeleteRecordsFromDB(keylist);

	// 5.清除待保存的数据表
	m_UpdateList.clear();
	m_DeleteList.clear();

	return TRUE;
}

// *******************************************************
// 增加一条新记录到缓冲区
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::AddNewRecord(RecordInfo& record)					
{
	if(m_BufferList.size() > 10000)												// 如果数据条数大于10000，则需要清理缓冲区
		_ClearList();

	m_nMaxID++;
	record.ID = m_nMaxID;
	m_BufferList.insert(map<Table_Key,RecordInfo>::value_type(record.ID, record));
	m_UpdateList.insert(map<Table_Key,RecordInfo>::value_type(record.ID,m_BufferList.find(record.ID)->second));
	return TRUE;
}

// *******************************************************
// 更新缓冲区某条记录
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::UpdateRecord(RecordInfo& record)			
{
	map<Table_Key,RecordInfo>::iterator iter = m_BufferList.find(record.ID);
	if(iter != m_BufferList.end())
	{
		m_BufferList.erase(iter);
	}
	m_BufferList.insert(map<Table_Key,RecordInfo>::value_type(record.ID,record));

	map<Table_Key,RecordInfo>::iterator iter2 = m_UpdateList.find(record.ID);
	if(iter2 != m_UpdateList.end())
	{
		m_UpdateList.erase(iter2);
	}
	iter = m_BufferList.find(record.ID);
	m_UpdateList.insert(map<Table_Key,RecordInfo>::value_type(record.ID,iter->second));

	return TRUE;
}

// *******************************************************
// 删除某条记录
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::DeleteRecord(Table_Key record_id)				
{
	map<Table_Key,RecordInfo>::iterator iter = m_BufferList.find(record_id);
	if(iter != m_BufferList.end())
	{
		m_DeleteList.insert(map<Table_Key,RecordInfo>::value_type(record_id,iter->second));
		m_BufferList.erase(iter);
	}

	iter = m_UpdateList.find(record_id);
	if(iter != m_UpdateList.end())
	{
		m_UpdateList.erase(iter);
	}

	return TRUE;
}

// *******************************************************
// 取得表最大Key值
// 参数：表名
// 要求表的key名均为“ID”
// *******************************************************
template <typename RecordInfo>
Table_Key CDBTableTemplate<RecordInfo>::_SelectMaxIDFromDB()				
{
	CString strCmd;
	strCmd.Format(_T("SELECT MAX(ID) AS MaxID FROM %s"),m_strTableName);

	if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
	{
		return FALSE;
	}

	Table_Key maxkey;
	if (!m_rs.GetFieldValue(_T("MaxID"), (long&)maxkey))
		ASSERT(0);

	m_rs.Close();
	return maxkey;
}

// *******************************************************
// 取得表最大Key值
// 参数：表名
// 要求表的key名均为“ID”
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::_DeleteRecordsFromDB(vector<Table_Key> KeyList)				
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		Table_Key record_id = KeyList[i];

		CString strCmd;
		strCmd.Format(_T("SELECT * FROM %s WHERE ID = %d"),m_strTableName, record_id);

		if (!m_rs.Open(strCmd, CAdoRecordSet::otQuery))
		{
			ASSERT(0);
			return FALSE;
		}
		if(m_rs.GetRecordCount())
			m_rs.Delete();
		m_rs.Close();
	}

	return TRUE;
}

// *******************************************************
// 清除缓冲中的无须更新的数据
// 1.先清除m_BufferList中所有的数据
// 2.将m_UpdateList的数据赋值到m_BufferList中
// *******************************************************
template <typename RecordInfo>
void CDBTableTemplate<RecordInfo>::_ClearList()
{
	m_BufferList.clear();
	map<Table_Key,RecordInfo>::iterator iter = m_UpdateList.begin();
	while(iter != m_UpdateList.end())
	{
		m_UpdateList.insert(map<Table_Key,RecordInfo>::value_type(iter->first,iter->second));
		iter++;
	}
}

// *******************************************************
// 取得记录信息
// *******************************************************
template <typename RecordInfo>
const RecordInfo& CDBTableTemplate<RecordInfo>::GetRecordInfo(Table_Key key)
{
	// 如果缓冲区中已经有此数据，直接返回
	map<Table_Key,RecordInfo>::iterator iter = m_BufferList.find(key);
	if(iter != m_BufferList.end())
		return iter->second;		

	// 如果缓冲区中没有此数据，查找数据库
	CString sql;
	sql.Format(_T("SELECT * FROM %s WHERE ID = %d"),m_strTableName,key);
	vector<Table_Key> list;
	_SelectRecordsFromDB(sql,list);
	
	iter = m_BufferList.find(key);
	if(iter != m_BufferList.end())
		return iter->second;		

	// 如果仍然没查到，返回空记录
	return m_NullRecord;
}