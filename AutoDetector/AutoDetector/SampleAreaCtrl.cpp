// SampleAreaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleAreaCtrl.h"
#include "MemDC.h"
#include "StandardTest.h"
#include "SampleConfigDlg.h"
#include "StandardConfigDlg.h"
#include "GeneralFuction.h"
#include "FontLib.h"

IMPLEMENT_DYNAMIC(CSampleAreaCtrl, CStatic)

CSampleAreaCtrl::CSampleAreaCtrl(const SAControlType & ctType)
{
	m_pMenu = NULL;
	m_CurrenSelectedMode = NoneSelected;
	m_CurrenSelectedStanderdKey = 0;
	m_pSampleAreaData = CSampleAreaData::GetInstance();
	m_pReagentAreaData = CReagentAreaData::GetInstance();
	m_ControlType = ctType;
	m_pToolTip = NULL;
}

CSampleAreaCtrl::~CSampleAreaCtrl()
{
	if(m_pMenu)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}
	delete m_pToolTip;
}

BEGIN_MESSAGE_MAP(CSampleAreaCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_All_Deselect,OnClickMenuCancelAllSelect)
	ON_COMMAND(IDM_Sample_Config,OnClickMenuSampleConfig)
	ON_COMMAND(IDM_Standard_Config,OnClickMenuStandardCongfig)
	ON_COMMAND(IDM_Delete_Sample,OnClickMenuDeleteSample)
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_SIZE()
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	END_MESSAGE_MAP()

// CSampleAreaCtrl message handlers

void CSampleAreaCtrl::PreSubclassWindow()
{
	m_IsLButtonDown = false;
	m_SelectStartPos = 0;

	EnableToolTips(TRUE);

	CStatic::PreSubclassWindow();
}

void CSampleAreaCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc,m_rcClient); // 默认矩形为裁剪区域
	memDC.CopyFromSrcDC();
	_DrawFrame(&memDC);
	memDC.CopyToSrcDC();
}

void CSampleAreaCtrl::_DrawFrame(CDC *pDC)
{
	CPen penNodesRegionFrame(PS_SOLID,1, RGB(171,171,171));  // 灰线画笔
	CBrush brTitle(RGB(245,194,85));						 // 黄色标题背景画刷
	CBrush brBackGround(RGB(255,255,255));					 // 白色背景画刷

	int nSavedDC = pDC->SaveDC();
	// 刷白色客户区域背景色
	pDC->SelectObject(brBackGround);
	pDC->FillRect(&m_rcClient,&brBackGround);

	// 绘制标题区
	pDC->SelectObject(brTitle);
	pDC->FillRect(&m_rcTitleRegion,&brTitle);
	CGeneralFuction * pFution = CGeneralFuction::GetInstance();
	pFution->DrawShadowCation(pDC,m_rcTitleRegion,_T("样本区"));

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

	// 画节点区域背景
	pDC->SelectObject(penNodesRegionFrame);
	pDC->Rectangle(&m_rcNodesRegion);

	// 画节点区域框架和点
	for(int x=0; x<=NUM_COL; x++)
	{
		if(x >0)
		{
			RECT rcTop,rcBottom;
			_GetNodeRect(x,1,rcTop);
			_GetNodeRect(x,NUM_ROW + 1,rcBottom);
			pDC->MoveTo(CPoint(rcTop.left,rcTop.top));
			pDC->LineTo(CPoint(rcBottom.left,rcBottom.bottom));
		}

		for(int y=1; y <= NUM_ROW+1; y++)
		{
			_DrawNode(x,y,pDC);
		}
	}
	pDC->RestoreDC(nSavedDC);
}

