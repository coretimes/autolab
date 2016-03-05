//*****************************************************************************
// 文件名 : StandardConfigListItem.h
// 
// 类名   : 标准品测试配置列表项数据，本列表位于【标准品配置】弹出对话框上
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-10       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "ListItemData.h"

namespace StandardConfigList
{
	enum ListColumn
	{
		colPos = 0,					// 样本位
		colProject,					// 检测项目
		colBatchNum,				// 标准品批号
		colConcIndex,				// 浓度点索引
		colRepeat,					// 重复测试次数
		ColumnCount
	};
}

using namespace StandardConfigList;

class CStandardConfigListItem : public CListItemData
{
public:
	CStandardConfigListItem();
	~CStandardConfigListItem(void);
	// ---------------------------------------------------------------------------
	// 摘要：
	//     更新数据
	// 参数：
	//     1.pos - 样本位
	//     2.nStandardRecordID - 标准品记录ID
	//     3.BatchNum - 批号
	//     4.ConcIndex - 浓度点索引
	//     5.repeat - 重复次数
	// ---------------------------------------------------------------------------
	void SetPosNum(const uint16 &PosNum = 0);

	uint16 GetPosNum()
	{
		return m_PosNum;
	}

	void SetBatchNum(const CString &BatchNum)
	{
		m_strBatchNum = BatchNum;
	}

	CString GetBatchNum() const
	{
		return m_strBatchNum;
	}

	void SetProject(const CString &Project)
	{
		m_strProject = Project;
	}

	void SetConcIndex(const int &ConcIndex)
	{
		m_ConcIndex = ConcIndex;
	}

	int GetConcIndex() const
	{
		return m_ConcIndex;
	}

	void SetRepeat(const int &repeat)
	{
		m_Repeat = repeat;
	}

	int GetRepeat() const
	{
		return m_Repeat;
	}

	void SetOptionsConcIndex(const vector<CString> & arrOptions)
	{
		m_OptionsOfConcIndex.clear();
		for(int i = 0; i < (int)arrOptions.size(); i++)
		{
			m_OptionsOfConcIndex.push_back(arrOptions[i]);
		}
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     判断本项某列单元格是否与另外某项数据同列单元格等价，
	//     当等价时，绘制表格时需要合并为一个单元格
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.pItemData - 上列表项数据指针
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
	{
		// 本表中没有合并单元格，所以全部返回false
		return false;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得某单元格内容
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
	virtual ColumnProperties GetColumnProperties(const int &ColumnIndex);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得子项目个数
	// ---------------------------------------------------------------------------
	virtual int GetSubItemCount()
	{
		return (int)StandardConfigList::ColumnCount;
	}

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得组合框属性,如果某列的属性是Optional，则调用此函数获得组合框选项
	// 参数：
	//     1.ColumnIndex - 列索引，如果此列不是Optional，则返回false
	//     2.strOptions - 下拉后显示选项内容
	//     3.nDefaultSelectedIndex - 默认选中索引
	// ---------------------------------------------------------------------------
	virtual bool GetComboxProperties(const int &ColumnIndex,
									 vector<CString>&strOptions,
									 int &nDefaultSelectedIndex,
									 bool &bIsEditable);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     取得微调按钮的属性
	// 参数：
	//     1.ColumnIndex - 列索引，如果此列不是Spin，则返回false
	//     2.fInitial - 初始值
	//     3.lower - 取值范围下限
	//     4.upper - 取值范围上限
	//     5.increament - 按钮单击增量
	virtual bool GetSpinBtnProperties(const int &ColumnIndex,
									  double &initial,
									  double &lower, 
									  double &upper,
									  double &increament);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     组合框控件选项更新响应函数
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.newSelectedIndex - 新选项索引
	// ---------------------------------------------------------------------------
	virtual void OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     编辑控件内容更新响应函数
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.contex - 文本内容
	// ---------------------------------------------------------------------------
	virtual void OnEnChange(const int &ColumnIndex,const CString & newContex);

private:
	uint16 m_PosNum;						// 样本位
	CString m_strProject;					// 检测项目
	CString m_strBatchNum;					// 批号
	int m_ConcIndex;						// 浓度点索引
	int m_Repeat;							// 重复次数
	vector<CString> m_OptionsOfConcIndex;	// 浓度点索引选项
};
