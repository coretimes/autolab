//*****************************************************************************
// �ļ��� : MeteringManagement.h
// 
// ��Ҫ   : ���ڵ���� (ͷ�ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-2       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <map>
using namespace std;

//*******************************************************
// ����������Ϣkey
//*******************************************************
struct RealatedTestKey
{
	TestUnit_ID ParentUnitID;							// �������Ե�ԪID
	ConcPoint_Index ConcIndex;							// Ũ�������������������Ե�Ԫ����ֵΪ0�����ڱ�׼Ʒ����Ũ�ȵ�������

	bool operator<(const RealatedTestKey & rightvalue) const
	{
		if (ParentUnitID > rightvalue.ParentUnitID)
			return false;

		if (ParentUnitID == rightvalue.ParentUnitID)
		{
			if (ConcIndex >= rightvalue.ConcIndex)
				return false;
		}
		return true;
	}
};

//*******************************************************
// ���ڵ�ṹ
//*******************************************************
struct MeteringNode
{
	enum MeteringState
	{
		Unsent,											// δ����
		Sending,										// ���ݷ�����
		WaitePipetting,									// �ȴ�����
		Pipetting,										// ���ڼ���
		AfterPipetting,									// �������
		Incubating,										// ������
		WaiteCleaning,									// �ȴ���ϴ
		Cleaning,										// ������ϴ
		WaiteMetering,									// �ȴ����
		Metering,										// ���ڲ��
		Over											// ������
	};

	Metering_ID ID;										// ID
	double Rlu;											// ����ֵ
	bool IsValid;										// ������ֵ�Ƿ���Ч
	Tick_ID begin;										// ��ʼ����
	Tick_ID end;										// ��������
	MeteringState state;								// ���ڵ�״̬
	RealatedTestKey RealatedInfoKey;					// ������Ϣkey
	Metering_Index MeteringIndex;						// ���ڵ�����

	MeteringNode()
	{
		ID = 0;
		Rlu = 0;
		IsValid = false;
		begin = 0;
		end = 0;
		state = Unsent;
		RealatedInfoKey.ParentUnitID = 0;
		RealatedInfoKey.ConcIndex = -1;
	}
};

//*******************************************************
// ���ڵ��������������ģʽ
//*******************************************************
class CMeteringManagement
{
private:
	CMeteringManagement(void);

public:
	~CMeteringManagement(void);

	// --------------------------------------------------
	// ժҪ��
	//     ȡ�õ���ģʽ��ʵ��
	// --------------------------------------------------
	static CMeteringManagement * GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CMeteringManagement;
		return m_pInstance;
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ɾ��ʵ��
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	// --------------------------------------------------
	// ժҪ��
	//     �����²��ڵ�
	// ������
	//     1.TestUnitID - �������Ե�Ԫ��ID
	//     2.ConcIndex - ����Ũ�ȵ�����
	//     3.MeterIndex - ��Ũ�ȵ��е�����
	// --------------------------------------------------
	Metering_ID AndNode(TestUnit_ID TestUnitID,int ConcIndex,int MeterIndex);

	// --------------------------------------------------
	// ժҪ��
	//     ɾ�����ڵ�
	// --------------------------------------------------
	bool DeleteNode(Metering_ID ID);

	// --------------------------------------------------
	// ժҪ��
	//     ���ݲ��ڵ�ID��ȡ�ò��ڵ�
	// ������
	//     1.ID - ���ڵ�ID
	//     2.node - ���ز��ڵ���Ϣ�洢��
	// --------------------------------------------------
	void GetNode(const Metering_ID & ID,MeteringNode & node) const
	{
		map<Metering_ID, MeteringNode>::const_iterator iter = m_NodeList.find(ID);
		if(iter != m_NodeList.end())
			node = iter->second;
	}

	// --------------------------------------------------
	// ժҪ��
	//     ȡ�õ�ǰ��ʱ����
	// --------------------------------------------------
	Tick_ID GetCurrentTick()
	{
		return m_CurrentTick;
	}

	// --------------------------------------------------
	// ժҪ��
	//     ���õ�ǰ��ʱ����
	// ������
	//     1.tick - ����
	// --------------------------------------------------
	void SetCurrentTick(Tick_ID tick)
	{
		m_CurrentTick = tick;
	}

	// --------------------------------------------------
	// ժҪ��
	//     ������������֮���ʱ����
	// ������
	//     1.interval - ����ʱ�䣨���룩
	// --------------------------------------------------
	void SetTickInterval(int interval)
	{
		m_nTickInterval = interval;
	}

	// --------------------------------------------------
	// ժҪ��
	//     ȡ����������֮���ʱ����
	// --------------------------------------------------
	int GetTickInterval()
	{
		return m_nTickInterval;
	}

private:
	static CMeteringManagement * m_pInstance;
	map<Metering_ID, MeteringNode> m_NodeList;
	multimap<RealatedTestKey,Metering_ID> m_SearchList;		// �ɹ�����Ϣ��ѯID

	Metering_ID m_MaxID;									// ����ܽڵ�ID										
	Tick_ID m_CurrentTick;									// ��ǰ��ʱ���ڣ�����λ����ǰ����ͬ��
	int m_nTickInterval;									// ��������ʱ��������λ�����룩
};
