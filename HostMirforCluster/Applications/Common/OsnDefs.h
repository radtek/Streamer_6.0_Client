#ifndef		_OSNDEFS_H_
#define		_OSNDEFS_H_

#include "..\OsnApi\OsnApiCommon.h"

//instllation type
#define	OSN_INSTALLTYPE_REPLIDISK		0x01
#define OSN_INSTALLTYPE_REPLIVOL		0x02
#define OSN_INSTALLTYPE_NETDISK			0x04


// define the volume usageFlag
#define OSN_VOLUME_MAPPING               0x01
#define OSN_VOLUME_SNAPSHOT_SOURCE       0x02
#define OSN_VOLUME_SNAPSHOT_TARGET       0x04
#define OSN_VOLUME_BACKUP_DESTINATION    0x08
#define OSN_VOLUME_BACKUP_SOURCE         0x10
#define OSN_VOLUME_MIRROR_PRIMARY        0x20
#define OSN_VOLUME_MIRROR_SECONDARY      0x40
#define OSN_VOLUME_ASYNCHROUNOUS_SOURCE  0x80
#define OSN_VOLUME_GLOBAL_CDP_RESOURCE            0x100
#define OSN_VOLUME_BUSY                  0x200
#define OSN_VOLUME_NAS_LOOPBACK          0x400
#define OSN_NAS_GROUP_VOLUME             0x800

#define OSN_VOLUME_OSNMIR_SOURCE				0x1000
#define OSN_VOLUME_OSNMIR_TARGET					0x2000
#define OSN_VOLUME_OSNMIR_ASYNCH_SOURCE	0x4000
#define OSN_VOLUME_PERVOL_CDP_RESOURCE		0x8000

// define the GUILOCK flag
#define OSN_GUI_CONTROL                  0x01
#define OSN_GUI_OPEN                     0x02
#define OSN_WEB_CONTROL                  0x04//used when web is under control
#define OSN_WEB_OPEN                     0x08//used when web opened

#define	MAXIMUM_CLIENT_NUMBER			 64
#define	MAXIMUM_BACKUP_RELATIONS		 128
#define	MAXIMUM_DISK_NUMBER				 128
#define	MAXIMUM_POOL_NUMBER				 128
#define	MAXIMUM_SNAPSHOT_NUMBER			 128
#define	MAXIMUM_TARGET_PORT				 16
#define	MAXIMUM_INITIATOR_PORT			 64
#define MAXIMUM_ADAPTER_NUMBER           32




//Server type
typedef enum SERVER_TYPE
{
	OSNSOLUTION,
	DATAGUARD,
	HAEXPRESS,
	CDPSERVER,
	SANSERVER,
	STREAMER, 
	UNKNOWN,
	REMOTEGUI
};
//modified in 5.0 (Aug,2011) for >2TB support
typedef struct _REPLICATION_IO_ENTRY
{
	ULONG			m_BlockLength;
	ULONGLONG		m_BlockOffset;
} REPLICATION_IO_ENTRY, *PREPLICATION_IO_ENTRY;




#define OSN_MAX_MARKER					 16


#define REPLICATION_CACHE_ENTRY_TABLE_SIZE		1						//one block for IoEntry table
#define	REPLICATION_CACHE_SIZE					1024					//512K
#define REPLICATION_CACHE_TOTAL_SIZE			(REPLICATION_CACHE_SIZE + REPLICATION_CACHE_ENTRY_TABLE_SIZE)
#define REPLICATION_CACHE_DEFAULT_MAX_IOS		(BLOCK_SIZE / sizeof(REPLICATION_IO_ENTRY))		//one block entry table size

typedef    struct _ASYNCH_STORAGE_INFO
{
	ULONG            m_OutstandingWrites;                            //writes to be transferred
	ULONG            m_OutstandingData;                              //data to be transferred
	ULONG            m_StorageUsedPercentage; 
	ULONGLONG		  m_StorageBlockSize;							  //buffer volume size in blocks
}ASYNCH_STORAGE_INFO,*PASYNCH_STORAGE_INFO;

typedef struct _SERVER_LICENCE_INFO
{
	int  m_Client;
	int  m_Capacity;
	int  m_Features;
	int  m_Trial;
}SERVER_LICENCE_INFO,*PSERVER_LICENCE_INFO;

typedef enum MIRROR_STATE
{
	MIRROR_STATE_UP,
	MIRROR_STATE_LOCAL_DOWN,
	MIRROR_STATE_MIRROR_DOWN,
	MIRROR_STATE_IN_RECOVERY,
	MIRROR_STATE_DISABLED,
	MIRROR_STATE_BOTH_DOWN,
	MIRROR_STATE_NEW
};


typedef		enum _REPLICATION_MODE
{
	MODE_ASYNCH,					//asynchronous replication
	MODE_SYNCH,						//synchronous replication
	MODE_ASYNCH_TIMEMARKER,			//synch if link up, bitmap if dow
	MODE_NOT_REPLICATED,			//not replicated
} REPLICATION_MODE, *PREPLICATION_MODE;




