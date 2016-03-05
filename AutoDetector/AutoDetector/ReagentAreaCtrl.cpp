// ReagentAreaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentAreaCtrl.h"
#include "MemDC.h"
#include "FontLib.h"
#include "ReagentSelectDlg.h"
#include "SampleAreaData.h"
// CReagentAreaCtrl

IMPLEMENT_DYNAMIC(CReagentAreaCtrl, CStatic)

CReagentAreaCtrl::CReagentAreaCtrl(const RAControlType & ctType)
{
	m_pMenu = NULL;
	m_ControlType = ctType;
	m_pReagentData = CReagentAreaData::GetInstance();
	m_bIsLButtonDown = FALSE;
	m_HoverPos = 0;
	m_pToolTip = NULL;
}

CReagentAreaCtrl::~CReagentAreaCtrl()
{
	if(m_pMenu)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}
	delete m_pToolTip;
}

BEGIN_MESSAGE_MAP(CReagentAreaCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	ON_COMMAND(IDM_Reagent_Select,OnClickMenuReagentSelect)
	ON_COMMAND(IDM_Reagent_Delete,OnClickMenuReagentDelete)
END_MESSAGE_MAP()

// CReagentAreaCtrl message handlers

void CReagentAreaCtrl::PreSubclassWindow()
{
	CGeneralFuction * pGenFunc = CGeneralFuction::GetInstance();
	pGenFunc->LoadImageFromResource(m_PlusPress,IDR_PlusPress,_T("PNG"));

	m_hIconPlus = AfxGetApp()->LoadIcon(IDI_Plus);
	m_hIconPlusDisable = AfxGetApp()->LoadIcon(IDI_PlusDisable);

	CStatic::PreSubclassWindow();
}

void CReagentAreaCtrl::OnPaint()
{
	CPaintDC dc(this);
	CMemDC memDC(&dc,m_rcClient); // Ĭ�Ͼ���Ϊ�ü�����
	memDC.CopyFromSrcDC();
	_DrawFrame(&memDC);
	memDC.CopyToSrcDC();
}

void CReagentAreaCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// ȡ�õ�ǰ���ָ��λ��
	Reagent_Pos pos;
	_WhichNode(point,pos);

	// �������ڵ������뵱ǰ����������ͬ�������κδ�������
	if( m_HoverPos == pos) 
		return ;

	// ����뽹�����겻ͬ��������������״̬������Ч
	m_bIsLButtonDown = FALSE;

	// ���ԭ����ڵ���Ч����ȡ��ԭ�ڵ��ѡ��״̬
	if (m_HoverPos > 0)
	{
		_InvalidPos(m_HoverPos);
		m_pReagentData->GetNode(m_HoverPos)->IsHover = false;
		m_pReagentData->SpreadUpdateMessage(m_HoverPos);
		m_HoverPos = 0;
	}

	// ������ڵ���Ч
	if(pos > 0)
	{
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		pNode->IsHover = true;
		_InvalidPos(pos);
		m_HoverPos = pos;
		m_pReagentData->SpreadUpdateMessage(pos);
	}

	if (m_bMouseTrack)							// ������׷��
	{
		TRACKMOUSEEVENT csTME;

		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;				// ָ��Ҫ׷�ٵĴ���
		csTME.dwHoverTime = 10;					// ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent(&csTME);				// ����Windows��WM_MOUSELEAVE�¼�֧��	
		m_bMouseTrack = FALSE;
		CStatic::OnMouseMove(nFlags, point);	// ���Ѿ�׷�٣���ֹͣ׷��
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CReagentAreaCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ����ǡ��ſ��������ڵĿؼ�������Ӧ����Ϣ
	if(m_ControlType == raForShowing)
		return ;

	Reagent_Pos pos;
	_WhichNode(point,pos);

	// ������ڵ���Ч,�����κδ���
	if(pos < 1)
	{
		return ;
	}

	ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);

	// ������ڵ����Թ�
	if(pNode->IsThereTube == false)
	{
		return;
	}
	m_bIsLButtonDown = TRUE;
	pNode->IsHover = true;

	// �ػ潹��λ��
	_InvalidPos(pos);
	CStatic::OnLButtonDown(nFlags, point);
}

void CReagentAreaCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// ����ǡ��ſ��������ڵĿؼ�������Ӧ����Ϣ
	if(m_ControlType == raForShowing)
		return ;

	Reagent_Pos pos;
	_WhichNode(point,pos);

	if (m_bIsLButtonDown == TRUE)
	{
		m_bIsLButtonDown = FALSE;
		// �ػ潹��λ��
		_InvalidPos(pos);

		// ������ڵ��Ѿ����ù��Լ����򵯳��˵������򵯳���ѡ���Լ����Ի���
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		if(pNode->IsSetingReagent)
		{
			// �����˵�
			_DisplayMenu();
		}
		else
		{
			// ������ѡ���Լ����Ի���
			OnClickMenuReagentSelect();
		}
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CReagentAreaCtrl::_DisplayMenu()
{
	if(m_pMenu != NULL)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}

	m_pMenu = new CMenu();
	m_pMenu->CreatePopupMenu();

	m_pMenu->AppendMenu(MF_STRING,IDM_Reagent_Select,_T("�޸��Լ�����..."));
	m_pMenu->AppendMenu(MF_STRING,IDM_Reagent_Delete,_T("ɾ���Լ�"));

	CPoint pt;
	GetCursorPos(&pt);
	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x + 5,pt.y + 5,this);
}

void CReagentAreaCtrl::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect(m_rcClient);

	m_rcTitleRegion.top = m_rcClient.top;
	m_rcTitleRegion.left = m_rcClient.left;
	m_rcTitleRegion.right = m_rcClient.right;
	m_rcTitleRegion.bottom = (int)(m_rcClient.bottom * 0.08);

	m_rcNodesRegion.top = m_rcClient.top + (int)(m_rcClient.Height() * 0.2);
	m_rcNodesRegion.left = m_rcClient.left + 10;
	m_rcNodesRegion.right = m_rcClient.right - 10;
	m_rcNodesRegion.bottom = m_rcNodesRegion.top + (int)(m_rcNodesRegion.Width() * 0.7);

	// �����ʾ���߻�δ�������򴴽�
	if(m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl();
		m_pToolTip->Create(this);
		m_pToolTip->SetDelayTime(350);					// �ӳ�ʱ��350����
		m_pToolTip->SetMaxTipWidth(400);

		// ����ToolTip
		for(int i=1; i <= NUM_REAGENTPOS; i++)
		{
			RECT rect = _GetPosRect(i);
			m_pToolTip->AddTool(this,LPSTR_TEXTCALLBACK ,&rect,i);
		}
	}
	// �����ʾ�����Ѿ�����������¾�������
	else
	{
		for(int i=1; i <= NUM_REAGENTPOS; i++)
		{

			RECT rect = _GetPosRect(i);
			m_pToolTip->SetToolRect(this,i,&rect);
		}
	}

	// TODO: �ڴ˴������Ϣ����������
}

