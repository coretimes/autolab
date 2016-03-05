#pragma once

typedef unsigned char		uchar;
typedef unsigned char		uint8;
typedef	  signed char		sint8;
typedef unsigned short		uint16;
typedef	  signed short		sint16;
typedef unsigned int		uint32;
typedef	  signed int		sint32;
typedef unsigned __int64	uint64;
typedef   signed __int64	sint64;

typedef uint32				TestUnit_ID;		// ���Ե�ԪID
typedef uint32				Metering_ID;		// ���ڵ�ID
typedef uint16				Tick_ID;			// ������ʱ����
typedef uint32				Table_Key;			// ���ݿ��keyֵ
typedef uint32				Sample_Key;			// ������keyֵ
typedef uint32				Standard_Key;		// ��׼Ʒ��keyֵ
typedef uint32				Curve_Key;			// ��׼����keyֵ
typedef uint32				Result_Key;			// �����keyֵ
typedef uint32				Reagent_Key;		// �Լ���keyֵ
typedef uint16				Sample_Pos;			// ������λ������
typedef uint16				Reagent_Pos;		// �Լ���λ������
typedef uint16				Incu_Pos;			// ������λ������
typedef int					ConcPoint_Index;	// ͬһ���Ե�Ԫ�ڣ�Ũ�ȵ�����
typedef int					Metering_Index;		// ͬһ�������Խڵ��ڣ����ڵ�����

// ��Ŀ���ƺͼ����ɵġ�˫���ơ�����
typedef struct
{
	CString ProjectName;		// ��Ŀ����
	CString ShortName;			// ��Ŀ���
} ProjectFullName;

//*****************************************************
// ������������
//*****************************************************
enum CurveType
{
	Default = 0,
	Spline,	
	FourParams
};
