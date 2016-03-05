//*****************************************************************************
// �ļ��� : MemDC.h
// 
// ��Ҫ   : �ڴ��豸��������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-4       ��־��       �½�
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
	void CopyFromSrcDC(void);													// ��Դdc�ϵ�����copy�����ڴ�dc
	void CopyToSrcDC(void);														// �ѱ��ڴ�dc�ϵ�����copy��Դdc

protected:
	CBitmap m_bitmap;
	CBitmap* m_pOldBitMap;
	CDC* m_pSrcDC;
	CRect m_rect;
};
