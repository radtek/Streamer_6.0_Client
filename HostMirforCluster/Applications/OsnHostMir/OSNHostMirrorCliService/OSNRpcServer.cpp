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
		printf("CoInitializeEx error\n");
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
		printf("CoInitializeSecurity error\n");
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

DWORD COSNRpcServer::OSNRpcGetBasicInfo(char *pHostname,char *pIpAddress,char *pSysVersion)
{
	ULONG                         IPAddress;
	char                          *phostName = NULL;
	char                          *IpAddress;
	struct	in_addr               sin_addr; 

	phostName = pOSNService->GetHostName();
	if(phostName == NULL)
	{
		return EXIT_FAILURE;
	}
	strcpy(pHostname,phostName);

	DWORD dw = pOSNService->GetIPAddressByHostName(phostName,&IPAddress);
	if(0 != dw)
	{
		return EXIT_FAILURE;
	}
	sin_addr.S_un.S_addr = IPAddress;
	IpAddress = inet_ntoa(sin_addr);
	if(IpAddress == NULL)
	{
		return EXIT_FAILURE;
	}
	strcpy(pIpAddress,IpAddress);

	dw = OSNRpcGetSysVersion(pSysVersion);
	if(dw == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

DWORD COSNRpcServer::OSNRpcSetServiceInfo()
{
	m_IsProtected = false;
	memset(m_ServerIP,0,sizeof(m_ServerIP));
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

		pXMLTemp->GetXMLNodeText("streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,sizeof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","MirrorType",MirrorTypeC);
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

		pXMLTemp->GetXMLNodeText("streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","MirrorType",MirrorTypeC);
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

		pXMLTemp->GetXMLNodeText("streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","MirrorType",MirrorTypeC);
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
	
	memset(pUnicode,0,256*sizeof(wchar_t));

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeText("streamer","SrcGUID",SrcGUIDC);
		m_pCopyXML->CharToWchar(SrcGUIDC,SrcGUIDW,_countof(SrcGUIDW));
		pSrc = new wstring(SrcGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","DesGUID",DesGUIDC);
		m_pCopyXML->CharToWchar(DesGUIDC,DesGUIDW,_countof(DesGUIDW));
		pDes = new wstring(DesGUIDW);

		pXMLTemp->GetXMLNodeText("streamer","MirrorType",MirrorTypeC);
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

	COSNxml *pXMLTemp = new COSNxml();
	
	if(pXMLTemp->UTF_8ToUnicode((char *)pXML,pUnicode))
	{
		pXMLTemp->LoadFile(pUnicode);

		pXMLTemp->GetXMLNodeText("streamer","IpAddr",m_ServerIP);
		m_IsProtected = true;
		
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
	m_pCopyXML->InitializeMembers();

	m_pCopyXML->RefreshClientXML();
	m_pCopyXML->RefreshDiskListXML();
	m_pCopyXML->RefreshVolumeListXML();

	return EXIT_SUCCESS;
}

bool COSNRpcServer::OSNRpcIoctlDispatch(PHC_MESSAGE_HEADER	pMsgHeader)
{
	bool                          status  = false;
	DWORD                         dw;
	wchar_t                       *pXMLText = (wchar_t*)((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);

	switch(pMsgHeader->cmd)
	{
	case OSN_REMOTE_CMD_FINDNEWCLIENT:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");
			char hostname[128],ipAddress[32],SysVersion[64]; 

			dw = OSNRpcGetBasicInfo(hostname,ipAddress,SysVersion);
			if(EXIT_SUCCESS == dw)
			{
				pXMLTemp->AddXMLElement("Client");
				pXMLTemp->AddXMLAttribute("Client","Name",hostname);
				pXMLTemp->AddXMLAttribute("Client","IpAddr",ipAddress);
				pXMLTemp->AddXMLAttribute("Client","SystemType","1");
				pXMLTemp->AddXMLAttribute("Client","SystemVersion",SysVersion);
				DWORD size = pXMLTemp->GetXMLText(pXMLText);

				pMsgHeader->version = OSN_MSGHEAD_PROTVERSION_BASIC;
				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_SUCCESS;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			else
			{
				pMsgHeader->version = OSN_MSGHEAD_PROTVERSION_BASIC;
				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = 0;
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_SETADDCLIENTMARK:
		{
			m_pCopyXML->m_pTempXML = new COSNxml();
			m_pCopyXML->m_pTempXML->CreateXMLFile("streamer");

			dw = OSNRpcGetServiceInfo((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
			if(EXIT_SUCCESS == dw)
			{
				DWORD dw = OSNRpcGetClientInfo();
				if(EXIT_SUCCESS == dw)
				{
					DWORD size = m_pCopyXML->m_pTempXML->GetXMLText(pXMLText);

					pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_SUCCESS;
					pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
					pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
					pMsgHeader->dataLength = size;
				}
				else
				{
					m_pCopyXML->m_pTempXML->AddXMLElement("Error");
					m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errcode","0");
					m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errorsummary","0");
					m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errorinfo","0");
					DWORD size = m_pCopyXML->m_pTempXML->GetXMLText(pXMLText);

					pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
					pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
					pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
					pMsgHeader->dataLength = size;
				}
			}
			else
			{
				m_pCopyXML->m_pTempXML->AddXMLElement("Error");
				m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errcode","0");
				m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errorsummary","0");
				m_pCopyXML->m_pTempXML->AddXMLAttribute("Error","Errorinfo","0");
				DWORD size = m_pCopyXML->m_pTempXML->GetXMLText(pXMLText);

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(m_pCopyXML->m_pTempXML);
		}
		break;

	case OSN_REMOTE_CMD_SETREMOVECLIENTMARK:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");

			dw = OSNRpcSetServiceInfo();
			if(EXIT_SUCCESS == dw)
			{
				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_SUCCESS;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = 0;
			}
			else
			{
				pXMLTemp->AddXMLElement("Error");
				pXMLTemp->AddXMLAttribute("Error","Errcode","0");
				pXMLTemp->AddXMLAttribute("Error","Errorsummary","0");
				pXMLTemp->AddXMLAttribute("Error","Errorinfo","0");
				DWORD size = pXMLTemp->GetXMLText(pXMLText);

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_CREATEMIRROR:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");

			dw = OSNRpcSetMirror((DWORD)pMsgHeader + OSNRPC_HCMSGHEAD_LEN);
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

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_REMOVEMIRROR:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");

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

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_INITMIRTOSERVER:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");

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

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(pXMLTemp);
		}
		break;

	case OSN_REMOTE_CMD_GETINITMIRFROMSERVER:
		{
			COSNxml *pXMLTemp = new COSNxml();
			pXMLTemp->CreateXMLFile("streamer");

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

				pMsgHeader->rtnStatus = OSN_MSGHEAD_RTNSTATUS_FAILED;
				pMsgHeader->parseType = OSN_MSGHEAD_PARSE_XML;		
				pMsgHeader->flag = OSN_MSGHEAD_CMD_FLAG_RESPONSE;		
				pMsgHeader->dataLength = size;
			}
			delete(pXMLTemp);
		}
		break;
	}

	return true;
}

//Process the income command
bool COSNRpcServer::OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET ConnectSocket)
{

	PHC_MESSAGE_HEADER	pMsgHeader = (PHC_MESSAGE_HEADER) m_sRecvMsg;
	char			*pParaBuffer = &m_sRecvMsg[OSNRPC_HCMSGHEAD_LEN];


	//check the command ID. if it is not xml command, go to other
	if (pMsgHeader->parseType & OSN_MSGHEAD_PARSE_XML)
	{
		OSNRpcIoctlDispatch(pMsgHeader);

		//send back the response
		//pMsgHeader->flags	= OSNRPC_HBCMD_RESPONSE;
		OSNRpcSendMsg(outSockAddr, (char *) m_sRecvMsg, OSNRPC_HCMSGHEAD_LEN + pMsgHeader->dataLength,ConnectSocket);
		shutdown(ConnectSocket,SD_BOTH);
		closesocket(ConnectSocket);
	}
	else	//analyze the response
	{
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



	}

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

	PHB_MESSAGE_HEADER		pMsgHeader = (PHB_MESSAGE_HEADER) m_sRecvMsg;
	char				*pParaBuffer = &m_sRecvMsg[OSNRPC_HBMSGHEAD_LEN];

	pMsgHeader->retStatus = CMD_HBSTATUS_SUCCESS;

	pMsgHeader->paraNum		= 0;
	pMsgHeader->dataLength	= 0;

	return true;

}

bool COSNRpcServer::OSNRpcCmdNotification(char *m_sRecvMsg)
{
	PHB_MESSAGE_HEADER msgHeader=(PHB_MESSAGE_HEADER)m_sRecvMsg;

	char				*pBuffer 	     = &m_sRecvMsg[OSNRPC_HBMSGHEAD_LEN];
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

	msgHeader->retStatus=CMD_HBSTATUS_SUCCESS;
	msgHeader->dataLength=pList->m_Count*sizeof(NOTIFICATION_OBJECT)+sizeof(NOTIFICATION_OBJECT_LIST);
	return true;
}