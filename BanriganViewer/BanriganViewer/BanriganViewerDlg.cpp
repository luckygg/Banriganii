
// BanriganViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BanriganViewer.h"
#include "BanriganViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBanriganViewerDlg dialog

CBanriganViewerDlg::CBanriganViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBanriganViewerDlg::IDD, pParent)
	, m_nRbtnMonitor(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pImgBuf = NULL;
	m_pPocBuf = NULL;
	m_pImgBmpInfo = NULL;
	m_pPocBmpInfo = NULL;
}

void CBanriganViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RBTN_MONITOR1, m_nRbtnMonitor);
}

BEGIN_MESSAGE_MAP(CBanriganViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECTION, &CBanriganViewerDlg::OnBnClickedBtnConnection)
	ON_BN_CLICKED(IDC_BTN_GETIMAGE, &CBanriganViewerDlg::OnBnClickedBtnGetimage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LIVE, &CBanriganViewerDlg::OnBnClickedBtnLive)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MONITOR, &CBanriganViewerDlg::OnBnClickedBtnMonitor)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RBTN_MONITOR1, IDC_RBTN_MONITOR3, OnBnClickedRBtnMonitor)
	ON_BN_CLICKED(IDC_BTN_MEASURE, &CBanriganViewerDlg::OnBnClickedBtnMeasure)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POCX, &CBanriganViewerDlg::OnDeltaposSpinPocx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POCY, &CBanriganViewerDlg::OnDeltaposSpinPocy)
	ON_BN_CLICKED(IDC_CHKBTN_SETPOC, &CBanriganViewerDlg::OnBnClickedChkbtnSetpoc)
	ON_BN_CLICKED(IDC_BTN_SET_REGISTER, &CBanriganViewerDlg::OnBnClickedBtnSetRegister)
END_MESSAGE_MAP()


// CBanriganViewerDlg message handlers

BOOL CBanriganViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitControls();

	CreateImgBmpInfo(CAMERA_WIDTH,CAMERA_HEIGHT);
	CreatePocBmpInfo(POC_WIDTH,POC_HEIGHT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBanriganViewerDlg::InitControls()
{
	// ----- IP & Port ----- //
	CIPAddressCtrl *pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IP_BANRIGAN);
	pIP->SetAddress(192,168,0,1);
	SetDlgItemInt(IDC_EDIT_PORT, 10000);

	// ----- Get Image ----- //
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
	pCB->SetCurSel(0);

	// ----- Image Buffer ----- //
	m_pImgBuf = new BYTE[CAMERA_WIDTH * CAMERA_HEIGHT];
	memset(m_pImgBuf, 0, CAMERA_WIDTH * CAMERA_HEIGHT);

	m_pPocBuf = new BYTE[POC_WIDTH * POC_HEIGHT];
	memset(m_pPocBuf, 0, POC_WIDTH * POC_HEIGHT);

	// ----- Mode & Display ----- //
	pCB = (CComboBox*)GetDlgItem(IDC_CB_RUNMODE);
	pCB->SetCurSel(0);

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY1);
	pCB->SetCurSel(0);

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY2);
	pCB->SetCurSel(0);

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY3);
	pCB->SetCurSel(0);

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY4);
	pCB->SetCurSel(0);

	// ----- Measure Image ----- //
	pCB = (CComboBox*)GetDlgItem(IDC_CB_MEASURE);
	pCB->SetCurSel(0);

	// ----- POC X,Y Edit & Spin Control ----- //
	UDACCEL accels[] = {{-1,1}};
	CSpinButtonCtrl* pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_POCX);
	pSP->SetRange(-640,636);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_POCX,640/2-64);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_POCY);
	pSP->SetRange(-511,512);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_POCY,480/2-64);

	// ----- Picture Control ----- //
	CRect rect;
	GetDlgItem(IDC_PC_CAMERA)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_PC_CAMERA)->MoveWindow(rect.left,rect.top,640,480);
}

void CBanriganViewerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(100);

	if (m_Banrigan.IsOpened())
		m_Banrigan.ClosePort();

	if (m_pImgBuf != NULL)
	{
		delete []m_pImgBuf;
		m_pImgBuf = NULL;
	}

	if (m_pPocBuf != NULL)
	{
		delete []m_pPocBuf;
		m_pPocBuf = NULL;
	}

	if (m_pImgBmpInfo != NULL)
	{
		delete []m_pImgBmpInfo;
		m_pImgBmpInfo = NULL;
	}

	if (m_pPocBmpInfo != NULL)
	{
		delete []m_pPocBmpInfo;
		m_pPocBmpInfo = NULL;
	}

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBanriganViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBanriganViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBanriganViewerDlg::OnBnClickedBtnConnection()
{
	CString caption;
	GetDlgItemText(IDC_BTN_CONNECTION, caption);

	if (caption == L"Connect")
	{
		CIPAddressCtrl *pCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IP_BANRIGAN);
		BYTE IP[4] = {0,};
		int port = 0;
		pCtrl->GetAddress(IP[0],IP[1],IP[2],IP[3]);
		port = GetDlgItemInt(IDC_EDIT_PORT);

		CString address;
		address.Format(L"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]);
		
		if (m_Banrigan.OpenPort(address, port) == false)
		{
			AfxMessageBox(m_Banrigan.GetLastErrorMsg());
			return;
		}

		SetDlgItemText(IDC_BTN_CONNECTION, L"Disconnect");
	}
	else
	{
		if (m_Banrigan.ClosePort() == false)
		{
			AfxMessageBox(m_Banrigan.GetLastErrorMsg());
			return;
		}

		SetDlgItemText(IDC_BTN_CONNECTION, L"Connect");
	}
}

void CBanriganViewerDlg::OnBnClickedBtnGetimage()
{
	bool ret = false;

	if (m_Banrigan.IsOpened() == false) return;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
	int sel = pCB->GetCurSel();

	if (sel == 8) // 8 : POC2
	{
		long pocX=0,pocY=0;
		float refX=0,refY=0;
		ret = m_Banrigan.GetRegisterData(2,pocX,pocY,refX,refY,m_pPocBuf);
		if (ret == false)
		{
			AfxMessageBox(m_Banrigan.GetLastErrorMsg());
			return;
		}

		CDraw::DrawImage(GetDlgItem(IDC_PC_POC),m_pPocBuf,m_pPocBmpInfo,POC_WIDTH,POC_HEIGHT,false);
	}
	else
	{
		ret = m_Banrigan.GetImage(sel, m_pImgBuf);
		if (ret == false)
		{
			AfxMessageBox(m_Banrigan.GetLastErrorMsg());
			return;
		}

		int state = IsDlgButtonChecked(IDC_CHKBTN_CROSSLINE);
		CDraw::DrawImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,state);
	}
}
// Create Image Bitmap Info 
void CBanriganViewerDlg::CreateImgBmpInfo(int nWidth, int nHeight)
{
	if (m_pImgBmpInfo != NULL)
	{
		delete []m_pImgBmpInfo;
		m_pImgBmpInfo = NULL;
	}

	m_pImgBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];

	m_pImgBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pImgBmpInfo->bmiHeader.biPlanes = 1;
	m_pImgBmpInfo->bmiHeader.biBitCount = 8;
	m_pImgBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pImgBmpInfo->bmiHeader.biSizeImage = 0;
	m_pImgBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pImgBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pImgBmpInfo->bmiHeader.biClrUsed = 0;
	m_pImgBmpInfo->bmiHeader.biClrImportant = 0;

	for (int i = 0 ; i < 256 ; i++)
	{
		m_pImgBmpInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pImgBmpInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pImgBmpInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pImgBmpInfo->bmiColors[i].rgbReserved = 0;
	}

	m_pImgBmpInfo->bmiHeader.biWidth = nWidth;
	m_pImgBmpInfo->bmiHeader.biHeight = -nHeight;
}
// Create POC Image Bitmap Info 
void CBanriganViewerDlg::CreatePocBmpInfo(int nWidth, int nHeight)
{
	if (m_pPocBmpInfo != NULL)
	{
		delete []m_pPocBmpInfo;
		m_pPocBmpInfo = NULL;
	}

	m_pPocBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];

	m_pPocBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pPocBmpInfo->bmiHeader.biPlanes = 1;
	m_pPocBmpInfo->bmiHeader.biBitCount = 8;
	m_pPocBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pPocBmpInfo->bmiHeader.biSizeImage = 0;
	m_pPocBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pPocBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pPocBmpInfo->bmiHeader.biClrUsed = 0;
	m_pPocBmpInfo->bmiHeader.biClrImportant = 0;

	for (int i = 0 ; i < 256 ; i++)
	{
		m_pPocBmpInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pPocBmpInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pPocBmpInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pPocBmpInfo->bmiColors[i].rgbReserved = 0;
	}

	m_pPocBmpInfo->bmiHeader.biWidth = nWidth;
	m_pPocBmpInfo->bmiHeader.biHeight = -nHeight;
}

