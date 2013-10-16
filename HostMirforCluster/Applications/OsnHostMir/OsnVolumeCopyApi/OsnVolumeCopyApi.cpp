// OsnVolumeCopyApi.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "stdio.h"
#include "atlbase.h"
#include "windows.h"
#include "winioctl.h"
#include <wtypes.h>//GUID
#include "OsnVolumeCopyApi.h"


#define _CRT_SECURE_NO_WARNINGS
#define GUID_STRING_LEN 36

#ifdef _MANAGED
#pragma managed(push, off)
#endif


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
	return TRUE;
}


int	__cdecl	OSNCheckDiskRawProperty(unsigned int diskNumber)
{
	#define DRIVE_LAYOUT_SIZE 4 * sizeof(PARTITION_INFORMATION) + 8
	
	char name[1024];
	ULONG retVal;
	
	sprintf_s(name, "\\\\.\\PhysicalDrive%d", diskNumber);

	HANDLE file = CreateFile ( name,
                        GENERIC_READ|GENERIC_WRITE, 
						FILE_SHARE_READ|FILE_SHARE_WRITE, 
						NULL,	
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (INVALID_HANDLE_VALUE == file) 
	{
			return -1;
	}

	PDRIVE_LAYOUT_INFORMATION pPartitionInfo;
	
	pPartitionInfo=(PDRIVE_LAYOUT_INFORMATION)
							malloc(DRIVE_LAYOUT_SIZE);

	if(!pPartitionInfo)
	{
		CloseHandle(file);
	    return -2;
	}

	if (! DeviceIoControl(file,
					IOCTL_DISK_GET_DRIVE_LAYOUT , 
					NULL,0,
					pPartitionInfo, DRIVE_LAYOUT_SIZE,
					&retVal, NULL))
	{
		free(pPartitionInfo);

		CloseHandle(file);
		return GetLastError();
	}

	CloseHandle(file);
	
	if(pPartitionInfo->PartitionCount>4||
		pPartitionInfo->PartitionCount==0)
	{
		free(pPartitionInfo);

		return -4;
	}

	if(pPartitionInfo->Signature==0)
	{
		free(pPartitionInfo);
		return -5;
	}

	for(ULONG i=0;i<pPartitionInfo->PartitionCount;i++)
	{
		if(pPartitionInfo->PartitionEntry[i].PartitionLength.QuadPart||
			pPartitionInfo->PartitionEntry[i].StartingOffset.QuadPart)
		{
			free(pPartitionInfo);

			return -6;
		}
	}

	free(pPartitionInfo);

	return 0;	
}

int __cdecl OSNGetDiskFreeSpace(unsigned int diskIndex,unsigned __int64 *pDiskFreeSize)
{

	char	deviceName[MAX_PATH];
	sprintf_s(deviceName,"\\\\.\\PhysicalDrive%d\0",diskIndex);

	__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;


	if(0==GetDiskFreeSpaceEx(deviceName,(PULARGE_INTEGER)&i64FreeBytesToCaller,(PULARGE_INTEGER)&i64TotalBytes,(PULARGE_INTEGER)&i64FreeBytes))
		return -1;


	*pDiskFreeSize = i64FreeBytes/BLOCK_SIZE;
	return 0;


}

int	__cdecl	OSNGetDiskSize(unsigned int	diskIndex,
					  unsigned __int64		*pDiskSize)
{	
	HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,"\\\\.\\PhysicalDrive%d\0",diskIndex);
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return ERROR_OPEN_FAILED;
	}

	PARTITION_INFORMATION	partitionInfo;
	DWORD					retBytes;
	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_PARTITION_INFO,
					NULL,
					0,
					&partitionInfo,
					sizeof(PARTITION_INFORMATION),
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		return GetLastError();
	}
	CloseHandle(hFile);

	*pDiskSize = (partitionInfo.PartitionLength.QuadPart / BLOCK_SIZE);

	return 0;

}

int	__cdecl	OSNGetDiskSignature(unsigned int diskIndex)
{	
	HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,"\\\\.\\PhysicalDrive%d\0",diskIndex);
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return ERROR_OPEN_FAILED;
	}

	ULONG	size = sizeof(DRIVE_LAYOUT_INFORMATION) + 32*sizeof(PARTITION_INFORMATION);
	PDRIVE_LAYOUT_INFORMATION	pDriveLayoutInfo = (PDRIVE_LAYOUT_INFORMATION) malloc(size);
	DWORD						retBytes;


	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_DRIVE_LAYOUT,
					NULL,
					0,
					pDriveLayoutInfo,
					size,
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		return 0;
	}

	CloseHandle(hFile);

	return pDriveLayoutInfo->Signature;

}

