// C2DCameraThread.cpp : ʵ���ļ�
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



// C2DCameraThread ��Ϣ�������

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
	//ע���豸
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//����豸�����Ľṹ��
	LONG lUserID = NET_DVR_Login_V30("192.0.0.64", 8000, "admin", "12345", &struDeviceInfo);
	if (lUserID < 0)
	{
		if (NET_DVR_GetLastError() == NET_DVR_PASSWORD_ERROR)//�û����������
		{
			//���������Ϣ
		}
		else if(NET_DVR_GetLastError() == NET_DVR_OVER_MAXLINK)//���ӵ�DVR�Ŀͻ��˴ﵽ���
		{				
			//���������Ϣ
		}
	}
	LONG m_lPlayHandle = -1;
	NET_DVR_CLIENTINFO struPlayInfo;
	this->GetDlgItem(IDC_COLOR_VIEW, &struPlayInfo.hPlayWnd); //���ò��ž��Ϊ��Ч���
	g_hWnd = struPlayInfo.hPlayWnd;
	struPlayInfo.lChannel = 1; //Ԥ��ͨ����
	struPlayInfo.lLinkMode = 0; //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	struPlayInfo.sMultiCastIP = "0.0.0.0";
	//�����ʵ�ֹ���
	m_lPlayHandle = NET_DVR_RealPlay_V30(lUserID, &struPlayInfo, NULL, NULL, true);
	int iError = NET_DVR_GetLastError();
	ASSERT(iError == 0);
	if(m_lPlayHandle >=0)
	{
		BOOL bRet = NET_DVR_MakeKeyFrame(lUserID, struPlayInfo.lChannel); //����I֡
		DWORD m_iNetDelayBufNum = 15; //����Ƶ����ʱ���������֡����ȡֵ��Χ[1,50]������������ʱ�Ͳ��������ȿ���ͨ���˽ӿ������е���
		bRet = NET_DVR_SetPlayerBufNumber(m_lPlayHandle,m_iNetDelayBufNum);
	}
}
