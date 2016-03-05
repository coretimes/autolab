//*****************************************************************************
// 文件名 : ReagentSelectListItem.h
// 
// 类名   : 试剂选择列表数据对象，本列表位于【选择试剂】弹出对话框上
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
#include "ReagentTable.h"
namespace ReagentSelectList
{
	enum ListColumn
	{
		colProject = 0,					// 检测项目
		colBatchNum,					// 试剂批号
		colProductDate,					// 生产日期
		ColumnCount
	};
}
using namespace ReagentSelectList;

class CReagentSelectListItem :	public CListItemData
{
public:
	CReagentSelectListItem(	const Reagent_Key & key,
							const CString & strBatchNum,
							const ProjectFullName & ProjectName,
							const COleDateTime & dtProductDate);
	~CReagentSelectListItem();

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
		return (int)ReagentSelectList::ColumnCount;
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
	//     取得项目名称
	// ---------------------------------------------------------------------------
	ProjectFullName GetProjectName() const
	{
		return m_ProjectName;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得批号
	// ---------------------------------------------------------------------------
	CString GetBatchName() const
	{
		return m_strBatchNum;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得试剂ID
	// ---------------------------------------------------------------------------
	Reagent_Key GetReagentKey()
	{
		return m_ReagentKey;
	}

private:
	Reagent_Key m_ReagentKey;
	CString m_strBatchNum;
	ProjectFullName m_ProjectName;
	COleDateTime m_dtProductDate;

private:
	static ListColumn SortColumn;		// 排序列名
	static bool IsAsc;					// 是否升序排，true是，false为降序
};
