//*****************************************************************************
// �ļ��� : CDBConnect.h
// 
// ��Ҫ   : ���ݿ�������,������������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-7       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "AdoDatabase.h"
class CDBConnect
{
private:													// ����ģʽ�����캯��Ϊ˽��
	CDBConnect(void);
public:
	static CDBConnect * GetInstance() 						// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CDBConnect();
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

	~CDBConnect(void);

public:
	CAdoDatabase m_db;										

private:
	static CDBConnect * m_pInstance;
};
