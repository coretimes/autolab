//*****************************************************************************
// �ļ��� : SampleAreaData.h
// 
// ����   : ����������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 1.���ɣ�
//	 ��������120���ڵ�����ɣ����нڵ���10��12��
//   �ڵ�����ֵ������m_ArrSamplePos[12][10]������,��Ӧ������(col-1,row-1)
//
// 2.ÿ���ڵ������ֱ�ŷ�ʽ: 
//   a.��ά����(Coord)��(col,row)��ʽ,ȡֵΪ(1,1)��(12,10)��
//     ������0�к�11�зֱ��ʾ�����кͱ�����;
//
//   b.һά����(Num)��ȡֵΪ1~120,�����϶��£��������˳���γɣ�Ϊ����ͨѶ�ͱ���
//
// 3.���ýڵ�ġ�������Ŀ���顱���ԣ�
//   ����һ����������Ҫ���Զ����Ŀ������������������Ա�������Ҫʹ�õ��Լ����Լ�����λ�ñ��
// 
//*****************************************************************************

#pragma once
#include "SampleTest.h"
#include "TestUnit.h"
#include "Types.h"

#define NUM_ROW				10				// ����������
#define NUM_COL				12				// ����������
#define NUM_SAMPLEPOS		120				// ����λ����					

// ���������ݸ�����Ϣ����
// wparam: λ������
// lparam: NULL.
#define UM_SampleArea_StateChange		WM_USER + 1001

// ��������ɾ�仯��Ϣ����
// wparam: NULL
// lparam: NULL.
#define UM_SampleArea_DataUpdate			WM_USER + 1002

//*********************************
// �������ýڵ�
//*********************************
struct SampleConfigNode
{
	struct TestProjInfo							// �����Ŀ��Ϣ�ṹ
	{
		Reagent_Pos ReagentPos;					// ������Ŀʹ���Լ������Լ�����λ�ñ��
		int Repetitions;						// �ظ�������
		double ThoeryConc;						// ����Ũ��
		TestUnit_ID TestUintKeys;				// ���������еĲ��Ե�Ԫkey���������δ��ʼ�����keyֵΪ0������Ϊ��0	
		TestProjInfo()
		{
			ReagentPos = 0;
			Repetitions = 0;
			ThoeryConc = -1.0;
			TestUintKeys = 0;
		}
	};
	Sample_Pos PosNum;							// λ�ñ��
	bool IsThereTube;							// �Ƿ����Թ�
	bool IsSelected;							// �Ƿ���ѡ��״̬
	bool IsSetingSample;						// �Ƿ��Ѿ�����������Ϣ
	bool IsStandard;							// �Ƿ��Ǳ�׼Ʒ
	Sample_Key RecordID;						// ����׼Ʒ�����������еļ�¼ID
	int StdPointIndex;							// ��׼������������Ǳ�׼Ʒ��ֵΪ��׼���������������������Ϊ-1
	map<Reagent_Pos,TestProjInfo> TestProjArr;	// ������Ŀ����,map<�Լ���λ�ñ��,�����Ŀ��Ϣ>
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
	// ժҪ��
	//	   ɾ��ʵ��
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
	// ժҪ��
	//	   ������������״̬
	// ������
	//     1.col - �б��
	//     2.IsThereTube - ture���������Թܣ�false���������Թ�
	// --------------------------------------------------
	void SetColumeState(const int &col,const bool &IsThereTube);
	
