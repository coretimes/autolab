//*****************************************************************************
// 文件名 : CurveManagement.h
// 
// 概要   : 曲线管理器，管理由曲线数据库数据生成的曲线
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "CaliCurve.h"

class CCurveManagement
{
private:
	CCurveManagement(void);
public:
	~CCurveManagement(void);
	// --------------------------------------------------
	// 摘要：
	//	   取得单体模式对象实体指针函数
	// --------------------------------------------------
	static CCurveManagement * GetInstance();			

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

	// --------------------------------------------------
	// 摘要：
	//     根据指定曲线ID获取曲线指针
	// --------------------------------------------------
	CCaliCurve* GetCurve(uint32 CurveID);					

private:
	static CCurveManagement * m_pInstance;				// 单体指针
	map<UINT, CCaliCurve*> m_CurveList;					// 曲线表缓冲区
};
