
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


//�Ŵ��� ���� �ڵ� ���.
//0000h ��� �������� ���� ���ǵ��� ���� ��ɿ��� �������� �ʴ� 00000000h 
//0001h ũ�� �̻� ���� ����� ũ�Ⱑ ���� ũ��� �ٸ� 00000000h 
//0002h �� �ֹ߼� �޸� (�÷��� ���޸�) �̻� �� �ֹ߼� �޸� (�÷��� �޸�)�� �̻� ���� 00000000h 
//0010h �Ķ���� �̻� ���� ����� �Ű� ������ ���ǵ��� ���� �� �� �ϰ����� 00000000h 
//0011h ��� �̻� ���� ���� ������ �����Ǵ� 00000000h 
//0012h ��� ���� �̻� ��� �������� ��� ���࿡ ���� 00000000h 
//0013h �ּ� �̻� �ý��� �����͸� �б� / ���� �� �ּҰ� ������ ���� 00000000h ��ũ �׷� �����͸� �б� / ���� �ߪ��� �Ű� ���� ��ȣ�� ������ ���� 00000000h ����� ������ �б� / ���� �Ű� ���� ��ȣ�� ������ ���� 00000000h 
//0016h �̹��� ������ ���� ������ �̹��� �����Ͱ� �������� �ʴ� 00000000h 
//0017h �̹��� ĸó �̻� ������ ī�޶��� ĸó ���� �̻� ī�޶� ��ȣ 1 ~ 4bit : ī�޶� 1 ~ 4�� ���� 0019h ��� ������ ���� ������ ��� �����Ͱ� �������� �ʴ� 00000000h 
//001Ah �� �ֹ߼� �޸� (�÷��� ���޸�) �뷮 ���� �� �ֹ߼� �޸� (�÷��� �޸�)�� �뷮 �� ������ 00000000h 
//0020h ��� ���� ��� ��� DI �Է¿� ���� ó�� �Ǵ� PLC ����� ���� ���̱� ������ ������ �� �����ϴ� 00000000h 
//0023h ��ũ �޸� �뷮 ���� ��ũ �޸� (RAM)�� �뷮�� ������ 00000000h 0024h �÷ο� ���� ��� ���� ������ �帧 ���� ����� �������� �ʴ� 00000000h
//0025h ��� �α� ���� ������ ��� �αװ� �������� �ʴ� 00000000h 
//0026h ���� ���α׷� �帧 ���� ������ ���� ���α׷� ������ �帧�� �������� �ʴ� 00000000h 
//0030h �ý��� ������ �̻� �ý��� ������ �� �� ���� ������ ��� �Ű� �������ִ�
//////////////////////////////////////////////////////////////////////////
CString CBanrigan::GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG)
{
	CString errInfo = L"", errMsg = L"";
	errInfo.Format(L"msg-size : 0x%X, main-code : 0x%X, msg-id : 0x%X, err-code : 0x%X.", Cmd.MsgSize, Cmd.MainCode, Cmd.MsgID, CmdNG.ErrCode);
	switch (CmdNG.ErrCode)
	{
	case 0  : errMsg.Format(L"Message : ��ɾ� �� ����."); break;
	case 1  : errMsg.Format(L"Message : ���� ����� ũ�Ⱑ ���� ũ��� �ٸ��ϴ�."); break;
	case 2  : errMsg.Format(L"Message : �÷��� �޸� �̻� ����."); break;
	case 16 : errMsg.Format(L"Message : �Ķ���� �̻� ����."); break;
	case 17 : errMsg.Format(L"Message : �̻� ����."); break;
	case 18 : errMsg.Format(L"Message : ��� ���� ����."); break;
	case 19 : errMsg.Format(L"Message : �б� / ���� �ּ� ���� ���."); break;
	case 22 : errMsg.Format(L"Message : �̹��� ������ ����."); break;
	case 23 : errMsg.Format(L"Message : ������ ī�޶��� ĸ�� ����."); break;
	case 25 : errMsg.Format(L"Message : ������ Register Data�� �������� ����."); break;
	case 26 : errMsg.Format(L"Message : �÷��� �޸� �뷮 ����."); break;
	case 32 : errMsg.Format(L"Message : DI �Է¿� ���� ó�� �Ǵ� PLC ����� ���� ���̱� ������ ���� �Ұ�."); break;
	case 35 : errMsg.Format(L"Message : ��ũ �޸� �뷮 ����."); break;
	case 37 : errMsg.Format(L"Message : ������ ��� �αװ� �������� ����."); break;
	case 38 : errMsg.Format(L"Message : ������ Application Data Flow�� �������� ����."); break;
	case 48 : errMsg.Format(L"Message : �ý��� �����͸� �� �� ������ ���."); break;
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

	// 0001h ǥ�� ���� 
	// 0002h ���� ȭ�� (���� ���α׷� �帧 ���� ����) 
	// 0011h ���� ȭ�� : 1 ȭ�� ǥ�� (disp1 ǥ�� �̹����� ����) 
	// 0012h ���� ȭ�� : 2 �̹��� ǥ�� (disp1 / disp2 ǥ�� �̹��� ����) 
	// 0013h ���� ȭ�� : 4 �̹��� ǥ�� (disp1 / disp2 / disp3 / disp4 ǥ�� �̹��� ����) 
	// 0021h ���� ȭ�� : 1 ȭ�� ǥ�� (disp1 ǥ�� �̹����� ����) 
	// 0022h ���� ȭ�� : 2 �̹��� ǥ�� (disp1 / disp2 ǥ�� �̹��� ����) 
	// 0023h ���� ȭ�� : 4 �̹��� ǥ�� (disp1 / disp2 / disp3 / disp4 ǥ�� �̹��� ����)

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

	// 0001h ~ 0004h ���̺� �̹��� (ī�޶� 1 ~ 4)
	// 0011h ~ 0014h ���� �̹��� (ī�޶� 1 ~ 4)
	// 0021h ~ 0024h ���� �̹��� (ī�޶� 1 ~ 4)
	// 0031h ~ 0050h ����� �̹��� (1 ~ 32)

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
	// 0001h ~ 0004h ���̺� �̹��� (ī�޶� 1 ~ 4)
	// 0011h ~ 0014h ���� �̹��� (ī�޶� 1 ~ 4)
	// 0021h ~ 0024h ���� �̹��� (ī�޶� 1 ~ 4)
	// 0031h ~ 0050h ����� �̹��� (1 ~ 32)

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

	//128x128 ����.
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
	SndCmd.Version  = 0x0000;	// 0 ���� ����.
	SndCmd.System   = 0x0000;	// 0 ���� ����.
	SndCmd.Reg		= 0x0001;	// 0 : ��� ����, 1 : ��� ����.
	SndCmd.Match	= 0x0001;	// 1 : POC, 2 : ����ȭ.
	SndCmd.Reserve1	= 0x0000;	// ����.
	SndCmd.RegPosX	= nRegPosX; // -640 ~ +636
	SndCmd.RegPosY  = nRegPosY;	// -511 ~ +512
	SndCmd.RegSizeW = nWidth;	// 128 ����.
	SndCmd.RegSizeH = nHeight;	// 128 ����.
	SndCmd.POCSizeW = 4;		// 4 ����.
	SndCmd.POCSizeH = 4;		// 4 ����.
	SndCmd.RefPosX	= 0;		//nRegPosX + 64; // ��� �̹��� �߽� ����.
	SndCmd.RefPosY  = 0;		//nRegPosY + 64; // ��� �̹��� �߽� ����.
	SndCmd.Mask		= 0x0000;	// 0 ���� ����.
	SndCmd.MaskPosX	= 0x0000;	// 0 ���� ����.
	SndCmd.MaskPosY	= 0x0000;	// 0 ���� ����.
	SndCmd.MaskSizeW= 0x0000;	// 0 ���� ����.
	SndCmd.MaskSizeH= 0x0000;	// 0 ���� ����.
	SndCmd.Reserve2 = 0x0000;	// 0 ���� ����.

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

	// ���� ���� �� msg-size = 0x18h + Flow ���� ��� ũ��
	// ����, ���� ������� 16�� �⺻�̹Ƿ� ������ ������ ��� ������ �Ʒ��� ���� ����Ѵ�.
	int size = (RcvCmd.MsgSize-0x18)/4 - 16;
	nResultSize = size;
	int data[16]={0,};
	//Receive((void *)data, sizeof(int)*(16));
	Receive((void *)pResult, sizeof(float)*(size+16));

	return true;
}

//2016-10-14 ggkim
// long -> int ����.
//bool CBanrigan::SetUserData(const int nUserNo, long lData)
bool CBanrigan::SetUserData(const int nUserNo, int nData)
{
	StSndSetUserData SndCmd;
	StCommonCmd		 RcvCmd;
	ZeroMemory((void *)&SndCmd,sizeof(SndCmd));
	ZeroMemory((void *)&RcvCmd,sizeof(RcvCmd));

	//2016-10-14 ggkim
	//SndCmd.Cmd.MsgSize	= 0x0018 + 1*sizeof(long); ������ ����.
	SndCmd.Cmd.MsgSize	= 0x0018 + 1*sizeof(long)*5; // msg-size = 0x0018 + [Data Count] * [data size]
	SndCmd.Cmd.MainCode = CMD_SET_USR;
	SndCmd.Cmd.MsgID	= 0x0000;
	SndCmd.SubCode		= 0x0000;
	SndCmd.Target		= 0x0101; 
	//2016-10-14 ggkim
	//0x103 -> 0x101 ����.
	// 0x0101 : User Data [Integer] : ��ü ������
	// 0x0102 : User Data [Integer] : ���� ����
	// 0x0103 : User Data [Integer] : ���� ����
	// 0x0201 : User Data [Real]	: ��ü ������
	// 0x0202 : User Data [Real]	: ���� ����
	// 0x0203 : User Data [Real]	: ���� ����
	SndCmd.Address		= nUserNo;
	SndCmd.Num			= 1; // Data Count

	Send((void *)&SndCmd,sizeof(SndCmd));

	//2016-10-14 ggkim
	//���� �����Ϳ��� ��ü ������ �������� ����.
	//long -> int ����.
	//Send((void *)&lData,sizeof(long));
	//���簪 Parameter ����[4B] /  ���簪[4B] / �ʱⰪ Parameter ����[4B] / �ʱⰪ[4B] / �ʱ�ȭ Ÿ�̹�[4B]
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