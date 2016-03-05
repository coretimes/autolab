#include "StdAfx.h"
#include "Protocol.h"

// *******************************************************
// 数据包基类
// *******************************************************
CPacket::CPacket(void)
{
	m_pDataBuffer = NULL;
	m_header.address = PROT_ADDRESS_CODE;
}

bool CPacket::CreatByteStream(char * pDestBuffer,const int& DestBufferLength)
{
	if(m_pDataBuffer == NULL)
	{
		ASSERT(0);
		return false;
	}
	if(pDestBuffer == NULL)
	{
		ASSERT(0);
		return false;
	}

	int HeaderLength = sizeof(PacketHeader);
	if(DestBufferLength < m_header.length + HeaderLength)
	{	
		ASSERT(0);
		return false;
	}
	memset(pDestBuffer,0,DestBufferLength);
	memcpy(pDestBuffer,&m_header,HeaderLength);							// 把命令包头部拷贝到目标缓冲区
	memcpy(pDestBuffer + HeaderLength,m_pDataBuffer,m_header.length);	// 把命令包数据区域拷贝到目标缓冲区
	return true;
}

// *******************************************************
// 心跳包类
// *******************************************************
CHeartBeatPacket::CHeartBeatPacket(void)
{
	m_header.command = PROT_COMMAND_HEARTBEAT;
	m_header.length = sizeof(HeartbeatData);							// 数据长度赋值
	m_pDataBuffer = (char*)&m_Data;										// 心跳包数据区域指针直接指向m_Data;
}

// *******************************************************
//  读配置参数命令包类
// *******************************************************
CReadParamsPacket::CReadParamsPacket(void)
{
	m_header.command = PROT_COMMAND_READPARAMS;
	m_header.length = sizeof(ReadParamsCmd);
	m_pDataBuffer = (char*)&m_Cmd;										// 命令包数据区域指针直接指向m_Cmd;
}

// *******************************************************
// 下发实验数据命令包
// *******************************************************
CSendTestPacket::CSendTestPacket(void)
{
	m_header.command = PROT_COMMAND_SENDTEST;
	m_header.length = 0;
	m_pDataBuffer = NULL;												// 数据区域指针暂时赋值为空，当加入新节点时，重新分配空间
	m_TestCount = 0;
}

void CSendTestPacket::AddNode(NodeParams node)
{
	// 由于数据区域长度发生变化，所以需要重新分配空间
	if(m_pDataBuffer)
		delete m_pDataBuffer;
	m_arrTestNodes.push_back(node);
	m_TestCount++;
	m_header.length = sizeof(m_TestCount) + m_TestCount * sizeof(NodeParams);

	m_pDataBuffer = new char[m_header.length];
	memset(m_pDataBuffer,0,m_header.length);							
	memcpy(m_pDataBuffer,&m_TestCount,sizeof(m_TestCount));				// 把节点个数拷贝到数据缓冲区
	memcpy(m_pDataBuffer + sizeof(m_TestCount),(char*)&m_arrTestNodes[0],m_TestCount * sizeof(NodeParams));
																		// 把节点数据拷贝到数据缓冲区
}
