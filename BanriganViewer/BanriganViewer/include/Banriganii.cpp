
#include "stdafx.h"
#include "Banriganii.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBanrigan::CBanrigan()
{
	m_bOpened = false;
	m_strLastError = L"";
}

CBanrigan::~CBanrigan()
{

}

#if 0
BEGIN_MESSAGE_MAP(CBanrigan, CSocket)
END_MESSAGE_MAP()
#endif	// 0

bool CBanrigan::OpenPort(CString strIP, int nPort)
{
	if (IsOpened() == true)
		ClosePort();

	if (Create() == FALSE)
	{
		m_strLastError = L"Failed to create socket.";
		return false;
	}

	BOOL ret = Connect(strIP,nPort);

	if (ret == TRUE)
	{
		m_bOpened = true;
	}
	else
	{
		m_bOpened = false;
		m_strLastError = L"Failed to open port.";
	}

	return ret;
}

bool CBanrigan::ClosePort()
{
	if (ShutDown() == FALSE)
	{
		m_strLastError = L"Failed to close port.";
		return false;
	}

	Close();

	m_bOpened = false;

	return true;
}


//매뉴얼 오류 코드 목록.
//0000h 명령 지원되지 않음 정의되지 않은 명령에서 지원하지 않는 00000000h 
//0001h 크기 이상 수신 명령의 크기가 정의 크기와 다른 00000000h 
//0002h 비 휘발성 메모리 (플래시 슈메모리) 이상 비 휘발성 메모리 (플래시 메모리)가 이상 상태 00000000h 
//0010h 파라미터 이상 수신 명령의 매개 변수에 정의되지 않은 것 및 일관성이 00000000h 
//0011h 모드 이상 동작 모드는 실행이 금지되는 00000000h 
//0012h 명령 실행 이상 어떠한 원인으로 명령 실행에 실패 00000000h 
//0013h 주소 이상 시스템 데이터를 읽기 / 쓰기 할 주소가 범위를 벗어 00000000h 워크 그룹 데이터를 읽기 / 쓰기 みする 매개 변수 번호가 범위를 벗어 00000000h 사용자 변수를 읽기 / 쓰기 매개 변수 번호가 범위를 벗어 00000000h 
//0016h 이미지 데이터 없음 지정한 이미지 데이터가 존재하지 않는 00000000h 
//0017h 이미지 캡처 이상 지정한 카메라의 캡처 실패 이상 카메라 번호 1 ~ 4bit : 카메라 1 ~ 4에 대응 0019h 등록 데이터 없음 지정한 등록 데이터가 존재하지 않는 00000000h 
//001Ah 비 휘발성 메모리 (플래시 슈메모리) 용량 부족 비 휘발성 메모리 (플래시 메모리)의 용량 이 부족한 00000000h 
//0020h 명령 실행 통신 명령 DI 입력에 의한 처리 또는 PLC 명령이 실행 중이기 때문에 실행할 수 없습니다 00000000h 
//0023h 워크 메모리 용량 부족 워크 메모리 (RAM)의 용량이 부족한 00000000h 0024h 플로우 실행 결과 없음 지정한 흐름 실행 결과가 존재하지 않는 00000000h
//0025h 결과 로그 없음 지정한 결과 로그가 존재하지 않는 00000000h 
//0026h 응용 프로그램 흐름 없음 지정한 응용 프로그램 데이터 흐름이 존재하지 않는 00000000h 
//0030h 시스템 데이터 이상 시스템 데이터 쓸 때 값이 범위를 벗어난 매개 변수가있는
//////////////////////////////////////////////////////////////////////////
CString CBanrigan::GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG)
{
	CString errInfo = L"", errMsg = L"";
	errInfo.Format(L"msg-size : 0x%X, main-code : 0x%X, msg-id : 0x%X, err-code : 0x%X.", Cmd.MsgSize, Cmd.MainCode, Cmd.MsgID, CmdNG.ErrCode);
	switch (CmdNG.ErrCode)
	{
	case 0  : errMsg.Format(L"Message : 명령어 미 지원."); break;
	case 1  : errMsg.Format(L"Message : 수신 명령의 크기가 정의 크기와 다릅니다."); break;
	case 2  : errMsg.Format(L"Message : 플래시 메모리 이상 상태."); break;
	case 16 : errMsg.Format(L"Message : 파라미터 이상 수신."); break;
	case 17 : errMsg.Format(L"Message : 이상 동작."); break;
	case 18 : errMsg.Format(L"Message : 명령 실행 실패."); break;
	case 19 : errMsg.Format(L"Message : 읽기 / 쓰기 주소 범위 벗어남."); break;
	case 22 : errMsg.Format(L"Message : 이미지 데이터 없음."); break;
	case 23 : errMsg.Format(L"Message : 지정한 카메라의 캡쳐 실패."); break;
	case 25 : errMsg.Format(L"Message : 지정한 Register Data가 존재하지 않음."); break;
	case 26 : errMsg.Format(L"Message : 플래시 메모리 용량 부족."); break;
	case 32 : errMsg.Format(L"Message : DI 입력에 의한 처리 또는 PLC 명령이 실행 중이기 때문에 실행 불가."); break;
	case 35 : errMsg.Format(L"Message : 워크 메모리 용량 부족."); break;
	case 37 : errMsg.Format(L"Message : 지정한 결과 로그가 존재하지 않음."); break;
	case 38 : errMsg.Format(L"Message : 지정한 Application Data Flow가 존재하지 않음."); break;
	case 48 : errMsg.Format(L"Message : 시스템 데이터를 쓸 때 범위를 벗어남."); break;
	}

	return errInfo+errMsg;
}

