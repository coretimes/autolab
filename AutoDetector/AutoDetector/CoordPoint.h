//*****************************************************************************
// �ļ��� : CCoordPoint.h
// 
// ��Ҫ   : ��ά��������Ͷ���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-6       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
 
#pragma once

class CCoordPoint
{
public:
	CCoordPoint(void)
	{
		x = 0.f;
		y = 0.f;
	}

	CCoordPoint(double xx,double yy)
	{
		x = xx;
		y = yy;
	}

public:
	~CCoordPoint(void){};

public:
	double x;																	// x����
	double y;																	// y����

public:
	bool operator < (const CCoordPoint& node) const
	{
		return x < node.x;
	}

	bool operator > (const CCoordPoint& node) const
	{
		return x > node.x;
	}

	bool operator == (const CCoordPoint& node) const
	{
		return x == node.x;
	}
};
