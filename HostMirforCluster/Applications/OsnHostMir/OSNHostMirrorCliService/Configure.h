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
	BYTE           version;         //1byte �޷��� Э��汾
	BYTE           parseType;       //1byte �޷��� �������ͣ�xml������������ʽ
	unsigned short rtnStatus;		//2bytes�޷��� ����ִ�гɹ�״̬ �ɹ���ʧ��
	unsigned short flag;		    //2bytes�޷��� �����־ ͬ������/�첽����/Ӧ��
	unsigned short pad;		        //2bytes�޷��� Ԥ���ռ�
	unsigned int cmd;               //4bytes�޷��� ��������
 	unsigned int dataLength;        //4bytes�޷��� XML�ļ�����
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
#define OSN_MSGHEAD_PROTVERSION_BASIC                            0x0000         	//Э��汾 ������
#define OSN_MSGHEAD_RTNSTATUS_SUCCESS                            0x0000 	        //����ִ�гɹ�
#define OSN_MSGHEAD_RTNSTATUS_FAILED                             0x0001	            //����ִ��ʧ��
#define OSN_MSGHEAD_CMD_FLAGSYN_ASK                              0x0002	            //ͬ����������
#define OSN_MSGHEAD_CMD_FLAGASYN_ASK                             0x0003	            //�첽��������
#define OSN_MSGHEAD_CMD_FLAG_RESPONSE                            0x0004	            //Ӧ������
#define OSN_MSGHEAD_PARSE_XML                                    0x0005 			//xml������ʽ

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

