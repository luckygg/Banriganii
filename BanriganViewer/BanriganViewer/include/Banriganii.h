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
	/// \brief					BANRIGAN에 연결.
	/// \param [in] strIP		Banrigan IP Address
	/// \param [in] nPort		Banrigan Port Number
	/// \param bool				결과 반환.
	bool OpenPort(CString strIP, int nPort);
	//******************************************************************************************************************
	/// \brief					BANRIGAN에 연결 해제.
	/// \param bool				결과 반환.
	bool ClosePort();						
	//******************************************************************************************************************
	/// \brief					BANRIGAN에 연결 확인.
	/// \param bool				결과 반환.
	bool IsOpened() { return m_bOpened; }
	//******************************************************************************************************************
	/// \brief					BANRIGAN에서 발생한 마지막 에러 메시지 확인.
	/// \param CString			마지막으로 발생한 에러 메시지 반환.
	CString GetLastErrorMsg() { return m_strLastError; }
	//******************************************************************************************************************
	/// \brief					BANRIGAN에 데이터 저장.
	/// \param [in] nType		0x0001 : System Information
	///							0x0010 : Current Work Group Number
	///							0x0100 : All User Variable (Integer + Real) 
	///							0x0101 : User Variable [Integer]
	///							0x0102 : User Variable [Real]
	///							0x0200 : All Register Data
	///							0x0201 ~ 0x0390 : Register Data No.1~400
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param bool				결과 반환.
	bool OnSaveData(const int nType, const int nRegNo=0);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 모니터 화면을 설정.
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
	/// \param bool				결과 반환.
	bool SetMonitor(const int nMode, const int nDisp1, const int nDisp2, const int nDisp3, const int nDisp4);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 이미지 획득.
	/// \param [in]  nImage		0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0011 ~ 0x0014 : Stop Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Image	(Camera 1~4)
	///							0x0031 ~ 0x0050 : User Image		(No.1~32)
	/// \param [out] pBuffer	출력 이미지.
	/// \param bool				결과 반환.
	bool GetImage(const int nImage, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 이미지 설정.
	/// \param [in]  nImage		0x0001 ~ 0x0004 : Live Image		(Camera 1~4)
	///							0x0011 ~ 0x0014 : Stop Image		(Camera 1~4)
	///							0x0021 ~ 0x0024 : Measurement Image	(Camera 1~4)
	///							0x0031 ~ 0x0050	: User Image		(No.1~32)
	/// \param [out] pBuffer	출력 이미지.
	/// \param bool				결과 반환.
	bool SetImage(const int nImage, const int nWidth, const int nHeight, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 취득.
	/// \param [in]  nRegNo		Register Number No.1~400
	/// \param [out] nRegOrgX	Origin Position X
	/// \param [out] nRegOrgY	Origin Position Y
	/// \param [out] nRegSizeW	Image Width Size
	/// \param [out] nRegSizeH	Image Height Size
	/// \param [out] nRefPosX	Reference Position X
	/// \param [out] nRefPosY	Reference Position Y
	/// \param bool				결과 반환.
	bool GetRegisterData(const int nRegNo, long &nRegOrgX, long &nRegOrgY, int &nRegSizeW, int &nRegSizeH, float &nRefPosX, float &nRefPosY);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 이미지 취득.
	/// \param [in]  nRegNo		Register Number No.1 ~ 400
	/// \param [out] pBuffer	이미지 데이터. ※ 출력되는 이미지 크기는 128x128만 가능.
	/// \param bool				결과 반환.
	bool GetRegisterImage(const int nRegNo, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 적용.
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param [in] nWidth		Image Width Size
	/// \param [in] nHeight		Image Height Size
	/// \param [in] nRegPosX	Origin Position X
	/// \param [in] nRegPosY	Origin Position Y
	/// \param [in] pBuffer		Image Pointer
	/// \param bool				결과 반환.
	bool SetRegisterData(const int nRegNo, const int nWidth, const int nHeight, const int nRegPosX, const int nRegPosY, const BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 유저 데이터 적용.
	/// \param [in] nUserNo		User Data Number No.1~1024
	/// \param [in] nData		User Data Value
	/// \param bool				결과 반환.
	bool SetUserData(const int nUserNo, int nData);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 제거.
	/// \param [in] nRegNo		Register Number No.1~400
	/// \param bool				결과 반환.
	bool OnDeleteRegisterData(const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 추가.
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
	/// \param bool				결과 반환.
	bool OnAddRegisterData(const int nRegNo, const int nImage, const int nRegOrgX, const int nRegOrgY, const int nRegSize, int nRefPosX, int nRefPosY);
	//******************************************************************************************************************
	/// \brief					 BANRIGAN의 Application Flow를 처음부터 실행.
	/// \param [in]  nGroup		 0 : Current Work Group Number
	///							 Work Group Number No.1~16
	/// \param [in]  nFlow		 0 : Current Application Flow Number
	///							 Application Flow Number No.1~64
	/// \param [Out] pResult	 Result Data Pointer
	/// \param bool				 결과 반환.
	bool OnExecute(const int nGroup, const int nFlow, float* pResult);
	//******************************************************************************************************************
	/// \brief					 BANRIGAN의 각종 리셋을 실행.
	/// \param [in]  nType		 0:실행 상태, 1:Flow 실행 결과, 2:User변수[현재값], 3:화상 데이터, 4:로그 데이터, 5:알람 리셋, 6:소프트웨어 리셋, 7:하드웨어 리셋
	/// \param bool				 결과 반환.
	bool OnReset(const int nType);

private :
	CString GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG);
};
