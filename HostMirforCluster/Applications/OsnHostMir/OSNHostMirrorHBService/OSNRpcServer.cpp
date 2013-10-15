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

/*
#include "..\common\OsnDefs.h"
#include "..\common\OSNCommon.h"
#include "..\common\DDOsnHa.h"
#include "..\common\OSNNdIoctl.h"
#include "..\OSNApi\OSNApi2.h"
#include "..\OSNApi\OSNApi.h"

#include"..\OSNApi\IsBusApi.h"
#include "..\OSNApi\OSNHaApi.h"

#include "..\common\DDOsnHa.h"*/
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

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HBMAX_MSG_LEN);

}

COSNRpcServer::COSNRpcServer(UINT inPort) : COSNRpcSocket(inPort, true)
{
	m_nRPCState			 = OSNRPC_STOPPED;
	m_handleThread		 = INVALID_HANDLE_VALUE;
	m_dwThreadID		 = 0;
	m_dwSocketThreadID   = 0;
	m_startSocketThreadHandle = INVALID_HANDLE_VALUE;

	ZeroMemory((PVOID)&m_sRecvMsg, OSNRPC_HBMAX_MSG_LEN);
}

COSNRpcServer::~COSNRpcServer()
{
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
	pOSNService->LogMessage("INFO: OSN HeartBeat Listen thread stopped.");


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



//Process the income command
bool COSNRpcServer::OSNRpcProcessRequest(SOCKADDR_IN &outSockAddr,SOCKET ConnectSocket)
{

	PHB_MESSAGE_HEADER	pMsgHeader = (PHB_MESSAGE_HEADER) m_sRecvMsg;
	char			*pParaBuffer = &m_sRecvMsg[OSNRPC_HBMSGHEAD_LEN];


	//check the command ID. if it is not OSN command, go to error return routine
	if (pMsgHeader->flags & OSNRPC_HBCMD_ASK)
	{

		if(pMsgHeader->command==OSNRPC_CMD_NOTIFICATION_SET)
		{
			OSNRpcCmdNotification(m_sRecvMsg);
		}
		if(pMsgHeader->command == OSNPRC_CMD_GETPARTNER_INFO)
		{

		}

		//send back the response
		pMsgHeader->flags	= OSNRPC_HBCMD_RESPONSE;
		OSNRpcSendMsg(outSockAddr, (char *) m_sRecvMsg, OSNRPC_HBMSGHEAD_LEN + pMsgHeader->dataLength,ConnectSocket);
		shutdown(ConnectSocket,SD_BOTH);
		closesocket(ConnectSocket);
	}
	else	//analyze the response
	{
		pOSNService->LogMessage("failed");
		if(pMsgHeader->retStatus != CMD_HBSTATUS_SUCCESS)
		{
			char msg[256];
			sprintf(msg, "Command %d to %d.%d.%d.%d failed, return status = %d", pMsgHeader->command,
				outSockAddr.sin_addr.S_un.S_un_b.s_b1, outSockAddr.sin_addr.S_un.S_un_b.s_b2, 
				outSockAddr.sin_addr.S_un.S_un_b.s_b3, outSockAddr.sin_addr.S_un.S_un_b.s_b4,
				pMsgHeader->retStatus);

			pOSNService->LogMessage(msg);
			return false;
		}



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