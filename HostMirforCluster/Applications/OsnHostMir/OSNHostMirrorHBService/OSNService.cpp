#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1

#include "stdio.h"
#include "stdlib.h"
#include "afx.h"
#include "atlbase.h"

#include "winsock2.h"
#include "winnt.h"
#include "winioctl.h"

//#include "..\common\OsnDefs.h"
//#include "..\common\OSNCommon.h"
//#include "..\common\OSNNdIoctl.h"
//#include "..\OSNApi\OSNApi2.h"
//#include "..\OSNApi\OSNApi.h"
#include "..\common\OSNLicence.h"
//#include "..\OsnGui\OsnDmApi\OsnDmApi.h"
#include "..\OsnVolumeCopyApi\Configure.h"
#include "Configure.h"
#include "..\OSNService\OSNRpcCommon.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"
#include "OSNRemoteServer.h"

#include "OSNService.h"


#include "tchar.h"

#define			MAX_LOGFILE_SIZE		10240000	//10M
#define			MAX_LFBUFFER_SIZE		102400		//100K
typedef BOOL	(WINAPI *FChangeServiceConfig2) (SC_HANDLE, DWORD, LPVOID);



COSNService			*pOSNService;
//char				g_ExeFilePath[MAX_PATH];


int main(int argc, char* argv[])
{
    // Create the service object
    pOSNService = new COSNService();

    // Parse for standard arguments (install, uninstall, version etc.)

    if (!pOSNService->ParseStandardArgs(argc, argv)) {

        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger
        // when the service is started.

        pOSNService->StartService();
    }

    // When we get here, the service has been stopped
    return pOSNService->m_Status.dwWin32ExitCode;
}


COSNService::COSNService()
{
    strcpy(m_szServiceName, "OSNHBService");
	strcpy(m_szServiceDisplayName, "EIM HostMirror HeartBeat Service");

    m_iMajorVersion = 1;
    m_iMinorVersion = 0;

    m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_Status.dwCurrentState = SERVICE_STOPPED;
    m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    m_Status.dwWin32ExitCode = 0;
    m_Status.dwServiceSpecificExitCode = 0;
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;

	m_LicenceChecked = false;
	m_LicencedFeatures=0;
	m_OutstandingThread = 0;

	m_dwTotalFreeBufferSize	= 0;
	m_bBufferFull			= false;

	ZeroMemory((PVOID)fileDirectory, MAX_PATH);

	m_nNumberOfClientHosts	= 0;
	m_nTotalOfClient		= 0;
	m_TrialVersion			= false;

	m_PassEventToDss        = false;
	m_PassEventToVss	= false;

	m_pRemoteServer= NULL;
	m_HBSendNotificationThreadHandle=NULL;
	m_HBDssSendNotificationEventHandle=NULL;
	m_HBVssSendNotificationEventHandle=NULL;

    m_CreateRescanEventToHostmirror = false;
	

}

COSNService::~COSNService()
{
	if (m_hLogfileEvent)
		CloseHandle(m_hLogfileEvent);
}

BOOL COSNService::StartService()
{

    SERVICE_TABLE_ENTRY st[] = {
        {m_szServiceName, ServiceMain},
        {NULL, NULL}
    };

    BOOL b = ::StartServiceCtrlDispatcher(st);
    return b;

}

void COSNService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{

    // Register the control request handler
    pOSNService->m_Status.dwCurrentState = SERVICE_START_PENDING;
    pOSNService->m_hServiceStatus = 
		RegisterServiceCtrlHandler(pOSNService->m_szServiceName,
									Handler);
    if (pOSNService->m_hServiceStatus == NULL) 
	{
        return;
    }

	if(pOSNService->Initialize())
	{
		pOSNService->StartThread();
		//pOSNService->OSNRescanDisk();
	}

	return;
}


void COSNService::Handler(DWORD dwOpcode)
{
   
    switch (dwOpcode) {
    case SERVICE_CONTROL_STOP: // 1
        pOSNService->SetStatus(SERVICE_STOP_PENDING);
        pOSNService->OnStop();
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pOSNService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pOSNService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pOSNService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pOSNService->OnShutdown();
        break;

    default:
		break;
    }

    // Report current status
    ::SetServiceStatus(pOSNService->m_hServiceStatus, &pOSNService->m_Status);
}


BOOL COSNService::ParseStandardArgs(int argc, char* argv[])
{
    // See if we have any command line args we recognise
    if (argc <= 1) return FALSE;

    if (_stricmp(argv[1], "-v") == 0) 
	{
        // Spit out version info
        printf("%s Version %d.%d\n",
               m_szServiceName, m_iMajorVersion, m_iMinorVersion);
        printf("The service is %s installed\n",
               IsInstalled() ? "currently" : "not");
        return TRUE; // say we processed the argument

    } 
	else if (_stricmp(argv[1], "-i") == 0) 
	{
        // Request to install.
        if (IsInstalled()) 
		{
            printf("%s is already installed\n", m_szServiceName);
        } 
		else 
		{
            // Try and install the copy that's running
            if (Install()) 
			{
                printf("%s installed\n", m_szServiceName);
            } 
			else 
			{
                printf("%s failed to install. Error %d\n", m_szServiceName, GetLastError());
            }
        }
        return TRUE; // say we processed the argument

    } 
	else if (_stricmp(argv[1], "-u") == 0) 
	{
        // Request to uninstall.
        if (!IsInstalled()) 
		{
            printf("%s is not installed\n", m_szServiceName);
        } 
		else 
		{
            // Try and remove the copy that's installed
            if (Uninstall()) 
			{
                // Get the executable file path
                char szFilePath[_MAX_PATH];
                ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
                printf("%s removed. (You must delete the file (%s) yourself.)\n",
                       m_szServiceName, szFilePath);
            } 
			else 
			{
                printf("Could not remove %s. Error %d\n", m_szServiceName, GetLastError());
            }
        }

        return TRUE; // say we processed the argument
    
    }
         
    // Don't recognise the args
    return FALSE;
}

