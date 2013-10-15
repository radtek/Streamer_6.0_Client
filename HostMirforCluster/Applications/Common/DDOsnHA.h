
#ifndef _DDOsnHA_H
#define _DDOsnHA_H
#define  MAX_DEVICE_NAME_LENGTH               128
#define OSN_SerialNameLength                38
#define OSN_MAKE_IOCTL(t,c)\
        (ULONG)CTL_CODE((t), 0x800+(c), METHOD_BUFFERED, FILE_ANY_ACCESS)

#define OSNHA_DEVICE_TYPE					60009

	/*
	*   Set volume reject io 							|-------1|1
	*	Set volume  accept io							|------1-|2
	*	Get partner info									|-----1--|4
	*   Inform partner recover start 					|----1---|8
	*   suspend the recover							|---1----|16
	*	 Get bitmap buffer								|--1-----|32 // reserve
	*	 Inform partner Recover finish				|-1------|64
	*/



#define NOTIFICATION_BIT_DOWN							1	
#define NOTIFICATION_BIT_UP									2
#define NOTIFICATION_BIT_GETPARTNER					4
#define NOTIFICATION_BIT_RECOVERFINISH				8
#define NOTIFICATION_BIT_DISABLEREOCVER			16
#define NOTIFICATION_BIT_LOCAL_RECOVERFINISH	32



#ifndef  BIT_PER_BYTE
#define BIT_PER_BYTE     0x8
#endif

#define BLOCK_SIZE        512

//IOCTL between GUI and HA Manager
#define IOCTL_OSNHA_GET_DISKSTATE                        OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,100)
#define IOCTL_OSNHA_INITIALIZE                           OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,101)
#define IOCTL_OSNHA_STOP_INITIALIZE                      OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,102)
#define IOCTL_OSNHA_GET_RECOVERY_PERCENTAGE              OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,103)
#define IOCTL_OSNHA_GET_MIRROR_NOTIFICATION_LIST         OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,104)
#define IOCTL_OSNHA_REMOVE_MIRROR_NOTIFICATION           OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,105)
#define IOCTL_OSNHA_SET_DEVICE_NOTIFICATION           OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,106) 
#define IOCTL_OSNHA_CREATE_EVENT                         OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,107)
#define IOCTL_OSNHA_SIGNALED_EVENT                       OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,108)
#define IOCTL_OSNHA_CREATE_RESCAN_EVENT                  OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,109)
#define IOCTL_OSNHA_SET_INTERNAL_FLAG                    OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,115)
#define IOCTL_OSNHA_GET_HA_FLAG                      OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,116)
#define IOCTL_OSNHA_GET_PARTNERINFO                      OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,117)
#define IOCTL_OSNHA_SET_PARTNERINFO						OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,118)
#define IOCTL_OSNHA_SHUTDOWN						OSN_MAKE_IOCTL(OSNHA_DEVICE_TYPE,119)

// define the registry path
# define OSN_HA_QUEUE_SUBKEY	    L"OSNHA\\Parameters\\Queue"
# define OSN_HA_STATE_SUBKEY      L"OSNHA\\Parameters\\State"
# define OSN_HA_REJECTIO_SUBKEY   L"OSNHA\\Parameters\\RejectIO"
# define OSN_HA_PRIMARY_SUBKEY    L"OSNHA\\Parameters\\IOPrimary"
# define OSN_HA_REBOOT_SUBKEY    L"OSNHA\\Parameters\\Reboot"
# define OSN_HA_PARAMETERS        L"OSNHA\\Parameters"

// recovery flag
#define INITIALIZATION                      0x01
#define FULLRECOVERY                        0x02
#define RECOVERY                            0x04


// recovery mode
#define DISABLE                             0x0
#define RECOVERY_FROM_PRIMARY               0x01
#define RECOVERY_FROM_SECONDARY             0x02

// device test value
#define DEVICE_FINE                         0x0
#define TEST_PRIMARY_DEVICE                 0x01
#define TEST_SECONDARY_DEVICE               0x02
#define TEST_BOTH                           0x03

// define the state 
#define BOTH_HEALTHY                        0x1
#define LOCALUP_MIRRORDOWN            0x2
#define BOTH_DIRTY							  0x3
#define MIRRORUP_LOCALDOWN            0x4
#define LOCAL_RECOVERY                     0x8
#define MIRROR_RECOVERY                   0x10
#define LDTODD                                   0x20
#define MDTODD                                  0x40
#define LOCALDIRTY                             0x80
#define MIRRORDIRTY                           0x100
#define LOCAL_INIT                              0x200
#define MIRROR_INIT                            0x400


#define MAX_IRP                             0x8

