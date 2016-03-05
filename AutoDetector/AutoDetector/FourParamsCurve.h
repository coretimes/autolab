//*****************************************************************************
// �ļ��� : FourParamsAlgorithm.h
// 
// ��Ҫ   : �Ĳ�������㷨�� (ͷ�ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-7       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
//
//		       �Ĳ�������㷨ԭ��
// ------------------------------------------
//  �Ĳ���������ʽ:
//  y =		(Am - A0) / ( 1 + (X/X0) ^ P) + A0
//  
//  ����㷨��Levenberg-Marquardt�㷨
//*****************************************************************************

#pragma once
#include "CoordPoint.h"
#include <assert.h>
#include <vector>
#include "math.h"
#include "CoordPoint.h"
#include "Matrix.h"

using namespace std;

class CFourParamsCurve
{
public:
	CFourParamsCurve (const vector<CCoordPoint> & SamplePointArr);	// �������ʱ,��Ҫ�������������
	bool Reset(const vector<CCoordPoint> &SamplePointArr);					// ������������������
	double GetCorrelationCoe() const;										// ȡ�����ϵ��
	double GetYByX(const double &x) const;									// ���ݸ�����x����ֵ,ȡ�ø������ϵ�y����ֵ
	double GetXByY(const double &y) const;									// ���ݸ�����y����ֵ,����xֵ,���󷽳�S(x) - y = 0��
	double GetAm()const { return m_Am; }
	double GetA0()const { return m_A0; }
	double GetX0()const { return m_X0; }
	double GetP()const { return m_P; }
public:
	CFourParamsCurve(void);
	~CFourParamsCurve(void);

private:
	double m_A0;														// ����A0
	double m_Am;														// ����Am
	double m_X0;														// ����X0
	double m_P;															// ����P

private:
	vector<CCoordPoint> m_SamplePointArr;								// ���ߵ������ڵ�����
	int m_PointCount;													// �����ڵ�����

private:
	void _Calculate();													// ���㺯��
	void _Logit_Linear();												// ��Logit-Ln���Իع�
	void _LM_Nonelinear();												// Levenberg-Marquardt �����Իع�

	inline double dAm(double x)	const									// ���̶�Am������ƫ����
	{
		return 1.0 / ( 1.0 + pow( x / m_X0, m_P));
	}

	inline double dA0(double x)	const									// ���̶�A0������ƫ����
	{
		return 1.0 - dAm(x);
	}

	inline double dX0(double x)	const									// ���̶�X0������ƫ����
	{
		if(abs(x) < 0.00001 )
			x = 0.00001;
		return  ( m_P / m_X0  ) * 
				( m_Am - m_A0 ) * 
				(pow(x/m_X0, m_P)) /
				pow((1 + pow(x / m_X0, m_P)), 2);
	}

	inline double dP(double x)	const									// ���̶�P������ƫ����
	{
		if(abs(x) < 0.00001 )
			x = 0.00001;
		return  -log(x / m_X0) * dX0(x) * m_X0 / m_P; 
	}

	inline void JacobiMatr(CMatrix & mJ)	const						// ��ǰ����״̬�µ��ſ˱Ⱦ���
	{
		mJ.resize(m_PointCount,4);
		for(int row = 0; row < m_PointCount; row ++)
		{
			mJ[row][0] = dAm(m_SamplePointArr[row].x); 
			mJ[row][1] = dA0(m_SamplePointArr[row].x); 
			mJ[row][2] = dX0(m_SamplePointArr[row].x); 
			mJ[row][3] = dP(m_SamplePointArr[row].x); 
		}
	}

	inline void ParamsMatr(CMatrix & mP)	const						// ��ǰ����״̬�µĲ�������
	{
		mP.resize(4,1);
		mP[0][0] = m_Am; 
		mP[1][0] = m_A0; 
		mP[2][0] = m_X0; 
		mP[3][0] = m_P; 
	}

	inline double YResiduals()	const									// �������yֵ�ͼ���yֵ�Ĳв�
	{
		double dRes = 0;
		for(int i = 0; i < m_PointCount; i ++)
		{
			dRes += pow((m_SamplePointArr[i].y - GetYByX(m_SamplePointArr[i].x)),2);
																		// �� var_y = sum((Y[i] - fitY[i])^2),����yֵ�����yֵ���ƽ��
		}
		return dRes;
	}
	inline void YDevMatr(CMatrix & mY)	const							// ����ʵ��yֵ�ͼ���yֵ�����
	{
		mY.resize(m_PointCount,1);
		for(int i = 0; i < m_PointCount; i ++)
		{
			mY[i][0] = m_SamplePointArr[i].y - GetYByX(m_SamplePointArr[i].x);
		}
	}
};
