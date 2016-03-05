//*****************************************************************************
// �ļ��� : AutoDetectorSkin.h
// 
// ����   : ���Ի���Ƥ�����ƣ���������Ƥ��ʱ��ֻ����ݴ�����չ
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "skinobject.h"
#include <atlimage.h>
#include "BaseDialog.h"
class CAutoDetectorSkin :	public CSkinObject
{
public:
	CAutoDetectorSkin(CBaseDialog * pDlg);
	~CAutoDetectorSkin(void);
	
	virtual void DrawSkin(const CRect &rcClipArea);
private:
	CImage m_BannerLeft;				// ������������ͼƬ
	CImage m_BannerRight;				// ���������Һ��ͼƬ
	CImage m_BannerMiddle;				// ���������м���ͼƬ

	CImage m_MaxButtonNomal;			// ��󻯰�ť��ͨ״̬ͼƬ
	CImage m_MaxButtonHot;				// ��󻯰�ť�ȵ�״̬ͼƬ
	CImage m_MaxButtonPress;			// ��󻯰�ť����״̬ͼƬ
	CImage m_MinButtonNomal;			// ��С����ť��ͨ״̬ͼƬ
	CImage m_MinButtonHot;				// ��С����ť�ȵ�״̬ͼƬ
	CImage m_MinButtonPress;			// ��С����ť����״̬ͼƬ
	CImage m_CloseButtonNomal;			// �رհ�ť��ͨ״̬ͼƬ
	CImage m_CloseButtonPress;			// �رհ�ť����״̬ͼƬ
	CImage m_CloseButtonHot;			// �رհ�ť�ȵ�״̬ͼƬ
	CImage m_ResButtonNomal;			// ��ԭ��ť��ͨ״̬ͼƬ
	CImage m_ResButtonPress;			// ��ԭ��ť����״̬ͼƬ
	CImage m_ResButtonHot;				// ��ԭ��ť�ȵ�״̬ͼƬ
	CImage m_SettingButtonNomal;		// ���ð�ť��ͨ״̬ͼƬ
	CImage m_SettingButtonPress;		// ���ð�ť����״̬ͼƬ
	CImage m_SettingButtonHot;			// ���ð�ť�ȵ�״̬ͼƬ

	CBaseDialog * m_pDialog;			// �Ի���
private:
	void DrawButtons(CDC*pDC);
};
