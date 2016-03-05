//*****************************************************************************
// �ļ��� : BaseDialog.h
// 
// ����   : �Զ���ĶԻ��������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
//   ������Ի�����Ҫ����
//	 1.��С����ʱ���ӿؼ����ض�λ
//   2.Ƥ�����ܣ��ǿͻ������С�����Լ����ƽӿ�ʵ��
//*****************************************************************************

#pragma once
#include <vector>
#include "SkinObject.h"
using namespace std;

// ������仯����������
enum CoordTransMode
{
	tmNotChange = 0,				// ����
	tmQuantity,						// ��������
	tmProportion					// ����������
};

// �ؼ��ض�λ����
struct ControlsResizeProp
{
	UINT ID;						// �ؼ�ID
	CPoint ptOrigPosBps;			// ��ʼ��λ����
	CRect rcInitPosRect;			// �ؼ���ʼλ�þ��Σ�������Դģ��ȡ��
	CoordTransMode ctmXofPosBps;	// ��λ����xֵ�仯����
	CoordTransMode ctmYofPosBps;	// ��λ����yֵ�仯����
	CoordTransMode ctmWigth;		// ��ȱ仯����
	CoordTransMode ctmHight;		// ���ȱ仯����
	bool IsPaintBackGround;			// �Ƿ��Ի汳�������Ϊfalse�����ɸ�������䱳����Ĭ��Ϊtrue
	ControlsResizeProp()
	{
		ptOrigPosBps = CPoint(0,0);
		rcInitPosRect = CRect(0,0,0,0);
		ctmXofPosBps = tmNotChange;
		ctmYofPosBps = tmNotChange;
		ctmWigth = tmNotChange;
		ctmHight = tmNotChange;
		IsPaintBackGround = true;	
	}
};

// ���ڷǿͻ�����ť״̬
enum CButtonState
{
	bsNormal,					// ��ͨ״̬
	bsHot,						// �ȵ�״̬ 
	bsPress						// ����״̬ 
};

