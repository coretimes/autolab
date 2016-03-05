//*****************************************************************************
// 文件名 : TestConfirmListItem.h
// 
// 类名   : 测试确认列表控件数据项，本列表位于【测试信息确认】对话框上
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-1        崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "listitemdata.h"

namespace TestConfirmList
{
	enum ListColumn
	{
		colType = 0,				// 样品类型
		colSampleNo,				// 样品编号
		colConcPoint,				// 浓度点
		colSamplePos,				// 样本位
		colProject,					// 检测项目
		colRepeatTimes,				// 重复次数
		ColumnCount
	};
}
using namespace TestConfirmList;

class CTestConfirmListItem :	public CListItemData
{
public:
	CTestConfirmListItem(const Sample_Pos & SamplePos,const Reagent_Pos &ReagentPos);
public:
	~CTestConfirmListItem(void);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     判断本项某字段是否与另外某项数据同字段相等，
	//     当相等时，绘制表格时需要合并为一个单元格
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.pItemData - 上列表项数据指针
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData);

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
	//     取得子项目个数
	// ---------------------------------------------------------------------------
	virtual int GetSubItemCount()
	{
		return (int)ColumnCount;
	}

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
	//     改变排序方式函数
	// ---------------------------------------------------------------------------
	Sample_Pos GetSamplePosNum() const
	{	
		return m_SamplePos;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得试剂位置编号
	// ---------------------------------------------------------------------------
	Reagent_Pos GetReagentPosNum() const
	{	
		return m_ReagentPos;
	}

private:
	Sample_Pos m_SamplePos;
	Reagent_Pos m_ReagentPos;

private:
	static ListColumn SortColumn;		// 排序列名
	static bool IsAsc;					// 是否升序排，true是，false为降序
};
