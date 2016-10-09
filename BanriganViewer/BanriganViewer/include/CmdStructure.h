#pragma once

#define	CMD_SAVE		0x0104
#define	CMD_MONITOR		0x0105
#define CMD_SET_USR		0x0131
#define	CMD_GET_IMG		0x0132
#define	CMD_GET_REG		0x0135
#define	CMD_SET_REG		0x0136
#define CMD_ADD_REG		0x0137
#define	CMD_DEL_REG		0x0138
#define	CMD_EXECUTE		0x0140


//----- Common Command -----//
struct StCommonCmd
{
	unsigned long	MsgSize;
	unsigned short	MainCode;
	unsigned short	MsgID;
};

struct StCommonNG		
{
	unsigned long	ErrCode;
	unsigned long	Option;
};

//----- Execute Command -----//
struct StSndExecute
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	Option;
	unsigned long	Group;
	unsigned long	Flow;
};

struct StRcvExecute
{
	unsigned long	SubCode;
	unsigned long	Option;
	unsigned long	Group;
	unsigned long	Flow;
};

//----- Get Image Command -----//
struct StSndGetImage
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	Image;
};

struct StRcvGetImage
{
	unsigned long	SubCode;
	unsigned long	Image;
	unsigned long	Reserve1;
	unsigned long	Format;
	long			PosX;
	long			PosY;
	unsigned long	SizeW;
	unsigned long	SizeH;
	double			Reserve2;
};

//----- Set Register Data Command -----//
struct StSndSetRegData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	RegNo;
	unsigned long	Version;
	double			System;
	unsigned long	Reg;
	unsigned long	Match;
	unsigned long	Reserve1;
	long			RegPosX;
	long			RegPosY;
	unsigned long	RegSizeW;
	unsigned long	RegSizeH;
	unsigned long	POCSizeW;
	unsigned long	POCSizeH;
	float			RefPosX;
	float			RefPosY;
	unsigned long	Mask;
	float			MaskPosX;
	float			MaskPosY;
	float			MaskSizeW;
	float			MaskSizeH;
	unsigned long	Reserve2;
};

struct	StRcvSetRegData
{
	unsigned long	SubCode;
	unsigned long	RegNo;
};

//----- Get Register Data Command -----//
struct StSndGetRegData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	RegNo;
};

struct	StRcvGetRegData
{
	unsigned long	SubCode;
	unsigned long	RegNo;
	unsigned long	Version;
	double			System;
	unsigned long	Reg;
	unsigned long	Match;
	unsigned long	Reserve1;
	long			RegOrgX;
	long			RegOrgY;
	unsigned long	RegSizeW;
	unsigned long	RegSizeH;
	unsigned long	POCSizeW;
	unsigned long	POCSizeH;
	float			RefPosX;
	float			RefPosY;
	unsigned long	Mask;
	float			MaskPosX;
	float			MaskPosY;
	float			MaskSizeW;
	float			MaskSizeH;
	unsigned long	Reserve2;
};

//----- Delete Register Data Command -----//
struct StSndDeleteRegData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	RegNo;
};

struct	StRcvDeleteRegData
{
	unsigned long	SubCode;
	unsigned long	RegNo;
};

//----- Add Register Data Command -----//
struct StSndAddRegData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	RegNo;
	unsigned long	Version;
	double			System;
	unsigned long	Reserve1;
	unsigned long	Match;
	unsigned long	Image;
	long			RegPosX;
	long			RegPosY;
	unsigned long	RegSizeW;
	unsigned long	RegSizeH;
	unsigned long	POCSizeW;
	unsigned long	POCSizeH;
	float			RefPosX;
	float			RefPosY;
	unsigned long	Mask;
	float			MaskPosX;
	float			MaskPosY;
	float			MaskSizeW;
	float			MaskSizeH;
	unsigned long	Reserve2;
};

struct	StRcvAddRegData
{
	unsigned long	SubCode;
	unsigned long	RegNo;
};

//----- Set Monitor Command -----//
struct StSndSetMonitor
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	Type;
	unsigned long	Disp1;
	unsigned long	Disp2;
	unsigned long	Disp3;
	unsigned long	Disp4;
};

struct	StRcvSetMonitor
{
	unsigned long	SubCode;
	unsigned long	Type;
	unsigned long	Disp1;
	unsigned long	Disp2;
	unsigned long	Disp3;
	unsigned long	Disp4;
};

//----- Save Data Command -----//
struct StSndSaveData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	Type;
};

struct StRcvSaveData
{
	unsigned long	SubCode;
	unsigned long	RegNo;
};

//----- Set Register Data Command -----//
struct StSndUserData
{
	StCommonCmd		Cmd;
	unsigned long	SubCode;
	unsigned long	Target;
	unsigned long	Address;
	unsigned long	Num;
};

struct StRcvUserData
{
	unsigned long	SubCode;
	unsigned long	Target;
	unsigned long	Variable;
	unsigned long	Address;
	unsigned long	Num;
};