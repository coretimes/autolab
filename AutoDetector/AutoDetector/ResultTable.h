//*****************************************************************************
// 文件名 : ResultTable.h
// 
// 概要   : 样本结果表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

struct ResultRecord
{
	uint32 ID;					// key
	uint32 SampleID;			// 样本ID
	uint32 ReagentID;			// 试剂ID
	uint32 CurveID;				// 标准曲线ID
	CString RluString;			// 发光值字符串
	double TheoryConc;			// 理论浓度
	double FitConc;				// 拟合浓度
	CString Unit;				// 浓度单位
	CString Surveyor;			// 检验员
	DATE TestDate;				// 检验日期
};

// *******************************************************
// 结果表类
// *******************************************************
class CResultTable : public CDBTableTemplate<ResultRecord>
{
private:
	CResultTable(void);

public:
	static CResultTable * GetInstance() 											// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CResultTable();
		return m_pInstance;
	}
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

	~CResultTable(void);

	void GetResultsBySampleID(uint32 ReagentID,vector<ResultRecord>& resultlist);	// 获取指定样本ID的所有结果信息
	void GetResultsByReagentID(uint32 ReagentID,vector<ResultRecord>& resultlist);	// 获取指定结果ID的所有结果信息
	void GetResultsByDate(DATE date,vector<ResultRecord>& resultlist);				// 获取指定检验日期的所有结果信息

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// 更新某指定key值的记录至数据库
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																					// 从数据库中读取记录
private:
	static CResultTable * m_pInstance;
};
