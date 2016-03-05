#include "StdAfx.h"
#include "Protocol.h"

// *******************************************************
// ���ݰ�����
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
	memcpy(pDestBuffer,&m_header,HeaderLength);							// �������ͷ��������Ŀ�껺����
	memcpy(pDestBuffer + HeaderLength,m_pDataBuffer,m_header.length);	// ��������������򿽱���Ŀ�껺����
	return true;
}

// *******************************************************
// ��������
// *******************************************************
CHeartBeatPacket::CHeartBeatPacket(void)
{
	m_header.command = PROT_COMMAND_HEARTBEAT;
	m_header.length = sizeof(HeartbeatData);							// ���ݳ��ȸ�ֵ
	m_pDataBuffer = (char*)&m_Data;										// ��������������ָ��ֱ��ָ��m_Data;
}

// *******************************************************
//  �����ò����������
// *******************************************************
CReadParamsPacket::CReadParamsPacket(void)
{
	m_header.command = PROT_COMMAND_READPARAMS;
	m_header.length = sizeof(ReadParamsCmd);
	m_pDataBuffer = (char*)&m_Cmd;										// �������������ָ��ֱ��ָ��m_Cmd;
}

// *******************************************************
// �·�ʵ�����������
// *******************************************************
CSendTestPacket::CSendTestPacket(void)
{
	m_header.command = PROT_COMMAND_SENDTEST;
	m_header.length = 0;
	m_pDataBuffer = NULL;												// ��������ָ����ʱ��ֵΪ�գ��������½ڵ�ʱ�����·���ռ�
	m_TestCount = 0;
}

void CSendTestPacket::AddNode(NodeParams node)
{
	// �����������򳤶ȷ����仯��������Ҫ���·���ռ�
	if(m_pDataBuffer)
		delete m_pDataBuffer;
	m_arrTestNodes.push_back(node);
	m_TestCount++;
	m_header.length = sizeof(m_TestCount) + m_TestCount * sizeof(NodeParams);

	m_pDataBuffer = new char[m_header.length];
	memset(m_pDataBuffer,0,m_header.length);							
	memcpy(m_pDataBuffer,&m_TestCount,sizeof(m_TestCount));				// �ѽڵ�������������ݻ�����
	memcpy(m_pDataBuffer + sizeof(m_TestCount),(char*)&m_arrTestNodes[0],m_TestCount * sizeof(NodeParams));
																		// �ѽڵ����ݿ��������ݻ�����
}
