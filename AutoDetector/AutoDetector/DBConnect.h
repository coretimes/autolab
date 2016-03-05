//*****************************************************************************
// 文件名 : CDBConnect.h
// 
// 概要   : 数据库连接类,负责数据连接
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-7       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#pragma once
#include "AdoDatabase.h"
class CDBConnect
{
private:													// 单体模式，构造函数为私有
	CDBConnect(void);
public:
	static CDBConnect * GetInstance() 						// 取得实体指针
	{
		if(m_pInstance == NULL)
			m_pInstance = new CDBConnect();
		return m_pInstance;
	}

	// --------------------------------------------------
	// 摘要：
	//	   删除实体
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