bool CBanrigan::OnSaveData(const int nType, const int nRegNo)
{
	StSndSaveData	SndCmd;
	StCommonCmd		RcvCmd;

	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_SAVE; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;

	switch (nType)
	{
	case 0 : SndCmd.Type = 0x0001; break;	// System Information
	case 1 : SndCmd.Type = 0x0010; break;	// Current Work Group
	case 2 : SndCmd.Type = 0x0100; break;	// All User Variable (integer + real)
	case 3 : SndCmd.Type = 0x0101; break;	// User Variable [integer]
	case 4 : SndCmd.Type = 0x0102; break;	// User Variable [real]
	case 5 : SndCmd.Type = 0x0200; break;	// All Register Data
	case 6 : SndCmd.Type = 0x0200 + nRegNo; break; // Selected Register Data (1~400)
	}

	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if (RcvCmd.MainCode != 0x8104)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvSaveData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::SetMonitor(const int nImage, const int nMode, const int nDisp1=0, const int nDisp2=0, const int nDisp3=0, const int nDisp4=0)
{
	StSndSetMonitor SndCmd;
	StCommonCmd		RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0020;   
	SndCmd.Cmd.MainCode = CMD_MONITOR; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;

	if (nMode == 0)			// Run Mode
		SndCmd.Type = 0x0011 + nImage;
	else if (nMode == 1)	// Setting Mode
		SndCmd.Type = 0x0021 + nImage;

	// 0001h 표시 없음 
	// 0002h 운전 화면 (응용 프로그램 흐름 정보 설정) 
	// 0011h 운전 화면 : 1 화상 표시 (disp1 표시 이미지를 선택) 
	// 0012h 운전 화면 : 2 이미지 표시 (disp1 / disp2 표시 이미지 선택) 
	// 0013h 운전 화면 : 4 이미지 표시 (disp1 / disp2 / disp3 / disp4 표시 이미지 선택) 
	// 0021h 설정 화면 : 1 화상 표시 (disp1 표시 이미지를 선택) 
	// 0022h 설정 화면 : 2 이미지 표시 (disp1 / disp2 표시 이미지 선택) 
	// 0023h 설정 화면 : 4 이미지 표시 (disp1 / disp2 / disp3 / disp4 표시 이미지 선택)

	SndCmd.Disp1 = nDisp1;
	SndCmd.Disp2 = nDisp2;
	SndCmd.Disp3 = nDisp3;
	SndCmd.Disp4 = nDisp4;

	if (nDisp1 > 4)
		SndCmd.Disp1 = 0x0016 + nDisp1;
	if (nDisp2 > 4)
		SndCmd.Disp2 = 0x0016 + nDisp2;
	if (nDisp3 > 4)
		SndCmd.Disp3 = 0x0016 + nDisp3;
	if (nDisp4 > 4)
		SndCmd.Disp4 = 0x0016 + nDisp4;

	// 0001h ~ 0004h 라이브 이미지 (카메라 1 ~ 4)
	// 0011h ~ 0014h 정지 이미지 (카메라 1 ~ 4)
	// 0021h ~ 0024h 측정 이미지 (카메라 1 ~ 4)
	// 0031h ~ 0050h 사용자 이미지 (1 ~ 32)

	int nSend = sizeof(SndCmd);
	int nRcv  = sizeof(RcvCmd);
	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8105)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvSetMonitor RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::GetImage(const int nImage, BYTE* pBuffer)
{
	StSndGetImage SndCmd;
	StCommonCmd	  RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_GET_IMG; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;

	SndCmd.Image = nImage+1;
	// 0001h ~ 0004h 라이브 이미지 (카메라 1 ~ 4)
	// 0011h ~ 0014h 정지 이미지 (카메라 1 ~ 4)
	// 0021h ~ 0024h 측정 이미지 (카메라 1 ~ 4)
	// 0031h ~ 0050h 사용자 이미지 (1 ~ 32)

	if (nImage > 3)
		SndCmd.Image = 0x001D + nImage;

	Send((void *)&SndCmd,sizeof(SndCmd));

	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8132)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvGetImage RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	int len = RcvOK.SizeW * RcvOK.SizeH;

	BYTE *buffer = new BYTE[len];
	memset(buffer,0,len);

	int sum=0;
	for(int i = 0; i < len;)
	{
		sum = Receive((void *)(buffer + i),len - i);
		i += sum;
	}

	memcpy(pBuffer, buffer, len);

	delete []buffer;
	buffer = NULL;

	return true;
}