void CSampleAreaCtrl::_GetNodeRect(const int &col, const int &row, RECT &rect) const
{
	// 坐标位矩形的水平边长和垂直边长
	long HoriSideLength = (m_rcNodesRegion.right -m_rcNodesRegion.left) / (NUM_COL+1);
	long VertSideLength = (m_rcNodesRegion.bottom -m_rcNodesRegion.top) / (NUM_ROW+1);

	// 由于计算矩形平均边长时，会有误差，
	// 所以利用左标题栏和下标题栏交接处的左下角矩形
	RECT rcLeftBottom;
	rcLeftBottom.left = m_rcNodesRegion.left;
	rcLeftBottom.right = m_rcNodesRegion.right - HoriSideLength * (NUM_COL) - 1;

	rcLeftBottom.bottom = m_rcNodesRegion.bottom;
	rcLeftBottom.top = m_rcNodesRegion.top + VertSideLength * NUM_ROW - 1;

	// 如果是标题列
	if(col == 0)
	{
		rect.left = rcLeftBottom.left;
		rect.right = rcLeftBottom.right;
	}
	// 如果是最后一列，防止右边画两条线
	else if(col == NUM_COL)
	{
		rect.left = rcLeftBottom.right + HoriSideLength * (col - 1) - 1;
		rect.right = m_rcNodesRegion.right;
	}
	else	
	{
		rect.left = rcLeftBottom.right + HoriSideLength * (col - 1) - 1;
		rect.right = rect.left + HoriSideLength + 1;
	}

	if(row == NUM_ROW + 1)
	{
		rect.top = rcLeftBottom.top;
		rect.bottom = rcLeftBottom.bottom;
	}
	else
	{
		rect.top = m_rcNodesRegion.top + VertSideLength * (row-1);
		rect.bottom = rect.top + VertSideLength + 1;
	}
}

