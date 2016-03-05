#include "StdAfx.h"
#include "MainTabSkin.h"
#include "GeneralFuction.h"
#include "MemDC.h"
#include "FontLib.h"

CMainTabSkin::CMainTabSkin(CMainTab * pTabCtrl)
{
	m_pTabCtrl = pTabCtrl;
	// 窗口边框宽度和高度
	CGeneralFuction * pGenFunc = CGeneralFuction::GetInstance();
	pGenFunc->LoadImageFromResource(m_BannerLeft,IDR_Banner_Left,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_BannerRight,IDR_Banner_Right,_T("PNG"));
	pGenFunc->LoadImageFromResource(m_BannerMiddle,IDR_Banner_Middle,_T("PNG"));
}

CMainTabSkin::~CMainTabSkin(void)
{
}

void CMainTabSkin::DrawSkin(const CRect &rcClipArea)
{
	// 注意：若皮肤动态库中的OnPaint函数被调用，则此函数将不再被调用

	CPaintDC dc(m_pTabCtrl);										// 取得源环境DC

	CMemDC memDC(&dc,m_pTabCtrl->GetClienRect());					// 创建兼容的内存DC
	memDC.CopyFromSrcDC();											// 把源DC内容复制到内存DC上

	// 绘制头部背景
	CRect rcHeader = m_pTabCtrl->GetClienRect();
	rcHeader.bottom = rcHeader.top + m_pTabCtrl->GetHeaderHeight();

	//填充标题栏
	int LeftStartX = 0,												// 左边的横幅开始位置的x坐标
		MiddleStartX = m_BannerLeft.GetWidth() - 4,					// 中间的横幅开始位置的x坐标
		RightStartX = m_pTabCtrl->GetClienRect().Width() - (m_BannerRight.GetWidth() - 4);
																	// 右边的横幅开始位置的x坐标,4是右边界框宽度

	//画右横幅
	m_BannerRight.Draw( memDC.m_hDC,								// 设备环境			
		RightStartX,												// 目标x坐标
		0,															// 目标y坐标
		m_BannerRight.GetWidth() - 4,								// 目标宽度,4是右边界框宽度		
		m_pTabCtrl->GetHeaderHeight(),								// 目标长度 	
		0,															// 源x坐标	
		66,															// 源y坐标,66是标题栏高度
		m_BannerRight.GetWidth() - 4,								// 源宽度,4是右边界框宽度
		m_pTabCtrl->GetHeaderHeight());								// 源长度

	// 画左横幅
	m_BannerLeft.Draw(memDC.m_hDC,									// 设备环境
		LeftStartX,													// 目标x坐标
		0,															// 目标y坐标
		MiddleStartX - LeftStartX,									// 目标宽度
		m_pTabCtrl->GetHeaderHeight(),								// 目标长度 	
		4,															// 源x坐标	
		66,															// 源y坐标,66是标题栏高度
		MiddleStartX - LeftStartX,									// 源宽度
		m_pTabCtrl->GetHeaderHeight());								// 目标长度 

	// 填充中间横幅
	if(RightStartX > MiddleStartX)
		m_BannerMiddle.Draw( memDC.m_hDC,							// 设备环境
		MiddleStartX,												// 目标x坐标
		0,															// 目标y坐标
		RightStartX - MiddleStartX,									// 目标宽度
		m_pTabCtrl->GetHeaderHeight(),								// 目标长度 	
		0,															// 源x坐标	
		66,															// 源y坐标,66是标题栏高度
		m_BannerMiddle.GetWidth(),									// 源宽度
		m_pTabCtrl->GetHeaderHeight());								// 目标长度 	


	// 定义绘制元素变量
	DRAWITEMSTRUCT drawItem;
	drawItem.CtlType = ODT_TAB;
	drawItem.CtlID = GetDlgCtrlID(m_pTabCtrl->m_hWnd);
	drawItem.hwndItem = m_pTabCtrl->m_hWnd;
	drawItem.hDC = memDC.GetSafeHdc();
	drawItem.itemAction = ODA_DRAWENTIRE;
	int nItemCount = m_pTabCtrl->GetItemCount();

	// 如果标签项数量小于1，则不绘制
	if (nItemCount < 1)
		return;

	// 当前选中项索引取得
	int nCurSel = m_pTabCtrl->GetCurSel();

	// 遍历所有标签项，分别调用DrawItem函数进行绘制
	for(int i=0; i < m_pTabCtrl->GetItemCount(); i++)
	{
		drawItem.itemID = i;							// 索引赋值
		if (i == nCurSel)								// 当前选中状态判断
			drawItem.itemState = ODS_SELECTED;
		else
			drawItem.itemState = 0;
		m_pTabCtrl->GetItemRect(i, &drawItem.rcItem);
		m_pTabCtrl->DrawItem(&drawItem);
	}

	memDC.CopyToSrcDC();								// 把内存DC上的内容复制到源DC上
}

void CMainTabSkin::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rcItem(lpDrawItemStruct->rcItem);			//获取项目区域
	int nIndex = lpDrawItemStruct->itemID;			//获取项目索引
	int nState = lpDrawItemStruct->itemState;		//获取项目状态

	TCITEM tcItem;
	TCHAR strLabel[MAX_PATH] = {0};	
	tcItem.mask        = TCIF_TEXT;
	tcItem.pszText     = strLabel;					// 标签标题初始化
	tcItem.cchTextMax  = MAX_PATH;
	m_pTabCtrl->GetItem(nIndex, &tcItem);
	dc.SetBkMode(TRANSPARENT);

	// 选择标签标题的字体和颜色
	CFontsLib * pFontLib = CFontsLib::GetInstance();
	CFont * pFont = pFontLib->GetFont(FontTabs);
	dc.SelectObject(pFont);
	dc.SetTextColor(RGB(102,102,102));				// 字体颜色为灰色

	if (nState & ODS_SELECTED)						//处于选中状态
	{
		CPen penGray(PS_SOLID,1,RGB(171,171,171));
		CBrush brWhite(RGB(255,255,255));
		dc.SelectObject(&penGray);
		dc.FillRect(&rcItem,&brWhite);
		dc.MoveTo(rcItem.left,rcItem.bottom - 4);
		dc.LineTo(rcItem.left,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.bottom - 4);
	}
	dc.DrawText(strLabel, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc.Detach();
}
