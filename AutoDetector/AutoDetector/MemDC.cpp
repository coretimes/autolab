//*****************************************************************************
// 文件名 : MemDC.h
// 
// 概要   : 内存设备上下文类 (定义文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-4       崔志雷       新建
// ------------------------------------------
//*****************************************************************************

#include "StdAfx.h"
#include "MemDC.h"

CMemDC::CMemDC(CDC* pSrcDC, const CRect& rect)
{
	ASSERT(pSrcDC != NULL);

	m_pSrcDC = pSrcDC;
	m_rect = rect;

	if(m_pSrcDC->IsPrinting())									// 如果是打印机dc
	{
		m_bPrinting = m_pSrcDC->m_bPrinting;
		m_hDC = m_pSrcDC->m_hDC;
		m_hAttribDC = m_pSrcDC->m_hAttribDC;
	}
	else														// 如果不是打印机dc
	{
		CreateCompatibleDC(m_pSrcDC);							// 创建源DC的兼容DC
		if ( rect == CRect(0,0,0,0) )							// 如果给的矩形为空，则获得围绕剪切区边界的外接矩形的尺寸信息					
			m_pSrcDC->GetClipBox(&m_rect);

		if(m_bitmap.CreateCompatibleBitmap(m_pSrcDC, m_rect.Width(), m_rect.Height()) == TRUE)
			m_pOldBitMap = SelectObject(&m_bitmap);
	}
}

CMemDC::~CMemDC(void)
{
	if(m_pSrcDC->IsPrinting())									// 如果是打印机dc
	{
		m_hDC = m_hAttribDC = NULL;
	}
	else														// 如果不是打印机dc
	{
		if(m_pOldBitMap != NULL)
			SelectObject(m_pOldBitMap);
		DeleteObject(&m_bitmap);
	}
}

void CMemDC::CopyFromSrcDC(void)
{
	BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pSrcDC, m_rect.left, m_rect.top, SRCCOPY);
}


void CMemDC::CopyToSrcDC(void)
{
	m_pSrcDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),this, m_rect.left, m_rect.top, SRCCOPY);
}
