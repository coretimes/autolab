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
			// ������ڵ�û�Թܣ��򷵻�
			if(!m_ArrSamplePos[c - 1][r - 1].IsThereTube)
				continue;;

			// ������ڵ�û������������Ϣ���򷵻�
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
			// ���ڵ�����
			SampleConfigNode & CurrenConfigNode = m_ArrSamplePos[c - 1][r - 1];

			// ������ڵ�û�Թܣ��򷵻�
			if(!CurrenConfigNode.IsThereTube)
				continue;;

			// ������ڵ�û������������Ϣ���򷵻�
			if(!CurrenConfigNode.IsSetingSample)
				continue;

			// ����������Ŀ����
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iterTestProjArr = CurrenConfigNode.TestProjArr.begin();
			while(iterTestProjArr != CurrenConfigNode.TestProjArr.end())
			{
				// �ڲ�ѯ���������<Reagent_Pos,Sample_Pos>��Ϣ
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
	// �����������ڵ�����
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
	// ȡ������ѡ��
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
	// ����������Ŀ����
	map<Sample_Pos,SampleConfigNode::TestProjInfo>::iterator iter = ConfigNode.TestProjArr.begin();
	while(iter != ConfigNode.TestProjArr.end())
	{
		// �����û�����ɲ��Ե�Ԫ
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

			// �ѱ��ڵ���Ե�Ԫ��ֵΪ���ز��Ե�ԪID
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