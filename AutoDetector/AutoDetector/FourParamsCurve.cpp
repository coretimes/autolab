#include "StdAfx.h"
#include "FourParamsCurve.h"
#include <functional>														// less函数
#include <algorithm>														// sort函数
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

double CFourParamsCurve::GetCorrelationCoe() const	// 计算相关系数
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
// 做Logit-Ln线性回归
// 原四参数方程可变换为如下线性形式：
// ln((y - A0) / ( Am - y)) = p * ln(x0) - p * ln(x)
// 假设出A0 和 Am 的初始值，然后通过线性拟合求出x0 和 p 的初始值参数的初始值
// *****************************************************************
void CFourParamsCurve::_Logit_Linear()
{
	m_A0 = m_SamplePointArr[0].y + 0.00001;													// 将A0的初值设为输入的y值的最大值加1
	m_Am = (m_SamplePointArr.end() - 1)->y - 0.00001;										// 将Am的初值设为输入的y值的最小值减0.1

	// ---------------------------------------------------------------
	// 线性回归的样本点变换
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
	// 最小二乘拟合系数矩阵A = (X_T * X)^(-1) * X_T * Y
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
// Levenberg -Marquardt迭代法原理
// 是牛顿高斯迭代法的改进
// Pk = Pk + Dk
// Dk = (J(Pk)_T * J(Pk) + λ * I)^(-1) * J(Pk)_T * ε(k)
// 其中
// Pk：迭代k次后的值四个参数（Am，A0，X0，P0）形成的矩阵
// Dk：每次迭代Pk的增量矩阵
// J（Pk）：雅克比矩阵，Pk时的值
// ε(k)：拟合出的Y值与实际Y值的差
// λ：阻尼因子，大于0
// I：单位矩阵
// *****************************************************************
void CFourParamsCurve::_LM_Nonelinear()
{
	CMatrix J_Pk, J_Pk_T;				// 雅克比矩阵，m_PointCount行4列					
	JacobiMatr(J_Pk);
	J_Pk_T = J_Pk.Trans();

	CMatrix Pk;							// 参数矩阵，  4行1列
	ParamsMatr(Pk);

	CMatrix Dk;							// 每次迭代Pk的增量矩阵，  4行1列
	CMatrix Ek;							// 测量y值与计算y值的差
	YDevMatr(Ek);

	double residuals = YResiduals(),	// 计算测量y值和计算y值的残差
		   OldResiduals = residuals;
	double lamda = 1.0;					// 阻尼系数λ
	double ee =	pow(2,-20.0);			//ε,终止控制常数
	double factor = 10;					// 增长因子

	int MaxLoopTimes = 200;				// 循环次数不超过200次
	while(MaxLoopTimes > 0)
	{
		MaxLoopTimes--;
		CMatrix temp = J_Pk_T * J_Pk; 
		for(int i = 0; i < temp.rows(); i++)
		{
			temp[i][i] += lamda;		// 加入阻尼系数
		}

		CMatrix hk = J_Pk_T * Ek;		// 计算Dk 
		Dk = temp.Inverse() * hk;

		CMatrix OldEk = Ek;				// 由于新计算出的dk不一定是满足要求的，
		double OldAm = m_Am;			// 所以代入原参数之前，先保存原参数
		double OldA0 = m_A0;
		double OldX0 = m_X0;
		double OldP = m_P;

		m_Am += Dk[0][0];				// 代入参数
		m_A0 += Dk[1][0];
		m_X0 += Dk[2][0];
		m_P  += Dk[3][0];

		ParamsMatr(Pk);					// 保存新参数至矩阵	
		YDevMatr(Ek);					// 用新参数计算实际y值和计算y值的误差
		JacobiMatr(J_Pk);				// 用新参数计算雅克比矩阵
		J_Pk_T = J_Pk.Trans();
		residuals = YResiduals();		// 用新参数计算残差
		if(residuals == OldResiduals)  
			factor = 2.0;


		if(residuals < OldResiduals)	// 如果新残差小于旧残差
		{
			double omiga = sqrt(pow(Dk[0][0],2) + pow(Dk[1][0],2) + pow(Dk[2][0],2) + pow(Dk[3][0],2));
			if(omiga < ee)				// ||Dk|| 达到标准，即可退出
				return;
			lamda /= factor;
			OldResiduals = residuals;
		}
		else							// 如果新残差大于旧残差
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