int	__cdecl	OSNGetDiskGUIDByVolume(wchar_t			volumeLabel,		//C, D, E etc
						   char		*pDiskGUID)
{
	ULONG			retByte;
	char			volumeName[64];
	DISK_INFO     pDisk;
	DISK_INFOEX   pDiskEx;
	int restatus = ERROR_SUCCESS;

	sprintf_s(volumeName, "\\\\.\\%c:", volumeLabel);
	HANDLE	hVolume = CreateFile(volumeName,
							GENERIC_READ | GENERIC_WRITE, 
							FILE_SHARE_READ | FILE_SHARE_WRITE, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);
	if(hVolume==INVALID_HANDLE_VALUE)
		return GetLastError();
	
	
	ULONG					size = sizeof(VOLUME_DISK_EXTENTS) + 20 * sizeof(DISK_EXTENT);
	VOLUME_DISK_EXTENTS		*pVolumeDiskExtents = (VOLUME_DISK_EXTENTS	*) malloc(size);
	if(!pVolumeDiskExtents)
	{
		CloseHandle(hVolume);
		return ERROR_NOT_ENOUGH_MEMORY;
	}


	memset(pVolumeDiskExtents, 0, size);
	DWORD	status = OSNDeviceIoControl(hVolume,
										IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
										NULL,
										0,
										pVolumeDiskExtents,
										size,
										&retByte);


	if(status)	//fail
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return GetLastError();
	}
	

	if(pVolumeDiskExtents->NumberOfDiskExtents == 0)
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return ERROR_INVALID_DRIVE;
	}



	PDISK_EXTENT	pDiskExtent;


	//loop over partitions which consist of this volume
	for (ULONG i=0; i<pVolumeDiskExtents->NumberOfDiskExtents; i++)
	{
		//get signature through drive_layout
		pDiskExtent	= &pVolumeDiskExtents->Extents[i];

		if(i==0)
		{
			memset(&pDisk,0,sizeof(DISK_INFO));
			memset(&pDiskEx,0,sizeof(DISK_INFOEX));
			pDisk.m_DiskIndex=i;

			int ErrorCode=OsnGetDisk(&pDisk);
			if(ErrorCode == 0)
			{
				OsnGUIDToString(pDiskGUID,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
			}
			else
			{
				restatus = ERROR_INVALID_FUNCTION;
			}
		}

	}


   	free(pVolumeDiskExtents);
	CloseHandle(hVolume);
	return restatus;
}

int	__cdecl	OSNGetVolumeID(wchar_t			volumeLabel,		//C, D, E etc
						   unsigned int	&	diskSignature,
						   unsigned int &	startingOffset)
{

	DWORD			retByte;
	char			volumeName[64];
	sprintf_s(volumeName, "\\\\.\\%c:", volumeLabel);
	HANDLE	hVolume = CreateFile(volumeName,
							GENERIC_READ | GENERIC_WRITE, 
							FILE_SHARE_READ | FILE_SHARE_WRITE, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);
	if(hVolume==INVALID_HANDLE_VALUE)
		return GetLastError();
	
	
	ULONG					size = sizeof(VOLUME_DISK_EXTENTS) + 20 * sizeof(DISK_EXTENT);
	VOLUME_DISK_EXTENTS		*pVolumeDiskExtents = (VOLUME_DISK_EXTENTS	*) malloc(size);
	if(!pVolumeDiskExtents)
	{
		CloseHandle(hVolume);
		return ERROR_NOT_ENOUGH_MEMORY;
	}


	memset(pVolumeDiskExtents, 0, size);
	DWORD	status = OSNDeviceIoControl(hVolume,
										IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
										NULL,
										0,
										pVolumeDiskExtents,
										size,
										&retByte);


	if(status)	//fail
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return GetLastError();
	}
	

	if(pVolumeDiskExtents->NumberOfDiskExtents == 0)
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return ERROR_INVALID_DRIVE;
	}



	PDISK_EXTENT	pDiskExtent;
	DWORD			signature;
	DWORD			offset;


	//loop over partitions which consist of this volume
	for (ULONG i=0; i<pVolumeDiskExtents->NumberOfDiskExtents; i++)
	{
		//get signature through drive_layout
		pDiskExtent	= &pVolumeDiskExtents->Extents[i];

		signature	= OSNGetDiskSignature(pDiskExtent->DiskNumber);
		offset		= (DWORD) (pDiskExtent->StartingOffset.QuadPart / BLOCK_SIZE);	
		if(i==0)
		{
			diskSignature	= signature;
			startingOffset	= offset;
		}

	}


   	free(pVolumeDiskExtents);
	CloseHandle(hVolume);
	return ERROR_SUCCESS;
}


