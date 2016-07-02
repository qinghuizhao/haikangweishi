
// 2DGaitRecognitionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2DGaitRecognition.h"
#include "2DGaitRecognitionDlg.h"
#include "afxdialogex.h"
#include "C2DGaitThread.h"
extern int nFrame;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2DGaitRecognitionDlg �Ի���




CMy2DGaitRecognitionDlg::CMy2DGaitRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2DGaitRecognitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2DGaitRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_CtrlProgressBar);
}

BEGIN_MESSAGE_MAP(CMy2DGaitRecognitionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT, &CMy2DGaitRecognitionDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDCANCEL, &CMy2DGaitRecognitionDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDCANCEL2, &CMy2DGaitRecognitionDlg::OnBnClickedCancel2)
	ON_BN_CLICKED(IDC_INIT2, &CMy2DGaitRecognitionDlg::OnBnClickedInit2)
	ON_STN_CLICKED(IDC_COLOR_VIEW, &CMy2DGaitRecognitionDlg::OnStnClickedColorView)
	ON_MESSAGE(WM_WRITE_MESSAGE, &CMy2DGaitRecognitionDlg::ReceiveLogMessage)
	ON_MESSAGE(WM_UPDATE_PROGRESSBAR, &CMy2DGaitRecognitionDlg::ReceiveUpdateProgressBar)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS_BAR, &CMy2DGaitRecognitionDlg::OnNMCustomdrawProgressBar)
END_MESSAGE_MAP()


// CMy2DGaitRecognitionDlg ��Ϣ�������

BOOL CMy2DGaitRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_CtrlProgressBar.SetRange(0, nFrame);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy2DGaitRecognitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy2DGaitRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2DGaitRecognitionDlg::OnBnClickedInit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pVideoCapture = (C2DGaitThread*)AfxBeginThread(RUNTIME_CLASS(C2DGaitThread));
	Sleep(50);
	m_pVideoCapture->PostThreadMessage(WM_CAMERA_CHECK, (WPARAM)NULL, (LPARAM)this->m_hWnd);
}


void CMy2DGaitRecognitionDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_pVideoCapture->PostThreadMessage(WM_CAMERA_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
	CDialogEx::OnCancel();
}

extern BOOL m_pstart;
void CMy2DGaitRecognitionDlg::OnBnClickedCancel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pstart=TRUE;
}

extern BOOL m_pback;
void CMy2DGaitRecognitionDlg::OnBnClickedInit2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pback=TRUE;
}
LRESULT  CMy2DGaitRecognitionDlg::ReceiveLogMessage(WPARAM wParam, LPARAM lParam)
{
	//д��־��Ϣ
	BSTR b = (BSTR)wParam;
	CString strLogMsg(b);
	SysFreeString(b);
	gr_write_log(strLogMsg);
	return 0;
}
LRESULT CMy2DGaitRecognitionDlg::ReceiveUpdateProgressBar(WPARAM wParam, LPARAM lParam)
{
	int nPos = (int)wParam;
	m_CtrlProgressBar.SetPos(nPos);
	return 0;
}
void  CMy2DGaitRecognitionDlg::gr_write_log(CString &strLogMsg)
{
	//��ȡ��ǰϵͳʱ��
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d:%d"), time.GetYear(),
		time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
	pEdit->SetSel(pEdit->GetWindowTextLength(), -1);
	pEdit->ReplaceSel(strTime + _T(" ") + strLogMsg + _T("\r\n"));
}

void CMy2DGaitRecognitionDlg::OnStnClickedColorView()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void CMy2DGaitRecognitionDlg::OnNMCustomdrawProgressBar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
