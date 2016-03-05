//*****************************************************************************
// 文件名 : ReagentTable.h
// 
// 概要   : 试剂表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "DBTableTemplate.h"
#include <algorithm>
#include "Types.h"

struct ReagentRecord
{
	Reagent_Key ID;				// key
	CString BatchNum;			// 批号
	CString ProjectName;		// 项目名称
	CString ShortName;			// 项目简称
	DATE ProductionDate;		// 生产日期
	DATE ExpirationDate;		// 失效日期
	int RatedTestTimes;			// 额定实验次数
	int RemainTimes;			// 剩余次数
	DATE FirstLoadTime;			// 首次装载时间
	DATE FirstUseTime;			// 首次使用时间
	CString Unit;				// 测量单位
	double UpperLimit;			// 结果上限
	double LowerLimit;			// 结果下限
	double SampleAmount;		// 样本用量
	double MagneticAmount;		// 磁标用量
	double HRPAmount;			// 酶标用量
	double Dilution;			// 稀释倍数
	double WashTimes;			// 清洗次数
	double IncubationTime;		// 温育时间
};

// *******************************************************
// 试剂表类
// *******************************************************
class CReagentTable : public CDBTableTemplate<ReagentRecord>
{
private:
	CReagentTable(void);

public:
	static CReagentTable * GetInstance() 											// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CReagentTable();
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

	~CReagentTable(void);

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     根据指定条件查询结果信息
	// 参数：
	//      1.ReagentList - 返回试剂列表
	// 		2 IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	//		3.ID - 指定key
	//		4.BatchNum - 批号
	//		5.ProjectName - 项目名称
	//		6.ShortName - 项目简称
	//		7.ProductionDateUpper - 生产日期上限
	//      8.ProductionDateLower - 生产日期下限
	//		9.ExpirationDateUpper - 失效日期上限
	//      10.ExpirationDateLower - 失效日期下限
	//      11.FirstLoadTime - 首次装载时间
	//      12.FirstUseTime  -首次使用时间
	//      13.UpperLimit   - 结果上限
	//      14.LowerLimit  - 结果下限
	// ---------------------------------------------------------------------------
	bool GetReagents(
		vector<ReagentRecord> & ReagentList,
		const bool &IsFuzzyQuery = true,
		const Reagent_Key &ID = 0,					          
		const CString &strBatchNum = _T(""),			         
		const CString &strProjectName =_T(""),			            
		const CString &strShortName = _T(""),                  
		const DATE &ProductionDateUpper = 0,
		const DATE &ProductionDateLower = 0,
		const DATE &ExpirationDateUpper =0,
		const DATE &ExpirationDateLower =0,
		const DATE   &FirstLoadTime=0,
		const DATE   &FirstUseTime =0,
		const double &UpperLimit = 0,			
		const double &LowerLimit = 0			
		);

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     获取指定检验日期的所有结果信息
	// 参数：
	// 		1 date - 指定的日期
	//      2.resultlist - 返回结果列表
	// ---------------------------------------------------------------------------
	bool GetReagentsByDate(const DATE &date,vector<ReagentRecord>& resultlist)
	{
		if( GetReagents(/*resultlist=*/          resultlist,
			/*IsFuzzyQuery=*/                         false,
			/*ID=*/                                       0,
			/*strBatchNum=*/                         _T(""),
			/*strProjectName=*/                      _T(""),
			/*strShortName=*/                        _T(""),
			/*ProductionDateUpper=*/                   date,
			/*ProductionDateLower=*/                   date,
			/*ExpirationDateUpper=*/                      0,
			/*ExpirationDateLower=*/                      0,
			/*FirstLoadTime=*/                            0,
			/*FirstUseTime*/                              0,
			/*UpperLimit*/                                0,
			/*LowerLimit*/                                0
			) == true)
			return true;
		else
			return false;
	}

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   获取指定项目名称的所有试剂结果信息
	// 参数：
	//      1.resultlist - 返回结果列表
	// 		2.strProjectName - 指定的项目名称
	// 		3.IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	// ---------------------------------------------------------------------------
	bool GetReagentsByProjectName(vector<ReagentRecord>& resultlist,const CString &strProjectName,const bool &IsFuzzyQuery = false)
	{
		if( GetReagents(/*resultlist=*/          resultlist,
			/*IsFuzzyQuery=*/                  IsFuzzyQuery,
			/*ID=*/                                       0,
			/*strBatchNum=*/                         _T(""),
			/*strProjectName=*/              strProjectName,
			/*strShortName=*/                        _T(""),
			/*ProductionDateUpper=*/                      0,
			/*ProductionDateLower=*/                      0,
			/*ExpirationDateUpper=*/                      0,
			/*ExpirationDateLower=*/                      0,
			/*FirstLoadTime=*/                            0,
			/*FirstUseTime*/                              0,
			/*UpperLimit*/                                0,
			/*LowerLimit*/                                0
			) == true)
			return true;
		else
			return false;
	}

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   取得各种不同“项目名称”
	// 参数：
	//      1.NameList - 返回项目名称的存储列表
	// ---------------------------------------------------------------------------
	void GetDistinctProjectNames(vector<ProjectFullName>& NameList);					

	static void SortByBatchNumAsc(vector<ReagentRecord>& resultlist)				// 按批号升序排序
	{ sort(resultlist.begin(),resultlist.end(),_LessByBatchNum);}

private:
	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   更新某指定key值的记录至数据库
	// 参数：
	//      1.KeyList - 指定的key值列表
	// ---------------------------------------------------------------------------
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);

	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   根据key 判断 V1 > V2 是否成立
	// ---------------------------------------------------------------------------
	static bool _LessByBatchNum(ReagentRecord & v1,ReagentRecord & v2)	
	{ return v1.BatchNum < v2.BatchNum; }

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   根据key 判断 V1 > V2 是否成立
	// ---------------------------------------------------------------------------
	static bool _LessByShortName(ProjectFullName & v1,ProjectFullName & v2)	
	{ return v1.ShortName < v2.ShortName; }

private:
	static CReagentTable * m_pInstance;
};
