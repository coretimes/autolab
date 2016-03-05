//*****************************************************************************
// �ļ��� : ReagentAreaData.h
// 
// ����   : �Լ�������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 1.���ɣ�
//	 �Լ���ʾ��ͼ�ؼ��ɱ��������ڵ�����15���ڵ㣩������������ɣ�ÿ���ڵ��������һ��uint32�����Լ�ID��
//   �ڵ�����ֵ������m_arrReagents[15]�У���Ӧ������(pos - 1)
//*****************************************************************************

#pragma once
#include "ReagentTable.h"

// �Լ������ݸ�����Ϣ����
// wparam: λ������
// lparam: NULL.
#define UM_ReagentArea_Update		WM_USER + 1000
#define  NUM_REAGENTPOS				15					// �Լ��������Լ�����

struct ReagentConfigNode
{
	Reagent_Pos pos;									// �Լ���λ�ñ��
	bool IsThereTube;									// �Ƿ����Թ�
	bool IsSetingReagent;								// �Ƿ��Ѿ������Լ���Ϣ
	Reagent_Key ReagentID;								// �Լ�ID
	bool IsHover;										// ��ǰ�ڵ��Ƿ��ڽ���״̬

	ReagentConfigNode()
	{
		pos = 0;
		IsThereTube = false;
		IsThereTube = false;
		IsSetingReagent = false;
		ReagentID = 0;
		IsHover = false;
	}
};
#pragma once

class CReagentAreaData
{
private:
	CReagentAreaData(void);

public:
	static CReagentAreaData * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CReagentAreaData();
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

	~CReagentAreaData(void);


	// --------------------------------------------------
	// ժҪ��
	//	   ȡ���Լ���������Ϣ�б�
	// ������
	//     1.ReagentConfigList - �����洢������Լ����ýڵ�����
	// --------------------------------------------------
	void GetReagentConfigList(map<Reagent_Pos,const ReagentConfigNode *> & mapReagents) const;

	// --------------------------------------------------
	// ժҪ��
	//	   �����Լ�λ�����ýڵ�
	// ������
	//     1.pos - �Լ�λ��
	// ���أ�
	//     2.�Լ����ýڵ�ָ��
	// --------------------------------------------------
	ReagentConfigNode * GetNode(const Reagent_Pos & pos)
	{	
		if(pos < 1 || pos > NUM_REAGENTPOS)
			return NULL;
		else
			return &m_ArrReagentPos[pos - 1];
	}

	// --------------------------------------------------
	// ժҪ��
	//	   �������ͼ
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
	};

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ի���ɢ�����ݸ��µ���Ϣ
	// ���أ�
	//     1.pos.���ݷ������µ��Լ�λ�ã����Ϊ0���������������
	// --------------------------------------------------
	void SpreadUpdateMessage(Reagent_Pos pos = 0);

private:
	static CReagentAreaData * m_pInstance;

private:
	ReagentConfigNode m_ArrReagentPos[NUM_REAGENTPOS];
	vector<CDialog *> m_AssociateDlgList ;							// �����Ի���ָ��
};
