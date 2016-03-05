#include "StdAfx.h"
#include "AutoDetectorSkin.h"
#include "GeneralFuction.h"
#include "MemDC.h"

CAutoDetectorSkin::CAutoDetectorSkin(CBaseDialog * pDlg)
{
	m_pDialog = pDlg;
	// 窗口边框宽度和高度
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
	//填充标题栏
	int LeftStartX = 0,												// 左边的横幅开始位置的x坐标
		MiddleStartX = m_BannerLeft.GetWidth(),						// 中间的横幅开始位置的x坐标
		RightStartX = rcWnd.right - m_BannerRight.GetWidth();		// 右边的横幅开始位置的x坐标

	 //画右横幅
	m_BannerRight.Draw( memDC.m_hDC,								// 设备环境			
		RightStartX,												// 目标x坐标
		0,															// 目标y坐标
		m_BannerRight.GetWidth(),									// 目标宽度		
		m_pDialog->GetCaptionHeight(),								// 目标长度 	
		0,															// 源x坐标	
		0,															// 源y坐标
		m_BannerRight.GetWidth(),									// 源宽度
		m_pDialog->GetCaptionHeight());								// 源长度
	// 画左横幅
	m_BannerLeft.Draw( memDC.m_hDC,									// 设备环境
		LeftStartX,													// 目标x坐标
		0,															// 目标y坐标
		MiddleStartX - LeftStartX,									// 目标宽度
		m_pDialog->GetCaptionHeight(),								// 目标长度
		0,															// 源x坐标	
		0,															// 源y坐标
		MiddleStartX - LeftStartX,									// 源宽度
		m_pDialog->GetCaptionHeight());								// 源长度
	// 填充中间横幅
	if(RightStartX > MiddleStartX)
		m_BannerMiddle.Draw( memDC.m_hDC,							// 设备环境
		MiddleStartX,												// 目标x坐标
		0,															// 目标y坐标
		RightStartX - MiddleStartX,									// 目标宽度
		m_pDialog->GetCaptionHeight(),								// 目标长度
		0,															// 源x坐标	
		0,															// 源y坐标
		m_BannerMiddle.GetWidth(),									// 源宽度
		m_pDialog->GetCaptionHeight());								// 源长度

	// 画边框区域
	// 边框画刷与背景同为白色，使看不出边框
	CBrush brWhite(RGB(255,255,255));

	// 左边框
	CRect rcLeftBorder = m_pDialog->GetLeftBorderRect();
	CRect rcLeftBorderCaption = rcLeftBorder;						// 标签左边的边框背景为图片
	rcLeftBorderCaption.bottom = rcLeftBorderCaption.top + 33 - 4;	// 33为标签高度

	// 画左边框上端的横幅背景
	m_BannerLeft.Draw( memDC.m_hDC,									// 设备环境
		rcLeftBorderCaption.left,									// 目标x坐标
		rcLeftBorderCaption.top,									// 目标y坐标
		rcLeftBorderCaption.Width(),								// 目标宽度
		rcLeftBorderCaption.Height(),								// 目标长度
		0,															// 源x坐标	
		m_pDialog->GetCaptionHeight(),								// 源y坐标
		rcLeftBorderCaption.Width(),								// 源宽度
		rcLeftBorderCaption.Height());								// 源长度


	rcLeftBorder.top = rcLeftBorderCaption.bottom;
	memDC.FillRect(rcLeftBorder,&brWhite);

	// 右边框
	CRect rcRightBorder = m_pDialog->GetRightBorderRect();
	CRect rcRightBorderCaption = rcRightBorder;							// 标签右边的边框背景为图片
	rcRightBorderCaption.bottom = rcRightBorderCaption.top +  33 - 4;	// 33为标签高度
	// 画由边框上端的横幅背景
	m_BannerRight.Draw( memDC.m_hDC,								// 设备环境
		rcRightBorderCaption.left,									// 目标x坐标
		rcRightBorderCaption.top,									// 目标y坐标
		rcRightBorderCaption.Width(),								// 目标宽度
		rcRightBorderCaption.Height(),								// 目标长度
		m_BannerRight.GetWidth() - rcRightBorderCaption.Width(),	// 源x坐标	
		m_pDialog->GetCaptionHeight(),								// 源y坐标
		rcRightBorderCaption.Width(),								// 源宽度
		rcRightBorderCaption.Height());								// 源长度

	rcRightBorder.top = rcRightBorderCaption.bottom;
	memDC.FillRect(rcRightBorder,&brWhite);

	memDC.FillRect(m_pDialog->GetBottomBorderRect(),&brWhite);

	// 画窗口边界线
	CBrush brBorder(RGB(100,100,100));
	memDC.SelectObject(brBorder);
	memDC.FrameRect(m_pDialog->GetWindSizeRect(),&brBorder);

	// 绘制按钮
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

	// 最大化化按钮按下状态时
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

	// 最大化化按钮热点状态时
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

	// 最大化化按钮普通状态时
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

	// 最小化按钮按下状态时
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

	// 最小化按钮热点状态时
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

	// 最小化按钮普通状态时
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
		0xFFFFFF);			// 源长度

	// 设置按钮
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
		0xFFFFFF);			// 源长度

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
		0xFFFFFF);			// 源长度

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
		0xFFFFFF);			// 源长度

	// 关闭按钮
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
		0xFFFFFF);			// 源长度

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
		0xFFFFFF);			// 源长度

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
		0xFFFFFF);			// 源长度
}