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
	CMemDC memDC(&dc,m_rcClient); // 默认矩形为裁剪区域
	memDC.CopyFromSrcDC();
	_DrawFrame(&memDC);
	memDC.CopyToSrcDC();
}

void CReagentAreaCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// 取得当前鼠标指向位置
	Reagent_Pos pos;
	_WhichNode(point,pos);

	// 如果焦点节点坐标与当前焦点坐标相同，则不做任何处理并返回
	if( m_HoverPos == pos) 
		return ;

	// 如果与焦点坐标不同，则鼠标左键按下状态不再有效
	m_bIsLButtonDown = FALSE;

	// 如果原焦点节点有效，则取消原节点的选中状态
	if (m_HoverPos > 0)
	{
		_InvalidPos(m_HoverPos);
		m_pReagentData->GetNode(m_HoverPos)->IsHover = false;
		m_pReagentData->SpreadUpdateMessage(m_HoverPos);
		m_HoverPos = 0;
	}

	// 如果本节点有效
	if(pos > 0)
	{
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		pNode->IsHover = true;
		_InvalidPos(pos);
		m_HoverPos = pos;
		m_pReagentData->SpreadUpdateMessage(pos);
	}

	if (m_bMouseTrack)							// 若允许追踪
	{
		TRACKMOUSEEVENT csTME;

		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;				// 指定要追踪的窗口
		csTME.dwHoverTime = 10;					// 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
		::_TrackMouseEvent(&csTME);				// 开启Windows的WM_MOUSELEAVE事件支持	
		m_bMouseTrack = FALSE;
		CStatic::OnMouseMove(nFlags, point);	// 若已经追踪，则停止追踪
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CReagentAreaCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 如果是“概况”窗口内的控件，则不响应此消息
	if(m_ControlType == raForShowing)
		return ;

	Reagent_Pos pos;
	_WhichNode(point,pos);

	// 如果本节点无效,则不做任何处理
	if(pos < 1)
	{
		return ;
	}

	ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);

	// 如果本节点无试管
	if(pNode->IsThereTube == false)
	{
		return;
	}
	m_bIsLButtonDown = TRUE;
	pNode->IsHover = true;

	// 重绘焦点位置
	_InvalidPos(pos);
	CStatic::OnLButtonDown(nFlags, point);
}

void CReagentAreaCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 如果是“概况”窗口内的控件，则不响应此消息
	if(m_ControlType == raForShowing)
		return ;

	Reagent_Pos pos;
	_WhichNode(point,pos);

	if (m_bIsLButtonDown == TRUE)
	{
		m_bIsLButtonDown = FALSE;
		// 重绘焦点位置
		_InvalidPos(pos);

		// 如果本节点已经设置过试剂，则弹出菜单，否则弹出“选择试剂”对话框
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		if(pNode->IsSetingReagent)
		{
			// 弹出菜单
			_DisplayMenu();
		}
		else
		{
			// 弹出“选择试剂”对话框
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

	m_pMenu->AppendMenu(MF_STRING,IDM_Reagent_Select,_T("修改试剂配置..."));
	m_pMenu->AppendMenu(MF_STRING,IDM_Reagent_Delete,_T("删除试剂"));

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

	// 如果提示工具还未创建，则创建
	if(m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl();
		m_pToolTip->Create(this);
		m_pToolTip->SetDelayTime(350);					// 延迟时间350毫秒
		m_pToolTip->SetMaxTipWidth(400);

		// 更新ToolTip
		for(int i=1; i <= NUM_REAGENTPOS; i++)
		{
			RECT rect = _GetPosRect(i);
			m_pToolTip->AddTool(this,LPSTR_TEXTCALLBACK ,&rect,i);
		}
	}
	// 如果提示工具已经创建，则更新矩形区域
	else
	{
		for(int i=1; i <= NUM_REAGENTPOS; i++)
		{

			RECT rect = _GetPosRect(i);
			m_pToolTip->SetToolRect(this,i,&rect);
		}
	}

	// TODO: 在此处添加消息处理程序代码
}

void CReagentAreaCtrl::_DrawFrame(CDC *pDC)
{
	CPen penNodesRegionFrame(PS_SOLID,1, RGB(171,171,171));  // 灰线画笔
	CBrush brTitle(RGB(146,189,244));						 // 蓝色标题背景画刷
	CBrush brBackGround(RGB(255,255,255));					 // 白色背景画刷

	int nSavedDC = pDC->SaveDC();
	// 刷白色客户区域背景色
	pDC->SelectObject(brBackGround);
	pDC->FillRect(&m_rcClient,&brBackGround);

	// 绘制标题区
	pDC->SelectObject(brTitle);
	pDC->FillRect(&m_rcTitleRegion,&brTitle);
	CGeneralFuction::GetInstance()->DrawShadowCation(pDC,m_rcTitleRegion,_T("试剂区"));

	// 画整个控件区域边框色
	CBrush brClientEdge(RGB(210,220,235));					 // 浅灰线边框颜色
	pDC->SelectObject(brClientEdge);
	pDC->FrameRect(&m_rcClient,&brClientEdge);

	// 画节点区域外围虚线
	CPen penDotLine(PS_DOT,1,RGB(222,222,222));
	CRect rcDotLine = m_rcNodesRegion;
	rcDotLine.InflateRect(5,5,5,5);
	pDC->SelectObject(brBackGround);
	pDC->SelectObject(penDotLine);
	pDC->Rectangle(rcDotLine);

	// 画试剂位置框架
	for(int pos = 1; pos <= NUM_REAGENTPOS; pos++)
	{
		CBrush brFrame(RGB(222,222,222));					 // 灰线线颜色

		// 画试剂位编号标题
		CBrush brNormalTitle(RGB(146,189,244));				 // 蓝色正常标题背景画刷
		CRect rcHeader = _GetHeaderRect(pos);

		pDC->FillRect(&rcHeader,&brNormalTitle);

		// 画试剂位
		CRect rcPos = _GetPosRect(pos);
		pDC->FrameRect(&rcPos,&brFrame);

		// 绘制标题文本
		CString strID;
		strID.Format(_T("%d"), pos);

		pDC->SetBkMode(TRANSPARENT);


		// 焦点位置的标题加下划线
		if(pos == m_HoverPos)
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontSerialNoUnderline));
		else
			pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontSerialNo));
		pDC->DrawText(strID, &rcHeader, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
	}

	// 画试管
	for(int pos = 1; pos <= NUM_REAGENTPOS; pos++)
	{
		ReagentConfigNode *pNode = m_pReagentData->GetNode(pos);
		// 如果没有试管，不再绘制其他元素
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

	// 如果本节点是焦点节点，则先画圆角矩形

	int	 nTubeLength = (rcPos.Height() - rcHeader.Height()) / 10;
	CRect rectTube;
	rectTube.left = rcPos.left;
	rectTube.right = rcPos.right;
	// 第1个试管
	rectTube.top = rcPos.top;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// 第2个试管
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// 第3个试管
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// 第4个试管
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength;
	_DrawSmallTube(rectTube,pDC);
	// 第5个大试管
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength * 2;
	_DrawBigTube(rectTube,pDC);
	// 第6个大试管
	rectTube.top = rectTube.bottom - 1;
	rectTube.bottom = rectTube.top + nTubeLength * 2;
	_DrawBigTube(rectTube,pDC);

	// 试管把
	CBrush brTubeHandle(RGB(250,250,255));							 // 浅黄色试管架画刷 	
	CRect rectTubeHandle;
	rectTubeHandle.left = rcPos.left;
	rectTubeHandle.right = rcPos.right;
	rectTubeHandle.top = rectTube.bottom - 1;
	rectTubeHandle.bottom = rcHeader.top;
	pDC->SelectObject(brTubeHandle);
	pDC->RoundRect(&rectTubeHandle,CPoint(15,15));

	// 如果有试管但还没有配置试剂
	if(pNode->IsSetingReagent == false)
	{
		// 试管把上的加号图标位置矩形为试管把矩形的正中央
		CRect rcIcon;
		rcIcon.left = rectTubeHandle.left + rectTubeHandle.Width() / 6;
		rcIcon.right = rectTubeHandle.right - rectTubeHandle.Width() / 6;
		int length = rcIcon.right - rcIcon.left;
		rcIcon.top = ((rectTubeHandle.top + rectTubeHandle.bottom) /2) - length / 2;
		rcIcon.bottom = rcIcon.top + length;

		// 如果是在“测试”窗口中的配置用的控件，则需要画“+”号图标
		if(m_ControlType == raForConfig)
		{
			// 如果是焦点
			if(pos == m_HoverPos)
			{
				// 如果鼠标按下，则
				if(m_bIsLButtonDown)
					m_PlusPress.Draw(pDC->m_hDC,								// 设备环境
					rcIcon.left,												// 目标x坐标
					rcIcon.top,													// 目标y坐标
					rcIcon.Width(),												// 目标宽度
					rcIcon.Height(),											// 目标长度 	
					0,															// 源x坐标	
					0,															// 源y坐标,66是标题栏高度
					m_PlusPress.GetWidth(),										// 源宽度
					m_PlusPress.GetHeight());									// 目标长度 

				else
					DrawIconEx( pDC->m_hDC,			// 设备环境句柄
					rcIcon.left,					// 图标左上角的x坐标，用逻辑坐标表示
					rcIcon.top,						// 图标左上角的x坐标，用逻辑坐标表示	
					m_hIconPlus,					// 要描绘的图标的句柄
					length,							// 希望的图标宽度
					length,							// 希望的图标高度
					0,								// 如果hIcon是个动画指针，那么该参数指定描绘动画中的哪幅图象
					NULL,							// 如设为一个刷子句柄，那么函数会将图标画入一幅内存位图，并用背景色填充
					DI_NORMAL);						// 用常规方式绘图
			}
			else
			{
				DrawIconEx( pDC->m_hDC,				// 设备环境句柄
					rcIcon.left,					// 图标左上角的x坐标，用逻辑坐标表示
					rcIcon.top,						// 图标左上角的x坐标，用逻辑坐标表示	
					m_hIconPlusDisable,				// 要描绘的图标的句柄
					length,							// 希望的图标宽度
					length,							// 希望的图标高度
					0,								// 如果hIcon是个动画指针，那么该参数指定描绘动画中的哪幅图象
					NULL,							// 如设为一个刷子句柄，那么函数会将图标画入一幅内存位图，并用背景色填充
					DI_NORMAL);						// 用常规方式绘图
			}

		}
		// 如果是在“概况”窗口中的显示用的控件
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
	//如果已经配置试剂
	else
	{
		// 如果是焦点
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
	CBrush brTubeRack(RGB(250,250,255));								 // 浅黄色试管架画刷 	
	CPen penTube(PS_SOLID,1, RGB(171,171,171));							 // 灰色试管画笔
	CBrush brLiquid(RGB(255,255,255));									 // 白色试剂画刷	

	// 试管小圆圈
	RECT rcCircleBounding;
	rcCircleBounding.left = rect.left + 3;
	rcCircleBounding.right = rect.right - 3;
	int Radius = (rcCircleBounding.right - rcCircleBounding.left) / 2;	// 圆的半径由试管架宽度决定
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
	CBrush brTubeRack(RGB(255,231,181));								 // 浅黄色试管架画刷 	
	CPen penTubeRack(PS_SOLID,1, RGB(222,222,222));						 // 浅灰色试管架画笔
	CPen penTube(PS_SOLID,1, RGB(171,171,171));							 // 灰色试管画笔
	CBrush brLiquid(RGB(255,255,255));									 // 白色试剂画刷	

	// 试管小圆圈
	RECT rcCircleBounding;
	rcCircleBounding.left = rect.left + 3;
	rcCircleBounding.right = rect.right - 3;
	int Radius = (rcCircleBounding.right - rcCircleBounding.left) / 2;	// 圆的半径由试管架宽度决定
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
	// 试剂位宽度
	int width =	m_rcNodesRegion.Width() / NUM_REAGENTPOS;
	CRect rect;
	rect.top = m_rcNodesRegion.top;
	// 试剂位自作向右编号为1、2、3......15
	rect.left = m_rcNodesRegion.left + width * ((int)pos - 1);
	rect.right = rect.left + width + 1;
	rect.bottom = m_rcNodesRegion.bottom;
	return rect;
}

CRect CReagentAreaCtrl::_GetHeaderRect(const Reagent_Pos &pos) const
{
	CRect rect = _GetPosRect(pos);

	// 标题高度占十分之一
	rect.top = (int)(rect.bottom - rect.Height() * 0.1);
	return rect;
}

void CReagentAreaCtrl::_WhichNode(const CPoint & pt, Reagent_Pos &pos) const
{
	// 如果没有击中，则行列返回负值
	pos = 0;
	for(int r = 1; r <= NUM_REAGENTPOS; r++)
	{
		RECT noderect = _GetPosRect(r);

		// 为防止鼠标击中测试过于灵敏，减小一点击中矩形
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
	// 在判断焦点热点焦点是否有效时，pos可能为负值，所以需要判断
	if (pos < 1 || pos > NUM_REAGENTPOS)
	{
		return;
	}

	CRect rect = _GetPosRect(pos);

	// 使节点矩形无效
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

	// 提示工具的ID就是位置坐标
	UINT nPos = static_cast<UINT>(pNMHDR->idFrom);
	ReagentConfigNode * pNode = m_pReagentData->GetNode(nPos);
	do 
	{
		m_mapToolTipTexts[pNode->pos] = _T("");
		// 如果没有试管
		if(pNode->IsThereTube == false)
		{
			m_mapToolTipTexts[pNode->pos] = _T("此处无试管");
			break;
		}

		// 如果没有设置试剂
		if(pNode->IsSetingReagent == false)
		{
			// 如果在配置界面，则提示为“点击添加试剂”
			if(m_ControlType == raForConfig)
				m_mapToolTipTexts[pNode->pos] = _T("点击添加试剂");
			// 如果在概况界面，提示为“未配置样本信息”
			else
				m_mapToolTipTexts[pNode->pos] = _T("未配置试剂信息");
			break;
		}
		// 如果已经设置试剂
		else
		{
			// 如果在配置界面，则提示为“点击修改或删除试剂”
			if(m_ControlType == raForConfig)
				m_mapToolTipTexts[pNode->pos] = _T("点击修改或删除试剂");
			// 如果在概况界面，提示为“未配置样本信息”
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
	// 取得剂数据库表指针
	CReagentTable *pReagentTable = CReagentTable::GetInstance();

	// 取得试剂记录信息
	ReagentRecord ReagnetInfo = pReagentTable->GetRecordInfo(pNode->ReagentID);

	CString strTipText;

	// 显示“检测项目”信息
	CString strProjectName;
	strProjectName.Format(_T("检测项目：　%s(%s)\n"),ReagnetInfo.ShortName,ReagnetInfo.ProjectName);
	strTipText += strProjectName;

	// 显示“试剂批号”信息
	CString strBatchNum;
	strBatchNum.Format(_T("试剂批号：　%s\n"),ReagnetInfo.BatchNum);
	strTipText += strBatchNum;

	// 显示“结果上限”信息
	CString strUpperLimitt;
	strUpperLimitt.Format(_T("结果上限：　%g %s\n"),ReagnetInfo.UpperLimit,ReagnetInfo.Unit);
	strTipText += strUpperLimitt;

	// 显示“结果下限”信息
	CString strLowerLimitt;
	strLowerLimitt.Format(_T("结果下限：　%g %s\n"),ReagnetInfo.LowerLimit,ReagnetInfo.Unit);
	strTipText += strLowerLimitt;

	// 显示“孵育时间”信息
	CString strIncubationTime;
	strIncubationTime.Format(_T("孵育时间：　%g min"),ReagnetInfo.IncubationTime);
	strTipText += strIncubationTime;

	m_mapToolTipTexts[pNode->pos] = strTipText;
	return TRUE;
}

void CReagentAreaCtrl::OnClickMenuReagentSelect()
{
	ReagentConfigNode *pNode = m_pReagentData->GetNode(m_HoverPos);
	CWaitCursor wait;
	// 弹出试剂选择对话框
	Reagent_Key key = pNode->ReagentID;
	CReagentSelectDlg dlg(key);
	dlg.DoModal();
	wait.Restore();

	// 判断取得的试剂ID
	if(key > 0)
	{
		// 设置数据
		pNode->IsSetingReagent = true;
		pNode->ReagentID = key;
		// 重绘焦点位置
		_InvalidPos(m_HoverPos);

		// 告诉“试剂区数据”对象，重绘所有视图
		m_pReagentData->SpreadUpdateMessage(0);
	}
}

void CReagentAreaCtrl::OnClickMenuReagentDelete()
{
	CSampleAreaData * pSampleData = CSampleAreaData::GetInstance();
	vector<Sample_Pos> TestSampleList;
	
	if(pSampleData->GetSampleByReagent(m_HoverPos,TestSampleList)>0)
	{
		MessageBox(_T("某些测试正在使用本试剂，请先在【样本区配置】界面删除相关测试！"),_T("提示"),MB_OK);
		return;		
	}

	ReagentConfigNode *pNode = m_pReagentData->GetNode(m_HoverPos);
	pNode->IsSetingReagent = false;
	pNode->ReagentID = 0;
	_InvalidPos(m_HoverPos);
	// 告诉“试剂区数据”对象，重绘所有视图
	m_pReagentData->SpreadUpdateMessage(0);
}