typedef enum CDP_RESTORE_STATE
{
	STATE_NOT_IN_RESTORE,
	STATE_START_RESTORE,
	STATE_RESTORING,
	STATE_RESTORED,		
	STATE_ABORTED,
	STATE_RESTORING_ATBOOT
};




//VOLUME_ID is used in SnapStreamer
//typedef struct _VOLUME_ID
//{
//	ULONG			m_DiskSignature;
//	ULONG			m_VolumeBlockOffset;
//} VOLUME_ID, *PVOLUME_ID;



//VOLUMEID used in VolumeReplicator, DiskReplicator and OsnND, OsnRC.
typedef union _VOLUMEID
{
	struct
	{
		ULONGLONG		m_VolumeBlockOffset;	
		ULONG			m_DiskSignature;	
		ULONG			m_NotUsed;
		//		ULONG			m_NotUsed2;
	} MBP_VolumeID;

	struct
	{
		GUID			m_VolumeGuid;		//4 bytes
	} SAN_VolumeID;
} VOLUMEID, *PVOLUMEID;



typedef struct _VOLUME_MARKER
{
	VOLUMEID	volumeID;
	UCHAR		marker[OSN_MAX_MARKER];
} VOLUME_MARKER, *PVOLUME_MARKER;




typedef struct _NOTRANSFER_PERIOD
{
	ULONG		m_StartSystemMinute;
	ULONG		m_EndSystemMinute;
} NOTRANSFER_PERIOD, *PNOTRANSFER_PERIOD;




typedef	struct _WWN
{
	char WWNString[16];
} WWN, *PWWN;



typedef struct _CALLBACKCONTEXT
{
	PVOID		m_pDiskDevice;
	PVOID		m_OrignalIrp;
	LONG	m_IrpCount;
	VOLUMEID	m_VolumeID;
} CALLBACKCONTEXT, *PCALLBACKCONTEXT;




/////////////////////////////////////////////////////////////////////////////////
//for snapStreamer
/////////////////////////////////////////////////////////////////////////////////

#define		GLOBAL_HEADER_OFFSET			0							//global info location (blocks)
#define		UNIT_STATE_TABLE_OFFSET			1							//one block reserved for global header
#define		SNAPSHOT_DESCRIPTION_OFFSET		16							//15 blocks for unit state table
#define		OSNCDP_BITMAP_SECTION_OFFSET			256							//in blocks
#define		OSNCDP_IMAGE_SECTION_OFFSET			0X206000						//in blocks = 1024MB for bitmap+12MB

#define		OSNCDP_MAX_BITMAP_BUFFERS		2							//keep bitmap buffers in memory for latest 4 snapshots

#define		BITMAP_MAPPING_SIZE				128							//in blocks = 64K. 
#define		DEFAULT_BITMAP_SIZE				2048						//in blocks = 1MB, good for 524GB

#define		GLOBAL_INFO_SIZE				OSNCDP_BITMAP_SECTION_OFFSET
#define		DATA_OFFSET_IN_IMAGE			2048						//in blocks = 1MB
#define		IO_OFFSET_BUFFER_SIZE			576							//in blocks = 288K, 256K not enough for 1GB image size
#define		OSNCDP_MAX_SNAPSHOT_NUMBER		1024						//since we only allocate 264MB bitmap buffer
#define		MAX_SNAPSHOT_NAME_LENGTH		32

#define		DEFAULT_IMAGE_SIZE				524288						//in blocks = 1GB

#define		HASH_TABLE_SIZE					2
#define		IO_SUBBUFFER_SIZE				(IO_OFFSET_BUFFER_SIZE / HASH_TABLE_SIZE)		//288blocks=144KB


typedef struct	_IO_OFFSET_ENTRY
{
	ULONGLONG	m_IoBlockOffset;
	ULONGLONG	m_BlockOffsetOnImage;
} IO_OFFSET_ENTRY, *PIO_OFFSET_ENTRY;

typedef struct	_IO_SUBBUFFER
{
	ULONG				m_IoCount;
	IO_OFFSET_ENTRY		m_IoOffsetEntry[1];
} IO_SUBBUFFER, *PIO_SUBBUFFER;



typedef struct	_IO_BUFFER
{
	ULONG			m_CurrentOffset;		//offset from start of this image, DATA_OFFSET_IN_IMAGE
	IO_SUBBUFFER	m_IoSubBuffer[1];
} IO_BUFFER, *PIO_BUFFER;



typedef struct	_SNAPSHOTID
{
	VOLUMEID		m_VolumeID;            
	unsigned int	m_SnapshotIndex;							//0-based, first-ever snapshot of a volume is 0
	LARGE_INTEGER	m_SnapshotTime;								//system time when snapshot is executed
	CHAR			m_SnapshotName[MAX_SNAPSHOT_NAME_LENGTH];	//readable name for the snapshot
} SNAPSHOTID, *PSNAPSHOTID;


