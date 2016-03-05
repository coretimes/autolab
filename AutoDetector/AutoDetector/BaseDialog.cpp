// ResizeDialog.cpp : ʵ���ļ�

#include "stdafx.h"
#include "BaseDialog.h"

// CBaseDialog �Ի���

IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

CBaseDialog::CBaseDialog(const UINT nIDTemplate, CWnd* pParentWnd)
:CDialog(nIDTemplate,pParentWnd)
{
	m_pSkin = NULL;

	// ����и��Ի���,��ʹ�������ͱ߿�ĸ߶�ϵͳĬ��ֵ
	if(pParentWnd == NULL)
	{
		m_BorderWidth = GetSystemMetrics(SM_CXBORDER);
		m_BorderHeight = GetSystemMetrics(SM_CYBORDER);
		m_CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	}
	// ����Ϊ0
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

	// ���ڳ�ʼ��С����
	CRect rect;
	GetClientRect(rect);
	m_WndInitSize.cx = static_cast<LONG>(rect.Width());
	m_WndInitSize.cy = static_cast<LONG>(rect.Height());

	EnableToolTips(TRUE);
	m_ToolTip.Create(this);
	m_ToolTip.SetDelayTime(500);

	//m_ToolTip.AddTool(this,_T("�ر�") ,&m_rcCloseButton,btClose);
	//m_ToolTip.AddTool(this,_T("���") ,&m_rcMaxButton,btMaxOrRestore);
	//m_ToolTip.AddTool(this,_T("��С��") ,&m_rcMinButton,btMin);
	//m_ToolTip.AddTool(this,_T("����") ,&m_rcSettingButton,btSetting);

	return TRUE;
}

void CBaseDialog::SetResize(ControlsResizeProp &Item)
{
	// ����IDȡ�ÿؼ�����ָ��
	CWnd * pWnd = GetDlgItem(Item.ID);
	if( pWnd == NULL)
	{
		ASSERT(0);	
		return;
	}

	// ���û�������ʼ���Σ���ȡĬ��ֵ
	pWnd->GetWindowRect(&Item.rcInitPosRect);
	ScreenToClient(Item.rcInitPosRect);

	m_vectCtrls.push_back(Item);
}

