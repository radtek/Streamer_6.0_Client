#include <fstream> 
#include "OsnMirrorCopyXML.h"
#include "Configure.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"
#include "OSNService.h"
#include "NewMirror.h"
#include "atlbase.h"


extern	COSNService			*pOSNService;

COsnMirrorCopyXML::COsnMirrorCopyXML()
{
	m_ClientID         = NULL;
	m_ServerIP         = NULL;
	m_ServerID         = NULL;
	m_TargetIqn       = NULL;
	m_TargetIPs        = NULL;

	pVolumeMirrorList  = NULL;
	pDiskMirrorList    = NULL;
	pVolumeList        = NULL;
	pDiskList          = NULL;

	ImagePath          = NULL;
	m_pTempXML         = NULL;

	this->pVolumeMirrorList = new CMirrorInfoList(128);
	this->pDiskMirrorList = new CMirrorInfoList(128);
	this->pVolumeList = new CVolumeInfoList(256);
	this->pDiskList = new CDiskInfoList(256);

	if(QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ClientID",&m_ClientID,StringKey) != ERROR_SUCCESS)
	{
		GUID guid;
		CoCreateGuid(&guid);
		OsnGUIDToString(m_ClientID,guid);
		SetRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ClientID",&m_ClientID,StringKey);
	}

	if(QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","Protected",&m_IsProtected,BoolKey) == ERROR_SUCCESS)
	{
		if(m_IsProtected == true)
		{
			QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIP",&m_ServerIP,StringKey);
			QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerID",&m_ServerID,StringKey);
			QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIqn",&m_TargetIqn,StringKey);
			QueryRegKey("SYSTEM\\CurrentControlSet\\Services\\OSNHCService","ServerIqn",&m_TargetIPs,StringKey);
		}
	}
	else
	{
		m_IsProtected = false;
	}

	ImagePath = new wstring(L"C:\\Program Files\\Enterprise Information Management\\OSN HostMirror");
	InitializeMembers();
}

COsnMirrorCopyXML::~COsnMirrorCopyXML()
{
	if(m_pTempXML != NULL)
		delete(m_pTempXML);

	if(pVolumeMirrorList != NULL)
		delete(pVolumeMirrorList);

	if(pDiskMirrorList != NULL)
		delete(pDiskMirrorList);

	if(pVolumeList != NULL)
		delete(pVolumeList);

	if(pDiskList != NULL)
		delete(pDiskList);

	if(ImagePath != NULL)
		delete(ImagePath);

	if(m_ClientID != NULL)
		delete(m_ClientID);

	if(m_ServerIP != NULL)
		delete(m_ServerIP);

	if(m_ServerID != NULL)
		delete(m_ServerID);

	if(m_TargetIqn != NULL)
		delete(m_TargetIqn);

	if(m_TargetIPs != NULL)
		delete(m_TargetIPs);

	return ;
}

DWORD COsnMirrorCopyXML::SetRegKey(char *pKeyName,char *pValueName,void *pValue,RegKey sign)
{
	CRegKey *pRegKey = new CRegKey();
	DWORD ret = 0;

	if(pRegKey->Open(HKEY_LOCAL_MACHINE,pKeyName)!= ERROR_SUCCESS)
	{
		if(pRegKey->Create(HKEY_LOCAL_MACHINE,pKeyName) != ERROR_SUCCESS)
		{
			delete(pRegKey);
			return EXIT_FAILURE;
		}
	}

	switch(sign)
	{
	case BoolKey:
		{
			if(*(bool *)pValue == true)
			{
				ret = pRegKey->SetStringValue(pValueName,"true");
			}
			else
			{
				ret = pRegKey->SetStringValue(pValueName,"false");
			}
		}
		break;
	case StringKey:
		{
			ret = pRegKey->SetStringValue(pValueName,(char*)pValue);
		}
		break;

	case DwordKey:
		{
			ret = pRegKey->SetDWORDValue(pValueName,*((DWORD*)pValue));
		}
		break;
	}
	pRegKey->Close();
	delete(pRegKey);
	return ret;
}

DWORD COsnMirrorCopyXML::QueryRegKey(char *pKeyName,char *pValueName,void *pValue,RegKey sign)
{
	CRegKey *pRegKey = new CRegKey();
	ULONG nChars = 0;
	DWORD ret = 0;
	char  pValueMir[10];

	if(pRegKey->Open(HKEY_LOCAL_MACHINE,pKeyName)!= ERROR_SUCCESS)
	{
		delete(pRegKey);
		return EXIT_FAILURE;
	}

	switch(sign)
	{
	case BoolKey:
		{
			nChars = sizeof(pValueMir);
			ret = pRegKey->QueryStringValue(pValueName,pValueMir,&nChars);
			if(strcpy_s(pValueMir,sizeof(pValueMir),"true") == 0)
			{
				*(bool*)pValue = true;
			}
			else
			{
				*(bool*)pValue = false;
			}
		}
		break;

	case StringKey:
		{
			nChars = 128;
			char **pValueSec = (char **)pValue;
			*pValueSec = (char *)malloc(128+1);
			ret = pRegKey->QueryStringValue(pValueName,*pValueSec,&nChars);
		}
		break;

	case DwordKey:
		{
			ret = pRegKey->QueryDWORDValue(pValueName,*((DWORD*)pValue));
		}
		break;
	}
	pRegKey->Close();
	delete(pRegKey);
	return ret;
}

//DWORD COsnMirrorCopyXML::QueryClientID()
//{
//	CRegKey *pRegKey = new CRegKey();
//	ULONG nChars = sizeof(ClientID);
//
//	if(pRegKey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNHCService") == ERROR_SUCCESS)
//	{
//		if(pRegKey->QueryStringValue("ClientID",ClientID,&nChars) != ERROR_SUCCESS)
//		{
//			pRegKey->Close();
//			delete(pRegKey);
//			return EXIT_FAILURE;
//		}
//
//		pRegKey->Close();
//		delete(pRegKey);
//		return EXIT_SUCCESS;
//	}
//	else
//	{
//		delete(pRegKey);
//		return EXIT_FAILURE;
//	}
//}
//
//DWORD COsnMirrorCopyXML::CreateClientID()
//{
//	GUID guid;
//	ULONG nChars = sizeof(ClientID);
//	CRegKey *pRegKey = new CRegKey();
//
//	if(S_OK == CoCreateGuid(&guid))
//	{
//		OsnGUIDToString(ClientID,guid);
//		if(pRegKey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNHCService")!= ERROR_SUCCESS)
//		{
//			if(pRegKey->Create(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNHCService") != ERROR_SUCCESS)
//			{
//				delete(pRegKey);
//				return EXIT_FAILURE;
//			}
//
//			pRegKey->SetStringValue("ClientID",ClientID);
//
//			pRegKey->Close();
//			delete(pRegKey);
//			return EXIT_SUCCESS;
//		}
//		else
//		{
//			if(pRegKey->QueryStringValue("ClientID",ClientID,&nChars) != ERROR_SUCCESS)
//			{
//				pRegKey->SetStringValue("ClientID",ClientID);
//			}
//
//			pRegKey->Close();
//			delete(pRegKey);
//			return EXIT_SUCCESS;
//		}
//	}
//
//	delete(pRegKey);
//	return EXIT_FAILURE;
//}

void COsnMirrorCopyXML::InitializeMembers()
{
	GetSystemDisksInfo();
	GetSystemVolumesInfo();
	ReadConfigurationFile();

	/*try
	{*/
		GetSystemMirrorInfo();
		//RefreshComputerNode();
	/*}
	catch(Exception^ exx)
	{
	myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",exx->Message->ToString()),EventLogEntryType::Error,024);
	}*/
}

DWORD COsnMirrorCopyXML::OSNInitWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,wchar_t *pResName)
{
	HRESULT hres;
	hres = CoCreateInstance(
		CLSID_WbemLocator,            
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) &(*m_pLoc));

	if (FAILED(hres))
	{
		printf("CoCreateInstance error\n");
		//CoUninitialize();
		return EXIT_FAILURE;       // Program has failed.
	}

	//使用pLoc连接到” root\cimv2” 并把pSvc的指针也搞定了
	hres = (*m_pLoc)->ConnectServer(
		_bstr_t(pResName), // WMI namespace
		NULL,                    // User name
		NULL,                    // User password
		0,                       // Locale
		NULL,                    // Security flags                
		0,                       // Authority      
		0,                       // Context object
		&(*m_pSvc)                    // IWbemServices proxy
		);                             
	if (FAILED(hres))
	{
		printf("ConnectServer error\n");
		//m_pLoc->Release();    
		//CoUninitialize();
		return EXIT_FAILURE;                // Program has failed.
	}
	//已经连接到WMI了

	hres = CoSetProxyBlanket(
		(*m_pSvc),                         // the proxy to set
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity
		EOAC_NONE                     // proxy capabilities
		);

	if (FAILED(hres))
	{
		printf("CoSetProxyBlanket error\n");
		//m_pSvc->Release();
		//m_pLoc->Release();    
		//CoUninitialize();
		return EXIT_FAILURE;               // Program has failed.
	}

	return EXIT_SUCCESS;
}

DWORD COsnMirrorCopyXML::OSNCloseWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,IEnumWbemClassObject **pEnumerator)
{
	if((*m_pSvc) != NULL)
	{
		(*m_pSvc)->Release();
		m_pSvc = NULL;
	}

	if((*m_pLoc) != NULL)
	{
		(*m_pLoc)->Release();
		(*m_pLoc) = NULL;
	}

	if((*pEnumerator) != NULL)
	{
		(*pEnumerator)->Release();
		(*pEnumerator) = NULL;
	}

	return EXIT_SUCCESS;
}

