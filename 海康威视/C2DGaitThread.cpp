// C2DGaitThread.cpp : 实现文件
//

#include "stdafx.h"
#include "2DGaitRecognition.h"
#include "C2DGaitThread.h"
#include "include/HCNetSDK.h"
#include "include/plaympeg4.h"
#include "Message.h"
#include <time.h>
LONG g_iPort = -1;  //用于保存软解码的播放库port号
LONG m_lPlayHandle = -1;
BOOL m_pstart=FALSE;
BOOL m_pback=FALSE;
int nFrame=10;
// C2DGaitThread

IMPLEMENT_DYNCREATE(C2DGaitThread, CWinThread)

C2DGaitThread::C2DGaitThread()
{
}

C2DGaitThread::~C2DGaitThread()
{
}

BOOL C2DGaitThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int C2DGaitThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(C2DGaitThread, CWinThread)
	ON_THREAD_MESSAGE(WM_CAMERA_CHECK, OnCameraCheck)
END_MESSAGE_MAP()


// C2DGaitThread 消息处理程序

void C2DGaitThread::OnCameraCheck(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString videoOpenniPath(b);
	SysFreeString(b);
	m_CameraFatherHwnd = (HWND)lParam;
	InitCamera();
}

void C2DGaitThread::InitCamera()
{
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	//注册设备
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	LONG lUserID = NET_DVR_Login_V30("192.0.0.64", 8000, "admin", "12345", &struDeviceInfo);
	NET_DVR_CLIENTINFO struPlayInfo;
	CWnd* pWnd = AfxGetMainWnd();
	pWnd->GetDlgItem(IDC_COLOR_VIEW, &struPlayInfo.hPlayWnd); //设置播放句柄为有效句柄
	g_hWnd = struPlayInfo.hPlayWnd;
	struPlayInfo.lChannel = 1; //预览通道号
	struPlayInfo.lLinkMode = 0; //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	struPlayInfo.sMultiCastIP = "0.0.0.0";
	m_lPlayHandle = NET_DVR_RealPlay_V30(lUserID, &struPlayInfo, NULL, NULL, true);
	int iError = NET_DVR_GetLastError();
	ASSERT(iError == 0);
	if(m_lPlayHandle >=0)
	{
		BOOL bRet = NET_DVR_MakeKeyFrame(lUserID, struPlayInfo.lChannel); //重置I帧
		DWORD m_iNetDelayBufNum = 15; //单视频播放时缓冲区最大帧数，取值范围[1,50]，设置网络延时和播放流畅度可以通过此接口来进行调节
		bRet = NET_DVR_SetPlayerBufNumber(m_lPlayHandle,m_iNetDelayBufNum);
		CString csPath=_T("F:/二维图片路径/");
		CCommon::gr_create_folder(csPath);
		char *pimg=new char[20];
		char *pt=new char[50];
		while(1){
			//软解码实现过程
		
			if (m_pback==TRUE)
			{
				CString WriteMessage=_T("开始采集保存背景！");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				time_t tp = time(NULL);
				tm* t= localtime(&tp);
				sprintf(pimg,"F:/二维图片路径/%d%02d%02d-%02d%02d%02d.bmp", t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
				NET_DVR_CapturePicture(m_lPlayHandle,pimg);
				WriteMessage=_T("背景保存结束！");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				m_pback=FALSE;
				delete t;
				t=NULL;
			}
			if (m_pstart==TRUE)
			{
				CString WriteMessage=_T("开始采集人体步态！");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				clock_t startCTime , endCTime;  
				time_t tp = time(NULL);
				tm* t= localtime(&tp);
				sprintf(pt,"F:/二维图片路径/%d%02d%02d-%02d%02d%02d.avi", t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
				NET_DVR_SaveRealData(m_lPlayHandle,pt);
				::PostMessage(m_CameraFatherHwnd, WM_UPDATE_PROGRESSBAR, (WPARAM)0, (LPARAM)0);
				startCTime = clock(); 
				endCTime = clock();
				while(double((endCTime-startCTime)/CLOCKS_PER_SEC<nFrame)){
                 endCTime = clock();
				 ::PostMessage(m_CameraFatherHwnd, WM_UPDATE_PROGRESSBAR, (WPARAM)((endCTime-startCTime)/CLOCKS_PER_SEC), (LPARAM)0);
				}
				NET_DVR_StopSaveRealData(m_lPlayHandle);
				WriteMessage=_T("步态人体采集完毕！");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				m_pstart=FALSE;
				::PostMessage(m_CameraFatherHwnd, WM_UPDATE_PROGRESSBAR, (WPARAM)nFrame, (LPARAM)0);
				delete t;
				t=NULL;
			}
		}
		delete[] pt;
		delete[] pimg;
	}
	//注销用户
	NET_DVR_Logout(lUserID);
	//释放SDK资源
	NET_DVR_Cleanup();
}
