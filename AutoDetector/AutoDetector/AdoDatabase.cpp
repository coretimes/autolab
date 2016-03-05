#include "StdAfx.h"
#include "AdoDatabase.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAdoDatabase::CAdoDatabase(void)
{
	m_pConnection = NULL;
	m_strConnection = _T("");
	m_pConnection.CreateInstance(__uuidof(Connection));
	m_nConnectionTimeout = 0;
}

CAdoDatabase::~CAdoDatabase(void)
{
	if (IsOpen())
	{
		ASSERT(0);		// did you forget to close it?
		Close();
	}

	m_pConnection.Release();
	m_pConnection = NULL;
	m_strConnection = _T("");

	//CoUninitialize();
}

BOOL CAdoDatabase::IsOpen(void)
{
	if (m_pConnection)
		return m_pConnection->GetState() != adStateClosed;
	return FALSE;
}

BOOL CAdoDatabase::Open(LPCTSTR lpszConnection /*= _T("")*/, LPCTSTR lpszUserID /*= _T("")*/, LPCTSTR lpszPassword /*= _T("")*/, long nTimeout /*= 0*/)
{
	if (IsOpen())
	{
		ASSERT(0);	// reopen
		Close();
	}

	if (wcslen(lpszConnection) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	m_strConnection = lpszConnection;

	try
	{
		HRESULT hr;

		if (nTimeout != 0)
		{
			m_nConnectionTimeout = nTimeout;
			m_pConnection->PutConnectionTimeout(m_nConnectionTimeout);
		}

		hr = m_pConnection->Open((_bstr_t)m_strConnection, lpszUserID, lpszPassword, NULL);
		return hr == S_OK;
	}
	catch (_com_error &e)
	{
		DumpError(e);
		ASSERT(0);
		return FALSE;
	}
}

void CAdoDatabase::Close(void)
{
	if (IsOpen())
		m_pConnection->Close();

	TRACE(_T("Success: Database('%s') closed successfully.\n"), m_strConnection);
}

void CAdoDatabase::DumpError(_com_error &e)
{
	CStringA ErrorStr;

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	ErrorStr.Format("CAdoDataBase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);

	TRACE(ErrorStr);
	ASSERT(0);
	// 注：如果发生此错误，检查程序类::InitInstance()中是否缺少AfxOleInit()函数的调用
}


CAdoRecordSet::CAdoRecordSet(void)
{
	m_pRecordset = NULL;
	m_pConnection = NULL;

	m_pRecordset.CreateInstance(__uuidof(Recordset));
}

CAdoRecordSet::CAdoRecordSet(CAdoDatabase* pAdoDatabase)
{
	ASSERT(pAdoDatabase && pAdoDatabase->IsOpen());

	m_pRecordset = NULL;
	m_pConnection = NULL;

	m_pRecordset.CreateInstance(__uuidof(Recordset));
	
	m_pConnection = pAdoDatabase->GetActiveConnection();
	ASSERT(m_pConnection);
}

CAdoRecordSet::~CAdoRecordSet(void)
{
	if (IsOpen())
	{
		ASSERT(0);		// did you forget to close it?
		Close();
	}

	if (m_pRecordset)
	{
		m_pRecordset.Release();
		m_pRecordset = NULL;
	}

	m_pConnection = NULL;
}

BOOL CAdoRecordSet::PutFieldValue(LPCTSTR lpszFieldName, _variant_t vtFld)
{
	if (wcslen(lpszFieldName) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	try
	{
		m_pRecordset->Fields->GetItem(lpszFieldName)->Value = vtFld;
		return TRUE;
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
}

BOOL CAdoRecordSet::PutFieldValue(_variant_t vtIndex, _variant_t vtFld)
{
	if (vtIndex.iVal < 0 || (size_t)vtIndex.iVal >= GetFieldCount())
	{
		ASSERT(0);		// out of bounds
		return FALSE;
	}

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
		return TRUE;
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
}

void CAdoRecordSet::DumpError(_com_error &e)
{
	CStringA ErrorStr;

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	ErrorStr.Format("CAdoRecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);

	TRACE(ErrorStr);

	ASSERT(0);
}

BOOL CAdoRecordSet::Open(_ConnectionPtr pDBConn, LPCTSTR lpszExec, OpenType opt)
{
	if (IsOpen())
	{
		ASSERT(0);
		Close();
	}

	if (wcslen(lpszExec) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	m_pConnection = pDBConn;
	
	CString strCmd = lpszExec;
	strCmd = strCmd.Trim();

	BOOL bIsSelect = strCmd.Left((int)wcslen(_T("SELECT "))).CompareNoCase(_T("SELECT ")) == 0;

	try
	{
		m_pRecordset->CursorType = adOpenStatic;
		m_pRecordset->CursorLocation = adUseClient;
		
		if (bIsSelect || opt == otQuery)
		{
			m_pRecordset->Open((LPCTSTR)strCmd, _variant_t((IDispatch*)pDBConn, TRUE), 
							adOpenStatic, adLockOptimistic, adCmdUnknown);
		}
		else if (opt = otTable)
		{
			m_pRecordset->Open((LPCTSTR)strCmd, _variant_t((IDispatch*)pDBConn, TRUE), 
							adOpenKeyset, adLockOptimistic, adCmdTable);
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return m_pRecordset != NULL;
}

BOOL CAdoRecordSet::Open(LPCTSTR lpszExec, OpenType opt)
{
	if (m_pConnection == NULL || m_pConnection->GetState() == adStateClosed)
	{
		ASSERT(0);		// you must call function Attach() to attach this recordset to an CAdoDatabase object first!
		return FALSE;
	}

	return Open(m_pConnection, lpszExec, opt);
}

void CAdoRecordSet::Close(void)
{
	if (IsOpen())
	{
		//CancelUpdate();
		m_pRecordset->Close();
	}
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, int nValue)
{
	_variant_t vtFld;

	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, int nValue)
{
	_variant_t vtFld;

	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, unsigned long ulValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_UI4;
	vtFld.ulVal = ulValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, unsigned long ulValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_UI4;
	vtFld.ulVal = ulValue;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, LPCTSTR lpszValue)
{
	_variant_t vtFld;
	_variant_t vtIndex;	

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	CString strValue = lpszValue;

	if(!strValue.IsEmpty())
		vtFld.vt = VT_BSTR;
	else
		vtFld.vt = VT_NULL;

	vtFld.bstrVal = strValue.AllocSysString();

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, LPCTSTR lpszValue)
{
	_variant_t vtFld;

	CString strValue = lpszValue;

	if(!strValue.IsEmpty())
		vtFld.vt = VT_BSTR;
	else
		vtFld.vt = VT_NULL;

	vtFld.bstrVal = strValue.AllocSysString();

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, bool bValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.boolVal = bValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, bool bValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.boolVal = bValue;

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, COleCurrency cyValue)
{
	if(cyValue.m_status == COleCurrency::invalid)
		return FALSE;

	_variant_t vtFld;

	vtFld.vt = VT_CY;
	vtFld.cyVal = cyValue.m_cur;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, COleCurrency cyValue)
{
	if(cyValue.m_status == COleCurrency::invalid)
		return FALSE;

	_variant_t vtFld;

	vtFld.vt = VT_CY;
	vtFld.cyVal = cyValue.m_cur;	

	return PutFieldValue(lpszFieldName, vtFld);
}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, _variant_t vtValue)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtValue);
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpszFieldName, _variant_t vtValue)
{
	return PutFieldValue(lpszFieldName, vtValue);
}

BOOL CAdoRecordSet::SetFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	vtFld.vt = VT_EMPTY;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}

BOOL CAdoRecordSet::SetFieldEmpty(LPCTSTR lpszFieldName)
{
	_variant_t vtFld;
	vtFld.vt = VT_EMPTY;

	return PutFieldValue(lpszFieldName, vtFld);
}

void CAdoRecordSet::CancelUpdate()
{
	m_pRecordset->CancelUpdate();
}

BOOL CAdoRecordSet::Update()
{
	BOOL bRlt = FALSE;

	try
	{
		if (m_pRecordset->Update() == S_OK)
			bRlt = TRUE;
		else
			ASSERT(0);
	}
	catch (_com_error &e)
	{
		DumpError(e);
		ASSERT(0);
	}

	if (bRlt == FALSE)
		m_pRecordset->CancelUpdate();

	return bRlt;
}


BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, _variant_t& vtValue)
{
	if (wcslen(lpszFieldName) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	int val = 0;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpszFieldName)->Value;
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	vtValue = vtFld;
	return TRUE;
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, _variant_t& vtValue)
{
	if (nIndex < 0 || nIndex >= m_pRecordset->Fields->GetCount())
	{
		ASSERT(0);
		return FALSE;
	}

	int val = 0;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(nIndex)->Value;
	}
	catch (_com_error &e)
	{
		ASSERT(0);
		DumpError(e);
		return FALSE;
	}

	vtValue = vtFld;
	return TRUE;
}


BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, int& nValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, nValue);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, int& nValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, nValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, long& lValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, (sint32&)lValue);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, long& lValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, (sint32&)lValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, unsigned long& ulValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, (uint32&)ulValue);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, unsigned long& ulValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, (uint32&)ulValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, double& dbValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, dbValue);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, double& dbValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, dbValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, COleDateTime& time)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, time);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, COleDateTime& time)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, time);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, bool& bValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, bValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, bool& bValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, bValue);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, LPTSTR lpszValue, size_t size)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, lpszValue, size);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, LPTSTR lpszValue, size_t size)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, lpszValue, size);
}

