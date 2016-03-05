//*****************************************************************************
// 文件名 : MorniterListItem.h
// 
// 类名   : 测试监控列表控件数据项，本列表位于【测试进度监控】对话框上
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
#include "MeteringManagement.h"
#include "TestUnit.h"

namespace MorniterListItem
{
	enum ListColumn
	{
		colType = 0,				// 样品类型
		colProject,					// 检测项目
		colSampleNo,				// 样品编号
		colConcPoint,				// 浓度点
		colSamplePos,				// 样本位
		colMeteringIndex,			// 测光节点索引
		colFinishTime,				// 完成时间
		colMeteringState,			// 测光状态
		colUseState,				// 使用状态
		colRlu,						// 发光值
		colAverageRlu,				// 平均发光值
		colCV,						// 变异系数
		colTheoryConc,				// 理论浓度
		colCalConc,					// 计算浓度
		colCurve,					// 标准曲线
		ColumnCount
	};
}
using namespace MorniterListItem;

class CMorniterListItem :public CListItemData
{
public:
	CMorniterListItem(Metering_ID ID);
	~CMorniterListItem(void);

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
	//     取得测光节点
	// ---------------------------------------------------------------------------
	const MeteringNode & GetMeteringNode() const
	{	return m_MeteringNode;}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得测试单元
	// ---------------------------------------------------------------------------
	const CTestUnit * GetTestUnit() const
	{	return m_pTestUnit;}

private:
	MeteringNode m_MeteringNode;				// 测管节点信息
	SampleTestNode m_SampleTestNode;			// 样本节点信息
	CTestUnit * m_pTestUnit;					// 测试单元信息
};