DWORD COsnMirrorCopyXML::CharToWchar(const char *pChar,wchar_t *pWchar,int Length)
{
	if(!pChar || !pWchar)
	{
		return EXIT_FAILURE;
	}
	int   nLen  = strlen(pChar) + 1;      
	int   nwLen = MultiByteToWideChar(CP_ACP,0,pChar,nLen,NULL,0);

	if(Length < nwLen)
		return EXIT_FAILURE;

	if(0 == MultiByteToWideChar(CP_ACP,0,pChar,nLen,pWchar,nwLen))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

DWORD COsnMirrorCopyXML::WcharToChar(const wchar_t *pWchar,char *pChar,int Length)
{
	if(!pChar || !pWchar)
	{
		return EXIT_FAILURE;
	}       

	int   nLen = wcslen(pWchar)+1;      
	if(Length < nLen)
	{
		return EXIT_FAILURE;
	}

	if(0 == WideCharToMultiByte(CP_ACP,0,pWchar,nLen,pChar,2*nLen,NULL,NULL)) 
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

void COsnMirrorCopyXML::GetSystemVolumesInfo()
{
	this->pVolumeList ->Clear();

	unsigned int            ErrorCode;
	unsigned int            Filesys = 0;
	unsigned __int64		size;
	unsigned __int64		freesize = 0;
	char                    DiskGUIDC[128];
	wchar_t                 DiskGUIDW[128];
	// clear the previous list
	//try
	//{
	IWbemServices          *m_pSvc = NULL;
	IWbemLocator           *m_pLoc = NULL;
	IEnumWbemClassObject   *pEnumerator = NULL;

	DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
	if(dw == EXIT_FAILURE)
	{
		printf("Init WMI error!\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return;
	}

	HRESULT hres;
	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_LogicalDisk"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
		);
	if (FAILED(hres))
	{
		printf("pSvc->ExecQuery error\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return ;               // Program has failed.
	}
	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	while(pEnumerator)
	{
		// 推出下一个对象
		pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
		//没有东西了就跳出去吧
		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		wstring *VolumeLabel	= new wstring(vtProp.bstrVal);	//C:, D:, E:, etc.

		OSNGetDiskGUIDByVolume(*VolumeLabel->c_str(),DiskGUIDC);
		CharToWchar(DiskGUIDC,DiskGUIDW,_countof(DiskGUIDW));
		wstring *diskguid = new wstring(DiskGUIDW);

		pclsObj->Get(L"FileSystem", 0, &vtProp, 0, 0);
		wstring *VolumeLabe2	= new wstring(vtProp.bstrVal);
		if(wcscmp(VolumeLabe2->c_str(),L"NTFS") == 0)
		{
			Filesys = NTFS;
		}
		else if(wcscmp(VolumeLabe2->c_str(),L"FAT32") == 0)
		{
			Filesys = FAT32;
		}
		else if(wcscmp(VolumeLabe2->c_str(),L"EXT3") == 0)
		{
			Filesys = EXT3;
		}
		else if(wcscmp(VolumeLabe2->c_str(),L"EXT4") == 0)
		{
			Filesys = EXT4;
		}
		else
		{
			Filesys = FUnknown;
		}

		pclsObj->Get(L"Size", 0, &vtProp, 0, 0);
		size = _wtoi64(vtProp.bstrVal);
		size = size / 512;
		VariantClear(&vtProp);

		pclsObj->Get(L"FreeSpace", 0, &vtProp, 0, 0);
		freesize = _wtoi64(vtProp.bstrVal);
		freesize = freesize / 512;
		VariantClear(&vtProp);

		unsigned int	signature = 0;	
		unsigned int	startingOffset = 0;

		wchar_t Name = *(VolumeLabel->c_str());

		ErrorCode = OSNGetVolumeID(Name, signature, startingOffset);
		if(ErrorCode==0)
		{
			size=0;
			char char_guid[128];
			bool ret=OsnCHeckIsGPTVolume((const char)*(VolumeLabel->c_str()),char_guid);
			wstring *guid = NULL;
			if(ret)
			{
				wchar_t   lpszFileW[256];
				CharToWchar(char_guid,lpszFileW,_countof(lpszFileW));
				guid=new wstring(lpszFileW);

				GUID guid0;

				char   lpszFileC[256];
				WcharToChar((wchar_t *)(guid->c_str()),lpszFileC,sizeof(lpszFileC));
				char* chguid = lpszFileC;

				OsnGUIDFromString(chguid,&guid0);
				int ret=OsnVolumeCopyGetVolumeSize(guid0,size);
				/*if(0!=ret)
				{
				myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume的size出现错误,errorcode=",ret.ToString()));
				}
				else
				{
				myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume:",VolumeLabel,"的size=",Convert::ToString(size)));
				}*/
			}
			else
			{
				VOLUMEID volume;
				volume.MBP_VolumeID.m_BlockOffset=startingOffset;
				volume.MBP_VolumeID.m_DiskSignature=signature;
				volume.MBP_VolumeID.m_NotUsed1=0;
				OsnGUIDToString(char_guid,volume.SAN_VolumeID.m_VolumeGuid);

				wchar_t   lpszFile[256];
				CharToWchar(char_guid,lpszFile,_countof(lpszFile));
				guid=new wstring(lpszFile);		

				int ret=OsnVolumeCopyGetVolumeSize(volume.SAN_VolumeID.m_VolumeGuid,size);
				/*if(0!=ret)
				{
				myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume的size出现错误,errorcode=",ret.ToString()));
				}
				else
				{
				myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume:",VolumeLabel,"的size=",Convert::ToString(size)));
				}*/
			}

			CVolumeInfo	*pVolumeInfo = new CVolumeInfo(Free,size,size-freesize,guid,VolumeLabel,diskguid,(FileSys)Filesys,OnLine);

			if(this->pVolumeList->GetVolumeInfo(guid) == NULL)
				this->pVolumeList->AddItem((DWORD)pVolumeInfo);
		}
		
		pclsObj->Release();
		delete(VolumeLabe2);
	}
	OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
	//}
	/*catch(Exception ^e)
	{
	MessageBox::Show("获取分区出异常.");
	MessageBox::Show(e->Message);
	}*/
}


void COsnMirrorCopyXML::GetSystemDisksInfo()
{
	this->pDiskList ->Clear();

	//try
	//{
	IWbemServices          *m_pSvc = NULL;
	IWbemLocator           *m_pLoc = NULL;
	IEnumWbemClassObject   *pEnumerator = NULL;

	DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
	if(dw == EXIT_FAILURE)
	{
		printf("Init WMI error!\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return;
	}

	DISK_INFO     pDisk;
	DISK_INFOEX   pDiskEx;
	int ErrorCode=0;

	HRESULT hres;
	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_DiskDrive"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
		);
	if (FAILED(hres))
	{
		printf("pSvc->ExecQuery error\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return ;               // Program has failed.
	}
	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	for(int i=0;i<64;i++)
	{
		pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
		//没有东西了就跳出去吧
		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		if(S_OK == pclsObj->Get(L"Model", 0, &vtProp, 0, 0))
		{
			wstring *DiskOEM	= new wstring(vtProp.bstrVal);	//C:, D:, E:, etc.
			VariantClear(&vtProp);

			memset(&pDisk,0,sizeof(DISK_INFO));
			memset(&pDiskEx,0,sizeof(DISK_INFOEX));
			pDisk.m_DiskIndex=i;

			ErrorCode=OsnGetDisk(&pDisk);
			if(ErrorCode==0)
			{
				wstring *chguid = NULL;
				char guid[128];
				OsnGUIDToString(guid,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);

				wchar_t   lpszFileW[256];
				CharToWchar(guid,lpszFileW,_countof(lpszFileW));
				chguid = new wstring(lpszFileW);

				unsigned __int64 UseSize = this->pVolumeList->GetBlocksBySignature(chguid);

				CDiskInfo *pNewDisk = new CDiskInfo(Free,pDisk.m_DiskSize,UseSize,pDisk.m_DiskIndex,chguid,DiskOEM,MBR,Basic,Inited);
				if(pNewDisk != NULL)
				{
					if(pDiskList->GetDiskInfo(pNewDisk->m_Guid)==NULL 
						||pDiskList->GetDiskByIndex(pNewDisk->m_DiskIndex) == NULL)
						pDiskList->AddItem((DWORD)pNewDisk);
				}
			}
		}

		pclsObj->Release();
	}
	OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
	//}
	//catch(Exception^ exx)
	//{
	//myEventLog->OSNWriteEventLog(String::Concat("GetSystemDisksInfo,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
	//}
}

void COsnMirrorCopyXML::MoveNext(char *pSou,char *pDes,char sign)
{
	int j=0;
	for(;*(pSou+j)!=sign && *(pSou+j)!='\0';j++)
	{
		*(pDes + j) = *(pSou + j);
	}
	*(pDes + j) = '\0';
}

void COsnMirrorCopyXML::ReadConfigurationFile()
{
	char pImagePath[256];
	char TempC[256];
	wchar_t  TempW[128];
	char *pSou,pDes[128];

	//read initiator information info
	WcharToChar(ImagePath->c_str(),pImagePath,sizeof(pImagePath));
	strcat_s(pImagePath, "\\OSNHostMirror.cfg");

	fstream fiHost(pImagePath,ios::in);
	if(!fiHost.is_open())
	{
		printf("ReadConfigurationFile OSNHostMirror.cfg error!");
		return ;
	}

	//String ^strMsg = "Illegal line found in Configuration file.";

	while(fiHost.getline(TempC,sizeof(TempC)))
	{
		if(*TempC == 'D')
		{
			pSou = TempC + strlen("DiskMirrorInfo") + 1;

			int i=0;
			unsigned int uiEIMMode = 2;
			unsigned int isCluster = 0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *clusterResourceName = new wstring(L"NONE");

			while(i < 5)
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if(i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					clusterResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			CMirrorInfo *pMirrorInfo = new CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
			this->pDiskMirrorList->AddItem((DWORD)pMirrorInfo);
		}

		if(*TempC == 'V')
		{
			pSou = TempC + strlen("VolumeMirrorInfo") + 1;

			int i=0;
			unsigned int uiEIMMode = 2;
			unsigned int isCluster = 0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *clusterResourceName = new wstring(L"NONE");

			while (i < 5)
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if(i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					clusterResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			CMirrorInfo *pMirrorInfo = new CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
			this->pVolumeMirrorList->AddItem((DWORD)pMirrorInfo);
		}
	}
	fiHost.close();
}

void COsnMirrorCopyXML::GetSystemMirrorInfo()
{

	if(this->m_InstallType == VolumeCopy)
		GetVolumeCopyMirrorInfo();
	else if(this->m_InstallType == DiskCopy)
		GetDiskCopyMirrorInfo();
	else
	{
		GetVolumeCopyMirrorInfo();
		GetDiskCopyMirrorInfo();
	}
}

void COsnMirrorCopyXML::GetDiskCopyMirrorInfo()
{
	/*try
	{*/
		MIRROR_INFO MirrorInfo ;
		for( int i=0;i<this->pDiskList->GetLength();i++)
		{
			CDiskInfo *pDiskInfo = (CDiskInfo *)(this->pDiskList->GetItem(i));
			if(pDiskInfo->m_Role!=Free)
				continue;

			GUID Srcguid0;
			char SrcguidC[64];
			WcharToChar(pDiskInfo->m_Guid->c_str(),SrcguidC,sizeof(SrcguidC));
			OsnGUIDFromString(SrcguidC,&Srcguid0);

			GUID Tgtguid0;
			char TgtguidC[64] = {"00000000-0000-0000-0000-000000000000"};
			OsnGUIDFromString(TgtguidC,&Tgtguid0);

			MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
			MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;


			int ErrorCode = OsnDiskCopyGetMirrorInfo(&MirrorInfo);
			if(ErrorCode==0)
			{   
				char    guidC[128];
				wchar_t guidW[128];
				OsnGUIDToString(guidC,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
				CharToWchar(guidC,guidW,_countof(guidW));

				wstring *strguid = new wstring(guidW);
				CDiskInfo *pTargetDisk= this->pDiskList->GetDiskInfo(strguid);
				delete(strguid);

				if(pTargetDisk!=nullptr)
				{
					pTargetDisk->m_Role =Mirror_Target;
				}
				pDiskInfo->m_Role=Mirror_Source;

				CMirrorInfo *pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(pDiskInfo->m_Guid,true);
				if(pMirrorInfo == NULL)
				{
					char    SrcguidC[128];
					wchar_t SrcguidW[128];
					OsnGUIDToString(SrcguidC,MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid);
					CharToWchar(SrcguidC,SrcguidW,_countof(SrcguidW));
					wstring *strSrcguid = new wstring(SrcguidW);

					char    DesguidC[128];
					wchar_t DesguidW[128];
					OsnGUIDToString(DesguidC,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
					wstring *strDesguid = new wstring(DesguidW);

					unsigned int eimmode = GetEimModebyGUID(strSrcguid,strDesguid);
					unsigned int isCluster = GetIsClusterByGUID(strSrcguid,strDesguid);
					wstring *ResourceName = GetClusterResourceName(strSrcguid,strDesguid);
					pMirrorInfo =  new CMirrorInfo(strSrcguid,strDesguid,eimmode,isCluster,ResourceName);

					this->pDiskMirrorList->AddItem((DWORD)pMirrorInfo);
				}
			}
		}
		WriteConfigurationFile();
	/*}
	catch(Exception^ exx)
	{
		myEventLog->OSNWriteEventLog(String::Concat("GetDiskCopyMirrorInfo,error：",exx->Message->ToString()),EventLogEntryType::Error,024);
	}*/
}

wstring* COsnMirrorCopyXML::GetClusterResourceName(wstring *srcguid,wstring *dstguid)
{
	//read initiator information info
	char pImagePath[256];
	char TempC[256];
	wchar_t  TempW[128];
	char *pSou,pDes[128];
	wstring  *RetName = new wstring(L"NONE");

	WcharToChar(ImagePath->c_str(),pImagePath,sizeof(pImagePath));
	strcat_s(pImagePath, "\\OSNHostMirror.cfg");
	fstream fiHost(pImagePath,ios::in);
	if(!fiHost.is_open())
	{
		printf("GetClusterResourceName OSNHostMirror.cfg error!");
		return RetName;
	}

	while(fiHost.getline(TempC,sizeof(TempC)))
	{
		if(*TempC == 'D')
		{
			//DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
			pSou = TempC + strlen("DiskMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode=2;
			unsigned int isCluster = 0;
			wstring  *ResourceName = new wstring(L"NONE");

			while ( i<5 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if( i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					ResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 && DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return ResourceName;
			}
			delete(SrcGuid);
			delete(DesGuid);

		}

		if(*TempC == 'V')
		{
			//VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
			pSou = TempC + strlen("VolumeMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode=2;
			unsigned int isCluster = 0;
			wstring  *ResourceName = new wstring(L"NONE");

			while ( i<5 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW); //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if(i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					ResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 && DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return ResourceName;
			}
			delete(SrcGuid);
			delete(DesGuid);
		}
	}
	fiHost.close();
	return RetName;
}

DWORD COsnMirrorCopyXML::GetIsClusterByGUID(wstring *srcguid,wstring *dstguid)
{
	char pImagePath[256];
	char TempC[256];
	wchar_t TempW[256];
	char *pSou,pDes[128];

	//read initiator information info
	WcharToChar(ImagePath->c_str(),pImagePath,sizeof(pImagePath));
	strcat_s(pImagePath,"\\OSNHostMirror.cfg");

	fstream fiHost(pImagePath,ios::in);
	if(!fiHost.is_open())
	{
		printf("GetIsClusterByGUID OSNHostMirror.cfg error!");
		return 2;
	}

	//String ^strMsg = "Illegal line found in Configuration file.";
	while(fiHost.getline(TempC,sizeof(TempC)))
	{
		if(*TempC == 'D')
		{
			//DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
			pSou = TempC + strlen("DiskMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode=2;
			unsigned int isCluster = 0;
			wstring *ResourceName = new wstring(L"NONE");

			while ( i<5 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if(i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					ResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 && DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return isCluster;
			}
			delete(SrcGuid);
			delete(DesGuid);

		}

		if(*TempC == 'V')
		{
			//VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
			pSou = TempC + strlen("VolumeMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode=2;
			unsigned int isCluster = 0;
			wstring *ResourceName = new wstring(L"NONE");

			while ( i<5 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW); //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}
				else if(i == 3)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(isCluster,pDes,10);
				}
				else if(i == 4)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					ResourceName->assign(TempW);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 && DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return isCluster;
			}
			delete(SrcGuid);
			delete(DesGuid);

		}


	}
	fiHost.close();
	return 0;
}

DWORD COsnMirrorCopyXML::GetEimModebyGUID(wstring *srcguid,wstring *dstguid)
{
	char pImagePath[256];
	char TempC[256];
	wchar_t TempW[256];
	char *pSou,pDes[128];

	//read initiator information info
	WcharToChar(ImagePath->c_str(),pImagePath,sizeof(pImagePath));
	strcat_s(pImagePath,"\\OSNHostMirror.cfg");

	fstream fiHost(pImagePath,ios::in);
	if(!fiHost.is_open())
	{
		printf("GetEimModebyGUID OSNHostMirror.cfg error!");
		return 2;
	}

	while(fiHost.getline(TempC,sizeof(TempC)))
	{
		if(*TempC == 'D')
		{
			//DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
			pSou = TempC + strlen("DiskMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode = 2;

			while ( i<3 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 && DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return uiEIMMode;
			}
			delete(SrcGuid);
			delete(DesGuid);

		}

		if(*TempC == 'V')
		{
			pSou = TempC + strlen("VolumeMirrorInfo") + 1;

			int i=0;
			wstring  *SrcGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			wstring  *DesGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
			unsigned int uiEIMMode = 2;

			while ( i<3 )
			{
				if(i == 0)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					SrcGuid->assign(TempW);
				}
				else if(i == 1)
				{
					MoveNext(pSou,pDes,';');
					CharToWchar(pDes,TempW,_countof(TempW));
					DesGuid->assign(TempW);
				}
				else if(i == 2)
				{
					MoveNext(pSou,pDes,';');
					_itoa_s(uiEIMMode,pDes,10);
				}

				pSou = pSou + strlen(pDes) + 1;
				++i;
			}

			if(SrcGuid->compare(srcguid->c_str())==0 &&DesGuid->compare(dstguid->c_str())==0)
			{
				delete(SrcGuid);
				delete(DesGuid);
				fiHost.close();
				return uiEIMMode;
			}
			delete(SrcGuid);
			delete(DesGuid);
		}
	}
	fiHost.close();
	return 2;
}

void COsnMirrorCopyXML::GetVolumeCopyMirrorInfo()
{
	/*try
	{*/
		MIRROR_INFO MirrorInfo ;
		for( int i=0;i<this->pVolumeList->GetLength();i++)
		{
			CVolumeInfo *pVolumeInfo = (CVolumeInfo *)(this->pVolumeList->GetItem(i));
			if(pVolumeInfo->m_Role!=Free)
				continue;

			/*try
			{*/
				GUID Srcguid0;
				char SrcguidC[64];
				WcharToChar(pVolumeInfo->m_GUID->c_str(),SrcguidC,sizeof(SrcguidC));
				OsnGUIDFromString(SrcguidC,&Srcguid0);
				
				GUID Tgtguid0;
				char TgtguidC[64] = {"00000000-0000-0000-0000-000000000000"};
				OsnGUIDFromString(TgtguidC,&Tgtguid0);

				MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
				MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
			/*}
			catch(Exception^ ee)
			{
				myEventLog->OSNWriteEventLog(String::Concat("Osnguid from string,error：",ee->Message->ToString()),EventLogEntryType::Error,024);
			}*/

			/*try
			{*/
				int ErrorCode = OsnVolumeCopyGetMirrorInfo(&MirrorInfo);
				if(ErrorCode==0)
				{   
					char    guidC[128];
					wchar_t guidW[128];
					OsnGUIDToString(guidC,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
					CharToWchar(guidC,guidW,_countof(guidC));

					wstring *strguid=new wstring(guidW);
					CVolumeInfo *pTargetVolume = this->pVolumeList->GetVolumeInfo(strguid);
					delete(strguid);

					if(pTargetVolume!= NULL)
					{
						pTargetVolume->m_Role = Mirror_Target;
					}
					pVolumeInfo->m_Role = Mirror_Source;

					CMirrorInfo *pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pVolumeInfo->m_GUID,true);
					if(pMirrorInfo == NULL)
					{
						char    SrcguidC[128];
						wchar_t SrcguidW[128];
						OsnGUIDToString(SrcguidC,MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid);
						CharToWchar(SrcguidC,SrcguidW,_countof(SrcguidW));
						wstring *Srcguid = new wstring(SrcguidW);

						char    DesguidC[128];
						wchar_t DesguidW[128];
						OsnGUIDToString(DesguidC,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
						CharToWchar(DesguidC,DesguidW,_countof(SrcguidW));
						wstring *Desguid = new wstring(DesguidW);

						unsigned int eimmode=GetEimModebyGUID(Srcguid,Desguid);
						unsigned int isCluster=GetIsClusterByGUID(Srcguid,Desguid);
						wstring *ResourceName = GetClusterResourceName(Srcguid,Desguid);
						pMirrorInfo = new CMirrorInfo(Srcguid,Desguid,eimmode,isCluster,ResourceName);

						this->pVolumeMirrorList->AddItem((DWORD)pMirrorInfo);
					}
				}
			/*}
			catch(Exception^ exxx)
			{
				myEventLog->OSNWriteEventLog(String::Concat("OsnVolumeCopyGetMirrorInfo,error：",exxx->Message->ToString()),EventLogEntryType::Error,024);
			}*/
		}
		WriteConfigurationFile();
	/*}
	catch(Exception^ exx)
	{
		myEventLog->OSNWriteEventLog(String::Concat("GetVolumeCopyMirrorInfo,error：",exx->Message->ToString()),EventLogEntryType::Error,024);

	}*/
}

void COsnMirrorCopyXML::QueryFCChannel()
{
	CWmi  *pWmi = new CWmi(L"ROOT\\WMI");
	if(!pWmi)
		return ;

	if(pWmi->Init(L"MSFC_FibrePortHBAAttributes",NULL))
	{
		for(int idx = 0; idx < pWmi->GetObjCount();++idx)
		{
			int      charSize = 0;
			UCHAR  * portWWN = NULL;
			IWbemClassObject *pObject;
			_variant_t vtProp;
			if(pWmi->GetObjectProp(idx,"Attributes",&vtProp,VT_UNKNOWN))
			{
				VARIANT vtProp2;
				pObject = (IWbemClassObject *)vtProp.punkVal;
				pObject->Get(L"PortWWN",0,&vtProp2,0,0);
				SAFEARRAY *psa = V_ARRAY(&vtProp2);
				portWWN = pWmi->SafeArrayToUInt8(psa,&charSize);
				for(int j = 0 ;j<charSize;++j)
					printf("%x",portWWN[j]);

				m_pTempXML->AddXMLChannelElement("ChannelList","Channel");
				m_pTempXML->AddXMLAttribute("Channel","Type","FC");
				m_pTempXML->AddXMLAttribute("Channel","InitiatorWWN",(char *)portWWN);
				
				VariantClear(&vtProp2);
			}

			VariantClear(&vtProp);
		}
	}

	delete pWmi;
}

DWORD COsnMirrorCopyXML::ConnectiSCSIChannel(char *pIPAddress,char *pIqn)
{
	//Connect to server
	string s("iscsicli AddTargetPortal ");
	s = s + pIPAddress + " " + "3260";
	DWORD dw = system(s.c_str());

	// Login to server
	if(dw == 0)
	{
			s = "iscsicli LoginTarget ";
			s = s + pIqn + " " + "T * * * * * * * * * * * * * * * 0";
			dw = system(s.c_str());
	}

	return dw;
}

DWORD COsnMirrorCopyXML::GetSessionIDByIqn(ULONGLONG *pSessionID,ULONGLONG *pAdapterID,char *pIqn)
{
	IWbemServices          *m_pSvc = NULL;
	IWbemLocator           *m_pLoc = NULL;
	IEnumWbemClassObject   *pEnumerator = NULL;
	IWbemClassObject       **m_pclsObj = NULL;
	IWbemClassObject       *pclsObj = NULL;
	VARIANT vtProp;
	VARIANT vtProp0;
	VARIANT vtProp1;
	HRESULT hres;

	DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\WMI");
	if(dw == EXIT_FAILURE)
	{
		printf("Init WMI error!\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	}

	wchar_t  pWQL[128] = {L"SELECT * FROM MSiSCSI_InitiatorSessionInfo"};

	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		pWQL,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
		);
	if (FAILED(hres))
	{
		printf("pSvc->ExecQuery error\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return 3;               // Program has failed.
	}

	ULONG uReturn = 0;

	while(pEnumerator)
	{
		// 推出下一个对象
		pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
		if(0 == uReturn)
		{
			break;
		}

		if(pclsObj->Get(L"UniqueAdapterId", 0, &vtProp, 0, 0) == S_OK)
		{
			*pAdapterID = vtProp.ullVal;
		}
		if(pclsObj->Get(L"SessionCount", 0, &vtProp, 0, 0) == S_OK)
		{ 
			VARTYPE vt;

			hres = pclsObj->Get(L"SessionsList", 0, &vtProp1, 0, 0);

			SAFEARRAY *psa = V_ARRAY(&vtProp1);
			SafeArrayGetVartype(psa,&vt);
			HRESULT hr=SafeArrayAccessData(psa,(void HUGEP**)&m_pclsObj);

			if(vt == VT_UNKNOWN)
			{
				for(int count=0;count<vtProp.lVal;count++)
				{
					if(m_pclsObj[count]->Get(L"TargetiSCSIName", 0, &vtProp0, 0, 0) == S_OK)
					{
						if(strcmp((_bstr_t)vtProp0.bstrVal,pIqn) == 0)
						{
							if(m_pclsObj[count]->Get(L"TargetiSCSIName", 0, &vtProp0, 0, 0) == S_OK)
							{
								*pSessionID = vtProp0.ullVal;
							}
						}
					}
				}
			}
			hr  = SafeArrayUnaccessData(psa);
		}
	}

	VariantClear(&vtProp);
	VariantClear(&vtProp0);
	VariantClear(&vtProp1);

	if(pclsObj != NULL)
	{
		pclsObj->Release();
	}
	OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
	return 0;
}

DWORD COsnMirrorCopyXML::DisConnectiSCSIChannel(char *pIPAddress,char *pIqn)
{
	DWORD dw = 0;
	ULONGLONG SessionID;
	ULONGLONG AdapterID;
	char pSessionID[64];
	char pAdapterID[64];

	dw = GetSessionIDByIqn(&SessionID,&AdapterID,pIqn);
	if(dw == EXIT_SUCCESS)
	{
		 _ui64toa_s(SessionID,pSessionID,sizeof(pSessionID),16);
		 _ui64toa_s(AdapterID,pAdapterID,sizeof(pAdapterID),16);

		 string s = "iscsicli LogoutTarget ";
		 s = s + pSessionID + pAdapterID + "-" + pSessionID; 

		 dw = system(s.c_str());
		 if(dw == EXIT_SUCCESS)
		 {
			 s = "iscsicli RemoveTargetPortal ";
			 s = s + pIPAddress + " 3260";
			 dw = system(s.c_str());
		 }
	}

	return dw;
}

DWORD COsnMirrorCopyXML::QueryiSCSIChannel()
{
	IWbemServices          *m_pSvc = NULL;
	IWbemLocator           *m_pLoc = NULL;
	IEnumWbemClassObject   *pEnumerator = NULL;
	IWbemClassObject       **m_pclsObj = NULL;
	IWbemClassObject       *pclsObj = NULL;
	VARIANT vtProp;
	VARIANT vtProp0;
	VARIANT vtProp1;
	HRESULT hres;

	DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\WMI");
	if(dw == EXIT_FAILURE)
	{
		printf("Init WMI error!\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	}

	wchar_t  pWQL[128] = {L"SELECT * FROM MSiSCSI_InitiatorSessionInfo"};

	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		pWQL,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
		);
	if (FAILED(hres))
	{
		printf("pSvc->ExecQuery error\n");
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return 3;               // Program has failed.
	}

	ULONG uReturn = 0;

	while(pEnumerator)
	{
		// 推出下一个对象
		pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
		if(0 == uReturn)
		{
			break;
		}
		if(pclsObj->Get(L"SessionCount", 0, &vtProp, 0, 0) == S_OK)
		{ 
			VARTYPE vt;

			hres = pclsObj->Get(L"SessionsList", 0, &vtProp1, 0, 0);

			SAFEARRAY *psa = V_ARRAY(&vtProp1);
			SafeArrayGetVartype(psa,&vt);
			HRESULT hr=SafeArrayAccessData(psa,(void HUGEP**)&m_pclsObj);

			if(vt == VT_UNKNOWN)
			{
				for(int count=0;count<vtProp.lVal;count++)
				{
					m_pTempXML->AddXMLChannelElement("ChannelList","Channel");
					m_pTempXML->AddXMLAttribute("Channel","Type","ISCSI");

					if(m_pclsObj[count]->Get(L"InitiatoriSCSIName", 0, &vtProp0, 0, 0) == S_OK)
					{
						m_pTempXML->AddXMLAttribute("Channel","InitiatorIqn",(_bstr_t)vtProp0.bstrVal);
					}
					if(m_pclsObj[count]->Get(L"TargetiSCSIName", 0, &vtProp0, 0, 0) == S_OK)
					{
						m_pTempXML->AddXMLAttribute("Channel","TargetIqn",(_bstr_t)vtProp0.bstrVal);
					}
				}
			}
			hr  = SafeArrayUnaccessData(psa);
		}
	}

	VariantClear(&vtProp);
	VariantClear(&vtProp0);
	VariantClear(&vtProp1);

	if(pclsObj != NULL)
	{
		pclsObj->Release();
	}
	OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
	return 0;
}

void COsnMirrorCopyXML::RefreshChannelListXML()
{
	m_pTempXML->AddXMLElement("Client","ChannelList");

	QueryFCChannel();
	QueryiSCSIChannel();
}

void COsnMirrorCopyXML::RefreshVolumeListXML()
{
	int index=0;
	for( int i=0;i<this->pVolumeList->GetLength();i++)
	{
		char temp[128];
		unsigned __int64 sizeTemp = 0;
		CVolumeInfo *pVolumeInfo =(CVolumeInfo *)(this->pVolumeList->GetItem(i));

		// 判断如果卷所在磁盘是镜像副本，那就过滤
		if(pDiskList->HideVolume(pVolumeInfo->m_GUID))
		{
			continue;
		}

		char DiskGuidC[64];
		WcharToChar(pVolumeInfo->m_DiskGUID->c_str(),DiskGuidC,sizeof(DiskGuidC));
		m_pTempXML->AddXMLVolElement("DiskList","Partition",DiskGuidC);

		WcharToChar((wchar_t*)pVolumeInfo->m_Protected.c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("Partition","IsProtected",temp);

		sizeTemp = pVolumeInfo->m_VolumeSize /2/1024;
		_i64toa_s(sizeTemp,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("Partition","Size",temp);

		sizeTemp = pVolumeInfo->m_UsedSpace /2/1024;
		_i64toa_s(sizeTemp,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("Partition","Used",temp);

		WcharToChar((wchar_t*)pVolumeInfo->m_GUID->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("Partition","Guid",temp);

		WcharToChar((wchar_t*)pVolumeInfo->m_VolumeLable->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("Partition","name",temp);

		_itoa_s(pVolumeInfo->m_FileSys,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("Partition","FS",temp);

		/*WcharToChar((wchar_t*)pVolumeInfo->m_DiskGUID->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("Partition","InPhyDiskGuid",temp);*/

		m_pTempXML->AddXMLAttribute("Partition","State","1");
		

		index++;
	}
}

void COsnMirrorCopyXML::RefreshDiskListXML()
{
	for( int i=0;i<this->pDiskList->GetLength();i++)
	{
		char temp[128];
		unsigned __int64 sizeTemp = 0;
		CDiskInfo *pDiskInfo = (CDiskInfo *)(this->pDiskList->GetItem(i));

		m_pTempXML->AddXMLElement("Client","DiskList");

		WcharToChar((wchar_t*)pDiskInfo->m_Protected.c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("DiskList","IsProtected",temp);

		sizeTemp = pDiskInfo->m_DiskSize /2/1024;
		_i64toa_s(sizeTemp,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("DiskList","Size",temp);

		sizeTemp = pDiskInfo->m_UsedSpace /2/1024;
		_i64toa_s(sizeTemp,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("DiskList","Used",temp);

		_itoa_s(pDiskInfo->m_DiskIndex,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("DiskList","Name",temp);

		WcharToChar((wchar_t*)pDiskInfo->m_Guid->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("DiskList","Guid",temp);

		/*WcharToChar((wchar_t*)pDiskInfo->m_DiskOEM->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("DiskList","Oem",temp);*/

		m_pTempXML->AddXMLAttribute("DiskList","FS","0");
		m_pTempXML->AddXMLAttribute("DiskList","State","1");

	}
}

void COsnMirrorCopyXML::RefreshClientXML()
{
	char hostname[32],*ipAddress = NULL,SysVersion[32]; 
	DWORD dw = pOSNService->m_OSNRpcServer.OSNRpcGetBasicInfo(hostname,&ipAddress,SysVersion);

	m_pTempXML->AddXMLElement("Client");

	m_pTempXML->AddXMLAttribute("Client","ID",m_ClientID);
	m_pTempXML->AddXMLAttribute("Client","IPs",ipAddress);
	m_pTempXML->AddXMLAttribute("Client","HostName",hostname);
	m_pTempXML->AddXMLAttribute("Client","SystemType",SYS_TYPE_WINDOWS);
	m_pTempXML->AddXMLAttribute("Client","SystemVersion",SysVersion);

	if(ipAddress != NULL)
	{
		delete(ipAddress);
	}
}

DWORD COsnMirrorCopyXML::NewMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType)
{
	DWORD ErrorCode =0;
	if(MirrorType == 0) // volume node
	{
		if(this->m_InstallType == DiskCopy)
		{
			printf("当前安装版本不支持卷复制");
			return EXIT_FAILURE;
		}
		if(this->pVolumeMirrorList->GetLength()>128)
		{
			printf("You have exceeded the maximum number of mirrors ");
			return EXIT_FAILURE;
		}

		//ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

		//wstring *guid  = SelectItem->SubItems[1]->Text;
		CVolumeInfo *pSelectVolume = this->pVolumeList->GetVolumeInfoByString(pSrcGuid);
		if(pSelectVolume == NULL)
		{
			printf("无法获取驱动器信息");
			return EXIT_FAILURE;
		}

		if(CheckVolIsBootable(pSelectVolume->m_VolumeLable))
		{
			printf("不支持对系统卷做镜像,请选择非系统卷.");
			return EXIT_FAILURE;
		}
		if(pSelectVolume->m_Role != Free)
		{
			printf("该卷已经存在镜像关系中，不能再创建镜像关系.");
			return EXIT_FAILURE;
		}
		
		ErrorCode = NewVolumeMirror(pSrcGuid,pDesGuid);
	}
	else if(MirrorType == 1) // disk node
	{
		if(this->m_InstallType == VolumeCopy)
		{
			printf("当前安装版本不支持磁盘复制");
			return EXIT_FAILURE;
		}

		if(this->pDiskMirrorList->GetLength()>128)
		{
			printf("You have exceeded the maximum number of mirrors ");
			return EXIT_FAILURE;
		}

		//ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

		//wstring *guid  = SelectItem->SubItems[0]->Text;

		CDiskInfo *pSelectDisk = this->pDiskList->GetDiskInfo(pSrcGuid);
		if(pSelectDisk == NULL)
		{
			printf("无法获取磁盘信息信息");
			return EXIT_FAILURE;
		}

		if(pSelectDisk->m_Role!=Free)
		{
			printf("该卷已经存在镜像关系中，不能再创建镜像关系.");
			return EXIT_FAILURE;
		}

		ErrorCode = NewDiskMirror(pSrcGuid,pDesGuid);
		// GetSystemDisksInfo();//diskmirror创建后，有可能有初始化后的操作，所以要刷新下disk信息
	}
	else
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

DWORD COsnMirrorCopyXML::RemoveMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType)
{
	CMirrorInfo *pMirrorInfo = NULL;
	CVolumeInfo *pVolumeInfo = NULL;
	CDiskInfo   *pDiskInfo   = NULL;
	//String^ strParaMeter=nullptr;

	//String^ sch        =SelectItem->SubItems[7]->Text;

	if(MirrorType == 0)
	{
		bool delflag = DeleteVolumeMirror(pSrcGuid,pDesGuid,MirrorType);

		pVolumeInfo = this->pVolumeList->GetVolumeInfoByString(pSrcGuid);
		if(!pVolumeInfo)
		{
			printf("无法获取此卷信息。");
			if(delflag)
				printf("镜像CDP计划删除失败，请手动删除！");
			return EXIT_FAILURE;
		}

	//	strParaMeter = String::Concat(strSrcSignOffset,"#");
		//strParaMeter = String::Concat(strParaMeter,"Volume ");
	}
	else if(MirrorType == 1)
	{
		bool delflag = DeleteDiskMirror(pSrcGuid,pDesGuid,MirrorType);

		pDiskInfo = this->pDiskList->GetDiskInfo(pSrcGuid);
		if(!pDiskInfo)
		{
			printf("无法获取此磁盘信息。");
			if(delflag)
				printf("镜像CDP计划删除失败，请手动删除！");
			return EXIT_FAILURE;
		}

		//strParaMeter = String::Concat(strSrcSignOffset,"#");
		//strParaMeter = String::Concat(strParaMeter,"Disk ");
	}
	else
	{
		printf("选择无效.");
	}

	/*if(sch->Equals("Schedule"))
	{
	try
	{
	String^ temppara=String::Concat("deletesilent","@",strParaMeter);
	backgroundCDPSchedule->RunWorkerAsync(temppara);
	}
	catch(Exception^ exx)
	{
	MessageBox::Show("有些镜像的CDP计划删除失败，请手动删除");
	myEventLog->OsnHostMirrorLog(String::Concat("删除CDP计划出现异常,error=",exx->Message->ToString()));
	}
	}*/
	return EXIT_SUCCESS;
}

DWORD COsnMirrorCopyXML::InitMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType)
{
	//String^ isInit= SelectItem->SubItems[5]->Text;

	// m_ListViewMutex->ReleaseMutex();
	/*if(isInit->Contains("初始化"))
	{
		MessageBox::Show("该镜像关系已经在初始化");
		return;
	}*/

	if(MirrorType == 0)
		InitializeVolumeMirror(pSrcGuid,pDesGuid,BASIC_INIT);
	else if(MirrorType == 1)
		InitializeDiskMirror(pSrcGuid,pDesGuid);

	Sleep(700);
	return EXIT_SUCCESS;
}

void COsnMirrorCopyXML::InitializeVolumeMirror(wstring *pSrcGuid,wstring *pDesGuid,INIT_TYPE type)
{
	/*try
	{*/
		CMirrorInfo *pMirrorInfo = NULL;
		CVolumeInfo *pSourceInfo = NULL;
		CVolumeInfo *pTargetInfo = NULL;

		if(pSrcGuid->length() > 8)
		{
			pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSrcGuid,pDesGuid);
			pSourceInfo = this->pVolumeList->GetVolumeInfoByString(pSrcGuid);
			pTargetInfo = this->pVolumeList->GetVolumeInfoByString(pDesGuid);
		}
		else
		{
			pSourceInfo = this->pVolumeList->GetVolumeInfo(pSrcGuid);
			pTargetInfo = this->pVolumeList->GetVolumeInfo(pDesGuid);

			if(pSourceInfo == NULL && pTargetInfo == NULL)
			{
				printf("无法获取驱动器信息");
				return;
			}
			else
			{
				if(pSourceInfo != NULL)
					pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSourceInfo->m_GUID,true);
				else
					pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pTargetInfo->m_GUID,false);

			}

		}
		if(CheckFileSystem(pSourceInfo->m_VolumeLable) != 2)
		{
			printf("源卷不是NTFS文件系统，不支持自动精简初始化.");
			return ;
		}

		if(pSourceInfo->m_VolumeSize > pTargetInfo->m_VolumeSize)
		{
			printf("源卷比目标卷小，请重新设置镜像关系,否则数据不能一致");
			return ;
		}

		if(pMirrorInfo == NULL)
		{
			printf("无法获取镜像信息");
			return;
		}

		MIRROR_INFO MirrorInfo;


		GUID Srcguid0;
		char SrcguidC[64];
		WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),SrcguidC,sizeof(SrcguidC));
		OsnGUIDFromString(SrcguidC,&Srcguid0);

		GUID Tgtguid0;
		char TgtguidC[64];
		WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),TgtguidC,sizeof(TgtguidC));
		OsnGUIDFromString(TgtguidC,&Tgtguid0);

		MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
		MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
		unsigned int ErrorCode = OsnStartVolumeCopyInitialization(&MirrorInfo,type);
		if(ErrorCode !=0)
		{
			printf("初始化失败,代码:%d",ErrorCode);
			return;
		}
	//}
	//catch(Exception^ exx)
	//{
	//	MessageBox::Show("初始化过程中出现异常，请稍后重试");
	//	myEventLog->OsnHostMirrorLog(String::Concat("初始化过程中出现异常，异常信息：",exx->Message->ToString()));
	//	//m_ListViewMutex->ReleaseMutex();
	//}

}

DWORD COsnMirrorCopyXML::CheckFileSystem(wstring *LabelName)
{

	if(LabelName == NULL)
		return 0;

	char VolumeNameBuffer[32];
	DWORD lpVolumeSerialNumber;
	DWORD lpMaximumComponentLength;
	DWORD lpFileSystemFlags;
	char  lpFileSystemNameBuffer[32];
	wstring temp0(LabelName->c_str());
	wstring temp1 = L"\\";
	char  LabelNameC[10];

	temp0 += temp1;
	WcharToChar((wchar_t*)temp0.c_str(),LabelNameC,sizeof(LabelNameC));
	BOOL ReturnValue = GetVolumeInformation(LabelNameC,
		VolumeNameBuffer,
		32,
		&lpVolumeSerialNumber,
		&lpMaximumComponentLength,
		&lpFileSystemFlags,
		lpFileSystemNameBuffer,
		32);
	if(ReturnValue == 0)
		return 0;

	if(strcmp("NTFS",lpFileSystemNameBuffer)!=0)
		return 1;
	else
		return 2;

}

void COsnMirrorCopyXML::InitializeDiskMirror(wstring *pSrcGuid,wstring *pDesGuid)
{
	/*try
	{*/
		CMirrorInfo *pMirrorInfo = NULL;
		CDiskInfo   *pSourceInfo = NULL;
		CDiskInfo   *pTargetInfo = NULL;


		pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(pSrcGuid,true);

		pSourceInfo = this->pDiskList->GetDiskInfo(pSrcGuid);
		pTargetInfo = this->pDiskList->GetDiskInfo(pDesGuid);


		if(pMirrorInfo == NULL)
		{
			printf("无法获取镜像信息");
			return;
		}
		//重新获取disk的容量的大小，然后判断源卷和目标卷的大小是否一样，map卷可以扩容会导致大小不一样
		DISK_INFO pSourceDisk;
		int ErrorCode=0;
		memset(&pSourceDisk,0,sizeof(DISK_INFO));
		pSourceDisk.m_DiskIndex=pSourceInfo->m_DiskIndex;
		ErrorCode=OsnGetDisk(&pSourceDisk);
		if(ErrorCode!=0)
		{
			printf("获取disk的容量失败");
			//myEventLog->OsnHostMirrorLog("获取disk的容量失败");
		}
		else
		{
			DISK_INFO pTargetDisk;
			int ErrorCode=0;
			memset(&pTargetDisk,0,sizeof(DISK_INFO));
			pTargetDisk.m_DiskIndex=pTargetInfo->m_DiskIndex;
			ErrorCode=OsnGetDisk(&pTargetDisk);
			if(0==ErrorCode)
			{
				if(pSourceDisk.m_DiskSize>pTargetDisk.m_DiskSize)
				{
					printf("源盘比目标盘的容量大，请重新设置镜像关系,否则数据不能一致");
					return;
				}
			}
		}

		MIRROR_INFO MirrorInfo;

		GUID Srcguid0;
		char SrcguidC[64];
		WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),SrcguidC,sizeof(SrcguidC));
		OsnGUIDFromString(SrcguidC,&Srcguid0);

		GUID Tgtguid0;
		char TgtguidC[64];
		WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),TgtguidC,sizeof(TgtguidC));
		OsnGUIDFromString(TgtguidC,&Tgtguid0);

		MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
		MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

		unsigned int Error = OsnStartDiskCopyInitialization(&MirrorInfo);
		if(Error !=0)
		{
			printf("初始化失败,代码:%d",Error);
			return;
		}
	//}
	//catch(Exception^ exx)
	//{
	//	MessageBox::Show("初始化过程中出现异常，请稍后重试");
	//	myEventLog->OsnHostMirrorLog(String::Concat("初始化过程中出现异常，异常信息：",exx->Message->ToString()));
	//	// m_ListViewMutex->ReleaseMutex();
	//}

}

bool  COsnMirrorCopyXML::DeleteVolumeMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType)
{
	CMirrorInfo *pMirrorInfo = NULL;
	CVolumeInfo *pSourceInfo = NULL;
	CVolumeInfo *pTargetInfo = NULL;

	if(MirrorType == 0)
	{
		pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcguid,strDesguid);
		pSourceInfo = this->pVolumeList->GetVolumeInfoByString(strSrcguid);
		pTargetInfo = this->pVolumeList->GetVolumeInfoByString(strDesguid);
	}
	else
	{
		pSourceInfo = this->pVolumeList->GetVolumeInfo(strSrcguid);
		pTargetInfo = this->pVolumeList->GetVolumeInfo(strDesguid);

		if(pSourceInfo == NULL && pTargetInfo == NULL)
		{
			return false;
		}
		else
		{
			if(pSourceInfo != NULL)
				pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSourceInfo->m_GUID,true);
			else
				pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pTargetInfo->m_GUID,false);
		}
	}

	if(pMirrorInfo == NULL)
	{
		printf("无法获取镜像信息");
		return false;
	}

	MIRROR_INFO MirrorInfo;

	GUID Srcguid0;
	char SrcguidC[64];
	WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),SrcguidC,sizeof(SrcguidC));
	OsnGUIDFromString(SrcguidC,&Srcguid0);

	GUID Tgtguid0;
	char TgtguidC[64];
	WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),TgtguidC,sizeof(TgtguidC));
	OsnGUIDFromString(TgtguidC,&Tgtguid0);

	MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
	MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

	unsigned int ErrorCode;
	QUERY_MIRROR_STATE MirrorState;
	ErrorCode = OsnGetMirrorState(&MirrorInfo,&MirrorState);	
	if(ErrorCode == 0)
	{
		if(MirrorState.m_State == INIT)
		{
			printf("镜像关系正在初始化，请先停止初始化.");
			return false;
		}
	}


	DWORD PreviousState = ReadPreviousState(pMirrorInfo->m_SourceGuid,true);

	char pSou[64];
	char pDes[64];
	WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),pSou,sizeof(pSou));
	WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),pDes,sizeof(pDes));

	if(pMirrorInfo->m_IsCluster==0)
	{
		ErrorCode =SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",pSou,pDes,false,4,0);
	}
	else
	{
		/*ErrorCode=SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",pMirrorInfo->m_SourceGuid,pMirrorInfo->m_TargetGuid,false,4,1);
		if(ErrorCode == 0)
		{
			SetClusterKey("System\\CurrentControlSet\\Services\\OsnVSS",pMirrorInfo->m_SourceGuid,false,DIRTY,DIRTY);
		}*/
	}
	if(ErrorCode!=0)
	{
		printf("删除注册表失败");
	}

	ErrorCode = OsnRemoveVolumeCopy(&MirrorInfo);
	if(ErrorCode==0)
	{
		if(pSourceInfo!=nullptr)
			pSourceInfo->m_Role = Free;
		if(pTargetInfo!=nullptr)
			pTargetInfo->m_Role = Free;

		this->pVolumeMirrorList->RemoveItem((DWORD)pMirrorInfo);

		WriteConfigurationFile();
	}
	else
	{	
		if(pMirrorInfo->m_IsCluster==0)
		{
			SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",pSou,pDes,true,PreviousState,0);
		}
		else
		{
			//SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",pMirrorInfo->m_SourceGuid,pMirrorInfo->m_TargetGuid,true,PreviousState,1);
		}
		printf("删除镜像出错 ,代码:%d",ErrorCode);
		return false;
	}
	return true;
}

bool  COsnMirrorCopyXML::DeleteDiskMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType)
{
	CMirrorInfo *pMirrorInfo     = NULL;
	CDiskInfo   *pSourceDiskInfo = NULL;
	CDiskInfo   *pTargetDiskInfo = NULL;


	pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcguid,true);

	pSourceDiskInfo = this->pDiskList->GetDiskInfo(strSrcguid);
	pTargetDiskInfo = this->pDiskList->GetDiskInfo(strDesguid);


	if(pMirrorInfo == NULL)
	{
		printf("无法获取镜像信息");
		return false;
	}

	MIRROR_INFO MirrorInfo;

	GUID Srcguid0;
	char SrcguidC[64];
	WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),SrcguidC,sizeof(SrcguidC));
	OsnGUIDFromString(SrcguidC,&Srcguid0);

	GUID Tgtguid0;
	char TgtguidC[64];
	WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),TgtguidC,sizeof(TgtguidC));
	OsnGUIDFromString(TgtguidC,&Tgtguid0);

	MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
	MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

	unsigned int ErrorCode;
	QUERY_MIRROR_STATE MirrorState;
	ErrorCode= OsnGetDiskCopyMirrorState(&MirrorInfo,&MirrorState);
	if(ErrorCode == 0)
	{
		if(MirrorState.m_State == INIT)
		{
			printf("镜像关系正在初始化，请先停止初始化.");
			return false;
		}
	}

	DWORD PreviousState = ReadPreviousState(pMirrorInfo->m_SourceGuid,FALSE);

	char pSou[64];
	char pDes[64];
	WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),pSou,sizeof(pSou));
	WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),pDes,sizeof(pDes));

	if(pMirrorInfo->m_IsCluster==0)
	{
		ErrorCode =SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",pSou,pDes,false,4,0);
	}
	else
	{
		/*ErrorCode=SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",pMirrorInfo->m_SourceGuid,pMirrorInfo->m_TargetGuid,false,4,1);
		if(ErrorCode ==0)
		{
			SetClusterKey("System\\CurrentControlSet\\Services\\OsnDSS",pMirrorInfo->m_SourceGuid,false,DIRTY,DIRTY);
		}*/
	}
	if(ErrorCode!=0)
	{
		printf("删除注册表失败");
	}


	ErrorCode = OsnRemoveDiskCopy(&MirrorInfo);
	if(ErrorCode==0)
	{

		if(pSourceDiskInfo != NULL)
			pSourceDiskInfo->m_Role = Free;
		if(pTargetDiskInfo != NULL)
			pTargetDiskInfo->m_Role = Free;

		this->pDiskMirrorList->RemoveItem((DWORD)pMirrorInfo);

		WriteConfigurationFile();
	}
	else
	{
		if(pMirrorInfo->m_IsCluster==0)
		{
			SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",pSou,pDes,true,PreviousState,0);
		}
		else
		{
			//SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",pSou,pDes,true,PreviousState,1);
		}
		printf("删除镜像出错 ,代码:%d",ErrorCode);
		return false;
	}
	return true;
}

