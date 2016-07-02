
// 2DGaitRecognitionDlg.cpp : 实现文件
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


// CMy2DGaitRecognitionDlg 对话框




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


// CMy2DGaitRecognitionDlg 消息处理程序

BOOL CMy2DGaitRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	m_CtrlProgressBar.SetRange(0, nFrame);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2DGaitRecognitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2DGaitRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2DGaitRecognitionDlg::OnBnClickedInit()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pVideoCapture = (C2DGaitThread*)AfxBeginThread(RUNTIME_CLASS(C2DGaitThread));
	Sleep(50);
	m_pVideoCapture->PostThreadMessage(WM_CAMERA_CHECK, (WPARAM)NULL, (LPARAM)this->m_hWnd);
}


void CMy2DGaitRecognitionDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pVideoCapture->PostThreadMessage(WM_CAMERA_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
	CDialogEx::OnCancel();
}

extern BOOL m_pstart;
void CMy2DGaitRecognitionDlg::OnBnClickedCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pstart=TRUE;
}

extern BOOL m_pback;
void CMy2DGaitRecognitionDlg::OnBnClickedInit2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pback=TRUE;
}
LRESULT  CMy2DGaitRecognitionDlg::ReceiveLogMessage(WPARAM wParam, LPARAM lParam)
{
	//写日志信息
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
	//获取当前系统时间
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
	// TODO: 在此添加控件通知处理程序代码
}



void CMy2DGaitRecognitionDlg::OnNMCustomdrawProgressBar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