// ���ڷǿͻ�����ťID
enum CButtonID
{
	btClose = 1,
	btMaxOrRestore,
	btMin,
	btSetting
};
//*********************************
// �ɵ���С�Ի�����
//*********************************
class CBaseDialog : public CDialog
{
	DECLARE_DYNAMIC(CBaseDialog)
public:
	explicit CBaseDialog(const UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CBaseDialog();

	// --------------------------------------------------
	// ժҪ��
	//	  ���ÿؼ��ض�λ����
	// ������
	//     1.newItem - �ؼ��ض�λ����
	// --------------------------------------------------
	void SetResize(ControlsResizeProp &Item);

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ñ߿�߶�
	// --------------------------------------------------
	int GetBorderHeight()
	{
		return m_BorderHeight;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ñ߿���
	// --------------------------------------------------
	int GetBorderWidth()
	{
		return m_BorderWidth;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ñ������߶�
	// --------------------------------------------------
	int GetCaptionHeight()
	{
		return m_CaptionHeight;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ô��ڴ�С���Σ��������
	// --------------------------------------------------
	CRect & GetWindSizeRect()
	{
		return m_rcWndSize;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ����߿����
	// --------------------------------------------------
	CRect & GetLeftBorderRect()
	{
		return m_rcLeftBorder;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ���ұ߿����
	// --------------------------------------------------
	CRect & GetRightBorderRect()
	{
		return m_rcRightBorder;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ���±߿����
	// --------------------------------------------------
	CRect & GetBottomBorderRect()
	{
		return m_rcBottomBorder;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ùرհ�ť��������
	// --------------------------------------------------
	CRect & GetCloseButtonRect()
	{
		return m_rcCloseButton;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ����󻯴��ڰ�ť��������
	// --------------------------------------------------
	CRect & GetMaxButtonRect()
	{
		return m_rcMaxButton;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�����ð�ť����
	// --------------------------------------------------
	CRect & GetSettingButtonRect()
	{
		return m_rcSettingButton;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ����С�����ڰ�ť��������
	// --------------------------------------------------
	CRect & GetMinButtonRect()
	{
		return m_rcMinButton;
	}

	// --------------------------------------------------
	// ժҪ��
	//	  ȡ����󻯰�ť״̬
	// --------------------------------------------------
	CButtonState & GetMaxButtonState()
	{
		return m_MaxButtonState;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ����С��ť״̬
	// --------------------------------------------------
	CButtonState & GetMinButtonState()
	{
		return m_MinButtonState;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�ùرհ�ť״̬
	// --------------------------------------------------
	CButtonState & GetCloseButtonState()
	{
		return m_CloseButtonState;
	}
	// --------------------------------------------------
	// ժҪ��
	//	  ȡ�����ð�ť״̬
	// --------------------------------------------------
	CButtonState & GetSettingButtonState()
	{
		return m_SettingButtonState;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	// --------------------------------------------------
	// ժҪ��
	//	  ��С�ı�ǰ����Ϣ��Ӧ����
	// --------------------------------------------------

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	// --------------------------------------------------
	// ժҪ��
	//	  ��С�ı�����Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// ժҪ��
	//	  ����������Ϣ��Ӧ
	// --------------------------------------------------
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------
	// ժҪ��
	//	  �ǿͻ�������
	// --------------------------------------------------
	afx_msg void OnNcPaint();

	// --------------------------------------------------
	// ժҪ��
	//	  �ǿͻ�������
	// --------------------------------------------------
	afx_msg BOOL OnNcActivate(BOOL bActive);

	// --------------------------------------------------
	// ժҪ��
	//	  �ǿͻ�����С����
	// --------------------------------------------------
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	// --------------------------------------------------
	// ժҪ��
	//	  �ǿͻ��������Ϣ��Ӧ
	// --------------------------------------------------
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);

	// --------------------------------------------------
	// ժҪ��
	//	  ����λ�÷����仯��Ϣ��Ӧ
	// --------------------------------------------------
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	// --------------------------------------------------
	// ժҪ��
	//	  ��ʼ���Ի�����
	// --------------------------------------------------
	virtual BOOL OnInitDialog();
	
	// --------------------------------------------------
	// ժҪ��
	//	  �����ض�λ���͡��������͡�ԭ��λ��������Լ����¾�������
	//     1.tagCtrtPosAndSize - ԭλ�ô�С����
	//     2.RateX - x����������
	//     3.RateY - y����������
	// --------------------------------------------------
	CRect _GetNewRect(const ControlsResizeProp &newItem);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CSize m_WndInitSize;						// ����ԭʼ��С
	CSize m_WndNewSize;							// �����´�С
	vector<ControlsResizeProp> m_vectCtrls;		// �ؼ���Сλ������
	CRgn m_rgnBackGround;						// �ؼ���������ı�������ָ��

	int m_BorderWidth;							// �߽��߿��
	int m_BorderHeight;							// �߽��߸߶�
	int m_CaptionHeight;						// �������ĸ߶�
	CRect m_rcWndSize;							// ���������С�������Ͻ�Ϊ��0��0�������꣬����WindowsDCʱʹ��			
	CRect m_rcCaption;							// ��������Դ������Ͻ�����
	CRect m_rcClient;							// �ͻ�������Դ������Ͻ�����
	CRect m_rcLeftBorder;						// ��߿������Դ������Ͻ�����
	CRect m_rcRightBorder;						// �ұ߿������Դ������Ͻ�����
	CRect m_rcBottomBorder;						// �±߿������Դ������Ͻ�����

	CRect m_rcMaxButton;						// ��󻯰�ť���Σ���Դ������Ͻ�����
	CRect m_rcMinButton;						// ��С����ť���Σ���Դ������Ͻ�����
	CRect m_rcCloseButton;						// �رհ�ť���Σ���Դ������Ͻ�����
	CRect m_rcSettingButton;					// ���ð�ť���Σ���Դ������Ͻ�����

	CButtonState m_MaxButtonState;				// ��󻯰�ť״̬
	CButtonState m_MinButtonState;				// ��С����ť״̬
	CButtonState m_CloseButtonState;			// �رհ�ť״̬
	CButtonState m_SettingButtonState;			// ���ð�ť״̬

	CSkinObject * m_pSkin;						// Ƥ��ָ��
	CToolTipCtrl m_ToolTip;						// ��ʾ����
};
