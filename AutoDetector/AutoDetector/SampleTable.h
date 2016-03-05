//*****************************************************************************
// 文件名 : SampleTable.h
// 
// 概要   : 样本表类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-7       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "DBTableTemplate.h"
#include <algorithm>

// *******************************************************
// 样本表记录结构
// *******************************************************
struct SampleRecord
{
	Sample_Key ID;				// key
	CString SampleNo;			// 样本编号,人工输入或自动生成
	CString Type;				// 样本类型
	double Dilution;			// 稀释倍数
	CString Dept;				// 送检单位名称
	CString Applicant;			// 送检人
	DATE ApplyDate;				// 送检日期
	DATE UpdateDate;			// 样本录入日期
	SampleRecord()
	{
		ID = 0;
		Dilution = 0;			// 稀释倍数
		ApplyDate = 0;			// 送检日期
		UpdateDate = 0;			// 样本录入日期
	}
};

// *******************************************************
// 样品表类
// *******************************************************
class CSampleTable : public CDBTableTemplate<SampleRecord>
{
private:
	CSampleTable(void);

public:
	static CSampleTable * GetInstance() 									// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSampleTable();
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

	~CSampleTable(void);

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     根据指定条件查询样本信息
	// 参数：
	//      1.SampleList - 返回样本存储列表
	// 		2 IsFuzzyQuery - 是否模糊查询,只对字符串类型数据有效
	//		3.ID - 指定key
	//		4.strSampleNo - 标本号
	//		5.strType - 样本类型
	//		6.dDilutionUpper - 稀释倍数上限
	//		7.dDilutionLower - 稀释倍数下下限
	//		8.strDept - 送检单位名称
	//		9.strApplicant - 送检人
	//		10.ApplyDateUpper - 送检时上限
	//      11.ApplyDateLower - 送检时间下限
	//		12.UpdateDateUpper - 更新时间上限
	//		13.UpdateDateLower - 更新时间下限
	// ---------------------------------------------------------------------------
	void GetSamples(
		vector<SampleRecord> & SampleList,
		const bool &IsFuzzyQuery = true,
		const uint32 &ID = 0,					          
		const CString &strSampleNo = _T(""),			         
		const CString &strType = _T(""),			            
		const double &dDilutionUpper = 0,                  
		const double &dDilutionLower = 0,
		const CString &strDept = _T(""),				      
		const CString &strApplicant = _T(""),
		const DATE &ApplyDateUpper = 0,				     
		const DATE &ApplyDateLower = 0,
		const DATE &UpdateDateUpper = 0,             
		const DATE &UpdateDateLower = 0
		);	          

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//     根据指定样本更新日期查询样本信息
	// ---------------------------------------------------------------------------
	void GetSamplesByUpdateDate(vector<SampleRecord>& SampleList,const DATE &date)
	{
		GetSamples(	/*SampleList = */		SampleList,
					/* IsFuzzyQuery = */    false,
					/*ID = */				0,					          
					/*strSampleNo = */		_T(""),			         
					/*&strType = */			_T(""),			            
					/*dDilutionUpper = */	0,                  
					/*dDilutionLower = */	0,
					/*strDept = */			_T(""),				      
					/*strApplicant = */		_T(""),
					/*ApplyDateUpper = */	0,				     
					/*ApplyDateLower = */	0,
					/*UpdateDateUpper = */	date,             
					/*UpdateDateLower = */	date
				   );
	}

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   读取所有样本信息
	// ---------------------------------------------------------------------------
	void GetAllSamples(vector<SampleRecord>& SampleList)					
	{
		GetSamples(SampleList);
	}

	// ---------------------------------------------------------------------------
	// 函数介绍：
	//	   取得各种不同“样本类型”名称
	// 参数：
	//      1.TypeList - 返回样本类型名称的存储列表
	// ---------------------------------------------------------------------------
	void GetDistinctType(vector<CString>& TypeList);					

	static void SortByIDDesc(vector<SampleRecord>& samplelist)				// 按key值降序排序
	{ sort(samplelist.begin(),samplelist.end(),_GreaterByID);}

	static void SortByIDAsc(vector<SampleRecord>& samplelist) 				// 按key值升序排序
	{ sort(samplelist.begin(),samplelist.end(),_LessByID);}

	static void SortBySampleNoDesc(vector<SampleRecord>& samplelist) 		// 按样本号降序排序
	{ sort(samplelist.begin(),samplelist.end(),_GreaterBySampleNo);}

	static void SortBySampleNoAsc(vector<SampleRecord>& samplelist) 		// 按样本号升序排序
	{ sort(samplelist.begin(),samplelist.end(),_LessBySampleNo);}

private:
	virtual BOOL _UpdateRecordsToDB(vector<UINT> KeyList);					// 更新某指定key值的记录至数据库
	virtual	BOOL _SelectRecordsFromDB(CString & strSQL,vector<UINT> &KeyList);
																			// 从数据库中读取记录

	// ---------------------------------------------------------------------------
	// 排序用比较函数
	// ---------------------------------------------------------------------------
	static bool _GreaterByID(SampleRecord & v1,SampleRecord & v2)			// 根据key 判断 V1 > V2 是否成立
	{ return v1.ID > v2.ID; }
	static bool _LessByID(SampleRecord & v1,SampleRecord & v2)				// 根据key 判断 V1 < V2 是否成立
	{ return v1.ID < v2.ID; }
	static bool _GreaterBySampleNo(SampleRecord & v1,SampleRecord & v2)		// 根据样本号 判断 V1 > V2 是否成立
	{ return v1.SampleNo > v2.SampleNo; }
	static bool _LessBySampleNo(SampleRecord & v1,SampleRecord & v2)		// 根据样本号 判断 V1 < V2 是否成立
	{ return v1.SampleNo < v2.SampleNo; }
private:
	static CSampleTable * m_pInstance;
};