void CSampleAreaCtrl::_DrawNode(const int &col, const int &row, CDC *pDC)
{
	int nSavedDC = pDC->SaveDC();

	CBrush brHearder(RGB(245,194,85)); 								 // 标题栏黄色背景画刷
	CBrush brTubeRack(RGB(255,255,204));							 // 浅黄色试管架画刷 	
	CBrush brSelected(RGB(120,245,245));							 // 亮蓝色选中画刷
	CBrush brHover(RGB(255,255,0));									 // 黄色焦点画刷
	CBrush brLiquid(RGB(255,255,255));								 // 白色试剂画刷	
	CPen penHover(PS_SOLID,2, RGB(171,171,171));					 // 灰线粗画笔	
	CPen penFrameline(PS_SOLID,1, RGB(171,171,171));				 // 灰色细画笔

	RECT rect;
	_GetNodeRect(col,row,rect);

	int nHoverCol, nHoverRow;
	m_pSampleAreaData->GetCoordByNum(m_pSampleAreaData->GetHoverPos(),nHoverCol,nHoverRow);

	// 如果是左下角，则不绘制
	if(col == 0 && row == NUM_ROW + 1)
	{
	}
	// 如果是标题栏
	else if(col == 0 || row == NUM_ROW + 1)
	{
		// 如果是焦点的标题，则用不同颜色绘制
		if(col == nHoverCol || row == nHoverRow)
			pDC->SelectObject(brHover);
		else
			pDC->SelectObject(brHearder);

		pDC->SelectObject(penFrameline);
		pDC->Rectangle(&rect);
		pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextColor(RGB(54,54,54));

		// 绘制标题文本
		CString strID;
		if(col==0)
			strID.Format(_T("%d"), row);

		if(row == NUM_ROW + 1)
			strID.Format(_T("%c"), col + 64);

		pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontSerialNo));
		pDC->DrawText(strID, &rect, DT_SINGLELINE | DT_CENTER |DT_VCENTER);

	}
	// 如果是节点
	else
	{
		SampleConfigNode * pCurrentNode = m_pSampleAreaData->GetNode(col,row);
		// 如果本位置有试管
		if(pCurrentNode->IsThereTube)
		{
			RECT rcCircleBounding;
			rcCircleBounding.top = rect.top + 3;
			rcCircleBounding.bottom = rect.bottom - 3;
			rcCircleBounding.left = rect.left + 3;
			rcCircleBounding.right = rect.right - 3;
			pDC->SelectObject(penFrameline);

			// 画矩形，如果是选中状态并且控件时配置用类型，则选中画刷,否则选浅黄色画刷
			if(pCurrentNode->IsSelected && m_ControlType == saForConfig)
				pDC->SelectObject(brSelected);
			else
				pDC->SelectObject(brTubeRack);

			pDC->Rectangle(&rect);
			// 画圆圈，如果是焦点，则选用不同颜色画刷
			if(col == nHoverCol && row == nHoverRow)
			{
				pDC->SelectObject(penHover);
			}
			pDC->SelectObject(brLiquid);
			pDC->Ellipse(&rcCircleBounding);


			// 如果已经设置了样本
			if(pCurrentNode->IsSetingSample == true)
			{
				pDC->SelectObject(CFontsLib::GetInstance()->GetFont(FontDefault));
				// 如果是样本,中间写“Y”字
				if(pCurrentNode->IsStandard == false)
				{
					pDC->SetBkMode(TRANSPARENT);
					
					pDC->SetTextColor(RGB(54,54,54));
					pDC->DrawText(_T("Y"), &rcCircleBounding, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
				}

				// 如果是标准品
				else
				{
					// 标注点A、B、C......
					CString strStdPoint;
					strStdPoint.Format(_T("%c"), pCurrentNode->StdPointIndex + 65);
					pDC->DrawText(strStdPoint, &rcCircleBounding, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
				}
			}
		}
	}

	pDC->RestoreDC(nSavedDC);
}
void CSampleAreaCtrl::_InvalidNode(const int &col, const int &row)
{  
	// 在判断焦点热点焦点是否有效时，col和row可能为负值，所以需要判断
	if(col < 0 || col > NUM_COL)
		return;

	if(row < 1 || row > NUM_ROW +1)
		return;

	RECT rect;
	// 使节点矩形无效
	_GetNodeRect(col,row,rect);
	InvalidateRect(&rect);

	//使对应坐标标题无效
	_GetNodeRect(0,row,rect);
	InvalidateRect(&rect);

	_GetNodeRect(col,NUM_ROW + 1,rect);
	InvalidateRect(&rect);

}

BOOL CSampleAreaCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CSampleAreaCtrl::PreTranslateMessage(MSG* pMsg)
{
	m_pToolTip->RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

void CSampleAreaCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 如果“控件类型”只是为了显示,则直接退出，禁止编辑
	if(m_ControlType == saForShowing)
		return;

	// 1.取得当前指针指向的节点坐标
	int CurrentCol,CurrentRow;
	_WhichNode(point,CurrentCol,CurrentRow);

	// 2.如果节点坐标无效，则不做任何处理
	if(CurrentCol < 1 || CurrentRow < 1)
		return;

	// 3.如果单击的是列标题，进入列单击响应函数
	if(CurrentRow == NUM_ROW + 1)
	{
		_OnClickColumnHeader(CurrentCol);
		return;
	}

	SampleConfigNode * pCurrentNode = m_pSampleAreaData->GetNode(CurrentCol,CurrentRow);

	// 4.如果节点没有试管，则不做任何处理
	if(pCurrentNode->IsThereTube == false)
		return;

	// 5.如果单击的是空节点，则进入空节点单击响应函数
	if(pCurrentNode->IsSetingSample == false)
	{
		_OnClickEmptyNode(CurrentCol,CurrentRow);
		return;
	}

	// 6.如果单击的是样本节点，则进入样本节点单击响应函数
	if(pCurrentNode->IsStandard == false)
	{
		_OnClickSampleNode(CurrentCol,CurrentRow);
		return;
	}

	// 7.如果单击的是标准品节点，则进入标准品节点单击响应函数
	if(pCurrentNode->IsStandard == true)
	{
		_OnClickStanderdNode(CurrentCol,CurrentRow);
		return;
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CSampleAreaCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 标识鼠标键已放开
	m_IsLButtonDown = false;

	// 如果“控件类型”只是为了显示,则直接退出，禁止编辑
	if(m_ControlType == saForShowing)
		return;

	// 取得节点坐标
	int CurrentCol,CurrentRow;
	_WhichNode(point,CurrentCol,CurrentRow);

	// 如果单击列为标题列或区域以外，则退出
	if( CurrentCol < 1)
		return;

	// 如果单击的是列标题
	if(CurrentRow == NUM_ROW + 1)
	{
		//显示菜单
		_DisplayMenu();
		return;
	}

	// 取得节点数据，判断是否“有反应杯”
	SampleConfigNode * pCurrentNode = m_pSampleAreaData->GetNode(CurrentCol,CurrentRow);
	if(!pCurrentNode->IsThereTube)
		return;

	if(_IsNodeCouldSelected(CurrentCol,CurrentRow))
	{
		//显示菜单
		_DisplayMenu();
	}
}

void CSampleAreaCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nHoverCol,nHoverRow;
	// 根据指针坐标，取得当前节点坐标
	_WhichNode(point,nHoverCol,nHoverRow);

	// 如果本节点是标题节点，则去掉焦点状态
	if(nHoverCol < 1 || nHoverRow < 1 || nHoverRow > NUM_ROW)
	{
		m_pSampleAreaData->SetHover(0);
		return ;
	}

	SampleConfigNode * pHoverNode = m_pSampleAreaData->GetNode(nHoverCol,nHoverRow);
	// 如果本节点无试管
	if(pHoverNode->IsThereTube == false)
	{
		m_pSampleAreaData->SetHover(0);
		return  ;
	}

	// 如果鼠标左键处于按下状态，则此时处于连选状态
	if(m_IsLButtonDown)
	{
		// 如果有起始坐标，说明目前处于连选状态
		if (m_SelectStartPos > 0)	
		{
			// 如果当前行是标题
			int tempRow = nHoverRow;
			if(nHoverRow > NUM_ROW)
				tempRow = NUM_ROW;

			// 当前焦点节点设置为终止选择节点
			Sample_Pos SelectEndPos = m_pSampleAreaData->GetNumByCoord(nHoverCol,tempRow);

			// 循环遍历所有节点
			for(int c = 1; c <= NUM_COL; c++)
			{
				for(int r = 1; r <= NUM_ROW; r++)
				{	
					Sample_Pos pos = m_pSampleAreaData->GetNumByCoord(c,r);

					SampleConfigNode * pNode = m_pSampleAreaData->GetNode(c,r);

					// 如果节点处于起始选择节点与终止选择节点之间，并且有资格被选中，则全部设为被选状态
					if(( pos >= m_SelectStartPos && pos <= SelectEndPos)
						||(pos >= SelectEndPos && pos <= m_SelectStartPos))
					{

						if(_IsNodeCouldSelected(c,r))
						{
							pNode->IsSelected = true;
							_InvalidNode(c,r);
						}
					}
					// 否则设置为原状态
					else
					{
						if(pNode->IsSelected != m_SelectState[c-1][r-1])
						{
							pNode->IsSelected = m_SelectState[c-1][r-1];
							_InvalidNode(c,r);
						}
					}
				}
			}
		}
	}

	m_pSampleAreaData->SetHover(m_pSampleAreaData->GetNumByCoord(nHoverCol,nHoverRow));

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
}

LRESULT CSampleAreaCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// 当鼠标离开整个控件时，也要去掉m_posLastHover位置的焦点状态
	m_bMouseTrack = TRUE;
	m_pSampleAreaData->SetHover(0);
	return 0;
}

