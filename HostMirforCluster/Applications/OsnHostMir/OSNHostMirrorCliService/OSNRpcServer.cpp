// OSNRpcServer.cpp: implementation of the COSNRpcServer class.
//
//////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1

#include <afxmt.h>
#include "atlbase.h"
#include <winsock2.h>
#include "Configure.h"

#include <assert.h>
#include "..\OsnVolumeCopyApi\Configure.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"

#include "OSNService.h"
#include <Iphlpapi.h>
#include <iostream>
#include "Common.h"
# pragma comment(lib, "Iphlpapi.lib")

extern	COSNService			*pOSNService;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COSNRpcServer::COSNRpcServer() : COSNRpcSocket()
{
	m_nRPCState			 = OSNRPC_STOPPED;
	m_nRPCMsgState		 = OSNRPC_STOPPED;
	m_handleThread		 = INVALID_HANDLE_VALUE;
	m_handleMsg          = INVALID_HANDLE_VALUE;
	m_dwThreadID		 = 0;
	m_dwMsgThreadID      = 0;
	m_dwSendThreadID     = 0;
	m_dwSocketThreadID   = 0;
	m_nError             = 0;
	m_startSocketThreadHandle = INVALID_HANDLE_VALUE;                 
	m_pCopyXML           = NULL;
	m_pMsgQueue          = NULL;
	m_hMutex             = NULL;
	m_hMutexFreshClient  = NULL;
	m_HCSendThreadHandle = INVALID_HANDLE_VALUE;

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HCMAX_MSG_LEN);

}

COSNRpcServer::COSNRpcServer(UINT inPort) : COSNRpcSocket(inPort, true)
{
	m_nRPCState			 = OSNRPC_STOPPED;
	m_nRPCMsgState		 = OSNRPC_STOPPED;
	m_handleThread		 = INVALID_HANDLE_VALUE;
	m_handleMsg          = INVALID_HANDLE_VALUE;
	m_dwThreadID		 = 0;
	m_dwMsgThreadID      = 0;
	m_dwSendThreadID     = 0;
	m_dwSocketThreadID   = 0;
	m_nError             = 0;
	m_startSocketThreadHandle = INVALID_HANDLE_VALUE;
	m_HCSendThreadHandle = INVALID_HANDLE_VALUE;
	m_pCopyXML           = NULL;
	m_pMsgQueue          = NULL;
	m_hMutex             = NULL;
	m_hMutexFreshClient  = NULL;

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HCMAX_MSG_LEN);
}

COSNRpcServer::~COSNRpcServer()
{
	if(m_pCopyXML != NULL)
	{
		delete(m_pCopyXML);
	}

	if(m_pMsgQueue != NULL)
	{
		while(m_pMsgQueue->Next())
		{
			WaitForSingleObject(m_hMutex,INFINITE);
			COSNMsgAccept *pObject = (COSNMsgAccept *)m_pMsgQueue->Next()->DeQueueHead();
			ReleaseMutex(m_hMutex);

			if(pObject->m_SocketAccept != INVALID_SOCKET)
			{
				shutdown(pObject->m_SocketAccept,SD_BOTH);
				closesocket(pObject->m_SocketAccept);
			}
			if(pObject->pMsg == NULL)
			{
				delete [] pObject->pMsg;
			}
			delete(pObject);
		}
		delete(m_pMsgQueue);
	}

	if(m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
	}

	if(m_hMutexFreshClient != NULL)
	{
		CloseHandle(m_hMutexFreshClient);
	}

	if(m_handleMsg != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handleMsg);
		m_handleMsg = INVALID_HANDLE_VALUE;
	}

	if (m_handleThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handleThread);
		m_handleThread = INVALID_HANDLE_VALUE;
	}

	if(m_HCSendThreadHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_HCSendThreadHandle);
		m_HCSendThreadHandle = INVALID_HANDLE_VALUE;
	}

	if(m_startSocketThreadHandle !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_startSocketThreadHandle);
		m_startSocketThreadHandle = INVALID_HANDLE_VALUE;
	}
}

DWORD WINAPI COSNRpcServer::OSNRpcMsgThread(void *pData)	
{
	LARGE_INTEGER m_liPerfFreq={0};
	QueryPerformanceFrequency(&m_liPerfFreq); //获取每秒多少CPU Performance Tick

	//CoInitialize(NULL);
	HRESULT hres;
	hres =  CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hres))
	{
		LOG(INFO) << "CoInitializeEx error";
		return EXIT_FAILURE;              // Program has failed.
	}
	// 设置进程安全级别
	hres =  CoInitializeSecurity(NULL,
		-1,      // COM negotiates service                 
		NULL,    // Authentication services
		NULL,    // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
		RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
		NULL,             // Authentication info
		EOAC_NONE,        // Additional capabilities
		NULL              // Reserved
		);            
	if (FAILED(hres))
	{
		LOG(INFO) << "CoInitializeSecurity error";
		//CoUninitialize();
		return EXIT_FAILURE;          // Program has failed.
	}

	COSNRpcServer *pOSNRpcServer = (COSNRpcServer *) pData;

	pOSNService->IncrementThreadCount();

	pOSNRpcServer->m_pCopyXML           = new COsnMirrorCopyXML();

	LARGE_INTEGER m_liPerfFront={0};
	LARGE_INTEGER m_liPerfNext={0};
	QueryPerformanceCounter(&m_liPerfFront);

	while (pOSNRpcServer->OSNRpcMsgState() == OSNRPC_RUNNING)
	{
		//fresh Client infomation 60per.
		QueryPerformanceCounter(&m_liPerfNext);
		if((((m_liPerfNext.QuadPart - m_liPerfFront.QuadPart) * 1000)/m_liPerfFreq.QuadPart) > 60000)
		{
			WaitForSingleObject(pOSNRpcServer->m_hMutexFreshClient,INFINITE);
			pOSNRpcServer->m_pCopyXML->InitializeMembers();
			ReleaseMutex(pOSNRpcServer->m_hMutexFreshClient);

			m_liPerfFront.QuadPart = m_liPerfNext.QuadPart;
		}

		if(pOSNRpcServer->m_pMsgQueue->Next() == NULL)
		{
			Sleep(200);
			continue;
		}

		COSNMsgAccept *pCOSNMsgAccept = NULL;
		WaitForSingleObject(pOSNRpcServer->m_hMutex,INFINITE);
		pCOSNMsgAccept = (COSNMsgAccept *)pOSNRpcServer->m_pMsgQueue->DeQueueHead();
		ReleaseMutex(pOSNRpcServer->m_hMutex);
		if(pCOSNMsgAccept == NULL)
		{
			Sleep(200);
			continue;
		}

		//process the income command
		pOSNRpcServer->OSNRpcProcessRequest(pCOSNMsgAccept->m_SocketAddr,pCOSNMsgAccept->m_SocketAccept,pCOSNMsgAccept->pMsg);

		shutdown(pCOSNMsgAccept->m_SocketAccept,SD_BOTH);
		closesocket(pCOSNMsgAccept->m_SocketAccept);
		delete [] pCOSNMsgAccept->pMsg;
		delete pCOSNMsgAccept;
	}

	pOSNService->DecrementThreadCount();
	pOSNService->LogMessage("INFO: OSN HostMirrorClient Msg thread stopped.");

	CoUninitialize();
	pOSNRpcServer->SetOSNRpcMsgState(OSNRPC_STOPPED);

	return 0;
}

