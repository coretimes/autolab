//*****************************************************************************
// �ļ��� : ConcNode.h
// 
// ��Ҫ   : ��׼Ʒ������ (ͷ�ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-8       ��־��       �½�
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
	virtual void MeteringNodeUpdate(uint32 MeterNodeID);	// �����ڵ���¡���Ӧ���������������µĲ��ڵ�ID

protected:
	virtual bool _RecalcConc(){return false;}				// ���¼���Ũ�Ⱥ���
};