void CSampleAreaCtrl::OnClickMenuCancelAllSelect()
{
	m_pSampleAreaData->CancelAllSelected();
	m_CurrenSelectedMode = NoneSelected;
	InvalidateRect(m_rcNodesRegion);
}

void CSampleAreaCtrl::OnClickMenuSampleConfig()
{
	// “样本配置”菜单点击事件响应

	// 生成“选中样本配置节点”的map类型数组，用来做CStandardConfigDlg或CSampleConfigDlg的构造函数参数
	map<Sample_Pos,SampleConfigNode *> SampleNodeList;
	map<Reagent_Pos,const ReagentConfigNode *> ReagentNodeList;
	m_pSampleAreaData->GetSelectedNodes(SampleNodeList);
	m_pReagentAreaData->GetReagentConfigList(ReagentNodeList);

	CWaitCursor wait;					// 打开沙漏光标，因为读数据库数据时间可能比较长
	// 弹出标准品配置对话框
	if(m_CurrenSelectedMode == EmptyNode)
	{	
		CSampleConfigDlg dlg(true,SampleNodeList,ReagentNodeList);
		if(dlg.DoModal() == IDCANCEL)
		{
			wait.Restore();				// 关闭沙漏光标
			return ;
		}
	}
	else
	{	
		CSampleConfigDlg dlg(false,SampleNodeList,ReagentNodeList);
		if(dlg.DoModal() == IDCANCEL)
		{
			wait.Restore();
			return ;
		}		
	}
	wait.Restore();						// 关闭沙漏光标

	// 更新查询表
	m_pSampleAreaData->UpdateSearchList();

	// 取消所有节点选择状态
	m_pSampleAreaData->CancelAllSelected();
	m_CurrenSelectedMode = NoneSelected;
	m_pSampleAreaData->SpreadDataUpdateMessage();
	// 重画所有节点
	InvalidateRect(&m_rcNodesRegion);
}

