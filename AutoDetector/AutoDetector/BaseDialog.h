//*****************************************************************************
// 文件名 : BaseDialog.h
// 
// 类名   : 自定义的对话框类基类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
//   本基类对话框主要负责：
//	 1.大小调整时，子控件的重定位
//   2.皮肤功能：非客户区域大小计算以及绘制接口实现
//*****************************************************************************

#pragma once
#include <vector>
#include "SkinObject.h"
using namespace std;

// “坐标变化方法”类型
enum CoordTransMode
{
	tmNotChange = 0,				// 不变
	tmQuantity,						// 定量缩放
	tmProportion					// 定比例缩放
};

// 控件重定位属性
struct ControlsResizeProp
{
	UINT ID;						// 控件ID
	CPoint ptOrigPosBps;			// 初始定位基点
	CRect rcInitPosRect;			// 控件初始位置矩形，根据资源模板取得
	CoordTransMode ctmXofPosBps;	// 定位基点x值变化方法
	CoordTransMode ctmYofPosBps;	// 定位基点y值变化方法
	CoordTransMode ctmWigth;		// 宽度变化方法
	CoordTransMode ctmHight;		// 长度变化方法
	bool IsPaintBackGround;			// 是否自绘背景，如果为false，则由父窗口填充背景，默认为true
	ControlsResizeProp()
	{
		ptOrigPosBps = CPoint(0,0);
		rcInitPosRect = CRect(0,0,0,0);
		ctmXofPosBps = tmNotChange;
		ctmYofPosBps = tmNotChange;
		ctmWigth = tmNotChange;
		ctmHight = tmNotChange;
		IsPaintBackGround = true;	
	}
};

// 窗口非客户区按钮状态
enum CButtonState
{
	bsNormal,					// 普通状态
	bsHot,						// 热点状态 
	bsPress						// 按下状态 
};

