
#ifndef _OSN_COMMON_H_
#define _OSN_COMMON_H_

#define BIT_PER_BYTE					8
#define	BLOCK_SIZE						512
#define OSN_MAX_MARKER					16
#define INITIALIZE_SIZE					1024			//512K

#define	DRIVER_SYMBOLIC_NAME	L"\\??\\OSNNd"



#define	OSN_MAX_HOSTNAME				32
#define	OSN_MAX_VOLUMENAME				128



//For server usage. TargetName should be unique, as well combination of ClientIpAddress and ServerIpAddress
typedef	struct _TARGET_INFO
{
	ULONG	ClientIpAddress;
	ULONG	ServerIpAddress;
	char	TargetName[OSN_MAX_HOSTNAME];
} TARGET_INFO, *PTARGET_INFO;



typedef struct _TARGET_LIST
{
	ULONG		TargetCount;
	TARGET_INFO	TargetInfo[1];
} TARGET_LIST, *PTARGET_LIST;



//For server usage. InitiatorName should be unique, as well combination of WWN and ServerIpAddress
typedef	struct _INITIATOR_INFO
{
	ULONG	ServerIpAddress;		//specific server IP address this Initiator will connect to 
	WWN		InitiatorWWN;
	char	InitiatorName[OSN_MAX_HOSTNAME];		
} INITIATOR_INFO, *PINITIATOR_INFO;



typedef	struct _BACKUP_INFO
{
	VOLUMEID		VolumeID;
	char			RemoteHostWWN[OSN_MAX_HOSTNAME];
	char			TargetVolumeName[OSN_MAX_VOLUMENAME];
	char			m_ClientType;							//VOLUME_REPLICATION, DISK_REPLICATION, SAN_REPLICATION
	ULONG			m_StartingBlockOffset;					//Required for VOLUME_REPLICATION's Boot Volume
} BACKUP_INFO, *PBACKUP_INFO;

typedef struct _BACKUP_INFO_LIST
{
	unsigned int num;
	BACKUP_INFO  m_BackupInfo[1];
}BACKUP_LIST,*PBACKUP_LIST;


typedef enum _DRIVER_MODE
{
	MODE_NONE,
	MODE_INITIATOR,
	MODE_TARGET,
	MODE_INITIATOR_TARGET
} DRIVER_MODE, *PDRIVER_MODE;



typedef enum _SESSION_STATE
{
	SESSION_CONNECTED,				//Endpoint opened and listening for connection
	SESSION_UNAVAILABLE,	
	SESSION_LOGGED_IN,				//Initiator connected on this endpoint, no further connection accepted
	SESSION_INTERNAL_ERROR,			//No initiator connected but any connection requests will be refused. same as UNAVAILABLE
	SESSION_SHUTDOWN_PENDING,
	SESSION_DISCONNECTED,
	SESSION_LISTENNING,
	SESSION_INIT,
	SESSION_DELETE_PENDING,
}	SESSION_STATE;

typedef	struct _ADAPTER_ADDRESSES_INFO
{
	unsigned int addressesLen;	
	char         PhysicalAddress[32];
} ADAPTER_ADDRESSES_INFO, *PADAPTER_ADDRESSES_INFO;

typedef struct _ADAPTER_ADDRESSES_LIST
{
	//char    serverName[OSN_MAX_HOSTNAME];  //not used
	unsigned int num;
	ADAPTER_ADDRESSES_INFO  m_AdapterAddressInfo[1];
} ADAPTER_ADDRESSES_LIST,*PADAPTER_ADDRESSES_LIST;

#endif // _OSN_COMMON_H_