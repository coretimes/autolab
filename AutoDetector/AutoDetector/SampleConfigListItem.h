//*****************************************************************************
// 文件名 : SampleConfigListItem.h
// 
// 类名   : 样本测试配置列表项数据,本列表位于【样本配置】弹出对话框上
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
#include "listitemdata.h"
#include "SampleTable.h"

namespace SampleConfigList
{
	enum ListColumn
	{
		colPos = 0,					// 样本位
		colSampleNo,				// 样本编号
		colType,					// 样本类型
		colDilution,				// 稀释倍数
		colDept,					// 送检单位名称
		colApplicant,				// 送检人
		colApplyDate,				// 送检日期
		colProjecName,				// 检测项目名称
		colRepeat,					// 重复测试次数
		colTheoryConc,				// 理论浓度
		ColumnCount
	};
}

using namespace SampleConfigList;

class CSampleConfigListItem :	public CListItemData
{
public:
	CSampleConfigListItem(void);
	~CSampleConfigListItem(void);
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
	void SetPosNum(const Sample_Pos &PosNum);

	Sample_Pos GetPosNum() const
	{
		return m_PosNum;
	}

	void SetSampleNo(const CString &strSampleNo)
	{
		m_SampleInfo.SampleNo = strSampleNo;
	}

	void SetSampleType(const CString &strSampleType)
	{
		m_SampleInfo.Type = strSampleType;
	}
	void SetSampleDilution(const double &dDilution)
	{
		m_SampleInfo.Dilution = dDilution;
	}

	void SetSampleDept(const CString &strSampleDept)
	{
		m_SampleInfo.Dept = strSampleDept;
	}

	void SetSampleApplicant(const CString &strApplicant)
	{
		m_SampleInfo.Applicant = strApplicant;
	}

	void SetRepeat(const int &repeat)
	{
		m_Repeat = repeat;
	}

	int GetRepeat() const
	{
		return m_Repeat;
	}

	void SetApplyDate(COleDateTime date)
	{
		m_SampleInfo.ApplyDate = date;
	}

	void SetTypeOptions(const vector<CString> & options)
	{
		for(int i=0; i<(int)options.size(); i++)
		{
			m_OptionsOfSampleTypes.push_back(options[i]);
		}
	}

	void SetProjectOptions(const vector<CString> & options)
	{
		for(int i=0; i< (int)options.size(); i++)
		{
			m_OptionsOfTestProjects.push_back(options[i]);
		}
	}

	void SetProjectIndex(const int & indexOfProject)
	{
		m_idxTestProject = indexOfProject;
	}

	int GetProjectIndex() const
	{
		return m_idxTestProject;
	}
	
	void SetThoeryConc(const double & conc)
	{
		m_dTheoryConc = conc;
	}

	double GetThoeryConc() const
	{
		return m_dTheoryConc;
	}

	void GetSampleInfo(SampleRecord & record) const
	{
		record = m_SampleInfo;
	}

	void SetSampleInfo(const SampleRecord & record)
	{
		m_SampleInfo = record;
	}
	// ---------------------------------------------------------------------------
	// 摘要：
	//     判断本项某列单元格是否与另外某项数据同列单元格等价，
	//     当等价时，绘制表格时需要合并为一个单元格
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.pItemData - 上列表项数据指针
	// ---------------------------------------------------------------------------
	virtual bool IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData);

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
	//     编辑控件内容更新响应函数
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.contex - 文本内容
	// ---------------------------------------------------------------------------
	virtual void OnEnChange(const int &ColumnIndex,const CString & newContex);

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
		return (int)ColumnCount;
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
	//     组合框控件选项更新响应函数
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.newSelectedIndex - 新选项索引
	// ---------------------------------------------------------------------------
	virtual void OnCbnSelchange(const int &ColumnIndex,const int & newSelectedIndex);

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
	//     取得时间控件的属性
	//     1.ColumnIndex - 列索引，如果此列不是DateCtrl，则返回false
	//     2.fInitial - 初始值
	// ---------------------------------------------------------------------------
	virtual bool GetDateCtrlProperties(const int &ColumnIndex,COleDateTime &initial);

	// ---------------------------------------------------------------------------
	// 摘要：
	//     组合框控件选项更新响应函数
	// 参数：
	//     1.ColumnIndex - 列索引
	//     2.newDate - 新选项索引
	// ---------------------------------------------------------------------------
	virtual void OnDtnchange(const int &ColumnIndex,const COleDateTime & newDate);

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

private:
	Sample_Pos m_PosNum;					 // 样本区位置编号
	SampleRecord m_SampleInfo;				 // 样本记录
	vector<CString> m_OptionsOfTestProjects; // 项目名称选项
	int m_idxTestProject;					 // 检测项目在m_OptionsOfTestProjects中的索引
	int m_Repeat;							 // 重复次数
	double m_dTheoryConc;					 // 理论浓度
	vector<CString> m_OptionsOfSampleTypes;  // 样本类型选项

private:
	static ListColumn SortColumn;			// 排序列名
	static bool IsAsc;						// 是否升序排，true是，false为降序
};