bool CBanrigan::GetRegisterImage(const int nRegNo, BYTE* pBuffer)
{
	StSndGetRegData SndCmd;
	StCommonCmd		RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_GET_REG; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.RegNo		= nRegNo;

	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8135)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvGetRegData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	//128x128 고정.
	int len = 128*128;

	BYTE *buffer = new BYTE[len];
	memset(buffer,0,len);

	int sum=0;
	for(int i = 0; i < len;)
	{
		sum = Receive((void *)(buffer + i),len - i);
		i += sum;
	}

	memcpy(pBuffer, buffer, len);

	delete []buffer;
	buffer = NULL;

	return true;
}

bool CBanrigan::GetRegisterData(const int nRegNo, long &nRegOrgX, long &nRegOrgY, int &nRegSizeW, int &nRegSizeH, float &nRefPosX, float &nRefPosY)
{
	StSndGetRegData SndCmd;
	StCommonCmd		RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_GET_REG; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.RegNo		= nRegNo;

	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8135)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvGetRegData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	nRegOrgX = RcvOK.RegOrgX;
	nRegOrgY = RcvOK.RegOrgY;
	nRegSizeW= RcvOK.RegSizeW;
	nRegSizeH= RcvOK.RegSizeH;
	nRefPosX = RcvOK.RefPosX;
	nRefPosY = RcvOK.RefPosY;

	return true;
}

