#ifndef _CDPSCHEDULE_H
#define _CDPSCHEDULE_H

enum RpcServiceCommand
{
	OSN_RC_CMD_GET_DISKLIST = 0x00000,   //��ȡClient���̻����Ϣ���������Ϊnull

	OSN_RC_CMD_CREATE_BACKUPINFO = 0x00001,   //֪ͨClient���������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
	//(1): DesTargetName
	//(2): DesVolumeID

	OSN_RC_CMD_DELETE_BACKUPINFO = 0x00002,   //֪ͨClientɾ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
	//(1): DesTargetName

	OSN_RC_CMD_GET_BACKUP_STATUS = 0x00003,   //��ȡClient������״̬���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_INITIALIZE_BACKUP = 0x00004,   //֪ͨClient��ʼ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_CANCEL_INITI_BACKUP = 0x00005,   //֪ͨClientȡ����ʼ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_GET_REPLICATE_MODE = 0x00006,   //��ȡClient������ģʽ���������Ϊnull

	OSN_RC_CMD_SET_REPLICATE_MODE = 0x00007,   //��ȡClient������ģʽ���������Ϊ(0):_REPLICATION_MODE(MODE_ASYNCH��MODE_SYNCH) 
	//(1):LogDeviceLabel (C:��D:)

	OSN_RC_CMD_GET_INITIATOR_INFO = 0x00008,   //��ȡClient�������Ϣ���������Ϊnull

	OSN_RC_CMD_READ_CONFIG_FILE = 0x00009,   //֪ͨClient���¶�ȡ�����ļ����������Ϊnull ���ͻ�����ӷ�����ʱʹ�ã�

	OSN_RC_CMD_DELETE_TARGET_INFO = 0x0000A,   //֪ͨClientɾ�����������������Ϊ(0):TargetName
	//(1):TargetIP

	OSN_RC_CMD_CREATE_CDP = 0x00010,   //֪ͨClient����CDP���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_ADD_TARGET_INFO = 0x00011,   //֪ͨClient��ӷ��������������Ϊ(0): TargetName
	//(1): TargetIPAddress
	//(2): ClientIPAddress
	OSN_RC_CMD_CREATE_CDP_SCHEDULE = 0x00012,   //֪ͨClient���CDP����ƻ����������Ϊ(0): TargetName
	//(1): TargetIPAddress
	//(2): SrcDiskSign
	//(3): CDPStartTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
	//WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
	//Second:1,2...,59;)
	//(4): CDPStopTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
	//WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
	//Second:1,2...,59;)
	//(5): CDPMode
	OSN_RC_CMD_DELETE_CDP_SCHEDULE = 0x00013,    //֪ͨClientɾ��CDP����ƻ����������Ϊ(0): SrcDiskSign

	OSN_RC_CMD_SET_STATUSERVICEIP = 0x00014,    //֪ͨClient�޸�BackupStatuThread�ķ��͵�Ŀ�ĵ�ַ���������Ϊ��0����StatuServiceIP
	OSN_RC_CMD_GET_RAWPARTITION = 0x00015,    //֪ͨClient��������б��������
	OSN_RC_CMD_GET_CDP_SCHEDULE = 0x00016,    //֪ͨClient��ȡCDP����ƻ�����, �������Ϊ(0):TargetName  (1):SrcDiskSign
	OSN_RC_CMD_CREATE_TIMECDP = 0x00017,    //֪ͨClient����CDP������IOCTL_OSNCFG_RC_TIME_MARKER�ķ�ʽ���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

};