#define OSN_REMOTE_CMD_FINDNEWCLIENT                             0x0007             //ѯ�ʿͻ����Ƿ�����
#define OSN_REMOTE_CMD_ADDCLIENT                                 0x0008             //���������������Ϳͻ����������
#define OSN_REMOTE_CMD_SETADDCLIENTMARK                          0x0009             //��������ͻ��˷����������
#define OSN_REMOTE_CMD_REMOVECLIENT                              0x000A             //�����������ɾ���ͻ�������
#define OSN_REMOTE_CMD_SETREMOVECLIENTMARK                       0x000B             //��������ͻ��˷����Ƴ�����
#define OSN_REMOTE_CMD_GETFCCHANNELINFO                          0x000C             //���������������ͻ�ȡ�ͻ���FCͨ����Ϣ����
#define OSN_REMOTE_CMD_CREATEPROTECT                             0x000D             //���������������ʹ�����������
#define OSN_REMOTE_CMD_RESPONSECREATEVOLUME                      0x000E             //�����������˷����½���������ָ��
#define OSN_REMOTE_CMD_RESPONSEMAPPINGVOLUME                     0x000F             //�����������˷���ӳ���������ָ��
#define OSN_REMOTE_CMD_RESPONSECREATEMIRROR                      0x0010             //�����������˷��ʹ�������������ָ��
#define OSN_REMOTE_CMD_RESPONSECREATECDP                         0x0011             //�����������˷��ʹ������ղ������ָ��
#define OSN_REMOTE_CMD_RESPONSESETCDPSCHEDULE                    0x0012             //�����������˷������ÿ��յ�ƻ��������ָ��
#define OSN_REMOTE_CMD_CREATEMIRROR                              0x0013             //��������ͻ��˷����½��������ָ��
#define OSN_REMOTE_CMD_SETCDPSCHEDULE                            0x0014             //��������ͻ��˷������ÿ��յ�ƻ�ָ��
#define OSN_REMOTE_CMD_INITMIRTOSERVER                           0x0015             //���������������ͳ�ʼ������ָ��
#define OSN_REMOTE_CMD_INITMIRTOCLIENT                           0x0016             //��������ͻ��˷��ͳ�ʼ������ָ��
#define OSN_REMOTE_CMD_GETINITMIRFROMSERVER                      0x0017             //���������������ͻ�ȡ�����ʼ�����Ȱٷֱ�ָ��
#define OSN_REMOTE_CMD_GETINITMIRFROMCLIENT                      0x0018             //��������ͻ��˷��ͻ�ȡ�����ʼ�����Ȱٷֱ�ָ��
#define OSN_REMOTE_CMD_REMOVEPROTECTTOSERVER                     0x0019             //����������������ɾ��Ӳ�̱���ָ��
#define OSN_REMOTE_CMD_REMOVEPROTECTTOCLIENT                     0x001A             //��������ͻ��˷��ͻ�ȡ�����ʼ�����Ȱٷֱ�ָ��
#define OSN_REMOTE_CMD_GETCHANNELINFO                            0x001B             //���������������ͻ�ȡ������ͨ����Ϣ����ָ��
#define OSN_REMOTE_CMD_RECOVERYCDP                               0x001C             //���������������ͻظ�����ָ��
#define OSN_REMOTE_CMD_ADDDISKTOPOOL                             0x001D             //�������������������Ӳ�̵��洢��ָ��
#define OSN_REMOTE_CMD_REMOVEDISKFROMPOOL                        0x001E             //���������������ʹӴ洢��ɾ��Ӳ�̵�ָ��
#define OSN_REMOTE_CMD_CREATECDPPOINTTOSERVER                    0x001F             //�����������������½�CDP��ָ��
#define OSN_REMOTE_CMD_CREATECDPPOINTTOCLIENT                    0x0020             //��������ͻ��˷����½�CDP��ָ��
#define OSN_REMOTE_CMD_REMOVECDPPOINTTOSERVER                    0x0021             //����������������ɾ��CDP��ָ��
#define OSN_REMOTE_CMD_REMOVECDPPOINTTOCLIENT                    0x0022             //��������ͻ��˷���ɾ��CDP��ָ��
#define OSN_REMOTE_CMD_ADDUSER                                   0x0023             //�ͻ������������������û�ָ��
#define OSN_REMOTE_CMD_REMOVEUSER                                0x0024             //�ͻ��������������ɾ���û�ָ��
#define OSN_REMOTE_CMD_LOGINSERVER                               0x0025             //�ͻ�������������͵�¼������ָ��
#define OSN_REMOTE_CMD_GETINITDATA                               0x0026             //�ͻ�������������ͻ�ȡ��ʼ����������ָ��
#define OSN_REMOTE_CMD_CHANGECLIENTNAME                          0x0027             //�ͻ���������������޸Ŀͻ�������ָ��
#define OSN_REMOTE_CMD_LOGOUTSERVER                              0x0028             //�ͻ���������������û�ע����¼ָ��
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYCLIENTID                  0x0029             //�ͻ��������������ɾ��ָ���ͻ��˵����б�����Ϣָ��
#define OSN_REMOTE_CMD_REMOVEOPERATIONLOGBYINDEXID               0x002A             //�ͻ��������������ɾ��ָ��������־��Ϣ��ָ��
#define OSN_REMOTE_CMD_SETCDPCHANNEL                             0x002B             //��������ͻ��˷������ÿ���ͨ��ָ��
#define OSN_REMOTE_CMD_GETCDPLISTWITHINTIME                      0x002C             //�������������ͻ�ȡָ��ʱ�����CDP���б�ָ��
#define OSN_REMOTE_CMD_STOPRECOVERYCDP                           0x002D             //�ͻ����������������ֹ�ָ����ݱ���ָ��
#define OSN_REMOTE_CMD_REMOVEMIRROR                              0x002E             //��������ͻ��˷���ɾ�������ָ��
#define OSN_REMOTE_CMD_REMOVEALLOPERATIONLOG                     0x002F             //�ͻ��������������ɾ�������в�����־��Ϣָ��
#define OSN_REMOTE_CMD_REFRESHCLIENTDETAILINFO                   0x0030             //��������ͻ��˷���ˢ�¿ͻ�����ϸ��Ϣ������
#define OSN_REMOTE_CMD_REFRESHCLIENTDISK                         0x0031             //�ͻ��������������ˢ�¿ͻ��˴��̵�����ָ��
#define OSN_REMOTE_CMD_GETOPERATIONLOGLIST                       0x0032             //�ͻ�������������ͻ�ȡ������־�б��ָ��
#define OSN_REMOTE_CMD_REFRESHCLIENTLIST                         0x0033             //�ͻ��������������ˢ�¿ͻ����б������ָ��
#define OSN_REMOTE_CMD_GETUSERLIST                               0x0034             //�ͻ�������������ͻ�ȡ�û��б��ָ��
#define OSN_REMOTE_CMD_GETALARMLOGLIST                           0x0035             //�ͻ��������������ˢ�±�����־�б��ָ��
#define OSN_REMOTE_CMD_TESTFCCHANNEL                             0x0036             //�ͻ�������������Ͳ���FCͨ���Ƿ����������ӵ�ָ��
#define OSN_REMOTE_CMD_TESTISCSICHANNEL                          0x0037             //�ͻ�������������Ͳ���ISCSIͨ���Ƿ����������ӵ�ָ��
#define OSN_REMOTE_CMD_CHANGEPASSWORD                            0x0038             //�ͻ���������������޸��û������ָ��
#define OSN_REMOTE_CMD_CHECKPASSWORD                             0x0039             //�ͻ����������������֤�û������ָ��
#define OSN_REMOTE_CMD_ALARMLOGREAD                              0x003A             //�ͻ�������������ͱ�����־�Ѷ���ָ��
#define OSN_REMOTE_CMD_GETCLIENTINFO                             0x003B             //���������������ͻ�ȡ�ͻ��˼�Ҫ��Ϣ������
#define OSN_REMOTE_CMD_REMOVEALLALARMLOG                         0x003D             //�ͻ��������������ɾ�����б�����Ϣָ��
#define OSN_REMOTE_CMD_REFRESHCHANNELLIST                        0x003E             //�ͻ��������������ˢ�¿ͻ���ͨ��������
#define OSN_REMOTE_CMD_GETCDPLIST                                0x003F             //��������ͻ��˷��ͻ�ȡCDP���б�ָ��
#define OSN_REMOTE_CMD_REMOVEALLALARMLOGWITHINTIME               0x0040             //�ͻ��������������ɾ��ָ��ʱ���ڵ����б�����Ϣָ��
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYCLIENTIDWITHINTIME        0x0041             //�ͻ��������������ɾ��ָ��ʱ���ָ���ͻ��˵ı�����Ϣָ��
#define OSN_REMOTE_CMD_CLIENTOFFLINE                             0x0042             //�����������˷��Ϳͻ��˵��ߵ�ָ��
#define OSN_REMOTE_CMD_CLIENTONLINE                              0x0043             //�����������˷��Ϳͻ������ߵ�ָ��
#define OSN_REMOTE_CMD_POOLOFFLINE                               0x0044             //�����������˷��ʹ洢�����ߵ�ָ��
#define OSN_REMOTE_CMD_POOLONLINE                                0x0045             //�����������˷��ʹ洢�����ߵ�ָ��
#define OSN_REMOTE_CMD_POOLDISKOFFLINE                           0x0046             //�����������˷��ʹ洢���д������ߵ�ָ��
#define OSN_REMOTE_CMD_POOLDISKONLINE                            0x0047             //�����������˷��ʹ洢���д������ߵ�ָ��
#define OSN_REMOTE_CMD_CLIENTDISKOFFLINETOSERVER                 0x0048             //�ͻ�������������ʹ��̵��ߵ�ָ��
#define OSN_REMOTE_CMD_CLIENTDISKONLINETOSERVER                  0x0049             //�ͻ�������������ʹ������ߵ�ָ��
#define OSN_REMOTE_CMD_CLIENTDISKOFFLINETOMANAGER                0x004A             //�����������˷��ʹ������ߵ�ָ��
#define OSN_REMOTE_CMD_CLIENTDISKONLINETOMANAGER                 0x004B             //�����������˷��ʹ������ߵ�ָ��
#define OSN_REMOTE_CMD_CHECKCLIENTONLINE                         0x004C             //��������ͻ��˷�����֤�ͻ�������ָ��
#define OSN_REMOTE_CMD_CHECKMANAGERONLINE                        0x004D             //�����������˷�����֤���������ָ��
#define OSN_REMOTE_CMD_REMOVEALARMLOGBYINDEXID                   0x004E             //�ͻ��������������ɾ��ָ��������Ϣ��ָ��
#define OSN_REMOTE_CMD_STOPCDPSCHEDULE                           0x004F             //��������ͻ��˷�����ͣ���ռƻ�ָ��

#endif // __CONFIGURE_H__