void CBaseDialog::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// ���öԻ�����С�ڳ�ʼ��С�����Ի���ģ���С��
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
	
	// ���������������
	m_rcWndSize.top = 0;
	m_rcWndSize.left = 0;
	m_rcWndSize.right = rect.right - rect.left;
	m_rcWndSize.bottom = rect.bottom - rect.top;

	// �������������
	m_rcCaption.top = 0;
	m_rcCaption.bottom = m_rcCaption.top + m_CaptionHeight;
	m_rcCaption.left = 1;
	m_rcCaption.right = m_rcWndSize.right - m_BorderWidth;

	// �ͻ������������
	m_rcClient.top = m_rcCaption.bottom;		
	m_rcClient.left = m_rcCaption.left;		
	m_rcClient.right = m_rcCaption.right;		
	m_rcClient.bottom = m_rcWndSize.bottom - m_BorderHeight;

	// ��߿����
	m_rcLeftBorder.top = m_rcCaption.bottom;		
	m_rcLeftBorder.left = m_rcWndSize.left;
	m_rcLeftBorder.right = m_rcWndSize.left + m_BorderWidth;		
	m_rcLeftBorder.bottom = m_rcWndSize.bottom;

	// �ұ߿����
	m_rcRightBorder.top = m_rcCaption.bottom;		
	m_rcRightBorder.right = m_rcWndSize.right;		
	m_rcRightBorder.left = m_rcWndSize.right - m_BorderWidth;
	m_rcRightBorder.bottom = m_rcWndSize.bottom;

	// �±߿����
	m_rcBottomBorder.bottom = m_rcWndSize.bottom;
	m_rcBottomBorder.top = m_rcWndSize.bottom - m_BorderHeight;		
	m_rcBottomBorder.left = m_rcWndSize.left;
	m_rcBottomBorder.right = m_rcWndSize.right;		

	// �رհ�ť,28 X 27
	m_rcCloseButton.right = m_rcWndSize.right - 1;
	m_rcCloseButton.left = m_rcCloseButton.right - 28;	
	m_rcCloseButton.top = m_rcWndSize.top + 1;
	m_rcCloseButton.bottom = m_rcCloseButton.top + 27;

	// ��󻯣���ԭ����ť,28 X 27
	m_rcMaxButton.right = m_rcCloseButton.left;
	m_rcMaxButton.left = m_rcMaxButton.right - 28;	
	m_rcMaxButton.top = m_rcWndSize.top + 1;
	m_rcMaxButton.bottom = m_rcMaxButton.top + 27;

	// ��С����ť,28 X 27
	m_rcMinButton.right = m_rcMaxButton.left;
	m_rcMinButton.left = m_rcMinButton.right - 28;	
	m_rcMinButton.top = m_rcWndSize.top + 1;
	m_rcMinButton.bottom = m_rcMinButton.top + 27;

	// ���ð�ť,28 X 27
	m_rcSettingButton.right = m_rcMinButton.left;
	m_rcSettingButton.left = m_rcSettingButton.right - 28;	
	m_rcSettingButton.top = m_rcWndSize.top + 1;
	m_rcSettingButton.bottom = m_rcSettingButton.top + 27;

	// ��ť״̬��ʼ��
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
		// ����ǰ���´������򡢾ɴ�������;ɿͻ�����ȫ����Ļ���꣩
		RECT rcNewWnd;
		CopyRect(&rcNewWnd,&lpncsp->rgrc[0]);

		// �¿ͻ������������
		RECT rcNewClient;
		rcNewClient.top = rcNewWnd.top + m_CaptionHeight;
		rcNewClient.bottom = rcNewWnd.bottom - m_BorderHeight;
		rcNewClient.left = rcNewWnd.left + m_BorderWidth;
		rcNewClient.right = rcNewWnd.right - m_BorderWidth;

		// �������¿ͻ�����
		CopyRect(&lpncsp->rgrc[0],&rcNewClient);
	}
}

