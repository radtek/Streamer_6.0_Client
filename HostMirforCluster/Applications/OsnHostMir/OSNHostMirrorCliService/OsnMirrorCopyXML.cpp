
#include "OsnMirrorCopyXML.h"
#include "Configure.h"
#include "OSNRpcSocket.h"
#include "OSNRpcServer.h"
#include "OSNService.h"
#include "NewMirror.h"
#include "atlbase.h"


extern	COSNService			*pOSNService;

DWORD COsnMirrorCopyXML::CreateClientID()
{
	GUID guid;
	CRegKey *pRegKey = new CRegKey();

	if(S_OK == CoCreateGuid(&guid))
	{
		OsnGUIDToString(ClientID,guid);
		if(pRegKey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNHCService")!= ERROR_SUCCESS)
		{
			if(pRegKey->Create(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNHCService") != ERROR_SUCCESS)
			{
				delete(pRegKey);
				return EXIT_FAILURE;
			}

			pRegKey->SetStringValue("ClientID",ClientID);

			pRegKey->Close();
			delete(pRegKey);
			return EXIT_SUCCESS;
		}
	}
	delete(pRegKey);
	return EXIT_FAILURE;
}

void COsnMirrorCopyXML::InitializeMembers()
{
	GetSystemDisksInfo();
	GetSystemVolumesInfo();
	//ReadConfigurationFile();

	/*try
	{
	GetSystemMirrorInfo();
	this->treeView1->SelectedNode=this->treeView1->Nodes[0];
	RefreshComputerNode();
	}*/
	/*catch(Exception^ exx)
	{
	myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",exx->Message->ToString()),EventLogEntryType::Error,024);
	}*/
}

DWORD COsnMirrorCopyXML::OSNInitWMI(IWbemServices *m_pSvc,IWbemLocator *m_pLoc)
{
	HRESULT hres;
	
	m_pLoc = 0;
	hres = CoCreateInstance(
		CLSID_WbemLocator,            
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) &m_pLoc);

	if (FAILED(hres))
	{
		printf("CoCreateInstance error\n");
		//CoUninitialize();
		return EXIT_FAILURE;       // Program has failed.
	}

	//使用pLoc连接到” root\cimv2” 并把pSvc的指针也搞定了
	hres = m_pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
		NULL,                    // User name
		NULL,                    // User password
		0,                       // Locale
		NULL,                    // Security flags                
		0,                       // Authority      
		0,                       // Context object
		&m_pSvc                    // IWbemServices proxy
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
		m_pSvc,                         // the proxy to set
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

DWORD COsnMirrorCopyXML::OSNCloseWMI(IWbemServices *m_pSvc,IWbemLocator *m_pLoc,IEnumWbemClassObject *pEnumerator)
{
	if(m_pSvc != NULL)
	{
		m_pSvc->Release();
		m_pSvc = NULL;
	}

	if(m_pLoc != NULL)
	{
		m_pLoc->Release();
		m_pLoc = NULL;
	}

	if(pEnumerator != NULL)
	{
		pEnumerator->Release();
		pEnumerator = NULL;
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

	HRESULT hres;
	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_LogicalDisk WHERE DriveType=3"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
		);
	if (FAILED(hres))
	{
		printf("pSvc->ExecQuery error\n");
		OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
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

			CVolumeInfo	*pVolumeInfo = new CVolumeInfo(Free,size,freesize,guid,VolumeLabel,diskguid,(FileSys)Filesys,OnLine);

			if(this->pVolumeList->GetVolumeInfo(guid) == NULL)
				this->pVolumeList->AddItem((DWORD)pVolumeInfo);
		}
		
		delete(VolumeLabe2);
	}
	OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
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

	DWORD dw = OSNInitWMI(m_pSvc,m_pLoc);
	if(dw == EXIT_FAILURE)
	{
		printf("Init WMI error!\n");
		OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
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
		OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
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
		pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
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

			CDiskInfo *pNewDisk = new CDiskInfo(Free,pDisk.m_DiskSize,pDisk.m_DiskIndex,chguid,DiskOEM,MBR,Basic,Inited);
			if(pNewDisk != NULL)
			{
				if(pDiskList->GetDiskInfo(pNewDisk->m_Guid)==NULL 
					||pDiskList->GetDiskByIndex(pNewDisk->m_DiskIndex) == NULL)
					pDiskList->AddItem((DWORD)pNewDisk);
			}
		}
	}
	OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
	//}
	//catch(Exception^ exx)
	//{
	//myEventLog->OSNWriteEventLog(String::Concat("GetSystemDisksInfo,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
	//}
}

//void ReadConfigurationFile()
//		 {
//			 String ^strLine = nullptr;
//			 array<String^>^split= nullptr;
//			 array<Char>^chars = {';'};

//			 //read initiator information info
//			 String ^strFileName = String::Concat(ImagePath, L"\\OSNHostMirror.cfg");
//			 FileInfo ^fiHost = gcnew FileInfo(strFileName);
//			 if(!fiHost->Exists)
//			 {
//				 return;
//			 }

//			 //String ^strMsg = "Illegal line found in Configuration file.";

//			 StreamReader ^srHost = fiHost->OpenText();
//			 while((strLine = srHost->ReadLine()))
//			 {
//				 if(strLine->StartsWith("DiskMirrorInfo"))
//				 {
//					 //DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
//					 strLine = strLine->Substring(strlen("DiskMirrorInfo") + 1);
//					 split = strLine->Split( chars );

//					 int i=0;
//					 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
//					 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
//					 unsigned int uiEIMMode = 2;
//					 unsigned int isCluster=0;
//					 String ^clusterResourceName = "NONE";

//					 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
//					 while ( myEnum->MoveNext() )
//			   {
//				   String^ s = safe_cast<String^>(myEnum->Current);

//				   if(i == 0)
//				   {
//					   SrcGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }
//				   else if(i == 1)
//				   {
//					   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }
//				   else if(i == 2)
//				   {
//					   uiEIMMode = Convert::ToInt32(s);
//				   }
//				   else if(i == 3)
//				   {
//					   isCluster=Convert::ToInt32(s);
//				   }
//				   else if(i == 4)
//				   {
//					   clusterResourceName = s;
//				   }
//				   ++i;
//			   }

//					 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
//					 this->pDiskMirrorList->AddItem(pMirrorInfo);
//				 }

//				 if(strLine->StartsWith("VolumeMirrorInfo"))
//				 {
//					 //VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
//					 strLine = strLine->Substring(strlen("VolumeMirrorInfo") + 1);
//					 split = strLine->Split( chars );

//					 int i=0;
//					 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
//					 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
//					 unsigned int uiEIMMode = 2;
//					 unsigned int isCluster = 0;
//					 String		^clusterResourceName = "NONE";

//					 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
//					 while ( myEnum->MoveNext() )
//			   {
//				   String^ s = safe_cast<String^>(myEnum->Current);

//				   if(i == 0)
//				   {
//					   SrcGuid =s; //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }
//				   /*else if(i == 1)
//				   {
//				   uiSrcDiskOffset = UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }*/
//				   else if(i == 1)
//				   {
//					   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }
//				   /* else if(i == 3)
//				   {
//				   uiDesDiskOffset = UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
//				   }*/
//				   else if(i == 2)
//				   {
//					   uiEIMMode = Convert::ToInt32(s);
//				   }
//				   else if(i == 3)
//				   {
//					   isCluster = Convert::ToInt32(s);
//				   }
//				   else if(i == 4)
//				   {
//					   clusterResourceName = s;
//				   }
//				   ++i;
//			   }

//					 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
//					 this->pVolumeMirrorList->AddItem(pMirrorInfo);
//				 }


//			 }
//			 srHost->Close();
//		 }

//void COsnMirrorCopyXML::GetSystemMirrorInfo()
//{
//
//	if(this->m_InstallType == VolumeCopy)
//		GetVolumeCopyMirrorInfo();
//	else if(this->m_InstallType == DiskCopy)
//		GetDiskCopyMirrorInfo();
//	else
//	{
//		GetVolumeCopyMirrorInfo();
//		GetDiskCopyMirrorInfo();
//
//	}
//}

void COsnMirrorCopyXML::RefreshVolumeListXML()
{
	int index=0;
	for( int i=0;i<this->pVolumeList->GetLength();i++)
	{
		char temp[128];
		CVolumeInfo *pVolumeInfo =(CVolumeInfo *)(this->pVolumeList->GetItem(i));

		// 判断如果卷所在磁盘是镜像副本，那就过滤
		if(pDiskList->HideVolume(pVolumeInfo->m_GUID))
		{
			continue;
		}

		char DiskGuidC[64];
		WcharToChar(pVolumeInfo->m_DiskGUID->c_str(),DiskGuidC,sizeof(DiskGuidC));
		m_pTempXML->AddXMLVolElement("ClientPhyDisk","PartitionVolume",DiskGuidC);

		WcharToChar((wchar_t*)pVolumeInfo->m_Protected.c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("PartitionVolume","IsProtected",temp);

		_i64toa_s(pVolumeInfo->m_VolumeSize,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("PartitionVolume","Size",temp);

		_i64toa_s(pVolumeInfo->m_UsedSpace,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("PartitionVolume","UsedSize",temp);

		WcharToChar((wchar_t*)pVolumeInfo->m_GUID->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("PartitionVolume","Guid",temp);

		WcharToChar((wchar_t*)pVolumeInfo->m_VolumeLable->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("PartitionVolume","Index",temp);

		_itoa_s(pVolumeInfo->m_FileSys,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("PartitionVolume","FileSys",temp);

		WcharToChar((wchar_t*)pVolumeInfo->m_DiskGUID->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("PartitionVolume","InPhyDiskGuid",temp);

		m_pTempXML->AddXMLAttribute("PartitionVolume","ClientID",ClientID);
		m_pTempXML->AddXMLAttribute("PartitionVolume","State","1");
		

		index++;
	}
}

void COsnMirrorCopyXML::RefreshDiskListXML()
{
	for( int i=0;i<this->pDiskList->GetLength();i++)
	{
		char temp[128];
		CDiskInfo *pDiskInfo = (CDiskInfo *)(this->pDiskList->GetItem(i));

		m_pTempXML->AddXMLDisElement("Client","ClientPhyDisk");

		WcharToChar((wchar_t*)pDiskInfo->m_Protected.c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","IsProtected",temp);

		_i64toa_s(pDiskInfo->m_DiskSize,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Size",temp);

		_itoa_s(pDiskInfo->m_DiskIndex,temp,sizeof(temp),10);
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Index",temp);

		WcharToChar((wchar_t*)pDiskInfo->m_Guid->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Guid",temp);

		WcharToChar((wchar_t*)pDiskInfo->m_DiskOEM->c_str(),temp,sizeof(temp));
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Oem",temp);

		m_pTempXML->AddXMLAttribute("ClientPhyDisk","ClientID",ClientID);

		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Format","MBR");
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","Style","0");
		m_pTempXML->AddXMLAttribute("ClientPhyDisk","State","1");

	}
}

void COsnMirrorCopyXML::RefreshClientXML()
{
	char hostname[32],ipAddress[32],SysVersion[32]; 
	DWORD dw = pOSNService->m_OSNRpcServer.OSNRpcGetBasicInfo(hostname,ipAddress,SysVersion);

	m_pTempXML->AddXMLElement("Client");

	if(pOSNService->m_OSNRpcServer.m_IsProtected == true)
	{
		m_pTempXML->AddXMLAttribute("Client","IsProtected","true");
	}
	else
	{
		m_pTempXML->AddXMLAttribute("Client","IsProtected","false");
	}

	m_pTempXML->AddXMLAttribute("Client","ClientID",ClientID);
	m_pTempXML->AddXMLAttribute("Client","State","0");
	m_pTempXML->AddXMLAttribute("Client","Name",hostname);
	m_pTempXML->AddXMLAttribute("Client","IpAddr",ipAddress);
	m_pTempXML->AddXMLAttribute("Client","ManagerServIP",pOSNService->m_OSNRpcServer.m_ServerIP);
	m_pTempXML->AddXMLAttribute("Client","SystemType","1");
	m_pTempXML->AddXMLAttribute("Client","SystemVersion",SysVersion);

	m_pTempXML->AddXMLAttribute("Client","Database","0");
	m_pTempXML->AddXMLAttribute("Client","DatabaseVersion","0");
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

		DWORD dw = OSNInitWMI(m_pSvc,m_pLoc);
		if(dw == EXIT_FAILURE)
		{
			printf("Init WMI error!\n");
			OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
			return 3;
		}

		HRESULT hres;
		wchar_t  pWQL[32];
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
			OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
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
			pclsObj->Get(L"Bootable", 0, &vtProp, 0, 0);
			wstring *VolumeLabel	= new wstring(vtProp.bstrVal);	//C:, D:, E:, etc.
            DWORD dw = _wtoi(VolumeLabel->c_str());
			delete(VolumeLabel);
			OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
			return dw;
		}
		OSNCloseWMI(m_pSvc,m_pLoc,pEnumerator);
		return false;
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
		//char pImagePath[256];
		//WcharToChar((wchar_t*)ImagePath->c_str(),pImagePath);
		//strcat(pImagePath,"\\OSNHostMirror.cfg");

		//fstream fi(pImagePath,ios::in|ios::out);
		//if(!fi->Exists)
		//{
		//	return;
		//}

		//fi->CopyTo(String::Concat(ImagePath, L"\\OSNHostMirror.cfg.bak"), true);

		//FileStream	^fs = fi->Open(FileMode::Create); 
		//StreamWriter	^sw = gcnew StreamWriter(fs); 
		//sw->AutoFlush = true;

		//int i;
		//for(i=0; i<this->pDiskMirrorList->GetLength(); i++)
		//{
		//	CMirrorInfo *pMirrorInfo = (CMirrorInfo *)(pDiskMirrorList->GetItem(i));
		//	
		//	wstring *strLine = String::Concat("DiskMirrorInfo=", pMirrorInfo->m_SourceGuid, ";");
		//	strLine = String::Concat(strLine, pMirrorInfo->m_TargetGuid, ";");
		//	strLine = String::Concat(strLine, pMirrorInfo->m_EIMMode.ToString(),";");
		//	strLine = String::Concat(strLine,pMirrorInfo->m_IsCluster.ToString(),";");
		//	strLine = String::Concat(strLine,pMirrorInfo->m_ClusterResourceName);
		//	sw->WriteLine(strLine);
		//}

		//for(i=0; i<this->pVolumeMirrorList->GetLength(); i++)
		//{
		//	CMirrorInfo *pMirrorInfo = (CMirrorInfo *)(pVolumeMirrorList->GetItem(i));

		//	String ^strLine = String::Concat("VolumeMirrorInfo=", pMirrorInfo->m_SourceGuid, ";");
		//	//strLine = String::Concat(strLine, pMirrorInfo->m_SourceOffset.ToString("X8"), ";");
		//	strLine = String::Concat(strLine, pMirrorInfo->m_TargetGuid, ";");
		//	//strLine = String::Concat(strLine, pMirrorInfo->m_TargetOffset.ToString("X8"), ";");
		//	strLine = String::Concat(strLine, pMirrorInfo->m_EIMMode.ToString(),";");
		//	strLine = String::Concat(strLine,pMirrorInfo->m_IsCluster.ToString(),";");
		//	strLine = String::Concat(strLine,pMirrorInfo->m_ClusterResourceName);
		//	sw->WriteLine(strLine);
		//	/*sw->WriteLine("VolumeMirrorInfo={0:G};{1:G};{2:G};{3:G};{4:G}", pMirrorInfo->m_SourceDiskSignature.ToString("X8"),
		//	pMirrorInfo->m_SourceOffset.ToString("X8"), pMirrorInfo->m_TargetDiskSignature.ToString("X8"), 
		//	pMirrorInfo->m_TargetOffset.ToString("X8"), pMirrorInfo->m_EIMMode.ToString());*/
		//}
		//sw->Close();
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