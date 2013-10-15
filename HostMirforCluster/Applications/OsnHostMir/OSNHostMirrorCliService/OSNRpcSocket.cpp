// OSNRpcSocket.cpp: implementation of the COSNRpcSocket class.
//
//////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1

#include <afxmt.h>
#include "atlbase.h"
#include <winsock2.h>

#include "Configure.h"
#include <assert.h>
//#include "..\OSNService\OSNRpcCommon.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"

/*
#include "..\common\OsnDefs.h"
#include "..\common\OSNCommon.h"
#include"..\OSNApi\IsBusApi.h"
#include "..\OSNApi\OSNHaApi.h"*/

#include "OSNService.h"


extern	COSNService			*pOSNService;

COSNRpcSocket::COSNRpcSocket()
{
	m_socket	= INVALID_SOCKET;
	m_unPort	= OSNRPC_HBSOCKET_SERVICE_PORT;
	m_nError	= 0;
	m_startSocketEventHandle =NULL;
	m_startSocketSuccess = false;
}

COSNRpcSocket::COSNRpcSocket(UINT inPort, bool inServer)
{
	m_socket	= INVALID_SOCKET;
	m_unPort	= inPort;
	m_nError	= 0;
	m_startSocketEventHandle =NULL;
	m_startSocketSuccess = false;
}

PVOID COSNRpcSocket::operator new(size_t Size)
{
	PVOID p;

	p = malloc(Size);
	ZeroMemory(p, Size);

	return  p;
}

VOID COSNRpcSocket::operator delete(PVOID p)
{
	if (p) 
	{
		free(p);
	}
	return;
}

COSNRpcSocket::~COSNRpcSocket()
{
	//close socket if socket hasn't been closed.
	OSNRpcStopSocket();

	if(m_startSocketEventHandle)
		CloseHandle(m_startSocketEventHandle);

}



//Start datagram Socket
DWORD WINAPI COSNRpcSocket::OSNRpcStartSocketThread(void *pData)
{
	//new m_socket
	COSNRpcServer *pOSNRpcServer = (COSNRpcServer *) pData;

	if (!pOSNRpcServer)
	{
		pOSNService->LogMessage("Invalid RpcServer");
		return ERROR_INVALID_PARAMETER;
	}

	pOSNRpcServer->m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (pOSNRpcServer->m_socket == INVALID_SOCKET)					//error return
	{
		pOSNRpcServer->OSNRpcSetLastError(WSAGetLastError());
		pOSNService->LogMessage("Invalid Socket");
		return ERROR_INVALID_PARAMETER;
	}

	//assign the local address/port to m_socket

	pOSNService->IncrementThreadCount();
	bool	bFailed = true;
	SOCKADDR_IN sinlocal;
	while (!pOSNRpcServer->m_startSocketSuccess)
	{
		sinlocal.sin_family			= AF_INET;			//AF_INET address family which contains IP address
		sinlocal.sin_addr.s_addr	= htonl(ADDR_ANY);
		sinlocal.sin_port			= htons((unsigned short)pOSNRpcServer->m_unPort);

		pOSNRpcServer->OSNRpcBindSocket(pOSNRpcServer->m_socket,(struct sockaddr *)&sinlocal,sizeof(sinlocal));
	/*	if (bind(pOSNRpcServer->m_socket, (struct sockaddr *)&sinlocal, sizeof(sinlocal)) == SOCKET_ERROR)					//error return
		{
			//for clients, if the port has been used, try the next available port.
			if (WSAGetLastError() == WSAEADDRINUSE && pOSNRpcServer->OSNRpcGetSocketPort() < 65535)
			{
				pOSNRpcServer->m_unPort++;
				continue;
			}
			else										//error return
			{
				pOSNRpcServer->OSNRpcSetLastError(WSAGetLastError());
				return WSAGetLastError();
			}
		}
		else		//success, break the loop and return
			break;
			*/
	}
     
	listen(pOSNRpcServer->m_socket,HBMAXIMUM_SOCKET_LISTENING);
	pOSNService->DecrementThreadCount();
	pOSNService->LogMessage("INFO: OSN HostMirrorClient socket start thread stopped.");
	return 0;
}


void COSNRpcSocket::OSNRpcBindSocket(SOCKET s,const sockaddr *name,int nameLen)
{
	if(!m_startSocketEventHandle)
	{
		Sleep(5000);
		return ;
	}

	DWORD dwWaitResult = WaitForSingleObject(m_startSocketEventHandle,300000);

	switch(dwWaitResult)
	{
		case WAIT_TIMEOUT:
		case WAIT_OBJECT_0:
			OSNRpcHandleBind(s,name,nameLen);
		case WAIT_ABANDONED:
		default:
			;
	}
}

void COSNRpcSocket::OSNRpcHandleBind(SOCKET s,const sockaddr *name,int nameLen)
{
	if(bind(s,name,nameLen) == SOCKET_ERROR)
	{
		m_startSocketSuccess = false;
		pOSNService->LogMessage("socket bind failed!");
	}
	else
	{
		m_startSocketSuccess = true;
	}
}

