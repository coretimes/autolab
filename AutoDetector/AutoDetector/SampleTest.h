//*****************************************************************************
// �ļ��� : SampleTest.h
// 
// ��Ҫ   : �������Խڵ���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-8       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once

#include "TestUnit.h"
#include <vector>
#include "SampleTable.h"
#include "ResultTable.h"

using namespace std;
class CSampleTest : public CTestUnit
{
public:
	CSampleTest(TestUnit_ID UintID);
	~CSampleTest(void);

public:
	void SetCurve(const uint32 &CurveID);

protected:
	// ---------------------------------------------------
	// ժҪ��
	//     ���¼���Ũ�Ⱥ���
	// ---------------------------------------------------
	bool _RecalcConc();

private:
	ResultRecord m_ResultRecord;							// �����¼
};
