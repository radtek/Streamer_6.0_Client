#include "NewMirror.h"
#include "atlbase.h"
#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"
#include "Common.h"

DWORD CNewMirror::CheckVolIsBootableOrSys(wstring *label)
{
	try
	{
		IWbemServices          *m_pSvc = NULL;
		IWbemLocator           *m_pLoc = NULL;
		IEnumWbemClassObject   *pEnumerator = NULL;

		DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
		if(dw == EXIT_FAILURE)
		{
			LOG(INFO) << "Init WMI error!\n";
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return -1;
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
			LOG(INFO) << "pSvc->ExecQuery error\n";
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return 3;               // Program has failed.
		}
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		/*wstring *sysvol=(wstring *)System::Environment::SystemDirectory;
		sysvol = sysvol->Substring(0,2);
		bool issysvol = false;
		if(sysvol->compare(label) == 0)
		{
			issysvol = true;
			return  issysvol;
		}*/

		while(pEnumerator)
		{
			// �Ƴ���һ������
			pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
			if(0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;
			DWORD dw = 3;
			if(pclsObj->Get(L"Bootable", 0, &vtProp, 0, 0) == S_OK)
			{
				dw = vtProp.boolVal;

				pclsObj->Release();
				OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
				VariantClear(&vtProp);
				return dw;
			}
		}
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return 3;
	}
	catch(exception err)
	{
		LOG(ERROR) << "CheckVolIsBootable:" << err.what();
	}
	return false;
}

DWORD CNewMirror::CheckFileSystem(wstring *LabelName)
{

	if(LabelName == NULL)
		return 0;

	char VolumeNameBuffer[32];
	DWORD lpVolumeSerialNumber;
	DWORD lpMaximumComponentLength;
	DWORD lpFileSystemFlags;
	char  lpFileSystemNameBuffer[32];
	wstring temp=L"\\";
	char  LabelNameC[10];

	temp +=  *LabelName;
	WcharToChar((wchar_t*)temp.c_str(),LabelNameC,sizeof(LabelNameC));
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

DWORD CNewMirror::CharToWchar(const char *pChar,wchar_t *pWchar,int Length)
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

DWORD CNewMirror::WcharToChar(const wchar_t *pWchar,char *pChar,int Length)
{
	if(!pChar || !pWchar)
	{
		return EXIT_FAILURE;
	}       

	int   nLen = wcslen(pWchar)+1;        

	if(Length < nLen)
		return EXIT_FAILURE;

	if(0 == WideCharToMultiByte(CP_ACP,0,pWchar,nLen,pChar,2*nLen,NULL,NULL)) 
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

DWORD CNewMirror::OSNInitWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,wchar_t *pResName)
{
	HRESULT hres;
	hres = CoCreateInstance(
		CLSID_WbemLocator,            
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) &(*m_pLoc));

	if (FAILED(hres))
	{
		LOG(INFO) << "CoCreateInstance error\n";
		//CoUninitialize();
		return EXIT_FAILURE;       // Program has failed.
	}

	//ʹ��pLoc���ӵ��� root\cimv2�� ����pSvc��ָ��Ҳ�㶨��
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
		LOG(INFO) << "ConnectServer error\n";
		//m_pLoc->Release();    
		//CoUninitialize();
		return EXIT_FAILURE;                // Program has failed.
	}
	//�Ѿ����ӵ�WMI��

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
		LOG(INFO) << "CoSetProxyBlanket error\n";
		//m_pSvc->Release();
		//m_pLoc->Release();    
		//CoUninitialize();
		return EXIT_FAILURE;               // Program has failed.
	}

	return EXIT_SUCCESS;
}