CRect CBaseDialog::_GetNewRect(const ControlsResizeProp &Item)
{
	// �ؼ���λ�û���
	CPoint newBasicPoint;

	// ���ݶ�λ����xֵ�仯���������¶�λ�����xֵ
	switch(Item.ctmXofPosBps)
	{
		// ������ܱ仯����ȡԭֵ
	case tmNotChange:
		{
			newBasicPoint.x = Item.ptOrigPosBps.x;
			break;
		}

		// ����Ƕ����仯����ȡ��������ֵ
	case tmQuantity:
		{
			newBasicPoint.x = Item.ptOrigPosBps.x + (m_WndNewSize.cx - m_WndInitSize.cx);
			break;
		}

		// ����Ƕ������仯���򰴴������ű���ֵ����
	case tmProportion:
		{
			newBasicPoint.x = (int)(Item.ptOrigPosBps.x * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			break;
		}
	default:
		ASSERT(0);
	}

	// ���ݶ�λ����yֵ�仯���������¶�λ�����yֵ
	switch(Item.ctmYofPosBps)
	{
		// ������ܱ仯����ȡԭֵ
	case tmNotChange:
		{
			newBasicPoint.y = Item.ptOrigPosBps.y;
			break;
		}

		// ����Ƕ����仯����ȡ��������ֵ
	case tmQuantity:
		{
			newBasicPoint.y = Item.ptOrigPosBps.y + (m_WndNewSize.cy - m_WndInitSize.cy);
			break;
		}

		// ����Ƕ������仯���򰴴������ű���ֵ����
	case tmProportion:
		{
			newBasicPoint.y = (int)(Item.ptOrigPosBps.y * ((float)m_WndNewSize.cy /(float) m_WndInitSize.cy));
			break;
		}
	default:
		ASSERT(0);
	}

	// �ؼ���λ�þ���
	CRect newRect;

	// ���ݿ�ȱ仯���������¾��ε���������
	switch(Item.ctmWigth)
	{
		// ������ܱ仯��������ͬʱ������ͬ��
		// ������ = �¶�λ���� + �������� - �ɶ�λ���㣩
	case tmNotChange:
		{
			newRect.left = Item.rcInitPosRect.left + (newBasicPoint.x - Item.ptOrigPosBps.x);
			newRect.right = Item.rcInitPosRect.right + (newBasicPoint.x - Item.ptOrigPosBps.x);
			break;
		}

		// ����Ƕ����仯����˵���ؼ�������겻�䣬�Ҳ�������ϴ�������ֵ����ʱ�붨λ����x�����޹�
	case tmQuantity:
		{
			newRect.left = Item.rcInitPosRect.left;
			newRect.right = Item.rcInitPosRect.right + (m_WndNewSize.cx - m_WndInitSize.cx);
			break;
		}

		// ����Ƕ������仯���򰴴������ű���ֵ����
		// ������ = �¶�λ���� + �������� - �ɶ�λ���㣩* ���ű���
	case tmProportion:
		{
			newRect.left = newBasicPoint.x + (int)((Item.rcInitPosRect.left - Item.ptOrigPosBps.x)  * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			newRect.right = newBasicPoint.x + (int)((Item.rcInitPosRect.right - Item.ptOrigPosBps.x)  * ((float)m_WndNewSize.cx /(float) m_WndInitSize.cx));
			break;
		}
	default:
		ASSERT(0);
	}

	// ���ݸ߶ȱ仯���������¾��ε���������
	switch(Item.ctmHight)
	{
		// ������ܱ仯����ȡԭֵ
	case tmNotChange:
		{
			newRect.top = Item.rcInitPosRect.top + (newBasicPoint.y - Item.ptOrigPosBps.y);
			newRect.bottom = Item.rcInitPosRect.bottom + (newBasicPoint.y - Item.ptOrigPosBps.y);
			break;
		}

		// ����Ƕ����仯����˵���ؼ��ϲ����겻�䣬�Ҳ�������ϴ�������ֵ����ʱ�붨λ����x�����޹�
	case tmQuantity:
		{
			newRect.top = Item.rcInitPosRect.top;
			newRect.bottom = Item.rcInitPosRect.bottom + (m_WndNewSize.cy - m_WndInitSize.cy);
			break;
		}

		// ����Ƕ������仯���򰴴������ű���ֵ����
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

		// �������������󻯰�ť��
		if(m_rcMaxButton.PtInRect(point))
		{
			m_MaxButtonState = bsPress;
			OnNcPaint();
			break;
		}

		// �������������С����ť��
		if(m_rcMinButton.PtInRect(point))
		{
			m_MinButtonState = bsPress;
			OnNcPaint();
			break;
		}

		// ��������������ð�ť��
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
		// ����������˹رհ�ť��
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

		// �������������󻯰�ť��
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

		// �������������С����ť��
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

		// ��������������ð�ť��
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

	// ����������˹رհ�ť��
	if(m_rcCloseButton.PtInRect(point))
	{
		if (m_CloseButtonState != bsPress)
			m_CloseButtonState = bsHot;
	}

	// �������������󻯰�ť��
	if(m_rcMaxButton.PtInRect(point))
	{
		if (m_MaxButtonState != bsPress)
			m_MaxButtonState = bsHot;
	}

	// �������������С����ť��
	if(m_rcMinButton.PtInRect(point))
	{
		if (m_MinButtonState != bsPress)
			m_MinButtonState = bsHot;
	}

	// ��������������ð�ť��
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

	// �������ؼ���Сλ�����ԡ����飬�ֱ��������ÿؼ���С��λ��
	for(int i=0; i < (int)m_vectCtrls.size(); i++)
	{
		// ����IDȡ�ÿؼ�����ָ��
		CWnd * pWnd = GetDlgItem(m_vectCtrls[i].ID);
		CRect newRect = _GetNewRect(m_vectCtrls[i]);
		pWnd->MoveWindow(&newRect,FALSE);

		// Ϊ����ؼ���˸������ؼ�ռ����������ı��������������
		// ���IsPaintBackGroundΪtrue����ѿؼ�����ӶԻ��򱳾���������ȥ
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