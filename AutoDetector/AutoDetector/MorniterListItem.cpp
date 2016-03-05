#include "StdAfx.h"
#include "MorniterListItem.h"
#include "TestManagement.h"
#include "ReagentTable.h"
#include "SampleAreaData.h"
#include "SampleTable.h"
#include "StandardTable.h"

CMorniterListItem::CMorniterListItem(Metering_ID ID)
{
	// 根据测光ID，取得测光节点数据
	CMeteringManagement * pList = CMeteringManagement::GetInstance();
	pList->GetNode(ID,m_MeteringNode);

	// 取得测管节点的测试单元信息
	m_pTestUnit = CTestManagement::GetInstance()->GetTestUnit(m_MeteringNode.RealatedInfoKey.ParentUnitID);

	// 根据测光节点内的测试单元ID，取得样本测试节点数据
	m_pTestUnit->GetSampleNodeInfo(m_MeteringNode.RealatedInfoKey.ConcIndex,m_SampleTestNode);
}

CMorniterListItem::~CMorniterListItem(void)
{
}

bool CMorniterListItem::IsFieldEquivalent(const int &ColumnIndex,const CListItemData * pItemData)
{
	if(pItemData == NULL)
		return false;
	const CMorniterListItem * pData = static_cast<const CMorniterListItem*>(pItemData);

	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);
	
	// 取得测光节点数据
	const MeteringNode & thatNode = pData->GetMeteringNode();

	// 取得测管节点的测试单元信息
	const CTestUnit * pThatUnit = pData->GetTestUnit();

	switch(colIndex)
	{
	case colType:					// 样品类型
	case colProject:				// 检测项目
	case colSampleNo:				// 样品编号
	case colCurve:					// 标准曲线
		{
			// 如果属于同一测试单元，则合并
			if (m_pTestUnit->GetID() == pThatUnit->GetID())
				return true;
			break;
		}

	case colConcPoint:				// 浓度点
	case colSamplePos:				// 样本位
	case colAverageRlu:				// 平均发光值
	case colCV:						// 变异系数
	case colTheoryConc:				// 理论浓度
	case colCalConc:				// 计算浓度
		{
			// 如果不属于同一测试单元，则不合并
			if (m_pTestUnit->GetID() != pThatUnit->GetID())
				return false;

			// 如果属于同一测试单元且属于同一浓度点，合并
			if (m_MeteringNode.RealatedInfoKey.ConcIndex == thatNode.RealatedInfoKey.ConcIndex)
				return true;
			break;
		}

	case colMeteringIndex:			// 测光节点索引
	case colFinishTime:				// 完成时间
	case colMeteringState:			// 测光状态
	case colUseState:				// 使用状态
	case colRlu:					// 发光值
		return false;

	default:
		break;
	}
	return false;
}

UINT CMorniterListItem::GetCellContex(const int &ColumnIndex,CString & contex)
{
	ListColumn colIndex = static_cast<ListColumn> (ColumnIndex);

	switch(colIndex)
	{
	case colType:					// 样品类型
		{
			if (m_SampleTestNode.IsStandard)
				contex = _T("标准品");
			else
				contex = _T("样本");
			break;
		}
	case colProject:				// 检测项目
		{
			CReagentTable *pReagentTable = CReagentTable::GetInstance();
			ReagentRecord reagentRecord = pReagentTable->GetRecordInfo(m_SampleTestNode.ReagentID);
			reagentRecord.ProjectName;
			contex.Format(_T("%d. %s(%s)"),m_SampleTestNode.ReagentPos, reagentRecord.ShortName,reagentRecord.ProjectName);
			return DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT;
		}
	case colSampleNo:				// 样品编号
		{

			if (m_SampleTestNode.IsStandard)
			{
				StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
				contex.Format(_T("%s"),record.BatchNum);
			}
			else
			{
				SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
				contex.Format(_T("%s"),record.SampleNo);
			}

			break;
		}

	case colConcPoint:				// 浓度点
		{
			if (m_SampleTestNode.IsStandard)
				contex.Format(_T("%c"),m_MeteringNode.RealatedInfoKey.ConcIndex + 65);
			else
				contex = _T("-");

			break;
		}

	case colSamplePos:				// 样本位
		{
			contex = CSampleAreaData::GetInstance()->GetCharCoordByNum(m_SampleTestNode.SamplePos);
			break;
		}

	case colMeteringIndex:			// 测光节点索引
		{
			contex.Format(_T("%d"),m_MeteringNode.MeteringIndex);
			break;
		}
	case colFinishTime:				// 完成时间
		{
			// 取得当前计时周期
			CMeteringManagement * pMeteringManagement = CMeteringManagement::GetInstance();
			Tick_ID curtick = pMeteringManagement->GetCurrentTick();
	
			// 剩余时间 = （结束周期 - 当前周期） * 周期时间间隔（秒）
 			CTimeSpan lefttime((m_MeteringNode.end -curtick) * pMeteringManagement->GetTickInterval() / 1000);

			// 取得当前时间
			CTime cm = CTime::GetCurrentTime();

			// 计算结束时间
			CTime finishTime = cm + lefttime;
			contex = finishTime.Format(_T("%H:%M"));
			break;
		}

	case colMeteringState:			// 测光状态
		{
			switch(m_MeteringNode.state)
			{
			case MeteringNode::Unsent:				// 未发送
				contex = _T("未发送");
				break;
			case MeteringNode::Sending:				// 数据发送中
				contex = _T("数据发送中");
				break;
			case MeteringNode::WaitePipetting:		// 等待加样
				contex = _T("等待加样");
				break;
			case MeteringNode::Pipetting:			// 正在加样
				contex = _T("正在加样");
				break;
			case MeteringNode::AfterPipetting:		// 加样完毕
				contex = _T("加样完毕");
				break;
			case MeteringNode::Incubating:			// 孵育中
				contex = _T("孵育中");
				break;
			case MeteringNode::WaiteCleaning:		// 等待清洗
				contex = _T("等待清洗");
				break;
			case MeteringNode::Cleaning:			// 正在清洗
				contex = _T("正在清洗");
				break;
			case MeteringNode::WaiteMetering:		// 等待测光
				contex = _T("等待测光");
				break;
			case MeteringNode::Metering:			// 正在测光
				contex = _T("正在测光");
				break;
			case MeteringNode::Over:				// 测光完毕
				contex = _T("测光完毕");
				break;
			}
			break;
		}
	case colUseState:								// 使用状态
		contex = _T("使用");
		break;
	case colRlu:									// 发光值
		break;
	case colAverageRlu:								// 平均发光值
		break;
	case colCV:										// 变异系数
		break;
	case colTheoryConc:								// 理论浓度
		if (m_SampleTestNode.IsStandard)
		{
			StandardRecord record = CStandardTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
			contex.Format(_T("%g%s"),record.StdConc[m_SampleTestNode.StdPointIndex],record.Unit);
		}
		else
		{
			//SampleRecord record = CSampleTable::GetInstance()->GetRecordInfo(m_SampleTestNode.SampleID);
			//contex.Format(_T("%s"),record.);
		}
		break;
	case colCalConc:								// 计算浓度
		break;
	case colCurve:									// 标准曲线
		break;
	default:
		break;
	}

	return 0;
}
