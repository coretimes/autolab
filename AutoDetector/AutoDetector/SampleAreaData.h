//*****************************************************************************
// 文件名 : SampleAreaData.h
// 
// 类名   : 样本区数据
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 1.构成：
//	 样本区由120个节点区组成，其中节点区10行12列
//   节点属性值保存于m_ArrSamplePos[12][10]数组中,对应索引是(col-1,row-1)
//
// 2.每个节点有两种编号方式: 
//   a.二维坐标(Coord)：(col,row)形式,取值为(1,1)到(12,10)；
//     但是用0列和11行分别表示标题列和标题行;
//
//   b.一维编码(Num)：取值为1~120,按自上而下，自左而右顺序形成，为方便通讯和保存
//
// 3.配置节点的“待测项目数组”属性：
//   由于一个样本可能要测试多个项目，此数组用来保存测试本样本所要使用的试剂在试剂区的位置编号
// 
//*****************************************************************************

#pragma once
#include "SampleTest.h"
#include "TestUnit.h"
#include "Types.h"

#define NUM_ROW				10				// 样本区行数
#define NUM_COL				12				// 样本区列数
#define NUM_SAMPLEPOS		120				// 样本位数量					

// 样本区数据更新消息定义
// wparam: 位置坐标
// lparam: NULL.
#define UM_SampleArea_StateChange		WM_USER + 1001

// 样本区增删变化消息定义
// wparam: NULL
// lparam: NULL.
#define UM_SampleArea_DataUpdate			WM_USER + 1002

//*********************************
// 样本配置节点
//*********************************
struct SampleConfigNode
{
	struct TestProjInfo							// 检测项目信息结构
	{
		Reagent_Pos ReagentPos;					// 测试项目使用试剂所在试剂区的位置编号
		int Repetitions;						// 重复测光次数
		double ThoeryConc;						// 理论浓度
		TestUnit_ID TestUintKeys;				// 关联进行中的测试单元key，如果测试未开始，则此key值为0，否则为非0	
		TestProjInfo()
		{
			ReagentPos = 0;
			Repetitions = 0;
			ThoeryConc = -1.0;
			TestUintKeys = 0;
		}
	};
	Sample_Pos PosNum;							// 位置编号
	bool IsThereTube;							// 是否有试管
	bool IsSelected;							// 是否处于选中状态
	bool IsSetingSample;						// 是否已经设置样本信息
	bool IsStandard;							// 是否是标准品
	Sample_Key RecordID;						// “标准品表”或“样本表”中的记录ID
	int StdPointIndex;							// 标准点索引：如果是标准品，值为标准点索引；如果是样本，则为-1
	map<Reagent_Pos,TestProjInfo> TestProjArr;	// 待测项目数组,map<试剂区位置编号,检测项目信息>
	SampleConfigNode()
	{
		PosNum = 0;
		IsThereTube = false;
		IsSelected = false;
		IsSetingSample = false;
		IsStandard = false;
		RecordID = 0;
		StdPointIndex = -1;
	}
};

class CSampleAreaData
{
private:
	CSampleAreaData(void);
public:
	static CSampleAreaData * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSampleAreaData();
		return m_pInstance;
	}

	// --------------------------------------------------
	// 摘要：
	//	   删除实体
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CSampleAreaData(void){};