typedef struct _IO_INFO
{
  ULONG  BitmapBufferoffset;
  ULONG  byteLength;

}IO_INFO,*pIO_INFO;


typedef enum HAMIRROR_STATE
{
  MIRROR_UP,
  MIRROR_DIRTY,
  MIRROR_DOWN,
  MIRROR_IN_RECOVERY,
  MIRROR_IN_FULL_RECOVERY,
  MIRROR_INITIALATION,
};

typedef enum HALOCAL_STATE
{
  LOCAL_UP,
  LOCAL_DIRTY,
  LOCAL_DOWN,
  LOCAL_IN_RECOVERY,
  LOCAL_IN_FULL_RECOVERY,
  LOCAL_INITIALAZATION,
};

typedef struct _OSNHA_INTERNAL_FLAG
{
	
	GUID  m_VolumeGuid;
	ULONG m_RecoveryFlag;      // 0= disable,1=initialize,2=fullrecovery,4=recovery
	ULONG m_DeviceTestFlag;    // 0= device fine,1=test local,2= test remote,3= test both

	ULONG m_RecoveryMode;      //0 = disable ,1= local->remote,2=remote->local
	ULONG m_DoubleDownValue;   // 0=  disable,0x20=LDTODD,0x40=MDTODD

    HALOCAL_STATE  m_LocalState;
    HAMIRROR_STATE m_MirrorState;

	ULONG m_MirrorType;
	ULONG m_DuringTime;
	ULONG m_IrpCount;

	bool		m_AcceptIOs;
	bool		m_RejectIOs;

	bool		m_NormalReboot;

}OSNHA_INTERNAL_FLAG,*POSNHA_INTERNAL_FLAG;

typedef struct _OSNHA_INTERNAL_FLAG_LIST
{
	ULONG m_Size;
    OSNHA_INTERNAL_FLAG m_Info[1];

}OSNHA_INTERNAL_FLAG_LIST,*POSNHA_INTERNAL_FLAG_LIST;

typedef struct _DISK_STATE_INFO
{
    bool         MirrorFlag;
	HALOCAL_STATE  LocalState;
    HAMIRROR_STATE MirrorState;
	ULONG   RecoveryValue;
	ULONG   RecoveryPercentage;
	bool    MirrorFound;
  
}DISK_STATE_INFO,*pDISK_STATE_INFO;


typedef struct _RERECVOERYIO
{
  ULONG blockoffset;
  ULONG blocklength;
}RERECOVERYIO,*PRECOVERYIO;


typedef struct _MIRROR_NOTIFICATION_INFO
{
	WCHAR		m_PSName[MAX_DEVICE_NAME_LENGTH];		//PS - Primary Server
	GUID			m_PVGuid;								//PV - Primary Volume
	WCHAR      m_SSName[MAX_DEVICE_NAME_LENGTH];		//SS - Secondary Server
	GUID		    m_SVGuid;								//SV - Secondary Volume
	ULONG		m_Flag;									//VOLUME_CONFIG_FLAGS_STD_MIR etc.
    

	ULONG      m_ValidBit; 
}MIRROR_NOTIFICATION_INFO,*PMIRROR_NOTIFICATION_INFO;


typedef struct _MIRROR_NOTIFICATION_LIST
{
	ULONG      m_Size;
	MIRROR_NOTIFICATION_INFO m_Info[1];
}MIRROR_NOTIFICATION_LIST,*PMIRROR_NOTIFICATION_LIST;



typedef struct _NOTIFICATION_INFO
{
	GUID m_VolumeGuid;
    ULONG m_ValidBit;
	ULONG m_HBServiceCheck;

}NOTIFICATION_INFO,*PNOTIFICATION_INFO;


typedef struct _NOTIFICATION_INFO_LIST
{
	ULONG m_Size;
	NOTIFICATION_INFO m_Info[1];
}NOTIFICATION_INFO_LIST,*PNOTIFICATION_INFO_LIST;


typedef struct _PARTNER_INFO
{

	GUID		m_VolumeGuid;
	ULONG	m_RecoverEnabled; // 0 ,recovery ,fullrecovery or init
	ULONG	m_RecoverDirection; // 0
	bool		m_HBServiceCheck;

}PARTNER_INFO,*PPARTNER_INFO;

typedef struct _PARTNER_INFO_LIST
{
	ULONG m_Size;
	PARTNER_INFO m_Info[1];
}PARTNER_INFO_LIST,*PPARTNER_INFO_LIST;

typedef struct _HACALLBACKCONTEXT
{
	LONG    IrpSuccessCount; 
	LONG		IrpCount;
	PVOID   pCOSNHAGroup;
	BOOLEAN Flag[MAX_IRP];
} HACALLBACKCONTEXT, *PHACALLBACKCONTEXT;



#endif