BOOL COSNService::IsInstalled()
{
    BOOL bResult = FALSE;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) {

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
                                           m_szServiceName,
                                           SERVICE_QUERY_CONFIG);
        if (hService) {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

BOOL COSNService::Install()
{

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

	DWORD	dwErrorCode	= 0;
    // Get the executable file path
    char szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         m_szServiceName,
                                         m_szServiceDisplayName,
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         SERVICE_AUTO_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) 
	{
		dwErrorCode	= GetLastError();
        ::CloseServiceHandle(hSCM);
		SetLastError(dwErrorCode);
        return FALSE;
    }

	//Add service description
	HMODULE advapi32;
	FChangeServiceConfig2 PChangeServiceConfig2;
	
	/* try to link the Win2k function dynamically */
	advapi32 = LoadLibrary ("advapi32");
	if (advapi32)
	{
		PChangeServiceConfig2 = (FChangeServiceConfig2) GetProcAddress 
								(advapi32, "ChangeServiceConfig2A");

		/* If we got the Win2k function, prefer it over NT 4.0 */
		if (PChangeServiceConfig2) 
		{			
			SERVICE_DESCRIPTION sd;

			sd.lpDescription = __TEXT("OSNHBService.");				
			PChangeServiceConfig2 (hService,SERVICE_CONFIG_DESCRIPTION, (LPVOID)(&sd));		
		} 
		FreeLibrary (advapi32);
	}

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    char szKey[256];
    HKEY hKey = NULL;
    strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\OSNHBService");
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS) 
	{
		dwErrorCode	= GetLastError();
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
		SetLastError(dwErrorCode);
        return FALSE;
    }

	char szEventPath[_MAX_PATH * 2];
	//sprintf(szEventPath, "%%SystemRoot%%\\System32\\DcsMsg.dll;%s", szFilePath);
	sprintf(szEventPath, "%s", szFilePath);

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    "EventMessageFile",
                    0,
                    REG_EXPAND_SZ, 
                    (CONST BYTE*)szEventPath,
                    (DWORD)strlen(szEventPath) + 1);     

    ::RegCloseKey(hKey);

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return TRUE;
}

BOOL COSNService::Uninstall()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
	DWORD	dwErrorCode	= 0;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_szServiceName,
                                       DELETE);
    if (hService) 
	{
       ::DeleteService(hService);
        bResult = TRUE;
        ::CloseServiceHandle(hService);
    }
    
	dwErrorCode	= GetLastError();
    ::CloseServiceHandle(hSCM);
	SetLastError(dwErrorCode);


    return bResult;

}


BOOL COSNService::Initialize()
{
    SetStatus(SERVICE_START_PENDING);
    
	stopServiceEventHandle = ::CreateEvent(NULL,FALSE,FALSE,"StopHeatBeatServiceEvent");
 
	// Perform the actual initialization
    BOOL bResult = OnInit(); 
    
    // Set final state
    m_Status.dwWin32ExitCode = GetLastError();
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;

    if (!bResult) 
	{
        SetStatus(SERVICE_STOPPED);
        return FALSE;    
    }
    
    SetStatus(SERVICE_RUNNING);

	return TRUE;

}

LONG	 COSNService:: GetRegistryValue(char *	pRegPath, 
														DWORD	dwValueType, 
														char *	pValueName, 
														char *	pValue, 
														DWORD &	dwValue, 
														DWORD & dwSize)
{
	CRegKey	cRegKey;
	LONG	lReturn = cRegKey.Open(HKEY_LOCAL_MACHINE, pRegPath);
	
	if (lReturn != ERROR_SUCCESS)
		return lReturn;

	if (dwValueType == OSN_REGVALUE_PCHAR)
	{
		lReturn	= cRegKey.QueryStringValue(pValueName, pValue, &dwSize);
		pValue[dwSize]	= '\0';
	}
	else 
		lReturn	= cRegKey.QueryDWORDValue(pValueName, dwValue);

	cRegKey.Close();
	return lReturn;
}

LONG COSNService:: GetExePath(char * pPath, unsigned int &dwSize)
{

	if (pPath == NULL || dwSize == 0)
	{
		return ERROR_INVALID_PARAMETER;
	}

	DWORD	dwValue;
	LONG	lReturn;

	ULONG   size = dwSize;

	lReturn	= GetRegistryValue(OSNCLIENT_REG_SERVICE, 
									OSN_REGVALUE_PCHAR, 
									"ImagePath\0", 
									pPath, 
									dwValue, 
									size);

	_TCHAR	*pCharIndex;
	pCharIndex = _tcsrchr(pPath, '\\');
	if (pCharIndex)
	{
		*pCharIndex	= '\0';
		size	= pCharIndex-pPath;
	}

	dwSize=size;

	return lReturn;
}

