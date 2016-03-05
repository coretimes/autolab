//*****************************************************************************
// �ļ��� : UserTable.h
// 
// ��Ҫ   : �û�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct UserRecord
{
	uint32 ID;					// key
	CString Name;				// ����ֵ�ַ���
	CString Password;			// ����
	CString Alias;				// �û�����
	uint32 Type;					// �û�����
};

// *******************************************************
// �û�����
// *******************************************************
class CUserTable : public CDBTableTemplate<UserRecord>
{
private:
	CUserTable(void);

public:
	static CUserTable * GetInstance();											// ȡ��ʵ��ָ��

	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CUserTable(void);

	void GetUsersByDate(DATE date,vector<UserRecord>& Userlist);				// ��ȡָ���������ڵ����н����Ϣ

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);						// ����ĳָ��keyֵ�ļ�¼�����ݿ�
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																				// �����ݿ��ж�ȡ��¼
private:
	static CUserTable * m_pInstance;
};