BOOL CAdoRecordSet::GetFieldValue(int nIndex, CString& strValue)
{
	_variant_t vtFld;
	GetFieldValue(nIndex, vtFld);
	return FormatVariant(vtFld, strValue);
}

BOOL CAdoRecordSet::GetFieldValue(LPCTSTR lpszFieldName, CString& strValue)
{
	_variant_t vtFld;
	GetFieldValue(lpszFieldName, vtFld);
	return FormatVariant(vtFld, strValue);
}

BOOL CAdoRecordSet::AddNew(void)
{
	HRESULT hr;

	try
	{
		hr = m_pRecordset->AddNew();
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	if (hr != S_OK)
		return FALSE;

	return TRUE;
}

BOOL CAdoRecordSet::Delete(void)
{
	HRESULT hr;

	try
	{
		hr = m_pRecordset->Delete(adAffectCurrent);
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	if (hr != S_OK)
		return FALSE;

	return TRUE;
}

BOOL  CAdoRecordSet::MoveFirst(void)
{
	if (!IsOpen())
	{
		ASSERT(0);
		return FALSE;
	}

	HRESULT hr;

	try
	{
		hr = m_pRecordset->MoveFirst();
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return hr == S_OK;
}

BOOL CAdoRecordSet::MoveNext(void)
{
	if (!IsOpen())
	{
		ASSERT(0);
		return FALSE;
	}

	HRESULT hr;

	try
	{
		hr = m_pRecordset->MoveNext();
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return hr == S_OK;
}

BOOL CAdoRecordSet::MovePrevious(void)
{
	if (!IsOpen())
	{
		ASSERT(0);
		return FALSE;
	}

	HRESULT hr;

	try
	{
		hr = m_pRecordset->MovePrevious();
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return hr == S_OK;
}

BOOL CAdoRecordSet::MoveLast(void)
{
	if (!IsOpen())
	{
		ASSERT(0);
		return FALSE;
	}

	HRESULT hr;

	try
	{
		hr = m_pRecordset->MoveLast();
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return hr == S_OK;
}


BOOL CAdoRecordSet::FormatVariant(_variant_t var, LPTSTR lpszBuf, size_t size)
{
	if (lpszBuf == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	if (var.vt == VT_NULL)
		return TRUE;

	size_t len = 0;

#ifdef UNICODE
	len = wcslen((LPCWSTR)(_bstr_t)var) + 1;
#else
	len = strlen((LPCSTR)(_bstr_t)var) + 1;
#endif // UNICODE

	if (size < len)
	{
		ASSERT(0);		// buffer is tooooo small!
		return FALSE;
	}

	BOOL bRlt = TRUE;
	DWORD dwLen = 0;

	switch (var.vt)
	{
		// string
	case VT_BSTR:
	case VT_LPSTR:
	case VT_LPWSTR:
	case VT_R8:
		{
#ifdef UNICODE
			_tcscpy_s(lpszBuf, size, (LPCWSTR)(_bstr_t)var);
#else
			strcpy_s(lpszBuf, size, (LPCSTR)(_bstr_t)var);
#endif // UNICODE
		}
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, CString& s)
{
	if (var.vt == VT_NULL)
		return TRUE;

	BOOL bRlt = TRUE;

	size_t len = 0;

#ifdef UNICODE
	len = wcslen((LPCWSTR)(_bstr_t)var) + 1;
#else
	len = strlen((LPCSTR)(_bstr_t)var) + 1;
#endif // UNICODE

	CString strValue;
	if (!FormatVariant(var, strValue.GetBufferSetLength((int)len), len))
	{
		bRlt = FALSE;
	}
	else
	{
		bRlt = TRUE;
		s = strValue;
	}

	strValue.ReleaseBuffer();

	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, sint32& n)
{
	BOOL bRlt = TRUE;
	sint32 nValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// short int
	case VT_I2:
		nValue = var.iVal;
		break;

		// int
	case VT_INT:
	case VT_I4:
		nValue = var.intVal;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	n = nValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, uint32& n)
{
	BOOL bRlt = TRUE;
	uint32 nValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// short unsigned int
	case VT_UI2:
		nValue = var.uiVal;
		break;

		// unsigned int
	case VT_UINT:
	case VT_UI4:
		nValue = var.uintVal;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	n = nValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, sint64& n)
{
	BOOL bRlt = TRUE;
	sint64 nValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// long long
	case VT_I8:
		nValue = var.llVal;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	n = nValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, uint64& n)
{
	BOOL bRlt = TRUE;
	uint64 nValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// unsigned long long
	case VT_UI8:
		nValue = var.ullVal;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	n = nValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, float& f)
{
	BOOL bRlt = TRUE;
	float fValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// float
	case VT_R4:
		fValue = var.fltVal;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	f = fValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, double& dbl)
{
	BOOL bRlt = TRUE;
	double dblValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// double
	case VT_R8:
		dblValue = var.dblVal;
		break;

	case VT_DECIMAL:
		{
			double val = (double)var.decVal.Lo64;
			val *= (var.decVal.sign == 128)? -1 : 1;
			val /= pow(10, (double)var.decVal.scale); 
			dblValue = val;
		}
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	dbl = dblValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, bool& b)
{
	BOOL bRlt = TRUE;
	bool bValue = 0;

	switch(var.vt)
	{
	case VT_NULL:
		break;

	case VT_BOOL:
		bValue = var.lVal ? true : false;
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	b = bValue;
	return bRlt;
}

BOOL CAdoRecordSet::FormatVariant(_variant_t var, COleDateTime& d)
{
	BOOL bRlt = TRUE;
	DATE dt;
	COleDateTime dValue;

	switch(var.vt)
	{
	case VT_NULL:
		break;

		// date
	case VT_DATE:
		dt = var.date;
		dValue = COleDateTime(dt);
		break;

	default:
		ASSERT(0);
		bRlt = FALSE;
		break;
	}

	d = dValue;
	return bRlt;
}
