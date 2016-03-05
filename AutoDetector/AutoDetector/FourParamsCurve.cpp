#include "StdAfx.h"
#include "FourParamsCurve.h"
#include <functional>														// less����
#include <algorithm>														// sort����
#include "float.h"
CFourParamsCurve::CFourParamsCurve(void)
{
}

CFourParamsCurve::~CFourParamsCurve(void)
{
	m_SamplePointArr.clear();
}

CFourParamsCurve::CFourParamsCurve(const vector<CCoordPoint> &InterNodeArr)
{
	Reset(InterNodeArr);
}

bool CFourParamsCurve::Reset(const vector<CCoordPoint> &InterNodeArr)
{
	m_SamplePointArr.clear();
	m_PointCount = (int)InterNodeArr.size();
	if( m_PointCount < 4)
		return false;
	for(int i= 0; i < m_PointCount; i++)
	{
		m_SamplePointArr.push_back(InterNodeArr[i]);
	}
	sort(m_SamplePointArr.begin(), m_SamplePointArr.end(), less<CCoordPoint>());
	_Calculate();

	if(_isnan(m_Am))
		return false;
	if(_isnan(m_A0))
		return false;
	if(_isnan(m_X0))
		return false;
	if(_isnan(m_P))
		return false;
	return true;
}

double CFourParamsCurve::GetCorrelationCoe() const	// �������ϵ��
{
	double sumy=0,										// sum(y)
		   sum1=0,										// sum((y - y_aver)*(y_fit - y_aver))
		   sum2=0,										// sum((y - y_aver)^2)
		   sum3=0,										// sum((y_fit - y_aver)^2)
		   y_aver=0;

	for(int i=0; i<m_PointCount; i++)
	{
		sumy += m_SamplePointArr[i].y; 
	}

	y_aver = sumy / m_PointCount;

	for(int i=0; i<m_PointCount; i++)
	{
		double y_fit = GetYByX(m_SamplePointArr[i].x);
		sum1 += (m_SamplePointArr[i].y - y_aver) * (y_fit - y_aver);
		sum2 += (m_SamplePointArr[i].y - y_aver) * (m_SamplePointArr[i].y - y_aver);
		sum3 += (y_fit - y_aver) * (y_fit - y_aver);
	}
	return sum1 / sqrt(sum2 * sum3);
}

double CFourParamsCurve::GetYByX(const double &x) const
{
	return (m_Am - m_A0) / ( 1.0 + pow( x / m_X0, m_P)) + m_A0;
}
double CFourParamsCurve::GetXByY(const double &y) const
{
	return pow(abs((m_Am - y)/(y - m_A0)),1/m_P) * m_X0;
}

void CFourParamsCurve::_Calculate()
{
	_Logit_Linear();
	_LM_Nonelinear();
}

// *****************************************************************
// ��Logit-Ln���Իع�
// ԭ�Ĳ������̿ɱ任Ϊ����������ʽ��
// ln((y - A0) / ( Am - y)) = p * ln(x0) - p * ln(x)
// �����A0 �� Am �ĳ�ʼֵ��Ȼ��ͨ������������x0 �� p �ĳ�ʼֵ�����ĳ�ʼֵ
// *****************************************************************
void CFourParamsCurve::_Logit_Linear()
{
	m_A0 = m_SamplePointArr[0].y + 0.00001;													// ��A0�ĳ�ֵ��Ϊ�����yֵ�����ֵ��1
	m_Am = (m_SamplePointArr.end() - 1)->y - 0.00001;										// ��Am�ĳ�ֵ��Ϊ�����yֵ����Сֵ��0.1

	// ---------------------------------------------------------------
	// ���Իع��������任
	// ---------------------------------------------------------------
	CMatrix xin(m_PointCount,2),yin(m_PointCount,1);
	for(int i=0; i< m_PointCount; i++)
	{
		double tempX ,tempY; 
		if( m_SamplePointArr[i].x < 0.00001)
			tempX = log(0.00001);
		else
			tempX = log(m_SamplePointArr[i].x);

		tempY = log((m_SamplePointArr[i].y - m_A0) / ( m_Am - m_SamplePointArr[i].y));
	
		xin[i][0] = 1.0;
		xin[i][1] = tempX;

		yin[i][0] = tempY;
	}

	// ---------------------------------------------------------
	// ��С�������ϵ������A = (X_T * X)^(-1) * X_T * Y
	// ---------------------------------------------------------
	CMatrix x_T = xin.Trans();
	CMatrix matrixA = (x_T * xin).Inverse() * x_T * yin;

	m_P = -matrixA[1][0];
	if(m_P == 0)
	{
		ASSERT(0);
		return ;
	}
	m_X0 = exp(matrixA[0][0] / m_P);
}

