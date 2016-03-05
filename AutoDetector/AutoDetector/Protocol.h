//*****************************************************************************
// �ļ��� : Packet.h
// 
// ��Ҫ   : ����ͨ��Э��
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-10       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "Types.h"
#include "Protocol.h"
#include <vector>
using namespace std;

#define PROT_ADDRESS_CODE					0xa0				//��ַ��
#define PROT_ERROR_MASK						0x80				//��������

// *******************************************************
// ���ݰ�����
// *******************************************************
class CPacket
{
public:
	CPacket(void);
	~CPacket(void){};

public:
	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ����ͨ�����ֽ���
	// ������
	//     1.pBuffer - Ŀ�껺����ָ��
	//     2.BufferLength - ����������
	// ---------------------------------------------------------------------------
	bool CreatByteStream(char * pDestBuffer,const int &DestBufferLength);

	// ---------------------------------------------------------------------------
	// ժҪ��
	//     ȡ�����ݰ����󳤶�
	// ---------------------------------------------------------------------------
	int GetBufferLength(){return sizeof(PacketHeader) + m_header.length;}

protected:

	struct PacketHeader{										// ���ݰ�ͷ
		uint8	address;										// ��ַ��
		uint8	command;										// ������
		uint16	length;											// ���ݳ���
	} m_header;

	char* m_pDataBuffer;										// ָ�����ݲ��ֻ�����ָ��
};

// *******************************************************
// ��������
// *******************************************************
#define PROT_COMMAND_HEARTBEAT				0x65				// ����������
class CHeartBeatPacket : public CPacket
{
public:
	CHeartBeatPacket(void);
	~CHeartBeatPacket(void){};

public:
	struct HeartbeatData {										// �������ݰ�
		uint16	current_tick;									// ��ǰ���ڼ�������
		uint16	tick_count;										// ��������		��
		uint8	main_control_unit;								// ���ص�Ԫ״̬	��
		uint8	mechanical_arm;									// ��е��״̬		��
		uint8	measuring_area;									// ������״̬		��
		uint8	material_area[5];								// �����Լ���״̬	��
		uint16	brood_chambe_temp;								// �������¶�		��
		uint16	reagnet_area_temp;								// �Լ����¶�	��
		uint16  incubation_area;								// ������״̬	��
	} m_Data;
};

// *******************************************************
// �����ò��������
// *******************************************************
#define PROT_COMMAND_READPARAMS				0x45				// �����ò����������
#define PROT_COMMAND_READPARAMS_ERROR		0xC5				// �����ò������������
class CReadParamsPacket : public CPacket
{
public:
	CReadParamsPacket(void);
	~CReadParamsPacket(void){};

public:
	struct ReadParamsCmd {										// �����ò�������
		uint8	start_addr;										// ��ʼ��ַ
		uint8	byte_count;										// ��������
	} m_Cmd;
};

// *******************************************************
// �·�ʵ�����������
// *******************************************************
#define PROT_COMMAND_SENDTEST				0x64				// �·�ʵ�������������
#define PROT_COMMAND_SENDTEST_ERROR			0xE4				// �·�ʵ���������������
class CSendTestPacket : public CPacket
{
public:
	struct NodeParams {											// ���Խڵ����
		uint16	key;											// �ڵ�����
		uint8	is_urgent;										// �Ӽ���־
		uint8	sample_pos;										// ����λ�ñ��
		uint8	sample_volume;									// ������
		uint8	reagent_pos;									// �Լ�λ�ñ��
		uint8	reagent_volume;									// �Լ���
		uint8	bead_volume;									// ������
		uint8	incubation_time;								// ����ʱ��
		uint8	wash_times;										// ��ϴ����
	};

public:
	CSendTestPacket();
	~CSendTestPacket(void){};
	void AddNode(NodeParams node);

public:
	uint16	m_TestCount;										// ���Խڵ�����
	vector<NodeParams> m_arrTestNodes;							// ���Խڵ�����
};
