//----------------------------------------------------------
// Banrigan ii Class
//----------------------------------------------------------
// Programmed by William Kim
//----------------------------------------------------------
// Last Update : 2017-01-18 16:07
// Modified by William Kim
//----------------------------------------------------------

#pragma once

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
	/// \param [in] strIP		Banrigan IP Address
	/// \param [in] nPort		Banrigan Port Number
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
	/// \param [in] nType		0x0001 : System Information
	///							0x0010 : Current Work Group Number
	///							0x0100 : All User Variable (Integer + Real) 
	///							0x0101 : User Variable [Integer]
	///							0x0102 : User Variable [Real]
	///							0x0200 : All Register Data
	///							0x0201 ~ 0x0390 : Register Data No.1~400
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param bool				��� ��ȯ.
	bool OnSaveData(const int nType, const int nRegNo=0);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� ����� ȭ���� ����.
	/// \param [in] nMode		0x0001 : NONE
	///							0x0011 : Run Mode 1 Display 
	///							0x0012 : Run Mode 2 Display
	///							0x0013 : Run Mode 4 Display
	///							0x0021 : Setting Mode 1 Display
	///							0x0022 : Setting Mode 2 Display
	///							0x0023 : Setting Mode 4 Display
	/// \param [in] nDisp1~4	0x0000 : None
	///							0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0011 ~ 0x0014 : Stop Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Imgae	(Camera 1~4)
	///							0x0031 ~ 0x0050 : User Image		(No.1~32)
	/// \param bool				��� ��ȯ.
	bool SetMonitor(const int nMode, const int nDisp1, const int nDisp2, const int nDisp3, const int nDisp4);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� �̹��� ȹ��.
	/// \param [in]  nImage		0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0011 ~ 0x0014 : Stop Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Image	(Camera 1~4)
	///							0x0031 ~ 0x0050 : User Image		(No.1~32)
	/// \param [out] pBuffer	��� �̹���.
	/// \param bool				��� ��ȯ.
	bool GetImage(const int nImage, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� �̹��� ����.
	/// \param [in]  nImage		0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0011 ~ 0x0014 : Stop Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Image	(Camera 1~4)
	///							0x0031 ~ 0x0050	: User Image		(No.1~32)
	/// \param [out] pBuffer	��� �̹���.
	/// \param bool				��� ��ȯ.
	bool SetImage(const int nImage, const int nWidth, const int nHeight, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ���.
	/// \param [in]  nRegNo		Register Number No.1~400
	/// \param [out] nRegOrgX	Origin Position X
	/// \param [out] nRegOrgY	Origin Position Y
	/// \param [out] nRegSizeW	Image Width Size
	/// \param [out] nRegSizeH	Image Height Size
	/// \param [out] nRefPosX	Reference Position X
	/// \param [out] nRefPosY	Reference Position Y
	/// \param bool				��� ��ȯ.
	bool GetRegisterData(const int nRegNo, long &nRegOrgX, long &nRegOrgY, int &nRegSizeW, int &nRegSizeH, float &nRefPosX, float &nRefPosY);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� �̹��� ���.
	/// \param [in]  nRegNo		Register Number No.1 ~ 400
	/// \param [out] pBuffer	�̹��� ������. �� ��µǴ� �̹��� ũ��� 128x128�� ����.
	/// \param bool				��� ��ȯ.
	bool GetRegisterImage(const int nRegNo, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ����.
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param [in] nWidth		Image Width Size
	/// \param [in] nHeight		Image Height Size
	/// \param [in] nRegPosX	Origin Position X
	/// \param [in] nRegPosY	Origin Position Y
	/// \param [in] pBuffer		Image Pointer
	/// \param bool				��� ��ȯ.
	bool SetRegisterData(const int nRegNo, const int nWidth, const int nHeight, const int nRegPosX, const int nRegPosY, const BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� ���� ������ ����.
	/// \param [in] nUserNo		User Data Number No.1~1024
	/// \param [in] nData		User Data Value
	/// \param bool				��� ��ȯ.
	bool SetUserData(const int nUserNo, int nData);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ ����.
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param bool				��� ��ȯ.
	bool OnDeleteRegisterData(const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN�� Ư�� �������� ������ �߰�.
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param [in] nImage		0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Image	(Camera 1~4)
	///							0x0031 ~ 0x0050 : User Image		(No.1~32)
	/// \param [in] nRegOrgX	Origin Position X
	/// \param [in] nRegOrgY	Origin Position Y
	/// \param [in] nRegWidth	Image Width Size
	/// \param [in] nRegHeight	Image Height Size
	/// \param [in] nRefPosX	Reference Position X
	/// \param [in] nRefPosY	Reference Position Y
	/// \param bool				��� ��ȯ.
	bool OnAddRegisterData(const int nRegNo, const int nImage, const int nRegOrgX, const int nRegOrgY, const int nRegSize, int nRefPosX, int nRefPosY);
	//******************************************************************************************************************
	/// \brief					 BANRIGAN�� Application Flow�� ó������ ����.
	/// \param [in]  nGroup		 0 : Current Work Group Number
	///							 Work Group Number No.1~16
	/// \param [in]  nFlow		 0 : Current Application Flow Number
	///							 Application Flow Number No.1~64
	/// \param [Out] pResult	 Result Data Pointer
	/// \param bool				 ��� ��ȯ.
	bool OnExecute(const int nGroup, const int nFlow, float* pResult);
	//******************************************************************************************************************
	/// \brief					 BANRIGAN�� ���� ������ ����.
	/// \param [in]  nType		 0:���� ����, 1:Flow ���� ���, 2:User����[���簪], 3:ȭ�� ������, 4:�α� ������, 5:�˶� ����, 6:����Ʈ���� ����, 7:�ϵ���� ����
	/// \param bool				 ��� ��ȯ.
	bool OnReset(const int nType);

private :
	CString GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG);
};
