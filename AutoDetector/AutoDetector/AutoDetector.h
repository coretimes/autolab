// AutoDetector.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAutoDetectorApp:
// �йش����ʵ�֣������ AutoDetector.cpp
//

class CAutoDetectorApp : public CWinApp
{
public:
	CAutoDetectorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAutoDetectorApp theApp;