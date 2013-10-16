
#ifndef _DDOsnSnap_H
#define _DDOsnSnap_H

#define OSNSNAP_DEVICE_TYPE					60004


#define IOCTL_OSNSNAP_CREATE_SNAPSHOT	CTL_CODE(OSNSNAP_DEVICE_TYPE, 2001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OSNSNAP_DELETE_SNAPSHOT   CTL_CODE(OSNSNAP_DEVICE_TYPE, 2002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OSNSNAP_ENABLE_SNAPSHOT   CTL_CODE(OSNSNAP_DEVICE_TYPE, 2003, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OSNSNAP_DISABLE_SNAPSHOT  CTL_CODE(OSNSNAP_DEVICE_TYPE, 2004, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OSNSNAP_GET_SNAP_INFO		CTL_CODE(OSNSNAP_DEVICE_TYPE, 2005, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OSNSNAP_COMPLETE_IMAGE	CTL_CODE(OSNSNAP_DEVICE_TYPE, 2006, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct OsnSnapInfo
{
	WCHAR			sourceName[128];
	WCHAR			desName[128];
	ULONG			sourceState;
	ULONG			desState;
	ULONG			bitSize;
	ULONG			ciStatus;
	ULONG			ciPercentage;
	LARGE_INTEGER	m_EnableTime;
};

#define SNAP_RELATION_DISABLED			0
#define SNAP_RELATION_ENABLED_OK		1
#define SNAP_RELATION_ENABLED_FAILED	2

#endif