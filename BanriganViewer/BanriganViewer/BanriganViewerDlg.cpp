
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
	m_pRegBuf = NULL;
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
	ON_BN_CLICKED(IDC_CHK_SETREG, &CBanriganViewerDlg::OnBnClickedChkbtnSetpoc)
	ON_BN_CLICKED(IDC_BTN_WRITEREGISTER, &CBanriganViewerDlg::OnBnClickedBtnWriteRegister)
	ON_BN_CLICKED(IDC_BTN_GETREGIMG, &CBanriganViewerDlg::OnBnClickedBtnGetregimg)
	ON_EN_UPDATE(IDC_EDIT_REGORGX, &CBanriganViewerDlg::OnUpdateRegister)
	ON_EN_UPDATE(IDC_EDIT_REGORGY, &CBanriganViewerDlg::OnUpdateRegister)
	ON_EN_UPDATE(IDC_EDIT_REGSIZE, &CBanriganViewerDlg::OnUpdateRegister)
	ON_EN_UPDATE(IDC_EDIT_REFPOSX, &CBanriganViewerDlg::OnUpdateRegister)
	ON_EN_UPDATE(IDC_EDIT_REFPOSY, &CBanriganViewerDlg::OnUpdateRegister)
	ON_BN_CLICKED(IDC_BTN_TEST, &CBanriganViewerDlg::OnBnClickedBtnTest)
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
	CreatePocBmpInfo(REG_WIDTH,REG_HEIGHT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBanriganViewerDlg::InitControls()
{
	// ----- IP Address Control ----- //
	CIPAddressCtrl *pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IP_BANRIGAN);
	pIP->SetAddress(192,168,0,1);
	SetDlgItemInt(IDC_EDIT_PORT, 10000);

	// ----- Image Buffer ----- //
	m_pImgBuf = new BYTE[CAMERA_WIDTH * CAMERA_HEIGHT];
	memset(m_pImgBuf, 0, CAMERA_WIDTH * CAMERA_HEIGHT);

	m_pRegBuf = new BYTE[REG_WIDTH * REG_HEIGHT];
	memset(m_pRegBuf, 0, REG_WIDTH * REG_HEIGHT);

	// ----- ComboBox Control ----- //
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
	pCB->SetCurSel(0);

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

	pCB = (CComboBox*)GetDlgItem(IDC_CB_MEASURE);
	pCB->SetCurSel(0);

	// ----- Edit & Spin Control ----- //
	UDACCEL accels[] = {{-1,1}};
	CSpinButtonCtrl* pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REGORGX);
	pSP->SetRange(-320,320);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_REGORGX,-240);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REGORGY);
	pSP->SetRange(-240,240);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_REGORGY,240);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REFPOSX);
	pSP->SetRange(-320,320);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_REFPOSX,0);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REFPOSY);
	pSP->SetRange(-240,240);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_REFPOSY,0);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_GROUP);
	pSP->SetRange(1,16);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_GROUP,1);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FLOW);
	pSP->SetRange(1,64);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_FLOW,1);

	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REGNUM);
	pSP->SetRange(1,400);
	pSP->SetAccel(1,accels);
	SetDlgItemInt(IDC_EDIT_REGNUM, 1);

	UDACCEL accSize[] = {{-4,4}};
	pSP = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REGSIZE);
	pSP->SetRange(4,480);
	pSP->SetAccel(4,accSize);
	SetDlgItemInt(IDC_EDIT_REGSIZE,480);

	// ----- Picture Control ----- //
	CRect rect;
	GetDlgItem(IDC_PC_CAMERA)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_PC_CAMERA)->MoveWindow(rect.left,rect.top,CAMERA_WIDTH,CAMERA_HEIGHT);

	GetDlgItem(IDC_PC_POC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_PC_POC)->MoveWindow(rect.left,rect.top,REG_WIDTH,REG_HEIGHT);
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

	if (m_pRegBuf != NULL)
	{
		delete []m_pRegBuf;
		m_pRegBuf = NULL;
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

	if (m_Banrigan.IsOpened() == false) 
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
		return;
	}

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
	int sel = pCB->GetCurSel();

	ret = m_Banrigan.GetImage(sel, m_pImgBuf);
	if (ret == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	int state = IsDlgButtonChecked(IDC_CHK_CROSSLINE);
	CDraw::DrawImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,state);
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
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
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

		int state = IsDlgButtonChecked(IDC_CHK_CROSSLINE);
		CDraw::DrawImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,state);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBanriganViewerDlg::OnBnClickedBtnMonitor()
{
	UpdateData(TRUE);

	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
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

void CBanriganViewerDlg::OnBnClickedChkbtnSetpoc()
{
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
		return;
	}

	int state = IsDlgButtonChecked(IDC_CHK_SETREG);

	GetDlgItem(IDC_EDIT_REGORGX)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_REGORGX)->EnableWindow(state);
	GetDlgItem(IDC_EDIT_REGORGY)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_REGORGY)->EnableWindow(state);
	GetDlgItem(IDC_EDIT_REGSIZE)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_REGSIZE)->EnableWindow(state);

	GetDlgItem(IDC_EDIT_REFPOSX)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_REFPOSX)->EnableWindow(state);
	GetDlgItem(IDC_EDIT_REFPOSY)->EnableWindow(state);
	GetDlgItem(IDC_SPIN_REFPOSY)->EnableWindow(state);

	GetDlgItem(IDC_BTN_WRITEREGISTER)->EnableWindow(state);
	OnUpdateRegister();
}

