// ResizeDialog.cpp : 实现文件

#include "stdafx.h"
#include "BaseDialog.h"

// CBaseDialog 对话框

IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

CBaseDialog::CBaseDialog(const UINT nIDTemplate, CWnd* pParentWnd)
:CDialog(nIDTemplate,pParentWnd)
{
	m_pSkin = NULL;

	// 如果有父对话框,则使标题栏和边框的高度系统默认值
	if(pParentWnd == NULL)
	{
		m_BorderWidth = GetSystemMetrics(SM_CXBORDER);
		m_BorderHeight = GetSystemMetrics(SM_CYBORDER);
		m_CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	}
	// 否则为0
	else
	{
		m_BorderWidth = 0;
		m_BorderHeight = 0;
		m_CaptionHeight = 0;
	}
}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBaseDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCACTIVATE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
END_MESSAGE_MAP()

BOOL CBaseDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 窗口初始大小设置
	CRect rect;
	GetClientRect(rect);
	m_WndInitSize.cx = static_cast<LONG>(rect.Width());
	m_WndInitSize.cy = static_cast<LONG>(rect.Height());

	EnableToolTips(TRUE);
	m_ToolTip.Create(this);
	m_ToolTip.SetDelayTime(500);

	//m_ToolTip.AddTool(this,_T("关闭") ,&m_rcCloseButton,btClose);
	//m_ToolTip.AddTool(this,_T("最大化") ,&m_rcMaxButton,btMaxOrRestore);
	//m_ToolTip.AddTool(this,_T("最小化") ,&m_rcMinButton,btMin);
	//m_ToolTip.AddTool(this,_T("设置") ,&m_rcSettingButton,btSetting);

	return TRUE;
}

void CBaseDialog::SetResize(ControlsResizeProp &Item)
{
	// 根据ID取得控件窗口指针
	CWnd * pWnd = GetDlgItem(Item.ID);
	if( pWnd == NULL)
	{
		ASSERT(0);	
		return;
	}

	// 如果没有输入初始矩形，则取默认值
	pWnd->GetWindowRect(&Item.rcInitPosRect);
	ScreenToClient(Item.rcInitPosRect);

	m_vectCtrls.push_back(Item);
}

void CBaseDialog::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// 设置对话框不能小于初始大小（即对话框模板大小）
	int nWidth = pRect->right - pRect->left;
	int nHeight = pRect->bottom- pRect->top;
	if (nWidth < m_WndInitSize.cx)
	{
		pRect->right = pRect->left + m_WndInitSize.cx;
	}

	if (nHeight < m_WndInitSize.cy)
	{
		pRect->bottom = pRect->top + m_WndInitSize.cy;
	}
}

void CBaseDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetWindowRect(rect);
	
	// 窗口区域相对坐标
	m_rcWndSize.top = 0;
	m_rcWndSize.left = 0;
	m_rcWndSize.right = rect.right - rect.left;
	m_rcWndSize.bottom = rect.bottom - rect.top;

	// 标题栏相对坐标
	m_rcCaption.top = 0;
	m_rcCaption.bottom = m_rcCaption.top + m_CaptionHeight;
	m_rcCaption.left = 1;
	m_rcCaption.right = m_rcWndSize.right - m_BorderWidth;

	// 客户区域相对坐标
	m_rcClient.top = m_rcCaption.bottom;		
	m_rcClient.left = m_rcCaption.left;		
	m_rcClient.right = m_rcCaption.right;		
	m_rcClient.bottom = m_rcWndSize.bottom - m_BorderHeight;

	// 左边框矩形
	m_rcLeftBorder.top = m_rcCaption.bottom;		
	m_rcLeftBorder.left = m_rcWndSize.left;
	m_rcLeftBorder.right = m_rcWndSize.left + m_BorderWidth;		
	m_rcLeftBorder.bottom = m_rcWndSize.bottom;

	// 右边框矩形
	m_rcRightBorder.top = m_rcCaption.bottom;		
	m_rcRightBorder.right = m_rcWndSize.right;		
	m_rcRightBorder.left = m_rcWndSize.right - m_BorderWidth;
	m_rcRightBorder.bottom = m_rcWndSize.bottom;

	// 下边框矩形
	m_rcBottomBorder.bottom = m_rcWndSize.bottom;
	m_rcBottomBorder.top = m_rcWndSize.bottom - m_BorderHeight;		
	m_rcBottomBorder.left = m_rcWndSize.left;
	m_rcBottomBorder.right = m_rcWndSize.right;		

	// 关闭按钮,28 X 27
	m_rcCloseButton.right = m_rcWndSize.right - 1;
	m_rcCloseButton.left = m_rcCloseButton.right - 28;	
	m_rcCloseButton.top = m_rcWndSize.top + 1;
	m_rcCloseButton.bottom = m_rcCloseButton.top + 27;

	// 最大化（还原）按钮,28 X 27
	m_rcMaxButton.right = m_rcCloseButton.left;
	m_rcMaxButton.left = m_rcMaxButton.right - 28;	
	m_rcMaxButton.top = m_rcWndSize.top + 1;
	m_rcMaxButton.bottom = m_rcMaxButton.top + 27;

	// 最小化按钮,28 X 27
	m_rcMinButton.right = m_rcMaxButton.left;
	m_rcMinButton.left = m_rcMinButton.right - 28;	
	m_rcMinButton.top = m_rcWndSize.top + 1;
	m_rcMinButton.bottom = m_rcMinButton.top + 27;

	// 设置按钮,28 X 27
	m_rcSettingButton.right = m_rcMinButton.left;
	m_rcSettingButton.left = m_rcSettingButton.right - 28;	
	m_rcSettingButton.top = m_rcWndSize.top + 1;
	m_rcSettingButton.bottom = m_rcSettingButton.top + 27;

	// 按钮状态初始化
	m_MaxButtonState = bsNormal;
	m_MinButtonState = bsNormal;
	m_CloseButtonState = bsNormal;
	m_SettingButtonState = bsNormal;

	HRGN hRgnWndRect = CreateRectRgn(0,0,m_rcWndSize.right,m_rcWndSize.bottom);
	SetWindowRgn(hRgnWndRect,TRUE);
	DeleteObject(hRgnWndRect);

	Invalidate(FALSE);
}

void CBaseDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if(bCalcValidRects)
	{
		// 处理前的新窗口区域、旧窗口区域和旧客户区域（全是屏幕坐标）
		RECT rcNewWnd;
		CopyRect(&rcNewWnd,&lpncsp->rgrc[0]);

		// 新客户区域绝对坐标
		RECT rcNewClient;
		rcNewClient.top = rcNewWnd.top + m_CaptionHeight;
		rcNewClient.bottom = rcNewWnd.bottom - m_BorderHeight;
		rcNewClient.left = rcNewWnd.left + m_BorderWidth;
		rcNewClient.right = rcNewWnd.right - m_BorderWidth;

		// 处理后的新客户区域
		CopyRect(&lpncsp->rgrc[0],&rcNewClient);
	}
}