void CSampleAreaCtrl::OnClickMenuStandardCongfig()
{
	// “标准品配置”菜单点击事件响应函数

	// 生成“选中样本配置节点”的map类型数组，用来做CStandardConfigDlg或CSampleConfigDlg的构造函数参数
	map<Sample_Pos,SampleConfigNode *> SampleNodeList;
	map<Reagent_Pos,const ReagentConfigNode *> ReagentNodeList;
	m_pSampleAreaData->GetSelectedNodes(SampleNodeList);
	m_pReagentAreaData->GetReagentConfigList(ReagentNodeList);

	CWaitCursor wait;					// 打开沙漏光标，因为读数据库数据时间可能比较长
	// 弹出标准品配置对话框
	if(m_CurrenSelectedMode == EmptyNode)
	{	
		CStandardConfigDlg dlg(true,SampleNodeList,ReagentNodeList);
		if(dlg.DoModal() == IDCANCEL)
			return ;
	}
	else
	{	
		CStandardConfigDlg dlg(false,SampleNodeList,ReagentNodeList);
		if(dlg.DoModal() == IDCANCEL)
			return ;
	}
	wait.Restore();

	// 更新查询表
	m_pSampleAreaData->UpdateSearchList();
	// 取消所有节点选择状态
	m_pSampleAreaData->CancelAllSelected();
	m_CurrenSelectedMode = NoneSelected;

	m_pSampleAreaData->SpreadDataUpdateMessage();
	// 重画所有节点
	InvalidateRect(&m_rcNodesRegion);
}

void CSampleAreaCtrl::OnClickMenuDeleteSample()
{
	// “删除样本(标准品)测试”菜单响应

	// 删除前确认
	if(MessageBox(_T("确定要删除选中位置的测试配置信息？"),_T("删除确认"),MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
		return;

	CGeneralFuction * pFunction = CGeneralFuction::GetInstance();
	for(int c = 1; c <= NUM_COL; c++)
	{
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode * pNode = m_pSampleAreaData->GetNode(c,r);
			if(pNode->IsSelected)
			{ 
				pNode->IsSelected = false;
				pNode->IsSetingSample = false;
				pNode->IsStandard = false;
				pNode->RecordID = 0;
				pNode->StdPointIndex = -1;
				pNode->TestProjArr.clear();

				// 更新样本提示文本
				//_UpdateSampleTipText(m_pSampleAreaData->GetNumByCoord(c,r));
				_InvalidNode(c,r);
			}
		}
	}
	m_CurrenSelectedMode = NoneSelected;
	m_pSampleAreaData->SpreadDataUpdateMessage();
	// 更新查询表
	m_pSampleAreaData->UpdateSearchList();
}

void CSampleAreaCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	_DisplayMenu();
	CStatic::OnRButtonDown(nFlags, point);
}

void CSampleAreaCtrl::_DisplayMenu()
{
	if(m_pMenu != NULL)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}

	m_pMenu = new CMenu();
	m_pMenu->CreatePopupMenu();

	switch(m_CurrenSelectedMode)
	{
	case NoneSelected:
		{
		}
		break;
	case SampleNode:
		{
			m_pMenu->AppendMenu(MF_STRING,IDM_Sample_Config,_T("修改样本测试..."));
			m_pMenu->AppendMenu(MF_STRING,IDM_Delete_Sample,_T("删除样本"));
			break;
		}
	case StanderdNode:
		{
			m_pMenu->AppendMenu(MF_STRING,IDM_Standard_Config,_T("修改标准品测试..."));
			m_pMenu->AppendMenu(MF_STRING,IDM_Delete_Sample,_T("删除标准品"));
			break;
		}
	case EmptyNode:
		{
			m_pMenu->AppendMenu(MF_STRING,IDM_Standard_Config,_T("添加标准品..."));
			m_pMenu->AppendMenu(MF_STRING,IDM_Sample_Config,_T("添加样本..."));
		}
		break;
	}
	m_pMenu->AppendMenu(MF_SEPARATOR);
	m_pMenu->AppendMenu(MF_STRING,IDM_All_Deselect,_T("取消选择"));

	CPoint pt;
	GetCursorPos(&pt);
	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x + 15,pt.y + 15,this);
}

void CSampleAreaCtrl::_DisplayPromptMenu()
{
	if(m_pMenu != NULL)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}

	m_pMenu = new CMenu();
	m_pMenu->CreatePopupMenu();
	m_pMenu->AppendMenu(MF_STRING,0,_T("本节点与已选节点类型不同，"));
	m_pMenu->AppendMenu(MF_STRING,1,_T("不能同时选择！"));

	CPoint pt;
	GetCursorPos(&pt);
	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x + 15,pt.y + 15,this);
}