bool COSNRpcServer::OSNRpcServerReceiveMsg(SOCKADDR_IN	&outSin,SOCKET *ConnectSocket)
{
	PHC_MESSAGE_HEADER pHeader;

	int		nSinLen	= sizeof(outSin);
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

	COSNMsgAccept *pCOSNMsgAccept = NULL;
	char *inMsg = NULL;
	try
	{
		pCOSNMsgAccept = new COSNMsgAccept();
		inMsg = new char[OSNRPC_HCMAX_MSG_LEN];
		if(inMsg == NULL)
		{
			shutdown(*ConnectSocket,SD_BOTH);
			closesocket(*ConnectSocket);
			return false;
		}
		ZeroMemory(inMsg, OSNRPC_HCMAX_MSG_LEN);
	}
	catch(exception err)
	{
		if(pCOSNMsgAccept != NULL)
		{
			delete(pCOSNMsgAccept);
		}
		printf("new memery error:%s",err.what());
		shutdown(*ConnectSocket,SD_BOTH);
		closesocket(*ConnectSocket);
		return false;
	}

	m_nError=recv(*ConnectSocket,inMsg,sizeof(HC_MESSAGE_HEADER),0);
	if (m_nError == SOCKET_ERROR)					//error return
	{
		OSNRpcSetLastError(WSAGetLastError());
		shutdown(*ConnectSocket,SD_BOTH);
		closesocket(*ConnectSocket);
		delete(inMsg);
		return false;
	}
	pHeader = (PHC_MESSAGE_HEADER)inMsg;
	if(pHeader->dataLength != 0)
	{
		m_nError=recv(*ConnectSocket,(char *)((DWORD)pHeader+OSNRPC_HCMSGHEAD_LEN),pHeader->dataLength,0);
		if (m_nError == SOCKET_ERROR)					//error return
		{
			OSNRpcSetLastError(WSAGetLastError());
			shutdown(*ConnectSocket,SD_BOTH);
			closesocket(*ConnectSocket);
			delete(inMsg);
			return false;
		}
	}

	pCOSNMsgAccept->pMsg = inMsg;
	pCOSNMsgAccept->m_SocketAccept = *ConnectSocket;
	pCOSNMsgAccept->m_SocketAddr   = outSin;

	WaitForSingleObject(m_hMutex,INFINITE);
	m_pMsgQueue->InsertQTail(pCOSNMsgAccept);
	ReleaseMutex(m_hMutex);
	return true;
}

//OSN Rpc service thread function
DWORD WINAPI COSNRpcServer::OSNRpcListenThread(void *pData)	
{
	COSNRpcServer *pOSNRpcServer = (COSNRpcServer *) pData;

	if (!pOSNRpcServer)
	{
		pOSNService->LogMessage("Invalid RpcServer");
		return ERROR_INVALID_PARAMETER;
	}

	if (pOSNRpcServer->m_socket == INVALID_SOCKET)
	{
		pOSNService->LogMessage("Invalid Socket");
		return ERROR_INVALID_PARAMETER;
	}
	pOSNService->IncrementThreadCount();
	//loop to wait the income message
	SOCKADDR_IN	sinAccept;							//receive the income address information
	SOCKET   sockConn;   
	
	pOSNRpcServer->m_pMsgQueue          = new CQueue();
	while (pOSNRpcServer->OSNRpcState() == OSNRPC_RUNNING)
	{
		if(!pOSNRpcServer->m_startSocketSuccess)
		{
			pOSNService->LogMessage("socket hav'not Bind");
			Sleep(30000);
			continue;
		}
		//recieve message failed, go to the next loop, continue waiting
		if (pOSNRpcServer->OSNRpcServerReceiveMsg(sinAccept,&sockConn))
		{
			
		}

		//if it is stopping OSN Rpc service command, break the loop
		if (pOSNRpcServer->OSNRpcState() == OSNRPC_STOP)
		{
			break;
		}
		else
		{
			Sleep(200);
		}

	}

	pOSNService->DecrementThreadCount();
	pOSNService->LogMessage("INFO: OSN HostMirrorClient Listen thread stopped.");

	CoUninitialize();
	pOSNRpcServer->SetOSNRpcState(OSNRPC_STOPPED);
	return 0;			//thread function return
}