void CBanriganViewerDlg::OnBnClickedBtnMeasure()
{
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
		return;
	}

	SetDlgItemText(IDC_LB_POSX1,L"");
	SetDlgItemText(IDC_LB_POSY1,L"");
	SetDlgItemText(IDC_LB_THETA1,L"");
	SetDlgItemText(IDC_LB_SCORE1,L"");
	SetDlgItemText(IDC_LB_TIME1,L"");
	SetDlgItemText(IDC_LB_POSX2,L"");
	SetDlgItemText(IDC_LB_POSY2,L"");
	SetDlgItemText(IDC_LB_THETA2,L"");
	SetDlgItemText(IDC_LB_SCORE2,L"");
	SetDlgItemText(IDC_LB_TIME2,L"");

	// 최대 64개의 데이터를 취득한다고 가정.
	float result[64] = {0,};
	int resultSize=0;
	int group = GetDlgItemInt(IDC_EDIT_GROUP);
	int flow  = GetDlgItemInt(IDC_EDIT_FLOW);

	if (m_Banrigan.OnExecute(group,flow,result,resultSize) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
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

	bool ret=false;
	int state = IsDlgButtonChecked(IDC_CHK_CROSSLINE);

	//POC2단계 사용 시 출력되는 데이터가 10개이므로 resultSize가 5보다 크다는 것은 POC2단계라고 판단.
	if (resultSize > 5)
		CDraw::DrawMeasureImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,result[21],result[22],state); // POC 2단계 결과의 중심 좌표.
	else
		CDraw::DrawMeasureImage(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,result[16],result[17],state); // POC 1단계 결과의 중심 좌표.

	CString value=L"";
	
	value.Format(L"%.3f", result[16]);
	SetDlgItemText(IDC_LB_POSX1,value);
	value.Format(L"%.3f", result[17]);
	SetDlgItemText(IDC_LB_POSY1,value);
	value.Format(L"%.3f", result[18]);
	SetDlgItemText(IDC_LB_THETA1,value);
	value.Format(L"%.0f", result[19]);
	SetDlgItemText(IDC_LB_SCORE1,value);
	value.Format(L"%.0f", result[20]);
	SetDlgItemText(IDC_LB_TIME1,value);

	if (resultSize > 5)
	{
		value.Format(L"%.3f", result[21]);
		SetDlgItemText(IDC_LB_POSX2,value);
		value.Format(L"%.3f", result[22]);
		SetDlgItemText(IDC_LB_POSY2,value);
		value.Format(L"%.3f", result[23]);
		SetDlgItemText(IDC_LB_THETA2,value);
		value.Format(L"%.0f", result[24]);
		SetDlgItemText(IDC_LB_SCORE2,value);
		value.Format(L"%.0f", result[25]);
		SetDlgItemText(IDC_LB_TIME2,value);
	}
}

