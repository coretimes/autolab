//*****************************************************************************
// �ļ��� : TestUnit.h
// 
// ��Ҫ   : ������������� (ͷ�ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-8       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <map>
#include <vector>

using namespace std;
// *******************************************************
// �������Խڵ�ṹ��һ�����Ե�Ԫ��һ�������������Խڵ����
// *******************************************************
struct SampleTestNode 
{
	enum TestState
	{
		NotStarted = 0,									// δ��ʼ
		Sending,										// ���ݷ�����
		WaiteMetering,									// �ȴ����
		InMetering,										// �����
		Calculating,									// ������
		Over											// ���
	};
	bool IsStandard;									// �Ƿ��Ǳ�׼Ʒ
	Sample_Key SampleID;								// ����׼Ʒ�����������еļ�¼ID
	ConcPoint_Index StdPointIndex;						// ��׼������������Ǳ�׼Ʒ��ֵΪ��׼���������������������Ϊ0
	Sample_Pos SamplePos;								// ��������������λ�ñ��
	Reagent_Key ReagentID;								// �������Լ�ID
	Reagent_Pos ReagentPos;								// �Լ����Լ����е�λ�ñ��
	int repeat;											// ���ܴ���
	bool IsValid;										// ���ڵ��Ƿ���Ч
	TestState state;									// �ڵ�״̬
	double AverRlu;										// ƽ������ֵ
	double CV;											// ����ϵ��
	double conc;										// ����Ũ��
	double ThoeryConc;									// ����Ũ��
	vector<Metering_ID> arrMeterNodeID;					// ���ڵ�ID����
	SampleTestNode()
	{
		IsStandard = false;
		SampleID = 0;
		StdPointIndex = -1;
		SamplePos = 0;
		ReagentID = 0;
		ReagentPos = 0;
		repeat = -1;
		IsValid = false;
		AverRlu = -1;
		CV = -1;
		conc = -1;
		ThoeryConc = -1;
	}
};

// *******************************************************
// ���Ե�Ԫ����
// *******************************************************
class CTestUnit
{
public:
	CTestUnit(){};
	virtual ~CTestUnit(void);

	// --------------------------------------------------
	// ժҪ��
	//     ����������Ũ�Ƚڵ�
	// ������
	//     1.node - �������ڵ�
	// --------------------------------------------------
	void AddSampleNode(const SampleTestNode & node);

	// ----------------------------------- ---------------
	// ժҪ��
	//     ȡ������Ũ�Ƚڵ�ָ��
	// ������
	//     1.index - Ũ�ȵ�����
	//     2.node - ���صĽڵ���Ϣ�洢��
	// --------------------------------------------------
	void GetSampleNodeInfo (const ConcPoint_Index & index,SampleTestNode & node) const
	{
		map<ConcPoint_Index,SampleTestNode>::const_iterator iter = m_mapSampleTestNodes.find(index);
		if(iter != m_mapSampleTestNodes.end())
			node = iter->second;
	}

	TestUnit_ID GetID() const
	{
		return m_UnitID;
	}

	// ---------------------------------------------------
	// ժҪ��
	//		�����ڵ����飬Ϊ�˱�����
	// ---------------------------------------------------
	const map<ConcPoint_Index,SampleTestNode> & GetSampleTestNodes() const
	{
		return m_mapSampleTestNodes;
	}

public:
	bool Recalc();													// ���¼��㺯�����������¼��㷢��ֵ�����¼���Ũ��������

protected:
	virtual bool _RecalcConc() = 0;									// ���¼���Ũ�Ⱥ���

protected:
	TestUnit_ID m_UnitID;											// ���Ե�ԪID
	map<ConcPoint_Index,SampleTestNode> m_mapSampleTestNodes;		// ���Ե�Ԫ�����ڵ�����	
};