	// --------------------------------------------------
	// ժҪ��
	//	   ���ݶ�ά����ȡ�ýڵ�ֵ
	// ������
	//     1.col - �б��
	//     2.row - �б��
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
	// ժҪ��
	//	   ����һά����ȡ�ýڵ�ֵ
	// ������
	//     1.pos - �б��
	// --------------------------------------------------
	SampleConfigNode* GetNode(const Sample_Pos & pos)
	{
		int c,r;
		GetCoordByNum(pos,c,r);
		return GetNode(c,r);
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ����ӻ�������ýڵ�󣬵��ô˺������������Ի�����
	// --------------------------------------------------
	void UpdateTestBuffer();

	// --------------------------------------------------
	// ժҪ��
	//	   ��ѯ�����
	// --------------------------------------------------
	void UpdateSearchList();

	// --------------------------------------------------
	// ժҪ��
	//	   ȡ�ñ�ѡ�еĽڵ�
	// ������
	//     1.SampleConfigNodeList - ����ȡ�õı�ѡ�нڵ��
	// --------------------------------------------------
	void GetSelectedNodes(map<Sample_Pos,SampleConfigNode *> & SampleConfigNodeList);

	// --------------------------------------------------
	// ժҪ��
	//	   ȡ�����нڵ��ѡ��״̬
	// --------------------------------------------------
	void CancelAllSelected();
	// --------------------------------------------------
	// ժҪ��
	//	   ����������ת����������2ά����ת��Ϊ1ά����
	// ������
	//     1.col - ��ά������б��
	//     2.row - ��ά������б��
	//     3.num - һάλ�ñ��
	// --------------------------------------------------

	Sample_Pos GetNumByCoord(const int &col, const int &row) const
	{
		// ��ά����ת��Ϊһά���룬�磺
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
	// ժҪ��
	//	   ����ת����������1ά����ת��Ϊ2ά����
	// ������
	//     1.num - һάλ�ñ��
	//     2.col - ��ά������б��
	//     3.row - ��ά������б��
	// --------------------------------------------------
	void GetCoordByNum(const Sample_Pos &num, int &col, int &row) const
	{
		// һά����ת��Ϊ��ά���룬�磺
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
	// ժҪ��
	//	   ȡ�ö�ά�����ַ���ŷ�ʽ
	// ������
	//     1.num - һάλ�ñ��
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
	// ժҪ��
	//	   �����Ի���
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
		SpreadDataUpdateMessage();
	};


	// --------------------------------------------------
	// ժҪ��
	//	   ���ý��������ʾ
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
	// ժҪ��
	//	   ȡ���ȵ�λ��
	// --------------------------------------------------
	Sample_Pos GetHoverPos() const
	{
		return m_HoverPos;
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ի���ɢ����״̬������Ϣ��
	// ������
	//     1.pos.���ݷ������µ�����λ�ã����Ϊ0���������������
	// --------------------------------------------------
	void SpreadStateChangeMessage(const Sample_Pos &pos = 0);

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ի���ɢ�����ݸ��¸��µ���Ϣ
	// --------------------------------------------------
	void SpreadDataUpdateMessage();

	// --------------------------------------------------
	// ժҪ��
	//	   ����ָ�����Լ�λ����Ϣ���ز��Ա��Լ�������λ�б�
	// ������
	//     1.ReagentPos.ָ�����Լ�λ��
	//     2.ListOfSamplePos.���ص�����λ�б�
	// ���أ�
	//     ����ReagentPos�Լ�������λ�ø���
	// --------------------------------------------------
	int GetSampleByReagent(const Reagent_Pos ReagentPos,vector<Sample_Pos> & ListOfSamplePos);
private:
	// --------------------------------------------------
	// ժҪ��
	//	   �������ýڵ�ת��Ϊ���Խڵ�
	//     ����һ���������ܻ���Զ����Ŀ������һ�����ýڵ���ܻ�������ɸ����Խڵ�
	// ������
	//     ConfigNode - �������ýڵ�
	//     vctTestNodes - ���ɵ��������Խڵ�����
	// --------------------------------------------------
	void _ConfigNodeToTestNodes(SampleConfigNode & ConfigNode);

private:
	SampleConfigNode m_ArrSamplePos[NUM_COL][NUM_ROW];			// �������ýڵ�����,����ȡֵΪ�����һ
	multimap<Reagent_Pos,Sample_Pos> m_SearchListInReagentPos;	// �˱�������ѯָ��������Ŀ��������Ϣ															
	bool bIsStaticMemsInit;										// ��̬�����Ƿ��Ѿ���ʼ��
	Sample_Pos m_HoverPos;										// �ȵ�����λ
private:
	static CSampleAreaData * m_pInstance;
	vector<CDialog *> m_AssociateDlgList ;						// �����Ի���ָ��
};
