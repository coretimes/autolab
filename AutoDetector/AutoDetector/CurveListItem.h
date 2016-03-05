//*****************************************************************************
// 文件名 : CurveListItem.h
// 
// 类名   : 曲线选择列表数据对象，本列表位于【标曲】对话框上
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
namespace CurveList
{
	enum ListColumn
	{
		colProject = 0,				// 检测项目
		colBatchNum,				// 标准批号
		colTestDate,				// 测试日期
		colDefaultFitType,			// 默认拟合模型
		colSurveyor,				// 检验员
		ColumnCount
	};
}

using namespace CurveList;

class CCurveListItem : public CListItemData
{
public:
	CCurveListItem(const CurveRecord & curve);
public:
	~CCurveListItem(void);

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
	//     判断本项是否位于另外某项数据之前,如果是，则需要把本数据与pItemData交换顺序
	// 参数：
	//     1.pItemData - 比较项指针
	// ---------------------------------------------------------------------------
	virtual bool IsBeforThan(const CListItemData * pItemData);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     改变排序方式函数
	// ---------------------------------------------------------------------------
	virtual bool ChangeSortMode(const int & col);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得子项目个数
	// ---------------------------------------------------------------------------
	virtual inline int GetSubItemCount()
	{
		return (int)CurveList::ColumnCount;
	}
	
	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得测试时间
	// ---------------------------------------------------------------------------
	COleDateTime GetTestTime() const
	{
		return m_CurveRecord.TestDate;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得曲线key
	// ---------------------------------------------------------------------------
	Curve_Key GetCurveKey() const
	{
		return m_CurveRecord.ID;
	}

private:
	CurveRecord m_CurveRecord;
	CString m_strProjectName;
	CString m_strBatchNum;

private:
	static ListColumn SortColumn;		// 排序列名
	static bool IsAsc;					// 是否升序排，true是，false为降序
};
