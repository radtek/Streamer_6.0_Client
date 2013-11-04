#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1

#include "stdio.h"
#include "stdlib.h"
#include "afx.h"
#include "atlbase.h"

#include "winsock2.h"
#include "winnt.h"
#include "winioctl.h"
#include <assert.h>

#include "Configure.h"
/*
#include "..\OSNApi\OsnApi.h"
#include "..\common\OsnDefs.h"
#include "..\common\OSNCommon.h"
#include"..\OSNApi\IsBusApi.h"
#include "..\OSNApi\OSNHaApi.h"
*/
#include "..\OsnVolumeCopyApi\Configure.h"
#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"
# include"OSNService.h"
# include "OSNRemoteServer.h"
extern	COSNService			*pOSNService; 

CRemoteServer::CRemoteServer(char *pName)
{
	memset(m_ServerName,0,sizeof(m_ServerName));

	ULONG size = strlen(pName);
	if(size>=128)
	{
		size = sizeof(m_ServerName)-1;
	}

	memcpy(m_ServerName,pName,size);

	for(int i=0;i<5;i++)
	{
		m_IPAddress[i]=0;
	}
}

CRemoteServer::~CRemoteServer()
{

}



NOTIFICATION_OBJECT * CRemoteServer::GetNotificationInfoByGuid(GUID VolumeGuid)
{
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	CQueue	*pNode = NotificationInfoList.Next();
	while( pNode )
	{
		pNotificationInfo = (NOTIFICATION_OBJECT *) pNode->GetItem();
		if(!memcmp(&pNotificationInfo->m_SrcId.SAN_VolumeID.m_VolumeGuid,
			&VolumeGuid,
			sizeof(GUID)))
		{
			return pNotificationInfo;
		}
		pNode = pNode->Next();
	}

	return NULL;
}

void CRemoteServer::FreeMemory()
{
	CQueue	*pNode = NotificationInfoList.Next();
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	ULONG ErrorCode =0;

	CQueue *pNewHeader;

	while( pNode )
	{
		pNotificationInfo=(NOTIFICATION_OBJECT *)pNode->GetItem();

		pNewHeader=pNode->Next();

		pNode->Remove();
		delete pNotificationInfo;
		pNode=pNewHeader;
	}



}


void CRemoteServer::ClearHBCheckBit(NOTIFICATION_OBJECT *pInfo)
{
	CQueue	*pNode = NotificationInfoList.Next();
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	ULONG ErrorCode =0;

	char Msg[256];
	while( pNode )
	{
		pNotificationInfo=(NOTIFICATION_OBJECT *)pNode->GetItem();

		if(!memcmp(&pNotificationInfo->m_SrcId,&pInfo->m_SrcId,sizeof(VOLUMEID)))
		{  

			ULONG BitValue = pNotificationInfo->m_HBServiceCheck;
			pNotificationInfo->m_HBServiceCheck&=~BitValue;

			if(pNotificationInfo->m_DssNotification)
			{
				ErrorCode = OSNDssRemoveMirrorNotification(pInfo);
			}
			else
			{
				ErrorCode = OSNVssRemoveMirrorNotification(pInfo);
			}


			if(ErrorCode==0)
			{
				pNotificationInfo->m_NotificationBit&=~BitValue;
				pNotificationInfo->m_NotSuccess=0;
				sprintf(Msg,"INFO: Tell the result of notification(%.8X_%.16I64X_%.8X).",
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1);

				pOSNService->LogMessage(Msg);
			}
			else
			{
				pNotificationInfo->m_HBServiceCheck|=BitValue;
				sprintf(Msg,
					"ERROR: Failed to tell  the result of notification(%.8X_%.16I64X_%.8X),code=%d.",
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
					pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1,
					ErrorCode);

				pOSNService->LogMessage(Msg);
			}
			return;
		}

		pNode = pNode->Next();
	}


}

void CRemoteServer::InsertNotificationInfotoServer(void *pObject)
{ 
	NotificationInfoList.InsertQTail(pObject); 
	pOSNService->LogNotificationToFile((NOTIFICATION_OBJECT *)pObject);

};

void CRemoteServer::CheckNotification()
{

	CQueue	*pNode = NotificationInfoList.Next();
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;

	char szMsg[256];
	CQueue *pNext= NULL;

	while( pNode )
	{
		pNotificationInfo = (NOTIFICATION_OBJECT *)pNode->GetItem();
		if(0 == pNotificationInfo->m_NotificationBit)
		{
			sprintf(szMsg,"INFO: remove notification(%.8X_%.16I64X_%.8X) from list.",
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1);
			pOSNService->LogMessage(szMsg);

			pNext = pNode->Next();
			pNode->Remove();
			delete pNotificationInfo;

			pNode = pNext;
			continue;
		}
		else
		{
			if(pNotificationInfo->m_HBServiceCheck==0)
			{
				pNotificationInfo->m_HBServiceCheck = pNotificationInfo->m_NotificationBit;
			}
		}

		pNode = pNode->Next();
	}
}



