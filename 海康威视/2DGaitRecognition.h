
// 2DGaitRecognition.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy2DGaitRecognitionApp:
// �йش����ʵ�֣������ 2DGaitRecognition.cpp
//

class CMy2DGaitRecognitionApp : public CWinApp
{
public:
	CMy2DGaitRecognitionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy2DGaitRecognitionApp theApp;