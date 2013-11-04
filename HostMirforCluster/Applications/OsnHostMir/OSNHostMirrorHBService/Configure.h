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

#define HBMAXIMUM_SOCKET_LISTENING       128
#define OSNRPC_HBSOCKET_CLIENT_PORT	49185		//default OSN RPC Client port
#define	OSNRPC_HBSOCKET_SERVICE_PORT	49186		//Default OSN Rpc service port
#define OSNRPC_HBSOCKET_NEW_PORT      49187        //this port for new mirror to remote

#define	OSNRPC_HBMAX_MSG_LEN			1024*512			//Maximum message length sent by OSN Rpc service 
#define OSNRPC_HBMAX_MSG				2			//Maximum outstanding messages sent by the client
#define	OSNRPC_HBMSGHEAD_LEN			sizeof(HB_MESSAGE_HEADER)
#define	OSNRPC_HBMAX_MSGPARA_LEN		OSNRPC_HBMAX_MSG_LEN - OSNRPC_HBMSGHEAD_LEN
#define	OSN_HBMAX_USERNAME		32			//Maximum length of the username, limited by windows
#define	OSN_HBMAX_PASSWORD		32			//Maximum length of the password, limited by windows


typedef struct HB_MESSAGE_HEADER
{
	unsigned short	retStatus;						//return Status indicating result of the command execution
	char			flags;							//ASK/RESPONSE
	char			command;						//PING, CREATE_BACKUP, etc.
	unsigned short	paraNum;						//number of parameters for this command
	DWORD	        dataLength;						//parameter data buffer length
	unsigned int	timeStamp;						//The time (h*60*60+m*60+s[sec]) the message being generated
	char			initiatorName[OSN_HBMAX_USERNAME];
	char			initiatorWWN[OSN_HBMAX_PASSWORD];
} HB_MESSAGE_HEADER, *PHB_MESSAGE_HEADER;



#define OSNRPC_HBCMD_ASK				0x01
#define OSNRPC_HBCMD_RESPONSE			0x02

#define CMD_HBSTATUS_SUCCESS			0x0000
#define	CMD_HBSTATUS_FAILED			0x0001

#define OSNRPC_CMD_NOTIFICATION_SET            0x0001
#define OSNPRC_CMD_GETPARTNER_INFO             0x0002
#define OSNPRC_CMD_SET_DISK_MIRROR             0x0003
#define OSNPRC_CMD_SET_VOLUME_MIRROR           0x0004
#define OSNPRC_CMD_SET_CLUSTER_ACTIVE          0X0005
#define OSNPRC_CMD_SET_RECOVERY_BITMAP         0x0006

#endif // __CONFIGURE_H__