DWORD COsnMirrorCopyXML::ReadPreviousState(wstring *Key,bool Flag)
{
	char temp[64];
	CRegKey *pRegkey = new CRegKey();
	DWORD state,&pstate = state;
	if(Flag) // VolumeCopy
	{
		if(pRegkey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNVSS\\Parameters\\State") == ERROR_SUCCESS)
		{
			WcharToChar(Key->c_str(),temp,sizeof(temp));
			pRegkey->QueryDWORDValue(temp,pstate);
			pRegkey->Close();
			if( state>=0 && state<=6 )
			{
				delete(pRegkey);
				return state;
			}
			else
			{
				delete(pRegkey);
				return 2;
			}
		}
		else
		{
			delete(pRegkey);
			return 2;
		}
	}
	else //DiskCopy
	{
		if(pRegkey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNDSS\\Parameters\\State") == ERROR_SUCCESS)
		{
			WcharToChar(Key->c_str(),temp,sizeof(temp));
			pRegkey->QueryDWORDValue(temp,pstate);
			pRegkey->Close();
			if( state>=0 && state<=6 )
			{
				delete(pRegkey);
				return state;
			}
			else
			{
				delete(pRegkey);
				return 2;
			}
		}
		else
		{
			delete(pRegkey);
			return 2;
		}
	}
}

DWORD COsnMirrorCopyXML::CheckVolIsBootable(wstring *label)
{
	/*try
	{*/
		IWbemServices          *m_pSvc = NULL;
		IWbemLocator           *m_pLoc = NULL;
		IEnumWbemClassObject   *pEnumerator = NULL;

		DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
		if(dw == EXIT_FAILURE)
		{
			printf("Init WMI error!\n");
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return 3;
		}

		HRESULT hres;
		wchar_t  pWQL[128];
		swprintf_s(pWQL,_countof(pWQL),L"Associators   of   {win32_LogicalDisk='%s'}   where   resultClass   =   Win32_DiskPartition",label->c_str());

		hres = m_pSvc->ExecQuery(
			bstr_t("WQL"),
			pWQL,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator
			);
		if (FAILED(hres))
		{
			printf("pSvc->ExecQuery error\n");
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return 3;               // Program has failed.
		}
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		while(pEnumerator)
		{
			// 推出下一个对象
			pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
			if(0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;
			if(S_OK == pclsObj->Get(L"Bootable", 0, &vtProp, 0, 0))
			{
				DWORD dw = vtProp.boolVal;
				pclsObj->Release();
				OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
				VariantClear(&vtProp);
				return dw;
			}
		}
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	/*}
	catch(Exception^ ex)
	{
		myEventLog->OSNWriteEventLog(String::Concat("CheckVolIsBootable: ",ex->ToString()),EventLogEntryType::Error,011);
	}*/
}

DWORD COsnMirrorCopyXML::NewVolumeMirror(wstring *pSrcGuid,wstring *pDesGuid)
{
	//try
	//{
		CVolumeInfo *pSelectDriver = this->pVolumeList->GetVolumeInfoByString(pSrcGuid);
		if(pSelectDriver==NULL)
		{
			printf("无法获取驱动器信息");
			return 1;
		}

		if(pSelectDriver->m_Role!=Free)
		{
			printf("当前驱动器已经有镜像");
			return 1;
		}

		CNewMirror *pNewMirror = new CNewMirror(pSelectDriver,
			this->pVolumeList,
			VolumeCopy,
			this->pDiskList);

		pNewMirror->MirrorOKClick(pSrcGuid,pDesGuid);

		MIRROR_INFO MirrorInfo;

		GUID Srcguid0;
		char SrcguidC[64];
		WcharToChar((wchar_t*)pNewMirror->pSourceVolume->m_GUID->c_str(),SrcguidC,sizeof(SrcguidC));
		OsnGUIDFromString(SrcguidC,&Srcguid0);

		GUID Tgtguid0;
		char TgtguidC[64];
		WcharToChar((wchar_t*)pNewMirror->pTargetVolume->m_GUID->c_str(),TgtguidC,sizeof(TgtguidC));
		OsnGUIDFromString(TgtguidC,&Tgtguid0);

		MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
		MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

		unsigned int ErrorCode;
		if(pNewMirror->m_IsCluster==0)
		{
			ErrorCode  =  OsnSetVolumeCopy(&MirrorInfo);
		}
		else
		{
			ErrorCode = OsnSetClusterVolumeCopy(&MirrorInfo);
		}
		if(ErrorCode != 0)
		{
			printf("设置镜像出错,代码=%d",ErrorCode);
			return 1;
		}

		CMirrorInfo *pMirrorInfo = new CMirrorInfo(pNewMirror->pSourceVolume->m_GUID,
			pNewMirror->pTargetVolume->m_GUID,pNewMirror->m_EIMMode,
			pNewMirror->m_IsCluster,pNewMirror->m_ClusterResourceName);

		pNewMirror->pSourceVolume->m_Role = Mirror_Source;
		pNewMirror->pTargetVolume->m_Role = Mirror_Target;

		char pSou[64];
		char pDes[64];
		WcharToChar((wchar_t*)pNewMirror->pSourceVolume->m_GUID->c_str(),pSou,sizeof(pSou));
		WcharToChar((wchar_t*)pNewMirror->pTargetVolume->m_GUID->c_str(),pDes,sizeof(pDes));

		if(pNewMirror->m_IsCluster==0)
		{
			ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",pSou,pDes,true,2,0);
		}
		else
		{
			/*ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pNewMirror->pSourceDriver->m_GUID,"}"),String::Concat("{",pNewMirror->pTargetDriver->m_GUID,"}"),true,2,1);
			if(ErrorCode==0)
				ErrorCode = SetClusterKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pNewMirror->pSourceDriver->m_GUID,"}"),true,DIRTY,DIRTY);*/
		}
		if(ErrorCode!=0)
		{
			printf("设置注册表失败.");
			OsnRemoveVolumeCopy(&MirrorInfo);
			return 1;
		}

		this->pVolumeMirrorList->AddItem((DWORD)pMirrorInfo);
		WriteConfigurationFile();

		delete(pNewMirror);
		return 0;
	//}
	/*catch(Exception^ exx)
	{
		MessageBox::Show("新建镜像过程中出现异常，请稍后重试");
		myEventLog->OsnHostMirrorLog(String::Concat("新建镜像过程中出现异常，异常信息：",exx->Message->ToString()));
	}
	return 1;*/
}

DWORD COsnMirrorCopyXML::NewDiskMirror(wstring *pSrcGuid,wstring *pDesGuid)
{

	//try
	//{

		//wstring *guid  = SelectItem->SubItems[1]->Text;

		CDiskInfo *pSelectDisk = this->pDiskList->GetDiskInfo(pSrcGuid);
		if(pSelectDisk == NULL)
		{
			printf("无法获取驱动器信息");
			return 1;
		}

		if(pSelectDisk->m_Role!=Free)
		{
			printf("当前驱动器已经有镜像.");
			return 1;
		}

		CNewMirror *pNewMirror = new CNewMirror(pSelectDisk,this->pDiskList,pVolumeList);

		pNewMirror->MirrorOKClick(pSrcGuid,pDesGuid);

		MIRROR_INFO MirrorInfo;

		//try
		//{
			GUID Srcguid0;
			char SrcguidC[64];
			WcharToChar((wchar_t*)pNewMirror->pSourceDisk->m_Guid->c_str(),SrcguidC,sizeof(SrcguidC));
			OsnGUIDFromString(SrcguidC,&Srcguid0);

			GUID Tgtguid0;
			char TgtguidC[64];
			WcharToChar((wchar_t*)pNewMirror->pTargetDisk->m_Guid->c_str(),TgtguidC,sizeof(TgtguidC));
			OsnGUIDFromString(TgtguidC,&Tgtguid0);

			MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
			MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
		/*}
		catch(Exception^ exx)
		{
			MessageBox::Show(String::Concat("new disk mirror error:",exx->Message->ToString()));
			return 1;
		}*/

		wstring  *ddGuid = new wstring(L"00000000-0000-0000-0000-000000000000");
		if(pNewMirror->pSourceDisk->m_Guid == ddGuid ||
			pNewMirror->pTargetDisk->m_Guid == ddGuid)
		{
			printf("源盘或目标盘没有初始化，请初始化！");
			return 1;
		}
		delete(ddGuid);

		unsigned int ErrorCode;
		if(pNewMirror->m_IsCluster==0)
		{
			ErrorCode =  OsnSetDiskCopy(&MirrorInfo);
		}
		else 
		{
			ErrorCode = OsnSetClusterDiskCopy(&MirrorInfo);
		}
		if(ErrorCode!=0)
		{
			printf("设置镜像出错,代码=%d",ErrorCode);
			return 1;
		}

		CMirrorInfo *pMirrorInfo = new CMirrorInfo(pNewMirror->pSourceDisk->m_Guid,
			pNewMirror->pTargetDisk->m_Guid,pNewMirror->m_EIMMode,
			pNewMirror->m_IsCluster,pNewMirror->m_ClusterResourceName);

		pNewMirror->pTargetDisk->m_Role = Mirror_Target;
		pNewMirror->pSourceDisk->m_Role = Mirror_Source;

		char pSou[64];
		char pDes[64];
		WcharToChar((wchar_t*)pNewMirror->pSourceDisk->m_Guid->c_str(),pSou,sizeof(pSou));
		WcharToChar((wchar_t*)pNewMirror->pTargetDisk->m_Guid->c_str(),pDes,sizeof(pDes));

		if(pNewMirror->m_IsCluster==0)
		{
			ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",pSou,pDes,true,2,0);
		}
		else
		{
			/*ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pNewMirror->pSourceDisk->m_Guid,"}"),String::Concat("{",pNewMirror->pTargetDisk->m_Guid,"}"),true,2,1);
			if(ErrorCode==0)
				SetClusterKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pNewMirror->pSourceDisk->m_Guid,"}"),true,DIRTY,DIRTY);*/
		}
		if(ErrorCode!=0)
		{
			printf("设置注册表失败.");
			OsnRemoveDiskCopy(&MirrorInfo);
			return 1;
		}

		this->pDiskMirrorList->AddItem((DWORD)pMirrorInfo);
		WriteConfigurationFile();

		delete(pNewMirror);

		return 0;
	/*}
	catch(Exception^ exx)
	{
		MessageBox::Show("新建镜像过程中出现异常，请稍后重试");
		myEventLog->OsnHostMirrorLog(String::Concat("新建镜像过程中出现异常，异常信息：",exx->Message->ToString()));
	}*/
	return 1;
}

void COsnMirrorCopyXML::WriteConfigurationFile()
{
	//try
	//{
		char pImagePath[256];
		char temp[256];
		fstream fi;
		ofstream fo;

		WcharToChar((wchar_t*)ImagePath->c_str(),pImagePath,sizeof(pImagePath));
		strcat_s(pImagePath,"\\OSNHostMirror.cfg");
		strcat_s(pImagePath,"\\OSNHostMirror.cfg.bak");

		fi.open(pImagePath,ios::in | ios::out);
		fo.open(temp);
		fo << fi.rdbuf();
		fo.close();
		fi.close();
		
		fi.open(pImagePath,ios::in | ios::out | ios::trunc);

		int i;
		for(i=0; i<this->pDiskMirrorList->GetLength(); i++)
		{
			CMirrorInfo *pMirrorInfo = (CMirrorInfo *)(pDiskMirrorList->GetItem(i));
			
			WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),temp,sizeof(temp));
			fi << "DiskMirrorInfo="<<temp<<";";

			WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),temp,sizeof(temp));
			fi << temp<<";";

			_itoa_s(pMirrorInfo->m_EIMMode,temp,10);
			fi << temp<<";";

			_itoa_s(pMirrorInfo->m_IsCluster,temp,10);
			fi << temp<<";";
			
			WcharToChar(pMirrorInfo->m_ClusterResourceName->c_str(),temp,sizeof(temp));
			fi << temp;

			fi << endl;
		}

		for(i=0; i<this->pVolumeMirrorList->GetLength(); i++)
		{
			CMirrorInfo *pMirrorInfo = (CMirrorInfo *)(pVolumeMirrorList->GetItem(i));

			WcharToChar(pMirrorInfo->m_SourceGuid->c_str(),temp,sizeof(temp));
			fi << "VolumeMirrorInfo="<<temp<<";";

			WcharToChar(pMirrorInfo->m_TargetGuid->c_str(),temp,sizeof(temp));
			fi << temp<<";";

			_itoa_s(pMirrorInfo->m_EIMMode,temp,10);
			fi << temp<<";";

			_itoa_s(pMirrorInfo->m_IsCluster,temp,10);
			fi << temp<<";";
			
			WcharToChar(pMirrorInfo->m_ClusterResourceName->c_str(),temp,sizeof(temp));
			fi << temp;

			fi << endl;
		}
		fi.close();
	/*}
	catch(Exception^ exx)
	{
		myEventLog->OSNWriteEventLog(String::Concat("WriteConfigurationFile,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
	}*/

}

