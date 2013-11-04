#ifndef _CLOBALLIST_H
#define _CLOBALLIST_H

#include "MirrorUIConfigure.h"
#include "Windows.h"
#include <vector>
using namespace std;

class CGlobalList
{
public: 
	const int m_Length;
	std::vector<DWORD>      *m_pArrayList;

public:
	CGlobalList(unsigned int Length):m_Length(Length)
	{
		m_pArrayList = new vector<DWORD> (0);
	}

	int GetLength()
	{
		return m_pArrayList->size();
	}

	DWORD GetItem(int index)
	{
		return m_pArrayList->at(index);
	}

	void AddItem(DWORD pObject)
	{
		std::vector<DWORD>::iterator it = m_pArrayList->end();
		m_pArrayList->insert(it,pObject);
	}

	void RemoveItem(DWORD pObject)
	{
		for(vector<DWORD>::iterator temp=m_pArrayList->begin(); temp!=m_pArrayList->end(); )
		{
			if(* temp == pObject)
			{
				m_pArrayList->erase(temp); //²»ÄÜÐ´³Éarr.erase(it);
				return ;
			}
			else
			{
				++temp;
			}
		}
		return;
	}

	void Clear()
	{
		m_pArrayList->clear();
	}
};


class CDiskInfoList :public CGlobalList
{
public:
	CDiskInfoList(unsigned int length):CGlobalList(length){}

	~CDiskInfoList()
	{
		CDiskInfo *pDiskInfo = NULL;
		for(DWORD i=0;i<m_pArrayList->size();i++)
		{
			pDiskInfo = (CDiskInfo *)(m_pArrayList->at(i));
			delete(pDiskInfo);
		}
		Clear();
	}

	CDiskInfo *GetDiskInfo(wstring *guid)
	{
		CDiskInfo *pDiskInfo = NULL;
		for(DWORD i=0;i<m_pArrayList->size();i++)
		{
			pDiskInfo = (CDiskInfo *)(m_pArrayList->at(i));
			if(pDiskInfo->m_Guid->compare(guid->c_str()) == 0)
				return pDiskInfo;
		}
		return NULL;
	}

	 CDiskInfo * GetDiskInfoByString(wstring *guid)
		{
			CDiskInfo *pDiskInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{
				pDiskInfo = (CDiskInfo *)(m_pArrayList->at(i));
				if(pDiskInfo->m_Guid->compare(guid->c_str()) == 0)
					return pDiskInfo;
			}
			return NULL;
		}

	CDiskInfo *GetDiskByIndex(unsigned int index)
	{
		CDiskInfo *pDiskInfo = NULL;
		for(DWORD i=0;i<m_pArrayList->size();i++)
		{
			pDiskInfo = (CDiskInfo *)(m_pArrayList->at(i));
			if(pDiskInfo->m_DiskIndex == index)
				return pDiskInfo;
		}
		return NULL;
	}

	bool HideVolume(wstring *guid)
	{
		CDiskInfo *pDiskInfo = NULL;
		for(DWORD i=0;i<m_pArrayList->size();i++)
		{
			pDiskInfo = (CDiskInfo *)(m_pArrayList->at(i));
			if(pDiskInfo == NULL)
				return false;

			if((pDiskInfo->m_Guid->compare(guid->c_str()) == 0)
				&&pDiskInfo->m_Role == Mirror_Target)
			{
				return true;
			}
		}
		return false;
	}
};

