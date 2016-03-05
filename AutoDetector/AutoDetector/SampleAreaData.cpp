#include "StdAfx.h"
#include "SampleAreaData.h"
#include "TestManagement.h"
#include "ReagentAreaData.h"

CSampleAreaData * CSampleAreaData::m_pInstance = NULL;

CSampleAreaData::CSampleAreaData(void)
{
	for(int c = 1; c <= NUM_COL; c++)
	{
		for(int r = 1; r <= NUM_ROW; r++)
		{
			Sample_Pos pos = GetNumByCoord(c,r);
			m_ArrSamplePos[c-1][r-1].PosNum = pos;
			m_ArrSamplePos[c-1][r-1].IsThereTube = false;
			m_ArrSamplePos[c-1][r-1].IsSetingSample = false;
			m_ArrSamplePos[c-1][r-1].IsStandard = false;
			m_ArrSamplePos[c-1][r-1].IsSelected =false;
		}
	}
	m_HoverPos = 0;
}

void CSampleAreaData::SetColumeState(const int &col,const bool &IsThereTube)
{
	if(col < 1 || col > NUM_COL)
		return;

	for(int row = 1; row <= NUM_ROW; row++)
	{
		m_ArrSamplePos[col-1][row-1].IsThereTube = IsThereTube;
	}
	SpreadDataUpdateMessage();
}

void CSampleAreaData::UpdateTestBuffer()
{
	for(int c = 1; c<=NUM_COL; c++)
	{
		for(int r=1; r <= NUM_ROW; r++)
		{
			// 如果本节点没试管，则返回
			if(!m_ArrSamplePos[c - 1][r - 1].IsThereTube)
				continue;;

			// 如果本节点没有设置样本信息，则返回
			if(!m_ArrSamplePos[c - 1][r - 1].IsSetingSample)
				continue;
			_ConfigNodeToTestNodes(m_ArrSamplePos[c - 1][r - 1]);
		}
	}
}

void CSampleAreaData::UpdateSearchList()
{
	m_SearchListInReagentPos.clear();
	for(int c = 1; c<=NUM_COL; c++)
	{
		for(int r=1; r <= NUM_ROW; r++)
		{
			// 本节点引用
			SampleConfigNode & CurrenConfigNode = m_ArrSamplePos[c - 1][r - 1];

			// 如果本节点没试管，则返回
			if(!CurrenConfigNode.IsThereTube)
				continue;;

			// 如果本节点没有设置样本信息，则返回
			if(!CurrenConfigNode.IsSetingSample)
				continue;

			// 遍历待测项目数组
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iterTestProjArr = CurrenConfigNode.TestProjArr.begin();
			while(iterTestProjArr != CurrenConfigNode.TestProjArr.end())
			{
				// 在查询表中则插入<Reagent_Pos,Sample_Pos>信息
				Reagent_Pos ReagentPos = iterTestProjArr->first;
				Sample_Pos SamplePos = GetNumByCoord(c,r);
				m_SearchListInReagentPos.insert(pair<Reagent_Pos,Sample_Pos>(ReagentPos,SamplePos));
				iterTestProjArr++;
			}
		}
	}
}

void CSampleAreaData::GetSelectedNodes(map<Sample_Pos,SampleConfigNode *> & SampleConfigNodeList)
{
	// 生成样本区节点容器
	for(int c = 1; c <= NUM_COL; c++)
	{
		for(int r = 1; r <= NUM_ROW; r++)
		{
			if(m_ArrSamplePos[c-1][r-1].IsSelected)
			{ 
				Sample_Pos pos = GetNumByCoord(c,r);				
				SampleConfigNodeList.insert(pair<Sample_Pos,SampleConfigNode *>(pos, &m_ArrSamplePos[c-1][r-1]));
			}
		}
	}
}

void CSampleAreaData::CancelAllSelected()
{
	// 取消所有选择
	for(int c = 1; c <= NUM_COL; c++)
	{
		for(int r = 1; r <= NUM_ROW; r++)
		{
			if(m_ArrSamplePos[c-1][r-1].IsSelected)
			{ 
				m_ArrSamplePos[c-1][r-1].IsSelected =false;
			}
		}
	}
}

void CSampleAreaData::_ConfigNodeToTestNodes(SampleConfigNode & ConfigNode)
{
	CTestManagement * pTestMan = CTestManagement::GetInstance();
	CReagentAreaData * pReagentData = CReagentAreaData::GetInstance();
	// 遍历待测项目数组
	map<Sample_Pos,SampleConfigNode::TestProjInfo>::iterator iter = ConfigNode.TestProjArr.begin();
	while(iter != ConfigNode.TestProjArr.end())
	{
		// 如果还没有生成测试单元
		if(iter->second.TestUintKeys < 1)
		{
			SampleTestNode newTestNode;
			newTestNode.IsStandard = ConfigNode.IsStandard;
			newTestNode.SampleID = ConfigNode.RecordID;
			newTestNode.StdPointIndex = ConfigNode.StdPointIndex;
			newTestNode.SamplePos = ConfigNode.PosNum;
			newTestNode.ReagentID = pReagentData->GetNode(iter->second.ReagentPos)->ReagentID;
			newTestNode.ReagentPos = iter->second.ReagentPos;
			newTestNode.repeat = iter->second.Repetitions;
			newTestNode.IsValid = true;

			// 把本节点测试单元赋值为返回测试单元ID
			iter->second.TestUintKeys = pTestMan->AddNewNode(newTestNode);
		}
		iter++;
	}
}

void CSampleAreaData::SpreadStateChangeMessage(const Sample_Pos &pos)
{
	for(int i=0; i < (int)(m_AssociateDlgList.size()); i++)
	{
		m_AssociateDlgList[i]->SendMessage(UM_SampleArea_StateChange,pos,NULL);
	}
};


void CSampleAreaData::SpreadDataUpdateMessage()
{
	for(int i=0; i < (int)(m_AssociateDlgList.size()); i++)
	{
		m_AssociateDlgList[i]->SendMessage(UM_SampleArea_DataUpdate,NULL,NULL);
	}
};

int CSampleAreaData::GetSampleByReagent(const Reagent_Pos ReagentPos,vector<Sample_Pos> & ListOfSamplePos)
{
	int num = (int)(m_SearchListInReagentPos.count(ReagentPos));
	multimap<Reagent_Pos,Sample_Pos>::iterator iter = m_SearchListInReagentPos.find(ReagentPos);
	for (int i=0; i<num; i++)
	{
		ListOfSamplePos.push_back(iter->second);
		iter++;
	}
	return num;
}