void CReagentAreaCtrl::_DrawFrame(CDC *pDC)
{
	CPen penNodesRegionFrame(PS_SOLID,1, RGB(171,171,171));  // ���߻���
	CBrush brTitle(RGB(146,189,244));						 // ��ɫ���ⱳ����ˢ
	CBrush brBackGround(RGB(255,255,255));					 // ��ɫ������ˢ

	int nSavedDC = pDC->SaveDC();
	// ˢ��ɫ�ͻ����򱳾�ɫ
	pDC->SelectObject(brBackGround);
	pDC->FillRect(&m_rcClient,&brBackGround);

	// ���Ʊ�����
	pDC->SelectObject(brTitle);
	pDC->FillRect(&m_rcTitleRegion,&brTitle);
	CGeneralFuction::GetInstance()->DrawShadowCation(pDC,m_rcTitleRegion,_T("�Լ���"));

	// �������ؼ�����߿�ɫ
	CBrush brClientEdge(RGB(210,220,235));					 // ǳ���߱߿���ɫ
	pDC->SelectObject(brClientEdge);
	pDC->FrameRect(&m_rcClient,&brClientEdge);

	// ���ڵ�������Χ����
	CPen penDotLine(PS_DOT,1,RGB(222,222,222));
	CRect rcDotLine = m_rcNodesRegion;
	rcDotLine.InflateRect(5,5,5,5);
	pDC->SelectObject(brBackGround);
	pDC->SelectObject(penDotLine);
	pDC->Rectangle(rcDotLine);

	// ���Լ�λ�ÿ��
	for(int pos = 1; pos <= NUM_REAGENTPOS; pos++)
	{
		CBrush brFrame(RGB(222,222,222));					 // ��������ɫ

		// ���Լ�λ��ű���
		CBrush brNormalTitle(RGB(146,189,244));				 // ��ɫ�������ⱳ����ˢ
		CRect rcHeader = _GetHeaderRect(pos);

		pDC->FillRect(&rcHeader,&brNormalTitle);

		// ���Լ�λ
		CRect rcPos = _GetPosRect(pos);
		pDC->FrameRect(&rcPos,&brFrame);

		// ���Ʊ����ı�
		CString strID;
		strID.Format(_T("%d"), pos);

		pDC->SetBkMode(TRANSPARENT);


		// ����λ�õı�����»���
		if(pos == m_HoverPos)
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontSerialNoUnderline));
		else
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontSerialNo));
		pDC->DrawText(strID, &rcHeader, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
	}

	// ���Թ�
	for(int pos = 1; pos <= NUM_REAGENTPOS; pos++)
	{
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		// ���û���Թܣ����ٻ�������Ԫ��
		if(pNode->IsThereTube == true)
			_DrawTubes((Reagent_Pos) pos,pDC);
	}

	pDC->RestoreDC(nSavedDC);
}

