#include "StdAfx.h"
#include "ReagentAreaData.h"

CReagentAreaData * CReagentAreaData::m_pInstance = NULL;

CReagentAreaData::CReagentAreaData(void)
{
	for(int i=0; i<NUM_REAGENTPOS; i++)
	{
		m_ArrReagentPos[i].pos = i + 1;
		m_ArrReagentPos[i].IsThereTube = false;
		m_ArrReagentPos[i].ReagentID = 0;
	}
	m_ArrReagentPos[0].IsThereTube = true;
	//m_ArrReagentPos[0].IsSetingReagent = true;
	//m_ArrReagentPos[0].ReagentID = 1;

	m_ArrReagentPos[1].IsThereTube = true;
	m_ArrReagentPos[1].IsSetingReagent = true;
	m_ArrReagentPos[1].ReagentID = 2;

	m_ArrReagentPos[2].IsThereTube = true;
	//m_ArrReagentPos[2].IsSetingReagent = true;
	//m_ArrReagentPos[2].ReagentID = 3;

	m_ArrReagentPos[3].IsThereTube = true;
	//m_ArrReagentPos[3].IsSetingReagent = true;
	//m_ArrReagentPos[3].ReagentID = 4;

	m_ArrReagentPos[4].IsThereTube = true;
	m_ArrReagentPos[4].IsSetingReagent = true;
	m_ArrReagentPos[4].ReagentID = 5;

	m_ArrReagentPos[5].IsThereTube = true;
	//m_ArrReagentPos[5].IsSetingReagent = true;
	//m_ArrReagentPos[5].ReagentID = 6;

	m_ArrReagentPos[6].IsThereTube = true;
	m_ArrReagentPos[6].IsSetingReagent = true;
	m_ArrReagentPos[6].ReagentID = 7;

	m_ArrReagentPos[7].IsThereTube = true;
	m_ArrReagentPos[7].IsSetingReagent = true;
	m_ArrReagentPos[7].ReagentID = 8;

	//m_ArrReagentPos[8].IsThereTube = true;
	//m_ArrReagentPos[8].IsSetingReagent = true;
	//m_ArrReagentPos[8].ReagentID = 9;

	m_ArrReagentPos[9].IsThereTube = true;
	m_ArrReagentPos[9].IsSetingReagent = true;
	m_ArrReagentPos[9].ReagentID = 10;

	m_ArrReagentPos[10].IsThereTube = true;
	//m_ArrReagentPos[10].IsSetingReagent = true;
	//m_ArrReagentPos[10].ReagentID = 11;

	m_ArrReagentPos[11].IsThereTube = true;
	m_ArrReagentPos[11].IsSetingReagent = true;
	m_ArrReagentPos[11].ReagentID = 12;

	//m_ArrReagentPos[12].IsThereTube = true;
	//m_ArrReagentPos[12].IsSetingReagent = true;
	//m_ArrReagentPos[12].ReagentID = 13;

	m_ArrReagentPos[13].IsThereTube = true;
	m_ArrReagentPos[13].IsSetingReagent = true;
	m_ArrReagentPos[13].ReagentID = 14;

	m_ArrReagentPos[14].IsThereTube = true;
	m_ArrReagentPos[14].IsSetingReagent = true;
	m_ArrReagentPos[14].ReagentID = 15;
}

CReagentAreaData::~CReagentAreaData(void)
{
}
void CReagentAreaData::GetReagentConfigList(map<Reagent_Pos,const ReagentConfigNode *> & mapReagents) const
{
	for (Reagent_Pos i=0; i < NUM_REAGENTPOS; i++)
	{
		if(m_ArrReagentPos[i].IsThereTube == false)
			continue;
		if(m_ArrReagentPos[i].ReagentID > 0)
			mapReagents.insert( pair<Reagent_Pos,const ReagentConfigNode *>(i + 1, &m_ArrReagentPos[i]));
	}
}

void CReagentAreaData::SpreadUpdateMessage(Reagent_Pos pos)
{
	for(int i=0; i < (int)(m_AssociateDlgList.size()); i++)
	{
		m_AssociateDlgList[i]->SendMessage(UM_ReagentArea_Update,pos,0);
	}
};