CRect CBaseDialog::_GetNewRect(const ControlsResizeProp &Item)
{
	// 控件新位置基点
	CPoint newBasicPoint;

	// 根据定位基点x值变化方法计算新定位基点的x值
	switch(Item.ctmXofPosBps)
	{
		// 如果不能变化，则取原值
	case tmNotChange:
		{
			newBasicPoint.x = Item.ptOrigPosBps.x;
			break;
		}

		// 如果是定量变化，则取窗口增量值
	case tmQuantity:
		{
			newBasicPoint.x = Item.ptOrigPosBps.x + (m_WndNewSize.cx - m_WndInitSize.cx);
			break;
		}

		// 如果是定比例变化，则按窗口缩放比例值增减
	case tmProportion:
		{
			newBasicPoint.x = (int)(Item.ptOrigPosBps.x * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			break;
		}
	default:
		ASSERT(0);
	}

	// 根据定位基点y值变化方法计算新定位基点的y值
	switch(Item.ctmYofPosBps)
	{
		// 如果不能变化，则取原值
	case tmNotChange:
		{
			newBasicPoint.y = Item.ptOrigPosBps.y;
			break;
		}

		// 如果是定量变化，则取窗口增量值
	case tmQuantity:
		{
			newBasicPoint.y = Item.ptOrigPosBps.y + (m_WndNewSize.cy - m_WndInitSize.cy);
			break;
		}

		// 如果是定比例变化，则按窗口缩放比例值增减
	case tmProportion:
		{
			newBasicPoint.y = (int)(Item.ptOrigPosBps.y * ((float)m_WndNewSize.cy /(float) m_WndInitSize.cy));
			break;
		}
	default:
		ASSERT(0);
	}

	// 控件新位置矩形
	CRect newRect;

	// 根据宽度变化方法计算新矩形的左右坐标
	switch(Item.ctmWigth)
	{
		// 如果不能变化，则左右同时增加相同量
		// 新坐标 = 新定位基点 + （旧坐标 - 旧定位基点）
	case tmNotChange:
		{
			newRect.left = Item.rcInitPosRect.left + (newBasicPoint.x - Item.ptOrigPosBps.x);
			newRect.right = Item.rcInitPosRect.right + (newBasicPoint.x - Item.ptOrigPosBps.x);
			break;
		}

		// 如果是定量变化，则说明控件左侧坐标不变，右侧坐标加上窗口增量值，此时与定位基点x坐标无关
	case tmQuantity:
		{
			newRect.left = Item.rcInitPosRect.left;
			newRect.right = Item.rcInitPosRect.right + (m_WndNewSize.cx - m_WndInitSize.cx);
			break;
		}

		// 如果是定比例变化，则按窗口缩放比例值增减
		// 新坐标 = 新定位基点 + （旧坐标 - 旧定位基点）* 缩放比例
	case tmProportion:
		{
			newRect.left = newBasicPoint.x + (int)((Item.rcInitPosRect.left - Item.ptOrigPosBps.x)  * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			newRect.right = newBasicPoint.x + (int)((Item.rcInitPosRect.right - Item.ptOrigPosBps.x)  * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			break;
		}
	default:
		ASSERT(0);
	}

	// 根据高度变化方法计算新矩形的上下坐标
	switch(Item.ctmHight)
	{
		// 如果不能变化，则取原值
	case tmNotChange:
		{
			newRect.top = Item.rcInitPosRect.top + (newBasicPoint.y - Item.ptOrigPosBps.y);
			newRect.bottom = Item.rcInitPosRect.bottom + (newBasicPoint.y - Item.ptOrigPosBps.y);
			break;
		}

		// 如果是定量变化，则说明控件上侧坐标不变，右侧坐标加上窗口增量值，此时与定位基点x坐标无关
	case tmQuantity:
		{
			newRect.top = Item.rcInitPosRect.top;
			newRect.bottom = Item.rcInitPosRect.bottom + (m_WndNewSize.cy - m_WndInitSize.cy);
			break;
		}

		// 如果是定比例变化，则按窗口缩放比例值增减
	case tmProportion:
		{
			newRect.top = newBasicPoint.y + (int)((Item.rcInitPosRect.top - Item.ptOrigPosBps.y)  * ((float)m_WndNewSize.cy /(float) m_WndInitSize.cy));
			newRect.bottom = newBasicPoint.y + (int)((Item.rcInitPosRect.bottom - Item.ptOrigPosBps.y)  * ((float)m_WndNewSize.cy /(float) m_WndInitSize.cy));
			break;
		}
	default:
		ASSERT(0);
	}

	return newRect;
}

BOOL CBaseDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CBaseDialog::OnNcActivate(BOOL bActive)
{
	OnNcPaint();
	return TRUE;
}

void CBaseDialog::OnNcPaint()
{
	if(m_pSkin)
		m_pSkin->DrawSkin(m_rcWndSize);
	else
		CDialog::OnNcPaint();
}

void CBaseDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	do 
	{
		CRect rect;
		GetWindowRect(rect);

		point.x -= rect.left;
		point.y -= rect.top;

		m_MaxButtonState = bsNormal;
		m_MinButtonState = bsNormal;
		m_CloseButtonState = bsNormal;
		m_SettingButtonState = bsNormal;

		if(m_rcCloseButton.PtInRect(point))
		{
			m_CloseButtonState = bsPress;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了最大化按钮上
		if(m_rcMaxButton.PtInRect(point))
		{
			m_MaxButtonState = bsPress;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了最小化按钮上
		if(m_rcMinButton.PtInRect(point))
		{
			m_MinButtonState = bsPress;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了设置按钮上
		if(m_rcSettingButton.PtInRect(point))
		{
			m_SettingButtonState = bsPress;
			OnNcPaint();
			break;
		}	

		CDialog::OnNcLButtonDown(nHitTest, point);
		break;
	} while (TRUE);
}

void CBaseDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	do 
	{
		CRect rect;
		GetWindowRect(rect);

		point.x -= rect.left;
		point.y -= rect.top;
		// 如果鼠标击中了关闭按钮上
		if(m_rcCloseButton.PtInRect(point))
		{
			if (m_CloseButtonState == bsPress)
				SendMessage(WM_CLOSE,0,0);

			m_MaxButtonState = bsNormal;
			m_MinButtonState = bsNormal;
			m_CloseButtonState = bsNormal;
			m_SettingButtonState = bsNormal;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了最大化按钮上
		if(m_rcMaxButton.PtInRect(point))
		{
			if (m_MaxButtonState == bsPress)
				if(IsZoomed())
					ShowWindow(SW_RESTORE);
				else
					ShowWindow(SW_MAXIMIZE);

			m_MaxButtonState = bsNormal;
			m_MinButtonState = bsNormal;
			m_CloseButtonState = bsNormal;
			m_SettingButtonState = bsNormal;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了最小化按钮上
		if(m_rcMinButton.PtInRect(point))
		{
			if (m_MinButtonState == bsPress)
				ShowWindow(SW_MINIMIZE);
			m_MaxButtonState = bsNormal;
			m_MinButtonState = bsNormal;
			m_CloseButtonState = bsNormal;
			m_SettingButtonState = bsNormal;
			OnNcPaint();
			break;
		}

		// 如果鼠标击中了设置按钮上
		//if(m_rcSettingButton.PtInRect(point))
		//{
		//	if (m_SettingButtonState == bsPress)
		//		SendMessage(WM_CLOSE,0,0);
			//m_MaxButtonState = bsNormal;
			//m_MinButtonState = bsNormal;
			//m_CloseButtonState = bsNormal;
			//m_SettingButtonState = bsNormal;
		//OnNcPaint();
		//}	

		CDialog::OnNcLButtonUp(nHitTest, point);
		break;
	} while (TRUE);
}

void CBaseDialog::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CRect rect;
	GetWindowRect(rect);

	point.x -= rect.left;
	point.y -= rect.top;

	m_MaxButtonState = bsNormal;
	m_MinButtonState = bsNormal;
	m_CloseButtonState = bsNormal;
	m_SettingButtonState = bsNormal;

	// 如果鼠标击中了关闭按钮上
	if(m_rcCloseButton.PtInRect(point))
	{
		if (m_CloseButtonState != bsPress)
			m_CloseButtonState = bsHot;
	}

	// 如果鼠标击中了最大化按钮上
	if(m_rcMaxButton.PtInRect(point))
	{
		if (m_MaxButtonState != bsPress)
			m_MaxButtonState = bsHot;
	}

	// 如果鼠标击中了最小化按钮上
	if(m_rcMinButton.PtInRect(point))
	{
		if (m_MinButtonState != bsPress)
			m_MinButtonState = bsHot;
	}

	// 如果鼠标击中了设置按钮上
	if(m_rcSettingButton.PtInRect(point))
	{
		if (m_SettingButtonState != bsPress)
			m_SettingButtonState = bsHot;
	}	
	//CDialog::OnNcMouseMove(nHitTest, point);
	OnNcPaint();
}

void CBaseDialog::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	CRect rect;
	GetClientRect(rect);
	m_WndNewSize = CSize(rect.Width(),rect.Height());
	m_rgnBackGround.DeleteObject();
	m_rgnBackGround.CreateRectRgnIndirect(&rect);

	// 遍历“控件大小位置属性”数组，分别重新设置控件大小和位置
	for(int i=0; i < (int)m_vectCtrls.size(); i++)
	{
		// 根据ID取得控件窗口指针
		CWnd * pWnd = GetDlgItem(m_vectCtrls[i].ID);
		CRect newRect = _GetNewRect(m_vectCtrls[i]);
		pWnd->MoveWindow(&newRect,FALSE);

		// 为避免控件闪烁，计算控件占用区域以外的背景区域，用于填充
		// 如果IsPaintBackGround为true，则把控件区域从对话框背景区域中刨去
		if(m_vectCtrls[i].IsPaintBackGround == true)
		{
			CRgn rgnControl;
			rgnControl.CreateRectRgnIndirect(&newRect);
			m_rgnBackGround.CombineRgn(&m_rgnBackGround,&rgnControl,RGN_DIFF);
		}
	}
}

BOOL CBaseDialog::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	m_ToolTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}