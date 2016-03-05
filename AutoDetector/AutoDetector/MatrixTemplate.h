//*****************************************************************************
// �ļ��� : MatrixTemplate.h
// 
// ��Ҫ   : ������ģ��
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-6       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
// ʵ���������͵ľ�����  
//*****************************************************************************

#pragma once
#include <vector>  
using std::vector;  

template <typename Object>  
class MATRIX  
{  
public:  
	explicit MATRIX() : array( 0 ) {}											// ��ֹ�����������캯�����������Զ�����ת�� 

	MATRIX( int rows, int cols):array( rows )									// �������������������
	{  
		for( int i = 0; i < rows; ++i )  
		{  
			array[i].resize( cols );  
		}  
	}  

	MATRIX( const MATRIX<Object>& m ){ *this = m;}								// ���ƹ��캯��  

	void resize( int rows, int cols );											// �ı䵱ǰ�����С  
	bool push_back( const vector<Object>& v );									// �ھ���ĩβ���һ������  
	bool insert_row( int rowindex,const vector<Object>& v );					// �ھ���ָ��λ�����һ������  
	void swap_row( int row1, int row2 );										// �������е�����  
	void swap_col( int col1, int col2 );										// �������е�����  

	int  rows() const{ return (int)array.size(); }								// ���ؾ�������
	int  cols() const { return rows() ? (int)(array[0].size()) : 0; }			// ���ؾ�������
	bool empty() const { return rows() == 0; }									// �Ƿ�Ϊ��  
	bool square() const { return (!(empty()) && rows() == cols()); }			// �Ƿ�Ϊ����  


	const vector<Object>& operator[](int row) const { return array[row]; }		//[]���������أ�����ĳ��������ɵ�����   
	vector<Object>& operator[](int row){ return array[row]; }					//[]���������أ�����ĳ��������ɵ�����  

	MATRIX<Object> & Trans();  
protected:  
	vector< vector<Object> > array;												// ��ά���飬���ݱ���  
};  

//**************************************
// �ı䵱ǰ�����С  
//**************************************
template <typename Object>  
void MATRIX<Object>::resize( int rows, int cols )  
{  
	int rs = this->rows();  
	int cs = this->cols();  

	if ( rows == rs && cols == cs )  
	{  
		return;  
	}  
	else if ( rows == rs && cols != cs )  
	{  
		for ( int i = 0; i < rows; ++i )  
		{  
			array[i].resize( cols );  
		}  
	}  
	else if ( rows != rs && cols == cs )  
	{  
		array.resize( rows );  
		for ( int i = rs; i < rows; ++i )  
		{  
			array[i].resize( cols );  
		}  
	}  
	else  
	{  
		array.resize( rows );  
		for ( int i = 0; i < rows; ++i )  
		{  
			array[i].resize( cols );  
		}  
	}  
}  

//**************************************
// �ھ���ĩβ���һ��  
//**************************************
template <typename Object>  
bool MATRIX<Object>::push_back( const vector<Object>& v )  
{  
	if ( rows() == 0 || cols() == (int)v.size() )  
	{  
		array.push_back( v );  
	}  
	else  
	{  
		return false;  
	}  

	return true;  
}

//**************************************
// �ھ���ָ��λ�����һ������  
//**************************************
template <typename Object>  
bool MATRIX<Object>::insert_row( int rowindex,const vector<Object>& v )  
{  
	if ( rows() == 0 || cols() == (int)v.size() )  
	{  
		array.insert(array.begin() + rowindex,v);  
	}  
	else  
	{  
		return false;  
	}  

	return true;  
}  

//**************************************
// ��������  
//**************************************
template <typename Object>  
void MATRIX<Object>::swap_row( int row1, int row2 )  
{  
	if ( row1 != row2 && row1 >=0 &&  
		row1 < rows() && row2 >= 0 && row2 < rows() )  
	{  
		vector<Object>& v1 = array[row1];  
		vector<Object>& v2 = array[row2];  
		vector<Object> tmp = v1;  
		v1 = v2;  
		v2 = tmp;  
	}  
}  

//**************************************
// ��������  
//**************************************
template <typename Object>  
void MATRIX<Object>::swap_col( int col1, int col2 )  
{  
	if ( col1 != col2 && col1 >=0 &&  
		col1 < cols() && col2 >= 0 && col2 < cols() )  
	{  
		for(int i = 0; i < rows(); i++)
		{
			Object tmp = array[i][col2];
			array[i][col1] = array[i][col2];
			array[i][col2] = tmp;
		}
	}  
}  

//**************************************
// ����ת��  
//**************************************
template <typename Object>  
MATRIX<Object> & MATRIX<Object>::Trans()  
{  
	MATRIX<Object> ret;  
	if ( empty() ) return ret;  

	int row = cols();  
	int col = rows();  
	ret.resize( row, col );  

	for ( int i = 0; i < row; ++i )  
	{  
		for ( int j = 0; j < col; ++j )  
		{  
			ret[i][j] = array[j][i];  
		}  
	}  

	return ret;  
}  