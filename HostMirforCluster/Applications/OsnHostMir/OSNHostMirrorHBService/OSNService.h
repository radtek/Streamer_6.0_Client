#ifndef __OSNSERVICE_H__
#define __OSNSERVICE_H__

#include "OSNRemoteServer.h"


#define	MAX_PD			16
#define	MAX_RESOURCE	256

#define OSN_REGVALUE_PCHAR						0
#define OSN_REGVALUE_DWORD						1

#define	OSNCLIENT_REG_SERVICE					"SYSTEM\\CurrentControlSet\\Services\\OSNHBService\0"

class COSNService
{

private:

	COSNRpcServer	m_OSNRpcServer;

	HANDLE			m_hLogfileEvent;

	DWORD			m_dwTotalFreeBufferSize;
    char			m_szServiceName[256];
	char			m_szServiceDisplayName[256];
    int				m_iMajorVersion;
    int				m_iMinorVersion;
    int             m_LicencedFeatures;

    SERVICE_STATUS_HANDLE	m_hServiceStatus;

	HANDLE			stopServiceEventHandle;

    
	DWORD			m_OutstandingThread;

	//WWN				m_HostWWN;
	char			m_Hostname[OSN_MAX_VOLUMENAME];
	DWORD			m_HostnameLength;
	ULONG			m_IpAddress;

	DWORD			m_DriverMode;

public:
    SERVICE_STATUS	m_Status;
	bool			m_LicenceChecked;

	char			fileDirectory[MAX_PATH];
	bool			m_TrialVersion;
	int				m_nTotalOfClient;
	int				m_nNumberOfClientHosts;
	bool			m_bBufferFull;
	char			g_ExeFilePath[MAX_PATH];


	HANDLE          m_HBSendNotificationThreadHandle;
	HANDLE          m_HBDssSendNotificationEventHandle;
	HANDLE			m_HBVssSendNotificationEventHandle;

	HANDLE			m_HBRescanDiskEventHandle;
	HANDLE			m_HBRescanDiskThreadHandle;
	BOOL            m_HBRescanDiskThreadRunFlag;

	BOOL            m_CreateRescanEventToHostmirror;

	BOOL            m_HBThreadRunFlag;
	BOOL            m_PassEventToDss;
	BOOL			    m_PassEventToVss;

	CRemoteServer	*m_pRemoteServer;
	

public:

	DWORD GetIPAddressByHostName(char *pServerName,ULONG *pIPAddress);

	CRemoteServer       * GetRemoteServerInfo(){return m_pRemoteServer;}
	void GetClusterServerInfo();

	static DWORD        WINAPI OSNHBSendNotificationThread(void *pData);
	static DWORD        WINAPI OSNHBRescanDiskThread(void *pData);

	void   HandleVssNotification();
	void   HandleDssNotification();

	void   SendNotificationToRemoteServer();

    void   QueryMirrorNotificationInfo(bool Dss =true);

	DWORD  CreateHBSendNotificationThread();
	DWORD  CreateHBRescanDiskThread();
	void   StopHBSendNotificationThread();
	void   StopHBRescanDiskThread();

	void   PassEventHandleToDriver();
    void   OSNRescanDisk();
	void CheckLogfile();
	COSNService::COSNService();
	COSNService::~COSNService();
	int Open(char *volumeName);
	int OpenFile(int fileIndex);
	int Close();
	int CloseFile(HANDLE hFile);

	BOOL	ParseStandardArgs(int argc, char* argv[]);
    BOOL	IsInstalled();
    BOOL	Install();
    BOOL	Uninstall();

    void	OnStop();
    void	OnInterrogate();
    void	OnPause();
    void	OnContinue();
    void	OnShutdown();
    BOOL	OnUserControl(DWORD dwOpcode);

    void	SetStatus(DWORD dwState, DWORD waitHint = 0);

	BOOL	StartService();
	static void	WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI Handler(DWORD dwOpcode);

	BOOL	Initialize();
	BOOL	OnInit();

	DWORD			StartThread();
	DWORD			StopThread();

	void	LogMessage(char *msg);
	void	LogMessage(char *msg1, char *msg2);
	void	LogMessage(char *msg,  DWORD errorCode);
	void	LogMessageFile(char *msg);
	void  LogNotificationToFile(NOTIFICATION_OBJECT const * const pNotificationInfo);


    inline  char    * GetHostName() {return m_Hostname;}
	inline	void	IncrementThreadCount() { m_OutstandingThread++; }
	inline	void	DecrementThreadCount() { m_OutstandingThread--; }
	inline	DWORD	ThreadCount() { return m_OutstandingThread; }
    inline  int     GetLicenceKeyValue() {return m_LicencedFeatures;}
	
	DWORD		SendSocketCommand(char *pBuffer,
													unsigned int bufferSize,
													unsigned int command,
													 ULONG ipAddress);

	void			FreeAllocatedMemory();
	LONG GetExePath(char * pPath, unsigned int &dwSize);
	LONG	 GetRegistryValue(char *	pRegPath, 
							DWORD	dwValueType, 
							char *	pValueName, 
							char *	pValue, 
							DWORD &	dwValue, 
							DWORD & dwSize);
	
};



#endif //__OSNSERVICE_H__
