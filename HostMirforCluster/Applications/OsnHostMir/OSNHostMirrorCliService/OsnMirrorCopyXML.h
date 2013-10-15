#include <stdio.h>
#include <locale.h>
#include "Globallist.h"
#include <Wbemidl.h>
#include "COMDEF.H"
#include "OSNXml.h"
#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"
# pragma comment(lib, "wbemuuid.lib")

class COsnMirrorCopyXML
{
public:  
	CMirrorInfoList *pVolumeMirrorList;
	CMirrorInfoList *pDiskMirrorList;
	CVolumeInfoList *pVolumeList;
	CDiskInfoList   *pDiskList;

	IWbemServices *m_pSvc;
	IWbemLocator  *m_pLoc;
	COSNxml       *m_pTempXML;

public:
	COsnMirrorCopyXML()
	{
		this->pVolumeMirrorList = new CMirrorInfoList(128);
		this->pDiskMirrorList = new CMirrorInfoList(128);
		this->pVolumeList = new CVolumeInfoList(256);
		this->pDiskList = new CDiskInfoList(256);

		InitializeMembers();
	}

	~COsnMirrorCopyXML()
	{
		delete(m_pTempXML);
		delete(pVolumeMirrorList);
		delete(pDiskMirrorList);
		delete(pVolumeList);
		delete(pDiskList);

		m_pSvc->Release();
		m_pLoc->Release();    
		CoUninitialize();
		return ;
	}

	void InitializeMembers()
	{
		DWORD dw = OSNInitWMI();
		if(dw == EXIT_FAILURE)
		{
			printf("Init WMI error!\n");
			return;
		}

		GetSystemDisksInfo();
		GetSystemVolumesInfo();

		OSNCloseWMI();
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

public:
	DWORD OSNInitWMI()
	{
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
			CoUninitialize();
			return EXIT_FAILURE;          // Program has failed.
		}

		//创建一个CLSID_WbemLocator对象
		m_pLoc = 0;
		hres = CoCreateInstance(
			CLSID_WbemLocator,            
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID *) &m_pLoc);
 
		if (FAILED(hres))
		{
			printf("CoCreateInstance error\n");
			CoUninitialize();
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
			m_pLoc->Release();    
			CoUninitialize();
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
			m_pSvc->Release();
			m_pLoc->Release();    
			CoUninitialize();
			return EXIT_FAILURE;               // Program has failed.
		}