void CSampleAreaCtrl::_WhichNode(const CPoint & pt, int &col, int &row) const
{
	// 如果没有击中，则行列返回负值
	col = -1;
	row = 0;
	for(int c = 0; c<=NUM_COL; c++)
	{
		for(int r=1; r <= NUM_ROW + 1; r++)
		{
			RECT noderect;
			_GetNodeRect(c,r,noderect);

			// 为防止鼠标击中测试过于灵敏，减小一点击中矩形
			noderect.top += 2;
			noderect.bottom -= 2;
			noderect.left += 2;
			noderect.right -= 2;

			if(PtInRect(&noderect, pt))
			{
				col = c;
				row = r;
				return;
			}
		}
	}
}

BOOL CSampleAreaCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

	// 提示工具的ID就是位置坐标
	UINT nPos = static_cast<UINT>(pNMHDR->idFrom);
	int col,row;
	m_pSampleAreaData->GetCoordByNum(nPos,col,row);
	SampleConfigNode * pNode = m_pSampleAreaData->GetNode(col,row);

	do 
	{
		m_mapToolTipTexts[pNode->PosNum] = _T("");
		// 如果没有试管
		if(pNode->IsThereTube == false)
		{
			m_mapToolTipTexts[pNode->PosNum] = _T("此处无试管");
			break;
		}

		// 如果没有没有设置样本
		if(pNode->IsSetingSample == false)
		{
			// 如果在配置界面，则提示为“点击添加样本”
			if(m_ControlType == saForConfig)
				m_mapToolTipTexts[pNode->PosNum] = _T("点击添加样本");
			// 如果在概况界面，提示为“为配置样本信息”
			else
				m_mapToolTipTexts[pNode->PosNum] = _T("未配置样本信息");
			break;
		}

		// 如果不是标准品
		if(pNode->IsStandard == false)
		{
			if(m_ControlType == saForConfig)
				m_mapToolTipTexts[pNode->PosNum] = _T("点击修改样本信息");
			else
				_UpdateSampleTipText(pNode);
			break;
		}
		// 如果是标准品
		if(pNode->IsStandard == true)
		{
			if(m_ControlType == saForConfig)
				m_mapToolTipTexts[pNode->PosNum] = _T("点击修改标准品信息");
			else
				_UpdateStandardTipText(pNode);
			break;
		}

		break;
	} while (TRUE);

	pTTT->lpszText = m_mapToolTipTexts[pNode->PosNum].AllocSysString();

	return TRUE;
}


BOOL CSampleAreaCtrl::_UpdateStandardTipText(const SampleConfigNode * pNode)
{
	// 取得通用函数类和标准品数据库表指针
	CStandardTable *pStandardTable = CStandardTable::GetInstance();

	// 取得标准品记录信息
	StandardRecord StandardInfo = pStandardTable->GetRecordInfo(pNode->RecordID);

	CString strTipText;

	// 显示“标准品批号”信息
	CString strBatchNum;
	strBatchNum.Format(_T("标准品　：　%s\n"),StandardInfo.BatchNum);
	strTipText += strBatchNum;

	// 显示“标准浓度”信息
	CString strPoint;
	int stdindex = pNode->StdPointIndex;
	strPoint.Format(_T("标准浓度：　%g %s\n"), StandardInfo.StdConc[stdindex],StandardInfo.Unit);
	strTipText += strPoint;

	// 显示“检测项目”信息
	CString strProjectName;
	strProjectName.Format(_T("检测项目：　%s（%s）\n"),StandardInfo.ShortName,StandardInfo.ProjectName);

	strTipText += strProjectName;

	// 显示“重复次数”信息
	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::const_iterator iter = pNode->TestProjArr.begin();
	CString strRepeate;
	strRepeate.Format(_T("重复次数：　%d"),iter->second.Repetitions);
	strTipText += strRepeate;

	m_mapToolTipTexts[pNode->PosNum] = strTipText;
	return TRUE;
}