int	__cdecl	OSNGetVolumeSize(wchar_t	volumeLabel,unsigned __int64 & pVolumeSize)
{

	DWORD			retByte;
	char			volumeName[64];
	pVolumeSize=0;
	sprintf_s(volumeName, "\\\\.\\%c:", volumeLabel);
	HANDLE	hVolume = CreateFile(volumeName,
							GENERIC_READ | GENERIC_WRITE, 
							FILE_SHARE_READ | FILE_SHARE_WRITE, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);
	if(hVolume==INVALID_HANDLE_VALUE)
		return GetLastError();
	
	PARTITION_INFORMATION part;
	ULONG retVal;

	if (! DeviceIoControl(hVolume,
					IOCTL_DISK_GET_PARTITION_INFO, 
					NULL,0,
					&part, sizeof(PARTITION_INFORMATION),
					&retVal, NULL))
	{
		CloseHandle(hVolume);
		return -1;
	}
	

	//part.PartitionLength.QuadPart /= BLOCK_SIZE;

	pVolumeSize=(ULONGLONG) part.PartitionLength.QuadPart;
	
	/*ULONG					size = sizeof(VOLUME_DISK_EXTENTS) + 20 * sizeof(DISK_EXTENT);
	VOLUME_DISK_EXTENTS		*pVolumeDiskExtents = (VOLUME_DISK_EXTENTS	*) malloc(size);
	if(!pVolumeDiskExtents)
	{
		CloseHandle(hVolume);
		return ERROR_NOT_ENOUGH_MEMORY;
	}


	memset(pVolumeDiskExtents, 0, size);
	DWORD	status = OSNDeviceIoControl(hVolume,
										IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
										NULL,
										0,
										pVolumeDiskExtents,
										size,
										&retByte);


	if(status)	//fail
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return GetLastError();
	}
	

	if(pVolumeDiskExtents->NumberOfDiskExtents == 0)
	{
		free(pVolumeDiskExtents);
		CloseHandle(hVolume);
		return ERROR_INVALID_DRIVE;
	}



	PDISK_EXTENT	pDiskExtent;
	DWORD           pSize=0;
	DWORD			offset;


	//loop over partitions which consist of this volume
	for (ULONG i=0; i<pVolumeDiskExtents->NumberOfDiskExtents; i++)
	{
		//get signature through drive_layout
		pDiskExtent	= &pVolumeDiskExtents->Extents[i];

		//signature	= OSNGetDiskSignature(pDiskExtent->DiskNumber);
		pSize =   pSize+(DWORD)(pDiskExtent->ExtentLength.QuadPart);
		offset		= (DWORD) (pDiskExtent->StartingOffset.QuadPart / BLOCK_SIZE);	

	}

    pVolumeSize=pSize;*/
   	//free(pVolumeDiskExtents);
	CloseHandle(hVolume);
	return ERROR_SUCCESS;
}


int __cdecl	OSNDeviceIoControl(HANDLE	hFile,
						DWORD	ioControlCode,	
						PVOID	pInBuffer,
						ULONG	inBufferSize,
						PVOID	pOutBuffer,
						ULONG	outBufferSize,
						PULONG	pRetBytes)
{
	DWORD	status = 0;
	
	if(!DeviceIoControl(hFile, 
						ioControlCode, 
						pInBuffer, 
						inBufferSize,
						pOutBuffer,
						outBufferSize,
						pRetBytes, 
						NULL))
	{
		status = GetLastError();
	}

	return status;
}

HANDLE  OsnOpenVolumeCopy()
{
	HANDLE	hFile;
    hFile = CreateFile("\\\\.\\OsnVolumeCopy",
							GENERIC_READ | GENERIC_WRITE, 
							FILE_SHARE_READ | FILE_SHARE_WRITE, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);

	return hFile;
}

