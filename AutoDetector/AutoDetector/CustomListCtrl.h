//*****************************************************************************
// 文件名 : CustomListCtrl.h
// 
// 类名   : 自定义列表视图控件
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 1.本列表视图控件可实现若干同列单元格的合并效果
//
// 2.每条列表项均有一个指向CListItemData类型数据的指针
//
// 3.判断某条列表项的某个单元格是否与上条列表项的同列单元格合并，
//   是由CListItemData数据判断决定的
// 
// 4.某个单元格是否可编辑也是由CListItemData数据判断决定的
//
// 5.列名和列宽度由创建者（父对话框）决定，当列宽度设为0时，可隐藏该列
//
// 6.当指针指向列表某个单元格时，可显示浮动控件
//   如：编辑框、“删除”按钮、“编辑”按钮、组合框、微调按钮等等
//   具体显示哪个浮动控件，根据CListItemData数据判断
// 
// 7.单击列表标头，可以实现排序功能，排序方式由数据CListItemData决定
//*****************************************************************************
#pragma once
#include "ListItemData.h"

//***************************************
// 浮动编辑框控件属性
//***************************************
struct FloatEdit
{
	CEdit * pCtrlEdit;					// 控件指针
	CString Context;					// 编辑控件内容
	int row;							// 控件所在行索引
	int col;							// 控件所在列索引
};

//***************************************
// 浮动组合框控件属性
//***************************************
struct FloatCombox
{
	CComboBox * pCtrlComboBox;			// 控件指针
	vector<CString> vctOptions;			// 组合框的选项内容数组
	bool IsEditable;					// 是否可编辑
	int DefaultSelectIndex;				// 默认索引
	int row;							// 控件所在行索引
	int col;							// 控件所在列索引
};

//***************************************
// 浮动旋转按钮（微调按钮）控件属性
//***************************************
struct FloatSpin
{
	CSpinButtonCtrl * pSpinButton;		// 控件指针
	CEdit * pBuddyEdit;					// 绑定编辑控件指针
	double InitValue;					// 微调按钮的初始值
	double IncreamentOfSpin;			// 微调按钮的增量变量
	double MaxValue;					// 微调按钮的上限
	double MinValue;					// 微调按钮的下限
	int row;							// 控件所在行索引
	int col;							// 控件所在列索引
};

//***************************************
// 浮动按钮控件属性
//***************************************
struct FloatButton
{
	enum ButtonFuncionCode				// 按钮功能码枚举
	{
		fcDelete = 0,					// "删除"
		fcEdit							// "编辑"
	} FuntionCode;

	CButton * pButton;					// 控件指针
	CString strCation;					// 标题
	int row;							// 控件所在行索引
	int col;							// 控件所在列索引
};

//***************************************
// 日期控件属性
//***************************************
struct FloatDateCtrl
{
	CDateTimeCtrl * pCtrlDate;			// 控件指针
	COleDateTime DefaultDate;			// 默认时间
	int row;							// 控件所在行索引
	int col;							// 控件所在列索引
};

//***************************************
// 控件列宽属性
//***************************************
struct ColumnProperty
{
	int ColumnIndex;					// 列索引
	int Width;							// 宽度值，如果宽度比例WidthRatio为非0值，则此值表示最小宽度值
	double WidthRatio;					// 宽度比例，如果为0，则列宽度值恒为Width

};

