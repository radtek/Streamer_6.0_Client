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

#define	OSNRPC_HBMAX_MSG_LEN			1024*512			//Maximum message length sent by OSN Rpc service 
#define OSNRPC_HBMAX_MSG				2			//Maximum outstanding messages sent by the client
#define	OSNRPC_HBMSGHEAD_LEN			sizeof(HB_MESSAGE_HEADER)
#define	OSNRPC_HBMAX_MSGPARA_LEN		OSNRPC_HBMAX_MSG_LEN - OSNRPC_HBMSGHEAD_LEN
#define	OSN_HBMAX_USERNAME		32			//Maximum length of the username, limited by windows
#define	OSN_HBMAX_PASSWORD		32			//Maximum length of the password, limited by windows


typedef struct HC_MESSAGE_HEADER
{
	unsigned short parseType;       //2bytes无符号 解析类型，xml解析或其他方式
	unsigned short rtnStatus;		//2bytes无符号 命令执行成功状态 成功或失败
	unsigned short flag;		    //2bytes无符号 命令标志 同步请求/异步请求/应答
	unsigned int cmd;               //4bytes无符号 具体命令
 	unsigned int dataLength;        //4bytes无符号 XML文件长度
} HC_MESSAGE_HEADER, *PHC_MESSAGE_HEADER;



#define OSNRPC_HBCMD_ASK				0x01
#define OSNRPC_HBCMD_RESPONSE			0x02

#define CMD_HBSTATUS_SUCCESS			0x0000
#define	CMD_HBSTATUS_FAILED			0x0001

#define OSNRPC_CMD_NOTIFICATION_SET            0x0001
#define OSNPRC_CMD_GETPARTNER_INFO            0x0002

#endif // __CONFIGURE_H__