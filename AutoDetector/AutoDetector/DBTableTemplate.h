//*****************************************************************************
// �ļ��� : DBTableTemplate.h
// 
// ��Ҫ   : ���ݿ����ģ��
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-7       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBConnect.h"
#include <map>
#include <vector>

using namespace std;

// *******************************************************
// ���ݿ���������ģ�嶨��
// *******************************************************
template <typename RecordInfo>
class CDBTableTemplate
{
public:
	CDBTableTemplate(void);
	~CDBTableTemplate(void);

	BOOL SaveToDB(void);												// �������ݱ�����DB
	BOOL AddNewRecord(RecordInfo& info);								// ���������в���������
	BOOL UpdateRecord(RecordInfo& info);								// ����ĳ����¼
	BOOL DeleteRecord(Table_Key key);										// ɾ��ָ��keyֵ�ļ�¼
	const RecordInfo& GetRecordInfo(Table_Key key);						// ��ȡָ��keyֵ�ļ�¼��Ϣ

protected:
	virtual BOOL _UpdateRecordsToDB(vector<Table_Key> KeyList) = 0;		// ����ĳЩkeyֵ�ļ�¼�����ݿ�

	virtual BOOL _SelectRecordsFromDB(CString & strSQL,vector<Table_Key> &KeyList) = 0;
																		// �����ݿ��ж�ȡ��¼

	BOOL _DeleteRecordsFromDB(vector<Table_Key> KeyList);					// �����ݿ���ɾ��ĳָ��keyֵ�ļ�¼
	Table_Key _SelectMaxIDFromDB();										// �����ݿ��в�ѯ���Keyֵ
	void _ClearList();													// ��������е�������µ�����

protected:
	map<Table_Key, RecordInfo> m_BufferList;							// ������,������Ҫ��дDB������
	map<Table_Key, RecordInfo> m_UpdateList;							// ������Ҫ�������ݿ�ļ�¼��keyֵ������
	map<Table_Key, RecordInfo> m_DeleteList;							// ������Ҫ�����ݿ���ɾ��������
	CString m_strTableName;												// ������
	Table_Key m_nMaxID;													// ���浱ǰDB������keyֵ
	CAdoRecordSet m_rs;
	RecordInfo m_NullRecord;											// ���ڶ������������������ͣ��˱�����ʾδ�鵽���ʽ���ġ������á�
};

// *******************************************************
// ���캯��
// *******************************************************
template <typename RecordInfo>
CDBTableTemplate<RecordInfo>::CDBTableTemplate(void)
{
	m_rs.Attach(&CDBConnect::GetInstance()->m_db);
}

// *******************************************************
// ��������
// *******************************************************
template <typename RecordInfo>
CDBTableTemplate<RecordInfo>::~CDBTableTemplate(void)
{
}

// *******************************************************
// ���滺������������DB
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::SaveToDB(void)							
{
	vector<Table_Key> keylist;

	// 1.ȡ��m_UpdateList���е�keyֵ
	map<Table_Key, RecordInfo> ::iterator iter = m_UpdateList.begin();
	while(iter!= m_UpdateList.end())
	{
		keylist.push_back(iter->first);
		iter++;
	}

	// 2.��������
	_UpdateRecordsToDB(keylist);

	// 3.ȡ��m_DeleteList���е�keyֵ
	keylist.clear();
	iter = m_DeleteList.begin();
	while(iter!= m_DeleteList.end())
	{
		keylist.push_back(iter->first);
		iter++;
	}

	// 4.ɾ������
	_DeleteRecordsFromDB(keylist);

	// 5.�������������ݱ�
	m_UpdateList.clear();
	m_DeleteList.clear();

	return TRUE;
}

// *******************************************************
// ����һ���¼�¼��������
// *******************************************************
template <typename RecordInfo>
BOOL CDBTableTemplate<RecordInfo>::AddNewRecord(RecordInfo& record)					
{
	if(m_BufferList.size() > 10000)												// ���������������10000������Ҫ��������
		_ClearList();

	m_nMaxID++;
	record.ID = m_nMaxID;
	m_BufferList.insert(map<Table_Key,RecordInfo>::value_type(record.ID, record));
	m_UpdateList.insert(map<Table_Key,RecordInfo>::value_type(record.ID,m_BufferList.find(record.ID)->second));
	return TRUE;
}

// *******************************************************
// ���»�����ĳ����¼
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
// ɾ��ĳ����¼
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
// ȡ�ñ����Keyֵ
// ����������
// Ҫ����key����Ϊ��ID��
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
// ȡ�ñ����Keyֵ
// ����������
// Ҫ����key����Ϊ��ID��
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
// ��������е�������µ�����
// 1.�����m_BufferList�����е�����
// 2.��m_UpdateList�����ݸ�ֵ��m_BufferList��
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
// ȡ�ü�¼��Ϣ
// *******************************************************
template <typename RecordInfo>
const RecordInfo& CDBTableTemplate<RecordInfo>::GetRecordInfo(Table_Key key)
{
	// ������������Ѿ��д����ݣ�ֱ�ӷ���
	map<Table_Key,RecordInfo>::iterator iter = m_BufferList.find(key);
	if(iter != m_BufferList.end())
		return iter->second;		

	// �����������û�д����ݣ��������ݿ�
	CString sql;
	sql.Format(_T("SELECT * FROM %s WHERE ID = %d"),m_strTableName,key);
	vector<Table_Key> list;
	_SelectRecordsFromDB(sql,list);
	
	iter = m_BufferList.find(key);
	if(iter != m_BufferList.end())
		return iter->second;		

	// �����Ȼû�鵽�����ؿռ�¼
	return m_NullRecord;
}