//socket start thread()
DWORD COSNRpcServer::StartSocketThread()
{
	WSADATA wdata;
	if(WSAStartup(MAKEWORD(2,2),&wdata))
		return WSAGetLastError();

	m_startSocketEventHandle = CreateEvent(NULL,false,false,"StartSocketEvent");

	m_startSocketThreadHandle = CreateThread(NULL,
		0,
		OSNRpcStartSocketThread,
		(void *)this,
		0,
		&m_dwSocketThreadID);

	if(m_startSocketThreadHandle == INVALID_HANDLE_VALUE)
	{
		return OSNRpcGetLastError();
	}

	::SetEvent(m_startSocketEventHandle);
	Sleep(500);

	return ERROR_SUCCESS;
}

DWORD COSNRpcServer::OSNRpcHeartBeat(char *pBuffer,unsigned int Length)
{
	char pIPAddress[21];
	PHC_MESSAGE_HEADER pHeader;
	m_pCopyXML->MoveNext(m_pCopyXML->m_ServerIP,pIPAddress,sizeof(pIPAddress),' ');
	WSADATA wsa;
	
	if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
	{
		LOG(ERROR)<<"HeartBeat WSAStartup error.";
		return -1;
	}
	
	SOCKET sock;
	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET){
		LOG(ERROR)<<"HeartBeat socket error.";
		WSACleanup();
		return -1;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0,sizeof(sockaddr_in));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr(pIPAddress);
	serverAddress.sin_port = htons(OSN_SERVER_LISTENING);
	
	connect(sock,(sockaddr*)&serverAddress,sizeof(serverAddress));
	if(WSAGetLastError() !=0)
	{
		LOG(ERROR)<<"HeartBeat connect error:"<<WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	int a = send(sock,pBuffer,Length,0);
	if(WSAGetLastError() != 0)
	{
		LOG(ERROR)<< "send() error:" << WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	memset(pBuffer,0,Length);

	timeval overtime;
	overtime.tv_sec=5;
	overtime.tv_usec=0;
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&overtime,sizeof(overtime));
	if(WSAGetLastError() != 0)
	{
		LOG(ERROR)<< "setsockopt() error:" <<WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	int bytes = recv(sock,pBuffer,sizeof(HC_MESSAGE_HEADER),0);

	pHeader = (PHC_MESSAGE_HEADER)pBuffer;

	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&overtime,sizeof(overtime));
	if(WSAGetLastError() != 0)
	{
		LOG(ERROR)<< "setsockopt() error:" <<WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	if(pHeader->dataLength != 0)
	{
		bytes = recv(sock,(char*)((DWORD)pHeader + OSNRPC_HCMSGHEAD_LEN),pHeader->dataLength,0);
		if(WSAGetLastError() != 0)
		{
			int a = WSAGetLastError();
			shutdown(sock,SD_BOTH);
			closesocket(sock);
			WSACleanup();
			LOG(ERROR)<<"recv() error:" << WSAGetLastError();
			return -1;
		}
	}
	
	PHC_MESSAGE_HEADER	pMsgHeader = (PHC_MESSAGE_HEADER)pBuffer;
	if(pMsgHeader->cmd == ST_OP_CLIENT_HEARTBEAT)
	{
		char *pXMLText = (char*)((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
		DWORD dw = OSNRpcGetServiceInfoSend((DWORD)pXMLText);
	}
		
	shutdown(sock,SD_BOTH);
	closesocket(sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI COSNRpcServer:: OSNHCSendThread(void *pData)
{
	//CoInitialize(NULL);
	HRESULT hres;
	hres =  CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hres))
	{
		LOG(INFO) << "CoInitializeEx error";
		return EXIT_FAILURE;              // Program has failed.
	}

	COSNRpcServer *pOSNRpcServer = (COSNRpcServer *) pData;

	char *pMsg = NULL;
	pMsg = new char[OSNRPC_HCMAX_MSG_LEN];
	
	pOSNService->IncrementThreadCount();

	while (pOSNRpcServer->OSNRpcSendState() == OSNRPC_RUNNING)
	{
		Sleep(5000);

		memset(pMsg,0,OSNRPC_HCMAX_MSG_LEN);

		PHC_MESSAGE_HEADER	pMsgHeader = (PHC_MESSAGE_HEADER)pMsg;
		pMsgHeader->version = OSN_MSGHEAD_PROTVERSION_BASIC;
		pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;
		pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_SUCCESS;
		pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAGSYN_ASK ;
		pMsgHeader->cmd = ST_OP_CLIENT_HEARTBEAT;
		char *pXMLText = (char*)((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);

		if(pOSNRpcServer->m_pCopyXML != NULL)
		{
			LOG(INFO) << "";

			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			if(pOSNRpcServer->m_pCopyXML->m_IsProtected == true)
			{
				WaitForSingleObject(pOSNRpcServer->m_hMutexFreshClient,INFINITE);
				DWORD dw = pOSNRpcServer->OSNRpcGetClientInfo(pXMLTemp);
				ReleaseMutex(pOSNRpcServer->m_hMutexFreshClient);

				if(EXIT_SUCCESS == dw)
				{

					DWORD size = pXMLTemp->GetXMLText(pXMLText);
					pMsgHeader->dataLength = size;

					pOSNRpcServer->OSNRpcHeartBeat(pMsg,sizeof(HC_MESSAGE_HEADER)+size);
				}
				else
				{
					LOG(INFO) << "Heartbeat OSNRpcGetClientInfo failed!";
				}
			}
			delete(pXMLTemp);
		}
		else
			LOG(INFO) << "pOSNRpcServer->m_pCopyXML == NULL.";
	}

	if(pMsg != NULL)
	{
		delete [] pMsg;
	}
	CoUninitialize();
	pOSNService->DecrementThreadCount();
	pOSNRpcServer->SetOSNRpcSendState(OSNRPC_STOPPED);

	pOSNService->LogMessage("INFO: HC Send thread stopped");
	return 0;			//thread function return
}

DWORD COSNRpcServer::StartMsgThread()
{
	//HostMirror Client handle cmd thread
	m_handleMsg = CreateThread(NULL,
		0,
		OSNRpcMsgThread,
		(void *) this,
		0,
		&m_dwMsgThreadID);
	if(m_handleMsg == INVALID_HANDLE_VALUE)			//error return 
	{
		SetOSNRpcMsgState(OSNRPC_STOPPED);
		return GetLastError();
	}
	SetOSNRpcMsgState(OSNRPC_RUNNING);
	return ERROR_SUCCESS;
}

DWORD COSNRpcServer::StartSendThread()
{
	//HostMirror Client beart thread
	m_HCSendThreadHandle = CreateThread(NULL,
		0,
		OSNHCSendThread,
		(void *) this,
		0,
		&m_dwSendThreadID);
	if(m_HCSendThreadHandle == INVALID_HANDLE_VALUE)  //error return 
	{
		SetOSNRpcSendState(OSNRPC_STOPPED);
		return GetLastError();
	}
	SetOSNRpcSendState(OSNRPC_RUNNING);
	return ERROR_SUCCESS;
}

DWORD COSNRpcServer::StartListenThread()
{
	//HostMirror Client receive cmd thread
	m_handleThread = CreateThread(NULL,
		0,
		OSNRpcListenThread,
		(void *) this,
		0,
		&m_dwThreadID);
	if(m_handleThread == INVALID_HANDLE_VALUE)			//error return 
	{
		OSNRpcStopSocket();
		OSNRpcSocketCleanup();

		SetOSNRpcState(OSNRPC_STOPPED);
		return GetLastError();
	}
	SetOSNRpcState(OSNRPC_RUNNING);

	return ERROR_SUCCESS;
}

void COSNRpcServer::StopSendThread()
{
	SetOSNRpcSendState(OSNRPC_STOP);
	CloseHandle(m_HCSendThreadHandle);
	m_HCSendThreadHandle	= INVALID_HANDLE_VALUE;
}

void COSNRpcServer::StopMsgThread()
{
	SetOSNRpcMsgState(OSNRPC_STOP);
	CloseHandle(m_handleMsg);
	m_handleMsg	= INVALID_HANDLE_VALUE;
}

void COSNRpcServer::StopListenThread()
{

	SetOSNRpcState(OSNRPC_STOP);

	//stop the socket, cleanup resource
	OSNRpcStopSocket();
	OSNRpcSocketCleanup();

	//clean up thead handle

	CloseHandle(m_handleThread);
	m_handleThread	= INVALID_HANDLE_VALUE;
}

DWORD COSNRpcServer::OSNRpcGetSysVersion(char *pSysVersion)
{
	OSVERSIONINFOEX osver;  
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX); 
	if (!GetVersionEx((LPOSVERSIONINFOA)&osver))  
	{  
		return EXIT_FAILURE;  
	}
	switch(osver.dwMajorVersion)
	{
	case 5:
		{
			switch(osver.dwMinorVersion)
			{
			case 0:
				{
					strcpy(pSysVersion,"Windows 2000");
				}
				break;
			case 1:
				{
					strcpy(pSysVersion,"Windows XP");
				}
				break;
			case 2:
				{
					strcpy(pSysVersion,"Windows Server 2003");
				}
				break;
			}
		}
		break;
	case 6:
		{
			switch(osver.dwMinorVersion)
			{
			case 0:
				{
					if(osver.wProductType != VER_NT_WORKSTATION)
					{
						strcpy(pSysVersion,"Windows Server 2008");
					}
					else
					{
						strcpy(pSysVersion,"Windows Vista");
					}
				}
				break;
			case 1:
				{
					if(osver.wProductType == VER_NT_WORKSTATION)
					{
						strcpy(pSysVersion,"Windows 7");
					}
					else
					{
						strcpy(pSysVersion,"Windows Server 2008 R2");
					}
				}
				break;
			case 2:
				{
					if(osver.wProductType == VER_NT_WORKSTATION)
					{
						strcpy(pSysVersion,"Windows 8");
					}
					else
					{
						strcpy(pSysVersion,"Windows Server 2012");
					}
				}
				break;
			case 3:
				{
					if(osver.wProductType == VER_NT_WORKSTATION)
					{
						strcpy(pSysVersion,"Windows 8.1 Preview");
					}
					else
					{
						strcpy(pSysVersion,"Windows Server 2012 R2 Preview");
					}
				}
				break;
			}
		}
		break;
	default:
		{
			strcpy(pSysVersion,"Unknow System Operating Version");
		}
		break;
	}

	return EXIT_SUCCESS;
}

char * COSNRpcServer::OSNRpcGetIPsInfo()
{
	PIP_ADAPTER_INFO pIpAdapterInfoMir = NULL;
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	DWORD  IPCount = 0;
	char   *IPlist = NULL;

	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;

		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];

		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{
		//输出网卡信息,可能有多网卡,因此通过循环去判断
		pIpAdapterInfoMir = pIpAdapterInfo; 
		while (pIpAdapterInfoMir)
		{
				//可能网卡有多IP,因此通过循环去判断
				IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfoMir->IpAddressList);
				do 
				{
					//cout<<pIpAddrString->IpAddress.String<<endl;
					IPCount++;
					pIpAddrString=pIpAddrString->Next;
				} while (pIpAddrString);

				pIpAdapterInfoMir = pIpAdapterInfoMir->Next;
		}

		IPlist = new char[17*IPCount];
		memset(IPlist,'\0',17*IPCount);

		pIpAdapterInfoMir = pIpAdapterInfo; 
		while (pIpAdapterInfoMir)
		{
				//可能网卡有多IP,因此通过循环去判断
				IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfoMir->IpAddressList);
				do 
				{
					strcat(IPlist,pIpAddrString->IpAddress.String);
					strcat(IPlist," ");
					pIpAddrString=pIpAddrString->Next;
				} while (pIpAddrString);

				pIpAdapterInfoMir = pIpAdapterInfoMir->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	return IPlist;
}

DWORD COSNRpcServer::OSNRpcGetBasicInfo(char *pHostname,char **pIpAddress,char *pSysVersion)
{
	char                          *phostName = NULL;
	DWORD                         dw;

	LOG(INFO) << "OSNRpcGetBasicInfo start.";

	phostName = pOSNService->GetHostName();
	if(phostName == NULL)
	{
		return EXIT_FAILURE;
	}
	strcpy(pHostname,phostName);

	*pIpAddress = OSNRpcGetIPsInfo();

	dw = OSNRpcGetSysVersion(pSysVersion);
	if(dw == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

DWORD COSNRpcServer::OSNRpcSetServiceInfo(DWORD pXML)
{
	char pTargetIP[16];
	char ServerID[64];
	char ClientID[64];
	wchar_t pUnicode[256];
	memset(pUnicode,0,256*sizeof(wchar_t));

	LOG(INFO) << "OSNRpcSetServiceInfo start.";
	COSNxml *pXMLTemp = new COSNxml();
	
	try
	{
		if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
		{
			pXMLTemp->LoadFile(pUnicode);

			pXMLTemp->GetXMLNodeAttribute("Streamer/Server","ID",ServerID,64);
			pXMLTemp->GetXMLNodeAttribute("Streamer/Client","ID",ClientID,64);

			if(strcmp(m_pCopyXML->m_ServerID,ServerID) != 0 || strcmp(m_pCopyXML->m_ClientID,ClientID) != 0)
			{
				delete(pXMLTemp);
				return EXIT_FAILURE;
			}
		}
		else
		{
			delete(pXMLTemp);
			return EXIT_FAILURE;
		}
	}
	catch(...)
	{
		LOG(ERROR) << "exception occured when remove Client";
		return EXIT_FAILURE;
	}
	
	delete(pXMLTemp);
	try
	{
		m_pCopyXML->MoveNext(m_pCopyXML->m_TargetIPs,pTargetIP,sizeof(pTargetIP),' ');
		m_pCopyXML->DisConnectiSCSIChannel(pTargetIP,m_pCopyXML->m_TargetIqn);

		m_pCopyXML->m_IsProtected = false;
		m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","Protected",&m_pCopyXML->m_IsProtected,BoolKey);

		memset(m_pCopyXML->m_ServerIP,0,strlen(m_pCopyXML->m_ServerIP));
		m_pCopyXML->DelRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIP");

		memset(m_pCopyXML->m_ServerID,0,64);
		m_pCopyXML->DelRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerID");
	}
	catch(...)
	{
		LOG(ERROR) << "删除客户端出现异常：";
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

DWORD COSNRpcServer::OSNRpcRemoveMirror(DWORD pXML)
{
	bool MirrorTypeB;
	char MirrorTypeC[5];
	char SrcGUIDC[64];
	char DesGUIDC[64];
	wchar_t SrcGUIDW[64];
	wchar_t DesGUIDW[64];
	wchar_t pUnicode[256];
	wstring *pSrc,*pDes;

	COSNxml *pXMLTemp = new COSNxml();

	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeText("Streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,sizeof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DiskType",MirrorTypeC);
		if(strcmp(MirrorTypeC,"0") == 0)
		{
			MirrorTypeB = 0;//volume
		}
		else
		{
			MirrorTypeB = 1;
		}
	}
	else
	{
		delete(pXMLTemp);
		return EXIT_FAILURE;
	}
	delete(pXMLTemp);

	DWORD dw = m_pCopyXML->RemoveMirror(pSrc,pDes,MirrorTypeB);

	delete(pXMLTemp);
	delete(pSrc);
	delete(pDes);

	return dw;
}

DWORD COSNRpcServer::OSNRpcGetInitMirrorRate(DWORD pXML)
{
	bool MirrorTypeB;
	char MirrorTypeC[5];
	char SrcGUIDC[64];
	char DesGUIDC[64];
	wchar_t SrcGUIDW[64];
	wchar_t DesGUIDW[64];
	wchar_t pUnicode[256];
	wstring *pSrc,*pDes;

	COSNxml *pXMLTemp = new COSNxml();

	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeText("Streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DiskType",MirrorTypeC);
		if(strcmp(MirrorTypeC,"0") == 0)
		{
			MirrorTypeB = 0;//volume
		}
		else
		{
			MirrorTypeB = 1;
		}
	}
	else
	{
		delete(pXMLTemp);
		return EXIT_FAILURE;
	}
	
	delete(pXMLTemp);
	delete(pSrc);
	delete(pDes);

	return 0;//m_pCopyXML->GetInitMirrorRate(pSrc,pDes,MirrorTypeB);
}

DWORD COSNRpcServer::OSNRpcGetiSCSIChannel(DWORD pXML)
{
	DWORD   dw = 0;
	wchar_t pUnicode[256];
	char    TargetIP[20];
	char    InitiatorIP[20];

	LOG(INFO) << "OSNRpcGetiSCSIChannel start.";

	memset(pUnicode,0,256*sizeof(wchar_t));

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Channel","TargetIqn",m_pCopyXML->m_TargetIqn,bufsize);
		m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIqn",m_pCopyXML->m_TargetIqn,StringKey);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Channel/Path","TargetIP",m_pCopyXML->m_TargetIPs,bufsize);
		m_pCopyXML->MoveNext(m_pCopyXML->m_TargetIPs,TargetIP,sizeof(TargetIP),' ');

		pXMLTemp->GetXMLNodeAttribute("Streamer/Channel/Path","InitiatorIP",m_pCopyXML->m_InitiatorIPs,bufsize);
		m_pCopyXML->MoveNext(m_pCopyXML->m_InitiatorIPs,InitiatorIP,sizeof(InitiatorIP),' ');

		dw = m_pCopyXML->ConnectiSCSIChannel(TargetIP,InitiatorIP,m_pCopyXML->m_TargetIqn);
	}
	else
	{
		delete(pXMLTemp);
		return EXIT_FAILURE;
	}

	return dw;
}

DWORD COSNRpcServer::OSNRpcInitMirror(DWORD pXML)
{
	bool MirrorTypeB;
	char MirrorTypeC[5];
	char SrcGUIDC[64];
	char DesGUIDC[64];
	wchar_t SrcGUIDW[64];
	wchar_t DesGUIDW[64];
	wchar_t pUnicode[256];
	wstring *pSrc,*pDes;

	COSNxml *pXMLTemp = new COSNxml();

	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeText("Streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("Streamer","DiskType",MirrorTypeC);
		if(strcmp(MirrorTypeC,"0") == 0)
		{
			MirrorTypeB = 0;//volume
		}
		else
		{
			MirrorTypeB = 1;
		}
	}
	else
	{
		delete(pXMLTemp);
		return EXIT_FAILURE;
	}

	DWORD dw = m_pCopyXML->InitMirror(pSrc,pDes,MirrorTypeB);

	delete(pXMLTemp);
	delete(pSrc);
	delete(pDes);

	return dw;
}

DWORD COSNRpcServer::OSNRpcSetMirror(DWORD pXML)
{
	bool MirrorTypeB;
	char MirrorTypeC[5];
	char SrcGUIDC[64];
	char DesGUIDC[64];
	wchar_t SrcGUIDW[64];
	wchar_t DesGUIDW[64];
	wchar_t pUnicode[256];
	wstring *pSrc;
	wstring *pDes;
	
	LOG(INFO) << "OSNRpcSetMirror start.";

	memset(pUnicode,0,256*sizeof(wchar_t));

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Backup","SrcGUID",SrcGUIDC,64);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Backup","DestGUID",DesGUIDC,64);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Backup","Type",MirrorTypeC,5);
		if(strcmp(MirrorTypeC,"0") == 0)
		{
			MirrorTypeB = 0;//volume
		}
		else
		{
			MirrorTypeB = 1;
		}
	}
	else
	{
		delete(pXMLTemp);
		return EXIT_FAILURE;
	}

	DWORD dw = m_pCopyXML->NewMirror(pSrc,pDes,MirrorTypeB);

	delete(pXMLTemp);
	delete(pSrc);
	delete(pDes);

	return dw;
}