		return EXIT_SUCCESS;
	}

	DWORD OSNCloseWMI()
	{
		m_pSvc->Release();
		m_pLoc->Release();    
		CoUninitialize();

		return EXIT_SUCCESS;
	}

	DWORD CharToWchar(char *pChar,wchar_t *pWchar)
	{
		if(!pChar || !pWchar)
		{
			return EXIT_FAILURE;
		}
		int   nLen  = strlen(pChar) + 1;      
		int   nwLen = MultiByteToWideChar(CP_ACP,0,pChar,nLen,NULL,0);  

		if(0 == MultiByteToWideChar(CP_ACP,0,pChar,nLen,pWchar,nwLen))
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}

	DWORD WcharToChar(wchar_t *pWchar,char *pChar)
	{
		if(!pChar || !pWchar)
		{
			return EXIT_FAILURE;
		}       

        int   nLen = wcslen(pWchar)+1;        

		if(0 == WideCharToMultiByte(CP_ACP,0,pWchar,nLen,pChar,2*nLen,NULL,NULL)) 
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}

	void GetSystemVolumesInfo()
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
			HRESULT hres;
			IEnumWbemClassObject* pEnumerator = NULL;
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
				CharToWchar(DiskGUIDC,DiskGUIDW);
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
					wstring *guid=NULL;
					if(ret)
					{
						wchar_t   lpszFileW[256];
						CharToWchar(char_guid,lpszFileW);
						guid=new wstring(lpszFileW);

						GUID guid0;

						char   lpszFileC[256];
						WcharToChar((wchar_t *)(guid->c_str()),lpszFileC);
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
						CharToWchar(char_guid,lpszFile);
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

					if(this->pVolumeList->GetVolumeInfo(guid)== NULL)
								 this->pVolumeList->AddItem((DWORD)pVolumeInfo);
				}
			}
		//}
	    /*catch(Exception ^e)
		{
			MessageBox::Show("获取分区出异常.");
			MessageBox::Show(e->Message);
		}*/
	}
	

	void GetSystemDisksInfo()
	{
		this->pDiskList ->Clear();
				 
		//try
		//{
			DISK_INFO     pDisk;
			DISK_INFOEX   pDiskEx;
			int ErrorCode=0;

			HRESULT hres;
			IEnumWbemClassObject* pEnumerator = NULL;
			hres = m_pSvc->ExecQuery(
				bstr_t("WQL"),
				bstr_t("SELECT * FROM Win32_DiskDriver WHERE DriveType=3"),
				WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
				NULL,
				&pEnumerator
				);
			if (FAILED(hres))
			{
				printf("pSvc->ExecQuery error\n");
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
					CharToWchar(guid,lpszFileW);
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

	//		 void GetSystemMirrorInfo()
	//		 {
	//			 
	//			 if(this->m_InstallType == VolumeCopy)
	//				 GetVolumeCopyMirrorInfo();
	//			 else if(this->m_InstallType == DiskCopy)
	//				 GetDiskCopyMirrorInfo();
	//			 else
	//			 {
	//				 GetVolumeCopyMirrorInfo();
	//				 GetDiskCopyMirrorInfo();

	//			 }
	//		 }

public:
	void RefreshVolumeListXML()
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

			m_pTempXML->AddXMLSubElement("ClientPhyDisk","PartitionVolume");

			WcharToChar((wchar_t*)pVolumeInfo->m_Protected.c_str(),temp);
			m_pTempXML->AddXMLAttribute("PartitionVolume","IsProtected",temp);

			_i64toa(pVolumeInfo->m_VolumeSize,temp,10);
			m_pTempXML->AddXMLAttribute("PartitionVolume","Size",temp);

			_i64toa(pVolumeInfo->m_UsedSpace,temp,10);
			m_pTempXML->AddXMLAttribute("PartitionVolume","UsedSize",temp);

			WcharToChar((wchar_t*)pVolumeInfo->m_GUID->c_str(),temp);
			m_pTempXML->AddXMLAttribute("PartitionVolume","Guid",temp);

			WcharToChar((wchar_t*)pVolumeInfo->m_VolumeLable->c_str(),temp);
			m_pTempXML->AddXMLAttribute("PartitionVolume","Index",temp);

			_itoa(pVolumeInfo->m_FileSys,temp,10);
			m_pTempXML->AddXMLAttribute("PartitionVolume","FileSys",temp);

			WcharToChar((wchar_t*)pVolumeInfo->m_DiskGUID->c_str(),temp);
			m_pTempXML->AddXMLAttribute("PartitionVolume","InPhyDiskGuid",temp);

			m_pTempXML->AddXMLAttribute("PartitionVolume","State","1");
			m_pTempXML->AddXMLAttribute("PartitionVolume","ClientID","0");

			index++;
		}
	}

	void RefreshDiskListXML()
		 {
			for( int i=0;i<this->pDiskList->GetLength();i++)
			 {
				 char temp[128];
				 CDiskInfo *pDiskInfo = (CDiskInfo *)(this->pDiskList->GetItem(i));

				 m_pTempXML->AddXMLSubElement("Client","ClientPhyDisk");

				 WcharToChar((wchar_t*)pDiskInfo->m_Protected.c_str(),temp);
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","IsProtected",temp);

				 _i64toa(pDiskInfo->m_DiskSize,temp,10);
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Size",temp);

				 _itoa(pDiskInfo->m_DiskIndex,temp,10);
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Index",temp);

				 WcharToChar((wchar_t*)pDiskInfo->m_Guid->c_str(),temp);
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Guid",temp);

				 WcharToChar((wchar_t*)pDiskInfo->m_DiskOEM->c_str(),temp);
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Oem","IBM");

				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","ClientID","0");
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Format","MBR");
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","Style","0");
				 m_pTempXML->AddXMLAttribute("ClientPhyDisk","State","1");

			 }
		 }
};