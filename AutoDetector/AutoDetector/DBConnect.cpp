#include "StdAfx.h"
#include "DBConnect.h"
#include <shlwapi.h>

CDBConnect * CDBConnect::m_pInstance = NULL;

CDBConnect::CDBConnect(void)
{
	// CString strFilePath = AppPrefs.GetDataFilePath(); 取得数据库文件路径
	CString strFilePath = _T("..\\bin\\data.mdb");		// 临时写死

	if (strFilePath.GetLength() == 0)
	{
		ASSERT(0);
		AfxMessageBox(_T("数据库文件路径为空！"));
	}

	if (!PathFileExists(strFilePath))
	{
		ASSERT(0);
		AfxMessageBox(_T("数据库文件路径不存在！"));
	}

	CString strConn;
	strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"), strFilePath);

	if(!m_db.Open(strConn, _T(""), _T(""), 3000))
	{
		AfxMessageBox(_T("数据库文件打开失败！"));
		ASSERT(0);
	}
}

CDBConnect::~CDBConnect(void)
{
	m_db.Close();
}
