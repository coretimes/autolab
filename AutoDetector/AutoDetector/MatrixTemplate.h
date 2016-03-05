//*****************************************************************************
// 文件名 : MatrixTemplate.h
// 
// 概要   : 矩阵类模板
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-6       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
// 实现任意类型的矩阵类  
//*****************************************************************************

#pragma once
#include <vector>  
using std::vector;  

template <typename Object>  
class MATRIX  
{  
public:  
	explicit MATRIX() : array( 0 ) {}											// 禁止“单参数构造函数”被用于自动类型转换 

	MATRIX( int rows, int cols):array( rows )									// 输入行列数，构造矩阵
	{  
		for( int i = 0; i < rows; ++i )  
		{  
			array[i].resize( cols );  
		}  
	}  

	MATRIX( const MATRIX<Object>& m ){ *this = m;}								// 复制构造函数  

	void resize( int rows, int cols );											// 改变当前矩阵大小  
	bool push_back( const vector<Object>& v );									// 在矩阵末尾添加一行数据  
	bool insert_row( int rowindex,const vector<Object>& v );					// 在矩阵指定位置添加一行数据  
	void swap_row( int row1, int row2 );										// 将换两行的数据  
	void swap_col( int col1, int col2 );										// 将换两列的数据  

	int  rows() const{ return (int)array.size(); }								// 返回矩阵行数
	int  cols() const { return rows() ? (int)(array[0].size()) : 0; }			// 返回矩阵列数
	bool empty() const { return rows() == 0; }									// 是否为空  
	bool square() const { return (!(empty()) && rows() == cols()); }			// 是否为方阵  


	const vector<Object>& operator[](int row) const { return array[row]; }		//[]操作符重载，返回某行数据组成的数组   
	vector<Object>& operator[](int row){ return array[row]; }					//[]操作符重载，返回某列数据组成的数组  

	MATRIX<Object> & Trans();  
protected:  
	vector< vector<Object> > array;												// 二维数组，数据保存  
};  

//**************************************
// 改变当前矩阵大小  
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
// 在矩阵末尾添加一行  
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
// 在矩阵指定位置添加一行数据  
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
// 交换两行  
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
// 交换两列  
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
// 矩阵转置  
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