DWORD COSNRpcServer::OSNRpcGetServiceInfoSend(DWORD pXML)
{
	DWORD status;
	char            m_ServerID[64];
	char            m_ServerIP[256];
	wchar_t pUnicode[256];
	memset(pUnicode,0,256*sizeof(wchar_t));

	LOG(INFO) << "OSNRpcGetServiceInfo start.";

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Server","ID",m_ServerID,64);
		if(strcmp(m_pCopyXML->m_ServerID,m_ServerID) != 0)
		{
			delete(pXMLTemp);
			return EXIT_FAILURE;
		}
		//m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerID",m_pCopyXML->m_ServerID,StringKey);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Server","IPs",m_ServerIP,bufsize);
		if(strcmp(m_pCopyXML->m_ServerIP,m_ServerIP) != 0)
		{
			strcpy_s(m_pCopyXML->m_ServerIP,bufsize,m_ServerIP);
			m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIP",m_pCopyXML->m_ServerIP,StringKey);
		}
		
		status = EXIT_SUCCESS;
	}
	else
	{
		status = EXIT_FAILURE;
	}
	
	delete(pXMLTemp);
	return status;
}

DWORD COSNRpcServer::OSNRpcGetServiceInfo(DWORD pXML)
{
	DWORD status;
	wchar_t pUnicode[256];
	memset(pUnicode,0,256*sizeof(wchar_t));

	LOG(INFO) << "OSNRpcGetServiceInfo start.";

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Server","ID",m_pCopyXML->m_ServerID,64);
		m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerID",m_pCopyXML->m_ServerID,StringKey);

		pXMLTemp->GetXMLNodeAttribute("Streamer/Server","IPs",m_pCopyXML->m_ServerIP,bufsize);
		m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIP",m_pCopyXML->m_ServerIP,StringKey);
		
		status = EXIT_SUCCESS;
	}
	else
	{
		status = EXIT_FAILURE;
	}
	
	delete(pXMLTemp);
	return status;
}