//shutdown/close socket. the allocated resource hasn't be free yet
bool COSNRpcSocket::OSNRpcStopSocket()
{
	if (OSNRpcIsSocketInit())
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_socket	= INVALID_SOCKET;
	}
	return true;
}

//send the message to the remote socket which defined in inSin
bool COSNRpcSocket::OSNRpcSendMsg(SOCKADDR_IN	inSin, 
										char*	inMsg, 
										int		inMsgLen,
										SOCKET ConnectSocket)
{
	m_nError    = send(ConnectSocket,inMsg,inMsgLen,0);
	if (m_nError == SOCKET_ERROR)					//error return
	{
		OSNRpcSetLastError(WSAGetLastError());
		pOSNService->LogMessage("Send message error =", m_nError);
		shutdown(ConnectSocket,SD_BOTH);
		closesocket(ConnectSocket);
		return false;
	}
	//shutdown(ConnectSocket,SD_BOTH);
	//closesocket(ConnectSocket);
	return true;
}



//recieve the message from the remote socket and the remote socket IP address/port keeps in outSin
bool COSNRpcSocket::OSNRpcReceiveMsg(SOCKADDR_IN	&outSin,
			 							char		*inMsg,
										int			inMsgLen,
									    SOCKET  *ConnectSocket)
{
	
	//clean the message buffer
	ZeroMemory(inMsg, inMsgLen);

	int		nSinLen	= sizeof(outSin);
/*	m_nError	= recvfrom(m_socket, 
							inMsg, 
							inMsgLen, 
							0, 
							(struct sockaddr*) &outSin, 
							(int *) &nSinLen); */
	*ConnectSocket=accept(m_socket,(struct sockaddr*) &outSin,(int *) &nSinLen);
	if(*ConnectSocket==INVALID_SOCKET)
	{
		OSNRpcSetLastError(WSAGetLastError());
		pOSNService->LogMessage("accept socket failed ;error =",WSAGetLastError());
		return false;
	}	
	timeval overtime;
	overtime.tv_sec=10;
	overtime.tv_usec=0;
    int Rresult=setsockopt(*ConnectSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&overtime,sizeof(timeval));
	int Sresult=setsockopt(*ConnectSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&overtime,sizeof(timeval));
	if(Sresult||Rresult)
	{
		pOSNService->LogMessage("set soket option error=",WSAGetLastError());
		shutdown(*ConnectSocket,SD_BOTH);
		closesocket(*ConnectSocket);
		return false;
	}
    m_nError=recv(*ConnectSocket,inMsg,inMsgLen,0);
    if (m_nError == SOCKET_ERROR)					//error return
	{
		OSNRpcSetLastError(WSAGetLastError());
		shutdown(*ConnectSocket,SD_BOTH);
		closesocket(*ConnectSocket);
		return false;
	}
	return true;
}




//Get the host name then call OSNRpcGetSocketAddrByName to get SOCKADDR_IN
bool COSNRpcSocket::OSNRpcGetLocalSockAddr(SOCKADDR_IN &outSin)
{
	//get the host name
	char	csLocalName[MAX_COMPUTERNAME_LENGTH+1];
	m_nError	= gethostname(csLocalName, MAX_COMPUTERNAME_LENGTH+1);
	if (m_nError == SOCKET_ERROR)					//error return
	{
		OSNRpcSetLastError(WSAGetLastError());
		return false;
	}

	//get the host information
	return OSNRpcGetSockAddrByName(csLocalName, m_unPort, outSin);
}

//retrieve the network byte order IP address by the host name or the host IP address
//assign information to SOCKADDR_IN
bool COSNRpcSocket::OSNRpcGetSockAddrByName(const char* inName, UINT inPort, SOCKADDR_IN &outSin)
{
	//retrieve the host information according to the name or IP address.
	PHOSTENT	pHostent;

	if (isalpha(inName[0]))						//using name
		pHostent	= gethostbyname(inName);
	else										//using IP address
	{
		UINT	unAddr;
		unAddr	= inet_addr(inName);
		if (unAddr == INADDR_NONE)				//error return
		{
			OSNRpcSetLastError(WSAGetLastError());
			return false;
		}

		pHostent	= gethostbyaddr((char *)&unAddr, 4, AF_INET);
	}

	if (pHostent == NULL)						//error return
	{
		OSNRpcSetLastError(WSAGetLastError());
		return false;
	}

	//assign the socket IP address/port
	outSin.sin_family	= AF_INET;
	outSin.sin_port		= htons(inPort);
	memcpy((char FAR *)&outSin.sin_addr, pHostent->h_addr, pHostent->h_length);

	return true;
}


bool COSNRpcSocket::OSNRpcIsSocketInit()
{
	if ((m_socket==INVALID_SOCKET)||(m_socket==SOCKET_ERROR)) 
		return false;
	else
		return true;
}