void CReagentAreaCtrl::_DrawTubes(const Reagent_Pos pos,CDC *pDC)
{
	CRect rcPos = _GetPosRect(pos);
	CRect rcHeader = _GetHeaderRect(pos);
	ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);

	// ������ڵ��ǽ���ڵ㣬���Ȼ�Բ�Ǿ���

	int	 nTubeLength = (rcPos.Height() - rcHeader.Height()) / 10;
	CRect rectTube;
	rectTube.left = rcPos.left;
	rectTube.right = rcPos.right;
	// ��1���Թ�
	rectTube.top = rcPos.top;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// ��2���Թ�
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// ��3���Թ�
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// ��4���Թ�
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// ��5�����Թ�
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength * 2;
	_DrawBigTube(rectTube,pDC);
	// ��6�����Թ�
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength * 2;
	_DrawBigTube(rectTube,pDC);

	// �Թܰ�
	CBrush brTubeHandle(RGB(250,250,255));							 // ǳ��ɫ�Թܼܻ�ˢ 	
	CRect rectTubeHandle;
	rectTubeHandle.left = rcPos.left;
	rectTubeHandle.right = rcPos.right;
	rectTubeHandle.top = rectTube.bottom - 1;
	rectTubeHandle.bottom = rcHeader.top;
	pDC->SelectObject(brTubeHandle);
	pDC->RoundRect(&rectTubeHandle,CPoint(15,15));

	// ������Թܵ���û�������Լ�
	if(pNode->IsSetingReagent == false)
	{
		// �Թܰ��ϵļӺ�ͼ��λ�þ���Ϊ�ԹܰѾ��ε�������
		CRect rcIcon;
		rcIcon.left = rectTubeHandle.left + rectTubeHandle.Width() / 6;
		rcIcon.right = rectTubeHandle.right - rectTubeHandle.Width() / 6;
		int length = rcIcon.right - rcIcon.left;
		rcIcon.top = ((rectTubeHandle.top + rectTubeHandle.bottom) /2) - length / 2;
		rcIcon.bottom = rcIcon.top + length;

		// ������ڡ����ԡ������е������õĿؼ�������Ҫ����+����ͼ��
		if(m_ControlType == raForConfig)
		{
			// ����ǽ���
			if(pos == m_HoverPos)
			{
				// �����갴�£���
				if(m_bIsLButtonDown)
					m_PlusPress.Draw(pDC->m_hDC,								// �豸����
					rcIcon.left,												// Ŀ��x����
					rcIcon.top,													// Ŀ��y����
					rcIcon.Width(),												// Ŀ����
					rcIcon.Height(),											// Ŀ�곤�� 	
					0,															// Դx����	
					0,															// Դy����,66�Ǳ������߶�
					m_PlusPress.GetWidth(),										// Դ���
					m_PlusPress.GetHeight());									// Ŀ�곤�� 

				else
					DrawIconEx( pDC->m_hDC,			// �豸�������
					rcIcon.left,					// ͼ�����Ͻǵ�x���꣬���߼������ʾ
					rcIcon.top,						// ͼ�����Ͻǵ�x���꣬���߼������ʾ	
					m_hIconPlus,					// Ҫ����ͼ��ľ��
					length,							// ϣ����ͼ����
					length,							// ϣ����ͼ��߶�
					0,								// ���hIcon�Ǹ�����ָ�룬��ô�ò���ָ����涯���е��ķ�ͼ��
					NULL,							// ����Ϊһ��ˢ�Ӿ������ô�����Ὣͼ�껭��һ���ڴ�λͼ�����ñ���ɫ���
					DI_NORMAL);						// �ó��淽ʽ��ͼ
			}
			else
			{
				DrawIconEx( pDC->m_hDC,				// �豸�������
					rcIcon.left,					// ͼ�����Ͻǵ�x���꣬���߼������ʾ
					rcIcon.top,						// ͼ�����Ͻǵ�x���꣬���߼������ʾ	
					m_hIconPlusDisable,				// Ҫ����ͼ��ľ��
					length,							// ϣ����ͼ����
					length,							// ϣ����ͼ��߶�
					0,								// ���hIcon�Ǹ�����ָ�룬��ô�ò���ָ����涯���е��ķ�ͼ��
					NULL,							// ����Ϊһ��ˢ�Ӿ������ô�����Ὣͼ�껭��һ���ڴ�λͼ�����ñ���ɫ���
					DI_NORMAL);						// �ó��淽ʽ��ͼ
			}

		}
		// ������ڡ��ſ��������е���ʾ�õĿؼ�
		else
		{
			if(pos == m_HoverPos)
			{
				
			}
			else
			{
			}
		}
	}
	//����Ѿ������Լ�
	else
	{
		// ����ǽ���
		if(pos == m_HoverPos)
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontTextBond));
		else
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontText));

		ReagentRecord record = CReagentTable::GetInstance()->GetRecordInfo(pNode->ReagentID);
		CGeneralFuction::GetInstance()->DrawVerticalText(pDC,record.ShortName,&rectTubeHandle,DT_VCENTER | DT_CENTER);
	}

	return;
}

void CReagentAreaCtrl::_DrawSmallTube(const CRect &rect,CDC *pDC)
{
	CBrush brTubeRack(RGB(250,250,255));								 // ǳ��ɫ�Թܼܻ�ˢ 	
	CPen penTube(PS_SOLID,1, RGB(171,171,171));							 // ��ɫ�Թܻ���
	CBrush brLiquid(RGB(255,255,255));									 // ��ɫ�Լ���ˢ	

	// �Թ�СԲȦ
	RECT rcCircleBounding;
	rcCircleBounding.left = rect.left + 3;
	rcCircleBounding.right = rect.right - 3;
	int Radius = (rcCircleBounding.right - rcCircleBounding.left) / 2;	// Բ�İ뾶���Թܼܿ�Ⱦ���
	rcCircleBounding.top = (rect.top + rect.bottom) / 2 - Radius;
	rcCircleBounding.bottom = rcCircleBounding.top + 2 * Radius;
	pDC->SelectObject(penTube);
	pDC->SelectObject(brTubeRack);
	pDC->RoundRect(&rect,CPoint(5,5));

	pDC->SelectObject(penTube);
	pDC->SelectObject(brLiquid);
	pDC->Ellipse(&rcCircleBounding);
}

