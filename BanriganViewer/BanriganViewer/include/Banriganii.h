// BanriganAction.h : header file
#if _MSC_VER > 1000
#pragma once
#endif

#include "CmdStructure.h"
#include "afxsock.h"

class CBanrigan : public CSocket
{
// Attributes
public:

// Operations
public:
	CBanrigan();
	virtual ~CBanrigan();

private :
	bool m_bOpened;
	CString m_strLastError;

public:
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ����.
	/// \param [in] strIP		�ݸ��� IP Address.
	/// \param [in] nPort		�ݸ��� Port Number.
	/// \param bool				��� ��ȯ.
	bool OpenPort(CString strIP, int nPort);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ���� ����.
	/// \param bool				��� ��ȯ.
	bool ClosePort();						
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ���� Ȯ��.
	/// \param bool				��� ��ȯ.
	bool IsOpened() { return m_bOpened; }

	//******************************************************************************************************************
	/// \brief					BANRIGAN���� �߻��� ������ ���� �޽��� Ȯ��.
	/// \param CString			���������� �߻��� ���� �޽��� ��ȯ.
	CString GetLastErrorMsg() { return m_strLastError; }

	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ������ ����.
	/// \param [in] nType		0:�ý��� ����, 1:���� ��ũ �׷� ��ȣ, 2:��� ����� ���� (������ + �Ǽ���), 3:����� ���� [����], 4:����� ���� [�Ǽ�], 5:��ü �������� ������. 6:Ư�� �������� ������
	/// \param [in] nRegNo		�������� ��ȣ (1 ~ 400).
	/// \param bool				��� ��ȯ.
	bool OnSaveData(const int nType, const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ����� ȭ���� ����.
	/// \param [in] nImage		0:1 Display, 1:2 Display, 2:4 Display.
	/// \param [in] nMode		0:Run Mode, 1:Setting Mode.
	/// \param [in] nDisp		0:NONE, 1:CAM1, 2:CAM2, 3:CAM3, 4:CAM4, 5:MEASURE1, 6:MEASURE2, 7:MEASURE3, 8:MEASURE4.
	/// \param bool				��� ��ȯ.
	bool SetMonitor(const int nImage, const int nMode, const int nDisp1, const int nDisp2, const int nDisp3, const int nDisp4);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� �̹��� ȹ��.
	/// \param [in]  nImage		0:CAM1, 1:CAM2, 2:CAM3, 3:CAM4, 4:MEASURE1, 5:MEASURE2, 6:MEASURE3, 7:MEASURE4;POC.
	/// \param [out] pBuffer	��� �̹���.
	/// \param bool				��� ��ȯ.
	bool GetImage(const int nImage, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ���.
	/// \param [in]  nRegNo		�������� ��ȣ (1 ~ 400).
	/// \param [out] nRegPosX	�̹��� ���� ��ġ X.
	/// \param [out] nRegPosY	�̹��� ���� ��ġ Y.
	/// \param [out] nRefPosX	������ ��ǥ X.
	/// \param [out] nRefPosY	������ ��ǥ Y.
	/// \param [out] pBuffer	��� �̹���.
	/// \param bool				��� ��ȯ.
	bool GetRegisterData(const int nRegNo, long &nRegPosX, long &nRegPosY, float &nRefPosX, float &nRefPosY, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ����.
	/// \param [in] nRegNo		�������� ��ȣ (1 ~ 400).
	/// \param [in] nWidth		�������� �̹��� ���� ������.
	/// \param [in] nHeight		�������� �̹��� ���� ������.
	/// \param [in] nRegPosX	�̹��� ���� ��ġ X.
	/// \param [in] nRegPosY	�̹��� ���� ��ġ Y.
	/// \param [in] pBuffer		�������� �̹���.
	/// \param bool				��� ��ȯ.
	bool SetRegisterData(const int nRegNo, const int nWidth, const int nHeight, const int nRegPosX, const int nRegPosY, const BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ����.
	/// \param [in] nRegNo		�������� ��ȣ (1 ~ 400).
	/// \param bool				��� ��ȯ.
	bool OnDeleteRegisterData(const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ �߰�.
	/// \param [in] nRegNo		�������� ��ȣ (1 ~ 400).
	/// \param [in] nImage		1:CAM1, 2:CAM2, 3:CAM3, 4:CAM4.
	/// \param [in] nRegPosX	�̹��� ������ ���� ��ġ X ����(�⺻�� 0).
	/// \param [in] nRegPosY	�̹��� ������ ���� ��ġ Y ����(�⺻�� 0).
	/// \param [in] nRegWidth	�̹��� ������ ���� ������(�⺻�� 128).
	/// \param [in] nRegHeight	�̹��� ������ ���� ������(�⺻�� 128).
	/// \param bool				��� ��ȯ.
	bool OnAddRegisterData(const int nRegNo, const int nImage, const int nRegPosX=0, const int nRegPosY=0, const int nRegWidth=128, const int nRegHeight=128);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Application Flow�� ó������ ����.
	/// \param [in] nGroup		0:���� ��ũ �׷� ��ȣ, 1~16 ��ũ �׷� ��ȣ ����
	/// \param [in] nFlow		0:���� �帧 ��ȣ, 1~64 �帧 ��ȣ ����
	/// \param [Out] pResult	��� ���� ��ȯ�� ������.
	/// \param [in] nResultSize	��� ���� ��ȯ�� �������� ������(���� ��� �� 16�� + ���� ��� ��).
	/// \param bool				��� ��ȯ.
	bool OnExecute(const int nGroup, const int nFlow, float* pResult, int nResultSize);
	
private :
	CString GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG);
};
