#ifndef _CONFIGURE_H
#define _CONFIGURE_H


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



enum DriverRole
{
	Free,
	Mirror_Source,
	Mirror_Target
	
};

enum MirrorState
{
	MDOWN,
	MDIRTY,
	MRECOVERY,
	MUP,
	MINIT,
	MMISSING
};
enum MirrorType
{
	VolumeMirror,
	DiskMirror
};

enum INSTALLTYPE
{
	VolumeCopy,
	DiskCopy,
	Both
};

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

enum KeyModifiers 
		{ 
			None=0,
			Alt=1,
			Control=2,
			Shift=4,
			Windows=8
        };
public ref class CDiskInfo
{
    public: unsigned int m_DiskIndex;
    //public: unsigned int m_DiskSignature;
    public: String^ m_Guid;
    public: unsigned __int64  m_DiskSize ; // block;
    public: unsigned __int64  m_FreeSpace;
    public: unsigned __int64  m_Role;
    
public:CDiskInfo (unsigned int Index,String^ guid,unsigned __int64 Size,unsigned __int64 FreeSize)// CDiskInfo (unsigned int Index,unsigned int Signature,unsigned __int64 Size,unsigned __int64 FreeSize)
		{
			m_DiskIndex = Index;
			//m_DiskSignature = Signature;
			m_Guid     = guid;
			m_DiskSize = Size;
			m_FreeSpace = FreeSize ;
			m_Role          = Free;
		}



};

public ref class CDriverInfo
	   {
		   public: String  ^m_VolumeLable; 
		   public: unsigned __int64 m_DriverSize; // blocks
	       public: String^ m_GUID;
		  // public: unsigned int m_DiskSignature ;
		  // public: unsigned int m_StartOffset;
	       public: DriverRole   m_Role;  
           
	   public: CDriverInfo(String ^VolumeLable,unsigned __int64 Size,String^ guid)//CDriverInfo(String ^VolumeLable,unsigned int Size,unsigned int Signature,unsigned int Offset)
			   {
				   m_VolumeLable   = VolumeLable;
				   m_DriverSize    = Size ;
				  // m_DiskSignature = Signature;
				  // m_StartOffset   = Offset;
				   m_Role          = Free;
				   m_GUID          = guid;
			   }

	   };


public ref class CMirrorInfo
	   {
	      //public: unsigned int m_SourceDiskSignature;
	      public: String^ m_SourceGuid;
	      //public: unsigned int m_SourceOffset;

	     // public: unsigned int m_TargetDiskSignature;
		  public: String^ m_TargetGuid;
	     // public: unsigned int m_TargetOffset;

	      public: MirrorState  m_State;
	      public: unsigned int m_EIMMode;  //1:EIM Volume is Source, 2:EIM Volume is Target, 3:Both are EIM Volume
		  public : unsigned int m_IsCluster;
	   public : String ^m_ClusterResourceName;

	      public:CMirrorInfo(String^ SGuid,//unsigned int SOffset,unsigned int TOffset,
					         String^ TGuid, unsigned int eimMode,unsigned int isCluster,String ^clusterResourceName)
				 {
					// m_SourceDiskSignature= SSignature;
					 m_SourceGuid  =SGuid;
					// m_SourceOffset= SOffset;
					 m_TargetGuid  =TGuid;
					// m_TargetDiskSignature = TSignature;
					// m_TargetOffset = TOffset;
					 m_EIMMode = eimMode;
					 m_IsCluster = isCluster;
					 m_ClusterResourceName = clusterResourceName;
				 }
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
	public: String^ Instance;
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
public ref class CDPArg
{
public:String^ argstr;
public:unsigned int EIMMode;
public: String^ guid;
//public:unsigned int signature;
//public:unsigned int offset;
private: System::Void toolStripStatusLabel2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}





#endif