HANDLE  OsnOpenDiskCopy()
{
		HANDLE	hFile;
    hFile = CreateFile("\\\\.\\OsnDiskCopy",
							GENERIC_READ | GENERIC_WRITE, 
							FILE_SHARE_READ | FILE_SHARE_WRITE, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);

	return hFile;
}

int		__cdecl		OsnVolumeMark(ASYN_MARKER *pMarkInfo)
{
    
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_ASYNORSYN,
							pMarkInfo,
							sizeof(ASYN_MARKER),
							NULL,
							0,
							&retBytes);

	CloseHandle(hVolumeCopyFile);
	return status;

}

int		__cdecl		OsnDiskMark(ASYN_MARKER *pMarkInfo)
{
    
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_ASYNORSYN,
							pMarkInfo,
							sizeof(ASYN_MARKER),
							NULL,
							0,
							&retBytes);

	CloseHandle(hVolumeCopyFile);
	return status;

}

int		__cdecl		OsnSetVolumeCopy(MIRROR_INFO *pMirrorInfo)
{
    
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_SET_MIRROR,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	
	CloseHandle(hVolumeCopyFile);
	return status;

}


int __cdecl OsnSetClusterVolumeCopy(MIRROR_INFO *pMirrorInfo)
{
		HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_SET_MIRROR_EX,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int		__cdecl		OsnRemoveVolumeCopy(MIRROR_INFO *pMirrorInfo)
{
   
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_REMOVE_MIRROR,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int     __cdecl     OsnStartVolumeCopyInitialization(MIRROR_INFO *pMirrorInfo,INIT_TYPE type)
{
   
	
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	if(type == BASIC_INIT)
	{
	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_INITIALIZE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	}
	else if(type == NTFS_INIT)
	{
		status= OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_INITIALIZE_EX,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	}

	CloseHandle(hVolumeCopyFile);
	return status;
}
int     __cdecl     OsnStopVolumeCopyInitialization(MIRROR_INFO *pMirrorInfo)
{
   
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_STOP_INITIALIZE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int		__cdecl		OsnVolumeCopyMarkCDP(VOLUME_MARKER *pMarkInfo)
{
	
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
	{
		return ERROR_OPEN_FAILED;
	}


	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_MARKER,
							pMarkInfo,
							sizeof(VOLUME_MARKER),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;


}

int     __cdecl      OsnGetRecoveryPercentage(MIRROR_INFO *pMirrorInfo,unsigned int	*pInitFlag)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_GET_PERCENTAGE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pInitFlag,
							sizeof(ULONG),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}


int     __cdecl     OsnGetMirrorState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_GET_DEVICESTATE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pState,
							sizeof(QUERY_MIRROR_STATE),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int __cdecl OsnGetVolumeClusterMirrorRemoteState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;
	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_QUERY_REMOTE_STATE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pState,
							sizeof(QUERY_MIRROR_STATE),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int     __cdecl     OsnVolumeCopyGetVolumeSize(GUID guid,unsigned __int64  &pSize)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	VOLUMEID	volumeID;
	volumeID.SAN_VolumeID.m_VolumeGuid=guid;
	//volumeID.MBP_VolumeID.m_DiskSignature	= Signature;
	//volumeID.MBP_VolumeID.m_BlockOffset		= Offset;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_GET_DISKSIZE,
							&volumeID,
							sizeof(VOLUMEID),
							&pSize,
							sizeof(unsigned __int64),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int     __cdecl     OsnVolumeCopyGetMirrorsList(MIRROR_INFO_LIST *pList,unsigned int BufferSize)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_GET_MIRROR_LIST,
							NULL,
							0,
							pList,
							BufferSize,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;


}

int     __cdecl     OsnGetDisks(DISK_INFO_LIST *pList,unsigned int BufferSize)
{
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_GET_DISKS,
							NULL,
							0,
							pList,
							BufferSize,
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;

}

int  __cdecl OsnGetDisk(DISK_INFO *pDiskInfo)
{
	
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_GET_DISK,
							pDiskInfo,
							sizeof(DISK_INFO),
							pDiskInfo,
							sizeof(DISK_INFO),
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;
}

int     __cdecl     OsnVolumeCopyGetMirrorInfo(MIRROR_INFO *pMirrorInfo)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_GET_MIRROR_INFO,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int    __cdecl      OsnVolumeCopySetContinueInit(ULONG Flag)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_SET_INIT_FLAG,
							&Flag,
							sizeof(ULONG),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int     __cdecl     OsnVolumeCopySetRemoveFlag(ULONG Flag)
{
	HANDLE	hVolumeCopyFile = OsnOpenVolumeCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNVSS_SET_FLAG,
							&Flag,
							sizeof(ULONG),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;


}


int    __cdecl      OsnDiskCopySetContinueInit(ULONG Flag)
{
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_SET_INIT_FLAG,
							&Flag,
							sizeof(ULONG),
							NULL,
							0,
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;
}

int		__cdecl		OsnSetDiskCopy(MIRROR_INFO *pMirrorInfo)
{
    
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_SET_MIRROR,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;

}

int    __cdecl OsnSetClusterDiskCopy(MIRROR_INFO *pMirrorInfo)
{
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_SET_MIRROR_EX,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;
}


int		__cdecl		OsnRemoveDiskCopy(MIRROR_INFO *pMirrorInfo)
{
   
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_REMOVE_MIRROR,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int     __cdecl     OsnStartDiskCopyInitialization(MIRROR_INFO *pMirrorInfo)
{
   
	
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_INITIALIZE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;
}

int		__cdecl		OsnDiskCopyMarkCDP(VOLUME_MARKER *pMarkInfo)
{
	
	HANDLE	hDiskCopyFile = OsnOpenDiskCopy();
	if(hDiskCopyFile == INVALID_HANDLE_VALUE)
	{
		return ERROR_OPEN_FAILED;
	}


	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hDiskCopyFile,
							IOCTL_OSNDSS_MARKER,
							pMarkInfo,
							sizeof(VOLUME_MARKER),
							NULL,
							0,
							&retBytes);
	CloseHandle(hDiskCopyFile);
	return status;


}

int     __cdecl     OsnStopDiskCopyInitialization(MIRROR_INFO *pMirrorInfo)
{
   
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_STOP_INITIALIZE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int     __cdecl      OsnGetDiskCopyRecoveryPercentage(MIRROR_INFO *pMirrorInfo,unsigned int	*pInitFlag)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_GET_PERCENTAGE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pInitFlag,
							sizeof(ULONG),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}


int     __cdecl     OsnGetDiskCopyMirrorState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_GET_DEVICESTATE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pState,
							sizeof(QUERY_MIRROR_STATE),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int     __cdecl     OsnGetDiskClusterMirrorRemoteState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_QUERY_REMOTE_STATE,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pState,
							sizeof(QUERY_MIRROR_STATE),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}


