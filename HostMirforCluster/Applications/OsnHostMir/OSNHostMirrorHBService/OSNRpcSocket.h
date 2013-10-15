// OSNRpcSocket.h: interface for the COSNRpcSocket class.

#ifndef	_OSNRPCSOCKET_H_
#define _OSNRPCSOCKET_H_


#define	OSN_REG_SERVER			"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\OSNHBService"
#define	OSN_REG_SERVERPARA		"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\OSNHBService\Parameters"



//socket thread state
enum OSNRPC_STATE
{
	OSNRPC_RUNNING,				//OSN Rpc socket thread is running
	OSNRPC_STOP,				//Stop OSN Rpc socket thread
	OSNRPC_STOPPED				//OSN Rpc socket thread has been stopped
};


class COSNRpcSocket  
{

public:

	COSNRpcSocket();
	COSNRpcSocket(UINT inPort, bool inServer);
	virtual ~COSNRpcSocket();

	PVOID operator new(size_t);
	VOID operator delete(PVOID);

public:
	

	bool OSNRpcGetSockAddrByName(const char*	inName, 
								UINT			inPort, 
								SOCKADDR_IN		&outSin);
	bool OSNRpcGetLocalSockAddr(SOCKADDR_IN &outSin);
	
	bool OSNRpcIsSocketInit();

	void OSNRpcBindSocket(SOCKET s,const sockaddr *name,int nameLen);

	void OSNRpcHandleBind(SOCKET s,const sockaddr *name,int nameLen);

	bool OSNRpcReceiveMsg(SOCKADDR_IN	&outSin, 
							char		*inMsg,
							int			inMsgLen,
							SOCKET *ConnectSocket);
	bool OSNRpcSendMsg(SOCKADDR_IN	inSin, 
						char*		inMsg, 
						int			inMsgLen,
					    SOCKET ConnectSocket);
	
	
	bool OSNRpcStopSocket();
    static DWORD WINAPI OSNRpcStartSocketThread(void *pData);



	inline DWORD	OSNRpcGetLastError() { return m_nError; }
	inline void		OSNRpcSetLastError(DWORD inError) { m_nError = inError; }
	inline void		OSNRpcSocketCleanup() { WSACleanup(); }
	inline DWORD	OSNRpcGetSocketPort() { return m_unPort; }
	inline void		OSNRpcSetSocketPort(DWORD inPort) { m_unPort=inPort; }



private:

	DWORD	m_unPort;		//socket port of OSN Rpc service/client
	DWORD	m_nError;		//standard WIN Socket error code
	

protected:

	SOCKET	m_socket;						//socket used in OSN Rpc server/client
	HANDLE			m_startSocketEventHandle;	//event handle for start socket
	bool			m_startSocketSuccess;		//the state of start socket success;

};



#endif // _OSNRPCSOCKET_H_