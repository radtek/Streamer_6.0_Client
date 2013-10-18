#include <locale.h>
#include "MirrorUIConfigure.h"
#include "Globallist.h"
#include <vector>
#include <Wbemidl.h>
#include "COMDEF.H"
//#include "MyClusApi.h"

class CNewMirror
{
public:  
	CVolumeInfo           *pSourceVolume;
	CVolumeInfo           *pTargetVolume;
	CVolumeInfoList       *pSystemVolumeList;
	CDiskInfo             *pSourceDisk;
	CDiskInfo             *pTargetDisk;
	CDiskInfoList         *pSystemDiskList;
	INSTALLTYPE           Type;
	unsigned int          m_EIMMode;
	unsigned int          m_IsCluster;
	wstring               *m_ClusterResourceName;
	std::vector<DWORD>    *arraylist;
	BOOLEAN               bNoClusterflag;

public:
	CNewMirror(CVolumeInfo *pVolumeInfo,CVolumeInfoList *pVolumeInfoList,INSTALLTYPE Ty,
		CDiskInfoList *pDiskInfoList)
	{
		pSourceVolume = pVolumeInfo;
		pTargetVolume = NULL;
		pSystemVolumeList = pVolumeInfoList;
		this->pSystemDiskList = pDiskInfoList;

		Type = Ty;
		bNoClusterflag = true;
		this->m_EIMMode = 2;  //target;
		this->m_IsCluster=0;//not Cluster;
		m_ClusterResourceName = new wstring(L"NONE");//not Cluster Resource
		//RefreshListView();
		arraylist = new vector<DWORD> (0);
	}

	CNewMirror(CDiskInfo *pDiskInfo,CDiskInfoList *pDiskInfoList,CVolumeInfoList *pVolumeInfoList)
	{
		pSourceDisk = pDiskInfo;
		pTargetDisk = NULL;
		this->pSystemDiskList = pDiskInfoList;
		pSystemVolumeList = pVolumeInfoList;
		bNoClusterflag = true;

		Type = DiskCopy;
		this->m_EIMMode = 2;  //target;
		this->m_IsCluster=0;//not Cluster;
		m_ClusterResourceName = new wstring(L"NONE");//not Cluster Resource

		//RefreshListView();
		arraylist = new vector<DWORD> (0);
	}

	~CNewMirror()
	{
		arraylist->clear();
	}

	DWORD CheckVolIsBootableOrSys(wstring *label);
	DWORD CheckFileSystem(wstring *LabelName);
	DWORD CheckDiskIsEIMDisk(int index);
	DWORD InitalizeDisk(int index);
	DWORD CheckVolIsEIMVol(wstring *LabelName);
	DWORD CharToWchar(const char *pChar,wchar_t *pWchar,int Length);
	DWORD WcharToChar(const wchar_t *pWchar,char *pChar,int Length);
	DWORD OSNInitWMI(IWbemServices *m_pSvc,IWbemLocator *m_pLoc);
	DWORD OSNCloseWMI(IWbemServices *m_pSvc,IWbemLocator *m_pLoc,IEnumWbemClassObject *pEnumerator);

	void  VolumeMirrorClick(wstring *pSrcGuid,wstring *pDesGuid);
	void  DiskMirrorClick(wstring *pSrcGuid,wstring *pDesGuid);

	void  MirrorOKClick(wstring *pSrcGuid,wstring *pDesGuid);
	/*void  ckbCluster_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	bool  CheckClusterResourceNameBind();*/
};