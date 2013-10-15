#include <string>
using namespace std;

enum DeviceRole
{
	Free,
	Mirror_Source,
	Mirror_Target
};

enum FileSys
{
	NTFS,
	FAT32,
	EXT3,
	EXT4,
	FUnknown
};

enum VolumeState
{
	OnLine,
	OffLine,
	VUnknown
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

enum DiskFormat
{
	MBR,
	GPT,
	RAW
};

enum DiskState
{
	Missing,
	Offline,
	UnInit,
	Initing,
	Inited,
	DUnknown
};

enum DiskStyle
{
	Basic,
	Dynamic
};

class CDiskInfo
{
public: 
	unsigned __int64  m_Role;

	wstring           m_Protected;    //磁盘是否已被保护
	unsigned __int64  m_DiskSize ;     //磁盘容量大小（MB）  
	unsigned int      m_DiskIndex;     //磁盘索引
    wstring           *m_Guid;         //磁盘的GUID
    wstring           *m_DiskOEM;      //磁盘厂商
    DiskFormat        m_DiskFormat;    //磁盘格式
	DiskStyle         m_DiskStyle;     //磁盘类型
	DiskState         m_DiskState;     //磁盘状态
    
public:
	CDiskInfo (DeviceRole role,unsigned __int64 Size,unsigned int Index,
		wstring *guid,wstring *DiskOEM,DiskFormat format,DiskStyle style,DiskState state)
	{
		if(role == Free)
			m_Protected = L"false";
		else
			m_Protected = L"true";

		m_DiskSize  = Size;
		m_DiskIndex = Index;
		m_Guid      = guid;
		m_DiskOEM   = DiskOEM;
		m_DiskFormat = format;
		m_DiskStyle = style;
		m_DiskState = Inited;
	}
};

class CVolumeInfo
{
public: 
	DeviceRole        m_Role;

	wstring           m_Protected;    //分区卷是否已被保护
	unsigned __int64  m_VolumeSize;    //blocks
	unsigned __int64  m_UsedSpace;     //已用容量（MB）
	wstring           *m_GUID;         //分区卷GUID
	wstring           *m_VolumeLable;  //盘符(C: D:)
	wstring           *m_DiskGUID;     //所在硬盘GUID
	FileSys           m_FileSys;       //文件系统
	VolumeState       m_VolumeState;   //分区状态

	      
public: 
	CVolumeInfo(DeviceRole role,unsigned __int64 Size,unsigned __int64 FreeSize,
		wstring *guid,wstring *VolumeLable,wstring *diskguid,FileSys filesys,VolumeState state)
	{
		if(role == Free)
			m_Protected = L"false";
		else
			m_Protected = L"true";

		m_VolumeSize    = Size ;
		m_UsedSpace     = Size - FreeSize;
		m_GUID          = guid;
		m_VolumeLable   = VolumeLable;
		m_DiskGUID      = diskguid;
		m_FileSys       = filesys;
		m_VolumeState   = state;
	}
};

class CMirrorInfo
{
public: 
	wstring           *m_SourceGuid;
	wstring           *m_TargetGuid;
	MirrorState       m_State;
	unsigned int      m_EIMMode;  //1:EIM Volume is Source, 2:EIM Volume is Target, 3:Both are EIM Volume
	unsigned int      m_IsCluster;
	wstring           *m_ClusterResourceName;

public:
	CMirrorInfo(wstring *SGuid,wstring *TGuid, 
		unsigned int eimMode,unsigned int isCluster,
		wstring *clusterResourceName)
	{
		m_SourceGuid  =SGuid;
		m_TargetGuid  =TGuid;
		m_EIMMode = eimMode;
		m_IsCluster = isCluster;
		m_ClusterResourceName = clusterResourceName;
	}
};

class OSNAccount
{
public: 
	wstring *Admin;
	wstring *Pass;
	
public:
	OSNAccount()
    { }

	OSNAccount(wstring *name, wstring *pass)
    {
		Admin = name;
        Pass = pass;
	}
};

class DatabaseInformation
{
public: 
	bool               IsHave;                    //是否有该数据库
	DATABASE_TYPE      Type;                      //什么数据库
	wstring            *volumeLabel;              //数据保存卷
	OSNAccount         *AdminAccount;             //管理员帐号
	bool               IsForceCDP;                //是否强制创建CDP而不管是否刷新成功与否
	wstring            *Instance;

public: 
	DatabaseInformation()
    { }
	
	DatabaseInformation(DATABASE_TYPE dbtype, bool Have, wstring *label, OSNAccount *account, bool forceCDP)
    {
		IsHave = Have;
        Type = dbtype;
        volumeLabel = label;
        AdminAccount = account;
        IsForceCDP = forceCDP;
		Instance=NULL;
    }
};

class CDPArg
{
public:
	wstring       *argstr;
    unsigned int EIMMode;
    wstring       *guid;

private: 
};