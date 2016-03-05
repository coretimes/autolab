#include "StdAfx.h"
#include "DBConnect.h"
#include <shlwapi.h>

CDBConnect * CDBConnect::m_pInstance = NULL;

CDBConnect::CDBConnect(void)
{
	// CString strFilePath = AppPrefs.GetDataFilePath(); ȡ�����ݿ��ļ�·��
	CString strFilePath = _T("..\\bin\\data.mdb");		// ��ʱд��

	if (strFilePath.GetLength() == 0)
	{
		ASSERT(0);
		AfxMessageBox(_T("���ݿ��ļ�·��Ϊ�գ�"));
	}

	if (!PathFileExists(strFilePath))
	{
		ASSERT(0);
		AfxMessageBox(_T("���ݿ��ļ�·�������ڣ�"));
	}

	CString strConn;
	strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"), strFilePath);

	if(!m_db.Open(strConn, _T(""), _T(""), 3000))
	{
		AfxMessageBox(_T("���ݿ��ļ���ʧ�ܣ�"));
		ASSERT(0);
	}
}

CDBConnect::~CDBConnect(void)
{
	m_db.Close();
}