DWORD COsnMirrorCopyXML::SetRegistryKey(char *ServiceKeyName,char *SourceString,char *TargetString,bool Flag,int State,int IsCluster)
{
	char temp[128];
	//RegistryKey ^pRegKey = Registry::LocalMachine;
	

	CRegKey *pParmeterKey = new CRegKey();
	strcpy_s(temp,_countof(temp),ServiceKeyName);
	strcat_s(temp,_countof(temp),"\\Parameters");
	if(pParmeterKey->Open(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
	{
		if(pParmeterKey->Create(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
		{
			delete(pParmeterKey);
			return 1;
		}
	}

	CRegKey *pStateKey = new CRegKey();
	strcpy_s(temp,_countof(temp),ServiceKeyName);
	strcat_s(temp,_countof(temp),"\\Parameters\\State");
	if(pStateKey->Open(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
	{
		if(pStateKey->Create(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
		{
			pParmeterKey->Close();
			delete(pParmeterKey);
			delete(pStateKey);
			return 1;
		}
	}

	CRegKey *pMirrorKey = new CRegKey();
	if(IsCluster==0)
	{
		strcpy_s(temp,_countof(temp),ServiceKeyName);
		strcat_s(temp,_countof(temp),"\\Parameters\\Mirrors");
		if(pMirrorKey->Open(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
		{
			if(pMirrorKey->Create(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
			{
				pMirrorKey->Close();
				pParmeterKey->Close();
				delete(pParmeterKey);
				delete(pStateKey);
				delete(pMirrorKey);
				return 1;
			}
		}
	}
	else
	{
		strcpy_s(temp,_countof(temp),ServiceKeyName);
		strcat_s(temp,_countof(temp),"\\Parameters\\ClusterMirrors");
		if(pMirrorKey->Open(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
		{
			if(pMirrorKey->Create(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
			{
				pMirrorKey->Close();
				pParmeterKey->Close();
				delete(pParmeterKey);
				delete(pStateKey);
				delete(pMirrorKey);
				return 1;
			}
		}
	}

	CRegKey *pRebootKey = new CRegKey();
	strcpy_s(temp,_countof(temp),ServiceKeyName);
	strcat_s(temp,_countof(temp),"\\Parameters\\Reboot");
	if(pRebootKey->Open(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
	{
		if(pRebootKey->Create(HKEY_LOCAL_MACHINE,temp) != ERROR_SUCCESS)
		{
			pRebootKey->Close();
			pMirrorKey->Close();
			pParmeterKey->Close();
			delete(pParmeterKey);
			delete(pStateKey);
			delete(pMirrorKey);
			delete(pRebootKey);
			return 1;
		}
	}


	if(Flag)
	{
		//int State = 2;

		pStateKey->SetDWORDValue(SourceString,State);
		pMirrorKey->SetStringValue(SourceString,TargetString);

		int Reboot = 0;
		pRebootKey->SetDWORDValue(SourceString,Reboot);
	}
	else
	{
		pStateKey->DeleteValue(SourceString);
		pMirrorKey->DeleteValue(SourceString);
		pRebootKey->DeleteValue(SourceString);

	}

	pParmeterKey->Close();
	pStateKey->Close();
	pMirrorKey->Close();
	pRebootKey->Close();

	delete(pParmeterKey);
	delete(pStateKey);
	delete(pMirrorKey);
	delete(pRebootKey);

	return 0;
}

bool CWmi::Init(wchar_t *lpszWMIObj,  wchar_t *lpszWhere)
{  
	if (!lpszWMIObj || wcslen(lpszWMIObj)==0) return false;  

	HRESULT hr;  

	// Initialize   

	hr = CoCreateInstance(  
		CLSID_WbemLocator,               
		0,   
		CLSCTX_INPROC_SERVER,   
		IID_IWbemLocator, (LPVOID *) &m_pLoc  
		);  

	if (FAILED(hr)) return false;  


	hr = m_pLoc->ConnectServer(  

		_bstr_t(m_WmiClass), // WMI namespace  
		NULL,                    // User name  
		NULL,                    // User password  
		0,                       // Locale  
		NULL,                    // Security flags                   
		0,                       // Authority         
		0,                       // Context object  
		&m_pSvc                    // IWbemServices proxy  
		);  

	if (FAILED(hr)) return false;  

	hr = CoSetProxyBlanket(  

		m_pSvc,                         // the proxy to set  
		RPC_C_AUTHN_WINNT,            // authentication service  
		RPC_C_AUTHZ_NONE,             // authorization service  
		NULL,                         // Server principal name  
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level  
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level  
		NULL,                         // client identity   
		EOAC_NONE                     // proxy capabilities       
		);  

	if (FAILED(hr)) return false;  

	hr = m_pSvc->GetObject(  
		bstr_t(lpszWMIObj),  
		WBEM_FLAG_RETURN_WBEM_COMPLETE,  
		NULL,  
		&m_pclsObj,  
		NULL);  

	if (FAILED(hr)) return false;  


	//构造WQL语句  
	bstr_t wql="SELECT * FROM ";  
	wql+=lpszWMIObj;  

	if (lpszWhere && wcslen(lpszWhere))  
	{  
		wql+=" WHERE ";  
		wql+=lpszWhere;  
	}  

	//查询对象  
	hr = m_pSvc->ExecQuery(  
		bstr_t("WQL"),   
		wql,  
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_RETURN_WBEM_COMPLETE,  
		NULL,  
		&m_pclsEnum);  

	if (FAILED(hr)) return false;  


	//遍历对象，存入Vector数组方便使用  
	while (m_pclsEnum)  
	{  

		ULONG uReturn = 0;  
		IWbemClassObject *pclsObj=NULL;  

		hr = m_pclsEnum->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);  

		if(0 == uReturn)  
		{  
			break;  
		}  

		if (pclsObj) m_clslist.push_back(pclsObj);  
	}  


	return true;  
}

//释放COM对象  
void CWmi::Release()  
{  
	for (vector<IWbemClassObject *>::iterator i=m_clslist.begin();i!=m_clslist.end();i++)  
	{  
		(*i)->Release();  
	}  

	if (m_pclsEnum)  
	{  
		m_pclsEnum->Release();  
		m_pclsEnum=NULL;  
	}  

	if (m_pSvc)  
	{  
		m_pSvc->Release();  
		m_pSvc=NULL;  
	}  

	if (m_pLoc)  
	{  
		m_pLoc->Release();  
		m_pLoc=NULL;  
	}  

}  

//获取对象的方法  
HRESULT  CWmi::GetMethod(LPCTSTR lpszMethodName, IWbemClassObject **ppIn)  
{  
	if (m_pclsObj )  
	{  
		return m_pclsObj->GetMethod(bstr_t(lpszMethodName), 0, ppIn, NULL);  
	}  

	return SEVERITY_ERROR;  
}  

//SAFEARRAY数组转bstr_t  
bstr_t   CWmi::SafeArrayToString(SAFEARRAY *psa)  
{  
	bstr_t rstr;  

	VARIANT* pData=NULL;  

	VARTYPE vt;  

	SafeArrayGetVartype(psa,&vt);  

	int numElements = psa->rgsabound[0].cElements - psa->rgsabound[0].lLbound;   

	HRESULT hr=SafeArrayAccessData(psa,(void HUGEP**)&pData);  

	if(SUCCEEDED(hr))  
	{  
		for(int idx=0; idx < numElements; idx++)  
		{  
			if (vt==VT_BSTR)  
			{  
				rstr += ((BSTR *)(pData))[idx];  
			}  
		}  
		hr = SafeArrayUnaccessData(psa);  
	}  

	return rstr;  
}  

UCHAR *  CWmi::SafeArrayToUInt8(SAFEARRAY *psa,int * charSize)
{
	UCHAR     *uchar;
	VARIANT*   pData=NULL;
	VARTYPE    vt;
	SafeArrayGetVartype(psa,&vt);  

	int numElements = psa->rgsabound[0].cElements - psa->rgsabound[0].lLbound;  
	uchar = new UCHAR[numElements];
	*charSize = numElements;

	HRESULT hr=SafeArrayAccessData(psa,(void HUGEP**)&pData);  
	if(SUCCEEDED(hr))  
	{  
		for(int idx=0; idx < numElements; idx++)  
		{  
			if (vt==VT_UI1)  
			{  
				uchar[idx] = ((UCHAR *)(pData))[idx];  
			}  
		}  
		hr = SafeArrayUnaccessData(psa);  
	}  

	return uchar;
}


bool CWmi::GetObjectProp(int iObj, LPCTSTR lpszPropName,_variant_t *vtProp,VARTYPE varType)
{
	if((unsigned int)iObj < m_clslist.size())
	{
		HRESULT hr = m_clslist[iObj]->Get(bstr_t(lpszPropName), 0, vtProp, 0, 0);
		if (SUCCEEDED(hr) && vtProp->vt == varType)  
		{ 
			return true;
		}
	}

	return false;
}