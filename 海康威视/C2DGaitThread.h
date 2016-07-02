#pragma once
#include "Common.h"


// C2DGaitThread

class C2DGaitThread : public CWinThread
{
	DECLARE_DYNCREATE(C2DGaitThread)

protected:
	C2DGaitThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~C2DGaitThread();
public:
	void OnCameraCheck(WPARAM wParam, LPARAM lParam);
	void InitCamera(); 
public:
	HWND m_CameraFatherHwnd;
	HWND g_hWnd;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


