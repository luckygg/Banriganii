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
	/// \brief					BANRIGAN에 연결.
	/// \param [in] strIP		반리간 IP Address.
	/// \param [in] nPort		반리간 Port Number.
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
	/// \param [in] nType		0:시스템 정보, 1:현재 워크 그룹 번호, 2:모든 사용자 변수 (정수형 + 실수형), 3:사용자 변수 [정수], 4:사용자 변수 [실수], 5:전체 레지스터 데이터. 6:특정 레지스터 데이터
	/// \param [in] nRegNo		레지스터 번호 (1 ~ 400).
	/// \param bool				결과 반환.
	bool OnSaveData(const int nType, const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 모니터 화면을 설정.
	/// \param [in] nImage		0:1 Display, 1:2 Display, 2:4 Display.
	/// \param [in] nMode		0:Run Mode, 1:Setting Mode.
	/// \param [in] nDisp		0:NONE, 1:CAM1, 2:CAM2, 3:CAM3, 4:CAM4, 5:MEASURE1, 6:MEASURE2, 7:MEASURE3, 8:MEASURE4.
	/// \param bool				결과 반환.
	bool SetMonitor(const int nImage, const int nMode, const int nDisp1, const int nDisp2, const int nDisp3, const int nDisp4);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 이미지 획득.
	/// \param [in]  nImage		0:CAM1, 1:CAM2, 2:CAM3, 3:CAM4, 4:MEASURE1, 5:MEASURE2, 6:MEASURE3, 7:MEASURE4;POC.
	/// \param [out] pBuffer	출력 이미지.
	/// \param bool				결과 반환.
	bool GetImage(const int nImage, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 취득.
	/// \param [in]  nRegNo		레지스터 번호 (1 ~ 400).
	/// \param [out] nRegPosX	이미지 시작 위치 X.
	/// \param [out] nRegPosY	이미지 시작 위치 Y.
	/// \param [out] nRefPosX	기준점 좌표 X.
	/// \param [out] nRefPosY	기준점 좌표 Y.
	/// \param [out] pBuffer	출력 이미지.
	/// \param bool				결과 반환.
	bool GetRegisterData(const int nRegNo, long &nRegPosX, long &nRegPosY, float &nRefPosX, float &nRefPosY, BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 적용.
	/// \param [in] nRegNo		레지스터 번호 (1 ~ 400).
	/// \param [in] nWidth		레지스터 이미지 가로 사이즈.
	/// \param [in] nHeight		레지스터 이미지 가로 사이즈.
	/// \param [in] nRegPosX	이미지 시작 위치 X.
	/// \param [in] nRegPosY	이미지 시작 위치 Y.
	/// \param [in] pBuffer		레지스터 이미지.
	/// \param bool				결과 반환.
	bool SetRegisterData(const int nRegNo, const int nWidth, const int nHeight, const int nRegPosX, const int nRegPosY, const BYTE* pBuffer);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 제거.
	/// \param [in] nRegNo		레지스터 번호 (1 ~ 400).
	/// \param bool				결과 반환.
	bool OnDeleteRegisterData(const int nRegNo);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 특정 레지스터 데이터 추가.
	/// \param [in] nRegNo		레지스터 번호 (1 ~ 400).
	/// \param [in] nImage		1:CAM1, 2:CAM2, 3:CAM3, 4:CAM4.
	/// \param [in] nRegPosX	이미지 데이터 시작 위치 X 방향(기본값 0).
	/// \param [in] nRegPosY	이미지 데이터 시작 위치 Y 방향(기본값 0).
	/// \param [in] nRegWidth	이미지 데이터 가로 사이즈(기본값 128).
	/// \param [in] nRegHeight	이미지 데이터 세로 사이즈(기본값 128).
	/// \param bool				결과 반환.
	bool OnAddRegisterData(const int nRegNo, const int nImage, const int nRegPosX=0, const int nRegPosY=0, const int nRegWidth=128, const int nRegHeight=128);
	//******************************************************************************************************************
	/// \brief					BANRIGAN의 Application Flow를 처음부터 실행.
	/// \param [in] nGroup		0:현재 워크 그룹 번호, 1~16 워크 그룹 번호 설정
	/// \param [in] nFlow		0:현재 흐름 번호, 1~64 흐름 번호 설정
	/// \param [Out] pResult	결과 값이 반환될 포인터.
	/// \param [in] nResultSize	결과 값이 반환될 포인터의 사이즈(공통 결과 수 16개 + 유저 결과 수).
	/// \param bool				결과 반환.
	bool OnExecute(const int nGroup, const int nFlow, float* pResult, int nResultSize);
	
private :
	CString GetErrorMessage(StCommonCmd Cmd, StCommonNG CmdNG);
};