bool CBanrigan::SetRegisterData(const int nRegNo, const int nWidth, const int nHeight, const int nRegPosX, const int nRegPosY, const BYTE* pBuffer)
{
	StSndSetRegData SndCmd;
	StCommonCmd		RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0060 + (nWidth*nHeight);   
	SndCmd.Cmd.MainCode = CMD_SET_REG; 
	SndCmd.Cmd.MsgID	= 0x0000;

	SndCmd.SubCode	= 0x0000;
	SndCmd.RegNo	= nRegNo;
	SndCmd.Version  = 0x0000;	// 0 으로 설정.
	SndCmd.System   = 0x0000;	// 0 으로 설정.
	SndCmd.Reg		= 0x0001;	// 0 : 등록 없음, 1 : 등록 있음.
	SndCmd.Match	= 0x0001;	// 1 : POC, 2 : 정규화.
	SndCmd.Reserve1	= 0x0000;	// 예약.
	SndCmd.RegPosX	= nRegPosX; // -640 ~ +636
	SndCmd.RegPosY  = nRegPosY;	// -511 ~ +512
	SndCmd.RegSizeW = nWidth;	// 128 고정.
	SndCmd.RegSizeH = nHeight;	// 128 고정.
	SndCmd.POCSizeW = 4;		// 4 고정.
	SndCmd.POCSizeH = 4;		// 4 고정.
	SndCmd.RefPosX	= 0;		//nRegPosX + 64; // 등록 이미지 중심 기준.
	SndCmd.RefPosY  = 0;		//nRegPosY + 64; // 등록 이미지 중심 기준.
	SndCmd.Mask		= 0x0000;	// 0 으로 설정.
	SndCmd.MaskPosX	= 0x0000;	// 0 으로 설정.
	SndCmd.MaskPosY	= 0x0000;	// 0 으로 설정.
	SndCmd.MaskSizeW= 0x0000;	// 0 으로 설정.
	SndCmd.MaskSizeH= 0x0000;	// 0 으로 설정.
	SndCmd.Reserve2 = 0x0000;	// 0 으로 설정.

	Send((void *)&SndCmd,sizeof(SndCmd));

	int len = nWidth * nHeight;

	int nsize = len;
	int rS,nS;
	for(nS = 0; nS < len;)
	{
		rS = Send((void *)(pBuffer + nS),len - nS);
		nS += rS;
	}

	Receive((void *)&RcvCmd,sizeof(RcvCmd));
	if(RcvCmd.MainCode != 0x8136)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvSetRegData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::OnDeleteRegisterData(const int nRegNo)
{
	StSndDeleteRegData SndCmd;
	StCommonCmd		   RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_DEL_REG; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.RegNo		= nRegNo;

	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8138)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvDeleteRegData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::OnAddRegisterData(const int nRegNo, const int nImage, const int nRegOrgX, const int nRegOrgY, const int nRegSize, int nRefPosX, int nRefPosY)
{
	StSndAddRegData	SndCmd;
	StCommonCmd		RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	int img = nImage+1;
	if (nImage > 3)
		img = 0x001D + nImage;

	SndCmd.Cmd.MsgSize	= 0x0060;   
	SndCmd.Cmd.MainCode = CMD_ADD_REG; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.RegNo		= nRegNo;
	SndCmd.Version		= 0x0000;
	SndCmd.System		= 0x0000;
	SndCmd.Reserve1		= 0x0000;
	SndCmd.Match		= 0x0001;
	SndCmd.Image		= img;
	SndCmd.RegPosX		= nRegOrgX;
	SndCmd.RegPosY		= nRegOrgY;
	SndCmd.RegSizeW		= nRegSize;
	SndCmd.RegSizeH		= nRegSize;
	SndCmd.POCSizeW		= 0x0004;
	SndCmd.POCSizeH		= 0x0004;
	SndCmd.RefPosX		= nRefPosX;
	SndCmd.RefPosY		= nRefPosY;
	SndCmd.Mask			= 0x0000;
	SndCmd.MaskPosX		= 0x0000;
	SndCmd.MaskPosY		= 0x0000;
	SndCmd.MaskSizeW	= 0x0000;
	SndCmd.MaskSizeH	= 0x0000;
	SndCmd.Reserve2		= 0x0000;

	Send((void *)&SndCmd,sizeof(SndCmd));
	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8137)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvAddRegData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::OnExecute(const int nGroup, const int nFlow, float* pResult, int &nResultSize)
{
	StSndExecute SndCmd;
	StCommonCmd  RcvCmd;

	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0018;   
	SndCmd.Cmd.MainCode = CMD_EXECUTE; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.Group		= nGroup;
	SndCmd.Flow			= nFlow;

	Send((void *)&SndCmd,sizeof(SndCmd));

	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8140)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvExecute RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	// 응답 정상 시 msg-size = 0x18h + Flow 종합 결과 크기
	// 또한, 공통 결과수가 16개 기본이므로 유저가 설정한 결과 개수는 아래와 같이 계산한다.
	int size = (RcvCmd.MsgSize-0x18)/4 - 16;
	nResultSize = size;
	int data[16]={0,};
	//Receive((void *)data, sizeof(int)*(16));
	Receive((void *)pResult, sizeof(float)*(size+16));

	return true;
}