DWORD COSNRpcServer::OSNRpcGetClientInfo(COSNxml *m_pTempXML)
{
	LOG(INFO) << "OSNRpcGetClientInfo start.";

	//m_pCopyXML->InitializeMembers();

	m_pCopyXML->RefreshClientXML(m_pTempXML);
	m_pCopyXML->RefreshDiskListXML(m_pTempXML);
	m_pCopyXML->RefreshVolumeListXML(m_pTempXML);
	m_pCopyXML->RefreshChannelListXML(m_pTempXML);

	return EXIT_SUCCESS;
}

bool COSNRpcServer::OSNRpcIoctlDispatch(PHC_MESSAGE_HEADER	pMsgHeader)
{
	bool                          status  = false;
	DWORD                         dw;
	char                          *pXMLText = (char*)((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);

	switch(pMsgHeader->cmd)
	{
	case ST_OP_SCAN_CLIENT:
		{
			LOG(INFO) << "ST_OP_SCAN_CLIENT CMD start.";

			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");
			char hostname[128],SysVersion[64],*IPs = NULL; 

			if(m_pCopyXML->m_IsProtected == false)
			{
				dw = OSNRpcGetBasicInfo(hostname,&IPs,SysVersion);
				if(EXIT_SUCCESS == dw)
				{
					pXMLTemp->AddXMLElement("Client");
					pXMLTemp->AddXMLAttribute("Client","Name",hostname);
					pXMLTemp->AddXMLAttribute("Client","IPs",IPs);
					pXMLTemp->AddXMLAttribute("Client","SystemType",SYS_TYPE_WINDOWS);
					pXMLTemp->AddXMLAttribute("Client","SystemVersion",SysVersion);
					pXMLTemp->AddXMLAttribute("Client","ID",m_pCopyXML->m_ClientID);

					DWORD size = pXMLTemp->GetXMLText(pXMLText);
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
					LOG(INFO) << "ST_OP_SCAN_CLIENT CMD is successful!";
				}
				else
				{
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
					LOG(INFO) << "ST_OP_SCAN_CLIENT CMD is failed(OSNRpcGetBasicInfo)!";
				}
				if(IPs != NULL)
				{
					delete(IPs);
				}
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_SCAN_CLIENT CMD is failed(m_IsProtected is true)!";
			}

			delete(pXMLTemp);
		}
		break;

	case ST_OP_ADD_CLIENTS:
		{
			LOG(INFO) << "ST_OP_ADD_CLIENTS CMD start.";

			COSNxml *m_pTempXML = new COSNxml();
			m_pTempXML->CreateXMLFile("Streamer");

			m_pCopyXML->m_IsProtected = true;
			m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","Protected",&m_pCopyXML->m_IsProtected,BoolKey);

			dw = OSNRpcGetServiceInfo((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				DWORD dw = OSNRpcGetClientInfo(m_pTempXML);
				if(EXIT_SUCCESS == dw)
				{
					DWORD size = m_pTempXML->GetXMLText(pXMLText);
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
					LOG(INFO) << "ST_OP_ADD_CLIENTS CMD successful!";
				}
				else
				{
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
					LOG(INFO) << "ST_OP_ADD_CLIENTS CMD failed(OSNRpcGetClientInfo)!";
				}
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_ADD_CLIENTS CMD failed(OSNRpcGetServiceInfo)!";
			}
			delete(m_pTempXML);
		}
		break;

	case ST_OP_GET_CLIENTS_INFO:
		{
			LOG(INFO) << "ST_OP_GET_CLIENTS_INFO CMD start.";

			COSNxml *m_pTempXML = new COSNxml();
			m_pTempXML->CreateXMLFile("Streamer");

			DWORD dw = OSNRpcGetClientInfo(m_pTempXML);
			if(EXIT_SUCCESS == dw)
			{
				DWORD size = m_pTempXML->GetXMLText(pXMLText);
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
				LOG(INFO) << "ST_OP_GET_CLIENTS_INFO CMD successful!";
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_GET_CLIENTS_INFO CMD failed(OSNRpcGetClientInfo)!";
			}
			delete(m_pTempXML);
		}
		break;

	case ST_OP_DEL_CLIENTS:
		{
			LOG(INFO) << "ST_OP_DEL_CLIENTS CMD start.";

			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			if(m_pCopyXML->m_IsProtected != false)
			{
				dw = OSNRpcSetServiceInfo((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
				if(EXIT_SUCCESS == dw)
				{
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
					LOG(INFO) << "ST_OP_DEL_CLIENTS CMD successful!";
				}
				else
				{
					OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
					LOG(INFO) << "ST_OP_DEL_CLIENTS CMD failed(OSNRpcSetServiceInfo)!";
				}
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "Client has been removed already!";
			}
			delete(pXMLTemp);
		}
		break;

	case ST_OP_ADD_BACKUP:
		{
			LOG(INFO) << "ST_OP_ADD_BACKUP CMD start.";

			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			dw = OSNRpcSetMirror((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_ADD_BACKUP CMD successful!";
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_ADD_BACKUP CMD failed(OSNRpcSetMirror)!";
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_REMOVEMIRROR:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			dw = OSNRpcRemoveMirror((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				
			}
			else
			{
				pXMLTemp->AddXMLElement("Error");
				pXMLTemp->AddXMLAttribute("Error","Errcode","0");
				pXMLTemp->AddXMLAttribute("Error","Errorsummary","0");
				pXMLTemp->AddXMLAttribute("Error","Errorinfo","0");
				DWORD size = pXMLTemp->GetXMLText(pXMLText);

				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_INITMIRTOSERVER:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			dw = OSNRpcInitMirror((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				
			}
			else
			{
				pXMLTemp->AddXMLElement("Error");
				pXMLTemp->AddXMLAttribute("Error","Errcode","0");
				pXMLTemp->AddXMLAttribute("Error","Errorsummary","0");
				pXMLTemp->AddXMLAttribute("Error","Errorinfo","0");
				DWORD size = pXMLTemp->GetXMLText(pXMLText);

				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_GETINITMIRFROMSERVER:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			dw = OSNRpcGetInitMirrorRate((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				
			}
			else
			{
				pXMLTemp->AddXMLElement("Error");
				pXMLTemp->AddXMLAttribute("Error","Errcode","0");
				pXMLTemp->AddXMLAttribute("Error","Errorsummary","0");
				pXMLTemp->AddXMLAttribute("Error","Errorinfo","0");
				DWORD size = pXMLTemp->GetXMLText(pXMLText);

				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,size);
			}
			delete(pXMLTemp);
		}
		break;

	case ST_OP_ESTABLISH_CHANNELS:
		{
			LOG(INFO) << "ST_OP_ESTABLISH_CHANNELS CMD start.";

			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("Streamer");

			dw = OSNRpcGetiSCSIChannel((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_SUCCESS,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_ESTABLISH_CHANNELS CMD successful!";
			}
			else
			{
				OSNRpcRetMsgHeader(pMsgHeader,ST_RES_FAILED,OSN_MSGHEAD_PARSE_XML,OSN_MSGHEAD_CMD_FLAG_RESPONSE,0);
				LOG(INFO) << "ST_OP_ESTABLISH_CHANNELS CMD failed(OSNRpcGetiSCSIChannel)!";
			}
			delete(pXMLTemp);
		}
		break;
	}

	return true;
}

inline void  COSNRpcServer::OSNRpcRetMsgHeader(PHC_MESSAGE_HEADER pMsgHeader,unsigned short rtnStatus,BYTE parseType,unsigned short flag,unsigned int dataLength)
{
	pMsgHeader->rtnStatus  = rtnStatus;
	pMsgHeader->parseType  = parseType;		
	pMsgHeader->flag       = flag;		
	pMsgHeader->dataLength = dataLength;
}

//Process the income command
bool COSNRpcServer::OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET &ConnectSocket,char *pMsg)
{

	PHC_MESSAGE_HEADER	pMsgHeader = (PHC_MESSAGE_HEADER) pMsg;
	char			*pParaBuffer = &pMsg[OSNRPC_HCMSGHEAD_LEN];


	//check the command ID. if it is not xml command, go to other
	//if (pMsgHeader->parseType & OSN_MSGHEAD_PARSE_XML)
//	{
		OSNRpcIoctlDispatch(pMsgHeader);

		//send back the response
		//pMsgHeader->flags	= OSNRPC_HBCMD_RESPONSE;
		OSNRpcSendMsg(outSockAddr, (char *) pMsg, OSNRPC_HCMSGHEAD_LEN + pMsgHeader->dataLength,ConnectSocket);
	//}
	//else	//analyze the response
	//{
		//pOSNService->LogMessage("failed");
		/*if(pMsgHeader->retStatus != CMD_HBSTATUS_SUCCESS)
		{
			char msg[256];
			sprintf(msg, "Command %d to %d.%d.%d.%d failed, return status = %d", pMsgHeader->command,
				outSockAddr.sin_addr.S_un.S_un_b.s_b1, outSockAddr.sin_addr.S_un.S_un_b.s_b2, 
				outSockAddr.sin_addr.S_un.S_un_b.s_b3, outSockAddr.sin_addr.S_un.S_un_b.s_b4,
				pMsgHeader->retStatus);

			pOSNService->LogMessage(msg);
			return false;
		}*/



	//}

	return true;

}

//Process SHUTDOWN command, no return messages
bool COSNRpcServer::OSNRpcCmdSHUTDOWN(SOCKADDR_IN inSockAddr)
{
	//retrieve the local socket address information
	SOCKADDR_IN	sinLocal;
	OSNRpcGetLocalSockAddr(sinLocal);

	int nReturn;
	nReturn = strncmp((char *)&(sinLocal.sin_addr), (char *)&(inSockAddr.sin_addr), sizeof(in_addr));
	if (nReturn == 0)
	{
		SetOSNRpcState(OSNRPC_STOP);
		SetOSNRpcMsgState(OSNRPC_STOP);
		SetOSNRpcSendState(OSNRPC_STOP);
	}

	return true;
}


//Process PING command, return message is port ID
bool COSNRpcServer::OSNRpcCmdPing()
{

	PHC_MESSAGE_HEADER		pMsgHeader = (PHC_MESSAGE_HEADER) m_sRecvMsg;
	char				*pParaBuffer = &m_sRecvMsg[OSNRPC_HCMSGHEAD_LEN];

	//pMsgHeader->retStatus = CMD_HBSTATUS_SUCCESS;

	//pMsgHeader->paraNum		= 0;
	pMsgHeader->dataLength	= 0;

	return true;

}

bool COSNRpcServer::OSNRpcCmdNotification(char *m_sRecvMsg)
{
	PHC_MESSAGE_HEADER msgHeader=(PHC_MESSAGE_HEADER)m_sRecvMsg;

	char				*pBuffer 	     = &m_sRecvMsg[OSNRPC_HCMSGHEAD_LEN];
	NOTIFICATION_OBJECT_LIST *pList=(NOTIFICATION_OBJECT_LIST *)(pBuffer);
	NOTIFICATION_OBJECT *pNotification=NULL;
	DWORD ErrorCode=0;

	char szMsg[256];

	for(unsigned int i=0;i<pList->m_Count;i++)
	{
		pNotification=(NOTIFICATION_OBJECT *)(&pList->m_Object[i]);

		if(pNotification->m_DssNotification)
		{
			ErrorCode=OSNDssSetNotification(pNotification);
		}
		else
		{
			ErrorCode=OSNVssSetNotification(pNotification);
		}
		if(!ErrorCode)
		{
			pNotification->m_HBServiceCheck&=~NOTIFICATION_BIT; 

			sprintf(szMsg,"INFO: Set volume(%.8X_%.16I64X_%.8X) notification successfully",
				pNotification->m_SrcId.MBP_VolumeID.m_DiskSignature,
				pNotification->m_SrcId.MBP_VolumeID.m_BlockOffset,
				pNotification->m_SrcId.MBP_VolumeID.m_NotUsed1);

			pOSNService->LogMessage(szMsg);
		}
		else
		{
			sprintf(szMsg,"ERROR:Failed to set volume (%.8X_%.16I64X_%.8X) Notification,errorCode=%d",
				pNotification->m_SrcId.MBP_VolumeID.m_DiskSignature,
				pNotification->m_SrcId.MBP_VolumeID.m_BlockOffset,
				pNotification->m_SrcId.MBP_VolumeID.m_NotUsed1,
				ErrorCode);

			pOSNService->LogMessage(szMsg);
		}
	}

	//msgHeader->retStatus=CMD_HBSTATUS_SUCCESS;
	msgHeader->dataLength=pList->m_Count*sizeof(NOTIFICATION_OBJECT)+sizeof(NOTIFICATION_OBJECT_LIST);
	return true;
}