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

BOOL CUserTable::_UpdateRecordsToDB(vector<UINT> KeyList)						// ����ĳָ��keyֵ�ļ�¼�����ݿ�
{
	for(int i=0; i < (int)KeyList.size(); i++)
	{
		uint32 User_key = KeyList[i];
		// -----------------------------------------------
		//  �����ݿ��в�ѯ��keyֵ�ļ�¼
		// -----------------------------------------------
		CString strCmd;
		strCmd.Format(_T("SELECT * FROM User WHERE ID = %d"), User_key);
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
		map<UINT,UserRecord>::iterator iter = m_BufferList.find(User_key);	// �ҵ���������Ӧ������
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
// �������ܣ�
// 1.����ָ��sql�Ĳ�ѯ���ݿ�
// 2.�����������û�в�ѯ���ļ�¼������뻺����
// 3.������������Ѿ��в�ѯ���ļ�¼���򲻼��뻺����
// 4.�������в�ѯ���ļ�¼��keyֵ�������ٴ��ڻ������в���
// �������
// 1.strSQL:	���룬��ѯ��SQL
// 2.keyList:	�������ѯ���ļ�¼keyֵ�б�
// ********************************************************************************
BOOL CUserTable::_SelectRecordsFromDB(CString & strSQL,vector<UINT> & keyList)
{
	if(m_BufferList.size() > 10000)												// ���������������10000������Ҫ��������
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
