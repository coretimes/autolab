//*****************************************************************************
// 文件名 : ConcNode.h
// 
// 概要   : 标准品测试类 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-8       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "TestUnit.h"
class CStandardTest :public CTestUnit
{
public:
	CStandardTest(TestUnit_ID UintID);
	~CStandardTest(void);

public:
	virtual void MeteringNodeUpdate(uint32 MeterNodeID);	// “测光节点更新”响应函数，参数：更新的测光节点ID

protected:
	virtual bool _RecalcConc(){return false;}				// 重新计算浓度函数
};
