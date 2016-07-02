// C2DCameraThread.cpp : 实现文件
//

#include "stdafx.h"
#include "2DGaitRecognition.h"
#include "C2DCameraThread.h"
#include "include/HCNetSDK.h"
#include "include/plaympeg4.h"

// C2DCameraThread

IMPLEMENT_DYNAMIC(C2DCameraThread, CWnd)

C2DCameraThread::C2DCameraThread()
{

}

C2DCameraThread::~C2DCameraThread()
{
}


BEGIN_MESSAGE_MAP(C2DCameraThread, CWnd)
	ON_THREAD_MESSAGE(WM_CAMERA_CHECK, OnCameraCheck)
END_MESSAGE_MAP()



// C2DCameraThread 消息处理程序

void C2DCameraThread::OnCameraCheck(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString videoOpenniPath(b);
	SysFreeString(b);
	m_CameraFatherHwnd = (HWND)lParam;
	InitCamera(m_CameraFatherHwnd);
}
void C2DCameraThread::InitCamera(HWND g_hWnd)
{
	//注册设备
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	LONG lUserID = NET_DVR_Login_V30("192.0.0.64", 8000, "admin", "12345", &struDeviceInfo);
	if (lUserID < 0)
	{
		if (NET_DVR_GetLastError() == NET_DVR_PASSWORD_ERROR)//用户名密码错误
		{
			//处理错误信息
		}
		else if(NET_DVR_GetLastError() == NET_DVR_OVER_MAXLINK)//连接到DVR的客户端达到最大
		{				
			//处理错误信息
		}
	}
	LONG m_lPlayHandle = -1;
	NET_DVR_CLIENTINFO struPlayInfo;
	this->GetDlgItem(IDC_COLOR_VIEW, &struPlayInfo.hPlayWnd); //设置播放句柄为有效句柄
	g_hWnd = struPlayInfo.hPlayWnd;
	struPlayInfo.lChannel = 1; //预览通道号
	struPlayInfo.lLinkMode = 0; //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	struPlayInfo.sMultiCastIP = "0.0.0.0";
	//软解码实现过程
	m_lPlayHandle = NET_DVR_RealPlay_V30(lUserID, &struPlayInfo, NULL, NULL, true);
	int iError = NET_DVR_GetLastError();
	ASSERT(iError == 0);
	if(m_lPlayHandle >=0)
	{
		BOOL bRet = NET_DVR_MakeKeyFrame(lUserID, struPlayInfo.lChannel); //重置I帧
		DWORD m_iNetDelayBufNum = 15; //单视频播放时缓冲区最大帧数，取值范围[1,50]，设置网络延时和播放流畅度可以通过此接口来进行调节
		bRet = NET_DVR_SetPlayerBufNumber(m_lPlayHandle,m_iNetDelayBufNum);
	}
}
