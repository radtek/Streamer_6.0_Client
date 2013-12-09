#ifndef _CDPSCHEDULE_H
#define _CDPSCHEDULE_H

enum RpcServiceCommand
{
	OSN_RC_CMD_GET_DISKLIST = 0x00000,   //获取Client磁盘或卷信息，输入参数为null

	OSN_RC_CMD_CREATE_BACKUPINFO = 0x00001,   //通知Client创建备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
	//(1): DesTargetName
	//(2): DesVolumeID

	OSN_RC_CMD_DELETE_BACKUPINFO = 0x00002,   //通知Client删除备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
	//(1): DesTargetName

	OSN_RC_CMD_GET_BACKUP_STATUS = 0x00003,   //获取Client备份组状态，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_INITIALIZE_BACKUP = 0x00004,   //通知Client初始化备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_CANCEL_INITI_BACKUP = 0x00005,   //通知Client取消初始化备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_GET_REPLICATE_MODE = 0x00006,   //获取Client备份组模式，输入参数为null

	OSN_RC_CMD_SET_REPLICATE_MODE = 0x00007,   //获取Client备份组模式，输入参数为(0):_REPLICATION_MODE(MODE_ASYNCH或MODE_SYNCH) 
	//(1):LogDeviceLabel (C:或D:)

	OSN_RC_CMD_GET_INITIATOR_INFO = 0x00008,   //获取Client的相关信息，输入参数为null

	OSN_RC_CMD_READ_CONFIG_FILE = 0x00009,   //通知Client重新读取配置文件，输入参数为null （客户端添加服务器时使用）

	OSN_RC_CMD_DELETE_TARGET_INFO = 0x0000A,   //通知Client删除服务器，输入参数为(0):TargetName
	//(1):TargetIP

	OSN_RC_CMD_CREATE_CDP = 0x00010,   //通知Client创建CDP，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

	OSN_RC_CMD_ADD_TARGET_INFO = 0x00011,   //通知Client添加服务器，输入参数为(0): TargetName
	//(1): TargetIPAddress
	//(2): ClientIPAddress
	OSN_RC_CMD_CREATE_CDP_SCHEDULE = 0x00012,   //通知Client添加CDP任务计划，输入参数为(0): TargetName
	//(1): TargetIPAddress
	//(2): SrcDiskSign
	//(3): CDPStartTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
	//WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
	//Second:1,2...,59;)
	//(4): CDPStopTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
	//WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
	//Second:1,2...,59;)
	//(5): CDPMode
	OSN_RC_CMD_DELETE_CDP_SCHEDULE = 0x00013,    //通知Client删除CDP任务计划，输入参数为(0): SrcDiskSign

	OSN_RC_CMD_SET_STATUSERVICEIP = 0x00014,    //通知Client修改BackupStatuThread的发送的目的地址，输入参数为（0）：StatuServiceIP
	OSN_RC_CMD_GET_RAWPARTITION = 0x00015,    //通知Client将其裸卷列表发给服务端
	OSN_RC_CMD_GET_CDP_SCHEDULE = 0x00016,    //通知Client获取CDP任务计划描述, 输入参数为(0):TargetName  (1):SrcDiskSign
	OSN_RC_CMD_CREATE_TIMECDP = 0x00017,    //通知Client创建CDP，采用IOCTL_OSNCFG_RC_TIME_MARKER的方式，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

};

