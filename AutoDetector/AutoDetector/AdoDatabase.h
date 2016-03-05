//*****************************************************************************
// 文件名 : AdoDatabase.h
// 
// 概要   : Ado 数据库访问类 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-7       崔志雷       新建（源于已有代码）
// ------------------------------------------
//*****************************************************************************

#pragma once

// ADO support
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include "Types.h"

class CAdoDatabase
{
public:
	CAdoDatabase(void);
public:
	virtual ~CAdoDatabase(void);

protected:
	// connection
	_ConnectionPtr m_pConnection;
	CString m_strConnection;
	long m_nConnectionTimeout;

protected:
	void DumpError(_com_error &e);

public:
	BOOL Open(LPCTSTR lpszConnection = _T(""), LPCTSTR lpszUserID = _T(""), LPCTSTR lpszPassword = _T(""), long nTimeout = 0);
	BOOL IsOpen(void);
	void Close(void);

	void SetConnectionTimeout(long nConnectionTimeout = 30)		{m_nConnectionTimeout = nConnectionTimeout;}
	_ConnectionPtr GetActiveConnection(void)					{return m_pConnection;}
};

class CAdoRecordSet
{
public:
	CAdoRecordSet(void);
	CAdoRecordSet(CAdoDatabase* pAdoDatabase);
public:
	virtual ~CAdoRecordSet(void);

public:
	_RecordsetPtr m_pRecordset;

protected:
	_ConnectionPtr m_pConnection;


protected:
	void DumpError(_com_error &e);

	BOOL PutFieldValue(LPCTSTR lpszFieldName, _variant_t vtFld);
	BOOL PutFieldValue(_variant_t vtIndex, _variant_t vtFld);

public:
	//------------------------------------------------------------------------
	// connection
	//------------------------------------------------------------------------
	enum OpenType {
		otQuery,
		otTable
	};
	
	BOOL Open(_ConnectionPtr pDBConn, LPCTSTR lpszExec, OpenType opt);
	BOOL Open(LPCTSTR lpszExec, OpenType opt);
	void Attach(CAdoDatabase* pDB)
	{
		if (pDB->IsOpen())
			m_pConnection = pDB->GetActiveConnection();
		else
			ASSERT(0);
	}
	void Detach(void)
	{
		m_pConnection = NULL;
	}

	BOOL IsConnectionOpen(void)
	{
		return m_pConnection != NULL && 
			m_pConnection->GetState() != adStateClosed;
	}

	BOOL IsOpen(void)
	{
		if (m_pRecordset != NULL && IsConnectionOpen())
			return m_pRecordset->GetState() != adStateClosed;
		return FALSE;
	}

	void Close(void);
	
	//------------------------------------------------------------------------
	// cursor moving
	//------------------------------------------------------------------------
	BOOL IsEOF(void)						{return m_pRecordset->adoEOF == VARIANT_TRUE;}		
	BOOL IsBOF(void)						{return m_pRecordset->BOF == VARIANT_TRUE;}

	BOOL MoveFirst(void);
	BOOL MoveNext(void);
	BOOL MovePrevious(void);
	BOOL MoveLast(void);

	//------------------------------------------------------------------------
	// data update
	//------------------------------------------------------------------------
	void CancelUpdate(void);
	BOOL Update(void);

	//------------------------------------------------------------------------
	// add & delete
	//------------------------------------------------------------------------
	BOOL AddNew(void);
	BOOL Delete(void);

	//------------------------------------------------------------------------
	// get & set
	//------------------------------------------------------------------------
	size_t GetRecordCount(void)				{return (size_t)m_pRecordset->GetRecordCount();}
	size_t GetFieldCount(void)				{return (size_t)m_pRecordset->Fields->GetCount();}


	BOOL GetFieldValue(int nIndex, _variant_t& vtValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, _variant_t& vtValue);

	BOOL GetFieldValue(int nIndex, int& nValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, int& nValue);
	
	BOOL GetFieldValue(int nIndex, long& lValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, long& lValue);
	
	BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, unsigned long& ulValue);
	
	BOOL GetFieldValue(int nIndex, double& dbValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, double& dbValue);
	
	BOOL GetFieldValue(int nIndex, CString& strValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, CString& strValue);
	
	BOOL GetFieldValue(int nIndex, LPTSTR lpszValue, size_t size);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, LPTSTR lpszValue, size_t size);

	BOOL GetFieldValue(int nIndex, COleDateTime& time);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, COleDateTime& time);
	
	BOOL GetFieldValue(int nIndex, bool& bValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, bool& bValue);
	
	BOOL GetFieldValue(int nIndex, COleCurrency& cyValue);
	BOOL GetFieldValue(LPCTSTR lpszFieldName, COleCurrency& cyValue);


	BOOL SetFieldValue(int nIndex, int nValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, int nValue);
	
	BOOL SetFieldValue(int nIndex, long lValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, long lValue);
	
	BOOL SetFieldValue(int nIndex, unsigned long ulValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, unsigned long ulValue);
	
	BOOL SetFieldValue(int nIndex, double dblValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, double dblValue);
	
	BOOL SetFieldValue(int nIndex, LPCTSTR lpszValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, LPCTSTR lpszValue);

	BOOL SetFieldValue(int nIndex, COleDateTime time);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, COleDateTime time);
	
	BOOL SetFieldValue(int nIndex, bool bValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, bool bValue);
	
	BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, COleCurrency cyValue);
	
	BOOL SetFieldValue(int nIndex, _variant_t vtValue);
	BOOL SetFieldValue(LPCTSTR lpszFieldName, _variant_t vtValue);


	BOOL SetFieldEmpty(int nIndex);
	BOOL SetFieldEmpty(LPCTSTR lpszFieldName);

private:
	BOOL FormatVariant(_variant_t var, LPTSTR lpszBuf, size_t size);
	BOOL FormatVariant(_variant_t var, CString& s);
	BOOL FormatVariant(_variant_t var, sint32& n);
	BOOL FormatVariant(_variant_t var, uint32& n);
	BOOL FormatVariant(_variant_t var, sint64& n);
	BOOL FormatVariant(_variant_t var, uint64& n);
	BOOL FormatVariant(_variant_t var, float& f);
	BOOL FormatVariant(_variant_t var, double& dbl);
	BOOL FormatVariant(_variant_t var, bool& b);
	BOOL FormatVariant(_variant_t var, COleDateTime& d);
};