BOOL COSNService::OnInit()
{
	CRegKey	aRegKey;
	unsigned int	size = 256;


	size = MAX_PATH;
	GetExePath(g_ExeFilePath, size);


	// create file "OSNHBService.log" for OSNHBService to log events
	DWORD	errorCode=0;
	FILE	*logFileStream;

	char logFile[MAX_PATH];
	sprintf(logFile, "%s\\OSNHostMirrorHBService.log", g_ExeFilePath);
	logFileStream = fopen(logFile, "a");
	if(!logFileStream)
	{
		return FALSE;
	}

	fclose(logFileStream);

	m_hLogfileEvent	= ::CreateEvent(NULL, FALSE, TRUE, "OSNHBLogfileEvent");
	if (!m_hLogfileEvent)
	{
		LogMessage("ERROR:Failed to create logfile event",GetLastError());
		return FALSE;
	}



	LogMessage("Execute Path = ", g_ExeFilePath);
	LogMessage("INFO: OSN HostMirror HB Service started\0");

	//get the computer's name
	m_HostnameLength = sizeof(m_Hostname);
	if (GetComputerName(m_Hostname, &m_HostnameLength))
	{
		m_Hostname[m_HostnameLength]='\0';
		LogMessage("INFO: The computer name is \0", m_Hostname);
	}
	else
	{
		errorCode	= GetLastError();
		LogMessage("WARNING: Failed to obtain the computer name\0", errorCode);
	}

	// get partner server name and IP address
	GetClusterServerInfo();



	//Gert MAC and inform OSNNd WWN
	/*ZeroMemory(&m_HostWWN, sizeof(WWN));
	if(OSNGetWWN(&m_HostWWN))
	{
		LogMessage("INFO: Local Host WWN = ", m_HostWWN.WWNString);
		OSNNotifyWWN(&m_HostWWN);
	}
	else
		LogMessage("WARNING: Failed to get Adapter Mac Address", GetLastError());*/


	//get IP Address

	/*char MacAddress[32];
	ZeroMemory(MacAddress, 32);
	if(OSNGetNetworkIpAddress(MacAddress,0))
	{
		m_IpAddress = OSNIpAddressCharToUlong(MacAddress);

		char	Msg[256];
		sprintf(Msg,"INFO: IP Address = %s, ULONG = %X", MacAddress, m_IpAddress);
		LogMessage(Msg);
		OSNNotifyIpAddress(m_IpAddress);
	}
	else
		LogMessage("WARNING: Failed to get Adapter IP Address", GetLastError());*/

	// read configuration
	return TRUE;

}

DWORD COSNService::StartThread()
{

	LogMessage("INFO: HeartBeat Service Main thread started.\0");
	DWORD status;

	status = m_OSNRpcServer.StartSocketThread();
	if(status)
	{
		LogMessage("ERROR: Failed to start Server HeartBeat Linsten Socket.",status);
	}
	else 
	{
		LogMessage("INFO: OSN HeartBeat Linsten Socket started.\0.",status);
	}

	status = m_OSNRpcServer.StartListenThread();//创建Thread,发送和接受远程过来的信息；
	if (status)
		LogMessage("ERROR: Failed to start Server HeartBeat Listen Thread.",status);
	else
		LogMessage("INFO: OSN HeartBeat Listen thread started.\0");

	status=CreateHBSendNotificationThread();//从驱动中获取notification发送远程
	if(status)
		LogMessage("ERROR:Failed to start HB Send Notification Thread.",status);
	else
		LogMessage("INFO: OSN HeartBeat Send Notification thread started.\0");

	status = CreateHBRescanDiskThread();
	if(status)
		LogMessage("Error:Failed to Start HB RescanDisk Thread.",status);
	else
		LogMessage("INFO:OSN Start HB Rescandisk Thread");
   

	DWORD	dwWaitResult;
	bool	bRunning = true;
	DWORD	dwErrorCode	= 0;


	DWORD	WaitCount = 0;

	while(bRunning)
	{
		
        PassEventHandleToDriver();

		if(!m_pRemoteServer || m_pRemoteServer->EmptyIPAddress())
		{
			GetClusterServerInfo();
		}
		
		dwWaitResult = ::WaitForSingleObject(stopServiceEventHandle, 300000);	//every 5 minutes
		switch (dwWaitResult)
		{
		case WAIT_TIMEOUT:
				break;

		case WAIT_OBJECT_0:
				bRunning = false;
				break;

		case WAIT_ABANDONED:
		default:
				;
		}

	}

	// stop service 

	LogMessage("INFO: OSN HostMirror HeartBeat Service has been stopped.\n");

	m_Status.dwWin32ExitCode = dwErrorCode;
	SetStatus(SERVICE_STOPPED);
	::ResetEvent(stopServiceEventHandle);

	

	return 0;

}




