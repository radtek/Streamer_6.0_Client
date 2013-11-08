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
	wstring         *ImagePath;  //delete
	string          *ImagePathString;//delete

	char            *m_ClientID;
	char            *m_ServerIP;
	char            *m_ServerID;
	char            *m_TargetIqn;
	char            *m_TargetIPs;
	char            *m_InitiatorIPs;
	bool            m_IsProtected;

	COSNxml       *m_pTempXML;
	INSTALLTYPE   m_InstallType;

public:
	COsnMirrorCopyXML();
	~COsnMirrorCopyXML();

	void COsnMirrorCopyXML::InitializeMembers();
	void COsnMirrorCopyXML::GetSystemVolumesInfo();
	void COsnMirrorCopyXML::GetSystemDisksInfo();
	void COsnMirrorCopyXML::RefreshVolumeListXML();
	void COsnMirrorCopyXML::RefreshChannelListXML();
	void COsnMirrorCopyXML::QueryFCChannel();
	void COsnMirrorCopyXML::RefreshDiskListXML();
	void COsnMirrorCopyXML::RefreshClientXML();
	void COsnMirrorCopyXML::GetVolumeCopyMirrorInfo();
	void COsnMirrorCopyXML::GetDiskCopyMirrorInfo();
	void COsnMirrorCopyXML::MoveNext(char *pSou,char *pDes,char sign);
	void COsnMirrorCopyXML::ReadConfigurationFile();
	void COsnMirrorCopyXML::WriteConfigurationFile();
	void COsnMirrorCopyXML::GetSystemMirrorInfo();

	DWORD COsnMirrorCopyXML::QueryPortNameByIP(char *pInitiatorIP,char *pPortName,char **ppHBAName);
	DWORD COsnMirrorCopyXML::ConnectiSCSIChannel(char *pTargetIP,char *pInitiatorIP,char *pIqn);
	DWORD COsnMirrorCopyXML::GetSessionIDByIqn(ULONGLONG *pSessionID,ULONGLONG *pAdapterID,char *pIqn);
	DWORD COsnMirrorCopyXML::DisConnectiSCSIChannel(char *pIPAddress,char *pIqn);
	DWORD COsnMirrorCopyXML::QueryiSCSIChannel();
	DWORD COsnMirrorCopyXML::GetIsClusterByGUID(wstring *srcguid,wstring *dstguid);
	DWORD COsnMirrorCopyXML::GetEimModebyGUID(wstring *srcguid,wstring *dstguid);
	DWORD COsnMirrorCopyXML::QueryRegKey(char *pKeyName,char *pValueName,void *pValue,RegKey sign);
	DWORD COsnMirrorCopyXML::SetRegKey(char *pKeyName,char *pValueName,void *pValue,RegKey sign);
	DWORD COsnMirrorCopyXML::QueryClientID();
	DWORD COsnMirrorCopyXML::CreateClientID();
	DWORD COsnMirrorCopyXML::OSNInitWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,wchar_t *pResName);
	DWORD COsnMirrorCopyXML::OSNCloseWMI(IWbemServices **m_pSvc,IWbemLocator **m_pLoc,IEnumWbemClassObject **pEnumerator);
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
	DWORD COsnMirrorCopyXML::CheckVolIsBootable(wstring *label);

	bool  COsnMirrorCopyXML::DeleteVolumeMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType);
	bool  COsnMirrorCopyXML::DeleteDiskMirror(wstring *strSrcguid,wstring *strDesguid,bool MirrorType);
	void  COsnMirrorCopyXML::InitializeVolumeMirror(wstring *pSrcGuid,wstring *pDesGuid,INIT_TYPE type);
	void  COsnMirrorCopyXML::InitializeDiskMirror(wstring *pSrcGuid,wstring *pDesGuid);

	wstring* COsnMirrorCopyXML::GetClusterResourceName(wstring *srcguid,wstring *dstguid);
};

class CWmi
{
public :
	CWmi(WCHAR *wmiClass)  
    {  
        m_pclsObj  = NULL;  
        m_pLoc     = NULL;  
        m_pSvc     = NULL;  
        m_pclsEnum = NULL;  
		memset(m_WmiClass,0,sizeof(m_WmiClass));
		memcpy(m_WmiClass,wmiClass,wcslen(wmiClass)*sizeof(WCHAR));
        //CoInitializeEx(0, COINIT_MULTITHREADED);  
    }  

	bool Init(wchar_t *lpszWMIObj,  wchar_t *lpszWhere=NULL);

	void Release();

	HRESULT GetMethod(LPCTSTR lpszMethodName, IWbemClassObject **ppIn);

	//SAFEARRAY数组转bstr_t  
    static bstr_t SafeArrayToString(SAFEARRAY *psa);

	static UCHAR *SafeArrayToUInt8(SAFEARRAY *psa,int * charSize);

	 //获取索引指定对象的的属性 
	bool    GetObjectProp(int iObj, LPCTSTR lpszPropName ,_variant_t *vtProp ,VARTYPE varType);  

	//返回查询到得总数  
    inline int GetObjCount()  
    {  
        return m_clslist.size();  
    }  
  
    ~CWmi()  
    {  
        Release();  
       // CoUninitialize();  
    }  
private:
public:
	WCHAR         m_WmiClass[128];
	IWbemLocator  *m_pLoc;  
	IWbemServices *m_pSvc;  

	IEnumWbemClassObject  *m_pclsEnum;  
	IWbemClassObject      *m_pclsObj;  

	vector<IWbemClassObject *> m_clslist;  
};

#endif