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
	// ���ID�ż�1
	m_MaxID++;								

	// �����²��ڵ�
	MeteringNode node;
	node.ID = m_MaxID;														// ���ID��ֵ
	node.RealatedInfoKey.ParentUnitID = unitID;								// �������Ե�ԪID
	node.RealatedInfoKey.ConcIndex = concindex;								// ����Ũ�ȵ�����
	node.MeteringIndex = MeterIndex;

	m_NodeList.insert(map<Metering_ID, MeteringNode>::value_type(m_MaxID,node));
	return node.ID;
}

bool CMeteringManagement::DeleteNode(Metering_ID nodeKey)
{
	m_NodeList.erase(nodeKey);
	return true;
}