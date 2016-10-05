
// BanriganViewerDlg.h : header file
//

#pragma once

#include "include/Banriganii.h"
#include "include/Draw.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define POC_WIDTH 128
#define POC_HEIGHT 128

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
	BYTE* m_pPocBuf;			// POC(Register == Reference Image) Buffer
	BITMAPINFO* m_pImgBmpInfo;	// Image Buffer Bitmap Info
	BITMAPINFO* m_pPocBmpInfo;	// POC(Reference Image) Buffer Bitmap Info
	CBanrigan m_Banrigan;		// Banrigan Class
	
private :
	int m_nRbtnMonitor;
	void InitControls();
	void CreateImgBmpInfo(int nWidth, int nHeight);
	void CreatePocBmpInfo(int nWidth, int nHeight);
	void GetROIImage(int nOrgX, int nOrgY, int nInWidth, int nInHeight, BYTE* pIn, int nOutWidth, int nOutHeight, BYTE* pOut);
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
	afx_msg void OnDeltaposSpinPocx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPocy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkbtnSetpoc();
	afx_msg void OnBnClickedBtnSetRegister();
};
