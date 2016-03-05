//*****************************************************************************
// �ļ��� : Matrix.h
// 
// ��Ҫ   : ������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-6       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
// double���;����࣬���ڿ�ѧ����  
// �̳���MATRIX��  
// ʵ�ֳ��ò��������أ���ʵ�ּ�����������ʽ�����Լ�LU�ֽ�  
//*****************************************************************************
//ע����������ֵ����0��ʼ  
#pragma once
#include "MatrixTemplate.h"

class CMatrix:public MATRIX<double>  
{  
public:  
	CMatrix():MATRIX<double>(){}  
	CMatrix( int c, int r ):MATRIX<double>(c,r){}  
	CMatrix( int c, int r,double initval);  
	CMatrix( const CMatrix& m){ *this  = m; }  

	CMatrix& operator =( const CMatrix& m );  
	CMatrix& operator+=( const CMatrix& m );  
	CMatrix& operator-=( const CMatrix& m );  
	CMatrix& operator*=( const CMatrix& m );  
	CMatrix& operator/=( const CMatrix& m );  

	bool operator==( const CMatrix& rhs ) const;							// ���ز�����==  
	bool operator!=( const CMatrix& rhs ) const;							// ���ز�����!=  
	CMatrix operator+( const CMatrix& rhs ) const;							// ���ز�����+  
	CMatrix operator-( const CMatrix& rhs ) const;							// ���ز�����-  
	CMatrix operator*( const CMatrix& rhs ) const;							// ���ز�����*�������������  
	CMatrix operator*( double aa ) const;									// ���ز�����*���������һ����ֵ
	CMatrix operator/( const CMatrix& rhs ) const;							// ���ز�����/  
	CMatrix operator/( double aa ) const;									// ���ز�����/���������һ����ֵ
	double Det() const;														// ��������ʽ  
	double Det(int start, int end ) const;									// �����Ӿ�������ʽ,
	double Algebraic(int indexRow, int indexCol ) const;					// ����ĳԪ�صĴ�������ʽʽ
	CMatrix AdjointMatr() const;											// ���ذ������
	CMatrix Abs() const;													// ��������Ԫ�صľ���ֵ  
	double Max() const;														// ����Ԫ�ص����ֵ  
	double Max(int& row, int& col) const;									// ����Ԫ���е����ֵ�����±�  
	double Min() const;														// ����Ԫ�ص���Сֵ  
	double Min(int& row, int& col) const;									// ����Ԫ�ص���Сֵ�����±�  
	CMatrix Trans() const;													// ����ת�þ���  
	CMatrix SubMatrix(int rb,int re,int cb,int ce) const;					// �����Ӿ���  
	CMatrix Inverse() const;												// ���������  
	CMatrix LU() const;														// ���㷽���LU�ֽ�  

private:
	inline double LxAbs( double d )	const									// �����ֵ
	{  
		return (d>=0)?(d):(-d);  
	}  

	inline bool isSignRev( const vector<double>& v )	const				// ������ʽֵʱ���ж��Ƿ���
	{  
		int p = 0;  
		int sum = 0;  
		int n = (int)v.size();  

		for ( int i = 0; i < n; ++i )  
		{  
			p = (int)v[i];  
			if ( p >= 0 )  
			{  
				sum += p;  
			}  
		}  

		// �����ż����˵�������  
		if ( sum % 2 == 0 ) 
		{  
			return false;  
		}  
		return true;  
	}  
	inline int max_idx(int k, int n )  const
	{  
		// ����k�������У�k���������Ԫ�ص��к�
		int p = k;  
		for ( int i = k+1; i < n; ++i )  
		{  
			if ( LxAbs(array[p][k]) < LxAbs(array[i][k]) )  
			{  
				p = i;  
			}  
		}  
		return p;  
	}  
};  
