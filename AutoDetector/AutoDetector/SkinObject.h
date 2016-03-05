//*****************************************************************************
// 文件名 : SkinObject.h
// 
// 类名   : 皮肤对象基类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "Resource.h"
class CSkinObject
{
public:
	CSkinObject();
	~CSkinObject(void);

	virtual void DrawSkin(const CRect & rcClipArea) = 0;

protected:
	CWnd * m_pWnd;
};
