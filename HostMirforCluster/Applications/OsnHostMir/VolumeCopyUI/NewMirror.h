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

			this->label1->Text = "��������:";

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
			this->label1->Text = L"Դ��(����):";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"��С(MB):";
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
			this->groupBox1->Text = L"��ѡ����";
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
			this->buttonOK->Text = L"ȷ��";
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
			this->labelMode->Text = L"EIM��";
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
			this->groupBox2->Text = L"��ȺӦ��";
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
			this->lbResourceName->Text = L"Դ���ڼ�Ⱥ�е���Դ����";
			// 
			// ckbCluster
			// 
			this->ckbCluster->AutoSize = true;
			this->ckbCluster->Location = System::Drawing::Point(6, 20);
			this->ckbCluster->Name = L"ckbCluster";
			this->ckbCluster->Size = System::Drawing::Size(84, 16);
			this->ckbCluster->TabIndex = 8;
			this->ckbCluster->Text = L"Ӧ���ڼ�Ⱥ";
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
			this->Text = L"����";
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

				// ��������ڴ����Ǿ���Ŀ���̣�����
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

			String ^DiskIndex = "��������";
			String ^Size = "��С(MB)";

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


				// ���Ŀ������Ͼ��Ѿ��о�����ڣ�����
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
				MessageBox::Show("��ȡ���̵��ṩ����Ϣ�������Ժ�����");
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
				MessageBox::Show(String::Concat("��ʼ�����̳����쳣,���ֶ���ʼ������",Convert::ToString(index)));
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
				MessageBox::Show("��ȡ����ṩ����Ϣʧ�ܣ����Ժ�����");

			}
			return -1;
		}
		void VolumeMirrorClick()
		{
			bool isEIMMirror = false;
			if(this->listView1->SelectedItems->Count!=1)
			{
				MessageBox::Show("��ѡ��������");
				return;
			}

			ListViewItem ^SelectItem = this->listView1->SelectedItems[0];
			String ^LabelName = SelectItem->SubItems[0]->Text;

			this->pTargetDriver = this->pSystemDriverList ->GetDriverInfoByLabel(LabelName);

			if(pTargetDriver==nullptr)
			{
				MessageBox::Show("�޷���ȡĿ��������");
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
				MessageBox::Show("Դ���Ŀ�������EIM�����ܽ������ϵ��");
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
					if(MessageBox::Show("Դ����EIM����ò�Ҫѡsourceģʽ���Ƿ����","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
					if(MessageBox::Show("Ŀ�����EIM����ò�Ҫѡtargetģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
					if(MessageBox::Show("Ŀ��������Դ����EIM����ò�Ҫѡbothģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
				MessageBox::Show("Դ���Ŀ�������һ�����ڱ�����״̬��������δ֪״̬�������Ƿ��ھ����ϵ�У��������Ѿ�ɾ�������ϵ��Ҫ��������Ч");
				return;
			}
			/* if(tgtret!=srcret)
			{
			if( MessageBox::Show("Դ���Ŀ���һ����MBR���̣�һ����GPT���̣����ܴ��������ϵ,������ѡ��Ŀ���","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
			{
			return;
			}
			}*/

			if(this->pSourceDriver->m_DriverSize>pTargetDriver->m_DriverSize)
			{
				if(MessageBox::Show("������Դ��С,�Ƿ�Ҫ����?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
				/* MessageBox::Show("������Դ��С,������ѡ�����");
				return;*/
			}
			int isbootvol=CheckVolIsBootableOrSys(LabelName);
			if(isbootvol)
			{
				MessageBox::Show("�������ϵͳ���������������������Ϊ�������ѡ��������");
				return;

			}

			// check the file system
			int ErrorCode = CheckFileSystem(LabelName);
			if(ErrorCode!=0)
			{
				if(MessageBox::Show("������Ѿ����ļ�ϵͳ����,����������ݻᱻ����,�Ƿ����?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
			}

			if(MessageBox::Show("���ٴ�ȷ���Ƿ����","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
				MessageBox::Show("��ѡ��������");
				return;
			}
			ListViewItem ^SelectItem = this->listView1->SelectedItems[0];
			String ^Index = SelectItem->SubItems[0]->Text;

			this->pTargetDisk = this->pSystemDiskList ->GetDiskByIndex(Convert::ToUInt32(Index));
			if(pTargetDisk==nullptr)
			{
				MessageBox::Show("�޷���ȡĿ��������");
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
				MessageBox::Show("Դ���̺�Ŀ����̶�����EIM�����ܽ������ϵ��");
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
					if(MessageBox::Show("Դ���̲���EIM����ò�Ҫѡsourceģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
					if(MessageBox::Show("Ŀ����̲���EIM����ò�Ҫѡtargetģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
					if(MessageBox::Show("Ŀ����̻�����Դ���̲���EIM����ò�Ҫѡbothģʽ���Ƿ������","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
				MessageBox::Show("Դ���Ŀ�������һ�����ڱ�����״̬��������δ֪״̬�������Ƿ��ھ����ϵ�У��������Ѿ�ɾ�������ϵ��Ҫ��������Ч");
				return;
			}
			if(tgtret!=srcret)
			{
				MessageBox::Show("Դ���Ŀ���һ����MBR���̣�һ����GPT���̣����ܴ��������ϵ,������ѡ��Ŀ���");
				return;
			}

			int ErrorCode =OSNCheckDiskRawProperty(pTargetDisk->m_DiskIndex);
			if(ErrorCode!=0)
			{
				if(MessageBox::Show("��������Ѿ��з�������,����������ݻᱻ����,�Ƿ����?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
			}
			if(this->pTargetDisk->m_Guid->Equals("00000000-0000-0000-0000-000000000000"))//û�г�ʼ������Ҫִ�г�ʼ������
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
				if(MessageBox::Show("������̱�Դ����ҪС,�Ƿ���Ҫ����?","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
				{
					return;
				}
				// MessageBox::Show("������̱�Դ����ҪС,���ܴ�������");
				// return;
			}

			if(MessageBox::Show("���ٴ�ȷ���Ƿ����","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
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
					 MessageBox::Show("��ѡ��EIM��Ľ�ɫ��");
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
						 MessageBox::Show("�������ü�ȺIP��");
						 return ;
					 }
					 else if(pParmeterKey->GetValue("PartnerName")==nullptr||pParmeterKey->GetValue("PriIPAddress")==nullptr||
						 pParmeterKey->GetValue("SecIPAddress")==nullptr)
					 {
						 MessageBox::Show("�������ü�ȺIP��");
						 return;
					 }
					 if(bNoClusterflag)
					 {
						int ret = ::MessageBox(NULL,TEXT("δ��⵽��Ⱥ����ȷ���Ƿ������������������ܾ�����޷���ȷ������"),TEXT("Warning"),MB_OKCANCEL);
						if(ret == IDCANCEL)
							return ;
					 }
					 else
					 {
						 if(this->cbResource->SelectedValue->ToString() == "")
						 {
							 MessageBox::Show("������Դ���ڼ�Ⱥ�е���Դ���ƣ�");
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
