
// 2DGaitRecognitionDlg.h : ͷ�ļ�
//

#pragma once
#include "C2DGaitThread.h"
#include "Message.h"
// CMy2DGaitRecognitionDlg �Ի���
class CMy2DGaitRecognitionDlg : public CDialogEx
{
// ����
public:
	CMy2DGaitRecognitionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY2DGAITRECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	CProgressCtrl m_CtrlProgressBar;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