enum RepServiceStatus
{
	RS_SUCCESS = 0x0,//�ɹ�
	RS_ERROR_NOT_FIND_TARGET = 0x00001 | 0x2A000000,  //ʧ��:Client������Ϣ��δ���ҵ���Target��Ϣ
	RS_ERROR_BACKUP_EXISTED = 0x00002 | 0x2A000000,  //ʧ��:�½�������ʱ��Client������Ϣ���Ѵ��ڸñ�������Ϣ
	RS_ERROR_NOT_FIND_DISK = 0x00003 | 0x2A000000,  //ʧ��:Client���Ҳ�����Ӧ���̻��
	RS_ERROR_API_ADD_BACKUP = 0x00004 | 0x2A000000,  //ʧ��:Client�е���OSNAddBackupInfo��������
	RS_ERROR_API_SET_DEVICE_ACTIVE = 0x00005 | 0x2A000000,  //ʧ��:Client�е���OSNSetDeviceActiveBackup��������
	RS_ERROR_API_SET_VOLUME_ACTIVE = 0x00006 | 0x2A000000,  //ʧ��:Client�е���OSNSetVolumeActiveBackup��������
	RS_ERROR_API_GET_DISK_INIT_STATUS = 0x00007 | 0x2A000000,  //ʧ��:Client�е���OSNCheckDiskInitInfo��������
	RS_ERROR_API_GET_DISK_MIRROR_STATUS = 0x00008 | 0x2A000000,  //ʧ��:Client�е���OSNGetDeviceState��������
	RS_ERROR_API_GET_VOLUME_INIT_STATUS = 0x00009 | 0x2A000000,  //ʧ��:Client�е���OSNCheckVolumeInitInfo��������
	RS_ERROR_API_GET_VOLUME_MIRROR_STATUS = 0x0000A | 0x2A000000,  //ʧ��:Client�е���OSNGetVolumeState��������
	RS_ERROR_BACKUP_NOT_EXISTED = 0x0000B | 0x2A000000,  //ʧ��:Client������Ϣ��δ�ҵ��˱�������Ϣ
	RS_ERROR_API_UNSET_DEVICE_ACTIVE = 0x0000C | 0x2A000000,  //ʧ��:Client�е���OSNUnsetDeviceActiveBackup��������
	RS_ERROR_API_UNSET_VOLUME_ACTIVE = 0x0000D | 0x2A000000,  //ʧ��:Client�е���OSNUnsetVolumeActiveBackup��������
	RS_ERROR_API_REMOVE_BACKUP = 0x0000E | 0x2A000000,  //ʧ��:Client�е���OSNRemoveBackupInfo��������
	RS_ERROR_BACKUP_DISABLED = 0x0000F | 0x2A000000,  //ʧ��:Client�б�����ΪDisabled״̬
	RS_ERROR_API_INIT_DISK_BACKUP = 0x00010 | 0x2A000000,  //ʧ��:Client�е���OSNInitializeDevice��������
	RS_ERROR_API_INIT_VOLUME_BACKUP = 0x00011 | 0x2A000000,  //ʧ��:Client�е���OSNInitializeVolume��������
	RS_ERROR_API_CANCEL_INIT_DISK_BACKUP = 0x00012 | 0x2A000000,  //ʧ��:Client�е���OSNCancelInitializeDevice��������
	RS_ERROR_API_CANCEL_INIT_VOLUME_BACKUP = 0x00013 | 0x2A000000,  //ʧ��:Client�е���OSNCancelInitializeVolume��������
	RS_ERROR_OPEN_REPLICATOR_REGKEY = 0x00014 | 0x2A000000,  //ʧ��:Client�д�"SOFTWARE\\Enterprise Information Management\\OSN Replicator"ע������
	RS_ERROR_OPEN_OSNSERVICE_PARA_REGKEY = 0x00015 | 0x2A000000,  //ʧ��:Client�д�"System\\CurrentControlSet\\Services\\OSNService\\Parameters"ע������
	RS_ERROR_SET_SAME_REPLICATION_MODE = 0x00016 | 0x2A000000,  //ʧ��:Client��ԭ�ȵı���ģʽ����Ҫ��ı���ģʽ��ͬ
	RS_ERROR_CANNOT_SET_SYNCH_MODE_BACKUP_EXISTED = 0x00017 | 0x2A000000,  //ʧ��:Client�������Ѿ������˱����飬�������첽����ģʽ��Ϊͬ��ģʽ
	RS_ERROR_API_GET_LOG_VOLUME_DEVICE_NAME = 0x00018 | 0x2A000000,  //ʧ��:Client�е���OSNGetLogVolumeDevice��������
	RS_ERROR_LOG_DEVICE_BACKUPED = 0x00019 | 0x2A000000,  //ʧ��:Client��Ҫ�����õ��첽���ݱ�����ѽ����˱����飬��ѡ�������첽���ݱ����
	RS_ERROR_API_SET_REPLICATION_MODE = 0x0001A | 0x2A000000,  //ʧ��:Client�е���OSNSetReplicationMode��������
	RS_ERROR_API_GET_SYSTEM_WWN = 0x0001B | 0x2A000000,  //ʧ��:Client�е���OSNGetCharWWN��������
	RS_ERROR_NOT_FIND_CONFIG_FILE = 0x0001C | 0x2A000000,  //ʧ��:ClientĿ¼��δ�ҵ������ļ�
	RS_ERROR_FILE_TARGET_LINE_ILLEGAL_FORMAT = 0x0001D | 0x2A000000,  //ʧ��:Client�����ļ��У�target��¼�и�ʽ����
	RS_ERROR_FILE_BACKUP_LINE_ILLEGAL_FORMAT = 0x0001E | 0x2A000000,  //ʧ��:Client�����ļ��У�backup��¼�и�ʽ����
	RS_ERROR_DELETE_SERVER_BACKUP_EXISTED = 0x0001F | 0x2A000000,  //ʧ��:Client���Խ����б����飬���Բ���ɾ��������
	RS_ERROR_API_REMOVE_TARGET_INFO = 0x00020 | 0x2A000000,  //ʧ��:Client�е���OSNRemoveTargetInfo��������
	RS_ERROR_FIND_TARGET = 0x00021 | 0x2A000000,  //ʧ��:Client������Ϣ���Ѿ����ڸ�Target��Ϣ
	RS_ERROR_API_ADD_TARGET_INFO = 0x00022 | 0x2A000000,  //ʧ��:Client�е���OSNAddTargetInfo��������
	RS_ERROR_API_GET_VOLUME_ID = 0x00023 | 0x2A000000,  //ʧ��:Client�е���OSNGetVolumeID�������� 
	RS_ERROR_API_GET_VOLUME_SIZE = 0x00024 | 0x2A000000,  //ʧ��:Client�е���OSNVfGetVolumeSize�������� 
	RS_ERROR_API_CATCH_EXCEPTION = 0x00025 | 0x2A000000,  //ʧ��:Client��API���������в����쳣
	RS_SUCCESS_START_CREATE_CDP = 0x00026 | 0x2A000000,  //�ɹ�:Client�п�ʼ�½�CDP
	RS_SUCCESS_FINISH_CREATE_CDP = 0x00027 | 0x2A000000,  //�ɹ�:Client������½�CDP
	RS_ERROR_NOT_FIND_RAWPARTITION = 0x00028 | 0x2A000000,  //ʧ�ܣ�Client��û�п�����Ϊ�첽�����̵����
	RS_ERROR_FILE_CREATE_CDP = 0x00029 | 0x2A000000,   //ʧ�ܣ�Client�˴���CDPʧ��
	RS_ERROR_SET_ASYNCH_LOG_DEVICE_BACKUP_EXISTED = 0x0002A | 0x2A000000,   //ʧ�ܣ�Client�������µ��첽�����ʧ�ܣ�����ԭ���첽��������Ѵ��ڱ�������Ϣ
	RS_ERROR_BACKUP_INITIALIZE = 0x002B | 0x2A000000,//ʧ�ܱ��������ڳ�ʼ��
	RS_ERROR_API_GET_DISK_RECOVERY_STATUS = 0x002C | 0x2A000000,//ʧ�ܣ���ȡ���̵Ļָ���Ϣʧ�� 
	RS_ERROR_API_GET_VOLUME_RECOVERY_STATUS = 0x002D | 0x2A000000,//ʧ�ܣ���ȡ��Ļָ���Ϣʧ��
};

