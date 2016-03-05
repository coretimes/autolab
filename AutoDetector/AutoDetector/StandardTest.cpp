#include "StdAfx.h"
#include "StandardTest.h"

CStandardTest::CStandardTest(TestUnit_ID UintID)
{
	m_UnitID = UintID;
}

CStandardTest::~CStandardTest(void)
{
}
void CStandardTest::MeteringNodeUpdate(uint32 MeterNodeID)
{
	//double SumRlu = 0;														// 发光值总和
	//double StdDev = 0;														// 发光值标准差
	//int NumOfValidNode = 0;													// 有效测光节点发光值


	//CMeteringManagement * pMeteringList = CMeteringManagement::GetInstance();
	//for(uint32 i=0; i < m_repeat; i++)
	//{
	//	if(pMeteringList->IsValid(m_arrMeterNodeID[i]))
	//	{
	//		SumRlu += pMeteringList->GetRlu(m_arrMeterNodeID[i]);			// 求和
	//		StdDev += pow(pMeteringList->GetRlu(m_arrMeterNodeID[i]),2);	// 求平方和
	//	}	
	//}

	//StdDev = sqrt(StdDev);													// 平方和再开平方
	//m_rlu = SumRlu / m_rlu;
	//m_cv = StdDev / m_rlu;
	//RecalcConc();
}