// *****************************************************************
// Levenberg -Marquardt������ԭ��
// ��ţ�ٸ�˹�������ĸĽ�
// Pk = Pk + Dk
// Dk = (J(Pk)_T * J(Pk) + �� * I)^(-1) * J(Pk)_T * ��(k)
// ����
// Pk������k�κ��ֵ�ĸ�������Am��A0��X0��P0���γɵľ���
// Dk��ÿ�ε���Pk����������
// J��Pk�����ſ˱Ⱦ���Pkʱ��ֵ
// ��(k)����ϳ���Yֵ��ʵ��Yֵ�Ĳ�
// �ˣ��������ӣ�����0
// I����λ����
// *****************************************************************
void CFourParamsCurve::_LM_Nonelinear()
{
	CMatrix J_Pk, J_Pk_T;				// �ſ˱Ⱦ���m_PointCount��4��					
	JacobiMatr(J_Pk);
	J_Pk_T = J_Pk.Trans();

	CMatrix Pk;							// ��������  4��1��
	ParamsMatr(Pk);

	CMatrix Dk;							// ÿ�ε���Pk����������  4��1��
	CMatrix Ek;							// ����yֵ�����yֵ�Ĳ�
	YDevMatr(Ek);

	double residuals = YResiduals(),	// �������yֵ�ͼ���yֵ�Ĳв�
		   OldResiduals = residuals;
	double lamda = 1.0;					// ����ϵ����
	double ee =	pow(2,-20.0);			//��,��ֹ���Ƴ���
	double factor = 10;					// ��������

	int MaxLoopTimes = 200;				// ѭ������������200��
	while(MaxLoopTimes > 0)
	{
		MaxLoopTimes--;
		CMatrix temp = J_Pk_T * J_Pk; 
		for(int i = 0; i < temp.rows(); i++)
		{
			temp[i][i] += lamda;		// ��������ϵ��
		}

		CMatrix hk = J_Pk_T * Ek;		// ����Dk 
		Dk = temp.Inverse() * hk;

		CMatrix OldEk = Ek;				// �����¼������dk��һ��������Ҫ��ģ�
		double OldAm = m_Am;			// ���Դ���ԭ����֮ǰ���ȱ���ԭ����
		double OldA0 = m_A0;
		double OldX0 = m_X0;
		double OldP = m_P;

		m_Am += Dk[0][0];				// �������
		m_A0 += Dk[1][0];
		m_X0 += Dk[2][0];
		m_P  += Dk[3][0];

		ParamsMatr(Pk);					// �����²���������	
		YDevMatr(Ek);					// ���²�������ʵ��yֵ�ͼ���yֵ�����
		JacobiMatr(J_Pk);				// ���²��������ſ˱Ⱦ���
		J_Pk_T = J_Pk.Trans();
		residuals = YResiduals();		// ���²�������в�
		if(residuals == OldResiduals)  
			factor = 2.0;


		if(residuals < OldResiduals)	// ����²в�С�ھɲв�
		{
			double omiga = sqrt(pow(Dk[0][0],2) + pow(Dk[1][0],2) + pow(Dk[2][0],2) + pow(Dk[3][0],2));
			if(omiga < ee)				// ||Dk|| �ﵽ��׼�������˳�
				return;
			lamda /= factor;
			OldResiduals = residuals;
		}
		else							// ����²в���ھɲв�
		{
			m_Am = OldAm;
			m_A0 = OldA0;
			m_X0 = OldX0;
			m_P = OldP;
			lamda *= factor;
			Ek = OldEk;
		}
	}
}