BOOL CSampleAreaCtrl::_UpdateSampleTipText(const SampleConfigNode * pNode)
{
	CSampleTable *pSampleTable = CSampleTable::GetInstance();
	CReagentTable *pReagentTable = CReagentTable::GetInstance();

	SampleRecord SampleInfo = pSampleTable->GetRecordInfo(pNode->RecordID);

	CString strTipText = _T("");

	// 显示“样本编号”信息
	CString strSampleNo;
	strSampleNo.Format(_T("样本编号：%s\n"),SampleInfo.SampleNo);
	strTipText += strSampleNo;

	// 遍历“检测项目数组”，显示各个检测项目信息
	map<Reagent_Pos,SampleConfigNode::TestProjInfo>::const_iterator iter = pNode->TestProjArr.begin();
	int i = 1;

	// 如果样本无任何测试项目
 	if(iter == pNode->TestProjArr.end())
	{
		CString strProjectName;
		strProjectName = _T("检测项目：无");
		strTipText += strProjectName;
	}
	else
	{
		while(iter != pNode->TestProjArr.end())
		{
			CString strProjectName;
			Reagent_Key ReagentID = m_pReagentAreaData->GetNode(iter->second.ReagentPos)->ReagentID;
			ReagentRecord reagentinfo = pReagentTable->GetRecordInfo(ReagentID);
			strProjectName.Format(_T("检测项目%d：%s(%s)"),i,reagentinfo.ShortName,reagentinfo.ProjectName);
			strTipText += strProjectName;

			CString strRepeate;
			strRepeate.Format(_T("，测试 %d 次\n"),iter->second.Repetitions);
			strTipText += strRepeate;

			iter++;
			i++;
		}
	}
	m_mapToolTipTexts[pNode->PosNum] = strTipText;

	return TRUE;
}

void CSampleAreaCtrl::_OnClickColumnHeader(const int &col)
{
	// 单击样本区示意图列标题的响应函数

	// 判断当前选择类型
	switch(m_CurrenSelectedMode)
	{
	case NoneSelected:
		// 如果该列有空节点，则设置为空节点选择状态，否则不反应
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode *pNode = m_pSampleAreaData->GetNode(col,r);
			if(pNode->IsThereTube == false)
				continue;;
			if(pNode->IsSetingSample)
				continue;
			m_CurrenSelectedMode = EmptyNode;
			break;
		}

	case EmptyNode:
		// 如果当前没有被选的节点，或选择状态为“空节点选择”，则把本列所有空节点设置为选择状态
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode *pNode = m_pSampleAreaData->GetNode(col,r);
			if(pNode->IsThereTube == false)
				continue;;
			if(pNode->IsSetingSample)
				continue;
			pNode->IsSelected =true;
			_InvalidNode(col,r);
		}
		break;

	case SampleNode:
		// 如果当前选择状态为“样本节点选择”，则把本列所有样本节点设置为选择状态
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode *pNode = m_pSampleAreaData->GetNode(col,r);
			if(pNode->IsThereTube == false)
				continue;;
			if(pNode->IsSetingSample == false)
				continue;
			if(pNode->IsStandard == true)
				continue;
			pNode->IsSelected =true;
			_InvalidNode(col,r);
		}
		break;
	case StanderdNode:
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode *pNode = m_pSampleAreaData->GetNode(col,r);
			if(pNode->IsThereTube == false)
				continue;;
			if(pNode->IsSetingSample == false)
				continue;
			if(pNode->IsStandard == false)
				continue;
			pNode->IsSelected =true;
			_InvalidNode(col,r);
		}
		break;
	}
	// 全选或全不选，
	// 如果本列有至少一个不是是选中状态，则单击后为全选状态；如果为全选状态，则单击后为全不选状态
}

void CSampleAreaCtrl::_OnClickEmptyNode(const int & col, const int &row)
{
	// 判断当前选择类型
	switch(m_CurrenSelectedMode)
	{
	case SampleNode:
	case StanderdNode:	
		_DisplayPromptMenu();
		return;
	case NoneSelected:
		m_CurrenSelectedMode = EmptyNode;
	case EmptyNode:
		{
			_NodeClickProcess(col,row);
		}
	}
}

void CSampleAreaCtrl::_OnClickSampleNode(const int & col, const int &row)
{
	// 判断当前选择类型
	switch(m_CurrenSelectedMode)
	{
	case EmptyNode:
	case StanderdNode:
		_DisplayPromptMenu();
		return;
	case NoneSelected:
		m_CurrenSelectedMode = SampleNode;
	case SampleNode:
		{
			_NodeClickProcess(col,row);
		}
	}
}

void CSampleAreaCtrl::_OnClickStanderdNode(const int & col, const int &row)
{
	// 判断当前选择类型
	SampleConfigNode * pNode = m_pSampleAreaData->GetNode(col,row);
	switch(m_CurrenSelectedMode)
	{
	case EmptyNode:
	case SampleNode:
		_DisplayPromptMenu();
		break;
	case NoneSelected:
		{
			m_CurrenSelectedMode = StanderdNode;
			m_CurrenSelectedStanderdKey = pNode->RecordID;
		}
	case StanderdNode:
		{
			if(_IsNodeCouldSelected(col,row))
				_NodeClickProcess(col,row);
			break;
		}
	default:
		return;
	}
}

