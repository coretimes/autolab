// TestManTab.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "TestManTab.h"
#include "FontLib.h"
#include "MemDC.h"

// CTestManTab

IMPLEMENT_DYNAMIC(CTestManTab, CTabCtrl)

CTestManTab::CTestManTab()
{
	m_nHoverItem = -1;
	m_nLastHover = -1;
}

CTestManTab::~CTestManTab()
{
}


BEGIN_MESSAGE_MAP(CTestManTab, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

// CTestManTab 消息处理程序

void CTestManTab::PreSubclassWindow()
{
	ModifyStyle(0,TCS_VERTICAL);
	m_TabHeight = 60;
	m_TabWidth = 130;
	SetMinTabWidth(80);												//设置最小选项卡的宽度
	SetItemSize(CSize(m_TabHeight,m_TabWidth));
	CTabCtrl::PreSubclassWindow();
}

void CTestManTab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);

	CMemDC memDC(&dc,rect)						;					// 创建兼容的内存DC
	memDC.CopyFromSrcDC();											// 把源DC内容复制到内存DC上

	// 绘制头部背景
	CRect rcHeader = rect;
	rcHeader.right = rcHeader.left + m_TabWidth;
	

	//填充标题栏
	CBrush brBackTiltle(RGB(244,244,244));
	memDC.FillRect(&rcHeader,&brBackTiltle);

	// 定义绘制元素变量
	DRAWITEMSTRUCT drawItem;
	drawItem.CtlType = ODT_TAB;
	drawItem.CtlID = GetDlgCtrlID();
	drawItem.hwndItem = this->m_hWnd;
	drawItem.hDC = memDC.GetSafeHdc();
	drawItem.itemAction = ODA_DRAWENTIRE;
	int nItemCount = GetItemCount();

	// 如果标签项数量小于1，则不绘制
	if (nItemCount < 1)
		return;

	// 当前选中项索引取得
	int nCurSel = GetCurSel();

	// 遍历所有标签项，分别调用DrawItem函数进行绘制
	for(int i=0; i < GetItemCount(); i++)
	{
		drawItem.itemID = i;							// 索引赋值
		if (i == nCurSel)								// 当前选中状态判断
			drawItem.itemState = ODS_SELECTED;
		else
			drawItem.itemState = 0;
		GetItemRect(i, &drawItem.rcItem);
		DrawItem(&drawItem);
	}

	CPen pen(PS_SOLID,2,RGB(200,200,200));
	memDC.SelectObject(pen);
	memDC.MoveTo(rcHeader.right,rcHeader.top);
	memDC.LineTo(rcHeader.right,rcHeader.bottom);

	memDC.CopyToSrcDC();								// 把内存DC上的内容复制到源DC上
}

void CTestManTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
	GetItem(nIndex, &tcItem);
	dc.SetBkMode(TRANSPARENT);

	// 选择标签标题的字体和颜色
	CFontsLib * pFontLib = CFontsLib::GetInstance();
	CFont * pFont = NULL;

	// 如果是当前焦点，则选用带下划线字体
	if(nIndex == m_nHoverItem)
		pFont = pFontLib->GetFont(FontTitleUnderline);
	else
		pFont = pFontLib->GetFont(FontTitle);

	dc.SelectObject(pFont);
	dc.SetTextColor(RGB(102,102,102));				// 字体颜色为灰色

	if (nState & ODS_SELECTED)						//处于选中状态
	{
		// 如果是当前焦点，则选用带下划线字体
		if(nIndex == m_nHoverItem)
			pFont = pFontLib->GetFont(FontTabsUnderline);
		else
			pFont = pFontLib->GetFont(FontTabs);

		dc.SelectObject(pFont);

		CBrush brSelectedBack(RGB(204,225,152));	// 选中画刷
		CRect rcFillBack = rcItem;					// 选中背景区域
		rcFillBack.top = rcItem.top + rcItem.Height() / 3;
		rcFillBack.bottom = rcItem.bottom - rcItem.Height() / 3;
		rcFillBack.left = rcFillBack.left;
		rcFillBack.right = rcFillBack.right - 4;
		dc.FillRect(&rcFillBack,&brSelectedBack);
	}

	dc.DrawText(strLabel, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	// 画箭头“↓”
	if( lpDrawItemStruct->itemID < (UINT)(GetItemCount() - 1))
	{
		pFont = pFontLib->GetFont(FontTitle);
		dc.SelectObject(pFont);

		CRect rcArrow;
		GetItemRect( lpDrawItemStruct->itemID + 1,rcArrow);
		rcArrow.top = rcItem.top;
		dc.DrawText(_T("↓"),rcArrow,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

	dc.Detach();
}

BOOL CTestManTab::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTestManTab::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	m_TabHeight = cy / 6;
	SetMinTabWidth(80);												//设置最小选项卡的宽度
	SetItemSize(CSize(m_TabHeight,m_TabWidth));
}

void CTestManTab::OnMouseMove(UINT nFlags, CPoint point)
{
	for(int i=0; i < GetItemCount(); i++)
	{
		CRect rcItem;
		GetItemRect( i,rcItem);

		// 如果击中了某个标签项，并且不等于当前焦点项
		if(PtInRect(&rcItem,point) && (i != m_nHoverItem))
		{
			m_nLastHover = m_nHoverItem;							// 保存原焦点索引
			m_nHoverItem = i;										// 新焦点索引
			InvalidateRect(&rcItem);								// 使本项无效
			GetItemRect(m_nLastHover,rcItem);
			InvalidateRect(&rcItem);								// 使原焦点项无效
		}	
	}
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));			// 鼠标指针设置为小手状

	if (m_bMouseTrack)												// 若允许追踪
	{
		TRACKMOUSEEVENT csTME;

		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;									// 指定要追踪的窗口
		csTME.dwHoverTime = 10;										// 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
		::_TrackMouseEvent(&csTME);									// 开启Windows的WM_MOUSELEAVE事件支持	
		m_bMouseTrack = FALSE;
		CTabCtrl::OnMouseMove(nFlags, point);						// 若已经追踪，则停止追踪
	}
}

LRESULT CTestManTab::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// 当鼠标离开整个控件时，也要去掉m_posLastHover位置的焦点状态
	m_bMouseTrack = TRUE;

	CRect rcItem;
	GetItemRect(m_nHoverItem,rcItem);
	InvalidateRect(&rcItem);								// 使原焦点项无效

	// 去掉焦点位置记录
	m_nLastHover = -1;
	m_nHoverItem = -1;
	return 0;
}
