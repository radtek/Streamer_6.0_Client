// OSNRpcServer.h: interface for the COSNRpcServer class.

#ifndef _OSNRPCSERVER_H_
#define _OSNRPCSERVER_H_
#include "Configure.h"
#include "OsnMirrorCopyXML.h"

class COSNRpcServer : public COSNRpcSocket 
{
public:
	COSNRpcServer();
	COSNRpcServer(UINT inPort);
	virtual ~COSNRpcServer();

public:

	bool OSNRpcCmdSHUTDOWN(SOCKADDR_IN inSockAddr);
	bool OSNRpcCmdNotification(char *m_sRecvMsg);


	bool OSNRpcCmdRESET();
	bool OSNRpcCmdPing();


	bool OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET &ConnectSocket,char *pMsg);

	void				StopListenThread();
	DWORD				StartListenThread();
	DWORD				StartSocketThread();
	static DWORD		WINAPI OSNRpcListenThread(void *pData);
	static DWORD		WINAPI OSNRpcMsgThread(void *pData);

	inline void			SetOSNRpcState(OSNRPC_STATE inState) { m_nRPCState = inState;}
	inline void			SetOSNRpcMsgState(OSNRPC_STATE inState) { m_nRPCMsgState = inState;}

	inline OSNRPC_STATE	OSNRpcState() { return m_nRPCState; }
	inline OSNRPC_STATE	OSNRpcMsgState() { return m_nRPCMsgState; }

	bool		OSNRpcServerReceiveMsg(SOCKADDR_IN	&outSin,SOCKET *ConnectSocket);

	//stream
	bool OSNRpcIoctlDispatch(PHC_MESSAGE_HEADER	pMsgHeader);
	inline void  COSNRpcServer::OSNRpcRetMsgHeader(PHC_MESSAGE_HEADER pMsgHeader,unsigned short rtnStatus,BYTE parseType,unsigned short flag,unsigned int dataLength);
	DWORD OSNRpcGetBasicInfo(char *pHostname,char **pIpAddress,char *SysVersion);
	DWORD OSNRpcGetSysVersion(char *pSysVersion);
	DWORD OSNRpcGetServiceInfo(DWORD pXML);
	DWORD OSNRpcSetMirror(DWORD pXML);
	DWORD OSNRpcRemoveMirror(DWORD pXML);
	DWORD OSNRpcInitMirror(DWORD pXML);
	DWORD OSNRpcGetInitMirrorRate(DWORD pXML);
	DWORD OSNRpcGetiSCSIChannel(DWORD pXML);
	DWORD OSNRpcSetServiceInfo(DWORD pXML);
	DWORD OSNRpcGetClientInfo();

	char * OSNRpcGetIPsInfo();

private:

	char			m_sRecvMsg[OSNRPC_HCMAX_MSG_LEN];	//message buffer to recieve/send from/to the remote
	char            msg[256];  
	OSNRPC_STATE	m_nRPCState;				//stop OSN Rpc service thread flag
	OSNRPC_STATE	m_nRPCMsgState;

	HANDLE			m_handleMsg;
	HANDLE			m_handleThread;				//thread handle for OSN Rpc service thread
	HANDLE			m_startSocketThreadHandle;	//thread handle for start socket
	
	DWORD			m_dwThreadID;				//thread ID for OSN Rpc service thread
	DWORD			m_dwMsgThreadID;
	DWORD			m_dwSocketThreadID;

	//stream
	COsnMirrorCopyXML *m_pCopyXML;
	CQueue            *m_pMsgQueue;
	HANDLE            m_hMutex;
	DWORD             m_nError;          
};

class COSNMsgAccept
{
public:
	char          *pMsg;
	SOCKET	      m_SocketAccept;
	SOCKADDR_IN   m_SocketAddr;

public:
	COSNMsgAccept()
	{
		pMsg           = NULL;
		m_SocketAccept = INVALID_SOCKET;
	}
};
#endif // _OSNRPCSERVER_H_

