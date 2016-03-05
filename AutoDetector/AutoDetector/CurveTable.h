//*****************************************************************************
// 文件名 : CurveTable.h
// 
// 概要   : 曲线表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct CurveRecord
{
	Curve_Key ID;				// key
	Standard_Key StandardID;	// 标准品ID
	Reagent_Key ReagentID;		// 试剂ID
	CurveType DefaultFitType;	// 默认拟合模型
	CString RluString[10];		// 发光值字符串
	CString Surveyor;			// 检验员
	DATE TestDate;				// 定标日期
};

// *******************************************************
// 曲线数据库表类
// *******************************************************
class CCurveTable : public CDBTableTemplate<CurveRecord>
{
private:
	CCurveTable(void);

public:
	static CCurveTable * GetInstance();												// 取得实体指针

	// --------------------------------------------------
	// 摘要：
	//	   删除实体
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CCurveTable(void);

	void GetResultsByDate(DATE date,vector<CurveRecord>& resultlist);				// 获取指定检验日期的所有结果信息

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     根据指定条件查询曲线信息
	// 参数：
	//      1.CurveList - 返回标准品存储列表
	// 		2 IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	//		3.strBatchNum - 批号
	//		4.strProjectName - 项目名称
	//		5.strShortName - 项目简称
	//		6.dtTestDateUpper - 定标日期上限
	//		7.dtTestDateLower - 定标日期下限
	//		8.strSurveyor - 检验员
	// ---------------------------------------------------------------------------
	bool GetCurves(
		vector<CurveRecord> & CurveList,
		const bool &IsFuzzyQuery = true,
		const CString &strBatchNum = _T(""),	
		const CString &strProjectName = _T(""),
		const CString &strShortName = _T(""),
		const DATE &dtTestDateUpper = 0, 
		const DATE &dtTestDateLower = 0,
		const CString &strSurveyor = _T(""));

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// 更新某指定key值的记录至数据库
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																					// 从数据库中读取记录
private:
	static CCurveTable * m_pInstance;
};
