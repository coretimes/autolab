#include "StdAfx.h"
#include "AutoDetectorSkin.h"
#include "GeneralFuction.h"
#include "MemDC.h"

CAutoDetectorSkin::CAutoDetectorSkin(CBaseDialog * pDlg)
{
	m_pDialog = pDlg;
	// ���ڱ߿��Ⱥ͸߶�
	CGeneralFuction * pGenFunc = CGeneralFuction::GetInstance();
	pGenFunc->LoadImageFromResource(m_BannerLeft,IDR_Banner_Left,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_BannerRight,IDR_Banner_Right,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_BannerMiddle,IDR_Banner_Middle,_T("PNG"));

	pGenFunc->LoadImageFromResource(m_MaxButtonNomal,IDR_MaxNormal,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_MaxButtonPress,IDR_MaxPress,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_MaxButtonHot,IDR_MaxHot,_T("PNG"));

	pGenFunc->LoadImageFromResource(m_MinButtonNomal,IDR_MinNormal,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_MinButtonPress,IDR_MinPress,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_MinButtonHot,IDR_MinHot,_T("PNG"));

	pGenFunc->LoadImageFromResource(m_ResButtonNomal,IDR_ResNormal,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_ResButtonHot,IDR_ResHot,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_ResButtonPress,IDR_ResPress,_T("PNG"));

	pGenFunc->LoadImageFromResource(m_SettingButtonNomal,IDR_SetingNormal,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_SettingButtonHot,IDR_SettingHot,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_SettingButtonPress,IDR_SettingPress,_T("PNG"));

	pGenFunc->LoadImageFromResource(m_CloseButtonNomal,IDR_CloseNormal,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_CloseButtonHot,IDR_CloseHot,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_CloseButtonPress,IDR_ClosePress,_T("PNG"));
}

CAutoDetectorSkin::~CAutoDetectorSkin(void)
{
}

