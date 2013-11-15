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
//#include "..\OSNService\OSNRpcCommon.h"
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
	m_handleThread		 = INVALID_HANDLE_VALUE;
	m_dwThreadID		 = 0;
	m_dwSocketThreadID   = 0;
	m_startSocketThreadHandle = INVALID_HANDLE_VALUE;

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HCMAX_MSG_LEN);

}

COSNRpcServer::COSNRpcServer(UINT inPort) : COSNRpcSocket(inPort, true)
{
	m_nRPCState			 = OSNRPC_STOPPED;
	m_handleThread		 = INVALID_HANDLE_VALUE;
	m_dwThreadID		 = 0;
	m_dwSocketThreadID   = 0;
	m_startSocketThreadHandle = INVALID_HANDLE_VALUE;

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HCMAX_MSG_LEN);
}

COSNRpcServer::~COSNRpcServer()
{
	delete(m_pCopyXML);
	if (m_handleThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handleThread);
		m_handleThread = INVALID_HANDLE_VALUE;
	}

	if(m_startSocketThreadHandle !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_startSocketThreadHandle);
		m_startSocketThreadHandle = INVALID_HANDLE_VALUE;
	}
}



//OSN Rpc service thread function
DWORD WINAPI COSNRpcServer::OSNRpcListenThread(void *pData)	
{
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
	
	pOSNRpcServer->m_pCopyXML           = new COsnMirrorCopyXML();
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
			//process the income command
			pOSNRpcServer->OSNRpcProcessRequest(sinAccept,sockConn);
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


//start OSN Rpc service, set socket thread status flag
DWORD COSNRpcServer::StartListenThread()
{

	//initialize socket
	//WSADATA	wdata;
	//if(WSAStartup(MAKEWORD(2,2), &wdata))
	//		return WSAGetLastError();


	//start the socket
	//if (!OSNRpcStartSocket(true))
	//	return OSNRpcGetLastError();
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

		m_pCopyXML->m_IsProtected = true;
		m_pCopyXML->SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","Protected",&m_pCopyXML->m_IsProtected,BoolKey);

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

DWORD COSNRpcServer::OSNRpcGetClientInfo()
{
	LOG(INFO) << "OSNRpcGetClientInfo start.";

	m_pCopyXML->InitializeMembers();

	m_pCopyXML->RefreshClientXML();
	m_pCopyXML->RefreshDiskListXML();
	m_pCopyXML->RefreshVolumeListXML();
	m_pCopyXML->RefreshChannelListXML();

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
			//m_pCopyXML           = new COsnMirrorCopyXML();

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

			m_pCopyXML->m_pTempXML = new COSNxml();
			m_pCopyXML->m_pTempXML->CreateXMLFile("Streamer");

			dw = OSNRpcGetServiceInfo((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				DWORD dw = OSNRpcGetClientInfo();
				if(EXIT_SUCCESS == dw)
				{
					DWORD size = m_pCopyXML->m_pTempXML->GetXMLText(pXMLText);
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
			delete(m_pCopyXML->m_pTempXML);
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
bool COSNRpcServer::OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET ConnectSocket)
{

	PHC_MESSAGE_HEADER	pMsgHeader = (PHC_MESSAGE_HEADER) m_sRecvMsg;
	char			*pParaBuffer = &m_sRecvMsg[OSNRPC_HCMSGHEAD_LEN];


	//check the command ID. if it is not xml command, go to other
	//if (pMsgHeader->parseType & OSN_MSGHEAD_PARSE_XML)
//	{
		OSNRpcIoctlDispatch(pMsgHeader);

		//send back the response
		//pMsgHeader->flags	= OSNRPC_HBCMD_RESPONSE;
		OSNRpcSendMsg(outSockAddr, (char *) m_sRecvMsg, OSNRPC_HCMSGHEAD_LEN + pMsgHeader->dataLength,ConnectSocket);
		shutdown(ConnectSocket,SD_BOTH);
		closesocket(ConnectSocket);
	//}
	//else	//analyze the response
	//{
		pOSNService->LogMessage("failed");
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