enum ScheduleMode
{
	MODE_MINUTES,
	MODE_DAYLY,
	MODE_WEEKLY,
	MODE_MONTHLY,
	//MODE_NONE,
};

enum WeekBase
{
	WEEKBASE_FIRST = 1,
	WEEKBASE_SECOND = 2,
	WEEKBASE_THIRD = 3,
	WEEKBASE_FOURTH = 4,
	WEEKBASE_LAST = 5,
	WEEKBASE_NONE = 6
};

enum DayOfWeek
{
	Sunday,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
};

enum CDP_SCHEDULE_MODE
{
	MODE_MONTH = 0,
	MODE_WEEK = 1,
	MODE_DAY = 2,
	MODE_MINUTE = 3,
	//MODE_NONE = 4,
};

enum CDPSET
{
	ADD = 0,
	DEL = 1,
	QUERY = 2,
};

class WeekModifier
{
public:
	WeekBase Base;
	vector<DayOfWeek> Days;

public: 
	WeekModifier()
	{
		Base = WEEKBASE_NONE;
	}
};

class ScheduleModifier
{
public:
	unsigned int IntervalDays;//1~365
	unsigned int IntervalMinutes;//1~1440
	WeekModifier Week;
};

class Date
{
public:
	unsigned int Year;//1~...
	unsigned int Month;//1~12
	unsigned int Day;//1~31

public:
	Date()
	{
		Year = 0;
		Month = 0;
		Day = 0;
	}

	Date(unsigned int year, unsigned int month, unsigned int day)
	{
		Year = year;
		Month = month;
		Day = day;
	}
};

class Time
{
public:
	unsigned int Hour;          //1~24
	unsigned int Minute;        //1~59
	unsigned int Sencond;       //1~59

public:
	Time()
	{
		Hour = 0;
		Minute = 0;
		Sencond = 0;
	}

	Time(unsigned int hour, unsigned int minute)
	{
		Hour = hour;
		Minute = minute;
		Sencond = 0;
	}
};

class CDPSchedule
{
public:
	ScheduleMode     Mode;
	Date             StartDates;
	Time             StartTime;
	Date             EndDates;
	Time             EndTime;
	ScheduleModifier Modifier;
};

class CRetMsgInfo
{
public: 
	RepServiceStatus m_RepServiceStatus;
	string           m_RetMsgInfo;
	unsigned int     m_ErrorCode;

public:
	CRetMsgInfo(){ }
	CRetMsgInfo(RepServiceStatus status, unsigned int iErrorCode, string strMsg)
	{
		m_RepServiceStatus = status;
		m_ErrorCode = iErrorCode;
		m_RetMsgInfo = strMsg;
	}

};

#endif