int     __cdecl     OsnDiskCopyGetMirrorsList(MIRROR_INFO_LIST *pList,unsigned int BufferSize)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_GET_MIRROR_LIST,
							NULL,
							0,
							pList,
							BufferSize,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;


}
int     __cdecl     OsnDiskCopyGetMirrorInfo(MIRROR_INFO *pMirrorInfo)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_GET_MIRROR_INFO,
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							pMirrorInfo,
							sizeof(MIRROR_INFO),
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;

}

int     __cdecl     OsnDiskCopySetRemoveFlag(ULONG Flag)
{
	HANDLE	hVolumeCopyFile = OsnOpenDiskCopy();
	if(hVolumeCopyFile == INVALID_HANDLE_VALUE)
		return ERROR_OPEN_FAILED;

	ULONG	retBytes;
	DWORD	status;

	status = OSNDeviceIoControl(hVolumeCopyFile,
							IOCTL_OSNDSS_SET_FLAG,
							&Flag,
							sizeof(ULONG),
							NULL,
							0,
							&retBytes);
	CloseHandle(hVolumeCopyFile);
	return status;


}

int     __cdecl     OsnGetVolumeGUID(const char drive,char** guid)
{
	char guid0[MAX_PATH];
	char guid2[MAX_PATH];
	char	deviceName[MAX_PATH];
	sprintf_s(deviceName,MAX_PATH,"%c:\\",drive);
    if(GetVolumeNameForVolumeMountPoint(deviceName,guid0,MAX_PATH))//guid为\\?\Volume{GUID}
	{
        GetVolumeNameForVolumeMountPoint(deviceName,guid2,MAX_PATH);
		*guid=&guid2[10];//返回的guid的格式为{guid}\   //
		return 0;
	}
	return 1;
}
bool    __cdecl     OsnCHeckIsGPTVolume(const char drive,char* guid)
{
    HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,MAX_PATH,"\\\\.\\%c:",drive);//最后一定要有一个:
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,//FILE_ATTRIBUTE_DEVICE,//FILE_ATTRIBUTE_NORMAL,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) //IOCTL_DISK_GET_PARTITION_INFO_EX
	{
		int ret=GetLastError();
		return false;
	}

	ULONG	size = sizeof(PARTITION_INFORMATION_EX);
	PARTITION_INFORMATION_EX*	pDriveLayoutInfo = (PARTITION_INFORMATION_EX*) malloc(size);
	DWORD						retBytes;


	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_PARTITION_INFO_EX,
					NULL,
					0,
					pDriveLayoutInfo,
					size,
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		int ret=GetLastError();
		return false;
	}

	CloseHandle(hFile);
	bool ret=pDriveLayoutInfo->PartitionStyle==PARTITION_STYLE_GPT?true:false;
	if(ret)
	{
		char *s;
	    RPC_STATUS	status = UuidToString((GUID *) &pDriveLayoutInfo->Gpt.PartitionId, (unsigned char **)&s);

	    sprintf(guid,"%s\0", s);	
			
	    RpcStringFree((unsigned char**)&s);

	}
	
	return ret;
}
int     __cdecl     OsnGetDiskGUID(unsigned int diskIndex,char* guid)
{
	HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,MAX_PATH,"\\\\.\\PhysicalDrive%d\0",diskIndex);
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return GetLastError();
	}

	ULONG	size = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + 32*sizeof(PARTITION_INFORMATION_EX);
	PDRIVE_LAYOUT_INFORMATION_EX	pDriveLayoutInfo = (PDRIVE_LAYOUT_INFORMATION_EX) malloc(size);
	DWORD						retBytes;


	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_DRIVE_LAYOUT_EX,
					NULL,
					0,
					pDriveLayoutInfo,
					size,
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		return GetLastError();
	}

	CloseHandle(hFile);
	DRIVE_LAYOUT_INFORMATION_GPT gpt=(DRIVE_LAYOUT_INFORMATION_GPT)pDriveLayoutInfo->Gpt;
	char *s;
	RPC_STATUS	status = UuidToString(&gpt.DiskId, (unsigned char **)&s);

	sprintf(guid,"%s\0", s);	
			
	RpcStringFree((unsigned char**)&s);

	return status!=RPC_S_OK;
}

