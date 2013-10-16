#ifndef _OSNMIRRORCOPYXML_H
#define _OSNMIRRORCOPYXML_H

#include <stdio.h>
#include <locale.h>
#include "Globallist.h"
#include <Wbemidl.h>
#include "COMDEF.H"
#include "OSNXml.h"
#include "..\OsnVolumeCopyApi\Configure.h"

# pragma comment(lib, "wbemuuid.lib")

class COsnMirrorCopyXML
{
public:  
	CMirrorInfoList *pVolumeMirrorList;
	CMirrorInfoList *pDiskMirrorList;
	CVolumeInfoList *pVolumeList;
	CDiskInfoList   *pDiskList;
	wstring         *ImagePath;

	IWbemServices *m_pSvc;
	IWbemLocator  *m_pLoc;
	COSNxml       *m_pTempXML;
	INSTALLTYPE   m_InstallType;

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
		return ;
	}

	void COsnMirrorCopyXML::InitializeMembers();
	void COsnMirrorCopyXML::GetSystemVolumesInfo();
	void COsnMirrorCopyXML::GetSystemDisksInfo();
	void COsnMirrorCopyXML::RefreshVolumeListXML();
	void COsnMirrorCopyXML::RefreshDiskListXML();
	void COsnMirrorCopyXML::RefreshClientXML();
	void COsnMirrorCopyXML::WriteConfigurationFile();

	DWORD COsnMirrorCopyXML::OSNInitWMI();
	DWORD COsnMirrorCopyXML::OSNCloseWMI();
	DWORD COsnMirrorCopyXML::CharToWchar(const char *pChar,wchar_t *pWchar,int Length);
	DWORD COsnMirrorCopyXML::WcharToChar(const wchar_t *pWchar,char *pChar,int Length);
	DWORD COsnMirrorCopyXML::NewMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType);
	DWORD COsnMirrorCopyXML::RemoveMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType);
	DWORD COsnMirrorCopyXML::InitMirror(wstring *pSrcGuid,wstring *pDesGuid,bool MirrorType);
	DWORD COsnMirrorCopyXML::NewVolumeMirror(wstring *pSrcGuid,wstring *pDesGuid);
	DWORD COsnMirrorCopyXML::NewDiskMirror(wstring *pSrcGuid,wstring *pDesGuid);
	DWORD COsnMirrorCopyXML::SetRegistryKey(char *ServiceKeyName,char *SourceString,char *TargetString,bool Flag,int State,int IsCluster);
	DWORD COsnMirrorCopyXML::ReadPreviousState(wstring *Key,bool Flag);
	DWORD COsnMirrorCopyXML::CheckFileSystem(wstring *LabelName);
	
	bool  COsnMirrorCopyXML::CheckVolIsBootable(wstring *label);
	bool  COsnMirrorCopyXML::DeleteVolumeMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType);
	bool  COsnMirrorCopyXML::DeleteDiskMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType);
	void  COsnMirrorCopyXML::InitializeVolumeMirror(wstring *pSrcGuid,wstring *pDesGuid,INIT_TYPE type);
	void  COsnMirrorCopyXML::InitializeDiskMirror(wstring *pSrcGuid,wstring *pDesGuid);
};

#endif