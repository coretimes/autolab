//*****************************************************************************
// 文件名 : SampleTest.h
// 
// 概要   : 样本测试节点类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-8       崔志雷       新建
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
	// 摘要：
	//     重新计算浓度函数
	// ---------------------------------------------------
	bool _RecalcConc();

private:
	ResultRecord m_ResultRecord;							// 结果记录
};
