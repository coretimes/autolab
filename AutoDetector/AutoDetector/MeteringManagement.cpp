#include "StdAfx.h"
#include "MeteringManagement.h"

CMeteringManagement * CMeteringManagement::m_pInstance = NULL;
CMeteringManagement::CMeteringManagement(void)
{
	m_MaxID = 0;
	m_CurrentTick = 0;
}

CMeteringManagement::~CMeteringManagement(void)
{
}

Metering_ID CMeteringManagement::AndNode(TestUnit_ID unitID, int concindex,int MeterIndex)
{
	// 最大ID号加1
	m_MaxID++;								

	// 创建新测光节点
	MeteringNode node;
	node.ID = m_MaxID;														// 测光ID赋值
	node.RealatedInfoKey.ParentUnitID = unitID;								// 所属测试单元ID
	node.RealatedInfoKey.ConcIndex = concindex;								// 所属浓度点索引
	node.MeteringIndex = MeterIndex;

	m_NodeList.insert(map<Metering_ID, MeteringNode>::value_type(m_MaxID,node));
	return node.ID;
}

bool CMeteringManagement::DeleteNode(Metering_ID nodeKey)
{
	m_NodeList.erase(nodeKey);
	return true;
}