void CBanriganViewerDlg::OnBnClickedBtnLive()
{
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	CString caption;
	GetDlgItemText(IDC_BTN_LIVE, caption);

	if (caption == L"Live Start")
	{
		SetTimer(100,30,NULL);
		SetDlgItemText(IDC_BTN_LIVE, L"Live Stop");
	}
	else
	{
		KillTimer(100);
		SetDlgItemText(IDC_BTN_LIVE, L"Live Start");
	}
}

void CBanriganViewerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		if (m_Banrigan.IsOpened() == false) return;

		bool ret = false;
		
		CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
		int sel = pCB->GetCurSel();

		ret = m_Banrigan.GetImage(sel, m_pImgBuf);
		if (ret == false)
		{
			KillTimer(100);
			AfxMessageBox(m_Banrigan.GetLastErrorMsg());
			SetDlgItemText(IDC_BTN_LIVE, L"Live Start");
		}

		int state = IsDlgButtonChecked(IDC_CHKBTN_CROSSLINE);
		CDraw::DrawImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,state);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBanriganViewerDlg::OnBnClickedBtnMonitor()
{
	UpdateData(TRUE);

	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_RUNMODE);
	int mode = pCB->GetCurSel();
	
	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY1);
	int dsp1 = pCB->GetCurSel();

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY2);
	int dsp2 = pCB->GetCurSel();

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY3);
	int dsp3 = pCB->GetCurSel();

	pCB = (CComboBox*)GetDlgItem(IDC_CB_DISPLAY4);
	int dsp4 = pCB->GetCurSel();

	if (m_Banrigan.SetMonitor(m_nRbtnMonitor,mode,dsp1, dsp2, dsp3, dsp4) == false)
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
}

void CBanriganViewerDlg::OnBnClickedRBtnMonitor(UINT ID)
{
	UpdateData(TRUE);

	switch (m_nRbtnMonitor)
	{
	case 0:
		GetDlgItem(IDC_CB_DISPLAY1)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CB_DISPLAY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CB_DISPLAY4)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_CB_DISPLAY1)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY2)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CB_DISPLAY4)->EnableWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_CB_DISPLAY1)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY2)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY3)->EnableWindow(TRUE );
		GetDlgItem(IDC_CB_DISPLAY4)->EnableWindow(TRUE );
		break;
	}
}
// Message
BOOL CBanriganViewerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		WPARAM wParam = 0;
		wParam = pMsg->wParam;

#if defined(_DEBUG)
#if defined(_WIN64)
		wParam = wParam >> 32;
#endif
#endif
		if (wParam == VK_RETURN)
			return TRUE;

		if (wParam == VK_ESCAPE)
			return TRUE;
	}
		
	return CDialogEx::PreTranslateMessage(pMsg);
}
// Spin Control
void CBanriganViewerDlg::OnDeltaposSpinPocx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData();

	int pocX = GetDlgItemInt(IDC_EDIT_POCX);
	int pocY = GetDlgItemInt(IDC_EDIT_POCY);
	CDraw::DrawImageWithROI(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,640,480,pocX, pocY);
	*pResult = 0;
}
// Spin Control
void CBanriganViewerDlg::OnDeltaposSpinPocy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData();

	int pocX = GetDlgItemInt(IDC_EDIT_POCX);
	int pocY = GetDlgItemInt(IDC_EDIT_POCY);
	CDraw::DrawImageWithROI(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,640,480,pocX, pocY);

	*pResult = 0;
}

void CBanriganViewerDlg::OnBnClickedChkbtnSetpoc()
{
	int state = IsDlgButtonChecked(IDC_CHKBTN_SETPOC);

	GetDlgItem(IDC_EDIT_POCX)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_POCX)->EnableWindow(state);
	GetDlgItem(IDC_EDIT_POCY)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_POCY)->EnableWindow(state);
}