class CVolumeInfoList: public CGlobalList
{
public: CVolumeInfoList(unsigned int length):CGlobalList(length){}

public: ~CVolumeInfoList()
		{
			CVolumeInfo *pVolumeInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{
				pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				delete(pVolumeInfo);
			}
			Clear();
		}

public: CVolumeInfo * GetVolumeInfo(wstring *guid)
		{
			CVolumeInfo *pVolumeInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{
				pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				if(NULL == pVolumeInfo)
				{
					return NULL;
				}
				if(pVolumeInfo->m_GUID->compare(guid->c_str()) == 0)
					return pVolumeInfo;
			}
			return NULL;
		}

public: CVolumeInfo * GetVolumeInfoByLabel(wstring *LabelName)
		{
			CVolumeInfo *pVolumeInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{
				pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				if(pVolumeInfo->m_VolumeLable->compare(LabelName->c_str()) == 0)
					return pVolumeInfo;
			}
			return NULL;
		}

public: CVolumeInfo * GetVolumeInfoByString(wstring *guid)
		{
			CVolumeInfo *pVolumeInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{
				pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				if(pVolumeInfo->m_GUID->compare(guid->c_str()) == 0)
					return pVolumeInfo;
			}
			return NULL;
		}

public: bool CheckVolumeMirrored(wstring *guid)
		{

			CVolumeInfo *pVolumeInfo = NULL;
			for(DWORD i=0;i<m_pArrayList->size();i++)
			{

				pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				if((pVolumeInfo->m_GUID->compare(guid->c_str()) == 0) &&
					pVolumeInfo->m_Role!=Free)
				{
					return true;
				}
			}
			return false;
		}

public : unsigned __int64 GetBlocksBySignature(wstring *guid)
		 {
			 unsigned __int64 TotalSize = 0;

			 CVolumeInfo *pVolumeInfo = NULL;

			 for(DWORD i=0;i<m_pArrayList->size();i++)
			 {
				 pVolumeInfo = (CVolumeInfo *)(m_pArrayList->at(i));
				 if(pVolumeInfo->m_DiskGUID->compare(guid->c_str()) == 0)
				 {
					 TotalSize= TotalSize+pVolumeInfo->m_VolumeSize;
				 }
			 }
			 return TotalSize;
		 }
};

class CMirrorInfoList : public CGlobalList
{
public:CMirrorInfoList(unsigned int length):CGlobalList(length){}

public:~CMirrorInfoList()
	   {
		   CMirrorInfo *pMirrorInfo = NULL;
		   for(DWORD i=0;i<m_pArrayList->size();i++)
		   {
			   pMirrorInfo = (CMirrorInfo *)(m_pArrayList->at(i));
			   delete(pMirrorInfo);
		   }
		   Clear();
	   }

public:CMirrorInfo* GetMirrorInfo(wstring* Sguid,bool Flag) 
	   {
		   CMirrorInfo *pMirrorInfo = NULL;
		   for(DWORD i=0;i<m_pArrayList->size();i++)
		   {
			   pMirrorInfo = (CMirrorInfo *)(m_pArrayList->at(i));
			   if(Flag)
			   {
				   if(pMirrorInfo->m_SourceGuid->compare(Sguid->c_str()) == 0)
					   return pMirrorInfo;
			   }
			   else
			   {
				   if(pMirrorInfo->m_TargetGuid->compare(Sguid->c_str()) == 0)
					   return pMirrorInfo;
			   }
		   }
		   return NULL;
	   }

public:CMirrorInfo *GetDiskMirrorInfo(wstring *guid,bool Flag)
	   {
		   CMirrorInfo *pMirrorInfo = NULL;
		   for(DWORD i=0;i<m_pArrayList->size();i++)
		   {
			   pMirrorInfo = (CMirrorInfo *)(m_pArrayList->at(i));
			   if(Flag)
			   {
				   if(pMirrorInfo->m_SourceGuid->compare(guid->c_str()) == 0)
					   return pMirrorInfo;
			   }
			   else
			   {
				   if(pMirrorInfo->m_TargetGuid->compare(guid->c_str()) == 0)
					   return pMirrorInfo;
			   }
		   }
		   return NULL;
	   }

public:CMirrorInfo* GetMirrorInfo(wstring *Sourceguid,wstring *Targetguid)
	   {
		   CMirrorInfo *pMirrorInfo = NULL;
		   for(DWORD i=0;i<m_pArrayList->size();i++)
		   {
			   pMirrorInfo = (CMirrorInfo *)(m_pArrayList->at(i));

			   if((pMirrorInfo->m_SourceGuid->compare(Sourceguid->c_str()) == 0)
				   &&(pMirrorInfo->m_TargetGuid->compare(Targetguid->c_str()) == 0))
				   return pMirrorInfo;
		   }
		   return NULL;
	   }
};

#endif