//*****************************************************************************
// 文件名 : Packet.h
// 
// 概要   : 串口通信协议
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "Types.h"
#include "Protocol.h"
#include <vector>
using namespace std;

#define PROT_ADDRESS_CODE					0xa0				//地址码
#define PROT_ERROR_MASK						0x80				//错误掩码

// *******************************************************
// 数据包基类
// *******************************************************
class CPacket
{
public:
	CPacket(void);
	~CPacket(void){};

public:
	// ---------------------------------------------------------------------------
	// 摘要：
	//     生成通信用字节流
	// 参数：
	//     1.pBuffer - 目标缓存区指针
	//     2.BufferLength - 缓存区长度
	// ---------------------------------------------------------------------------
	bool CreatByteStream(char * pDestBuffer,const int &DestBufferLength);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得数据包需求长度
	// ---------------------------------------------------------------------------
	int GetBufferLength(){return sizeof(PacketHeader) + m_header.length;}

protected:

	struct PacketHeader{										// 数据包头
		uint8	address;										// 地址码
		uint8	command;										// 命令码
		uint16	length;											// 数据长度
	} m_header;

	char* m_pDataBuffer;										// 指向数据部分缓冲区指针
};

// *******************************************************
// 心跳包类
// *******************************************************
#define PROT_COMMAND_HEARTBEAT				0x65				// 心跳功能码
class CHeartBeatPacket : public CPacket
{
public:
	CHeartBeatPacket(void);
	~CHeartBeatPacket(void){};

public:
	struct HeartbeatData {										// 心跳数据包
		uint16	current_tick;									// 当前周期计数器　
		uint16	tick_count;										// 周期总数		　
		uint8	main_control_unit;								// 主控单元状态	　
		uint8	mechanical_arm;									// 机械臂状态		　
		uint8	measuring_area;									// 测量区状态		　
		uint8	material_area[5];								// 样本试剂区状态	　
		uint16	brood_chambe_temp;								// 育温区温度		　
		uint16	reagnet_area_temp;								// 试剂区温度	　
		uint16  incubation_area;								// 孵育区状态	　
	} m_Data;
};

// *******************************************************
// 读配置参数命令包
// *******************************************************
#define PROT_COMMAND_READPARAMS				0x45				// 读配置参数命令功能码
#define PROT_COMMAND_READPARAMS_ERROR		0xC5				// 读配置参数命令错误码
class CReadParamsPacket : public CPacket
{
public:
	CReadParamsPacket(void);
	~CReadParamsPacket(void){};

public:
	struct ReadParamsCmd {										// 读配置参数命令
		uint8	start_addr;										// 起始地址
		uint8	byte_count;										// 参数个数
	} m_Cmd;
};

// *******************************************************
// 下发实验数据命令包
// *******************************************************
#define PROT_COMMAND_SENDTEST				0x64				// 下发实验数据命令功能码
#define PROT_COMMAND_SENDTEST_ERROR			0xE4				// 下发实验数据命令错误码
class CSendTestPacket : public CPacket
{
public:
	struct NodeParams {											// 测试节点参数
		uint16	key;											// 节点索引
		uint8	is_urgent;										// 加急标志
		uint8	sample_pos;										// 样本位置编号
		uint8	sample_volume;									// 样本量
		uint8	reagent_pos;									// 试剂位置编号
		uint8	reagent_volume;									// 试剂量
		uint8	bead_volume;									// 磁珠量
		uint8	incubation_time;								// 温育时间
		uint8	wash_times;										// 清洗次数
	};

public:
	CSendTestPacket();
	~CSendTestPacket(void){};
	void AddNode(NodeParams node);

public:
	uint16	m_TestCount;										// 测试节点数量
	vector<NodeParams> m_arrTestNodes;							// 测试节点数组
};
