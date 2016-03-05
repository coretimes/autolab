//*****************************************************************************
// �ļ��� : MemDC.h
// 
// ��Ҫ   : �ڴ��豸�������� (�����ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-4       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#include "StdAfx.h"
#include "MemDC.h"

CMemDC::CMemDC(CDC* pSrcDC, const CRect& rect)
{
	ASSERT(pSrcDC != NULL);

	m_pSrcDC = pSrcDC;
	m_rect = rect;

	if(m_pSrcDC->IsPrinting())									// ����Ǵ�ӡ��dc
	{
		m_bPrinting = m_pSrcDC->m_bPrinting;
		m_hDC = m_pSrcDC->m_hDC;
		m_hAttribDC = m_pSrcDC->m_hAttribDC;
	}
	else														// ������Ǵ�ӡ��dc
	{
		CreateCompatibleDC(m_pSrcDC);							// ����ԴDC�ļ���DC
		if ( rect == CRect(0,0,0,0) )							// ������ľ���Ϊ�գ�����Χ�Ƽ������߽����Ӿ��εĳߴ���Ϣ					
			m_pSrcDC->GetClipBox(&m_rect);

		if(m_bitmap.CreateCompatibleBitmap(m_pSrcDC, m_rect.Width(), m_rect.Height()) == TRUE)
			m_pOldBitMap = SelectObject(&m_bitmap);
	}
}

CMemDC::~CMemDC(void)
{
	if(m_pSrcDC->IsPrinting())									// ����Ǵ�ӡ��dc
	{
		m_hDC = m_hAttribDC = NULL;
	}
	else														// ������Ǵ�ӡ��dc
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
