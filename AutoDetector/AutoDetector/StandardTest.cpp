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
	//double SumRlu = 0;														// ����ֵ�ܺ�
	//double StdDev = 0;														// ����ֵ��׼��
	//int NumOfValidNode = 0;													// ��Ч���ڵ㷢��ֵ


	//CMeteringManagement * pMeteringList = CMeteringManagement::GetInstance();
	//for(uint32 i=0; i < m_repeat; i++)
	//{
	//	if(pMeteringList->IsValid(m_arrMeterNodeID[i]))
	//	{
	//		SumRlu += pMeteringList->GetRlu(m_arrMeterNodeID[i]);			// ���
	//		StdDev += pow(pMeteringList->GetRlu(m_arrMeterNodeID[i]),2);	// ��ƽ����
	//	}	
	//}

	//StdDev = sqrt(StdDev);													// ƽ�����ٿ�ƽ��
	//m_rlu = SumRlu / m_rlu;
	//m_cv = StdDev / m_rlu;
	//RecalcConc();
}
