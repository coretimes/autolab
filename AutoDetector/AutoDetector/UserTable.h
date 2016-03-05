//*****************************************************************************
// 文件名 : UserTable.h
// 
// 概要   : 用户表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct UserRecord
{
	uint32 ID;					// key
	CString Name;				// 发光值字符串
	CString Password;			// 密码
	CString Alias;				// 用户姓名
	uint32 Type;					// 用户类型
};

// *******************************************************
// 用户表类
// *******************************************************
class CUserTable : public CDBTableTemplate<UserRecord>
{
private:
	CUserTable(void);

public:
	static CUserTable * GetInstance();											// 取得实体指针

	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CUserTable(void);

	void GetUsersByDate(DATE date,vector<UserRecord>& Userlist);				// 获取指定检验日期的所有结果信息

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);						// 更新某指定key值的记录至数据库
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																				// 从数据库中读取记录
private:
	static CUserTable * m_pInstance;
};