public:
	// --------------------------------------------------
	// 摘要：
	//	   设置样本区列状态
	// 参数：
	//     1.col - 列编号
	//     2.IsThereTube - ture：本列有试管，false：本列无试管
	// --------------------------------------------------
	void SetColumeState(const int &col,const bool &IsThereTube);
	
	// --------------------------------------------------
	// 摘要：
	//	   根据二维坐标取得节点值
	// 参数：
	//     1.col - 列编号
	//     2.row - 行编号
	// --------------------------------------------------
	SampleConfigNode* GetNode(const int &col,const int &row)
	{
		if(col < 1 || col > NUM_COL ||row < 1 || row > NUM_ROW)
		{
			ASSERT(0);
			return NULL;
		}
		return &m_ArrSamplePos[col - 1][row - 1];
	}

	// --------------------------------------------------
	// 摘要：
	//	   根据一维坐标取得节点值
	// 参数：
	//     1.pos - 列编号
	// --------------------------------------------------
	SampleConfigNode* GetNode(const Sample_Pos & pos)
	{
		int c,r;
		GetCoordByNum(pos,c,r);
		return GetNode(c,r);
	}

	// --------------------------------------------------
	// 摘要：
	//	   当添加或更改配置节点后，调用此函数更新至测试缓冲区
	// --------------------------------------------------
	void UpdateTestBuffer();

	// --------------------------------------------------
	// 摘要：
	//	   查询表更新
	// --------------------------------------------------
	void UpdateSearchList();

	// --------------------------------------------------
	// 摘要：
	//	   取得被选中的节点
	// 参数：
	//     1.SampleConfigNodeList - 保存取得的被选中节点表
	// --------------------------------------------------
	void GetSelectedNodes(map<Sample_Pos,SampleConfigNode *> & SampleConfigNodeList);

	// --------------------------------------------------
	// 摘要：
	//	   取消所有节点的选择状态
	// --------------------------------------------------
	void CancelAllSelected();
	// --------------------------------------------------
	// 摘要：
	//	   样本区坐标转换函数，将2维坐标转化为1维坐标
	// 参数：
	//     1.col - 二维坐标的列编号
	//     2.row - 二维坐标的行编号
	//     3.num - 一维位置编号
	// --------------------------------------------------

	Sample_Pos GetNumByCoord(const int &col, const int &row) const
	{
		// 二维编码转化为一维编码，如：
		// (1,1)  -> 1
		// (1,10) -> 10
		// (2,1)  -> 11
		// (2,9) -> 19
		// (12,10) -> 120
		if(col <1 || col > NUM_COL || row < 1 || row > NUM_ROW)
			return 0;
		return (Sample_Pos)((col-1)*10 + row);
	}

	// --------------------------------------------------
	// 摘要：
	//	   坐标转换函数，将1维坐标转化为2维坐标
	// 参数：
	//     1.num - 一维位置编号
	//     2.col - 二维坐标的列编号
	//     3.row - 二维坐标的行编号
	// --------------------------------------------------
	void GetCoordByNum(const Sample_Pos &num, int &col, int &row) const
	{
		// 一维编码转化为二维编码，如：
		// 1  -> (1,1)
		// 10 -> (1,10)
		// 11 -> (2,1)  
		// 20 -> (2,10)

		if(num < 1 || num >NUM_SAMPLEPOS)
		{
			col = -1;
			row = -1;
			return ;
		}

		int src = (int)num;
		row = src % 10;
		if(row == 0)
			row = 10;
		col = ( src - row ) / 10 + 1;
	}

	// --------------------------------------------------
	// 摘要：
	//	   取得二维坐标字符编号方式
	// 参数：
	//     1.num - 一维位置编号
	// --------------------------------------------------
	CString GetCharCoordByNum(const Sample_Pos &num) const
	{
		// 1  -> "A1"
		// 11 -> "B1"  
		// 22 -> "C2"
		int col,row;
		GetCoordByNum(num,col,row);
		CString strCoord;
		strCoord.Format(_T("%c%d"),col + 64,row);
		return strCoord;
	}

	// --------------------------------------------------
	// 摘要：
	//	   关联对话框
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
		SpreadDataUpdateMessage();
	};


	// --------------------------------------------------
	// 摘要：
	//	   设置焦点高亮显示
	// --------------------------------------------------
	void SetHover(const Sample_Pos &pos)
	{
		Sample_Pos oldhover = m_HoverPos;
		m_HoverPos = pos;
		if(m_HoverPos > 0)	
			SpreadStateChangeMessage(m_HoverPos);
		if(oldhover > 0)	
			SpreadStateChangeMessage(oldhover);
	};
	
	// --------------------------------------------------
	// 摘要：
	//	   取得热点位置
	// --------------------------------------------------
	Sample_Pos GetHoverPos() const
	{
		return m_HoverPos;
	}

	// --------------------------------------------------
	// 摘要：
	//	   向关联对话框散布“状态更新消息”
	// 参数：
	//     1.pos.数据发生更新的样本位置，如果为0，则更新所有数据
	// --------------------------------------------------
	void SpreadStateChangeMessage(const Sample_Pos &pos = 0);

	// --------------------------------------------------
	// 摘要：
	//	   向关联对话框散布数据更新更新的消息
	// --------------------------------------------------
	void SpreadDataUpdateMessage();

	// --------------------------------------------------
	// 摘要：
	//	   根据指定的试剂位置信息返回测试本试剂的样本位列表
	// 参数：
	//     1.ReagentPos.指定的试剂位置
	//     2.ListOfSamplePos.返回的样本位列表
	// 返回：
	//     测试ReagentPos试剂的样本位置个数
	// --------------------------------------------------
	int GetSampleByReagent(const Reagent_Pos ReagentPos,vector<Sample_Pos> & ListOfSamplePos);
private:
	// --------------------------------------------------
	// 摘要：
	//	   样本配置节点转化为测试节点
	//     由于一个样本可能会测试多个项目，所以一个配置节点可能会产生若干个测试节点
	// 参数：
	//     ConfigNode - 样本配置节点
	//     vctTestNodes - 生成的样本测试节点数组
	// --------------------------------------------------
	void _ConfigNodeToTestNodes(SampleConfigNode & ConfigNode);

private:
	SampleConfigNode m_ArrSamplePos[NUM_COL][NUM_ROW];			// 样本配置节点数组,索引取值为坐标减一
	multimap<Reagent_Pos,Sample_Pos> m_SearchListInReagentPos;	// 此表用来查询指定测试项目的样本信息															
	bool bIsStaticMemsInit;										// 静态数据是否已经初始化
	Sample_Pos m_HoverPos;										// 热点样本位
private:
	static CSampleAreaData * m_pInstance;
	vector<CDialog *> m_AssociateDlgList ;						// 关联对话框指针
};
