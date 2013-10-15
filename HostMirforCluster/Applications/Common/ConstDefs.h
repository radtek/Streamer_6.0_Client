#ifndef _CONST_DEFS_H_
#define _CONST_DEFS_H_


#define		MAX_DEVICE_NAME_LENGTH					128
#define		MAC_8022_ADDRESS_SIZE_IN_STRING			18

#define		OSN_PREFIX								L"osn_"
#define		DEVICE_PREFIX							L"\\Device\\"
#define		OSNVM_DEVICE_PREFIX						L"\\Device\\osn_"
#define		OSNCFG_DEVICE_PREFIX					L"\\Device\\cfg_"
#define		OSNCFG_OSNVM_DEVICE_PREFIX				L"\\Device\\cfg_osn_"


#define		MAXIMUM_VOLUME_NUMBER					1024
#define		MAX_LUN_PER_TARGET						512
#define     MAXIMUM_MIRROR_NUMBER                   512


#define		VOLUME_TYPE_VOLUME_REPLICATION			0x01
#define		VOLUME_TYPE_NET_DISK					0x02
#define		VOLUME_TYPE_SAN_REPLICATION				0x04
#define		VOLUME_TYPE_DISK_REPLICATION			0x08


#define		VOLUME_REPLICATION_FLAG_MULTIPLE_RW		0x10000


#endif //_CONST_DEFS_H_