//2016-10-14 ggkim
// long -> int 변경.
//bool CBanrigan::SetUserData(const int nUserNo, long lData)
bool CBanrigan::SetUserData(const int nUserNo, int nData)
{
	StSndSetUserData SndCmd;
	StCommonCmd		 RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	//2016-10-14 ggkim
	//SndCmd.Cmd.MsgSize	= 0x0018 + 1*sizeof(long); 사이즈 변경.
	SndCmd.Cmd.MsgSize	= 0x0018 + 1*sizeof(long)*5; // msg-size = 0x0018 + [Data Count] * [data size]
	SndCmd.Cmd.MainCode = CMD_SET_USR;
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.Target		= 0x0101; 
	//2016-10-14 ggkim
	//0x103 -> 0x101 변경.
	// 0x0101 : User Data [Integer] : 전체 데이터
	// 0x0102 : User Data [Integer] : 현재 값만
	// 0x0103 : User Data [Integer] : 설정 값만
	// 0x0201 : User Data [Real]	: 전체 데이터
	// 0x0202 : User Data [Real]	: 현재 값만
	// 0x0203 : User Data [Real]	: 설정 값만
	SndCmd.Address		= nUserNo;
	SndCmd.Num			= 1; // Data Count

	Send((void *)&SndCmd,sizeof(SndCmd));

	//2016-10-14 ggkim
	//현재 데이터에서 전체 데이터 저장으로 변경.
	//long -> int 변경.
	//Send((void *)&lData,sizeof(long));
	//현재값 Parameter 정보[4B] /  현재값[4B] / 초기값 Parameter 정보[4B] / 초기값[4B] / 초기화 타이밍[4B]
	int wrData[5] = {0,};
	wrData[1] = nData;
	wrData[3] = nData;
	int size = sizeof(wrData);
	Send((void *)&wrData,sizeof(wrData));

	Receive((void *)&RcvCmd,sizeof(RcvCmd));
	if(RcvCmd.MainCode != 0x8131)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvSetUserData RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}

bool CBanrigan::OnReset(const int nType)
{
	StSndReset	SndCmd;
	StCommonCmd RcvCmd;

	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	SndCmd.Cmd.MsgSize	= 0x0010;   
	SndCmd.Cmd.MainCode = CMD_RESET; 
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	
	switch (nType)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		SndCmd.Type	= nType;
		break;
	case 5:
		SndCmd.Type	= 0x0100;
		break;
	case 6:
		SndCmd.Type	= 0x0200;
		break;
	case 7:
		SndCmd.Type	= 0x8000;
		break;
	}

	Send((void *)&SndCmd,sizeof(SndCmd));

	Receive((void *)&RcvCmd,sizeof(RcvCmd));

	if(RcvCmd.MainCode != 0x8102)
	{
		StCommonNG RcvNG;
		ZeroMemory((void *)&RcvNG,sizeof(RcvNG));
		Receive((void *)&RcvNG,sizeof(RcvNG));

		m_strLastError = GetErrorMessage(RcvCmd, RcvNG);
		return false;
	}

	StRcvReset RcvOK;
	ZeroMemory((void *)&RcvOK,sizeof(RcvOK));
	Receive((void *)&RcvOK,sizeof(RcvOK));

	return true;
}