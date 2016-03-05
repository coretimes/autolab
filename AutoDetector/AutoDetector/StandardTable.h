//*****************************************************************************
// 文件名 : StandardTable.h
// 
// 概要   : 标准品表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

// 宏定义：标准点浓度取值
struct StandardRecord
{
	Sample_Key ID;				// key
	CString BatchNum;			// 批号
	CString ProjectName;		// 项目名称
	CString ShortName;			// 项目简称
	DATE ProductionDate;		// 生产日期
	DATE ExpirationDate;		// 失效日期
	int RatedTestTimes;			// 额定实验次数
	int RemainTimes;			// 剩余次数
	DATE FirstLoadTime;			// 首次装载时间
	DATE FirstUseTime;			// 首次使用时间
	CString Unit;				// 浓度单位
	int CountOfStdPoints;		// 标准点个数
	double StdConc[10];			// 标准点浓度数组,标准点1浓度保存在StdConc[0]中、标准点2浓度保存在StdConcStdConc[1]中......
	StandardRecord()
	{
		ID = 0;
		ProductionDate = 0;
		ExpirationDate = 0;
		RatedTestTimes = 0;
		RemainTimes = 0;
		FirstLoadTime = 0;
		FirstUseTime = 0;
		CountOfStdPoints = 0;
		for(int i=0; i<10; i++)
			StdConc[i] = -1;
	}
};

// *******************************************************
// 标准品类
// *******************************************************
class CStandardTable : public CDBTableTemplate<StandardRecord>
{
private:
	CStandardTable(void);

public:
	static CStandardTable * GetInstance()											// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CStandardTable();
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

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     根据指定条件查询标准品信息
	// 参数：
	//      1.StandardList - 返回标准品存储列表
	// 		2 IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	//		3.ID - 指定key
	//		4.strBatchNum - 批号
	//		5.strProjectName - 项目名称
	//		6.strShortName - 项目简称
	//		7.dtProductionDateUpper - 生产日期上限
	//		8.dtProductionDateLower - 生产日期下限
	//		9.dtExpirationDateUpper - 失效日期上限
	//		10.dtExpirationDateLower - 失效日期下限
	// ---------------------------------------------------------------------------
	bool GetStandards(
		vector<StandardRecord> & StandardList,
		const bool &IsFuzzyQuery = true,
		const uint32 &ID = 0,
		const CString &strBatchNum = _T(""),	
		const CString &strProjectName = _T(""),
		const CString &strShortName = _T(""),
		const DATE &dtProductionDateUpper = 0, 
		const DATE &dtProductionDateLower = 0,
		const DATE &dtExpirationDateUpper = 0,
		const DATE &dtExpirationDateLower = 0
		);	          

	~CStandardTable(void);
	
	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     获取指定名称的所有标准品信息
	// 参数：
	//      1.StandardList - 返回标准品存储列表
	//		2.strProjectName - 项目名称
	// 		3 IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	// ---------------------------------------------------------------------------
	bool GetStandardsByProjecName( vector<StandardRecord>& StandardList,
									const CString &strProjectName,
									const bool &IsFuzzyQuery = false)
	{
		if( GetStandards(/*vector<StandardRecord> & = */ StandardList,
						/*bool IsFuzzyQuery = */		 IsFuzzyQuery,
						/*ID = */						 0,
						/*strBatchNum = */				_T(""),	
						/*strProjectName = */			strProjectName
					      )== true)
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

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   取得各种不同“批号”
	// 参数：
	//      1.BatchNumsList - 返回项目名称的存储列表
	// ---------------------------------------------------------------------------
	void GetDistinctBatchNums(vector<CString>& BatchNumsList);					

	static void SortByBatchNumAsc(vector<StandardRecord>& samplelist)				// 按批号升序排序
	{ sort(samplelist.begin(),samplelist.end(),_LessByBatchNum);}

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   根据简称判断 V1 > V2 是否成立
	// ---------------------------------------------------------------------------
	static bool _LessByShortName(ProjectFullName & v1,ProjectFullName & v2)	
	{ return v1.ShortName < v2.ShortName; }

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);							// 更新某指定key值的记录至数据库
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
	static bool _LessByBatchNum(StandardRecord & v1,StandardRecord & v2)			// 根据key 判断 V1 > V2 是否成立
	{ return v1.BatchNum < v2.BatchNum; }
																					// 从数据库中读取记录
private:
	static CStandardTable * m_pInstance;
};