//***************************************
// 自定义列表控件
//***************************************
class CCustomListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CCustomListCtrl)
public:
	CCustomListCtrl();
	virtual ~CCustomListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// --------------------------------------------------
	// 摘要：
	//	   增加列表项
	// 参数：
	//     1.nItem - 位置索引
	//     2.pItemData - CListItemData类型数据指针
	// --------------------------------------------------
	void AddItem(int nItem,CListItemData * pItemData);

	// --------------------------------------------------
	// 摘要：
	//	   设置列表是否允许被选中
	// --------------------------------------------------
	void EnableSelect(BOOL IsEnableSelect)
	{
		m_bCanBeSelected = IsEnableSelect;
	}
	// --------------------------------------------------
	// 摘要：
	//	   设置列表项高度
	// 参数：
	//     1.height - 高度值
	// --------------------------------------------------
	void SetItemHeight(const LONG &height);

	// --------------------------------------------------
	// 摘要：
	//	   重画所有列表项
	// --------------------------------------------------
	void RedrawAllItems(void)
	{
		RedrawItems(0,GetItemCount()-1);
	}

	// --------------------------------------------------
	// 摘要：
	//	   删除所有项并且删除关联数据,
	//     而CListc::DeleteAllItems不删除数据
	// --------------------------------------------------
	BOOL DeleteAllItemsAndDatas();

	// --------------------------------------------------
	// 摘要：
	//	   删除指定所有的多行
	// --------------------------------------------------
	BOOL DeleteMultItems(vector<int> &vctDeleteIndexes);

	// --------------------------------------------------
	// 摘要：
	//	   取得列数量
	// --------------------------------------------------
	int GetColumnCount() const
	{
		return m_HeaderCtrl.GetItemCount();
	}

	// --------------------------------------------------
	// 摘要：
	//	   判断某单元格是否与上单元格合并
	// --------------------------------------------------
	bool IsCellMergeWithTop(const int& row,const int& col) const
	{
		if(row <= 0)
			return false;
		CListItemData * pThisItem = reinterpret_cast<CListItemData *>(GetItemData(row));
		CListItemData * pTopItem = reinterpret_cast<CListItemData *>(GetItemData(row - 1));
		return pThisItem->IsFieldEquivalent(col,pTopItem);
	}

	// --------------------------------------------------
	// 摘要：
	//	   判断某单元格是否与下单元格合并
	// --------------------------------------------------
	bool IsCellMergeWithBottom(const int& row,const int& col) const
	{
		if(row >= GetItemCount() - 1)
			return false;
		CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(row));
		CListItemData * pBottomItem = reinterpret_cast<CListItemData *> (GetItemData(row + 1));
		return pThisItem->IsFieldEquivalent(col,pBottomItem);
	}

	// --------------------------------------------------
	// 摘要：
	//	   重新排序函数：排序列和排序方式由ListItemData决定
	// --------------------------------------------------
	void Reorder();

	// --------------------------------------------------
	// 摘要：
	//	   设置列宽属性
	// 参数：
	//	   1.columnProp - 列属性
	// --------------------------------------------------
	void SetColumnWidthProp(ColumnProperty columnProp);