DWORD CNewMirror::OSNCloseWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,IEnumWbemClassObject **pEnumerator)
{
	if((*m_pSvc) != NULL)
	{
		(*m_pSvc)->Release();
		(*m_pSvc) = NULL;
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

DWORD CNewMirror::CheckDiskIsEIMDisk(int index)
{
	IWbemServices         *m_pSvc = NULL;
	IWbemLocator          *m_pLoc = NULL;
	IEnumWbemClassObject* pEnumerator = NULL;

	try
	{
		DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
		if(dw == EXIT_FAILURE)
		{
			LOG(INFO) << "Init WMI error!\n";
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return -1;
		}

		HRESULT hres;
		wchar_t  pWQL[128];
		swprintf_s(pWQL,_countof(pWQL),L"Select * from Win32_DiskDrive where Index=%d",index);

		hres = m_pSvc->ExecQuery(
			bstr_t("WQL"),
			pWQL,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator
			);
		if (FAILED(hres))
		{
			LOG(INFO) << "pSvc->ExecQuery error\n";
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return -1;               // Program has failed.
		}
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		while(pEnumerator)
		{
			// �Ƴ���һ������
			pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
			if(0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;
			if(S_OK == pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0))
			{
				wstring *VolumeLabel	= new wstring(vtProp.bstrVal);	//C:, D:, E:, etc.
				
				VariantClear(&vtProp);
				if(wcsstr(VolumeLabel->c_str(),L"EIM"))
				{
					delete(VolumeLabel);
					pclsObj->Release();
					OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
					return 1;
				}
				else
				{
					delete(VolumeLabel);
					pclsObj->Release();
					OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
					return 2;
				}
			}
		}
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	}
	catch(...)
	{
		LOG(INFO) << "��ȡ���̵��ṩ����Ϣ�������Ժ�����";
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	}
}

DWORD CNewMirror::InitalizeDisk(int index)
{
	try
	{
		HANDLE hChildStdinRd = NULL, hChildStdinWr = NULL,hChildStdoutWr = NULL,hChildStdoutRd = NULL;
		SECURITY_ATTRIBUTES saAttr; 

		// Set the bInheritHandle flag so pipe handles are inherited. 

		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
		saAttr.bInheritHandle = TRUE; 
		saAttr.lpSecurityDescriptor = NULL;

		if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
		{
			printf("Stdout pipe creation failed\n");
			return 1;
		}
		SetHandleInformation( hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);

		if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 
		{
			printf("Stdin pipe creation failed\n");
			return 1;
		}
		SetHandleInformation( hChildStdinWr, HANDLE_FLAG_INHERIT, 0);

		// Now create the child process. 
		TCHAR szCmdline[]=TEXT("diskpart.exe");
		PROCESS_INFORMATION piProcInfo; 
		STARTUPINFO siStartInfo;
		BOOL bFuncRetn = FALSE; 

		ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
		ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
		siStartInfo.cb = sizeof(STARTUPINFO); 
		siStartInfo.hStdError = hChildStdoutWr;
		siStartInfo.hStdOutput = hChildStdoutWr;       //����̨����
		siStartInfo.hStdInput = hChildStdinRd;         //����
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		bFuncRetn = CreateProcess(NULL, 
			szCmdline,     // command line 
			NULL,          // process security attributes 
			NULL,          // primary thread security attributes 
			TRUE,          // handles are inherited 
			0,             // creation flags 
			NULL,          // use parent's environment 
			NULL,          // use parent's current directory 
			&siStartInfo,  // STARTUPINFO pointer 
			&piProcInfo);  // receives PROCESS_INFORMATION 

		if (bFuncRetn == 0) 
		{
			printf("CreateProcess failed\n");
			return 1;
		}
		else 
		{
			CloseHandle(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
		}

		// Write to pipe that is the standard input for a child process. 
		DWORD dwWritten;
		char pIndex[5];
		itoa(index,pIndex,10);
		string s = "select disk ";
		s += pIndex;
		WriteFile(hChildStdinWr, s.c_str(),s.size()+1, &dwWritten, NULL);
		WriteFile(hChildStdinWr, "clean",sizeof("clean"), &dwWritten, NULL);
		WriteFile(hChildStdinWr, "create partition primary",sizeof("create partition primary"), &dwWritten, NULL);
		WriteFile(hChildStdinWr, "delete partition",sizeof("delete partition"), &dwWritten, NULL);
		WriteFile(hChildStdinWr, "exit",sizeof("exit"), &dwWritten, NULL);

		if(hChildStdinRd == NULL)
		{
			CloseHandle(hChildStdinRd);
		}
		if(hChildStdinWr == NULL)
		{
			CloseHandle(hChildStdinWr);
		}
		if(hChildStdoutWr == NULL)
		{
			CloseHandle(hChildStdoutWr);
		}
		if(hChildStdoutRd == NULL)
		{
			CloseHandle(hChildStdoutRd);
		}
		Sleep(5000);
		return 0; 
	}
	catch(...)
	{
		LOG(ERROR) << "��ʼ�����̳����쳣,���ֶ���ʼ������:" << index;
		return 1;
	}
}

///1 eim disk 2,don't eim disk ,o get failed
DWORD CNewMirror::CheckVolIsEIMVol(wstring *LabelName)
{
	IWbemServices *m_pSvc = NULL;
	IWbemLocator  *m_pLoc = NULL;
	IEnumWbemClassObject* pEnumerator = NULL;

	try
	{
		DWORD dw = OSNInitWMI(&m_pSvc,&m_pLoc,L"ROOT\\CIMV2");
		if(dw == EXIT_FAILURE)
		{
			LOG(INFO) << "Init WMI error!\n";
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			return -1;
		}

		HRESULT hres;
		wchar_t  pWQL[128];
		swprintf_s(pWQL,_countof(pWQL),L"Associators of {win32_LogicalDisk='%s'} where resultClass = Win32_DiskPartition",LabelName->c_str());

		hres = m_pSvc->ExecQuery(
			bstr_t("WQL"),
			pWQL,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator
			);
		if (FAILED(hres))
		{
			OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
			LOG(INFO) << "pSvc->ExecQuery error\n";
			return -1;               // Program has failed.
		}
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		while(pEnumerator)
		{
			// �Ƴ���һ������
			pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
			if(0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;
			if(S_OK == pclsObj->Get(L"DiskIndex", 0, &vtProp, 0, 0))
			{
				int Index2 = vtProp.lVal;

				VariantClear(&vtProp);
				pclsObj->Release();
				OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
				return CheckDiskIsEIMDisk(Index2); 
			}
		}
	}
	catch(...)
	{
		LOG(INFO) << "��ȡ����ṩ����Ϣʧ�ܣ����Ժ�����";
		OSNCloseWMI(&m_pSvc,&m_pLoc,&pEnumerator);
		return -1;
	}
}
void CNewMirror::VolumeMirrorClick(wstring *pSrcGuid,wstring *pDesGuid)
{
	bool isEIMMirror = false;

	//ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

	this->pTargetVolume = this->pSystemVolumeList->GetVolumeInfoByString(pDesGuid);

	if(pTargetVolume == NULL)
	{
		LOG(INFO) << "�޷���ȡĿ��������";
		return;
	}

	CRegKey *pRegKey = new CRegKey();
	DWORD Value;
	if(pRegKey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNVSS\\Parameters")!= ERROR_SUCCESS)
	{
		isEIMMirror = false;
	}
	else 
	{
		pRegKey->QueryDWORDValue("EIMMirror",Value);
		if(Value == 0)
		{
			isEIMMirror = false;
			pRegKey->SetDWORDValue("EIMMirror",2);
		}
		else 
		{
			if(Value == 1)
			{
				isEIMMirror = true;
			}
		}
		pRegKey->Close();
	}

	int ret=CheckVolIsEIMVol(this->pTargetVolume->m_VolumeLable);
	int ret1=CheckVolIsEIMVol(this->pSourceVolume->m_VolumeLable);

	if(!isEIMMirror&&2==ret&&2==ret1)
	{
		LOG(INFO) << "Դ���Ŀ�������EIM�����ܽ������ϵ��";
		delete(pRegKey);
		return ;
	}

	//if(1==this->m_EIMMode)//source
	//{
	//	int ret=CheckVolIsEIMVol(this->pSourceVolume->m_VolumeLable);
	//	if(-1==ret)
	//	{
	//		return;
	//	}
	//	else if(2==ret)
	//	{
	//		printf("Դ����EIM��");
	//		/*if(MessageBox::Show("Դ����EIM����ò�Ҫѡsourceģʽ���Ƿ����","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
	//		{
	//			return;
	//		}*/
	//	}
	//}
	if(2==this->m_EIMMode)//target
	{
		int ret=CheckVolIsEIMVol(this->pTargetVolume->m_VolumeLable);
		if(-1==ret)
		{
			delete(pRegKey);
			return;
		}
		else if(2==ret)
		{
			LOG(INFO) << "Ŀ�����EIM��";
		}
	}
	//else if(4==this->m_EIMMode)//both
	//{
	//	int ret=CheckVolIsEIMVol(this->pTargetVolume->m_VolumeLable);
	//	int ret1=CheckVolIsEIMVol(this->pSourceVolume->m_VolumeLable);
	//	if(-1==ret||-1==ret)
	//	{
	//		return;
	//	}
	//	else if(2==ret||2==ret1)
	//	{
	//		if(MessageBox::Show("Ŀ��������Դ����EIM����ò�Ҫѡbothģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
	//		{
	//			return;
	//		}
	//	}
	//}
	int tgtret= OsnCheckGptVolume((const char)*this->pTargetVolume->m_VolumeLable->c_str());
	int srcret= OsnCheckGptVolume((const char)*this->pSourceVolume->m_VolumeLable->c_str());

	if(tgtret>1||srcret>1)
	{
		LOG(INFO) << "Դ���Ŀ�������һ�����ڱ�����״̬��������δ֪״̬�������Ƿ��ھ����ϵ�У��������Ѿ�ɾ�������ϵ��Ҫ��������Ч";
		delete(pRegKey);
		return;
	}

	if(this->pSourceVolume->m_VolumeSize>pTargetVolume->m_VolumeSize)
	{
		LOG(INFO) << "������Դ��С";
	}

	int isbootvol=CheckVolIsBootableOrSys(this->pTargetVolume->m_VolumeLable);
	if(isbootvol)
	{
		LOG(INFO) << "�������ϵͳ���������������������Ϊ�������ѡ��������";
		delete(pRegKey);
		return;
	}

	// check the file system
	int ErrorCode = CheckFileSystem(this->pTargetVolume->m_VolumeLable);
	if(ErrorCode!=0)
	{
		LOG(INFO) << "������Ѿ����ļ�ϵͳ����";
	}

	delete(pRegKey);
	return;
}

void CNewMirror::DiskMirrorClick(wstring *pSrcGuid,wstring *pDesGuid)
{
	bool isEIMMirror = false;

	this->pTargetDisk = this->pSystemDiskList ->GetDiskInfoByString(pDesGuid);
	if(pTargetDisk == NULL)
	{
		LOG(INFO) << "�޷���ȡĿ��������";
		return;
	}

	CRegKey *pRegKey = new CRegKey();
	DWORD Value;
	if(pRegKey->Open(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\OSNDSS\\Parameters")!= ERROR_SUCCESS)
	{
		isEIMMirror = false;
	}
	else 
	{
		pRegKey->QueryDWORDValue("EIMMirror",Value);
		if(Value == 0)
		{
			isEIMMirror = false;
			pRegKey->SetDWORDValue("EIMMirror",2);
		}
		else 
		{
			if(Value == 1)
			{
				isEIMMirror = true;
			}
		}
		pRegKey->Close();
	}

	int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
	int ret1=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
	if(!isEIMMirror&&2==ret&&2==ret1)
	{
		LOG(INFO) << "Դ���̺�Ŀ����̶�����EIM�����ܽ������ϵ��";
		delete(pRegKey);
		return ;
	}

	//if(1==this->m_EIMMode)//source
	//{
	//	int ret=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
	//	if(-1==ret)
	//	{
	//		return;
	//	}
	//	else if(2==ret)
	//	{
	//		if(MessageBox::Show("Դ���̲���EIM����ò�Ҫѡsourceģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
	//		{
	//			return;
	//		}
	//	}
	//}
	if(2==this->m_EIMMode)//target
	{
		int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
		if(-1 == ret)
		{
			delete(pRegKey);
			return;
		}
		else if(2==ret)
		{
			LOG(INFO) << "Ŀ�����EIM��";
		}
	}
	//else if(4==this->m_EIMMode)//both
	//{
	//	int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
	//	int ret1=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
	//	if(-1==ret||-1==ret)
	//	{
	//		return;
	//	}
	//	else if(2==ret||2==ret1)
	//	{
	//		if(MessageBox::Show("Ŀ����̻�����Դ���̲���EIM����ò�Ҫѡbothģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
	//		{
	//			return;
	//		}
	//	}
	//}

	// check the partition information on this disk
	int tgtret= OsnCheckGptDisk(this->pTargetDisk->m_DiskIndex);
	int srcret= OsnCheckGptDisk(this->pSourceDisk->m_DiskIndex);
	// MessageBox::Show(String::Concat("target=",tgtret.ToString()));
	//MessageBox::Show(String::Concat("source=",srcret.ToString()));
	if(tgtret>1||srcret>1)
	{
		LOG(INFO) << "Դ���Ŀ�������һ�����ڱ�����״̬��������δ֪״̬�������Ƿ��ھ����ϵ�У��������Ѿ�ɾ�������ϵ��Ҫ��������Ч";
		delete(pRegKey);
		return;
	}
	if(tgtret!=srcret)
	{
		LOG(INFO) << "Դ���Ŀ���һ����MBR���̣�һ����GPT���̣����ܴ��������ϵ,������ѡ��Ŀ���";
		delete(pRegKey);
		return;
	}

	int ErrorCode =OSNCheckDiskRawProperty(pTargetDisk->m_DiskIndex);
	if(ErrorCode!=0)
	{
		LOG(INFO) << "��������Ѿ��з�������";
	}
	if(this->pTargetDisk->m_Guid->compare(L"00000000-0000-0000-0000-000000000000"))//û�г�ʼ������Ҫִ�г�ʼ������
	{
		int ret=InitalizeDisk(this->pTargetDisk->m_DiskIndex);
		if(1 == ret)
		{
			delete(pRegKey);
			return;
		}
		else
		{
			DISK_INFO pDisk;
			int ErrorCode;
			memset(&pDisk,0,sizeof(DISK_INFO));
			pDisk.m_DiskIndex=this->pTargetDisk->m_DiskIndex;
			ErrorCode=OsnGetDisk(&pDisk);
			if(ErrorCode==0)
			{     
				wstring *chguid = NULL;
				char guidC[64];
				wchar_t guidW[64];
				OsnGUIDToString(guidC,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
				CharToWchar(guidC,guidW,_countof(guidW));
				chguid=new wstring(guidW);
				this->pTargetDisk->m_Guid=chguid;
				this->pTargetDisk->m_DiskSize=pDisk.m_DiskSize;
			}
		}
	}

	if(this->pSourceDisk->m_DiskSize>this->pTargetDisk->m_DiskSize)
	{
		LOG(INFO) << "������̱�Դ����ҪС";
	}

	delete(pRegKey);

	return;
}


void CNewMirror::MirrorOKClick(wstring *pSrcGuid,wstring *pDesGuid) 
{
	/*if(this->comboBoxMode->Text->Equals(String::Empty))
	{
		printf("��ѡ��EIM��Ľ�ɫ��");
		return;
	}
	if(this->comboBoxMode->Text->Equals("Source"))
	{
		this->m_EIMMode = 1;
	}
	else if(this->comboBoxMode->Text->Equals("Target"))
	{
		this->m_EIMMode = 2;
	}
	else if(this->comboBoxMode->Text->Equals("Both"))
	{
		this->m_EIMMode = 4;			
	}*/

	this->m_EIMMode = 2;
	
	/*if(this->ckbCluster->Checked)
	{

		RegistryKey ^pRegKey=Registry::LocalMachine;
		RegistryKey ^pParmeterKey;

		pParmeterKey=pRegKey->OpenSubKey(String::Concat("SYSTEM\\CurrentControlSet\\Services\\OSNHBService","\\Parameters"),true);
		if(pParmeterKey==nullptr)
		{
			printf("�������ü�ȺIP��");
			return ;
		}
		else if(pParmeterKey->GetValue("PartnerName")==nullptr||pParmeterKey->GetValue("PriIPAddress")==nullptr||
			pParmeterKey->GetValue("SecIPAddress")==nullptr)
		{
			printf("�������ü�ȺIP��");
			return;
		}
		if(bNoClusterflag)
		{
			int ret = ::MessageBox(NULL,TEXT("δ��⵽��Ⱥ����ȷ���Ƿ������������������ܾ�����޷���ȷ������"),TEXT("Warning"),MB_OKCANCEL);
			if(ret == IDCANCEL)
				return ;
		}
		else
		{
			if(this->cbResource->SelectedValue->ToString() == "")
			{
				printf("������Դ���ڼ�Ⱥ�е���Դ���ƣ�");
				return;
			}
		}
		this->m_IsCluster=1;
		if(bNoClusterflag)
			this->m_ClusterResourceName = "";
		else
			this->m_ClusterResourceName = this->cbResource->SelectedValue->ToString();
	}*/


	if(this->Type ==VolumeCopy)
	{
		VolumeMirrorClick(pSrcGuid,pDesGuid);
	}
	else
	{
		DiskMirrorClick(pSrcGuid,pDesGuid);
	}
}
//void CNewMirror::ckbCluster_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
//{
//	if(ckbCluster->Checked)
//	{
//		this->cbResource->Enabled =true;
//		if(CheckClusterResourceNameBind())
//		{
//			this->cbResource->DataSource = arraylist;
//		}
//	}
//	else if(!ckbCluster->Checked)
//		this->cbResource->Enabled = false;
//}
//
//bool CNewMirror::CheckClusterResourceNameBind()
//{
//	LPCLUSTERVERSIONINFO lpClusterInfo;
//	HCLUSENUM hclusenum;
//	HCLUSTER hcluster;
//	WCHAR	clusterName[128] ;	
//	WCHAR	nodeName[10][128];
//	WCHAR	*pResourceName;
//	DWORD	chClusterName =128;
//	DWORD	dwtype;
//	DWORD	enumCount;
//	DWORD	chNodeNameLen;
//	WCHAR *szbuffer;
//	DWORD ret = 0;
//
//	memset(clusterName,0,128);
//
//	for(int i = 0;i<10;i++)
//	{
//		memset(nodeName[i],0,128);
//	}
//	DWORD nodeState = ClusterNodeStateUnknown;
//
//	lpClusterInfo = (LPCLUSTERVERSIONINFO)malloc(sizeof(CLUSTERVERSIONINFO));
//	try
//	{
//		hcluster = OpenCluster(NULL);//name is NULL ,through LPC get the local cluster handle
//		if(hcluster == NULL)
//		{
//			free(lpClusterInfo);
//			bNoClusterflag = true;
//			return  false;
//		}
//		else
//		{
//			bNoClusterflag = false;
//		}
//
//		ret = GetClusterInformation(hcluster,clusterName,&chClusterName,lpClusterInfo);
//		if(ret !=ERROR_SUCCESS)
//		{
//			free(lpClusterInfo);
//			return false;
//		}
//
//		hclusenum = ClusterOpenEnum(hcluster,CLUSTER_ENUM_RESOURCE);
//		if(!hclusenum)
//		{
//			free(lpClusterInfo);
//			CloseCluster(hcluster);
//			return false;
//		}
//
//		dwtype = CLUSTER_ENUM_RESOURCE;
//		pResourceName = (LPWSTR)malloc(256);
//		szbuffer = (WCHAR *)malloc(256);
//		enumCount = ClusterGetEnumCount(hclusenum);
//		for(DWORD i= 0 ;i<enumCount ;++i)
//		{
//			chNodeNameLen = 256;
//			memset(szbuffer,0,sizeof(szbuffer));
//
//			ret = ClusterEnum(hclusenum,i,&dwtype,pResourceName,&chNodeNameLen);//get enum something
//			if(ret == ERROR_SUCCESS)
//			{
//				setlocale(LC_CTYPE, "chs");
//				String ^dd = Marshal::PtrToStringUni((IntPtr)pResourceName);
//				arraylist->Add(dd);
//				setlocale(LC_CTYPE, "C");
//			}
//
//		}
//	}catch(...)
//	{
//
//	}		
//	free(szbuffer);
//	free(pResourceName);
//
//	free(lpClusterInfo);
//	//CloseClusterNode(hnode);
//	ClusterCloseEnum(hclusenum);
//	CloseCluster(hcluster);
//	return true;
//}