ULONG CRemoteServer::GetNumberOfNotification()
{
	ULONG count=0;
	NOTIFICATION_OBJECT *pInfo=NULL;

	CQueue	*pNode = NotificationInfoList.Next();
	while( pNode )
	{   
		pInfo=(NOTIFICATION_OBJECT *)pNode->GetItem();

		if(pInfo->m_HBServiceCheck&NOTIFICATION_BIT)
		{
			count++;
		}

		pNode = pNode->Next();
	}

	return count;

}


void CRemoteServer::CopyNotificationInfoToBuffer(PVOID Buffer,ULONG count)
{
	if(!Buffer)
	{
		return;
	}

	NOTIFICATION_OBJECT_LIST *pList=(NOTIFICATION_OBJECT_LIST *)(Buffer);
	CQueue	*pNode = NotificationInfoList.Next();
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	unsigned int i=0;
	while( pNode )
	{
		pNotificationInfo=(NOTIFICATION_OBJECT *)pNode->GetItem();

		// skip the partner
		if(pNotificationInfo->m_HBServiceCheck&NOTIFICATION_BIT)
		{
			memcpy(&pList->m_Object[i],pNotificationInfo,sizeof(NOTIFICATION_OBJECT));
			i++;
		}

		if(count ==i)	
		{
			break;
		}

		pNode = pNode->Next();
	}

	pList->m_Count=i;

	return;
}

DWORD CRemoteServer::_SendNotificationToRemoteServer(ULONG IPAddress,ULONG Number)
{

	if(0 == IPAddress)
	{
		return 2;
	}

	char szMsg[256];
	int BufferSize= Number*sizeof(NOTIFICATION_OBJECT)+sizeof(NOTIFICATION_OBJECT_LIST);
	char *pBuffer=new char[BufferSize];
	if(!pBuffer)
	{
		pOSNService->LogMessage("WARNING:Failed to allocate memory for Notification list.",
			ERROR_NOT_ENOUGH_MEMORY);
		return 1;
	}
	CopyNotificationInfoToBuffer(pBuffer,Number);// copy to buffer

	DWORD Send=pOSNService->SendSocketCommand(pBuffer,
		BufferSize,
		0x0001,//OSNRPC_CMD_NOTIFICATION_SET,
		IPAddress);

	if(Send!=0)
	{
		pOSNService->LogMessage("ERROR:Failed to send notification command to remote.",
			Send);
		NotificationCallBack(pBuffer,BufferSize);
		delete [] pBuffer;
		return 2;
	}
	else
	{
		sprintf(szMsg,"INFO:Send notification(count=%d) command to remote.",Number);

		pOSNService->LogMessage(szMsg);
	}

	NotificationCallBack(pBuffer,BufferSize);

	delete [] pBuffer;
	return 0;

}

void  CRemoteServer:: NotificationCallBack(PVOID pBuffer,int size)
{
	NOTIFICATION_OBJECT_LIST *pList=(NOTIFICATION_OBJECT_LIST *)(pBuffer);
	DWORD ErrorCode=0;
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	// clear the list
	char Msg[256];

	for(unsigned int i=0;i<pList->m_Count;i++)
	{
		pNotificationInfo=(NOTIFICATION_OBJECT *)(&pList->m_Object[i]);
		if(pNotificationInfo->m_HBServiceCheck&NOTIFICATION_BIT)
		{
			NotificationSetRemoteState(pNotificationInfo);
			// the partner failed to handle this notification
			sprintf(Msg,"ERROR:Partner server not handle  notification for %.8X_%.16I64X_%.8X.",
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
				pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1);
			pOSNService->LogMessage(Msg);
			continue;

		}

		ClearHBCheckBit(pNotificationInfo);

	}

}

void CRemoteServer::NotificationSetRemoteState(NOTIFICATION_OBJECT *pObject)
{
	CQueue	*pNode = NotificationInfoList.Next();
	NOTIFICATION_OBJECT *pNotificationInfo=NULL;
	DWORD ErrorCode = 0;
	char Msg[256];

	while(pNode)
	{
		pNotificationInfo=(NOTIFICATION_OBJECT *)pNode->GetItem();

		if(!memcmp(&pNotificationInfo->m_SrcId,&pObject->m_SrcId,sizeof(VOLUMEID)))
		{  
			pNotificationInfo->m_NotSuccess++;
			if(pNotificationInfo->m_NotSuccess>=5)
			{
				if(pNotificationInfo->m_DssNotification)
				{
					ErrorCode = OSNDssSetRemoteState(pNotificationInfo);
				}
				else 
				{
					ErrorCode = OSNVssSetRemoteState(pNotificationInfo);
				}

				if(0 == ErrorCode)
				{
					pNotificationInfo->m_NotSuccess = 0 ;
				}
				else 
				{
					sprintf(Msg,"INFO: Set State of Remote Mirror Failed(%.8X_%.16I64X_%.8X).",
						pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
						pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
						pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1);

					pOSNService->LogMessage(Msg);
				}
			}
			return ;
		}
		pNode = pNode->Next();
	}
}