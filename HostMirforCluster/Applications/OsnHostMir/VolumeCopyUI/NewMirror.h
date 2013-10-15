#pragma once

#include <locale.h>
#include "MyClusApi.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Management;
using namespace System::Drawing;
using namespace System::Diagnostics;


namespace VolumeCopyUI {

	/// <summary>
	/// Summary for NewMirror
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CNewMirror : public System::Windows::Forms::Form
	{
	public:
		CNewMirror(CDriverInfo ^pDriverInfo,CDriverInfoList ^pDriverInfoList,INSTALLTYPE Ty,
			CDiskInfoList ^pDiskInfoList)
		{
			InitializeComponent();

			pSourceDriver = pDriverInfo;
			pTargetDriver= nullptr;
			pSystemDriverList = pDriverInfoList;
			this->pSystemDiskList = pDiskInfoList;

			this->textBox1->Text = pDriverInfo->m_VolumeLable;
			this->textBox2->Text = (pDriverInfo->m_DriverSize/(2*1024)).ToString();

			Type = Ty;
			bNoClusterflag = true;
			this->m_EIMMode = 2;  //target;
			this->m_IsCluster=0;//not Cluster;
			cbResource->Enabled = false;
			m_ClusterResourceName = "NONE";//not Cluster Resource
			RefreshListView();
			arraylist = gcnew ArrayList();

		}

		CNewMirror(CDiskInfo ^pDiskInfo,CDiskInfoList ^pDiskInfoList,CDriverInfoList ^pDriverInfoList)
		{
			InitializeComponent();

			pSourceDisk = pDiskInfo;
			pTargetDisk= nullptr;
			this->pSystemDiskList = pDiskInfoList;
			pSystemDriverList = pDriverInfoList;
			bNoClusterflag = true;

			this->label1->Text = "磁盘索引:";

			this->textBox1->Text = pDiskInfo->m_DiskIndex.ToString();
			this->textBox2->Text = (pDiskInfo->m_DiskSize/2048).ToString();

			Type = DiskCopy;
			this->m_EIMMode = 2;  //target;
			this->m_IsCluster=0;//not Cluster;
			m_ClusterResourceName = "NONE";//not Cluster Resource
			this->cbResource->Enabled =false;

			RefreshListView();
			arraylist = gcnew ArrayList();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CNewMirror()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  buttonOK;

	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	public:  CDriverInfo ^pSourceDriver;
	public:  CDriverInfo ^pTargetDriver;
	public:  CDriverInfoList ^pSystemDriverList;
	public:  CDiskInfo ^pSourceDisk;
	public:  CDiskInfo ^pTargetDisk;
	public:  CDiskInfoList ^pSystemDiskList;
	public:  INSTALLTYPE  Type;
	public:  unsigned int m_EIMMode;
	public : unsigned int m_IsCluster;
	public:	String ^m_ClusterResourceName;
	public: ArrayList ^arraylist;
	public: BOOLEAN   bNoClusterflag;

	private: System::Windows::Forms::ComboBox^  comboBoxMode;
	private: System::Windows::Forms::Label^  labelMode;
	public: 

	public: 


	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::CheckBox^  ckbCluster;

	private: System::Windows::Forms::Label^  lbResourceName;
	private: System::Windows::Forms::ComboBox^  cbResource;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->buttonOK = (gcnew System::Windows::Forms::Button());
			this->comboBoxMode = (gcnew System::Windows::Forms::ComboBox());
			this->labelMode = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->cbResource = (gcnew System::Windows::Forms::ComboBox());
			this->lbResourceName = (gcnew System::Windows::Forms::Label());
			this->ckbCluster = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"源卷(磁盘):";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"大小(MB):";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(95, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(133, 21);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(95, 39);
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->Size = System::Drawing::Size(133, 21);
			this->textBox2->TabIndex = 3;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->listView1);
			this->groupBox1->Location = System::Drawing::Point(5, 66);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(223, 170);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"可选对象";
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, this->columnHeader2});
			this->listView1->FullRowSelect = true;
			this->listView1->Location = System::Drawing::Point(20, 85);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(258, 145);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"DriveName";
			this->columnHeader1->Width = 80;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Size(MB)";
			this->columnHeader2->Width = 107;
			// 
			// buttonOK
			// 
			this->buttonOK->Location = System::Drawing::Point(111, 354);
			this->buttonOK->Name = L"buttonOK";
			this->buttonOK->Size = System::Drawing::Size(56, 22);
			this->buttonOK->TabIndex = 5;
			this->buttonOK->Text = L"确定";
			this->buttonOK->UseVisualStyleBackColor = true;
			this->buttonOK->Click += gcnew System::EventHandler(this, &CNewMirror::buttonOK_Click);
			// 
			// comboBoxMode
			// 
			this->comboBoxMode->FormattingEnabled = true;
			this->comboBoxMode->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Source", L"Target", L"Both"});
			this->comboBoxMode->Location = System::Drawing::Point(95, 239);
			this->comboBoxMode->Name = L"comboBoxMode";
			this->comboBoxMode->Size = System::Drawing::Size(113, 20);
			this->comboBoxMode->TabIndex = 6;
			this->comboBoxMode->Text = L"Target";
			// 
			// labelMode
			// 
			this->labelMode->AutoSize = true;
			this->labelMode->Location = System::Drawing::Point(24, 242);
			this->labelMode->Name = L"labelMode";
			this->labelMode->Size = System::Drawing::Size(47, 12);
			this->labelMode->TabIndex = 7;
			this->labelMode->Text = L"EIM卷：";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->cbResource);
			this->groupBox2->Controls->Add(this->lbResourceName);
			this->groupBox2->Controls->Add(this->ckbCluster);
			this->groupBox2->Location = System::Drawing::Point(14, 265);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(264, 83);
			this->groupBox2->TabIndex = 11;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"集群应用";
			// 
			// cbResource
			// 
			this->cbResource->FormattingEnabled = true;
			this->cbResource->Location = System::Drawing::Point(137, 50);
			this->cbResource->Name = L"cbResource";
			this->cbResource->Size = System::Drawing::Size(121, 20);
			this->cbResource->TabIndex = 11;
			// 
			// lbResourceName
			// 
			this->lbResourceName->AutoSize = true;
			this->lbResourceName->Location = System::Drawing::Point(4, 53);
			this->lbResourceName->Name = L"lbResourceName";
			this->lbResourceName->Size = System::Drawing::Size(137, 12);
			this->lbResourceName->TabIndex = 10;
			this->lbResourceName->Text = L"源盘在集群中的资源名：";
			// 
			// ckbCluster
			// 
			this->ckbCluster->AutoSize = true;
			this->ckbCluster->Location = System::Drawing::Point(6, 20);
			this->ckbCluster->Name = L"ckbCluster";
			this->ckbCluster->Size = System::Drawing::Size(84, 16);
			this->ckbCluster->TabIndex = 8;
			this->ckbCluster->Text = L"应用于集群";
			this->ckbCluster->UseVisualStyleBackColor = true;
			this->ckbCluster->CheckedChanged += gcnew System::EventHandler(this, &CNewMirror::ckbCluster_CheckedChanged);
			// 
			// CNewMirror
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(298, 388);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->labelMode);
			this->Controls->Add(this->comboBoxMode);
			this->Controls->Add(this->buttonOK);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"CNewMirror";
			this->Text = L"设置";
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		void VolumeListView()
		{
			this->listView1->Items->Clear();
			int index =0;

			for(int i =0;i<this->pSystemDriverList->GetLength();i++)
			{
				CDriverInfo ^pTemp = static_cast<CDriverInfo ^>(pSystemDriverList->GetItem(i));
				if(pTemp->m_VolumeLable->Equals(this->pSourceDriver->m_VolumeLable))
					continue;
				//if(pTemp->m_DriverSize<pSourceDriver->m_DriverSize)
				//continue;
				if(pTemp->m_Role!=Free)
					continue;

				// 如果卷所在磁盘是镜像目标盘，过滤
				if(this->pSystemDiskList->HideDriver(pTemp->m_GUID))
				{
					continue;
				}

				if(pTemp->m_VolumeLable->Contains("C")||pTemp->m_VolumeLable->Contains("c"))
					continue;


				this->listView1->Items->Add(pTemp->m_VolumeLable);
				this->listView1->Items[index]->SubItems->Add((pTemp->m_DriverSize/(2*1024)).ToString());
				index++;

			}

		}

		void DiskListView()
		{

			this->listView1->Items->Clear();
			this->listView1->Columns->Clear();

			String ^DiskIndex = "磁盘索引";
			String ^Size = "大小(MB)";

			listView1->Columns->Add(DiskIndex,50,HorizontalAlignment::Center);
			listView1->Columns->Add(Size,120,HorizontalAlignment::Center);


			int index =0;

			for(int i =0;i<this->pSystemDiskList->GetLength();i++)
			{
				CDiskInfo ^pTemp = static_cast<CDiskInfo ^>(pSystemDiskList->GetItem(i));
				if(pTemp->m_Guid==this->pSourceDisk->m_Guid)
					continue;
				//if(pTemp->m_DiskIndex == 0) // skip disk 0#
				//continue;

				//if(pTemp->m_DiskSize<pSourceDisk->m_DiskSize)
				//continue;


				// 如果目标磁盘上卷已经有镜像存在，过滤
				if(this->pSystemDriverList->CheckDriverMirrored(pTemp->m_Guid))
				{
					continue;
				}

				if(pTemp->m_Role!=Free)
					continue;


				this->listView1->Items->Add(pTemp->m_DiskIndex.ToString());
				this->listView1->Items[index]->SubItems->Add((pTemp->m_DiskSize/2048).ToString());
				index++;

			}

		}


		void RefreshListView()
		{

			if(this->Type == VolumeCopy)
				VolumeListView();
			else
				DiskListView();

		}