void CSampleAreaCtrl::_NodeClickProcess(const int & col, const int &row)
{
	// 鼠标左键按下状态为ture，此时拖动将会连选,
	m_IsLButtonDown = true;
	// 设置连选起始节点坐标值
	m_SelectStartPos = m_pSampleAreaData->GetNumByCoord(col,row);

	// 保存当前选中状态，用于取消最后一次选择
	for(int c = 1; c <= NUM_COL; c++)
	{
		for(int r = 1; r <= NUM_ROW; r++)
		{
			SampleConfigNode * pNode = m_pSampleAreaData->GetNode(c,r);
			m_SelectState[c-1][r-1] = pNode->IsSelected;
		}
	}

	// 设置当前节点为选中状态
	SampleConfigNode * pCurrentNode = m_pSampleAreaData->GetNode(col,row);
	pCurrentNode->IsSelected = true;
	_InvalidNode(col,row);
}

bool CSampleAreaCtrl::_IsNodeCouldSelected(int col, int row)
{
	SampleConfigNode * pCurrentNode = m_pSampleAreaData->GetNode(col,row);
	if(pCurrentNode == NULL)
		return false;

	// 如果节点无试管，则不能被选中
	if(pCurrentNode->IsThereTube == false)
		return false;

	// 判断当前选择类型
	switch(m_CurrenSelectedMode)
	{
	// 如果当前选择状态是“选择空样本节点”
	case EmptyNode:
		{
			// 如果节点已经被设置了样本，则没有资格被选中
			if(pCurrentNode->IsSetingSample == true)
				return false;
			else
				return true;
		}
	// 如果当前选择状态是“样本节点”
	case SampleNode:
		{
			// 如果节点没有设置样本，则没有资格被选中
			if(pCurrentNode->IsSetingSample == false)
				return false;
			// 如果节点被设置成了标准品，则没有资格被选中
			if(pCurrentNode->IsStandard == true)
				return false;
			return true;
		}
	case StanderdNode:
		{
			// 如果节点没有设置样本，则没有资格被选中
			if(pCurrentNode->IsSetingSample == false)
				return false;
			// 如果节点被设置成了样本，则没有资格被选中
			if(pCurrentNode->IsStandard == false)
				return false;
			// 如果本标准品ID与选中状态标准品ID不同，则没有资格选中
			if(pCurrentNode->RecordID != m_CurrenSelectedStanderdKey)
				return false;
		}
	default:
		return true;
	}
}

void CSampleAreaCtrl::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect(m_rcClient);

	m_rcTitleRegion.top = m_rcClient.top;
	m_rcTitleRegion.left = m_rcClient.left;
	m_rcTitleRegion.right = m_rcClient.right;
	m_rcTitleRegion.bottom = (int)(m_rcClient.bottom * 0.08);

	m_rcNodesRegion.top = m_rcClient.top + (int)(m_rcClient.Height() * 0.2);
	m_rcNodesRegion.left = m_rcClient.left + 10;
	m_rcNodesRegion.right = m_rcClient.right - 10;

	// 取得水平和垂直方向单位长度内的像素数
	CPaintDC dc(this);
	float cxInch = (float)dc.GetDeviceCaps(LOGPIXELSX);
	float cyInch = (float)dc.GetDeviceCaps(LOGPIXELSY);
	m_rcNodesRegion.bottom = m_rcNodesRegion.top + (int)(m_rcNodesRegion.Width() * (cyInch / cyInch)* (11.0/*行*/ / 13.0/*列*/));

	// 如果提示工具还未创建，则创建
	if(m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl();
		m_pToolTip->Create(this);
		m_pToolTip->SetDelayTime(350);					// 延迟时间350毫秒
		m_pToolTip->SetMaxTipWidth(400);

		// 更新ToolTip
		for(int i=1; i <= NUM_SAMPLEPOS; i++)
		{
			int c,r;
			m_pSampleAreaData->GetCoordByNum(i,c,r);

			RECT rect;
			_GetNodeRect(c,r,rect);

			m_pToolTip->AddTool(this,LPSTR_TEXTCALLBACK ,&rect,i);
		}
	}
	// 如果提示工具已经创建，则更新矩形区域
	else
	{
		for(int i=1; i <= NUM_SAMPLEPOS; i++)
		{
			int c,r;
			m_pSampleAreaData->GetCoordByNum(i,c,r);

			RECT rect;
			_GetNodeRect(c,r,rect);

			m_pToolTip->SetToolRect(this,i,&rect);
		}
	}
}
