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


	bool OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET ConnectSocket);

	void				StopListenThread();
	DWORD				StartListenThread();
	DWORD				StartSocketThread();
	static DWORD		WINAPI OSNRpcListenThread(void *pData);

	inline void			SetOSNRpcState(OSNRPC_STATE inState) { m_nRPCState = inState;}
	inline OSNRPC_STATE	OSNRpcState() { return m_nRPCState; }
	inline	bool		OSNRpcServerReceiveMsg(SOCKADDR_IN	&outSin,SOCKET *ConnectSocket)
						{
							return OSNRpcReceiveMsg(outSin,
													m_sRecvMsg,
													OSNRPC_HCMAX_MSG_LEN,
													ConnectSocket);
						}

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
	
	HANDLE			m_handleThread;				//thread handle for OSN Rpc service thread
	HANDLE			m_startSocketThreadHandle;	//thread handle for start socket
	
	DWORD			m_dwThreadID;				//thread ID for OSN Rpc service thread
	DWORD			m_dwSocketThreadID;

	//stream
	COsnMirrorCopyXML *m_pCopyXML;
};


#endif // _OSNRPCSERVER_H_

