//*****************************************************************************
// 文件名 : TestManagement.h
// 
// 概要   : 测试管理类 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-11       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "TestUnit.h"
#include <map>
#include "SampleTest.h"
#include "StandardTest.h"

// 测试缓冲区数据更新消息定义
// wparam: 测试单元ID
// lparam: NULL

#define UM_TestBuffer_Update		WM_USER + 1002

using namespace std;
typedef pair<Sample_Key,Reagent_Key> Sample_Reagent_Key;

class CTestManagement
{
public:
	CTestManagement(void);
	~CTestManagement(void);

public:
	static CTestManagement * GetInstance()											// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CTestManagement();
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
	// ---------------------------------------------------
	// 摘要：
	//		加入一个新的样本测试节点，自动分配测试单元
	// 参数：
	//		node - 新的样本测试节点
	// ---------------------------------------------------
	TestUnit_ID AddNewNode(const SampleTestNode & node);

	// ---------------------------------------------------
	// 摘要：
	//		根据测试单元ID，取得测试单元指针
	// 参数：
	//		ID - 测试单元ID
	// ---------------------------------------------------
	CTestUnit * GetTestUnit(TestUnit_ID ID)
	{
		map<TestUnit_ID, CTestUnit *>::iterator iter = m_TestBuffer.find(ID);
		if(iter != m_TestBuffer.end())
			return iter->second;
		else
			return NULL;
	}

	// ---------------------------------------------------
	// 摘要：
	//		取得测试缓冲区数据，为了遍历用
	// ---------------------------------------------------
	const map<TestUnit_ID, CTestUnit *> & GetTestBuffer() const
	{
		return m_TestBuffer;
	}

	// --------------------------------------------------
	// 摘要：
	//	   添加新关联对话框
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
	};

	// --------------------------------------------------
	// 摘要：
	//	   向关联对话框散布数据更新的消息
	// 返回：
	//     1.unitID.数据的测试单元ID
	// --------------------------------------------------
	void SpreadUpdateMessage(TestUnit_ID unitID);

private:
	// ---------------------------------------------------
	// 摘要：
	//		新建一个测试单元，返回单元ID
	// 参数：
	//		node - 新的样本测试节点
	// ---------------------------------------------------
	TestUnit_ID _AddNewUnit(const SampleTestNode & node);

private:
	// ---------------------------------------------------
	// 成员名：
	//		测试单元存储映射表
	// ---------------------------------------------------
	map<TestUnit_ID, CTestUnit *> m_TestBuffer;

	// ---------------------------------------------------
	// 成员名：
	//		查找表1,查找方式：样本ID -> 测试单元ID
	//      一个样本能有多个测试单元
	// ---------------------------------------------------
	multimap<Sample_Key,TestUnit_ID> m_SearchSampleKey;

	// ---------------------------------------------------
	// 成员名：
	//		查找表2,查找方式：试剂ID -> 测试单元ID
	//		一个试剂也能有多个测试单元
	// ---------------------------------------------------
	multimap<Reagent_Key,TestUnit_ID> m_SearchReagentKey;

	// ---------------------------------------------------
	// 成员名：
	//		查找表3,查找方式：<样本ID，试剂ID> -> 测试单元ID
	//		同一个样本，同一个试剂也能测试多次，形成多个测试单元
	// ---------------------------------------------------
	multimap<Sample_Reagent_Key,TestUnit_ID> m_Search_SampleReagentKey;

	// ---------------------------------------------------
	// 成员名：
	//		当前最大的测试单元ID
	// ---------------------------------------------------
	TestUnit_ID m_MaxUnitID;

private:
	static CTestManagement * m_pInstance;
	vector<CDialog *> m_AssociateDlgList ;							// 关联对话框指针
};
