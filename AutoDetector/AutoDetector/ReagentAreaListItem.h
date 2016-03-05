//*****************************************************************************
// 文件名 : ReagentAreaListItem.h
// 
// 类名   : 试剂区配置列表项数据,本列表位于【测试】页上的【试剂区配置】对话框上
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-1       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "listitemdata.h"

namespace ReagentAreaList
{
	enum ListColumn
	{
		colPos = 0,					// 位置编号
		colProject,					// 检测项目
		colBatchNum,				// 生产批号
		colSampleAmount,			// 样本用量
		colMagnetciAmount,			// 磁标用量
		colHRPAmount,				// 酶标用量
		colDilution,				// 稀释倍数
		colWashTimes,				// 清洗次数
		colIncuTime,				// 温育时间
		ColumnCount
	};
}
using namespace ReagentAreaList;

class CReagentAreaListItem :public CListItemData
{
public:
	CReagentAreaListItem(Reagent_Pos pos);
	~CReagentAreaListItem(void);

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
		// 本列表无需合并单元格
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
	//     设置本条目选中状态
	// ---------------------------------------------------------------------------
	virtual void SetHighlightState(bool state);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得本条目选中状态
	// ---------------------------------------------------------------------------
	virtual bool IsHighlight();

private:
	Reagent_Pos m_ReagentPos;
};