// 窗口非客户区按钮ID
enum CButtonID
{
	btClose = 1,
	btMaxOrRestore,
	btMin,
	btSetting
};
//*********************************
// 可调大小对话框类
//*********************************
class CBaseDialog : public CDialog
{
	DECLARE_DYNAMIC(CBaseDialog)
public:
	explicit CBaseDialog(const UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CBaseDialog();

	// --------------------------------------------------
	// 摘要：
	//	  设置控件重定位属性
	// 参数：
	//     1.newItem - 控件重定位属性
	// --------------------------------------------------
	void SetResize(ControlsResizeProp &Item);

	// --------------------------------------------------
	// 摘要：
	//	  取得边框高度
	// --------------------------------------------------
	int GetBorderHeight()
	{
		return m_BorderHeight;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得边框宽度
	// --------------------------------------------------
	int GetBorderWidth()
	{
		return m_BorderWidth;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得标题栏高度
	// --------------------------------------------------
	int GetCaptionHeight()
	{
		return m_CaptionHeight;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得窗口大小矩形，相对坐标
	// --------------------------------------------------
	CRect & GetWindSizeRect()
	{
		return m_rcWndSize;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得左边框矩形
	// --------------------------------------------------
	CRect & GetLeftBorderRect()
	{
		return m_rcLeftBorder;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得右边框矩形
	// --------------------------------------------------
	CRect & GetRightBorderRect()
	{
		return m_rcRightBorder;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得下边框矩形
	// --------------------------------------------------
	CRect & GetBottomBorderRect()
	{
		return m_rcBottomBorder;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得关闭按钮矩形区域
	// --------------------------------------------------
	CRect & GetCloseButtonRect()
	{
		return m_rcCloseButton;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得最大化窗口按钮矩形区域
	// --------------------------------------------------
	CRect & GetMaxButtonRect()
	{
		return m_rcMaxButton;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得设置按钮矩形
	// --------------------------------------------------
	CRect & GetSettingButtonRect()
	{
		return m_rcSettingButton;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得最小化窗口按钮矩形区域
	// --------------------------------------------------
	CRect & GetMinButtonRect()
	{
		return m_rcMinButton;
	}

	// --------------------------------------------------
	// 摘要：
	//	  取得最大化按钮状态
	// --------------------------------------------------
	CButtonState & GetMaxButtonState()
	{
		return m_MaxButtonState;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得最小按钮状态
	// --------------------------------------------------
	CButtonState & GetMinButtonState()
	{
		return m_MinButtonState;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得关闭按钮状态
	// --------------------------------------------------
	CButtonState & GetCloseButtonState()
	{
		return m_CloseButtonState;
	}
	// --------------------------------------------------
	// 摘要：
	//	  取得设置按钮状态
	// --------------------------------------------------
	CButtonState & GetSettingButtonState()
	{
		return m_SettingButtonState;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	// --------------------------------------------------
	// 摘要：
	//	  大小改变前的消息响应函数
	// --------------------------------------------------

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	// --------------------------------------------------
	// 摘要：
	//	  大小改变后的消息响应函数
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// 摘要：
	//	  擦除背景消息响应
	// --------------------------------------------------
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------
	// 摘要：
	//	  非客户区绘制
	// --------------------------------------------------
	afx_msg void OnNcPaint();

	// --------------------------------------------------
	// 摘要：
	//	  非客户区激活
	// --------------------------------------------------
	afx_msg BOOL OnNcActivate(BOOL bActive);

	// --------------------------------------------------
	// 摘要：
	//	  非客户区大小计算
	// --------------------------------------------------
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	// --------------------------------------------------
	// 摘要：
	//	  非客户区鼠标消息响应
	// --------------------------------------------------
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	  窗口位置发生变化消息响应
	// --------------------------------------------------
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	// --------------------------------------------------
	// 摘要：
	//	  初始化对话框函数
	// --------------------------------------------------
	virtual BOOL OnInitDialog();
	
	// --------------------------------------------------
	// 摘要：
	//	  根据重定位类型、伸缩类型、原定位基点等属性计算新矩形区域
	//     1.tagCtrtPosAndSize - 原位置大小属性
	//     2.RateX - x轴伸缩比例
	//     3.RateY - y轴伸缩比例
	// --------------------------------------------------
	CRect _GetNewRect(const ControlsResizeProp &newItem);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CSize m_WndInitSize;						// 窗口原始大小
	CSize m_WndNewSize;							// 窗口新大小
	vector<ControlsResizeProp> m_vectCtrls;		// 控件大小位置属性
	CRgn m_rgnBackGround;						// 控件区域以外的背景区域指针

	int m_BorderWidth;							// 边界线宽度
	int m_BorderHeight;							// 边界线高度
	int m_CaptionHeight;						// 标题栏的高度
	CRect m_rcWndSize;							// 窗口区域大小，即左上角为（0，0）的坐标，利用WindowsDC时使用			
	CRect m_rcCaption;							// 标题栏相对窗口左上角坐标
	CRect m_rcClient;							// 客户区域相对窗口左上角坐标
	CRect m_rcLeftBorder;						// 左边框矩形相对窗口左上角坐标
	CRect m_rcRightBorder;						// 右边框矩形相对窗口左上角坐标
	CRect m_rcBottomBorder;						// 下边框矩形相对窗口左上角坐标

	CRect m_rcMaxButton;						// 最大化按钮矩形，相对窗口左上角坐标
	CRect m_rcMinButton;						// 最小化按钮矩形，相对窗口左上角坐标
	CRect m_rcCloseButton;						// 关闭按钮矩形，相对窗口左上角坐标
	CRect m_rcSettingButton;					// 设置按钮矩形，相对窗口左上角坐标

	CButtonState m_MaxButtonState;				// 最大化按钮状态
	CButtonState m_MinButtonState;				// 最小化按钮状态
	CButtonState m_CloseButtonState;			// 关闭按钮状态
	CButtonState m_SettingButtonState;			// 设置按钮状态

	CSkinObject * m_pSkin;						// 皮肤指针
	CToolTipCtrl m_ToolTip;						// 提示工具
};
