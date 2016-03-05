//*****************************************************************************
// 文件名 : ReagentAreaData.h
// 
// 类名   : 试剂区数据
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12       崔志雷       新建
// ------------------------------------------
// 备注：
// ------------------------------------------
// 1.构成：
//	 试剂区示意图控件由标题区、节点区（15个节点）及其他区域组成，每个节点的属性是一个uint32类型试剂ID；
//   节点属性值保存于m_arrReagents[15]中，对应索引是(pos - 1)
//*****************************************************************************

#pragma once
#include "ReagentTable.h"

// 试剂区数据更新消息定义
// wparam: 位置坐标
// lparam: NULL.
#define UM_ReagentArea_Update		WM_USER + 1000
#define  NUM_REAGENTPOS				15					// 试剂区容纳试剂个数

struct ReagentConfigNode
{
	Reagent_Pos pos;									// 试剂区位置编号
	bool IsThereTube;									// 是否有试管
	bool IsSetingReagent;								// 是否已经设置试剂信息
	Reagent_Key ReagentID;								// 试剂ID
	bool IsHover;										// 当前节点是否处于焦点状态

	ReagentConfigNode()
	{
		pos = 0;
		IsThereTube = false;
		IsThereTube = false;
		IsSetingReagent = false;
		ReagentID = 0;
		IsHover = false;
	}
};
#pragma once

class CReagentAreaData
{
private:
	CReagentAreaData(void);

public:
	static CReagentAreaData * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CReagentAreaData();
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

	~CReagentAreaData(void);


	// --------------------------------------------------
	// 摘要：
	//	   取得试剂区配置信息列表
	// 参数：
	//     1.ReagentConfigList - 用来存储输出的试剂配置节点数组
	// --------------------------------------------------
	void GetReagentConfigList(map<Reagent_Pos,const ReagentConfigNode *> & mapReagents) const;

	// --------------------------------------------------
	// 摘要：
	//	   根据试剂位置配置节点
	// 参数：
	//     1.pos - 试剂位置
	// 返回：
	//     2.试剂配置节点指针
	// --------------------------------------------------
	ReagentConfigNode * GetNode(const Reagent_Pos & pos)
	{	
		if(pos < 1 || pos > NUM_REAGENTPOS)
			return NULL;
		else
			return &m_ArrReagentPos[pos - 1];
	}

	// --------------------------------------------------
	// 摘要：
	//	   添加新视图
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
	};

	// --------------------------------------------------
	// 摘要：
	//	   向关联对话框散布数据更新的消息
	// 返回：
	//     1.pos.数据发生更新的试剂位置，如果为0，则更新所有数据
	// --------------------------------------------------
	void SpreadUpdateMessage(Reagent_Pos pos = 0);

private:
	static CReagentAreaData * m_pInstance;

private:
	ReagentConfigNode m_ArrReagentPos[NUM_REAGENTPOS];
	vector<CDialog *> m_AssociateDlgList ;							// 关联对话框指针
};
