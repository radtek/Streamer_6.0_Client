#include "eventlog.h"
namespace HostmirrorScheduleService
{
	using namespace System;
	using namespace System::Management;
	using namespace System::Text;
	using namespace System::Threading;
	using namespace System::Security::Cryptography;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;
	using namespace Microsoft::Win32;

	enum DATABASE_TYPE
    {
        ORACLE,
        SQLSERVER,
        SYBASE,
        DOMINO,
        EXCHANGE,
        MYSQL,
        INFORMIX
    };


public ref class OSNAccount
    {
	public: String^ Admin;
	public: String^ Pass;
	public: OSNAccount()
        { }
	public:  OSNAccount(String^ name, String^ pass)
        {
            Admin = name;
            Pass = pass;
        }
    };
public ref class DatabaseInformation
    {
	public: bool IsHave;//是否有该数据库
	public: DATABASE_TYPE Type;//什么数据库
	public: String^ volumeLabel;//数据保存卷
	public: OSNAccount^ AdminAccount;//管理员帐号
	public: bool IsForceCDP;//是否强制创建CDP而不管是否刷新成功与否
	public: String^ Instance;//数据库实例名或者是服务器名
	public: DatabaseInformation()
        { }
	public: DatabaseInformation(DATABASE_TYPE dbtype, bool Have, String^ label, OSNAccount^ account, bool forceCDP)
        {
            IsHave = Have;
            Type = dbtype;
            volumeLabel = label;
            AdminAccount = account;
            IsForceCDP = forceCDP;
			Instance=nullptr;
        }
    };

#define m_strVSSParaRegPath   L"System\\CurrentControlSet\\Services\\OsnVSS\\Parameters"
UInt32 GetDiskIndexByDriveLabel(String ^Name)
	 {
		 try
            {
               
           
                    String ^WQL = "Associators   of   {win32_LogicalDisk='%s'}   where   resultClass   =   Win32_DiskPartition";

                    String ^NewLine = WQL->Replace("%s", Name);

                    // obtain the disk 
                    ManagementObjectSearcher ^DiskPartitionSearcher =
                      gcnew ManagementObjectSearcher("root\\CIMV2",
                       NewLine);

                     ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();
        
					 System::Collections::IEnumerator	^objEnum= DiskCollection->GetEnumerator();
					 while(objEnum->MoveNext())
					 {
 
                         ManagementObject ^queryObj=static_cast<ManagementObject ^>(objEnum->Current);                       
						 UInt32 Index = Convert::ToUInt32(queryObj->GetPropertyValue("DiskIndex"));
						 {
							 return Index;
						 }


                    }
                

			   return -1;


            }
            catch (...)
            {
                return -1;
            }
	 }

String^ GetCurrentOSVersion()
		 {
			 String^ strOSVersion = nullptr;
			 try
			 { 
				 ManagementObjectSearcher ^OSVersionSearcher = gcnew 
				                           ManagementObjectSearcher("Select * from Win32_OperatingSystem");
				 ManagementObjectCollection::ManagementObjectEnumerator ^myOSData = OSVersionSearcher->Get()->GetEnumerator();
				 myOSData->MoveNext();
				 ManagementBaseObject ^mboOS = myOSData->Current;
			 
				strOSVersion = mboOS->GetPropertyValue("Caption")->ToString(); 
			 }
			 catch(Exception ^ex)
			 {
				 CEventLog::OSNWriteEventLog(ex->ToString(),EventLogEntryType::Error,023);
			 }
			 return strOSVersion;
		 }
 array<Byte>^ GetLegalKey()
        {
			System::Security::Cryptography::SymmetricAlgorithm^ mobj = gcnew RijndaelManaged();
            String^ key = "xininfocore";
            //string iv = "heinfocored";
            array<Byte>^ legalKey;
            //byte[] legalIV;
            mobj->GenerateKey();
            array<Byte>^ temp = mobj->Key;
            int KeyLength = temp->Length;
            if (key->Length > KeyLength)
            {
                key = key->Substring(0, KeyLength);
            }
            else if (key->Length < KeyLength)
            {
                key = key->PadRight(KeyLength, ' ');
            }
			Encoding^ as=ASCIIEncoding::ASCII;
			legalKey = as->GetBytes(key);
            return legalKey;
        }
 array<Byte>^ GetLegalIV()
        {
			System::Security::Cryptography::SymmetricAlgorithm^ mobj = gcnew RijndaelManaged();
            //string key = "xininfocore";
            String^ iv = "heinfocored";
            //byte[] legalKey;
            array<Byte>^ legalIV;
            mobj->GenerateKey();
            array<Byte>^ temp = mobj->IV;
            int IVLength = temp->Length;
            if (iv->Length > IVLength)
            {
                iv = iv->Substring(0, IVLength);
            }
            else if (iv->Length < IVLength)
            {
                iv = iv->PadRight(IVLength, ' ');
            }
			Encoding^ as=ASCIIEncoding::ASCII;
			legalIV = as->GetBytes(iv);
            return legalIV;
        }
 String^ OSNDecry(String^ msg)
        {
            try
            {
				System::Security::Cryptography::SymmetricAlgorithm^ mobj = gcnew RijndaelManaged();
                array<Byte>^ legalKey = GetLegalKey();
                array<Byte>^ legalIV = GetLegalIV();
				array<unsigned char>^ inmsg = Convert::FromBase64String(msg);
                MemoryStream^ ms = gcnew MemoryStream();
                mobj->Key = legalKey;
                mobj->IV = legalIV;
                ICryptoTransform^ decry = mobj->CreateDecryptor();
				CryptoStream^ cs = gcnew CryptoStream(ms, decry, CryptoStreamMode::Write);
                cs->Write(inmsg, 0, inmsg->Length);
                cs->FlushFinalBlock();
                ms->Close();
                array<unsigned char>^ outmsg = ms->ToArray();
                cs->Close();
				Encoding^ u8 = Encoding::UTF8;
                String^ ret=u8->GetString(outmsg);
				return ret;
            }
            catch (Exception^ ex)
            {
				CEventLog::OSNWriteEventLog(String::Concat("解密时出现异常;",ex->Message->ToString()),EventLogEntryType::Error,0);
				//MessageBox::Show("解密时出现异常");
            }
            return nullptr;
        }
 int CheckFileSystem(String ^LabelName)
{
	  
	if(LabelName == nullptr)
		return 0;

	  wchar_t VolumeNameBuffer[32];
	  DWORD lpVolumeSerialNumber;
	  DWORD lpMaximumComponentLength;
	  DWORD lpFileSystemFlags;
	  wchar_t  lpFileSystemNameBuffer[32];

	  BOOL ReturnValue = GetVolumeInformation((wchar_t *)Marshal::StringToHGlobalAnsi(String::Concat(LabelName,"\\")).ToPointer(),
		                                     VolumeNameBuffer,
											 32,
											 &lpVolumeSerialNumber,
											 &lpMaximumComponentLength,
											 &lpFileSystemFlags,
											 lpFileSystemNameBuffer,
											 32);
	if(ReturnValue == 0)
		return 0;

    if(wcscmp(L"NTFS",lpFileSystemNameBuffer)!=0)
		return 1;
	else
		return 2;

}

