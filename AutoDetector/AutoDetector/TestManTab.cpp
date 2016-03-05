// TestManTab.cpp : ʵ���ļ�
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

// CTestManTab ��Ϣ�������

void CTestManTab::PreSubclassWindow()
{
	ModifyStyle(0,TCS_VERTICAL);
	m_TabHeight = 60;
	m_TabWidth = 130;
	SetMinTabWidth(80);												//������Сѡ��Ŀ��
	SetItemSize(CSize(m_TabHeight,m_TabWidth));
	CTabCtrl::PreSubclassWindow();
}

void CTestManTab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);

	CMemDC memDC(&dc,rect)						;					// �������ݵ��ڴ�DC
	memDC.CopyFromSrcDC();											// ��ԴDC���ݸ��Ƶ��ڴ�DC��

	// ����ͷ������
	CRect rcHeader = rect;
	rcHeader.right = rcHeader.left + m_TabWidth;
	

	//��������
	CBrush brBackTiltle(RGB(244,244,244));
	memDC.FillRect(&rcHeader,&brBackTiltle);

	// �������Ԫ�ر���
	DRAWITEMSTRUCT drawItem;
	drawItem.CtlType = ODT_TAB;
	drawItem.CtlID = GetDlgCtrlID();
	drawItem.hwndItem = this->m_hWnd;
	drawItem.hDC = memDC.GetSafeHdc();
	drawItem.itemAction = ODA_DRAWENTIRE;
	int nItemCount = GetItemCount();

	// �����ǩ������С��1���򲻻���
	if (nItemCount < 1)
		return;

	// ��ǰѡ��������ȡ��
	int nCurSel = GetCurSel();

	// �������б�ǩ��ֱ����DrawItem�������л���
	for(int i=0; i < GetItemCount(); i++)
	{
		drawItem.itemID = i;							// ������ֵ
		if (i == nCurSel)								// ��ǰѡ��״̬�ж�
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

	memDC.CopyToSrcDC();								// ���ڴ�DC�ϵ����ݸ��Ƶ�ԴDC��
}

void CTestManTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rcItem(lpDrawItemStruct->rcItem);			//��ȡ��Ŀ����
	int nIndex = lpDrawItemStruct->itemID;			//��ȡ��Ŀ����
	int nState = lpDrawItemStruct->itemState;		//��ȡ��Ŀ״̬

	TCITEM tcItem;
	TCHAR strLabel[MAX_PATH] = {0};	
	tcItem.mask        = TCIF_TEXT;
	tcItem.pszText     = strLabel;					// ��ǩ�����ʼ��
	tcItem.cchTextMax  = MAX_PATH;
	GetItem(nIndex, &tcItem);
	dc.SetBkMode(TRANSPARENT);

	// ѡ���ǩ������������ɫ
	CFontsLib * pFontLib = CFontsLib::GetInstance();
	CFont * pFont = NULL;

	// ����ǵ�ǰ���㣬��ѡ�ô��»�������
	if(nIndex == m_nHoverItem)
		pFont = pFontLib->GetFont(FontTitleUnderline);
	else
		pFont = pFontLib->GetFont(FontTitle);

	dc.SelectObject(pFont);
	dc.SetTextColor(RGB(102,102,102));				// ������ɫΪ��ɫ

	if (nState & ODS_SELECTED)						//����ѡ��״̬
	{
		// ����ǵ�ǰ���㣬��ѡ�ô��»�������
		if(nIndex == m_nHoverItem)
			pFont = pFontLib->GetFont(FontTabsUnderline);
		else
			pFont = pFontLib->GetFont(FontTabs);

		dc.SelectObject(pFont);

		CBrush brSelectedBack(RGB(204,225,152));	// ѡ�л�ˢ
		CRect rcFillBack = rcItem;					// ѡ�б�������
		rcFillBack.top = rcItem.top + rcItem.Height() / 3;
		rcFillBack.bottom = rcItem.bottom - rcItem.Height() / 3;
		rcFillBack.left = rcFillBack.left;
		rcFillBack.right = rcFillBack.right - 4;
		dc.FillRect(&rcFillBack,&brSelectedBack);
	}

	dc.DrawText(strLabel, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	// ����ͷ������
	if( lpDrawItemStruct->itemID < (UINT)(GetItemCount() - 1))
	{
		pFont = pFontLib->GetFont(FontTitle);
		dc.SelectObject(pFont);

		CRect rcArrow;
		GetItemRect( lpDrawItemStruct->itemID + 1,rcArrow);
		rcArrow.top = rcItem.top;
		dc.DrawText(_T("��"),rcArrow,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
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
	SetMinTabWidth(80);												//������Сѡ��Ŀ��
	SetItemSize(CSize(m_TabHeight,m_TabWidth));
}

void CTestManTab::OnMouseMove(UINT nFlags, CPoint point)
{
	for(int i=0; i < GetItemCount(); i++)
	{
		CRect rcItem;
		GetItemRect( i,rcItem);

		// ���������ĳ����ǩ����Ҳ����ڵ�ǰ������
		if(PtInRect(&rcItem,point) && (i != m_nHoverItem))
		{
			m_nLastHover = m_nHoverItem;							// ����ԭ��������
			m_nHoverItem = i;										// �½�������
			InvalidateRect(&rcItem);								// ʹ������Ч
			GetItemRect(m_nLastHover,rcItem);
			InvalidateRect(&rcItem);								// ʹԭ��������Ч
		}	
	}
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));			// ���ָ������ΪС��״

	if (m_bMouseTrack)												// ������׷��
	{
		TRACKMOUSEEVENT csTME;

		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;									// ָ��Ҫ׷�ٵĴ���
		csTME.dwHoverTime = 10;										// ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent(&csTME);									// ����Windows��WM_MOUSELEAVE�¼�֧��	
		m_bMouseTrack = FALSE;
		CTabCtrl::OnMouseMove(nFlags, point);						// ���Ѿ�׷�٣���ֹͣ׷��
	}
}

LRESULT CTestManTab::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// ������뿪�����ؼ�ʱ��ҲҪȥ��m_posLastHoverλ�õĽ���״̬
	m_bMouseTrack = TRUE;

	CRect rcItem;
	GetItemRect(m_nHoverItem,rcItem);
	InvalidateRect(&rcItem);								// ʹԭ��������Ч

	// ȥ������λ�ü�¼
	m_nLastHover = -1;
	m_nHoverItem = -1;
	return 0;
}