int     __cdecl     OsnGUIDToString(char* pStringBuffer,GUID guid)
{
	if(!pStringBuffer)
		return -1;

	char *s;
	RPC_STATUS	status = UuidToString(&guid, (unsigned char **)&s);

	sprintf(pStringBuffer,"%s\0", s);	
			
	RpcStringFree((unsigned char**)&s);

	return status!=RPC_S_OK;
}
int     __cdecl     OsnGUIDFromString(char* pStringBuffer,GUID *guid)
{
	if(!pStringBuffer || !guid)
		return -1;

	char s[64];
	strncpy(s, &pStringBuffer[0], GUID_STRING_LEN);	//get rid of osn_{}
	s[GUID_STRING_LEN] = '\0';

	RPC_STATUS	status = UuidFromString((unsigned char *) s, guid);

	return status!=RPC_S_OK;
}
int    __cdecl     OsnCheckGptDisk(unsigned int diskIndex)
{
	HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,MAX_PATH,"\\\\.\\PhysicalDrive%d\0",diskIndex);
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return 2;
	}

	ULONG	size = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + 32*sizeof(PARTITION_INFORMATION_EX);
	PDRIVE_LAYOUT_INFORMATION_EX	pDriveLayoutInfo = (PDRIVE_LAYOUT_INFORMATION_EX) malloc(size);
	DWORD						retBytes;


	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_DRIVE_LAYOUT_EX,
					NULL,
					0,
					pDriveLayoutInfo,
					size,
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		return 3;
	}
	CloseHandle(hFile);
	if(PARTITION_STYLE_GPT==pDriveLayoutInfo->PartitionStyle)
	{
		return 1;
	}
	return -1;
}
int    __cdecl     OsnCheckGptVolume(const char drive)
{
    HANDLE	hFile;
	char	deviceName[MAX_PATH];

	sprintf_s(deviceName,MAX_PATH,"\\\\.\\%c:",drive);//最后一定要有一个:
	hFile =	CreateFile(deviceName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						0,//FILE_ATTRIBUTE_DEVICE,//FILE_ATTRIBUTE_NORMAL,
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) //IOCTL_DISK_GET_PARTITION_INFO_EX
	{
		//int ret=GetLastError();
		return 2;
	}

	ULONG	size = sizeof(PARTITION_INFORMATION_EX);
	PARTITION_INFORMATION_EX*	pDriveLayoutInfo = (PARTITION_INFORMATION_EX*) malloc(size);
	DWORD						retBytes;


	if(!DeviceIoControl(hFile,
					IOCTL_DISK_GET_PARTITION_INFO_EX,
					NULL,
					0,
					pDriveLayoutInfo,
					size,
					&retBytes,
					NULL))
	{
		CloseHandle(hFile);
		//int ret=GetLastError();
		return 3;
	}

	CloseHandle(hFile);
	int ret=pDriveLayoutInfo->PartitionStyle==PARTITION_STYLE_GPT?1:-1;
	
	return ret;
}

