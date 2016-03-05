#include "StdAfx.h"
#include "SampleTest.h"
#include "GeneralFuction.h"
#include "CurveManagement.h"

CSampleTest::CSampleTest(TestUnit_ID UintID)
{
	m_UnitID = UintID;
}

CSampleTest::~CSampleTest(void)
{
}


void CSampleTest::SetCurve(const uint32& CurveID)
{
	m_ResultRecord.CurveID = CurveID;
}

bool CSampleTest::_RecalcConc()
{
	// 根据曲线ID，创建曲线
	CCurveManagement* pCurveList = CCurveManagement::GetInstance();
	CCaliCurve* pCurve = pCurveList->GetCurve(m_ResultRecord.CurveID);
	if(pCurve == NULL)
		return false;
	else
	{
		// 循环遍历每个样本节点，计算各样本节点的理论浓度值
		map<ConcPoint_Index,SampleTestNode>::iterator iter = m_mapSampleTestNodes.begin();
		while(iter != m_mapSampleTestNodes.end())
		{
			iter->second.conc = pCurve->GetConcByRul(iter->second.AverRlu);
			iter++;
		}
	}
	return true;
}