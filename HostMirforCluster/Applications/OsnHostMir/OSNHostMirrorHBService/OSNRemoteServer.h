# ifndef _OSNREMOTESERVER_
# define _OSNREMOTESERVER_

#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"

class CRemoteServer
{
	public:

		CRemoteServer::CRemoteServer(char *pName);
		CRemoteServer::~CRemoteServer();

		CQueue NotificationInfoList;

        NOTIFICATION_OBJECT *GetNotificationInfoByGuid(GUID VolumeGuid);

		void InsertNotificationInfotoServer(void *pObject);

		inline char * GetServerName() {return m_ServerName; };
		inline ULONG GetIPAddress(int AdapterIndex){return m_IPAddress[AdapterIndex];}
		inline void	   SetIPAddress(int AdapaterIndex,ULONGLONG IPAddress)
		{
			m_IPAddress[AdapaterIndex]=static_cast<ULONG>(IPAddress);
		}
	
		inline bool	   EmptyIPAddress(){return ((0 == m_IPAddress[0])||(0==m_IPAddress[1]));}
	
		void CheckNotification();
		ULONG GetNumberOfNotification();

		void CopyNotificationInfoToBuffer(PVOID Buffer,ULONG count);
		

		DWORD  _SendNotificationToRemoteServer(ULONG IPAddress,ULONG Number);//send notification to remote server

		void   NotificationCallBack(PVOID pBuffer,int size);
		void   NotificationSetRemoteState(NOTIFICATION_OBJECT *pObject);
		void	FreeMemory();
		void ClearHBCheckBit(NOTIFICATION_OBJECT *pInfo);
		

    private:

		char  m_ServerName[128];
		ULONG	m_IPAddress[5];
		

	    
};

# endif