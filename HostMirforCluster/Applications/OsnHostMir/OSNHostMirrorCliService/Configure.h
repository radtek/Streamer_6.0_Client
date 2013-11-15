#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__
#define  OSN_MAX_VOLUMENAME 128 

#include<assert.h>
class CQueue
{
	CQueue		*m_pNext;
	CQueue		*m_pPrevious;
	void		*m_pQItem;

public:

	CQueue()
	{
		m_pNext = NULL;
		m_pPrevious	= NULL;
		m_pQItem	= NULL;
	}

	~CQueue(){};

	bool	IsEmpty() { return m_pNext ==  NULL; }

	void *	GetItem() { return m_pQItem; }

	void	InsertQTail(void *pQItem)
	{
		CQueue *pNewItem = new CQueue();
		pNewItem->m_pQItem = pQItem;

		CQueue *pLast = this;	//header
		while(pLast->m_pNext)
			pLast = pLast->m_pNext;

		pLast->m_pNext = pNewItem;
		pNewItem->m_pPrevious = pLast;
	}

	void *	DeQueueHead()
	{
		CQueue	*pFirst = m_pNext;
		if(!pFirst)
			return NULL;

		PVOID	pObject = pFirst->m_pQItem;
		m_pNext = pFirst->m_pNext;
		if(m_pNext)
			m_pNext->m_pPrevious = this;

		delete pFirst;

		return pObject;
	}

	CQueue	*Next() { return m_pNext; }
	CQueue	*Previous() { return m_pPrevious; }
	void	Remove() 
	{
		assert(m_pPrevious);
		m_pPrevious->m_pNext = m_pNext;

		if(m_pNext)
			m_pNext->m_pPrevious = m_pPrevious;

		delete this;
	}
};

#define HCMAXIMUM_SOCKET_LISTENING       128
#define OSNRPC_HCSOCKET_CLIENT_PORT	    9997		//default OSN RPC Client port
#define	OSNRPC_HCSOCKET_SERVICE_PORT	9998		//Default OSN Rpc service port

#define	OSNRPC_HCMAX_MSG_LEN			1024*512			//Maximum message length sent by OSN Rpc service
#define OSNRPC_HCMAX_MSG				2			//Maximum outstanding messages sent by the client
#define	OSNRPC_HCMSGHEAD_LEN			sizeof(HC_MESSAGE_HEADER)
#define	OSNRPC_HCMAX_MSGPARA_LEN		OSNRPC_HCMAX_MSG_LEN - OSNRPC_HCMSGHEAD_LEN
#define	OSN_HBMAX_USERNAME		32			//Maximum length of the username, limited by windows
#define	OSN_HBMAX_PASSWORD		32			//Maximum length of the password, limited by windows


typedef struct HC_MESSAGE_HEADER
{
	BYTE           version;         //1byte 无符号 协议版本
	BYTE           parseType;       //1byte 无符号 解析类型，xml解析或其他方式
	unsigned short rtnStatus;		//2bytes无符号 命令执行成功状态 成功或失败
	unsigned short flag;		    //2bytes无符号 命令标志 同步请求/异步请求/应答
	unsigned short pad;		        //2bytes无符号 预留空间
	unsigned int cmd;               //4bytes无符号 具体命令
 	unsigned int dataLength;        //4bytes无符号 XML文件长度
} HC_MESSAGE_HEADER, *PHC_MESSAGE_HEADER;

//#define OSNRPC_HBCMD_ASK				0x01
//#define OSNRPC_HBCMD_RESPONSE			0x02
//
//#define CMD_HBSTATUS_SUCCESS			0x0000
//#define	CMD_HBSTATUS_FAILED			0x0001
//
//#define OSNRPC_CMD_NOTIFICATION_SET            0x0001
//#define OSNPRC_CMD_GETPARTNER_INFO            0x0002


//Stream6.0
#define OSN_MSGHEAD_PROTVERSION_BASIC                            0x0000         	//协议版本 基本版
#define OSN_MSGHEAD_RTNSTATUS_SUCCESS                            0x0000 	        //命令执行成功
#define OSN_MSGHEAD_RTNSTATUS_FAILED                             0x0001	            //命令执行失败
#define OSN_MSGHEAD_CMD_FLAGSYN_ASK                              0x0002	            //同步请求命令
#define OSN_MSGHEAD_CMD_FLAGASYN_ASK                             0x0003	            //异步请求命令
#define OSN_MSGHEAD_CMD_FLAG_RESPONSE                            0x0004	            //应答命令
#define OSN_MSGHEAD_PARSE_XML                                    0x0005 			//xml解析方式

#define SYS_TYPE_LINUX                                           ("0")
#define SYS_TYPE_WINDOWS                                         ("1")

enum st_res_code
{
	ST_RES_SUCCESS,
	ST_RES_FAILED,
	ST_RES_CMD_FINISHED,
};

enum st_op_code
{
	ST_OP_SCAN_CLIENT = 1,
	ST_OP_ADD_CLIENTS,
	ST_OP_DEL_CLIENTS,
	ST_OP_ADD_BACKUP,
	ST_OP_ESTABLISH_CHANNELS,
};