// Called when the service control manager wants to stop the service
// Cleanup
void COSNService::OnStop()
{
	   // stop the listen thread
	m_OSNRpcServer.StopListenThread();

	// stop the HeartBeat thread
	StopHBSendNotificationThread();
	StopHBRescanDiskThread();

	::SetEvent(stopServiceEventHandle);

	CloseHandle(m_HBRescanDiskEventHandle);
	CloseHandle(m_HBRescanDiskThreadHandle);
}

void COSNService::OnInterrogate()
{

}

void COSNService::OnPause()
{

}

void COSNService::OnContinue()
{

}

void COSNService::OnShutdown()
{
	LogMessage("INFO:Osn HeartBeat Service shutdown...\n");
	::SetEvent(stopServiceEventHandle);
}

BOOL COSNService::OnUserControl(DWORD dwOpcode)
{
	return FALSE;
}

void COSNService::SetStatus(DWORD dwState, DWORD waitHint)
{
    m_Status.dwCurrentState = dwState;  
	m_Status.dwWaitHint = waitHint;
    ::SetServiceStatus(m_hServiceStatus, &m_Status);
}

void COSNService::LogMessage(char *msg)
{
	char timeStamp[512];
	SYSTEMTIME	currentTime;
	GetLocalTime(&currentTime);
	sprintf(timeStamp,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d - %s.\n\0",
		currentTime.wMonth, currentTime.wDay, currentTime.wYear,
		currentTime.wHour, currentTime.wMinute, currentTime.wSecond,
		msg);

	LogMessageFile(timeStamp);
}

void COSNService::LogMessage(char *msg1, char *msg2)
{
	char timeStamp[256];
	SYSTEMTIME	currentTime;
	GetLocalTime(&currentTime);
	sprintf(timeStamp,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d - %s%s.\n\0",
		currentTime.wMonth,currentTime.wDay,currentTime.wYear,
		currentTime.wHour,currentTime.wMinute, currentTime.wSecond,
		msg1,msg2);

	LogMessageFile(timeStamp);
}

void COSNService::LogMessage(char *msg, DWORD errorCode)
{
	char timeStamp[256];
	SYSTEMTIME	currentTime;
	GetLocalTime(&currentTime);
	sprintf(timeStamp,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d - %s. Errorcode=%d\n\0",
		currentTime.wMonth,currentTime.wDay,currentTime.wYear, 
		currentTime.wHour,currentTime.wMinute, currentTime.wSecond,
		msg,errorCode);

	LogMessageFile(timeStamp);
}


void COSNService::LogMessageFile(char *msg)
{
	if(!msg)
		return;

	//wait untill the event is signaled
	WaitForSingleObject(m_hLogfileEvent, INFINITE);
	//set the event non-signaled automatically

	//check the logfile size. Truncated it to 100K if it is bigger than 10M.
	CheckLogfile();

	char logFilename[MAX_PATH];
	sprintf(logFilename, "%s\\OSNHeartBeatService.log", g_ExeFilePath);

	FILE	*logFile;
	logFile = fopen(logFilename, "a");
	if(!logFile)
	{
		::SetEvent(m_hLogfileEvent);
		return;
	}

	fprintf(logFile, msg);
	fclose(logFile);

	::SetEvent(m_hLogfileEvent);
}

