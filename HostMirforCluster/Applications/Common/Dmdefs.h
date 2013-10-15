#ifndef _DISPATCH_H_
#define _DISPATCH_H_

namespace OsnDm
{
	struct VolumeGroupInfo
	{
		WCHAR		Name[32];
		GUID		Id;
		ULONG		RecordSize;
		ULONG		CatalogSize;//not used
		ULONG		State;
		ULONG		TotalRecordNumber;
		ULONG		TotalFreeRecordNumber;
	};

	struct VolumeGroupList
	{
		ULONG		Number;
		VolumeGroupInfo Info[1];
	};

	struct DiskInfo
	{
		ULONG		DiskNumber;
		GUID		DiskId;
		GUID		VolumeGroupId;
		CHAR		ScsiInfo[64];
		ULONGLONG	Size;//in bytes
	};

	struct DiskList
	{
		ULONG		Number;
		GUID		VolumeGroupId;
		DiskInfo	Info[1];
	};

	struct RecordInfo
	{
		GUID		VolumeId;
		ULONG		VolumeOffset;//in records
	};

	struct VolumeInfo
	{
		GUID		VolumeId;
		GUID		VolumeGroupId;
		WCHAR		Name[64];
		ULONGLONG	Size;//in bytes
		ULONG		NumberRecords;
		RecordInfo	Record[1];
	};

	struct VolumeList
	{
		ULONG		Number;
		GUID		VolumeGroupId;
		VolumeInfo	Info[1];
	};
};

#endif
