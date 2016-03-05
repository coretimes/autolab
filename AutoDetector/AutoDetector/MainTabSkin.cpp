#include "StdAfx.h"
#include "MainTabSkin.h"
#include "GeneralFuction.h"
#include "MemDC.h"
#include "FontLib.h"

CMainTabSkin::CMainTabSkin(CMainTab * pTabCtrl)
{
	m_pTabCtrl = pTabCtrl;
	// ���ڱ߿��Ⱥ͸߶�
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
	// ע�⣺��Ƥ����̬���е�OnPaint���������ã���˺��������ٱ�����

	CPaintDC dc(m_pTabCtrl);										// ȡ��Դ����DC

	CMemDC memDC(&dc,m_pTabCtrl->GetClienRect());					// �������ݵ��ڴ�DC
	memDC.CopyFromSrcDC();											// ��ԴDC���ݸ��Ƶ��ڴ�DC��

	// ����ͷ������
	CRect rcHeader = m_pTabCtrl->GetClienRect();
	rcHeader.bottom = rcHeader.top + m_pTabCtrl->GetHeaderHeight();

	//��������
	int LeftStartX = 0,												// ��ߵĺ����ʼλ�õ�x����
		MiddleStartX = m_BannerLeft.GetWidth() - 4,					// �м�ĺ����ʼλ�õ�x����
		RightStartX = m_pTabCtrl->GetClienRect().Width() - (m_BannerRight.GetWidth() - 4);
																	// �ұߵĺ����ʼλ�õ�x����,4���ұ߽����

	//���Һ��
	m_BannerRight.Draw( memDC.m_hDC,								// �豸����			
		RightStartX,												// Ŀ��x����
		0,															// Ŀ��y����
		m_BannerRight.GetWidth() - 4,								// Ŀ����,4���ұ߽����		
		m_pTabCtrl->GetHeaderHeight(),								// Ŀ�곤�� 	
		0,															// Դx����	
		66,															// Դy����,66�Ǳ������߶�
		m_BannerRight.GetWidth() - 4,								// Դ���,4���ұ߽����
		m_pTabCtrl->GetHeaderHeight());								// Դ����

	// ������
	m_BannerLeft.Draw(memDC.m_hDC,									// �豸����
		LeftStartX,													// Ŀ��x����
		0,															// Ŀ��y����
		MiddleStartX - LeftStartX,									// Ŀ����
		m_pTabCtrl->GetHeaderHeight(),								// Ŀ�곤�� 	
		4,															// Դx����	
		66,															// Դy����,66�Ǳ������߶�
		MiddleStartX - LeftStartX,									// Դ���
		m_pTabCtrl->GetHeaderHeight());								// Ŀ�곤�� 

	// ����м���
	if(RightStartX > MiddleStartX)
		m_BannerMiddle.Draw( memDC.m_hDC,							// �豸����
		MiddleStartX,												// Ŀ��x����
		0,															// Ŀ��y����
		RightStartX - MiddleStartX,									// Ŀ����
		m_pTabCtrl->GetHeaderHeight(),								// Ŀ�곤�� 	
		0,															// Դx����	
		66,															// Դy����,66�Ǳ������߶�
		m_BannerMiddle.GetWidth(),									// Դ���
		m_pTabCtrl->GetHeaderHeight());								// Ŀ�곤�� 	


	// �������Ԫ�ر���
	DRAWITEMSTRUCT drawItem;
	drawItem.CtlType = ODT_TAB;
	drawItem.CtlID = GetDlgCtrlID(m_pTabCtrl->m_hWnd);
	drawItem.hwndItem = m_pTabCtrl->m_hWnd;
	drawItem.hDC = memDC.GetSafeHdc();
	drawItem.itemAction = ODA_DRAWENTIRE;
	int nItemCount = m_pTabCtrl->GetItemCount();

	// �����ǩ������С��1���򲻻���
	if (nItemCount < 1)
		return;

	// ��ǰѡ��������ȡ��
	int nCurSel = m_pTabCtrl->GetCurSel();

	// �������б�ǩ��ֱ����DrawItem�������л���
	for(int i=0; i < m_pTabCtrl->GetItemCount(); i++)
	{
		drawItem.itemID = i;							// ������ֵ
		if (i == nCurSel)								// ��ǰѡ��״̬�ж�
			drawItem.itemState = ODS_SELECTED;
		else
			drawItem.itemState = 0;
		m_pTabCtrl->GetItemRect(i, &drawItem.rcItem);
		m_pTabCtrl->DrawItem(&drawItem);
	}

	memDC.CopyToSrcDC();								// ���ڴ�DC�ϵ����ݸ��Ƶ�ԴDC��
}

void CMainTabSkin::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
	m_pTabCtrl->GetItem(nIndex, &tcItem);
	dc.SetBkMode(TRANSPARENT);

	// ѡ���ǩ������������ɫ
	CFontsLib * pFontLib = CFontsLib::GetInstance();
	CFont * pFont = pFontLib->GetFont(FontTabs);
	dc.SelectObject(pFont);
	dc.SetTextColor(RGB(102,102,102));				// ������ɫΪ��ɫ

	if (nState & ODS_SELECTED)						//����ѡ��״̬
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
