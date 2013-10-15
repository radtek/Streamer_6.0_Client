
#ifndef _DDOsnMIRROR_H
#define _DDOsnMIRROR_H



/*	|--ABS--|----SL---|-----SS-----|-SH IET Data--------------
	ABS: ASYNCH_BUFFER_HEADER
	SL:	 Active Segment Listed List, short?
	SS:  Segment State: In use or free, bitmap?
	SH:	 Segment Header
	IET: IO ENTRY TABLE
	Data:Segment Data	*/


#define		GLOBAL_HEADER_SIZE					1		//one block for global header
#define		SEGMENT_LIST_SIZE					512		//256K for list space, short
#define		SEGMENT_STATE_SIZE					32		//16K for state space, bitmap


#define		SEGMENT_LIST_OFFSET					GLOBAL_HEADER_SIZE
#define		SEGMENT_STATE_OFFSET				(SEGMENT_LIST_OFFSET + SEGMENT_LIST_SIZE)
#define		SEGMENT_OFFSET						(SEGMENT_STATE_OFFSET + SEGMENT_STATE_SIZE)


#define		SEGMENT_HEADER_SIZE					1		//one block for segment header
#define		SEGMENT_IOENTRY_SIZE				16		//8KB, in blocks
#define		DEFAULT_SEGMENT_DATASIZE			4096	//2MB data, in blocks

#define		ENTRY_SECTION_OFFSET_INSEGMENT		SEGMENT_HEADER_SIZE
#define		DATA_SECTION_OFFSET_INSEGMENT		(SEGMENT_HEADER_SIZE + SEGMENT_IOENTRY_SIZE)
#define		DEFAULT_SEGMENT_SIZE				(DEFAULT_SEGMENT_DATASIZE + DATA_SECTION_OFFSET_INSEGMENT)

#define		OSN_INTERNAL_IOCTL_MARKER			-1		//marker ioctl code for use in asynch handling


#define		OSNMIR_GLOBAL_IDENTIFICATION		"INFOCORE_OSNMIRROR_GLOBAL_HEADER"
typedef struct	_ASYNCH_BUFFER_HEADER
{
	ULONG			m_SequenceNumber;				//one block
	ULONG			m_SegmentListOffset;			//one block
	ULONG			m_SegmentStateOffset;			//256K? 128K segments = 256GB
	ULONG			m_SegmentOffset;				//16K? 16K*8=128K segments
	ULONG			m_SegmentSize;					//2MB by default, multipled by 2
	ULONG			m_ActiveSegmentCount;
	ULONG			m_StoredSegmentCount;
	ULONG			m_ProcessedSegmentCount;		//=m_StoredSegmentCount - m_ActiveSegmentCount
	ULONG			m_MaxSegmentCount;				//128K
	ULONG			m_OutstandingWriteCount;
	ULONG			m_ProcessedWriteCount;					
	GUID			m_PrimaryVolumeGuid;
	GUID			m_SecondaryVolumeGuid;
	GUID			m_BufferVolumeGuid;
	char			m_Reserved[28];
	char			m_Identification[36];			//OSNMIR_GLOBAL_IDENTIFICATION
} ASYNCH_BUFFER_HEADER, *PASYNCH_BUFFER_HEADER;


typedef enum SEGMENT_STATE
{
	SEGMENT_STATE_FREE,
	SEGMENT_STATE_IN_USE,
};



#define		OSNMIR_SEGMENT_IDENTIFICATION				"INFOCORE_OSNMIRROR_SEGMENT_HEADER"
typedef struct	_SEGMENT_HEADER
{
	LARGE_INTEGER		m_TimeStamp;
	ULONG				m_NumberOfIos;	
	ULONG				m_DataSize;
	ULONG				m_IoEntryOffset;			//one block for header
	ULONG				m_DataOffset;				//4 blocks? for Io Entry Table = 256 writes
	ULONG				m_SeqNum;
	ULONG				m_ProcessedIos;
	ULONG				m_ProcessedData;
	ULONG				m_SegmentIndex;
	char				m_Reserved[28];
	char				m_Identification[36];		//OSNMIR_SEGMENT_IDENTIFICATION
} SEGMENT_HEADER, *PSEGMENT_HEADER;


typedef	struct	_IO_ENTRY
{
	ULONGLONG	m_WriteOffset;		//in BLOCKs
	ULONG		m_WriteLength;		//in BLOCKs
} IO_ENTRY, *PIO_ENTRY;



typedef struct	_LUN_MIRROR_INFO
{
	REPLICATION_MODE	m_Mode;
	VOLUMEID			m_PrimaryVolume;
	VOLUMEID			m_SecondaryVolume;
	VOLUMEID			m_BufferVolume;
	BOOLEAN				m_NewAssigned;
	BOOLEAN             m_bDualPortShared;
} LUN_MIRROR_INFO, *PLUN_MIRROR_INFO;



#define OSNMIRROR_DEVICE_TYPE					60010
#define	MAX_MIRROR_IRPS							4

#endif