 /*argstr  Volumehostmirror,label,nothing*/
		 /*argstr Diskhostmirror,index,volumelabellist*/
 bool CreateCDP(String^ argstr)
		 {
			 String^ filepath=System::AppDomain::CurrentDomain->BaseDirectory;
			 Process^ proc=gcnew Process();
			 try
			 {
				 filepath = String::Concat(filepath,"\\OSNDBProxy.exe");
				 proc->StartInfo->FileName=filepath;
				 proc->StartInfo->UseShellExecute=false;
				 proc->StartInfo->RedirectStandardInput=true;
				 proc->StartInfo->RedirectStandardOutput=true;
				 proc->StartInfo->RedirectStandardError=true;
				 proc->StartInfo->CreateNoWindow=true;
				 proc->StartInfo->Arguments=argstr;

				 proc->Start();
				 String^ msg=proc->StandardOutput->ReadToEnd();
				 proc->WaitForExit();
				 if(proc->ExitCode == 0)
				 {
					 CEventLog::OSNWriteEventLog(String::Concat("Create CDP Success\n",msg),EventLogEntryType::Information,0);
				 }
				 else
				 {
					 CEventLog::OSNWriteEventLog(String::Concat("Create CDP Failed \n",msg),EventLogEntryType::Information,0);
				 }
				 return true;
			 }
			 catch(Exception^ ee)
			 {
				 CEventLog::OSNWriteEventLog(ee->Message->ToString(),EventLogEntryType::Error,0);
				 return false;
			 }
			 finally
			 {
				 if(proc)
					 proc->Close();
			 }
			 return false;
		 }
void MakeDiskCDP(int index)
	   {
		   try
		   {
			   String^ para="diskhostmirror";
			   para=String::Concat(para,",",Convert::ToString(index));
			   CreateCDP(para);
		   }
		   catch(Exception ^err)
			{
				String ^str = "创建CDP 失败,错误信息";
				str =String::Concat(str,err);
				CEventLog::OSNWriteEventLog(str,EventLogEntryType::Error, 0);
			}
	   }
void MakeVolumeCDP(String^ label)
	   {
		   try
		   {
			   if(!label->EndsWith(":"))
			   {
				   label=String::Concat(label,":");
			   }
			   String^ para="volumehostmirror";
			   para=String::Concat(para,",",label);
			   CreateCDP(para);
		   }
		   catch(Exception^ ex)
		   {
			   CEventLog::OSNWriteEventLog(String::Concat("MakeVolumeCDP exception=",ex->Message->ToString()),EventLogEntryType::Error, 034);
		   }
	   }

	    
		
	     
		 
		 
		 
		 
		
		 
		
}