void CBanriganViewerDlg::OnBnClickedBtnWriteRegister()
{
	bool ret = false;
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
		return;
	}

	int regNum = GetDlgItemInt(IDC_EDIT_REGNUM);
	// 레지스터 데이터를 수정하는 경우, 기존 데이터는 반드시 지우고 새로 생성하여 작성해야 함.
	if (m_Banrigan.OnDeleteRegisterData(regNum) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	int regOrgX = GetDlgItemInt(IDC_EDIT_REGORGX);
	int regOrgY = GetDlgItemInt(IDC_EDIT_REGORGY);
	int regSize = GetDlgItemInt(IDC_EDIT_REGSIZE);
	int refPosX = GetDlgItemInt(IDC_EDIT_REFPOSX);
	int refPosY = GetDlgItemInt(IDC_EDIT_REFPOSY);
	
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CB_IMAGE);
	int image	= pCB->GetCurSel();
	if (m_Banrigan.OnAddRegisterData(regNum,image,regOrgX,regOrgY,regSize,refPosX,refPosY) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	if (m_Banrigan.OnSaveData(6,regNum) == false)
	{
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}
}

void CBanriganViewerDlg::OnBnClickedBtnGetregimg()
{
	if (m_Banrigan.IsOpened() == false)
	{
		AfxMessageBox(L"반리간이 연결되어 있지 않습니다.");
		return;
	}

	int num = GetDlgItemInt(IDC_EDIT_REGNUM);

	memset(m_pRegBuf, 0, REG_WIDTH*REG_HEIGHT);
	if (m_Banrigan.GetRegisterImage(num, m_pRegBuf) == false)
	{
		CDraw::DrawImage(GetDlgItem(IDC_PC_REGISTER),m_pRegBuf,m_pPocBmpInfo,REG_WIDTH,REG_HEIGHT,false);
		AfxMessageBox(m_Banrigan.GetLastErrorMsg());
		return;
	}

	CDraw::DrawImage(GetDlgItem(IDC_PC_REGISTER),m_pRegBuf,m_pPocBmpInfo,REG_WIDTH,REG_HEIGHT,false);
}

void CBanriganViewerDlg::OnUpdateRegister()
{
	int regOrgX = GetDlgItemInt(IDC_EDIT_REGORGX);
	int regOrgY = GetDlgItemInt(IDC_EDIT_REGORGY);
	int regSize = GetDlgItemInt(IDC_EDIT_REGSIZE);
	int refPosX = GetDlgItemInt(IDC_EDIT_REFPOSX);
	int refPosY = GetDlgItemInt(IDC_EDIT_REFPOSY);
	regOrgX += 320;
	regOrgY = 240 - regOrgY;
	refPosX += 320;
	refPosY = 240 - refPosY;
	
	int state = IsDlgButtonChecked(IDC_CHK_CROSSLINE);
	CDraw::DrawImageWithROI(GetDlgItem(IDC_PC_CAMERA),m_pImgBuf,m_pImgBmpInfo,CAMERA_WIDTH,CAMERA_HEIGHT,regOrgX,regOrgY,regSize,refPosX,refPosY,state);
}

void CBanriganViewerDlg::OnBnClickedBtnTest()
{
	m_Banrigan.SetUserData(1,0);
	m_Banrigan.SetUserData(2,-10);
	m_Banrigan.SetUserData(3,10);
}