#define OSN_REMOTE_CMD_FINDNEWCLIENT                             0x0007             //询问客户端是否可添加
#define OSN_REMOTE_CMD_ADDCLIENT                                 0x0008             //管理端向服务器发送客户端添加请求
#define OSN_REMOTE_CMD_SETADDCLIENTMARK                          0x0009             //服务器向客户端发送添加请求
#define OSN_REMOTE_CMD_REMOVECLIENT                              0x000A             //向服务器发送删除客户端请求
#define OSN_REMOTE_CMD_SETREMOVECLIENTMARK                       0x000B             //服务器向客户端发送移除请求
#define OSN_REMOTE_CMD_GETFCCHANNELINFO                          0x000C             //管理端向服务器发送获取客户端FC通道信息请求
#define OSN_REMOTE_CMD_CREATEPROTECT                             0x000D             //管理端向服务器发送创建保护请求
#define OSN_REMOTE_CMD_RESPONSECREATEVOLUME                      0x000E             //服务器向管理端发送新建卷操作结果指令
#define OSN_REMOTE_CMD_RESPONSEMAPPINGVOLUME                     0x000F             //服务器向管理端发送映射卷操作结果指令
#define OSN_REMOTE_CMD_RESPONSECREATEMIRROR                      0x0010             //服务器向管理端发送创建镜像操作结果指令
#define OSN_REMOTE_CMD_RESPONSECREATECDP                         0x0011             //服务器向管理端发送创建快照操作结果指令
#define OSN_REMOTE_CMD_RESPONSESETCDPSCHEDULE                    0x0012             //服务器向管理端发送设置快照点计划操作结果指令
#define OSN_REMOTE_CMD_CREATEMIRROR                              0x0013             //服务器向客户端发送新建镜像操作指令
#define OSN_REMOTE_CMD_SETCDPSCHEDULE                            0x0014             //服务器向客户端发送设置快照点计划指令
#define OSN_REMOTE_CMD_INITMIRTOSERVER                           0x0015             //管理端向服务器发送初始化镜像指令
#define OSN_REMOTE_CMD_INITMIRTOCLIENT                           0x0016             //服务器向客户端发送初始化镜像指令
#define OSN_REMOTE_CMD_GETINITMIRFROMSERVER                      0x0017             //管理端向服务器发送获取镜像初始化进度百分比指令
#define OSN_REMOTE_CMD_GETINITMIRFROMCLIENT                      0x0018             //服务器向客户端发送获取镜像初始化进度百分比指令
#define OSN_REMOTE_CMD_REMOVEPROTECTTOSERVER                     0x0019             //管理端向服务器发送删除硬盘保护指令
#define OSN_REMOTE_CMD_REMOVEPROTECTTOCLIENT                     0x001A             //服务器向客户端发送获取镜像初始化进度百分比指令
#define OSN_REMOTE_CMD_GETCHANNELINFO                            0x001B             //管理端向服务器发送获取服务器通道信息请求指令
#define OSN_REMOTE_CMD_RECOVERYCDP                               0x001C             //管理端向服务器发送回复快照指令
#define OSN_REMOTE_CMD_ADDDISKTOPOOL                             0x001D             //管理端向服务器发送添加硬盘到存储池指令
#define OSN_REMOTE_CMD_REMOVEDISKFROMPOOL                        0x001E             //管理端向服务器发送从存储池删除硬盘的指令
#define OSN_REMOTE_CMD_CREATECDPPOINTTOSERVER                    0x001F             //管理端向服务器发送新建CDP点指令
#define OSN_REMOTE_CMD_CREATECDPPOINTTOCLIENT                    0x0020             //服务器向客户端发送新建CDP点指令
#define OSN_REMOTE_CMD_REMOVECDPPOINTTOSERVER                    0x0021             //管理端向服务器发送删除CDP点指令
#define OSN_REMOTE_CMD_REMOVECDPPOINTTOCLIENT                    0x0022             //服务器向客户端发送删除CDP点指令
#define OSN_REMOTE_CMD_ADDUSER                                   0x0023             //客户端向服务器发送添加用户指令
#define OSN_REMOTE_CMD_REMOVEUSER                                0x0024             //客户端向服务器发送删除用户指令
#define OSN_REMOTE_CMD_LOGINSERVER                               0x0025             //客户端向服务器发送登录服务器指令
#define OSN_REMOTE_CMD_GETINITDATA                               0x0026             //客户端向服务器发送获取初始化界面数据指令
#define OSN_REMOTE_CMD_CHANGECLIENTNAME                          0x0027             //客户端向服务器发送修改客户端名的指令
#define OSN_REMOTE_CMD_LOGOUTSERVER                              0x0028             //客户端向服务器发送用户注销登录指令
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYCLIENTID                  0x0029             //客户端向服务器发送删除指定客户端的所有报警信息指令
#define OSN_REMOTE_CMD_REMOVEOPERATIONLOGBYINDEXID               0x002A             //客户端向服务器发送删除指定操作日志信息的指令
#define OSN_REMOTE_CMD_SETCDPCHANNEL                             0x002B             //服务器向客户端发送设置快照通道指令
#define OSN_REMOTE_CMD_GETCDPLISTWITHINTIME                      0x002C             //管理端向服务器送获取指定时间段内CDP点列表指令
#define OSN_REMOTE_CMD_STOPRECOVERYCDP                           0x002D             //客户端向服务器发送中止恢复备份保护指令
#define OSN_REMOTE_CMD_REMOVEMIRROR                              0x002E             //服务器向客户端发送删除镜像的指令
#define OSN_REMOTE_CMD_REMOVEALLOPERATIONLOG                     0x002F             //客户端向服务器发送删除的所有操作日志信息指令
#define OSN_REMOTE_CMD_REFRESHCLIENTDETAILINFO                   0x0030             //服务器向客户端发送刷新客户端详细信息的请求
#define OSN_REMOTE_CMD_REFRESHCLIENTDISK                         0x0031             //客户端向服务器发送刷新客户端磁盘的请求指令
#define OSN_REMOTE_CMD_GETOPERATIONLOGLIST                       0x0032             //客户端向服务器发送获取操作日志列表的指令
#define OSN_REMOTE_CMD_REFRESHCLIENTLIST                         0x0033             //客户端向服务器发送刷新客户端列表的请求指令
#define OSN_REMOTE_CMD_GETUSERLIST                               0x0034             //客户端向服务器发送获取用户列表的指令
#define OSN_REMOTE_CMD_GETALARMLOGLIST                           0x0035             //客户端向服务器发送刷新报警日志列表的指令
#define OSN_REMOTE_CMD_TESTFCCHANNEL                             0x0036             //客户端向服务器发送测试FC通道是否能正常连接的指令
#define OSN_REMOTE_CMD_TESTISCSICHANNEL                          0x0037             //客户端向服务器发送测试ISCSI通道是否能正常连接的指令
#define OSN_REMOTE_CMD_CHANGEPASSWORD                            0x0038             //客户端向服务器发送修改用户密码的指令
#define OSN_REMOTE_CMD_CHECKPASSWORD                             0x0039             //客户端向服务器发送验证用户密码的指令
#define OSN_REMOTE_CMD_ALARMLOGREAD                              0x003A             //客户端向服务器发送报警日志已读的指令
#define OSN_REMOTE_CMD_GETCLIENTINFO                             0x003B             //管理端向服务器发送获取客户端简要信息的请求
#define OSN_REMOTE_CMD_REMOVEALLALARMLOG                         0x003D             //客户端向服务器发送删除所有报警信息指令
#define OSN_REMOTE_CMD_REFRESHCHANNELLIST                        0x003E             //客户端向服务器发送刷新客户端通道的请求
#define OSN_REMOTE_CMD_GETCDPLIST                                0x003F             //服务器向客户端发送获取CDP点列表指令
#define OSN_REMOTE_CMD_REMOVEALLALARMLOGWITHINTIME               0x0040             //客户端向服务器发送删除指定时间内的所有报警信息指令
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYCLIENTIDWITHINTIME        0x0041             //客户端向服务器发送删除指定时间段指定客户端的报警信息指令
#define OSN_REMOTE_CMD_CLIENTOFFLINE                             0x0042             //服务器向管理端发送客户端掉线的指令
#define OSN_REMOTE_CMD_CLIENTONLINE                              0x0043             //服务器向管理端发送客户端上线的指令
#define OSN_REMOTE_CMD_POOLOFFLINE                               0x0044             //服务器向管理端发送存储池下线的指令
#define OSN_REMOTE_CMD_POOLONLINE                                0x0045             //服务器向管理端发送存储池上线的指令
#define OSN_REMOTE_CMD_POOLDISKOFFLINE                           0x0046             //服务器向管理端发送存储池中磁盘下线的指令
#define OSN_REMOTE_CMD_POOLDISKONLINE                            0x0047             //服务器向管理端发送存储池中磁盘上线的指令
#define OSN_REMOTE_CMD_CLIENTDISKOFFLINETOSERVER                 0x0048             //客户端向服务器发送磁盘掉线的指令
#define OSN_REMOTE_CMD_CLIENTDISKONLINETOSERVER                  0x0049             //客户端向服务器发送磁盘上线的指令
#define OSN_REMOTE_CMD_CLIENTDISKOFFLINETOMANAGER                0x004A             //服务器向管理端发送磁盘下线的指令
#define OSN_REMOTE_CMD_CLIENTDISKONLINETOMANAGER                 0x004B             //服务器向管理端发送磁盘上线的指令
#define OSN_REMOTE_CMD_CHECKCLIENTONLINE                         0x004C             //服务器向客户端发送验证客户端在线指令
#define OSN_REMOTE_CMD_CHECKMANAGERONLINE                        0x004D             //服务器向管理端发送验证管理端在线指令
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYINDEXID                   0x004E             //客户端向服务器发送删除指定报警信息的指令
#define OSN_REMOTE_CMD_STOPCDPSCHEDULE                           0x004F             //服务器向客户端发送暂停快照计划指令

#endif // __CONFIGURE_H__