void CReagentAreaCtrl::_DrawBigTube(const CRect &rect,CDC *pDC)
{
	CBrush brTubeRack(RGB(255,231,181));								 // ǳ��ɫ�Թܼܻ�ˢ 	
	CPen penTubeRack(PS_SOLID,1, RGB(222,222,222));						 // ǳ��ɫ�Թܼܻ���
	CPen penTube(PS_SOLID,1, RGB(171,171,171));							 // ��ɫ�Թܻ���
	CBrush brLiquid(RGB(255,255,255));									 // ��ɫ�Լ���ˢ	

	// �Թ�СԲȦ
	RECT rcCircleBounding;
	rcCircleBounding.left = rect.left + 3;
	rcCircleBounding.right = rect.right - 3;
	int Radius = (rcCircleBounding.right - rcCircleBounding.left) / 2;	// Բ�İ뾶���Թܼܿ�Ⱦ���
	rcCircleBounding.top = (rect.top + rect.bottom) / 2 - Radius;
	rcCircleBounding.bottom = rcCircleBounding.top + 2 * Radius;
	pDC->SelectObject(penTube);
	pDC->SelectObject(brTubeRack);
	pDC->RoundRect(&rect,CPoint(10,10));

	pDC->SelectObject(penTube);
	pDC->SelectObject(brLiquid);
	pDC->Ellipse(&rcCircleBounding);
}

CRect CReagentAreaCtrl::_GetPosRect(const Reagent_Pos &pos) const
{
	// �Լ�λ���
	int width =	m_rcNodesRegion.Width() / NUM_REAGENTPOS;
	CRect rect;
	rect.top = m_rcNodesRegion.top;
	// �Լ�λ�������ұ��Ϊ1��2��3......15
	rect.left = m_rcNodesRegion.left + width * ((int)pos - 1);
	rect.right = rect.left + width + 1;
	rect.bottom = m_rcNodesRegion.bottom;
	return rect;
}

CRect CReagentAreaCtrl::_GetHeaderRect(const Reagent_Pos &pos) const
{
	CRect rect = _GetPosRect(pos);

	// ����߶�ռʮ��֮һ
	rect.top = (int)(rect.bottom - rect.Height() * 0.1);
	return rect;
}

void CReagentAreaCtrl::_WhichNode(const CPoint & pt, Reagent_Pos &pos) const
{
	// ���û�л��У������з��ظ�ֵ
	pos = 0;
	for(int r = 1; r <= NUM_REAGENTPOS; r++)
	{
		RECT noderect = _GetPosRect(r);

		// Ϊ��ֹ�����в��Թ�����������Сһ����о���
		noderect.left += 2;
		noderect.right -= 2;

		if(PtInRect(&noderect, pt))
		{		
			pos = r;
			return;
		}
	}
}

void CReagentAreaCtrl::_InvalidPos(const Reagent_Pos &pos)
{
	// ���жϽ����ȵ㽹���Ƿ���Чʱ��pos����Ϊ��ֵ��������Ҫ�ж�
	if (pos < 1 || pos > NUM_REAGENTPOS)
	{
		return;
	}

	CRect rect = _GetPosRect(pos);

	// ʹ�ڵ������Ч
	_GetPosRect(pos);
	InvalidateRect(&rect);
}

BOOL CReagentAreaCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CReagentAreaCtrl::PreTranslateMessage(MSG* pMsg)
{
	m_pToolTip->RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

BOOL CReagentAreaCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

	// ��ʾ���ߵ�ID����λ������
	UINT nPos = static_cast<UINT>(pNMHDR->idFrom);
	ReagentConfigNode * pNode = m_pReagentData->GetNode(nPos);
	do 
	{
		m_mapToolTipTexts[pNode->pos] = _T("");
		// ���û���Թ�
		if(pNode->IsThereTube == false)
		{
			m_mapToolTipTexts[pNode->pos] = _T("�˴����Թ�");
			break;
		}

		// ���û�������Լ�
		if(pNode->IsSetingReagent == false)
		{
			// ��������ý��棬����ʾΪ���������Լ���
			if(m_ControlType == raForConfig)
				m_mapToolTipTexts[pNode->pos] = _T("�������Լ�");
			// ����ڸſ����棬��ʾΪ��δ����������Ϣ��
			else
				m_mapToolTipTexts[pNode->pos] = _T("δ�����Լ���Ϣ");
			break;
		}
		// ����Ѿ������Լ�
		else
		{
			// ��������ý��棬����ʾΪ������޸Ļ�ɾ���Լ���
			if(m_ControlType == raForConfig)
				m_mapToolTipTexts[pNode->pos] = _T("����޸Ļ�ɾ���Լ�");
			// ����ڸſ����棬��ʾΪ��δ����������Ϣ��
			else
				_UpdateReagentTipText(pNode);
			break;
		}
		break;
	} while (TRUE);

	pTTT->lpszText = m_mapToolTipTexts[pNode->pos].AllocSysString();

	return TRUE;
}