typedef enum  _CDP_SNAPSHOT_LABEL
{
	CDP_LABEL_UNDEFINED=0,//PREVIOUS VERSION
	CDP_LABEL_CREATE=1,//IOCTL_OSNCDP_CREATE_SNAPSHOT
	CDP_LABEL_MARKER=2,//IOCTL_OSNCFG_CDP_MARKER
	CDP_LABEL_MIRROR_MARKER=3,//IOCTL_OSNCFG_CDP_MARKER_MIRROR
	CDP_LABEL_HACDP_MARKER=4 //IOCTL_OSNCFG_HACDP_MARKER

}CDP_SNAPSHOT_LABEL;

typedef struct	_SNAPSHOT_INFO
{
	unsigned int	   m_DataSize;			//saved data size
	bool			   m_Loopback;			//true if loopback is created
	CDP_SNAPSHOT_LABEL m_SnapshotLabel;
} SNAPSHOT_INFO, *PSNAPSHOT_INFO;

typedef struct _SNAPSHOT_DETAIL_INFO
{
	SNAPSHOT_INFO		 m_SnapshotLoopBackInfo;
	LARGE_INTEGER		 m_SnapshotTime;
	CDP_RESTORE_STATE    m_RestoredState;
	OSNGuid              m_RestoreVolumeID;

} SNAPSHOT_DETAIL_INFO,*PSNAPSHOT_DETAIL_INFO;

typedef struct _SNAPSHOT_DETAIL_INFO_LIST
{
	unsigned int m_Size;
	SNAPSHOT_DETAIL_INFO m_SnapshotDetailInfo[1];
} SNAPSHOT_DETAIL_INFO_LIST,*PSNAPSHOT_DETAIL_INFO_LIST;





//typedef struct	_SNAPSHOT_DESCRIPTION
//{
//	SNAPSHOTID		m_SnapshotID;
//	LARGE_INTEGER	m_MasterSnapshotTime;
//	LARGE_INTEGER	m_ChildSnapshotTime;

//	UCHAR			m_VolumeLabel;					// "C", "D" etc.
//	ULONG			m_DiskIndex;					// physical disk number starting from 0
//	ULONG			m_BitmapOffset;					//in blocks on storage partition
//	ULONG			m_ImageOffset;					//in blocks on storage partition

//	ULONG			m_BitmapSize;					//blocks per bit
//	ULONG			m_BitmapCount;					//number of bits set
//} SNAPSHOT_DESCRIPTION, *PSNAPSHOT_DESCRIPTION;




typedef enum	_SNAPSHOT_SPACE_STATE
{
	NOT_USED,
	IN_USE
} SNAPSHOT_SPACE_STATE;



typedef struct	_GLOBAL_HEADER
{
	ULONG					m_BitmapSection;							//bitmap starts from here
	ULONG					m_ImageSection;								//image data starts from here
	ULONG					m_ImageCount;
	ULONG					m_SnapshotUnitSize;							//in blocks
	ULONG					m_MaxUnitNumber;							//volumeSize/UnitSize
	CHAR					m_Characteristics[32];						//text OSNSS_CHARACTERISTICS
	GUID					m_SystemGuid;								//the system GUID writing this header						
	ULONG					m_reserved[4];																
	ULONG					m_VolumeDiskIndex;
	ULONG					m_VolumeDiskSignature;
	ULONG					m_VolumeOffset;								//in blocks
	LARGE_INTEGER			m_TimeStamp;								//time when snapshot volume is set
} GLOBAL_HEADER, *PGLOBAL_HEADER;

typedef struct _RAWDISK_INFO
{
	ULONG m_DiskIndex;
	ULONG m_DiskSize;//MB
	int m_DiskSignature;
}RAWDISK_INFO,*PRAWDISK_INFO;

typedef struct _RAWDISK_INFO_LIST
{
	int count;
	RAWDISK_INFO m_RawDiskInfo[1];
}RAWDISK_INFO_LIST,*PRAWDISK_INFO_LIST;

#define			OSNSS_POLICY_MANUAL				0x00
#define			OSNSS_POLICY_PRESET_DAY			0x01
#define			OSNSS_POLICY_PRESET_WEEK		0x02
#define			OSNSS_POLICY_PRESET_BOOT		0x04
#define			OSNSS_POLICY_PRESET_HOUR		0x08
#define			OSNSS_POLICY_PRESET_5MINUTES	0x10
#define			OSNSS_POLICY_PRESET_10MINUTES	0x20
#define			OSNSS_POLICY_PRESET_30MINUTES	0x40





#define			OSNSS_CHARACTERISTICS		"INFOCORESNAPSHOTSTREAMER"
#define			OSNSS_LB_PREFIX				L"OSNSSLB_"
#define			OSNSS_LB_WITH_DEVICE_PREFIX	L"\\Device\\OSNSSLB_"

//end for snapStream




#endif		//_OSNDEFS_H_