#ifndef _MIRRORUICONFIGURE_H
#define _MIRRORUICONFIGURE_H

#include <string>
using namespace std;

#define bufsize 256

enum RegKey
{
	BoolKey,
	StringKey,
	DwordKey,
};

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

	wstring           m_Protected;    //�����Ƿ��ѱ�����
	unsigned __int64  m_DiskSize ;     //����������С��MB��  
	unsigned __int64  m_UsedSpace;
	unsigned int      m_DiskIndex;     //��������
    wstring           *m_Guid;         //���̵�GUID
    wstring           *m_DiskOEM;      //���̳���
    DiskFormat        m_DiskFormat;    //���̸�ʽ
	DiskStyle         m_DiskStyle;     //��������
	DiskState         m_DiskState;     //����״̬
    
public:
	CDiskInfo (DeviceRole role,unsigned __int64 Size,unsigned __int64 UsedSize,unsigned int Index,
		wstring *guid,wstring *DiskOEM,DiskFormat format,DiskStyle style,DiskState state)
	{
		if(role == Free)
			m_Protected = L"false";
		else
			m_Protected = L"true";

		m_Role      = role;
		m_DiskSize  = Size;
		m_UsedSpace = UsedSize;
		m_DiskIndex = Index;
		m_Guid      = guid;
		m_DiskOEM   = DiskOEM;
		m_DiskFormat = format;
		m_DiskStyle = style;
		m_DiskState = Inited;
	}

	~CDiskInfo()
	{
		delete(m_Guid);
		delete(m_DiskOEM);
	}
};

class CVolumeInfo
{
public: 
	DeviceRole        m_Role;

	wstring           m_Protected;    //�������Ƿ��ѱ�����
	unsigned __int64  m_VolumeSize;    //blocks
	unsigned __int64  m_UsedSpace;     //����������MB��
	wstring           *m_GUID;         //������GUID
	wstring           *m_VolumeLable;  //�̷�(C: D:)
	wstring           *m_DiskGUID;     //����Ӳ��GUID
	FileSys           m_FileSys;       //�ļ�ϵͳ
	VolumeState       m_VolumeState;   //����״̬

	      
public: 
	CVolumeInfo(DeviceRole role,unsigned __int64 Size,unsigned __int64 UsedSize,
		wstring *guid,wstring *VolumeLable,wstring *diskguid,FileSys filesys,VolumeState state)
	{
		if(role == Free)
			m_Protected = L"false";
		else
			m_Protected = L"true";

		m_Role          = role;
		m_VolumeSize    = Size ;
		m_UsedSpace     = UsedSize;
		m_GUID          = guid;
		m_VolumeLable   = VolumeLable;
		m_DiskGUID      = diskguid;
		m_FileSys       = filesys;
		m_VolumeState   = state;
	}

	~CVolumeInfo()
	{
		delete(m_GUID);
		delete(m_VolumeLable);
		delete(m_DiskGUID);
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

	~CMirrorInfo()
	{
		delete(m_ClusterResourceName);
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

	~OSNAccount()
	{
		delete(Admin);
		delete(Pass);
	}
};

class DatabaseInformation
{
public: 
	bool               IsHave;                    //�Ƿ��и����ݿ�
	DATABASE_TYPE      Type;                      //ʲô���ݿ�
	wstring            *volumeLabel;              //���ݱ����
	OSNAccount         *AdminAccount;             //����Ա�ʺ�
	bool               IsForceCDP;                //�Ƿ�ǿ�ƴ���CDP�������Ƿ�ˢ�³ɹ����
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

	~DatabaseInformation()
	{
		delete(Instance);
		delete(AdminAccount);
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


#endif