BOOL CReagentAreaCtrl::_UpdateReagentTipText(const ReagentConfigNode * pNode)
{
	// ȡ�ü����ݿ��ָ��
	CReagentTable *pReagentTable = CReagentTable::GetInstance();

	// ȡ���Լ���¼��Ϣ
	ReagentRecord ReagnetInfo = pReagentTable->GetRecordInfo(pNode->ReagentID);

	CString strTipText;

	// ��ʾ�������Ŀ����Ϣ
	CString strProjectName;
	strProjectName.Format(_T("�����Ŀ����%s(%s)\n"),ReagnetInfo.ShortName,ReagnetInfo.ProjectName);
	strTipText += strProjectName;

	// ��ʾ���Լ����š���Ϣ
	CString strBatchNum;
	strBatchNum.Format(_T("�Լ����ţ���%s\n"),ReagnetInfo.BatchNum);
	strTipText += strBatchNum;

	// ��ʾ��������ޡ���Ϣ
	CString strUpperLimitt;
	strUpperLimitt.Format(_T("������ޣ���%g %s\n"),ReagnetInfo.UpperLimit,ReagnetInfo.Unit);
	strTipText += strUpperLimitt;

	// ��ʾ��������ޡ���Ϣ
	CString strLowerLimitt;
	strLowerLimitt.Format(_T("������ޣ���%g %s\n"),ReagnetInfo.LowerLimit,ReagnetInfo.Unit);
	strTipText += strLowerLimitt;

	// ��ʾ������ʱ�䡱��Ϣ
	CString strIncubationTime;
	strIncubationTime.Format(_T("����ʱ�䣺��%g min"),ReagnetInfo.IncubationTime);
	strTipText += strIncubationTime;

	m_mapToolTipTexts[pNode->pos] = strTipText;
	return TRUE;
}

void CReagentAreaCtrl::OnClickMenuReagentSelect()
{
	ReagentConfigNode *pNode = m_pReagentData->GetNode(m_HoverPos);
	CWaitCursor wait;
	// �����Լ�ѡ��Ի���
	Reagent_Key key = pNode->ReagentID;
	CReagentSelectDlg dlg(key);
	dlg.DoModal();
	wait.Restore();

	// �ж�ȡ�õ��Լ�ID
	if(key > 0)
	{
		// ��������
		pNode->IsSetingReagent = true;
		pNode->ReagentID = key;
		// �ػ潹��λ��
		_InvalidPos(m_HoverPos);

		// ���ߡ��Լ������ݡ������ػ�������ͼ
		m_pReagentData->SpreadUpdateMessage(0);
	}
}

void CReagentAreaCtrl::OnClickMenuReagentDelete()
{
	CSampleAreaData * pSampleData = CSampleAreaData::GetInstance();
	vector<Sample_Pos> TestSampleList;
	
	if(pSampleData->GetSampleByReagent(m_HoverPos,TestSampleList)>0)
	{
		MessageBox(_T("ĳЩ��������ʹ�ñ��Լ��������ڡ����������á�����ɾ����ز��ԣ�"),_T("��ʾ"),MB_OK);
		return;		
	}

	ReagentConfigNode *pNode = m_pReagentData->GetNode(m_HoverPos);
	pNode->IsSetingReagent = false;
	pNode->ReagentID = 0;
	_InvalidPos(m_HoverPos);
	// ���ߡ��Լ������ݡ������ػ�������ͼ
	m_pReagentData->SpreadUpdateMessage(0);
}
