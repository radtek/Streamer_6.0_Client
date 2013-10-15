#ifndef _CLOBALLIST_H
#define _CLOBALLIST_H

namespace VolumeCopyUI {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Microsoft::Win32;
	using namespace System::Net;
    using namespace System::Runtime::InteropServices;


    public ref class CGlobalList
	{
	   public: const int m_Length;
	   public: ArrayList ^m_pArrayList;

	public:CGlobalList(unsigned int Length):m_Length(Length)
			  {
				  m_pArrayList = gcnew ArrayList(m_Length);
			  }
	   public: int GetLength()
			   {
				   return m_pArrayList->Count;
			   }

	   public: Object ^GetItem(int index)
			   {
				   return m_pArrayList[index];
			   }

		public:void AddItem(Object ^pObject)
			   {
				   m_pArrayList->Add(pObject);
			   }

		public:void RemoveItem(Object ^pObject)
			   {
				   m_pArrayList->Remove(pObject);
			   }
		public:void Clear()
			   {
				   m_pArrayList->Clear();
			   }
	};

    
	public ref class CDiskInfoList :public CGlobalList
	{
		public:CDiskInfoList(unsigned int length):CGlobalList(length){}
		public:CDiskInfo ^GetDiskInfo(String^ guid)
		   {
			   CDiskInfo ^pDiskInfo = nullptr;
			   for(int i=0;i<m_pArrayList->Count;i++)
			   {
				   pDiskInfo = static_cast<CDiskInfo ^>(m_pArrayList[i]);
				   if(pDiskInfo->m_Guid == guid)
					   return pDiskInfo;

			   }

			   return nullptr;
		   }
	     public:CDiskInfo ^GetDiskByIndex(unsigned int index)
				{
					CDiskInfo ^pDiskInfo = nullptr;
				   for(int i=0;i<m_pArrayList->Count;i++)
				   {
					   pDiskInfo = static_cast<CDiskInfo ^>(m_pArrayList[i]);
					   if(pDiskInfo->m_DiskIndex== index)
						   return pDiskInfo;

				   }

				   return nullptr;

				}

		public: bool HideDriver(String^ guid)
				{
					
					CDiskInfo ^pDiskInfo = nullptr;
					for(int i=0;i<m_pArrayList->Count;i++)
					{
					   pDiskInfo = static_cast<CDiskInfo ^>(m_pArrayList[i]);
					   if(pDiskInfo->m_Guid== guid
						   &&pDiskInfo->m_Role == Mirror_Target)
					   {
						   return true;
					   }

					}

				   return false;

				}
	};

	public ref class CDriverInfoList: public CGlobalList
	{
	    public: CDriverInfoList(unsigned int length):CGlobalList(length){}
	    public: CDriverInfo ^ GetDriverInfo(String^ guid)
				{
					CDriverInfo ^pDriverInfo = nullptr;
					for(int i=0;i<m_pArrayList->Count;i++)
					{
						pDriverInfo = static_cast<CDriverInfo ^>(m_pArrayList[i]);
						if(pDriverInfo->m_GUID==guid)
							return pDriverInfo;

					}

					return nullptr;
				}
		 public: CDriverInfo ^ GetDriverInfoByLabel(String ^LabelName)
				{
					CDriverInfo ^pDriverInfo = nullptr;
					for(int i=0;i<m_pArrayList->Count;i++)
					{
						pDriverInfo = static_cast<CDriverInfo ^>(m_pArrayList[i]);
						if(pDriverInfo->m_VolumeLable->Equals(LabelName))
							return pDriverInfo;

					}

					return nullptr;
				}
		  public: CDriverInfo ^ GetDriverInfoByString(String ^guid)
				{
					CDriverInfo ^pDriverInfo = nullptr;
					for(int i=0;i<m_pArrayList->Count;i++)
					{
						pDriverInfo = static_cast<CDriverInfo ^>(m_pArrayList[i]);
						//String ^Temp = pDriverInfo->m_DiskSignature.ToString("X8");
						//Temp = String::Concat(Temp,"_");
						//Temp = String::Concat(Temp,pDriverInfo->m_StartOffset.ToString("X8"));
						if(pDriverInfo->m_GUID->Equals(guid))
							return pDriverInfo;

					}

					return nullptr;
				}

		  public: bool CheckDriverMirrored(String^ guid)
				  {
					  
					  CDriverInfo ^pDriverInfo = nullptr;
					  for(int i=0;i<m_pArrayList->Count;i++)
					  {
						
						  pDriverInfo = static_cast<CDriverInfo ^>(m_pArrayList[i]);
						  if(pDriverInfo->m_GUID == guid &&
							  pDriverInfo->m_Role!=Free)
						  {
							  return true;
						  }
						 
					  }
					return false;


				  }
	public : unsigned __int64 GetBlocksBySignature(String^ guid)
			 {
				 unsigned __int64 TotalSize = 0;

				 CDriverInfo ^pDriverInfo = nullptr;

				 for(int i=0;i<m_pArrayList->Count;i++)
				 {
					pDriverInfo = static_cast<CDriverInfo ^>(m_pArrayList[i]);
					if(pDriverInfo->m_GUID == guid)
					{
						TotalSize= TotalSize+pDriverInfo->m_DriverSize;

					}

				 }

				 return TotalSize;

			 }

	};