protected:
	DECLARE_MESSAGE_MAP()

	// --------------------------------------------------
	// 摘要：
	//	   WM_MEASUREITEM消息响应函数
	// --------------------------------------------------
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		lpMeasureItemStruct->itemHeight = m_ItemHeight;	    // 行高设置
	}

	// --------------------------------------------------
	// 摘要：
	//	   WM_SIZE消息响应函数
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// 摘要：
	//	   鼠标左键按下消息响应函数
	// --------------------------------------------------
	afx_msg void OnLButtonDown(uint32 nFlags, CPoint point);

	// --------------------------------------------------
	// 摘要：
	//	   鼠标滑轮滚动消息响应函数
	// --------------------------------------------------
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	// --------------------------------------------------
	// 摘要：
	//	   水平滚动条滚动消息响应函数
	// --------------------------------------------------
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// --------------------------------------------------
	// 摘要：
	//	   垂直滚动条滚动消息响应函数
	// --------------------------------------------------
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// --------------------------------------------------
	// 摘要：
	//	   标头拖动消息响应
	// --------------------------------------------------
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// 摘要：
	//	   标头单击消息响应
	// --------------------------------------------------
	afx_msg void OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// 摘要：
	//	   子类化前的初始化函数
	// --------------------------------------------------
	virtual void PreSubclassWindow();

	// --------------------------------------------------
	// 摘要：
	//	   控制指定单元格上浮动控件的显示状态
	// 参数：
	//     1.row - 行索引
	//     2.col - 列索引
	// --------------------------------------------------
	void _ControlFloatShowState(const int &row, const int &col);

	// --------------------------------------------------
	// 摘要：
	//	   显示编辑框
	// --------------------------------------------------
	void _ShowEditCtrl(FloatEdit & newEditCtrl);

	// --------------------------------------------------
	// 摘要：
	//	   浮动编辑框内容变化响应函数
	// --------------------------------------------------
	afx_msg void OnEnChangeEdit();

	// --------------------------------------------------
	// 摘要：
	//	   显示按钮
	// --------------------------------------------------
	void _ShowButton(FloatButton & newButton);

	// --------------------------------------------------
	// 摘要：
	//	   浮动按钮单击响应消息
	// --------------------------------------------------
	afx_msg void OnFloatBtnClk(void);

	// --------------------------------------------------
	// 摘要：
	//	   显示组合框
	// --------------------------------------------------
	void _ShowCombox(FloatCombox newCombox);

	// --------------------------------------------------
	// 摘要：
	//	   浮动组合框选项变化响应函数
	// --------------------------------------------------
	afx_msg void OnComboxSelChange(void);

	// --------------------------------------------------
	// 摘要：
	//	   显示微调按钮及绑定的编辑框
	// --------------------------------------------------
	void _ShowSpin(FloatSpin newSpin);

	// --------------------------------------------------
	// 摘要：
	//	   浮动微调按钮单击响应函数
	// --------------------------------------------------
	afx_msg void OnDeltaposSpinBtn(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// 摘要：
	//	   显示时间空间
	// --------------------------------------------------
	void _ShowDateCtrl(FloatDateCtrl newDateCtrl);

	// ---------------------------------------------------------
	// 函数介绍：
	//     “送检日期”变化响应函数
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeFloatDate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// 函数介绍：
	//     销毁函数，销毁之前先删除关联数据
	// ---------------------------------------------------------
	afx_msg void OnDestroy();

	// --------------------------------------------------
	// 摘要：
	//	   隐藏所有浮动控件
	// --------------------------------------------------
	void _HideAllFloatCtrl();

	// --------------------------------------------------
	// 摘要：
	//	   根据像素点取得单元格的索引
	// 参数：
	//     1.pt - 像素点
	//     2.row - 行索引
	//     3.col - 列索引
	// --------------------------------------------------
	bool _GetCellIndex(const CPoint& pt, int& row, int& col);

	// --------------------------------------------------
	// 摘要：
	//	   根据实际行列索引，取得合并单元格的首行和末行
	//     1.row - 实际行索引
	//     2.col - 实际列索引
	//     3.FirstRowInMerges - 如果是合并单元格，则值为合并单元格的首行，
	//                          如果不是合并单元格，则等于row值
	//     4.LastRowInMerges - 如果是合并单元格，则值为合并单元格的末行，
	//                         如果不是合并单元格，则等于row值
	// 参数：
	// --------------------------------------------------
	void _GetMergeCellRows(const int& row,const int& col,int &FirstRowInMerges,int &LastRowInMerges);

	// --------------------------------------------------
	// 摘要：
	//	   取得单元格矩形
	// --------------------------------------------------
	void _GetCellRect(const int & row, const int &col,CRect &rect)
	{
		GetSubItemRect(row,col,LVIR_LABEL,rect);
		if( col == 0)
			rect.left -= 4;
	}

	// --------------------------------------------------
	// 摘要：
	//	   根据像素点，取得合并单元格的矩形
	// 参数：
	//     1.pt - 像素点
	// --------------------------------------------------
	void _GetMergeCellRect(const int & row, const int &col,CRect &rect);

	// --------------------------------------------------
	// 摘要：
	//	   排序
	// 参数：
	//     1.pt - 像素点
	// --------------------------------------------------
	static bool _SortFunc(CListItemData * v1, CListItemData * v2)
	{
		return v1->IsBeforThan(v2);
	}

protected:
	vector<ColumnProperty> m_ColumnWidth;	// 列宽属性数组

	LONG m_ItemHeight;						// 列表行高
	int m_FocusRow;							// 焦点行索引
	int m_FocusCol;							// 焦点列索引
	bool m_bMouseTrack;						// 标识是否追踪当前鼠标状态，之所以要这样定义
											// 是要避免鼠标已经在窗体之上时，
											// 一移动鼠标就不断重复产生 WM_MOUSEHOVER

	CHeaderCtrl m_HeaderCtrl;				// 表头控件指针
	vector<FloatEdit> m_vctEditCtrl;		// 浮动编辑控件数组
	vector<FloatCombox> m_vctCombox;		// 浮动组合控件数组
	vector<FloatButton> m_vctButton;		// 浮动按钮指针数组
	vector<FloatSpin> m_vctSpinBtn;			// 浮动微调按钮控件数组
	vector<FloatDateCtrl> m_vctDateCtrl;	// 浮动时间控件数组

	BOOL m_bCanBeSelected;					// 是否允许被选中

public:
	COLORREF m_GridFrameColor;				// 边框颜色
	COLORREF m_SelectedBackColor;			// 选中背景颜色
};