#pragma endregion
		bool CheckVolIsBootableOrSys(String^ label)
		{
			try
			{

				String ^WQL = "Associators   of   {win32_LogicalDisk='%s'}   where   resultClass   =   Win32_DiskPartition";

				String ^NewLine = WQL->Replace("%s", label);

				// obtain the disk 
				ManagementObjectSearcher ^DiskPartitionSearcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
					NewLine);
				String^ sysvol=(String ^)System::Environment::SystemDirectory;
				sysvol=sysvol->Substring(0,2);
				bool issysvol=false;
				if(sysvol->Equals(label))
				{
					issysvol=true;
					return  issysvol;
				}

				ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();

				System::Collections::IEnumerator	^objEnum1= DiskCollection->GetEnumerator();
				while(objEnum1->MoveNext())
				{
					ManagementObject ^queryObj1=static_cast<ManagementObject ^>(objEnum1->Current);                       
					bool ret=Convert::ToBoolean(queryObj1->GetPropertyValue("Bootable"));
					return issysvol||ret;
				}
				return issysvol;
			}
			catch(...)
			{
				// myEventLog->OSNWriteEventLog(String::Concat("CheckVolIsBootable: ",ex->ToString()),EventLogEntryType::Error,011);
			}
			return false;

		}
		int CheckFileSystem(String ^LabelName)
		{

			if(LabelName == nullptr)
				return 0;

			char VolumeNameBuffer[32];
			DWORD lpVolumeSerialNumber;
			DWORD lpMaximumComponentLength;
			DWORD lpFileSystemFlags;
			char  lpFileSystemNameBuffer[32];

			BOOL ReturnValue = GetVolumeInformation((char *)Marshal::StringToHGlobalAnsi(String::Concat(LabelName,"\\")).ToPointer(),
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

		int CheckDiskIsEIMDisk(int index)
		{
			try
			{
				String^ WQL="Select * from Win32_DiskDrive where Index=%s";
				String^ newLine=WQL->Replace("%s",index.ToString());
				ManagementObjectSearcher ^DiskPartitionSearcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
					newLine);
				ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();

				System::Collections::IEnumerator	^objEnum1= DiskCollection->GetEnumerator();
				while(objEnum1->MoveNext())
				{
					ManagementObject ^queryObj1=static_cast<ManagementObject ^>(objEnum1->Current);                       
					String^  ret=Convert::ToString(queryObj1->GetPropertyValue("PNPDeviceID"));
					if(ret->Contains("EIM"))
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
				return 0;
			}
			catch(...)
			{
				MessageBox::Show("获取磁盘的提供商信息出错，请稍后重试");
			}
			return -1;
		}

		int InitalizeDisk(int index)
		{
			Process^ proc=gcnew Process();
			try
			{
				proc->StartInfo->FileName="diskpart.exe";
				proc->StartInfo->RedirectStandardError=true;
				proc->StartInfo->RedirectStandardInput=true;
				proc->StartInfo->RedirectStandardOutput=true;
				proc->StartInfo->CreateNoWindow=true;
				proc->StartInfo->UseShellExecute=false;
				proc->Start();
				proc->StandardInput->WriteLine(String::Concat("select disk ",Convert::ToString(index)));
				proc->StandardInput->Flush();
				proc->StandardInput->WriteLine("clean");
				proc->StandardInput->Flush();
				proc->StandardInput->WriteLine("create partition primary");
				proc->StandardInput->Flush();
				proc->StandardInput->WriteLine("delete partition");
				proc->StandardInput->Flush();
				proc->StandardInput->WriteLine("exit");
				proc->StandardInput->Flush();
				proc->WaitForExit(2000);
				System::Threading::Thread::Sleep(5*1000);
				return 0;
			}
			catch(...)
			{
				MessageBox::Show(String::Concat("初始化磁盘出现异常,请手动初始化磁盘",Convert::ToString(index)));
				return 1;
			}
		}

		///1 eim disk 2,don't eim disk ,o get failed
		int CheckVolIsEIMVol(String ^LabelName)
		{
			try
			{
				String ^WQL = "Associators of {win32_LogicalDisk='%s'} where resultClass = Win32_DiskPartition";

				String ^NewLine = WQL->Replace("%s", LabelName);

				// obtain the disk 
				ManagementObjectSearcher ^DiskPartitionSearcher =gcnew ManagementObjectSearcher("root\\CIMV2",NewLine);

				ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();

				System::Collections::IEnumerator	^objEnum= DiskCollection->GetEnumerator();
				while(objEnum->MoveNext())
				{
					ManagementObject ^queryObj=safe_cast<ManagementObject ^>(objEnum->Current);                       
					UInt32 Index2 = Convert::ToUInt32(queryObj->GetPropertyValue("DiskIndex"));
					return CheckDiskIsEIMDisk(Index2);
				}
			}
			catch(...)
			{
				MessageBox::Show("获取卷的提供商信息失败，请稍后重试");

			}
			return -1;
		}
		void VolumeMirrorClick()
		{
			bool isEIMMirror = false;
			if(this->listView1->SelectedItems->Count!=1)
			{
				MessageBox::Show("请选择驱动器");
				return;
			}

			ListViewItem ^SelectItem = this->listView1->SelectedItems[0];
			String ^LabelName = SelectItem->SubItems[0]->Text;

			this->pTargetDriver = this->pSystemDriverList ->GetDriverInfoByLabel(LabelName);

			if(pTargetDriver==nullptr)
			{
				MessageBox::Show("无法获取目标驱动器");
				return;
			}

			RegistryKey ^pRegKey=Registry::LocalMachine;
			RegistryKey ^pParmeterKey;

			pParmeterKey=pRegKey->OpenSubKey(String::Concat("SYSTEM\\CurrentControlSet\\Services\\OSNVSS","\\Parameters"),true);
			if(pParmeterKey==nullptr)
			{
				isEIMMirror = false;
			}
			else if(pParmeterKey->GetValue("EIMMirror")==nullptr)
			{
				isEIMMirror = false;
				pParmeterKey->SetValue("EIMMirror",2);
			}
			else if(Convert::ToUInt32(pParmeterKey->GetValue("EIMMirror")) == 1)
			{
				isEIMMirror = true;
			}

			int ret=CheckVolIsEIMVol(this->pTargetDriver->m_VolumeLable);
			int ret1=CheckVolIsEIMVol(this->pSourceDriver->m_VolumeLable);

			if(!isEIMMirror&&2==ret&&2==ret1)
			{
				MessageBox::Show("源卷和目标卷都不是EIM卷，不能建镜像关系！");
				return ;
			}

			if(1==this->m_EIMMode)//source
			{
				int ret=CheckVolIsEIMVol(this->pSourceDriver->m_VolumeLable);
				if(-1==ret)
				{
					return;
				}
				else if(2==ret)
				{
					if(MessageBox::Show("源卷不是EIM卷，最好不要选source模式，是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}
			else if(2==this->m_EIMMode)//target
			{
				int ret=CheckVolIsEIMVol(this->pTargetDriver->m_VolumeLable);
				if(-1==ret)
				{
					return;
				}
				else if(2==ret)
				{
					if(MessageBox::Show("目标卷不是EIM卷，最好不要选target模式，是否继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}
			else if(4==this->m_EIMMode)//both
			{
				int ret=CheckVolIsEIMVol(this->pTargetDriver->m_VolumeLable);
				int ret1=CheckVolIsEIMVol(this->pSourceDriver->m_VolumeLable);
				if(-1==ret||-1==ret)
				{
					return;
				}
				else if(2==ret||2==ret1)
				{
					if(MessageBox::Show("目标卷或者是源卷不是EIM卷，最好不要选both模式，是否继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}
			int tgtret= OsnCheckGptVolume((const char)LabelName[0]);
			int srcret=OsnCheckGptVolume((const char)this->pSourceDriver->m_VolumeLable[0]);
			// MessageBox::Show(String::Concat("target=",tgtret.ToString()));
			//MessageBox::Show(String::Concat("source=",srcret.ToString()));
			if(tgtret>1||srcret>1)
			{
				MessageBox::Show("源卷和目标卷中有一个卷处于被保护状态或者其他未知状态，请检查是否处于镜像关系中，或者是已经删除镜像关系但要重启后生效");
				return;
			}
			/* if(tgtret!=srcret)
			{
			if( MessageBox::Show("源卷和目标卷一个是MBR磁盘，一个是GPT磁盘，不能创建镜像关系,请重新选择目标卷","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
			{
			return;
			}
			}*/

			if(this->pSourceDriver->m_DriverSize>pTargetDriver->m_DriverSize)
			{
				if(MessageBox::Show("镜像卷比源卷小,是否要继续?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
				/* MessageBox::Show("镜像卷比源卷小,请重新选择镜像卷");
				return;*/
			}
			int isbootvol=CheckVolIsBootableOrSys(LabelName);
			if(isbootvol)
			{
				MessageBox::Show("镜像卷是系统卷或者是启动卷，不能设置为镜像卷，请选择其他卷");
				return;

			}

			// check the file system
			int ErrorCode = CheckFileSystem(LabelName);
			if(ErrorCode!=0)
			{
				if(MessageBox::Show("镜像卷已经有文件系统存在,如果继续数据会被覆盖,是否继续?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
			}

			if(MessageBox::Show("请再次确认是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
			{
				return;
			}

			this->DialogResult =System::Windows::Forms::DialogResult::Yes;
			return;

		}

		void DiskMirrorClick()
		{
			bool isEIMMirror = false;
			if(this->listView1->SelectedItems->Count!=1)
			{
				MessageBox::Show("请选择驱动器");
				return;
			}
			ListViewItem ^SelectItem = this->listView1->SelectedItems[0];
			String ^Index = SelectItem->SubItems[0]->Text;

			this->pTargetDisk = this->pSystemDiskList ->GetDiskByIndex(Convert::ToUInt32(Index));
			if(pTargetDisk==nullptr)
			{
				MessageBox::Show("无法获取目标驱动器");
				return;
			}

			RegistryKey ^pRegKey=Registry::LocalMachine;
			RegistryKey ^pParmeterKey;

			pParmeterKey=pRegKey->OpenSubKey(String::Concat("SYSTEM\\CurrentControlSet\\Services\\OSNDSS","\\Parameters"),true);
			if(pParmeterKey==nullptr)
			{
				isEIMMirror = false;
			}
			else if(pParmeterKey->GetValue("EIMMirror")==nullptr)
			{
				isEIMMirror = false;
				pParmeterKey->SetValue("EIMMirror",2);
			}
			else if(Convert::ToUInt32(pParmeterKey->GetValue("EIMMirror")) == 1)
			{
				isEIMMirror = true;
			}

			int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
			int ret1=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
			if(!isEIMMirror&&2==ret&&2==ret1)
			{
				MessageBox::Show("源磁盘和目标磁盘都不是EIM卷，不能建镜像关系！");
				return ;
			}

			if(1==this->m_EIMMode)//source
			{
				int ret=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
				if(-1==ret)
				{
					return;
				}
				else if(2==ret)
				{
					if(MessageBox::Show("源磁盘不是EIM卷，最好不要选source模式，是否继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}
			else if(2==this->m_EIMMode)//target
			{
				int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
				if(-1==ret)
				{
					return;
				}
				else if(2==ret)
				{
					if(MessageBox::Show("目标磁盘不是EIM卷，最好不要选target模式，是否继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}
			else if(4==this->m_EIMMode)//both
			{
				int ret=CheckDiskIsEIMDisk(this->pTargetDisk->m_DiskIndex);
				int ret1=CheckDiskIsEIMDisk(this->pSourceDisk->m_DiskIndex);
				if(-1==ret||-1==ret)
				{
					return;
				}
				else if(2==ret||2==ret1)
				{
					if(MessageBox::Show("目标磁盘或者是源磁盘不是EIM卷，最好不要选both模式，是否继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
					{
						return;
					}
				}
			}

			// check the partition information on this disk
			int tgtret= OsnCheckGptDisk(Convert::ToUInt32(Index));
			int srcret=OsnCheckGptDisk(this->pSourceDisk->m_DiskIndex);
			// MessageBox::Show(String::Concat("target=",tgtret.ToString()));
			//MessageBox::Show(String::Concat("source=",srcret.ToString()));
			if(tgtret>1||srcret>1)
			{
				MessageBox::Show("源卷和目标卷中有一个卷处于被保护状态或者其他未知状态，请检查是否处于镜像关系中，或者是已经删除镜像关系但要重启后生效");
				return;
			}
			if(tgtret!=srcret)
			{
				MessageBox::Show("源卷和目标卷一个是MBR磁盘，一个是GPT磁盘，不能创建镜像关系,请重新选择目标卷");
				return;
			}

			int ErrorCode =OSNCheckDiskRawProperty(pTargetDisk->m_DiskIndex);
			if(ErrorCode!=0)
			{
				if(MessageBox::Show("镜像磁盘已经有分区存在,如果继续数据会被覆盖,是否继续?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
			}
			if(this->pTargetDisk->m_Guid->Equals("00000000-0000-0000-0000-000000000000"))//没有初始化，需要执行初始化操作
			{
				int ret=InitalizeDisk(this->pTargetDisk->m_DiskIndex);
				if(1==ret)
				{
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
						String^ chguid=nullptr;
						char guid[128];
						OsnGUIDToString(guid,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
						chguid=gcnew String(guid);
						this->pTargetDisk->m_Guid=chguid;
						this->pTargetDisk->m_DiskSize=pDisk.m_DiskSize;
					}
				}

			}


			if(this->pSourceDisk->m_DiskSize>this->pTargetDisk->m_DiskSize)
			{
				if(MessageBox::Show("镜像磁盘比源磁盘要小,是否需要继续?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
				// MessageBox::Show("镜像磁盘比源磁盘要小,不能创建镜像");
				// return;
			}

			if(MessageBox::Show("请再次确认是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
			{
				return;
			}

			this->DialogResult =System::Windows::Forms::DialogResult::Yes;
			return;
		}


	private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if(this->comboBoxMode->Text->Equals(String::Empty))
				 {
					 MessageBox::Show("请选择EIM卷的角色。");
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
				 }

				 if(this->ckbCluster->Checked)
				 {

					 RegistryKey ^pRegKey=Registry::LocalMachine;
					 RegistryKey ^pParmeterKey;

					 pParmeterKey=pRegKey->OpenSubKey(String::Concat("SYSTEM\\CurrentControlSet\\Services\\OSNHBService","\\Parameters"),true);
					 if(pParmeterKey==nullptr)
					 {
						 MessageBox::Show("请先设置集群IP！");
						 return ;
					 }
					 else if(pParmeterKey->GetValue("PartnerName")==nullptr||pParmeterKey->GetValue("PriIPAddress")==nullptr||
						 pParmeterKey->GetValue("SecIPAddress")==nullptr)
					 {
						 MessageBox::Show("请先设置集群IP！");
						 return;
					 }
					 if(bNoClusterflag)
					 {
						int ret = ::MessageBox(NULL,TEXT("未检测到集群，请确认是否继续。如果继续，可能镜像会无法正确工作。"),TEXT("Warning"),MB_OKCANCEL);
						if(ret == IDCANCEL)
							return ;
					 }
					 else
					 {
						 if(this->cbResource->SelectedValue->ToString() == "")
						 {
							 MessageBox::Show("请设置源盘在集群中的资源名称！");
							 return;
						 }
					 }
					 this->m_IsCluster=1;
					 if(bNoClusterflag)
						 this->m_ClusterResourceName = "";
					 else
						 this->m_ClusterResourceName = this->cbResource->SelectedValue->ToString();
				 }


				 if(this->Type ==VolumeCopy)
				 {
					 VolumeMirrorClick();
				 }
				 else
				 {
					 DiskMirrorClick();
				 }
			 }
	private: System::Void ckbCluster_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if(ckbCluster->Checked)
				 {
					 this->cbResource->Enabled =true;
					 if(CheckClusterResourceNameBind())
					 {
						 this->cbResource->DataSource = arraylist;
					 }
				 }
				 else if(!ckbCluster->Checked)
					 this->cbResource->Enabled = false;
			 }

	private: bool CheckClusterResourceNameBind()
			 {
				 LPCLUSTERVERSIONINFO lpClusterInfo;
				 HCLUSENUM hclusenum;
				 HCLUSTER hcluster;
				 WCHAR	clusterName[128] ;	
				 WCHAR	nodeName[10][128];
				 WCHAR	*pResourceName;
				 DWORD	chClusterName =128;
				 DWORD	dwtype;
				 DWORD	enumCount;
				 DWORD	chNodeNameLen;
				 WCHAR *szbuffer;
				 DWORD ret = 0;

				 memset(clusterName,0,128);

				 for(int i = 0;i<10;i++)
				 {
					 memset(nodeName[i],0,128);
				 }
				 DWORD nodeState = ClusterNodeStateUnknown;

				 lpClusterInfo = (LPCLUSTERVERSIONINFO)malloc(sizeof(CLUSTERVERSIONINFO));
				 try
				 {
					 hcluster = OpenCluster(NULL);//name is NULL ,through LPC get the local cluster handle
					 if(hcluster == NULL)
					 {
						 free(lpClusterInfo);
						 bNoClusterflag = true;
						 return  false;
					 }
					 else
					 {
						 bNoClusterflag = false;
					 }

					 ret = GetClusterInformation(hcluster,clusterName,&chClusterName,lpClusterInfo);
					 if(ret !=ERROR_SUCCESS)
					 {
						 free(lpClusterInfo);
						 return false;
					 }

					 hclusenum = ClusterOpenEnum(hcluster,CLUSTER_ENUM_RESOURCE);
					 if(!hclusenum)
					 {
						 free(lpClusterInfo);
						 CloseCluster(hcluster);
						 return false;
					 }

					 dwtype = CLUSTER_ENUM_RESOURCE;
					 pResourceName = (LPWSTR)malloc(256);
					 szbuffer = (WCHAR *)malloc(256);
					 enumCount = ClusterGetEnumCount(hclusenum);
					 for(DWORD i= 0 ;i<enumCount ;++i)
					 {
						 chNodeNameLen = 256;
						 memset(szbuffer,0,sizeof(szbuffer));

						 ret = ClusterEnum(hclusenum,i,&dwtype,pResourceName,&chNodeNameLen);//get enum something
						 if(ret == ERROR_SUCCESS)
						 {
							 setlocale(LC_CTYPE, "chs");
							 String ^dd = Marshal::PtrToStringUni((IntPtr)pResourceName);
							 arraylist->Add(dd);
							 setlocale(LC_CTYPE, "C");
						 }

					 }
				 }catch(...)
				 {

				 }		
				 free(szbuffer);
				 free(pResourceName);

				 free(lpClusterInfo);
				 //CloseClusterNode(hnode);
				 ClusterCloseEnum(hclusenum);
				 CloseCluster(hcluster);
				 return true;
			 }
	};
}
