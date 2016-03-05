//*****************************************************************************
// 文件名 : CurveDetailListItem.h
// 
// 类名   : 曲线详细信息列表项数据，本列表位于【标曲】对话框上
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-2        崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "listitemdata.h"
#include "CurveTable.h"

namespace CurveDetailListItem
{
	enum ListColumn
	{
		colIndex = 0,			// 标准点索引
		colTheoryConc,			// 理论浓度
		colTestRlu,				// 测量发光值
		colAverageRlu,			// 平均发光值
		colCalConc,				// 计算浓度
		colFitError,			// 拟合误差
		colCV,					// 变异系数
		colInhibitionRate,		// 抑制率
		ColumnCount
	};
}
using namespace CurveDetailListItem;

class CCurveDetailListItem : public CListItemData
{
public:
	CCurveDetailListItem(int nConcIndex, 
						 double dTheoryConc,
						 CString strUnit,
						 double dTestRlu,
						 double dAverageRlu,
						 double dCalConc,
						 double dFitError,
						 double dCV,
						 double dInhibitionRate);

	~CCurveDetailListItem(void);
	// ---------------------------------------------------------------------------
	// 摘要：
	//     判断本项某字段是否与另外某项数据同字段相等，
	//     当相等时，绘制表格时需要合并为一个单元格
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.pItemData - 上列表项数据指针
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
	{
		// 本列表没有合并单元格
		return false;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得某单元格文本内容
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.contex - 文本内容
	// 返回值：
	//     文本对齐格式
	// ---------------------------------------------------------------------------
	virtual UINT GetCellContex(const int &ColumnIndex,CString & contex);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得某列属性
	// ---------------------------------------------------------------------------
	virtual ColumnProperties GetColumnProperties(const int &ColumnIndex)
	{
		return CanNotChange;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得子项目个数
	// ---------------------------------------------------------------------------
	virtual inline int GetSubItemCount()
	{
		return (int)CurveDetailListItem::ColumnCount;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得浓度点索引
	// ---------------------------------------------------------------------------
	int GetConcIndex() const
	{
		return m_nConcIndex;
	}

private:
	int m_nConcIndex;
	double m_dTheoryConc;
	CString m_strUint;
	double m_dTestRlu;
	double m_dAverageRlu;
	double m_dCalConc;
	double m_dFitError;
	double m_dCV;
	double m_dInhibitionRate;
};