enum RepServiceStatus
{
	RS_SUCCESS = 0x0,//成功
	RS_ERROR_NOT_FIND_TARGET = 0x00001 | 0x2A000000,  //失败:Client配置信息中未能找到该Target信息
	RS_ERROR_BACKUP_EXISTED = 0x00002 | 0x2A000000,  //失败:新建备份组时，Client配置信息中已存在该备份组信息
	RS_ERROR_NOT_FIND_DISK = 0x00003 | 0x2A000000,  //失败:Client中找不到相应磁盘或卷
	RS_ERROR_API_ADD_BACKUP = 0x00004 | 0x2A000000,  //失败:Client中调用OSNAddBackupInfo函数出错
	RS_ERROR_API_SET_DEVICE_ACTIVE = 0x00005 | 0x2A000000,  //失败:Client中调用OSNSetDeviceActiveBackup函数出错
	RS_ERROR_API_SET_VOLUME_ACTIVE = 0x00006 | 0x2A000000,  //失败:Client中调用OSNSetVolumeActiveBackup函数出错
	RS_ERROR_API_GET_DISK_INIT_STATUS = 0x00007 | 0x2A000000,  //失败:Client中调用OSNCheckDiskInitInfo函数出错
	RS_ERROR_API_GET_DISK_MIRROR_STATUS = 0x00008 | 0x2A000000,  //失败:Client中调用OSNGetDeviceState函数出错
	RS_ERROR_API_GET_VOLUME_INIT_STATUS = 0x00009 | 0x2A000000,  //失败:Client中调用OSNCheckVolumeInitInfo函数出错
	RS_ERROR_API_GET_VOLUME_MIRROR_STATUS = 0x0000A | 0x2A000000,  //失败:Client中调用OSNGetVolumeState函数出错
	RS_ERROR_BACKUP_NOT_EXISTED = 0x0000B | 0x2A000000,  //失败:Client配置信息中未找到此备份组信息
	RS_ERROR_API_UNSET_DEVICE_ACTIVE = 0x0000C | 0x2A000000,  //失败:Client中调用OSNUnsetDeviceActiveBackup函数出错
	RS_ERROR_API_UNSET_VOLUME_ACTIVE = 0x0000D | 0x2A000000,  //失败:Client中调用OSNUnsetVolumeActiveBackup函数出错
	RS_ERROR_API_REMOVE_BACKUP = 0x0000E | 0x2A000000,  //失败:Client中调用OSNRemoveBackupInfo函数出错
	RS_ERROR_BACKUP_DISABLED = 0x0000F | 0x2A000000,  //失败:Client中备份组为Disabled状态
	RS_ERROR_API_INIT_DISK_BACKUP = 0x00010 | 0x2A000000,  //失败:Client中调用OSNInitializeDevice函数出错
	RS_ERROR_API_INIT_VOLUME_BACKUP = 0x00011 | 0x2A000000,  //失败:Client中调用OSNInitializeVolume函数出错
	RS_ERROR_API_CANCEL_INIT_DISK_BACKUP = 0x00012 | 0x2A000000,  //失败:Client中调用OSNCancelInitializeDevice函数出错
	RS_ERROR_API_CANCEL_INIT_VOLUME_BACKUP = 0x00013 | 0x2A000000,  //失败:Client中调用OSNCancelInitializeVolume函数出错
	RS_ERROR_OPEN_REPLICATOR_REGKEY = 0x00014 | 0x2A000000,  //失败:Client中打开"SOFTWARE\\Enterprise Information Management\\OSN Replicator"注册表出错
	RS_ERROR_OPEN_OSNSERVICE_PARA_REGKEY = 0x00015 | 0x2A000000,  //失败:Client中打开"System\\CurrentControlSet\\Services\\OSNService\\Parameters"注册表出错
	RS_ERROR_SET_SAME_REPLICATION_MODE = 0x00016 | 0x2A000000,  //失败:Client中原先的备份模式和所要求的备份模式相同
	RS_ERROR_CANNOT_SET_SYNCH_MODE_BACKUP_EXISTED = 0x00017 | 0x2A000000,  //失败:Client中由于已经建立了备份组，不能由异步备份模式改为同步模式
	RS_ERROR_API_GET_LOG_VOLUME_DEVICE_NAME = 0x00018 | 0x2A000000,  //失败:Client中调用OSNGetLogVolumeDevice函数出错
	RS_ERROR_LOG_DEVICE_BACKUPED = 0x00019 | 0x2A000000,  //失败:Client中要求设置的异步备份保存卷已建立了备份组，请选择其它异步备份保存卷
	RS_ERROR_API_SET_REPLICATION_MODE = 0x0001A | 0x2A000000,  //失败:Client中调用OSNSetReplicationMode函数出错
	RS_ERROR_API_GET_SYSTEM_WWN = 0x0001B | 0x2A000000,  //失败:Client中调用OSNGetCharWWN函数出错
	RS_ERROR_NOT_FIND_CONFIG_FILE = 0x0001C | 0x2A000000,  //失败:Client目录下未找到配置文件
	RS_ERROR_FILE_TARGET_LINE_ILLEGAL_FORMAT = 0x0001D | 0x2A000000,  //失败:Client配置文件中，target记录行格式有误
	RS_ERROR_FILE_BACKUP_LINE_ILLEGAL_FORMAT = 0x0001E | 0x2A000000,  //失败:Client配置文件中，backup记录行格式有误
	RS_ERROR_DELETE_SERVER_BACKUP_EXISTED = 0x0001F | 0x2A000000,  //失败:Client中仍建立有备份组，所以不能删除服务器
	RS_ERROR_API_REMOVE_TARGET_INFO = 0x00020 | 0x2A000000,  //失败:Client中调用OSNRemoveTargetInfo函数出错
	RS_ERROR_FIND_TARGET = 0x00021 | 0x2A000000,  //失败:Client配置信息中已经存在该Target信息
	RS_ERROR_API_ADD_TARGET_INFO = 0x00022 | 0x2A000000,  //失败:Client中调用OSNAddTargetInfo函数出错
	RS_ERROR_API_GET_VOLUME_ID = 0x00023 | 0x2A000000,  //失败:Client中调用OSNGetVolumeID函数出错 
	RS_ERROR_API_GET_VOLUME_SIZE = 0x00024 | 0x2A000000,  //失败:Client中调用OSNVfGetVolumeSize函数出错 
	RS_ERROR_API_CATCH_EXCEPTION = 0x00025 | 0x2A000000,  //失败:Client中API函数调用中捕获到异常
	RS_SUCCESS_START_CREATE_CDP = 0x00026 | 0x2A000000,  //成功:Client中开始新建CDP
	RS_SUCCESS_FINISH_CREATE_CDP = 0x00027 | 0x2A000000,  //成功:Client中完成新建CDP
	RS_ERROR_NOT_FIND_RAWPARTITION = 0x00028 | 0x2A000000,  //失败：Client中没有可以作为异步保存盘的裸卷
	RS_ERROR_FILE_CREATE_CDP = 0x00029 | 0x2A000000,   //失败：Client端创建CDP失败
	RS_ERROR_SET_ASYNCH_LOG_DEVICE_BACKUP_EXISTED = 0x0002A | 0x2A000000,   //失败：Client端设置新的异步保存卷失败，由于原有异步保存卷上已存在备份组信息
	RS_ERROR_BACKUP_INITIALIZE = 0x002B | 0x2A000000,//失败备份组正在初始化
	RS_ERROR_API_GET_DISK_RECOVERY_STATUS = 0x002C | 0x2A000000,//失败：获取磁盘的恢复信息失败 
	RS_ERROR_API_GET_VOLUME_RECOVERY_STATUS = 0x002D | 0x2A000000,//失败：获取卷的恢复信息失败
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