int __cdecl OSNCreateDssRescanEvent()
{
	HANDLE DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues=0;
			if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_CREATE_RESCAN_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
		{
			retValues=GetLastError();
		}

	CloseHandle(DssHandle);
	return retValues ;
}

int __cdecl OSNCreateVssRescanEvent()
{
	HANDLE VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues=0;
			if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_CREATE_RESCAN_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
		{
			retValues=GetLastError();
		}

	CloseHandle(VssHandle);
	return retValues ;
}


int  __cdecl OSNCreateDssNotificationEvent()
{
	HANDLE DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}


	DWORD	retBytes;
	DWORD	retValues=0;
		if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_CREATE_NOTIFICATION_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
		{
			retValues=GetLastError();
		}

    CloseHandle(DssHandle);
    return retValues;

}
int  __cdecl OSNCreateVssNotificationEvent()
{
	HANDLE VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}


	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_CREATE_NOTIFICATION_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
    return retValues;

}

int __cdecl  OSNDssSignaledTheEvent()
{
	HANDLE	DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}


	DWORD	retBytes;
	DWORD	retValues = 0;
	if(DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_SIGNALED_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}
    CloseHandle(DssHandle);
    return retValues;

}

int __cdecl  OSNVssSignaledTheEvent()
{
	HANDLE	VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}


	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_SIGNALED_EVENT,
							NULL,
							0,
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
    return retValues;

}

int __cdecl OSNDssGetMirrorNotificationList(char *pBuffer,unsigned int *Count)
{
	HANDLE	DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_GET_MIRROR_NOTIFICATION_LIST,
							NULL,
							0,
							pBuffer,
							*Count,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(DssHandle);
	*Count=retBytes;
    return retValues;
}
int __cdecl OSNVssGetMirrorNotificationList(char *pBuffer,unsigned int *Count)
{
	HANDLE	VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_GET_MIRROR_NOTIFICATION_LIST,
							NULL,
							0,
							pBuffer,
							*Count,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
	*Count=retBytes;
    return retValues;

}

int __cdecl OSNDssRemoveMirrorNotification(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE	DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_REMOVE_MIRROR_NOTIFICATION,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(DssHandle);
    return retValues;


}
int __cdecl OSNVssRemoveMirrorNotification(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE	VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_REMOVE_MIRROR_NOTIFICATION,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
    return retValues;


}

int __cdecl OSNDssSetNotification(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE	DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_SET_MIRROR_NOTIFICATION,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(DssHandle);
    return retValues;

}
int __cdecl OSNVssSetNotification(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE	VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_SET_MIRROR_NOTIFICATION,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
    return retValues;

}

int __cdecl OSNDssSetRemoteState(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE DssHandle = OsnOpenDiskCopy();
	if(DssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(DssHandle,
							IOCTL_OSNDSS_SET_REMOTE_MIRRORSTATE,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(DssHandle);
	return retValues;
}

int __cdecl OSNVssSetRemoteState(NOTIFICATION_OBJECT *pBuffer)
{
	HANDLE	VssHandle = OsnOpenVolumeCopy();
	if(VssHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD	retBytes;
	DWORD	retValues = 0;
	if(!DeviceIoControl(VssHandle,
							IOCTL_OSNVSS_SET_REMOTE_MIRRORSTATE,
							pBuffer,
							sizeof(NOTIFICATION_OBJECT),
							NULL,
							0,
							&retBytes,
							NULL))
	{
		retValues = GetLastError();
	}

    CloseHandle(VssHandle);
    return retValues;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

