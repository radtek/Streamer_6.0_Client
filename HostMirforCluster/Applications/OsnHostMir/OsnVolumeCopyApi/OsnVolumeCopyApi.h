#ifndef _OSN_VolumeCopy_H
#define _OSN_VolumeCopy_H

//#include "..\..\..\Core\OSNVolumeCopy\Configure.h"
#include <wtypes.h>//GUID
#include"Configure.h"


int	__cdecl OSNCheckDiskRawProperty(unsigned int diskIndex);
int __cdecl OSNGetDiskFreeSpace(unsigned int diskIndex,unsigned __int64 *pDiskFreeSize);

int	__cdecl	OSNGetDiskSize(unsigned int	diskIndex,
					  unsigned __int64		*pDiskSize);

int	__cdecl	OSNGetDiskSignature(unsigned int diskIndex);
int	__cdecl	OSNGetVolumeID(wchar_t				volumeLabel,		//C, D, E etc
						   unsigned int	&	diskSignature,
						   unsigned int &	startingOffset);
int	__cdecl	OSNGetDiskGUIDByVolume(wchar_t			volumeLabel,		//C, D, E etc
						   char		*pDiskGUID);
int	__cdecl	OSNGetVolumeSize(wchar_t	volumeLabel,unsigned __int64 & pVolumeSize);


int __cdecl	OSNDeviceIoControl(HANDLE	hFile,
						DWORD	ioControlCode,	
						PVOID	pInBuffer,
						ULONG	inBufferSize,
						PVOID	pOutBuffer,
						ULONG	outBufferSize,
						PULONG	pRetBytes);

int		__cdecl		OsnSetVolumeCopy(MIRROR_INFO *pMirrorInfo);
int		__cdecl		OsnVolumeMark(ASYN_MARKER *pMarkInfo);
int		__cdecl		OsnDiskMark(ASYN_MARKER *pMarkInfo);
int		 __cdecl OsnSetClusterVolumeCopy(MIRROR_INFO *pMirrorInfo);
int		__cdecl		OsnRemoveVolumeCopy(MIRROR_INFO *pMirrorInfo);

int     __cdecl     OsnStartVolumeCopyInitialization(MIRROR_INFO *pMirrorInfo,INIT_TYPE);
int     __cdecl     OsnStopVolumeCopyInitialization(MIRROR_INFO *pMirrorInfo);

int     __cdecl     OsnGetRecoveryPercentage(MIRROR_INFO *pMirrorInfo,unsigned int	*pInitFlag);

int     __cdecl     OsnGetMirrorState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState);

int     __cdecl     OsnVolumeCopyGetVolumeSize(GUID guid,unsigned __int64 &pSize);

int     __cdecl     OsnVolumeCopyGetMirrorsList(MIRROR_INFO_LIST *pList,unsigned int BufferSize);

int     __cdecl     OsnVolumeCopyGetMirrorInfo(MIRROR_INFO *pMirrorInfo);
int     __cdecl     OsnVolumeCopySetRemoveFlag(ULONG Flag);
int    __cdecl      OsnVolumeCopySetContinueInit(ULONG Flag);
int		__cdecl		OsnVolumeCopyMarkCDP(VOLUME_MARKER *pMarkInfo);

HANDLE  OsnOpenVolumeCopy();

HANDLE  OsnOpenDiskCopy();

int		__cdecl		OsnDiskCopyMarkCDP(VOLUME_MARKER *pMarkInfo);
int    __cdecl      OsnDiskCopySetContinueInit(ULONG Flag);
int		__cdecl		OsnSetDiskCopy(MIRROR_INFO *pMirrorInfo);
int    __cdecl		OsnSetClusterDiskCopy(MIRROR_INFO *pMirrorInfo);
int		__cdecl		OsnRemoveDiskCopy(MIRROR_INFO *pMirrorInfo);

int     __cdecl     OsnStartDiskCopyInitialization(MIRROR_INFO *pMirrorInfo);
int     __cdecl     OsnStopDiskCopyInitialization(MIRROR_INFO *pMirrorInfo);

int     __cdecl     OsnGetDiskCopyRecoveryPercentage(MIRROR_INFO *pMirrorInfo,unsigned int	*pInitFlag);

int     __cdecl     OsnGetDiskCopyMirrorState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState);

int     __cdecl     OsnDiskCopyGetMirrorsList(MIRROR_INFO_LIST *pList,unsigned int BufferSize);

int     __cdecl     OsnDiskCopyGetMirrorInfo(MIRROR_INFO *pMirrorInfo);

int     __cdecl     OsnDiskCopySetRemoveFlag(ULONG Flag);

int     __cdecl     OsnGetDisks(DISK_INFO_LIST *pList,unsigned int BufferSize);
int     __cdecl     OsnGetDisk(DISK_INFO *pDiskInfo);
int     __cdecl     OsnGetVolumeGUID(const char ,char** );
bool    __cdecl     OsnCHeckIsGPTVolume(const char ,char* );
int     __cdecl     OsnGetDiskGUID(unsigned int ,char*);
int     __cdecl     OsnGUIDToString(char* pStringBuffer,GUID guid);
int     __cdecl     OsnGUIDFromString(char* pStringBuffer,GUID *guid);
int    __cdecl     OsnCheckGptDisk(unsigned int diskIndex);
int    __cdecl     OsnCheckGptVolume(const char drive);
int     __cdecl     OsnGetDiskClusterMirrorRemoteState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState);
int __cdecl OsnGetVolumeClusterMirrorRemoteState(MIRROR_INFO *pMirrorInfo,QUERY_MIRROR_STATE *pState);
int  __cdecl OSNCreateDssNotificationEvent();
int  __cdecl OSNCreateVssNotificationEvent();
int __cdecl  OSNCreateDssRescanEvent();
int __cdecl  OSNCreateVssRescanEvent();
int __cdecl  OSNDssSignaledTheEvent();
int __cdecl  OSNVssSignaledTheEvent();

int __cdecl OSNDssGetMirrorNotificationList(char *pBuffer,unsigned int *Count);
int __cdecl OSNVssGetMirrorNotificationList(char *pBuffer,unsigned int *Count);

int __cdecl OSNDssRemoveMirrorNotification(NOTIFICATION_OBJECT *pBuffer);
int __cdecl OSNVssRemoveMirrorNotification(NOTIFICATION_OBJECT *pBuffer);

int __cdecl OSNDssSetNotification(NOTIFICATION_OBJECT *pBuffer);
int __cdecl OSNVssSetNotification(NOTIFICATION_OBJECT *pBuffer);

int __cdecl OSNDssSetRemoteState(NOTIFICATION_OBJECT *pBuffer);
int __cdecl OSNVssSetRemoteState(NOTIFICATION_OBJECT *pBuffer);

#endif 
