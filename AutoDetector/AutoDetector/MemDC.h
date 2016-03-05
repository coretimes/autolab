//*****************************************************************************
// 文件名 : MemDC.h
// 
// 概要   : 内存设备上下文类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-4       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "afxwin.h"

class CMemDC :public CDC
{
public:
	CMemDC(CDC* pSrcDC, const CRect& rect = CRect(0,0,0,0));
public:
	~CMemDC(void);
public:
	void CopyFromSrcDC(void);													// 把源dc上的像素copy到本内存dc
	void CopyToSrcDC(void);														// 把本内存dc上的像素copy到源dc

protected:
	CBitmap m_bitmap;
	CBitmap* m_pOldBitMap;
	CDC* m_pSrcDC;
	CRect m_rect;
};
