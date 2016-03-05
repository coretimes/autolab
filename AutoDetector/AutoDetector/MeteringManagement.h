//*****************************************************************************
// 文件名 : MeteringManagement.h
// 
// 概要   : 测光节点管理 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-2       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <map>
using namespace std;

//*******************************************************
// 关联测试信息key
//*******************************************************
struct RealatedTestKey
{
	TestUnit_ID ParentUnitID;							// 所属测试单元ID
	ConcPoint_Index ConcIndex;							// 浓度索引：对于样本测试单元，本值为0；对于标准品，是浓度点索引号

	bool operator<(const RealatedTestKey & rightvalue) const
	{
		if (ParentUnitID > rightvalue.ParentUnitID)
			return false;

		if (ParentUnitID == rightvalue.ParentUnitID)
		{
			if (ConcIndex >= rightvalue.ConcIndex)
				return false;
		}
		return true;
	}
};

//*******************************************************
// 测光节点结构
//*******************************************************
struct MeteringNode
{
	enum MeteringState
	{
		Unsent,											// 未发送
		Sending,										// 数据发送中
		WaitePipetting,									// 等待加样
		Pipetting,										// 正在加样
		AfterPipetting,									// 加样完毕
		Incubating,										// 孵育中
		WaiteCleaning,									// 等待清洗
		Cleaning,										// 正在清洗
		WaiteMetering,									// 等待测光
		Metering,										// 正在测光
		Over											// 测光完毕
	};

	Metering_ID ID;										// ID
	double Rlu;											// 发光值
	bool IsValid;										// 本发光值是否有效
	Tick_ID begin;										// 开始周期
	Tick_ID end;										// 结束周期
	MeteringState state;								// 测光节点状态
	RealatedTestKey RealatedInfoKey;					// 关联信息key
	Metering_Index MeteringIndex;						// 测光节点索引

	MeteringNode()
	{
		ID = 0;
		Rlu = 0;
		IsValid = false;
		begin = 0;
		end = 0;
		state = Unsent;
		RealatedInfoKey.ParentUnitID = 0;
		RealatedInfoKey.ConcIndex = -1;
	}
};

//*******************************************************
// 测光节点管理容器，单体模式
//*******************************************************
class CMeteringManagement
{
private:
	CMeteringManagement(void);

public:
	~CMeteringManagement(void);

	// --------------------------------------------------
	// 摘要：
	//     取得单体模式的实体
	// --------------------------------------------------
	static CMeteringManagement * GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CMeteringManagement;
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

	// --------------------------------------------------
	// 摘要：
	//     增加新测光节点
	// 参数：
	//     1.TestUnitID - 所属测试单元的ID
	//     2.ConcIndex - 所属浓度点索引
	//     3.MeterIndex - 在浓度点中的索引
	// --------------------------------------------------
	Metering_ID AndNode(TestUnit_ID TestUnitID,int ConcIndex,int MeterIndex);

	// --------------------------------------------------
	// 摘要：
	//     删除测光节点
	// --------------------------------------------------
	bool DeleteNode(Metering_ID ID);

	// --------------------------------------------------
	// 摘要：
	//     根据测光节点ID，取得测光节点
	// 参数：
	//     1.ID - 测光节点ID
	//     2.node - 返回测光节点信息存储区
	// --------------------------------------------------
	void GetNode(const Metering_ID & ID,MeteringNode & node) const
	{
		map<Metering_ID, MeteringNode>::const_iterator iter = m_NodeList.find(ID);
		if(iter != m_NodeList.end())
			node = iter->second;
	}

	// --------------------------------------------------
	// 摘要：
	//     取得当前计时周期
	// --------------------------------------------------
	Tick_ID GetCurrentTick()
	{
		return m_CurrentTick;
	}

	// --------------------------------------------------
	// 摘要：
	//     设置当前计时周期
	// 参数：
	//     1.tick - 周期
	// --------------------------------------------------
	void SetCurrentTick(Tick_ID tick)
	{
		m_CurrentTick = tick;
	}

	// --------------------------------------------------
	// 摘要：
	//     设置相邻周期之间的时间间隔
	// 参数：
	//     1.interval - 周期时间（毫秒）
	// --------------------------------------------------
	void SetTickInterval(int interval)
	{
		m_nTickInterval = interval;
	}

	// --------------------------------------------------
	// 摘要：
	//     取得相邻周期之间的时间间隔
	// --------------------------------------------------
	int GetTickInterval()
	{
		return m_nTickInterval;
	}

private:
	static CMeteringManagement * m_pInstance;
	map<Metering_ID, MeteringNode> m_NodeList;
	multimap<RealatedTestKey,Metering_ID> m_SearchList;		// 由关联信息查询ID

	Metering_ID m_MaxID;									// 最大测管节点ID										
	Tick_ID m_CurrentTick;									// 当前计时周期，与下位机当前周期同步
	int m_nTickInterval;									// 相邻周期时间间隔（单位，毫秒）
};
