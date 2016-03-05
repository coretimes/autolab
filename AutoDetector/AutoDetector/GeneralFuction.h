//*****************************************************************************
// �ļ��� : GeneralFuction.h
// 
// ��Ҫ   : ͨ�ú������Կ��ܻᷴ�����õ����Ա�޹صĺ�����ͳһ�����ڴ�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-9       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <vector>
#include <atlimage.h>

using namespace std; 
class CGeneralFuction
{
private:
	CGeneralFuction(void){};

public:
	static CGeneralFuction * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CGeneralFuction();
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

	~CGeneralFuction(void){};

	// --------------------------------------------------
	// ժҪ��
	//     ��ͺ���
	// ������
	//     1.datalist - ������������
	// ���أ�
	//     �����Ԫ�غ�
	// --------------------------------------------------
	double Sum(const vector<double> & datalist) const;

	// --------------------------------------------------
	// ժҪ��
	//     ��ƽ��ֵ����
	// ������
	//     1.datalist - ������������
	// ���أ�
	//     �����Ԫ��ƽ��ֵ
	// --------------------------------------------------
	double Average(const vector<double> & datalist) const;

	// --------------------------------------------------
	// ժҪ��
	//     �����ϵ������
	// ������
	//     1.datalist - ������������
	// ���أ�
	//     ����ı���ϵ��
	// --------------------------------------------------
	double GetCV(const vector<double> & datalist) const;

	// --------------------------------------------------
	// ժҪ��
	//     �󷽲�
	// ������
	//     1.datalist - ������������
	// ���أ�
	//     ����ķ���
	// --------------------------------------------------
	double Variance(const vector<double> & datalist) const;
	
	// --------------------------------------------------
	// ժҪ��
	//     ���׼��
	// ������
	//     1.datalist - ������������
	// ���أ�
	//     ����ı�׼���
	// --------------------------------------------------
	double StdDev(const vector<double> & datalist) const;

	// --------------------------------------------------
	// ժҪ��
	//     ��������ֵ�ַ������Ѹ�����ֵ�����ڸ���������
	// ������
	//     1.strRluList - �ַ�����ʽ���磺123131/119259/130034
	//     2.arrRluList - ָ���ı��淢��ֵ������
	// --------------------------------------------------
	void ParseRluString(const CString &strRluList,vector<double> & arrRluList) const;

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ӱ����
	// --------------------------------------------------
	void DrawShadowCation(CDC * pDc,const CRect &rcTextRect,const CString &strContext);

	// --------------------------------------------------
	// ժҪ��
	//	 ���ƴ�ֱ�ı�  
	// --------------------------------------------------
	int DrawVerticalText(CDC* pDC, const CString &strText,const CRect & rcTextRect, const UINT &format);

	// --------------------------------------------------
	// ժҪ��
	//	 �����Զ�����Դ����  
	// --------------------------------------------------
	bool LoadImageFromResource(CImage& image,const UINT &nResID,const LPCTSTR &lpTyp);

private:
	static CGeneralFuction * m_pInstance;
};
