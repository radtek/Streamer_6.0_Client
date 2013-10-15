#ifndef _LUN_H_
#define _LUN_H_

// LUN Level Address Structure

union LUN_LEVEL 
{
	struct 
	{
		UCHAR		MethodSpecific_1	: 6;
		UCHAR		AddressMethod		: 2;
		UCHAR		MethodSpecific_2;
	} Common;


	struct 
	{
		UCHAR		BusIdentifier		: 6;
		UCHAR		AddressMethod		: 2;		
		UCHAR		TargetLun;
	} PeripheralDevice;


	struct 
	{
		UCHAR		LunMsb				: 6;
		UCHAR		AddressMethod		: 2;		
		UCHAR		LunLsb;
	} DeviceSpecific;


	struct 
	{
		UCHAR		Target				: 6;
		UCHAR		AddressMethod		: 2;		
		UCHAR		Lun					: 5;
		UCHAR		BusNumber			: 3;
	} LogicalUnit;

	USHORT			Raw;
};


union OSNLUN
{
	UCHAR Byte[8];
	struct 
	{
		LUN_LEVEL	Level1;
		LUN_LEVEL	Level2;
		LUN_LEVEL	Level3;
		LUN_LEVEL	Level4;
	};
	ULONGLONG QuadPart;

	bool operator == (OSNLUN lun)
	{
		return QuadPart==lun.QuadPart;
	}

	bool operator == (ULONGLONG lun)
	{
		return QuadPart==lun;
	}
};


#pragma warning (disable:4200)

typedef struct _REPORT_LUN_LIST
{
	ULONG		LunListLength;
	ULONG		Reserved;
	OSNLUN		Lun[];
} REPORT_LUN_LIST, *PREPORT_LUN_LIST;

#pragma warning (default:4200)

#endif