	public ref class CMirrorInfoList : public CGlobalList
	{
 	   public:CMirrorInfoList(unsigned int length):CGlobalList(length){}
	   public:CMirrorInfo^ GetMirrorInfo(String^ Sguid,bool Flag) 
			  {
				  CMirrorInfo ^pMirrorInfo = nullptr;
				  for(int i=0;i<m_pArrayList->Count;i++)
				  {
					  pMirrorInfo = static_cast<CMirrorInfo ^>(m_pArrayList[i]);
					  if(Flag)
					  {
						  if(pMirrorInfo->m_SourceGuid==Sguid)
						     return pMirrorInfo;
					  }
					  else
					  {
						  if(pMirrorInfo->m_TargetGuid==Sguid)
						      return pMirrorInfo;

					  }
				  }

				  return nullptr;
			  }

	public:CMirrorInfo ^GetDiskMirrorInfo(String^ guid,bool Flag)
		   {
			      CMirrorInfo ^pMirrorInfo = nullptr;
				  for(int i=0;i<m_pArrayList->Count;i++)
				  {
					  pMirrorInfo = static_cast<CMirrorInfo ^>(m_pArrayList[i]);
					  if(Flag)
					  {
						  if(pMirrorInfo->m_SourceGuid==guid)
						     return pMirrorInfo;
					  }
					  else
					  {
						  if(pMirrorInfo->m_TargetGuid==guid)
						      return pMirrorInfo;

					  }
				  }

				  return nullptr;
		   }

	    public:CMirrorInfo^ GetMirrorInfo(String ^Sourceguid,String ^Targetguid)
			  {
				  CMirrorInfo ^pMirrorInfo = nullptr;
				  for(int i=0;i<m_pArrayList->Count;i++)
				  {
					  pMirrorInfo = static_cast<CMirrorInfo ^>(m_pArrayList[i]);
					 // String ^Source=pMirrorInfo->m_SourceDiskSignature.ToString("X8");
					 // Source = String::Concat(Source,"_");
					 // Source = String::Concat(Source,pMirrorInfo->m_SourceOffset.ToString("X8"));

					 // String ^Target = pMirrorInfo->m_TargetDiskSignature.ToString("X8");
					 // Target = String::Concat(Target,"_");
					 // Target = String::Concat(Target,pMirrorInfo->m_TargetOffset.ToString("X8"));


					  if(pMirrorInfo->m_SourceGuid->Equals(Sourceguid)&&pMirrorInfo->m_TargetGuid->Equals(Targetguid))
						  return pMirrorInfo;
				  }

				  return nullptr;
			  }


	};


}

#endif