void CBanriganViewerDlg::OnBnClickedBtnMeasure()
{
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	float result[20] = {0,};
	if (m_Banrigan.OnExecute(1,1,result,20) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		SetDlgItemText(IDC_LB_POSX,L"");
		SetDlgItemText(IDC_LB_POSY,L"");
		SetDlgItemText(IDC_LB_THETA,L"");
		SetDlgItemText(IDC_LB_SCORE,L"");

		return;
	}

	// 획득한 이미지 취득.
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_MEASURE);
	int sel = pCB->GetCurSel();

	memset(m_pImgBuf,0,CAMERA_WIDTH*CAMERA_HEIGHT);
	if (m_Banrigan.GetImage(sel+4,m_pImgBuf) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	int state = IsDlgButtonChecked(IDC_CHKBTN_CROSSLINE);
	CDraw::DrawMeasureImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,result[16],result[17],state);

	CString value=L"";
	value.Format(L"%.2f", result[16]);
	SetDlgItemText(IDC_LB_POSX,value);
	value.Format(L"%.2f", result[17]);
	SetDlgItemText(IDC_LB_POSY,value);
	value.Format(L"%.0f", result[18]);
	SetDlgItemText(IDC_LB_THETA,value);
	value.Format(L"%.2f", result[19]);
	SetDlgItemText(IDC_LB_SCORE,value);
}

// POC 2->1 등록 과정.
// POC 2->1 은 Register 1 -> Register 2 순서로 POC를 실행한다.
void CBanriganViewerDlg::OnBnClickedBtnSetRegister()
{
	bool ret = false;
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	// ---------- Register 1 삭제 & 등록 & 작성 ---------- //
	// 1. 기존에 등록되어있던 Register 1을 삭제한다.
	if (m_Banrigan.OnDeleteRegisterData(1) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	// 2. Register 1번을 새로 추가한다. (POC 2->1 에서 '2'에 해당.)
	//    Camera 1번 이미지를 선택하고, 화면 중심 기준으로 offset (-240,240) , size (480,480) 로 설정한다.
	if (m_Banrigan.OnAddRegisterData(1,1,-240,240,480,480) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	// ---------- Register 2 삭제 & 등록 & 작성 ---------- //
	// 3. 기존에 등록되어있던 Register 2를 삭제한다.
	if (m_Banrigan.OnDeleteRegisterData(2) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	// 4. Register 2번을 새로 추가한다. (POC 2->1 에서 '1'에 해당.)
	if (m_Banrigan.OnAddRegisterData(2,1) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	int orgX = GetDlgItemInt(IDC_EDIT_POCX);
	int orgY = GetDlgItemInt(IDC_EDIT_POCY);
	
	memset(m_pPocBuf, 0, POC_WIDTH*POC_HEIGHT);

	// 5. 마지막으로 취득한 이미지 (m_pImgBuf)에서 128x128 만큼 레퍼런스 이미지를 추출한다.
	GetROIImage(orgX, orgY, CAMERA_WIDTH, CAMERA_HEIGHT, m_pImgBuf, POC_WIDTH, POC_HEIGHT, m_pPocBuf);

	CDraw::DrawImage(GetDlgItem(IDC_PC_POC),m_pPocBuf,m_pPocBmpInfo,POC_WIDTH,POC_HEIGHT,false);

	// 6. 추출한 레퍼런스 이미지를 Register 2번에 설정한다.
	if (m_Banrigan.SetRegisterData(2,POC_WIDTH,POC_HEIGHT,POC_WIDTH/2,POC_HEIGHT/2,m_pPocBuf) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	// 7. 모든 Register 데이터를 반리간에 저장한다.
	if (m_Banrigan.OnSaveData(5,1) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}
}

void CBanriganViewerDlg::GetROIImage(int nOrgX, int nOrgY, int nInWidth, int nInHeight, BYTE* pIn, int nOutWidth, int nOutHeight, BYTE* pOut)
{
	for (int j=0; j<nInHeight; j++)
	{
		for (int i=0; i<nInWidth; i++)
		{
			if (j >= nOrgY && j < nOrgY + nOutHeight)
				memcpy(pOut+(nOutWidth*(j-nOrgY)), pIn + (nInWidth*j) + nOrgX, nOutWidth);
		}
	}
}