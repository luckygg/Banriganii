
// BanriganViewerDlg.h : header file
//

#pragma once

#include "include/Banriganii.h"
#include "include/Draw.h"

#define CAMERA_WIDTH	640
#define CAMERA_HEIGHT	480
#define REG_WIDTH		128
#define REG_HEIGHT		128

// CBanriganViewerDlg dialog
class CBanriganViewerDlg : public CDialogEx
{
// Construction
public:
	CBanriganViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BANRIGANVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public :
	BYTE* m_pImgBuf;			// Image Buffer
	BYTE* m_pRegBuf;			// POC(Register == Reference Image) Buffer
	BITMAPINFO* m_pImgBmpInfo;	// Image Buffer Bitmap Info
	BITMAPINFO* m_pPocBmpInfo;	// POC(Reference Image) Buffer Bitmap Info
	CBanrigan m_Banrigan;		// Banrigan Class
	
private :
	int m_nRbtnMonitor;
	void InitControls();
	void CreateImgBmpInfo(int nWidth, int nHeight);
	void CreatePocBmpInfo(int nWidth, int nHeight);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnection();
	afx_msg void OnBnClickedBtnGetimage();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnLive();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnMonitor();
	afx_msg void OnBnClickedRBtnMonitor(UINT ID);
	
	afx_msg void OnBnClickedBtnMeasure();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedChkbtnSetpoc();
	afx_msg void OnBnClickedBtnWriteRegister();
	afx_msg void OnBnClickedBtnGetregimg();
	afx_msg void OnUpdateRegister();
	afx_msg void OnBnClickedBtnAlarmreset();
};
