
// 2DGaitRecognitionDlg.h : 头文件
//

#pragma once
#include "C2DGaitThread.h"
#include "Message.h"
// CMy2DGaitRecognitionDlg 对话框
class CMy2DGaitRecognitionDlg : public CDialogEx
{
// 构造
public:
	CMy2DGaitRecognitionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2DGAITRECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	CProgressCtrl m_CtrlProgressBar;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInit();
public:
	C2DGaitThread* m_pVideoCapture;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel2();
	afx_msg void OnBnClickedInit2();
	afx_msg void OnStnClickedColorView();
public:
	LRESULT ReceiveUpdateProgressBar(WPARAM wParam, LPARAM lParam);
	LRESULT ReceiveLogMessage(WPARAM wParam, LPARAM lParam);
	void  gr_write_log(CString &strLogMsg);

	afx_msg void OnNMCustomdrawProgressBar(NMHDR *pNMHDR, LRESULT *pResult);
};