void CAutoDetectorSkin::DrawSkin(const CRect &rcClipArea)
{
	CRect rcWnd = m_pDialog->GetWindSizeRect();

	CDC *pDC = m_pDialog->GetWindowDC();
	CMemDC memDC(pDC,rcClipArea);
	memDC.CopyFromSrcDC();
	//��������
	int LeftStartX = 0,												// ��ߵĺ����ʼλ�õ�x����
		MiddleStartX = m_BannerLeft.GetWidth(),						// �м�ĺ����ʼλ�õ�x����
		RightStartX = rcWnd.right - m_BannerRight.GetWidth();		// �ұߵĺ����ʼλ�õ�x����

	 //���Һ��
	m_BannerRight.Draw( memDC.m_hDC,								// �豸����			
		RightStartX,												// Ŀ��x����
		0,															// Ŀ��y����
		m_BannerRight.GetWidth(),									// Ŀ����		
		m_pDialog->GetCaptionHeight(),								// Ŀ�곤�� 	
		0,															// Դx����	
		0,															// Դy����
		m_BannerRight.GetWidth(),									// Դ���
		m_pDialog->GetCaptionHeight());								// Դ����
	// ������
	m_BannerLeft.Draw( memDC.m_hDC,									// �豸����
		LeftStartX,													// Ŀ��x����
		0,															// Ŀ��y����
		MiddleStartX - LeftStartX,									// Ŀ����
		m_pDialog->GetCaptionHeight(),								// Ŀ�곤��
		0,															// Դx����	
		0,															// Դy����
		MiddleStartX - LeftStartX,									// Դ���
		m_pDialog->GetCaptionHeight());								// Դ����
	// ����м���
	if(RightStartX > MiddleStartX)
		m_BannerMiddle.Draw( memDC.m_hDC,							// �豸����
		MiddleStartX,												// Ŀ��x����
		0,															// Ŀ��y����
		RightStartX - MiddleStartX,									// Ŀ����
		m_pDialog->GetCaptionHeight(),								// Ŀ�곤��
		0,															// Դx����	
		0,															// Դy����
		m_BannerMiddle.GetWidth(),									// Դ���
		m_pDialog->GetCaptionHeight());								// Դ����

	// ���߿�����
	// �߿�ˢ�뱳��ͬΪ��ɫ��ʹ�������߿�
	CBrush brWhite(RGB(255,255,255));

	// ��߿�
	CRect rcLeftBorder = m_pDialog->GetLeftBorderRect();
	CRect rcLeftBorderCaption = rcLeftBorder;						// ��ǩ��ߵı߿򱳾�ΪͼƬ
	rcLeftBorderCaption.bottom = rcLeftBorderCaption.top + 33 - 4;	// 33Ϊ��ǩ�߶�

	// ����߿��϶˵ĺ������
	m_BannerLeft.Draw( memDC.m_hDC,									// �豸����
		rcLeftBorderCaption.left,									// Ŀ��x����
		rcLeftBorderCaption.top,									// Ŀ��y����
		rcLeftBorderCaption.Width(),								// Ŀ����
		rcLeftBorderCaption.Height(),								// Ŀ�곤��
		0,															// Դx����	
		m_pDialog->GetCaptionHeight(),								// Դy����
		rcLeftBorderCaption.Width(),								// Դ���
		rcLeftBorderCaption.Height());								// Դ����


	rcLeftBorder.top = rcLeftBorderCaption.bottom;
	memDC.FillRect(rcLeftBorder,&brWhite);

	// �ұ߿�
	CRect rcRightBorder = m_pDialog->GetRightBorderRect();
	CRect rcRightBorderCaption = rcRightBorder;							// ��ǩ�ұߵı߿򱳾�ΪͼƬ
	rcRightBorderCaption.bottom = rcRightBorderCaption.top +  33 - 4;	// 33Ϊ��ǩ�߶�
	// ���ɱ߿��϶˵ĺ������
	m_BannerRight.Draw( memDC.m_hDC,								// �豸����
		rcRightBorderCaption.left,									// Ŀ��x����
		rcRightBorderCaption.top,									// Ŀ��y����
		rcRightBorderCaption.Width(),								// Ŀ����
		rcRightBorderCaption.Height(),								// Ŀ�곤��
		m_BannerRight.GetWidth() - rcRightBorderCaption.Width(),	// Դx����	
		m_pDialog->GetCaptionHeight(),								// Դy����
		rcRightBorderCaption.Width(),								// Դ���
		rcRightBorderCaption.Height());								// Դ����

	rcRightBorder.top = rcRightBorderCaption.bottom;
	memDC.FillRect(rcRightBorder,&brWhite);

	memDC.FillRect(m_pDialog->GetBottomBorderRect(),&brWhite);

	// �����ڱ߽���
	CBrush brBorder(RGB(100,100,100));
	memDC.SelectObject(brBorder);
	memDC.FrameRect(m_pDialog->GetWindSizeRect(),&brBorder);

	// ���ư�ť
	DrawButtons(&memDC);

	memDC.CopyToSrcDC();
	m_pDialog->ReleaseDC(pDC);
}
void CAutoDetectorSkin::DrawButtons(CDC * pDC)
{
	CImage &ImageMaxButton = m_MaxButtonNomal;
	CImage &ImageMinButton = m_MinButtonNomal;
	CImage &ImageCloseButton = m_CloseButtonNomal;
	CImage &ImageSettingButton = m_SettingButtonNomal;

	// ��󻯻���ť����״̬ʱ
	if(m_pDialog->GetMaxButtonState() == bsPress)
		if(m_pDialog->IsZoomed())
			m_ResButtonPress.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_ResButtonPress.GetWidth(),
			m_ResButtonPress.GetHeight(),
			0,
			0,
			m_ResButtonPress.GetWidth(),
			m_ResButtonPress.GetHeight(),
			0xFFFFFF);	
		else
			m_MaxButtonPress.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_MaxButtonPress.GetWidth(),
			m_MaxButtonPress.GetHeight(),
			0,
			0,
			m_MaxButtonPress.GetWidth(),
			m_MaxButtonPress.GetHeight(),
			0xFFFFFF);	

	// ��󻯻���ť�ȵ�״̬ʱ
	if(m_pDialog->GetMaxButtonState() == bsHot)
		if(m_pDialog->IsZoomed())
			m_ResButtonHot.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_ResButtonHot.GetWidth(),
			m_ResButtonHot.GetHeight(),
			0,
			0,
			m_ResButtonHot.GetWidth(),
			m_ResButtonHot.GetHeight(),
			0xFFFFFF);	
		else
			m_MaxButtonHot.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_MaxButtonHot.GetWidth(),
			m_MaxButtonHot.GetHeight(),
			0,
			0,
			m_MaxButtonHot.GetWidth(),
			m_MaxButtonHot.GetHeight(),
			0xFFFFFF);	

	// ��󻯻���ť��ͨ״̬ʱ
	if(m_pDialog->GetMaxButtonState() == bsNormal)
		if(m_pDialog->IsZoomed())
			m_ResButtonNomal.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_ResButtonNomal.GetWidth(),
			m_ResButtonNomal.GetHeight(),
			0,
			0,
			m_ResButtonNomal.GetWidth(),
			m_ResButtonNomal.GetHeight(),
			0xFFFFFF);	
		else
			m_MaxButtonNomal.TransparentBlt( pDC->m_hDC,
			m_pDialog->GetMaxButtonRect().left,
			m_pDialog->GetMaxButtonRect().top,
			m_MaxButtonNomal.GetWidth(),
			m_MaxButtonNomal.GetHeight(),
			0,
			0,
			m_MaxButtonNomal.GetWidth(),
			m_MaxButtonNomal.GetHeight(),
			0xFFFFFF);	

	// ��С����ť����״̬ʱ
	if(m_pDialog->GetMinButtonState() == bsPress)
		m_MinButtonPress.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetMinButtonRect().left,
		m_pDialog->GetMinButtonRect().top,
		m_MinButtonPress.GetWidth(),
		m_MinButtonPress.GetHeight(),
		0,
		0,
		m_MinButtonPress.GetWidth(),
		m_MinButtonPress.GetHeight(),
		0xFFFFFF);	

	// ��С����ť�ȵ�״̬ʱ
	if(m_pDialog->GetMinButtonState() == bsHot)
		m_MinButtonHot.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetMinButtonRect().left,
		m_pDialog->GetMinButtonRect().top,
		m_MinButtonHot.GetWidth(),
		m_MinButtonHot.GetHeight(),
		0,
		0,
		m_MinButtonHot.GetWidth(),
		m_MinButtonHot.GetHeight(),
		0xFFFFFF);	

	// ��С����ť��ͨ״̬ʱ
	if(m_pDialog->GetMinButtonState() == bsNormal)
		m_MinButtonNomal.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetMinButtonRect().left,
		m_pDialog->GetMinButtonRect().top,
		m_MinButtonNomal.GetWidth(),
		m_MinButtonNomal.GetHeight(),
		0,
		0,
		m_MinButtonNomal.GetWidth(),
		m_MinButtonNomal.GetHeight(),
		0xFFFFFF);			// Դ����

	// ���ð�ť
	if(m_pDialog->GetSettingButtonState() == bsPress)
		m_SettingButtonPress.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetSettingButtonRect().left,
		m_pDialog->GetSettingButtonRect().top,
		m_SettingButtonPress.GetWidth(),
		m_SettingButtonPress.GetHeight(),
		0,
		0,
		m_SettingButtonPress.GetWidth(),
		m_SettingButtonPress.GetHeight(),
		0xFFFFFF);			// Դ����

	if(m_pDialog->GetSettingButtonState() == bsHot)
		m_SettingButtonHot.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetSettingButtonRect().left,
		m_pDialog->GetSettingButtonRect().top,
		m_SettingButtonHot.GetWidth(),
		m_SettingButtonHot.GetHeight(),
		0,
		0,
		m_SettingButtonHot.GetWidth(),
		m_SettingButtonHot.GetHeight(),
		0xFFFFFF);			// Դ����

	if(m_pDialog->GetSettingButtonState() == bsNormal)
		m_SettingButtonNomal.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetSettingButtonRect().left,
		m_pDialog->GetSettingButtonRect().top,
		m_SettingButtonNomal.GetWidth(),
		m_SettingButtonNomal.GetHeight(),
		0,
		0,
		m_SettingButtonNomal.GetWidth(),
		m_SettingButtonNomal.GetHeight(),
		0xFFFFFF);			// Դ����

	// �رհ�ť
	if(m_pDialog->GetCloseButtonState() == bsPress)
		m_CloseButtonPress.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetCloseButtonRect().left,
		m_pDialog->GetCloseButtonRect().top,
		m_CloseButtonPress.GetWidth(),
		m_CloseButtonPress.GetHeight(),
		0,
		0,
		m_SettingButtonNomal.GetWidth(),
		m_SettingButtonNomal.GetHeight(),
		0xFFFFFF);			// Դ����

	if(m_pDialog->GetCloseButtonState() == bsHot)
		m_CloseButtonHot.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetCloseButtonRect().left,
		m_pDialog->GetCloseButtonRect().top,
		m_CloseButtonHot.GetWidth(),
		m_CloseButtonHot.GetHeight(),
		0,
		0,
		m_CloseButtonHot.GetWidth(),
		m_CloseButtonHot.GetHeight(),
		0xFFFFFF);			// Դ����

	if(m_pDialog->GetCloseButtonState() == bsNormal)
		m_CloseButtonNomal.TransparentBlt( pDC->m_hDC,
		m_pDialog->GetCloseButtonRect().left,
		m_pDialog->GetCloseButtonRect().top,
		m_CloseButtonNomal.GetWidth(),
		m_CloseButtonNomal.GetHeight(),
		0,
		0,
		m_CloseButtonNomal.GetWidth(),
		m_CloseButtonNomal.GetHeight(),
		0xFFFFFF);			// Դ����
}