void COSNService::CheckLogfile()
{
	char	logFilename[MAX_PATH];
	char    logFileBakName[MAX_PATH];
	sprintf(logFilename, "%s\\OSNHeartBeatService.log", g_ExeFilePath);
    sprintf(logFileBakName,"%s\\OSNHeartBeatService.log.bak", g_ExeFilePath);
    

	HANDLE	hLogfile;
	hLogfile	= CreateFile(logFilename, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hLogfile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwFileSize;
	dwFileSize	= GetFileSize(hLogfile, NULL);
	if (dwFileSize == -1)
	{
		CloseHandle(hLogfile);
		return;
	}

	if (dwFileSize >= MAX_LOGFILE_SIZE)
	{
		CloseHandle(hLogfile);
		CopyFile(logFilename, logFileBakName, false);
		DeleteFile(logFilename);
		//DeleteFile(baklogFilename);
		
		hLogfile	= CreateFile(logFilename, 
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
		if (hLogfile == INVALID_HANDLE_VALUE)
			return;

		CloseHandle(hLogfile);
	}
	else
		CloseHandle(hLogfile);

}



DWORD WINAPI COSNService:: OSNHBSendNotificationThread(void *pData)
{
	    
	    COSNService *pOSNService = (COSNService *) pData;
		if (!pOSNService)
		{
			return ERROR_INVALID_PARAMETER;
		}

		DWORD dwWaitResult=0;
		pOSNService->m_HBThreadRunFlag=true;
		pOSNService->IncrementThreadCount();

		while (pOSNService->m_HBThreadRunFlag)
		{
			pOSNService->HandleDssNotification();
			pOSNService->HandleVssNotification();

			// send to remote server
			pOSNService->SendNotificationToRemoteServer();
		}

		pOSNService->DecrementThreadCount();

		
		CloseHandle(pOSNService->m_HBSendNotificationThreadHandle);
		pOSNService->m_HBSendNotificationThreadHandle=INVALID_HANDLE_VALUE;

		pOSNService->LogMessage("INFO: HeartBeat Server thread stopped");
		return 0;			//thread function return


}

DWORD COSNService::CreateHBSendNotificationThread()
{
	DWORD ThreadID;
	this->m_HBSendNotificationThreadHandle=CreateThread(NULL,
														0,
														OSNHBSendNotificationThread,
														(void *)this,
														0,
														&ThreadID);
	if(this->m_HBSendNotificationThreadHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	return 0;
}

DWORD COSNService::CreateHBRescanDiskThread()
{
	DWORD ThreadID;
	this->m_HBRescanDiskThreadHandle = CreateThread(NULL,
													0,
													OSNHBRescanDiskThread,
													(void *)this,
													0,
													&ThreadID);
	if(this->m_HBRescanDiskThreadHandle == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	return 0;
}

void COSNService::StopHBRescanDiskThread()
{
	  m_HBRescanDiskThreadRunFlag=false;

	 if(m_HBRescanDiskEventHandle)
	 {
		::SetEvent(m_HBRescanDiskEventHandle);
	 }

     // stop the thread
	  DWORD ExitCode =0;
	  if(m_HBRescanDiskThreadHandle!=INVALID_HANDLE_VALUE)
	  {
			GetExitCodeThread(m_HBRescanDiskThreadHandle,&ExitCode);
			TerminateThread(m_HBRescanDiskThreadHandle,ExitCode);
	  }
}

DWORD  WINAPI COSNService::OSNHBRescanDiskThread(void *pData)
{
	COSNService *pOSNService = (COSNService *) pData;
		if (!pOSNService)
		{
			return ERROR_INVALID_PARAMETER;
		}

		DWORD dwWaitResult=0;
		pOSNService->m_HBRescanDiskThreadRunFlag =true;
		

		while (pOSNService->m_HBRescanDiskThreadRunFlag)
		{
		
			if(!pOSNService->m_HBRescanDiskEventHandle)
			{
				Sleep(5000);
				continue;
			}

			dwWaitResult = ::WaitForSingleObject(pOSNService->m_HBRescanDiskEventHandle, 300000);	//every  5 mins

			switch (dwWaitResult)
			{
				case WAIT_TIMEOUT:
					{
						break;
					}

				case WAIT_OBJECT_0:
					{
						pOSNService->OSNRescanDisk();
						
						break;
					}
			
				case WAIT_ABANDONED:
				default:
					
					;
			}
		}

		CloseHandle(pOSNService->m_HBRescanDiskThreadHandle);
		pOSNService->m_HBRescanDiskThreadHandle=INVALID_HANDLE_VALUE;
		
		pOSNService->LogMessage("INFO:HeartBeat Rescan thread stopped");
		return 0;			//thr

}

void COSNService::StopHBSendNotificationThread()
{
	m_HBThreadRunFlag=false;

	if(m_HBDssSendNotificationEventHandle)
	{
		::SetEvent(m_HBDssSendNotificationEventHandle);
	}
	Sleep(200);

	  // stop the thread
	  DWORD ExitCode =0;
	  if(m_HBSendNotificationThreadHandle!=INVALID_HANDLE_VALUE)
	  {
			GetExitCodeThread(m_HBSendNotificationThreadHandle,&ExitCode);
			TerminateThread(m_HBSendNotificationThreadHandle,ExitCode);
	  }

	FreeAllocatedMemory();	
}

void COSNService::SendNotificationToRemoteServer()
{
	
	ULONG ErrorCode=0;
    ULONG NotificationCount=0;

	if(!m_pRemoteServer)
	{
		return;
	}
		
	NotificationCount=m_pRemoteServer->GetNumberOfNotification();
	if(NotificationCount==0)
	{
		return;
	}

	// send notification info
	if(NotificationCount!=0)
	{
		ErrorCode= m_pRemoteServer-> _SendNotificationToRemoteServer(m_pRemoteServer->GetIPAddress(0),
																								 NotificationCount);
		if(2==ErrorCode)
		{
			ErrorCode= m_pRemoteServer-> _SendNotificationToRemoteServer(m_pRemoteServer->GetIPAddress(1),
																									NotificationCount);
		}
	}

	if(0 == ErrorCode)
	{
		m_pRemoteServer->CheckNotification();
	}
}

DWORD	COSNService::	SendSocketCommand(char *pBuffer,
													unsigned int bufferSize,
													unsigned int command,
													 ULONG ipAddress)
	{
		
		if(bufferSize>OSNRPC_HBMAX_MSG_LEN-OSNRPC_HBMSGHEAD_LEN)
		{
			LogMessage("ERROR: Message to be send exceed the total socket buffer.\n");
			return -1;
		}

		//new m_socket
		SOCKET ConnectSocket;
		ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (ConnectSocket == INVALID_SOCKET)
		{
			LogMessage("ERROR:Invalid socket",WSAGetLastError());
			return WSAGetLastError();
		}
		
	   char msg[OSNRPC_HBMAX_MSG_LEN];
	   memset(msg,0,sizeof(msg));

	   HB_MESSAGE_HEADER	messageHeader;
       memset(&messageHeader,0,sizeof(HB_MESSAGE_HEADER));


       memcpy(&msg[OSNRPC_HBMSGHEAD_LEN],pBuffer,bufferSize);
	   msg[OSNRPC_HBMSGHEAD_LEN+bufferSize]='\0';

	   messageHeader.command = command;
	   //memcpy(messageHeader.initiatorWWN,	&m_HostWWN, sizeof(WWN));
	   //messageHeader.initiatorWWN[sizeof(WWN)] = '\0';
	   memcpy(messageHeader.initiatorName,	m_Hostname, m_HostnameLength);
	   messageHeader.initiatorName[m_HostnameLength] = '\0';
	   messageHeader.paraNum=1;
	   messageHeader.retStatus=CMD_STATUS_INIT;
	   messageHeader.flags	  = OSNRPC_HBCMD_ASK;
	   messageHeader.dataLength =bufferSize;

	   // set the massage header
	   memcpy(msg,&messageHeader,sizeof(HB_MESSAGE_HEADER));

	   SOCKADDR_IN targetSocket;
	   targetSocket.sin_family=AF_INET;
	   targetSocket.sin_port=htons(OSNRPC_HBSOCKET_SERVICE_PORT);

	   in_addr	addr;
	   addr.S_un.S_addr = ipAddress;
	   targetSocket.sin_addr	= addr; 

	   if(connect(ConnectSocket,(struct sockaddr *)&targetSocket,sizeof(SOCKADDR_IN)))
	   {
		   LogMessage("ERROR:Socket Connect failed",WSAGetLastError());
		   return WSAGetLastError();
	   }

		if(!m_OSNRpcServer.OSNRpcSendMsg(targetSocket, 
										msg, 
										sizeof(HB_MESSAGE_HEADER) + messageHeader.dataLength,
										ConnectSocket))
		{
			LogMessage("ERROR:Failed to send message",WSAGetLastError());
			return 1;
		}
        
		timeval overtime;
		overtime.tv_sec=4;
		overtime.tv_usec=0;
		int Rresult=setsockopt(ConnectSocket,
										SOL_SOCKET,SO_RCVTIMEO,
										(char*)&overtime,
										sizeof(timeval));
		if(Rresult)
		{
			LogMessage("ERROR:Failed to set soket option ",WSAGetLastError());
			shutdown(ConnectSocket,SD_BOTH);
			closesocket(ConnectSocket);
			return 1;
		}

		DWORD m_nError=recv(ConnectSocket,msg,OSNRPC_HBMAX_MSG_LEN,0);
		if (m_nError == SOCKET_ERROR)					
		{
			shutdown(ConnectSocket,SD_BOTH);
			closesocket(ConnectSocket);
			LogMessage("ERROR:Failed to recv message from remote socket",WSAGetLastError());
			return WSAGetLastError();
		}
          
		memcpy(&messageHeader,msg,OSNRPC_HBMSGHEAD_LEN);
		memcpy(pBuffer,&msg[OSNRPC_HBMSGHEAD_LEN],bufferSize);

		if(messageHeader.retStatus!=CMD_HBSTATUS_SUCCESS)
		{
			return 1;
		}

		shutdown(ConnectSocket,SD_BOTH);
		closesocket(ConnectSocket);

		return 0;

	}

DWORD COSNService::GetIPAddressByHostName(char *pServerName,ULONG *pIPAddress)
{
	 if(!pServerName)
	 {   
		 LogMessage("ERROR:Invalid ServerName");
		 return 1;
	 }
	 hostent *hostEnt=NULL;
	 char msg[128];
	 hostEnt = gethostbyname(pServerName);
	 if(hostEnt!=NULL)
	 {
		 *pIPAddress=((in_addr*)hostEnt->h_addr)->s_addr;
		 sprintf(msg,"INFO: Server %s,IP Address= %X", pServerName,*pIPAddress);
		 LogMessage(msg);
		 return 0;
	 }
	 else
	 {
		 sprintf(msg,"ERROR:Failed to get IPAddress,ServerName=%s,errorcode=%d",pServerName,WSAGetLastError());
		 LogMessage(msg);
		 return 1;
	 }
    
}

void  COSNService::LogNotificationToFile(NOTIFICATION_OBJECT const * const pNotificationInfo)
{
	
	char szMsg[256];

	sprintf(szMsg,"INFO: Save notification(%.8X_%.16I64X_%.8X),bitvalue=%d.",
			pNotificationInfo->m_SrcId.MBP_VolumeID.m_DiskSignature,
			pNotificationInfo->m_SrcId.MBP_VolumeID.m_BlockOffset,
			pNotificationInfo->m_SrcId.MBP_VolumeID.m_NotUsed1,
			pNotificationInfo->m_NotificationBit);
			
	LogMessage(szMsg);

}

// 从驱动中获取NOTIFICATION LIST
void  COSNService::QueryMirrorNotificationInfo(bool Dss)
{
	if(!m_pRemoteServer)
	{
		return;
	}

	unsigned int size=16*sizeof(MIRROR_NOTIFICATION_INFO)+sizeof(MIRROR_NOTIFICATION_LIST);
	char *pBuffer=new char[size];
	unsigned Count=16;
	if(!pBuffer)
	{
		LogMessage("ERROR:Failed to allocate memory for getting mirror notification list");
		return;
	}

	memset(pBuffer,0,size);
	DWORD ErrorCode=0;
	
	if(Dss)
	{
		ErrorCode = OSNDssGetMirrorNotificationList(pBuffer,&size);
	}
	else
	{
		ErrorCode = OSNVssGetMirrorNotificationList(pBuffer,&size);
	}

	if(ErrorCode)
	{
		LogMessage("ERROR:Failed to get MirrorNotificationList\n",ErrorCode);

		delete [] pBuffer;
		return;
	}

    MIRROR_NOTIFICATION_LIST *pList=(MIRROR_NOTIFICATION_LIST *)(pBuffer);
	MIRROR_NOTIFICATION_INFO *pMirrorNotificationInfo=NULL;

	NOTIFICATION_OBJECT *pTempInfo=NULL;
	char *pNotificationInfoBuffer=NULL;
	for(unsigned int i=0;i<pList->m_Count;i++)
	{
		pMirrorNotificationInfo=(MIRROR_NOTIFICATION_INFO *)&pList->m_Info[i];
		if(0 == pMirrorNotificationInfo->m_NotificationBit)
		{
			continue;
		}

		pTempInfo=m_pRemoteServer->GetNotificationInfoByGuid(pMirrorNotificationInfo->m_SrcId.SAN_VolumeID.m_VolumeGuid);
		if(!pTempInfo)
		{
			pNotificationInfoBuffer=new char[sizeof(NOTIFICATION_OBJECT)];
			if(!pNotificationInfoBuffer)
			{
				LogMessage("ERROR:Failed to new NOTIFICATION_INFO Object");
				continue;
			}

		    pTempInfo=(NOTIFICATION_OBJECT *)(pNotificationInfoBuffer);

			memcpy(&pTempInfo->m_SrcId,&pMirrorNotificationInfo->m_SrcId,sizeof(VOLUMEID));
			pTempInfo->m_NotificationBit=pMirrorNotificationInfo->m_NotificationBit;
			pTempInfo->m_HBServiceCheck=pTempInfo->m_NotificationBit;
			pTempInfo->m_NotSuccess = 0;
			pTempInfo->m_DssNotification=pMirrorNotificationInfo->m_DssNotification;
			m_pRemoteServer->InsertNotificationInfotoServer(pTempInfo);
			
		}	
	}

	delete [] pBuffer;
}

void   COSNService::PassEventHandleToDriver()//在驱动创建时间，获取事件handle
{
	
	DWORD ErrorCode=0;
	DWORD VssErrorCode = 0;
	if(!m_PassEventToDss)
	{
		ErrorCode=OSNCreateDssNotificationEvent();
		if(!ErrorCode)
		{
			LogMessage("INFO:Create Dss Notification Event successfully");
			m_HBDssSendNotificationEventHandle=::OpenEvent(SYNCHRONIZE,	FALSE,"OSNDSSHBNotificationEvent");
			//m_HBDssSendNotificationEventHandle=::OpenEvent(SYNCHRONIZE,FALSE,"OSNDSSHBNotificationEvent");
			if(m_HBDssSendNotificationEventHandle)
			{
			   m_PassEventToDss=true;
			   QueryMirrorNotificationInfo();
			   char name[128];
			   sprintf(name,"INFO:Open dss event handle ,handle=%x",m_HBDssSendNotificationEventHandle);
			   LogMessage(name);

			}
			else
			{
				LogMessage("INFO:Failed to open dss event.",GetLastError());
			}
		}
		else
		{
			LogMessage("INFO:Failed to create dss Notification Event .",ErrorCode);
		}
	}

	if(!m_PassEventToVss)
	{
		ErrorCode=OSNCreateVssNotificationEvent();
		if(!ErrorCode)
		{
			LogMessage("INFO:Create Vss Event successfully");
			m_HBVssSendNotificationEventHandle=::OpenEvent(SYNCHRONIZE,FALSE,"OSNVSSHBNotificationEvent");
			if(m_HBVssSendNotificationEventHandle)
			{
			   m_PassEventToVss=true;
			   QueryMirrorNotificationInfo(false);
			   char name[128];
			   sprintf(name,"INFO:Open vss event handle ,handle=%x",m_HBVssSendNotificationEventHandle);
			   LogMessage(name);

			}
			else
			{
				LogMessage("INFO:Failed to open vss rescan event.",GetLastError());
			}
		}
		else
		{
			LogMessage("INFO:Failed to create vss Event.",ErrorCode);
		}
	}

	if(!m_CreateRescanEventToHostmirror)
	{
		ErrorCode=OSNCreateDssRescanEvent();
		VssErrorCode = OSNCreateVssRescanEvent();
		if(!ErrorCode&&!VssErrorCode)
		{
			LogMessage("INFO:Create Rescan and open Event successfully");
			m_HBRescanDiskEventHandle=::OpenEvent(SYNCHRONIZE,FALSE,"OSNHBRescanEvent");
			if(m_HBRescanDiskEventHandle)
			{
			   m_CreateRescanEventToHostmirror=true;
			   char name[128];
			   sprintf(name,"INFO:Open rescan event handle ,handle=%x",m_HBRescanDiskEventHandle);
			   LogMessage(name);

			}
			else
			{
				LogMessage("INFO:Failed to open the rescan event.",GetLastError());
			}
		}
		else
		{
			LogMessage("INFO:Failed to create Rescan Event.",ErrorCode);
		}
	}

	return;
}

void COSNService::FreeAllocatedMemory()
{
	if(m_pRemoteServer)
	{
		m_pRemoteServer->FreeMemory();
		delete m_pRemoteServer;
	}
}


void COSNService::HandleDssNotification()
{
	if(!pOSNService->m_HBDssSendNotificationEventHandle)
	{
		Sleep(5000);
		return;
	}

	DWORD dwWaitResult = ::WaitForSingleObject(pOSNService->m_HBDssSendNotificationEventHandle, 
														   3000);	//every  3 sec
	
	switch (dwWaitResult)
	{
		case WAIT_TIMEOUT:
			{
				if(pOSNService->m_pRemoteServer)
					OSNDssSignaledTheEvent();
				break;
			}

		case WAIT_OBJECT_0:
			{
				if(pOSNService->m_HBThreadRunFlag)
				{
					pOSNService->QueryMirrorNotificationInfo();
				}
				break;
			}
	
		case WAIT_ABANDONED:
		default:
			;
	}
}

void COSNService::HandleVssNotification()
{
	if(!pOSNService->m_HBVssSendNotificationEventHandle)
	{
		Sleep(5000);
		return;
	}

	DWORD	dwWaitResult = ::WaitForSingleObject(pOSNService->m_HBVssSendNotificationEventHandle, 
														   3000);	//every  3 sec

	switch (dwWaitResult)
	{
		case WAIT_TIMEOUT:
			{
				if(pOSNService->m_pRemoteServer)
				OSNVssSignaledTheEvent();
				break;
			}

		case WAIT_OBJECT_0:
			{
				if(pOSNService->m_HBThreadRunFlag)
				{
					pOSNService->QueryMirrorNotificationInfo(false);
				}
				break;
			}
	
		case WAIT_ABANDONED:
		default:
			;
	}

}

void COSNService::GetClusterServerInfo()
{
	CRegKey HBServiceKey;
	ULONGLONG  IPAddress=0;
	char   ServerName[128];
	ULONG Size =sizeof(ServerName);
	memset(ServerName,0,sizeof(ServerName));
	
	if(HBServiceKey.Open(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services\\OSNHBService\\Parameters")==ERROR_SUCCESS)
	{
		// query cluster server name
		if(HBServiceKey.QueryStringValue("PartnerName",ServerName,&Size)==ERROR_SUCCESS)
		{
			if(!m_pRemoteServer)
			{
				m_pRemoteServer = new CRemoteServer(ServerName);
			}
			//LogMessage("PatenerName is OK");
			//GetIPAddressByHostName(ServerName,&IPAddress);
		}

		if(!m_pRemoteServer)
		{
			HBServiceKey.Close();
			return;
		}

		// query ip address
		if(HBServiceKey.QueryQWORDValue("PriIPAddress",IPAddress)== ERROR_SUCCESS)
		{
			m_pRemoteServer->SetIPAddress(0,IPAddress);
		}

		if(HBServiceKey.QueryQWORDValue("SecIPAddress",IPAddress)== ERROR_SUCCESS)
		{
			m_pRemoteServer->SetIPAddress(1,IPAddress);
		}

		HBServiceKey.Close();
	}
	else if(HBServiceKey.Create(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services\\OSNHBService\\Parameters")==ERROR_SUCCESS)
	{
		// query cluster server name
		if(HBServiceKey.QueryStringValue("PartnerName",ServerName,&Size)==ERROR_SUCCESS)
		{
			if(!m_pRemoteServer)
			{
				m_pRemoteServer = new CRemoteServer(ServerName);
			}
		}

		if(!m_pRemoteServer)
		{
			HBServiceKey.Close();
			return;
		}

		// query ip address
		if(HBServiceKey.QueryQWORDValue("PriIPAddress",IPAddress)== ERROR_SUCCESS)
		{
			m_pRemoteServer->SetIPAddress(0,IPAddress);
		}

		if(HBServiceKey.QueryQWORDValue("SecIPAddress",IPAddress)== ERROR_SUCCESS)
		{
			m_pRemoteServer->SetIPAddress(1,IPAddress);
		}

		HBServiceKey.Close();
	}
}
void COSNService::OSNRescanDisk()
{
	char AppName[MAX_PATH]; 
	char CmdLine[MAX_PATH];
    
	memset(AppName,0,sizeof(AppName));
	memset(CmdLine,0,sizeof(CmdLine));

	sprintf(AppName, "%s\\inf\\OsnDevCn.exe", g_ExeFilePath);
	sprintf(CmdLine,"%s","-r rescan");
	
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory (&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
    if( !CreateProcess(AppName,   // module name
        CmdLine,                  // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)         // Pointer to PROCESS_INFORMATION structure
		)
	{
        sprintf( AppName,"Exec Rescan command failed (%d)", GetLastError());
        return;
	}

    
    if(WAIT_TIMEOUT==WaitForSingleObject(pi.hProcess, 10000))//after 10000,terminate process if the process have not terminate
   {
	   DWORD ExitCode =0;
	   GetExitCodeProcess(pi.hProcess,&ExitCode);
	   TerminateProcess(pi.hProcess,ExitCode);
   }

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle( pi.hThread );
	
	return;  
}