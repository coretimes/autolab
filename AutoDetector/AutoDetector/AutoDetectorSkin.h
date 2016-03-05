//*****************************************************************************
// 文件名 : AutoDetectorSkin.h
// 
// 类名   : 主对话框皮肤绘制，当设置新皮肤时，只需根据此类扩展
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "skinobject.h"
#include <atlimage.h>
#include "BaseDialog.h"
class CAutoDetectorSkin :	public CSkinObject
{
public:
	CAutoDetectorSkin(CBaseDialog * pDlg);
	~CAutoDetectorSkin(void);
	
	virtual void DrawSkin(const CRect &rcClipArea);
private:
	CImage m_BannerLeft;				// 标题区域左横幅图片
	CImage m_BannerRight;				// 标题区域右横幅图片
	CImage m_BannerMiddle;				// 标题区域中间横幅图片

	CImage m_MaxButtonNomal;			// 最大化按钮普通状态图片
	CImage m_MaxButtonHot;				// 最大化按钮热点状态图片
	CImage m_MaxButtonPress;			// 最大化按钮按下状态图片
	CImage m_MinButtonNomal;			// 最小化按钮普通状态图片
	CImage m_MinButtonHot;				// 最小化按钮热点状态图片
	CImage m_MinButtonPress;			// 最小化按钮按下状态图片
	CImage m_CloseButtonNomal;			// 关闭按钮普通状态图片
	CImage m_CloseButtonPress;			// 关闭按钮按下状态图片
	CImage m_CloseButtonHot;			// 关闭按钮热点状态图片
	CImage m_ResButtonNomal;			// 还原按钮普通状态图片
	CImage m_ResButtonPress;			// 还原按钮按下状态图片
	CImage m_ResButtonHot;				// 还原按钮热点状态图片
	CImage m_SettingButtonNomal;		// 设置按钮普通状态图片
	CImage m_SettingButtonPress;		// 设置按钮按下状态图片
	CImage m_SettingButtonHot;			// 设置按钮热点状态图片

	CBaseDialog * m_pDialog;			// 对话框
private:
	void DrawButtons(CDC*pDC);
};
