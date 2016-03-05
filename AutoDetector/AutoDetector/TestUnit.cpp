#include "StdAfx.h"
#include "TestUnit.h"
#include "GeneralFuction.h"
#include "MeteringManagement.h"

CTestUnit::~CTestUnit(void)
{
}

void CTestUnit::AddSampleNode(const SampleTestNode & node)
{
	m_mapSampleTestNodes[node.StdPointIndex] = node;

	// 生成测光节点，保存测光节点ID
	for(int i = 0; i< node.repeat; i++)
	{
		Metering_ID mid = CMeteringManagement::GetInstance()->AndNode(m_UnitID,node.StdPointIndex,i + 1);
		m_mapSampleTestNodes[node.StdPointIndex].arrMeterNodeID.push_back(mid);
	}
}

bool CTestUnit::Recalc(void)
{
	CGeneralFuction * pFuntion = CGeneralFuction::GetInstance();
	vector<double> rlulist;
	CMeteringManagement * pMeteringList = CMeteringManagement::GetInstance();

	// 循环遍历每个样本节点，计算各样本节点的平均发光值
	map<ConcPoint_Index,SampleTestNode>::iterator iter = m_mapSampleTestNodes.begin();
	while(iter != m_mapSampleTestNodes.end())
	{
		// 循环遍历每个测光节点
		for(int i=0; i < iter->second.repeat; i++)
		{
			MeteringNode node;
			pMeteringList->GetNode(iter->second.arrMeterNodeID[i],node);
			if(node.IsValid)
			{
				rlulist.push_back(node.Rlu);
			}	
		}
		iter->second.AverRlu = pFuntion->Average(rlulist);
		iter->second.CV = pFuntion->GetCV(rlulist);
		iter++;
	}
	return _RecalcConc();
}

