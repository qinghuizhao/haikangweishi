// C2DGaitThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2DGaitRecognition.h"
#include "C2DGaitThread.h"
#include "include/HCNetSDK.h"
#include "include/plaympeg4.h"
#include "Message.h"
#include <time.h>
LONG g_iPort = -1;  //���ڱ��������Ĳ��ſ�port��
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int C2DGaitThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(C2DGaitThread, CWinThread)
	ON_THREAD_MESSAGE(WM_CAMERA_CHECK, OnCameraCheck)
END_MESSAGE_MAP()


// C2DGaitThread ��Ϣ�������

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
	// ��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	//ע���豸
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//����豸�����Ľṹ��
	LONG lUserID = NET_DVR_Login_V30("192.0.0.64", 8000, "admin", "12345", &struDeviceInfo);
	NET_DVR_CLIENTINFO struPlayInfo;
	CWnd* pWnd = AfxGetMainWnd();
	pWnd->GetDlgItem(IDC_COLOR_VIEW, &struPlayInfo.hPlayWnd); //���ò��ž��Ϊ��Ч���
	g_hWnd = struPlayInfo.hPlayWnd;
	struPlayInfo.lChannel = 1; //Ԥ��ͨ����
	struPlayInfo.lLinkMode = 0; //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	struPlayInfo.sMultiCastIP = "0.0.0.0";
	m_lPlayHandle = NET_DVR_RealPlay_V30(lUserID, &struPlayInfo, NULL, NULL, true);
	int iError = NET_DVR_GetLastError();
	ASSERT(iError == 0);
	if(m_lPlayHandle >=0)
	{
		BOOL bRet = NET_DVR_MakeKeyFrame(lUserID, struPlayInfo.lChannel); //����I֡
		DWORD m_iNetDelayBufNum = 15; //����Ƶ����ʱ���������֡����ȡֵ��Χ[1,50]������������ʱ�Ͳ��������ȿ���ͨ���˽ӿ������е���
		bRet = NET_DVR_SetPlayerBufNumber(m_lPlayHandle,m_iNetDelayBufNum);
		CString csPath=_T("F:/��άͼƬ·��/");
		CCommon::gr_create_folder(csPath);
		char *pimg=new char[20];
		char *pt=new char[50];
		while(1){
			//�����ʵ�ֹ���
		
			if (m_pback==TRUE)
			{
				CString WriteMessage=_T("��ʼ�ɼ����汳����");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				time_t tp = time(NULL);
				tm* t= localtime(&tp);
				sprintf(pimg,"F:/��άͼƬ·��/%d%02d%02d-%02d%02d%02d.bmp", t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
				NET_DVR_CapturePicture(m_lPlayHandle,pimg);
				WriteMessage=_T("�������������");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				m_pback=FALSE;
				delete t;
				t=NULL;
			}
			if (m_pstart==TRUE)
			{
				CString WriteMessage=_T("��ʼ�ɼ����岽̬��");
				::PostMessage(m_CameraFatherHwnd, WM_WRITE_MESSAGE, (WPARAM)WriteMessage.AllocSysString(), (LPARAM)NULL);
				clock_t startCTime , endCTime;  
				time_t tp = time(NULL);
				tm* t= localtime(&tp);
				sprintf(pt,"F:/��άͼƬ·��/%d%02d%02d-%02d%02d%02d.avi", t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
				NET_DVR_SaveRealData(m_lPlayHandle,pt);
				::PostMessage(m_CameraFatherHwnd, WM_UPDATE_PROGRESSBAR, (WPARAM)0, (LPARAM)0);
				startCTime = clock(); 
				endCTime = clock();
				while(double((endCTime-startCTime)/CLOCKS_PER_SEC<nFrame)){
                 endCTime = clock();
				 ::PostMessage(m_CameraFatherHwnd, WM_UPDATE_PROGRESSBAR, (WPARAM)((endCTime-startCTime)/CLOCKS_PER_SEC), (LPARAM)0);
				}
				NET_DVR_StopSaveRealData(m_lPlayHandle);
				WriteMessage=_T("��̬����ɼ���ϣ�");
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
	//ע���û�
	NET_DVR_Logout(lUserID);
	//�ͷ�SDK��Դ
	NET_DVR_Cleanup();
}
