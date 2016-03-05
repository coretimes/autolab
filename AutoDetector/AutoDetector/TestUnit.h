//*****************************************************************************
// 文件名 : TestUnit.h
// 
// 概要   : 样本测试虚基类 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-8       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <map>
#include <vector>

using namespace std;
// *******************************************************
// 样本测试节点结构，一个测试单元由一个或多个样本测试节点组成
// *******************************************************
struct SampleTestNode 
{
	enum TestState
	{
		NotStarted = 0,									// 未开始
		Sending,										// 数据发送中
		WaiteMetering,									// 等待测光
		InMetering,										// 测光中
		Calculating,									// 计算中
		Over											// 完毕
	};
	bool IsStandard;									// 是否是标准品
	Sample_Key SampleID;								// “标准品表”或“样本表”中的记录ID
	ConcPoint_Index StdPointIndex;						// 标准点索引：如果是标准品，值为标准点索引；如果是样本，则为0
	Sample_Pos SamplePos;								// 样本在样本区的位置编号
	Reagent_Key ReagentID;								// 测试用试剂ID
	Reagent_Pos ReagentPos;								// 试剂在试剂区中的位置编号
	int repeat;											// 复管次数
	bool IsValid;										// 本节点是否有效
	TestState state;									// 节点状态
	double AverRlu;										// 平均发光值
	double CV;											// 变异系数
	double conc;										// 计算浓度
	double ThoeryConc;									// 理论浓度
	vector<Metering_ID> arrMeterNodeID;					// 测光节点ID数组
	SampleTestNode()
	{
		IsStandard = false;
		SampleID = 0;
		StdPointIndex = -1;
		SamplePos = 0;
		ReagentID = 0;
		ReagentPos = 0;
		repeat = -1;
		IsValid = false;
		AverRlu = -1;
		CV = -1;
		conc = -1;
		ThoeryConc = -1;
	}
};

// *******************************************************
// 测试单元基类
// *******************************************************
class CTestUnit
{
public:
	CTestUnit(){};
	virtual ~CTestUnit(void);

	// --------------------------------------------------
	// 摘要：
	//     增加新样本浓度节点
	// 参数：
	//     1.node - 新样本节点
	// --------------------------------------------------
	void AddSampleNode(const SampleTestNode & node);

	// ----------------------------------- ---------------
	// 摘要：
	//     取得样本浓度节点指针
	// 参数：
	//     1.index - 浓度点索引
	//     2.node - 返回的节点信息存储区
	// --------------------------------------------------
	void GetSampleNodeInfo (const ConcPoint_Index & index,SampleTestNode & node) const
	{
		map<ConcPoint_Index,SampleTestNode>::const_iterator iter = m_mapSampleTestNodes.find(index);
		if(iter != m_mapSampleTestNodes.end())
			node = iter->second;
	}

	TestUnit_ID GetID() const
	{
		return m_UnitID;
	}

	// ---------------------------------------------------
	// 摘要：
	//		样本节点数组，为了遍历用
	// ---------------------------------------------------
	const map<ConcPoint_Index,SampleTestNode> & GetSampleTestNodes() const
	{
		return m_mapSampleTestNodes;
	}

public:
	bool Recalc();													// 重新计算函数，包括重新计算发光值和重新计算浓度两大步骤

protected:
	virtual bool _RecalcConc() = 0;									// 重新计算浓度函数

protected:
	TestUnit_ID m_UnitID;											// 测试单元ID
	map<ConcPoint_Index,SampleTestNode> m_mapSampleTestNodes;		// 测试单元样本节点数组	
};
