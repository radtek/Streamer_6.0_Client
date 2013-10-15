#pragma once
#include <stdio.h>
#include "MyClusApi.h"
#include <locale.h>
#using <mscorlib.dll>



//#define IOCTL_OSNVSS_ASYNORSYN				OSN_MAKE_IOCTL(OSNVSS_DEVICE_TYPE,36)

namespace VolumeCopyUI {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Runtime::InteropServices;
	using namespace System::Runtime ::CompilerServices;
	using namespace System::Management;
	using namespace System::Threading;
	using namespace System::Diagnostics;
	using namespace System::IO;
	using namespace System::Data::SqlClient;
	using namespace System::Xml;
	using namespace System::Text;
	using namespace System::Xml::Serialization;
	using namespace System::Security::Cryptography;
	using namespace System::Runtime::InteropServices;
	using namespace System::Security::Permissions;



	[DllImport("user32.dll",SetLastError=true)]
	bool RegisterHotKey(IntPtr hWnd,
		int    id,
		KeyModifiers fsModified,
		System::Windows::Forms::Keys vk);
	[DllImport("user32.dll",SetLastError=true)]
	bool UnregisterHotKey(IntPtr hWnd,int id);

	/// <summary>
	/// Summary for OsnMirrorCopyUI
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class OsnMirrorCopyUI : public System::Windows::Forms::Form
	{


	public: delegate void CreateCDPScheduleDelegate(String^ type,String^ para);
	public:
		OsnMirrorCopyUI(void)
		{
			InitializeComponent();
			//m_ListViewMutex=gcnew Mutex(false);
			this->KeyPreview=true;


			ImagePath = Application::StartupPath;
			myEventLog = gcnew CHostMirrorLog(ImagePath);

			InitializeMembers();

			RefreshStatusBar();


		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~OsnMirrorCopyUI()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  FileToolStripMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  AboutToolStripMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  RefreshToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SettingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AboutToolStripMenuItem;
	protected: 










	public:  String ^m_strOSVersion;
	public:  String ^ImagePath;
	public : String ^computerName;
	public:  String ^AdminName;
	public:  String ^AdminPass;
	public:  String ^MysqlAdmin;
	public:  String ^MysqlPass;
			 //public:  Mutex^ m_ListViewMutex;
	public:  CHostMirrorLog ^myEventLog;
	public:  IPAddress ^LocalIPAddress;
	public:	 IPHostEntry ^hostInfo;
	public:  INSTALLTYPE m_InstallType;
	public:  CMirrorInfoList ^pVolumeMirrorList;
	public:  CMirrorInfoList ^pDiskMirrorList;
	public:  CDriverInfoList ^pDriverList;
	public:  CDiskInfoList ^pDiskList;
	public:  String ^m_strVSSParaRegPath;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;

	private: System::Windows::Forms::ToolStripMenuItem^  DeleteMirror;
	private: System::Windows::Forms::ToolStripMenuItem^  Initialize;
	private: System::Windows::Forms::ToolStripMenuItem^  StopInitialize;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ToolStripMenuItem^  EditToolStripMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  NewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DeleteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  InitializeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  StopInitializeToolStripMenuItem;


	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ToolStripMenuItem^  HelpToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;

	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ImageList^  imageList1;
	private: System::Windows::Forms::ToolStrip^  ToolStrip1;
	private: System::Windows::Forms::ToolStripButton^  ToolStripRefreshButton;
	private: System::Windows::Forms::ToolStripButton^  ToolStripSettingButton;
	private: System::Windows::Forms::ToolStripButton^  ToolStripHelpButton;

	private: System::Windows::Forms::ContextMenuStrip^  NewMirror;
	private: System::Windows::Forms::ToolStripMenuItem^  NewMirrorMenuItem;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;

	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripMenuItem^  CreateCDPToolStripMenu;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  CreateCDPToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  CreateCDPScheduleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ViewCDPScheduleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DelCDPScheduleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  CreateCDPScheduleToolStripMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  ViewCDPScheduleToolStripMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  DelCDPScheuleToolStripMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  DatabaseConfigToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ClusterIPToolStripMenuItem;
	private: System::ComponentModel::BackgroundWorker^  backgroundCDPSchedule;
	private: System::ComponentModel::BackgroundWorker^  backgroundCreateCDP;
	private: System::ComponentModel::BackgroundWorker^  backgroundDBConf;
private: System::Windows::Forms::ToolStripMenuItem^  ToolStripResourceName;


private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
private: System::Windows::Forms::ToolStripMenuItem^  contextToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  NormalInitializeToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  NTFSInitializeToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  NormalInitialize;
private: System::Windows::Forms::ToolStripMenuItem^  NTFSInitialize;
private: System::Windows::Forms::ToolStripMenuItem^  transferModeToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  transferMode1ToolStripMenuItem;















	public: 
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::TreeNode^  treeNode1 = (gcnew System::Windows::Forms::TreeNode(L"本地卷", 1, 1));
			System::Windows::Forms::TreeNode^  treeNode2 = (gcnew System::Windows::Forms::TreeNode(L"本地磁盘", 2, 2));
			System::Windows::Forms::TreeNode^  treeNode3 = (gcnew System::Windows::Forms::TreeNode(L"镜像关系", 3, 3));
			System::Windows::Forms::TreeNode^  treeNode4 = (gcnew System::Windows::Forms::TreeNode(L"OSN Node", 0, 0, gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) {treeNode1, 
				treeNode2, treeNode3}));
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(OsnMirrorCopyUI::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->FileToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->RefreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SettingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DatabaseConfigToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClusterIPToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->EditToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DeleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->InitializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NormalInitializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NTFSInitializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->StopInitializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->transferModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->CreateCDPToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->CreateCDPScheduleToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ViewCDPScheduleToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DelCDPScheuleToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->ToolStripResourceName = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->transferMode1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AboutToolStripMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->HelpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->DeleteMirror = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Initialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NormalInitialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NTFSInitialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->StopInitialize = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->CreateCDPToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->CreateCDPScheduleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ViewCDPScheduleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DelCDPScheduleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->contextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->ToolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->ToolStripRefreshButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->ToolStripSettingButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->ToolStripHelpButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->NewMirror = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->NewMirrorMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundCDPSchedule = (gcnew System::ComponentModel::BackgroundWorker());
			this->backgroundCreateCDP = (gcnew System::ComponentModel::BackgroundWorker());
			this->backgroundDBConf = (gcnew System::ComponentModel::BackgroundWorker());
			this->menuStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->ToolStrip1->SuspendLayout();
			this->NewMirror->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->menuStrip1->AutoSize = false;
			this->menuStrip1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->menuStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->menuStrip1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->menuStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->FileToolStripMenu, 
				this->EditToolStripMenu, this->AboutToolStripMenu});
			this->menuStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->menuStrip1->Location = System::Drawing::Point(0, 1);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->menuStrip1->Size = System::Drawing::Size(734, 20);
			this->menuStrip1->Stretch = false;
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// FileToolStripMenu
			// 
			this->FileToolStripMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->RefreshToolStripMenuItem, 
				this->SettingToolStripMenuItem, this->DatabaseConfigToolStripMenuItem, this->ClusterIPToolStripMenuItem, this->ExitToolStripMenuItem});
			this->FileToolStripMenu->Name = L"FileToolStripMenu";
			this->FileToolStripMenu->Size = System::Drawing::Size(41, 16);
			this->FileToolStripMenu->Text = L"&系统";
			this->FileToolStripMenu->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::FileToolStripMenu_Click);
			// 
			// RefreshToolStripMenuItem
			// 
			this->RefreshToolStripMenuItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->RefreshToolStripMenuItem->Name = L"RefreshToolStripMenuItem";
			this->RefreshToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F5));
			this->RefreshToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->RefreshToolStripMenuItem->Text = L"刷新       ";
			this->RefreshToolStripMenuItem->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->RefreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::RefreshToolStripMenuItem_Click);
			// 
			// SettingToolStripMenuItem
			// 
			this->SettingToolStripMenuItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->SettingToolStripMenuItem->Name = L"SettingToolStripMenuItem";
			this->SettingToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F3));
			this->SettingToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->SettingToolStripMenuItem->Text = L"系统设置   ";
			this->SettingToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::SystemSetting_Click);
			// 
			// DatabaseConfigToolStripMenuItem
			// 
			this->DatabaseConfigToolStripMenuItem->Name = L"DatabaseConfigToolStripMenuItem";
			this->DatabaseConfigToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F4));
			this->DatabaseConfigToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->DatabaseConfigToolStripMenuItem->Text = L"数据库设置 ";
			this->DatabaseConfigToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::DatabaseConfigToolStripMenuItem_Click);
			// 
			// ClusterIPToolStripMenuItem
			// 
			this->ClusterIPToolStripMenuItem->Name = L"ClusterIPToolStripMenuItem";
			this->ClusterIPToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F6));
			this->ClusterIPToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->ClusterIPToolStripMenuItem->Text = L"集群IP设置";
			this->ClusterIPToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::ClusterIPToolStripMenuItem_Click);
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->ExitToolStripMenuItem->Text = L"退出";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::exitToolStripMenuItem_Click);
			// 
			// EditToolStripMenu
			// 
			this->EditToolStripMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->NewToolStripMenuItem, 
				this->DeleteToolStripMenuItem, this->InitializeToolStripMenuItem, this->StopInitializeToolStripMenuItem, this->transferModeToolStripMenuItem, 
				this->toolStripSeparator1, this->CreateCDPToolStripMenu, this->CreateCDPScheduleToolStripMenu, this->ViewCDPScheduleToolStripMenu, 
				this->DelCDPScheuleToolStripMenu, this->toolStripSeparator3, this->ToolStripResourceName, this->transferMode1ToolStripMenuItem});
			this->EditToolStripMenu->Name = L"EditToolStripMenu";
			this->EditToolStripMenu->Size = System::Drawing::Size(41, 16);
			this->EditToolStripMenu->Text = L"&编辑";
			// 
			// NewToolStripMenuItem
			// 
			this->NewToolStripMenuItem->Name = L"NewToolStripMenuItem";
			this->NewToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->NewToolStripMenuItem->Text = L"新建";
			this->NewToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::newMirrorToolStripMenuItem_Click);
			// 
			// DeleteToolStripMenuItem
			// 
			this->DeleteToolStripMenuItem->Name = L"DeleteToolStripMenuItem";
			this->DeleteToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->DeleteToolStripMenuItem->Text = L"删除";
			this->DeleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::deleteMirrorToolStripMenuItem_Click);
			// 
			// InitializeToolStripMenuItem
			// 
			this->InitializeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->NormalInitializeToolStripMenuItem, 
				this->NTFSInitializeToolStripMenuItem});
			this->InitializeToolStripMenuItem->Name = L"InitializeToolStripMenuItem";
			this->InitializeToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->InitializeToolStripMenuItem->Text = L"初始化";
			// 
			// NormalInitializeToolStripMenuItem
			// 
			this->NormalInitializeToolStripMenuItem->Name = L"NormalInitializeToolStripMenuItem";
			this->NormalInitializeToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->NormalInitializeToolStripMenuItem->Text = L"普通初始化";
			this->NormalInitializeToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::NormalInitializeToolStripMenuItem_Click);
			// 
			// NTFSInitializeToolStripMenuItem
			// 
			this->NTFSInitializeToolStripMenuItem->Name = L"NTFSInitializeToolStripMenuItem";
			this->NTFSInitializeToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->NTFSInitializeToolStripMenuItem->Text = L"自动精简初始化";
			this->NTFSInitializeToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::NTFSInitializeToolStripMenuItem_Click);
			// 
			// StopInitializeToolStripMenuItem
			// 
			this->StopInitializeToolStripMenuItem->Name = L"StopInitializeToolStripMenuItem";
			this->StopInitializeToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->StopInitializeToolStripMenuItem->Text = L"取消初始化";
			this->StopInitializeToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::stopInitializeToolStripMenuItem_Click);
			// 
			// transferModeToolStripMenuItem
			// 
			this->transferModeToolStripMenuItem->Name = L"transferModeToolStripMenuItem";
			this->transferModeToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->transferModeToolStripMenuItem->Text = L"切换传输方式";
			this->transferModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::transferModeToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(187, 6);
			// 
			// CreateCDPToolStripMenu
			// 
			this->CreateCDPToolStripMenu->Name = L"CreateCDPToolStripMenu";
			this->CreateCDPToolStripMenu->Size = System::Drawing::Size(190, 22);
			this->CreateCDPToolStripMenu->Text = L"新建CDP快照";
			this->CreateCDPToolStripMenu->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::CreateCDPToolStripMenuItem_Click);
			// 
			// CreateCDPScheduleToolStripMenu
			// 
			this->CreateCDPScheduleToolStripMenu->Name = L"CreateCDPScheduleToolStripMenu";
			this->CreateCDPScheduleToolStripMenu->Size = System::Drawing::Size(190, 22);
			this->CreateCDPScheduleToolStripMenu->Text = L"创建CDP任务计划";
			this->CreateCDPScheduleToolStripMenu->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::CreateCDPScheduleToolStripMenu_Click);
			// 
			// ViewCDPScheduleToolStripMenu
			// 
			this->ViewCDPScheduleToolStripMenu->Name = L"ViewCDPScheduleToolStripMenu";
			this->ViewCDPScheduleToolStripMenu->Size = System::Drawing::Size(190, 22);
			this->ViewCDPScheduleToolStripMenu->Text = L"查看CDP任务计划";
			this->ViewCDPScheduleToolStripMenu->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::ViewCDPScheduleToolStripMenu_Click);
			// 
			// DelCDPScheuleToolStripMenu
			// 
			this->DelCDPScheuleToolStripMenu->Name = L"DelCDPScheuleToolStripMenu";
			this->DelCDPScheuleToolStripMenu->Size = System::Drawing::Size(190, 22);
			this->DelCDPScheuleToolStripMenu->Text = L"删除CDP任务计划";
			this->DelCDPScheuleToolStripMenu->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::DelCDPScheuleToolStripMenu_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(187, 6);
			// 
			// ToolStripResourceName
			// 
			this->ToolStripResourceName->Name = L"ToolStripResourceName";
			this->ToolStripResourceName->Size = System::Drawing::Size(190, 22);
			this->ToolStripResourceName->Text = L"配置源盘集群资源名称";
			this->ToolStripResourceName->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::ToolStripResourceName_Click);
			// 
			// transferMode1ToolStripMenuItem
			// 
			this->transferMode1ToolStripMenuItem->Name = L"transferMode1ToolStripMenuItem";
			this->transferMode1ToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->transferMode1ToolStripMenuItem->Text = L"切换传输方式";
			this->transferMode1ToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::transferMode1ToolStripMenuItem_Click);
			// 
			// AboutToolStripMenu
			// 
			this->AboutToolStripMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->AboutToolStripMenuItem, 
				this->HelpToolStripMenuItem});
			this->AboutToolStripMenu->Name = L"AboutToolStripMenu";
			this->AboutToolStripMenu->Size = System::Drawing::Size(41, 16);
			this->AboutToolStripMenu->Text = L"&帮助";
			// 
			// AboutToolStripMenuItem
			// 
			this->AboutToolStripMenuItem->Name = L"AboutToolStripMenuItem";
			this->AboutToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->AboutToolStripMenuItem->Text = L"关于";
			this->AboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::aboutToolStripMenuItem_Click);
			// 
			// HelpToolStripMenuItem
			// 
			this->HelpToolStripMenuItem->Name = L"HelpToolStripMenuItem";
			this->HelpToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->HelpToolStripMenuItem->Text = L"帮助  F1";
			this->HelpToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::helpToolStripMenuItem_Click);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {this->DeleteMirror, 
				this->Initialize, this->StopInitialize,this->transferMode1ToolStripMenuItem, this->toolStripSeparator2, this->CreateCDPToolStripMenuItem, this->CreateCDPScheduleToolStripMenuItem, 
				this->ViewCDPScheduleToolStripMenuItem, this->DelCDPScheduleToolStripMenuItem, this->toolStripSeparator4, this->contextToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(191, 236);
			// 
			// DeleteMirror
			// 
			this->DeleteMirror->Name = L"DeleteMirror";
			this->DeleteMirror->Size = System::Drawing::Size(190, 22);
			this->DeleteMirror->Text = L"删除";
			this->DeleteMirror->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::deleteMirrorToolStripMenuItem_Click);
			// 
			// Initialize
			// 
			this->Initialize->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->NormalInitialize, 
				this->NTFSInitialize});
			this->Initialize->Name = L"Initialize";
			this->Initialize->Size = System::Drawing::Size(190, 22);
			this->Initialize->Text = L"初始化";
			// 
			// NormalInitialize
			// 
			this->NormalInitialize->Name = L"NormalInitialize";
			this->NormalInitialize->Size = System::Drawing::Size(160, 22);
			this->NormalInitialize->Text = L"普通初始化";
			this->NormalInitialize->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::NormalInitializeToolStripMenuItem_Click);
			// 
			// NTFSInitialize
			// 
			this->NTFSInitialize->Name = L"NTFSInitialize";
			this->NTFSInitialize->Size = System::Drawing::Size(160, 22);
			this->NTFSInitialize->Text = L"自动精简初始化";
			this->NTFSInitialize->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::NTFSInitializeToolStripMenuItem_Click);
			// 
			// StopInitialize
			// 
			this->StopInitialize->Name = L"StopInitialize";
			this->StopInitialize->Size = System::Drawing::Size(190, 22);
			this->StopInitialize->Text = L"停止初始化";
			this->StopInitialize->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::stopInitializeToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(187, 6);
			// 
			// CreateCDPToolStripMenuItem
			// 
			this->CreateCDPToolStripMenuItem->Name = L"CreateCDPToolStripMenuItem";
			this->CreateCDPToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->CreateCDPToolStripMenuItem->Text = L"新建CDP快照";
			this->CreateCDPToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::CreateCDPToolStripMenuItem_Click);
			// 
			// CreateCDPScheduleToolStripMenuItem
			// 
			this->CreateCDPScheduleToolStripMenuItem->Name = L"CreateCDPScheduleToolStripMenuItem";
			this->CreateCDPScheduleToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->CreateCDPScheduleToolStripMenuItem->Text = L"新建CDP任务计划";
			this->CreateCDPScheduleToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::CreateCDPScheduleToolStripMenu_Click);
			// 
			// ViewCDPScheduleToolStripMenuItem
			// 
			this->ViewCDPScheduleToolStripMenuItem->Name = L"ViewCDPScheduleToolStripMenuItem";
			this->ViewCDPScheduleToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->ViewCDPScheduleToolStripMenuItem->Text = L"查看CDP任务计划";
			this->ViewCDPScheduleToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::ViewCDPScheduleToolStripMenu_Click);
			// 
			// DelCDPScheduleToolStripMenuItem
			// 
			this->DelCDPScheduleToolStripMenuItem->Name = L"DelCDPScheduleToolStripMenuItem";
			this->DelCDPScheduleToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->DelCDPScheduleToolStripMenuItem->Text = L"删除CDP任务计划";
			this->DelCDPScheduleToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::DelCDPScheuleToolStripMenu_Click);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(187, 6);
			// 
			// contextToolStripMenuItem
			// 
			this->contextToolStripMenuItem->Name = L"contextToolStripMenuItem";
			this->contextToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->contextToolStripMenuItem->Text = L"配置源盘集群资源名称";
			this->contextToolStripMenuItem->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::ToolStripResourceName_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 30000;
			this->timer1->Tick += gcnew System::EventHandler(this, &OsnMirrorCopyUI::timer1_Tick);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->splitContainer1->ImeMode = System::Windows::Forms::ImeMode::On;
			this->splitContainer1->Location = System::Drawing::Point(0, 41);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->treeView1);
			this->splitContainer1->Panel1MinSize = 130;
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->listView1);
			this->splitContainer1->Panel2MinSize = 130;
			this->splitContainer1->Size = System::Drawing::Size(735, 281);
			this->splitContainer1->SplitterDistance = 158;
			this->splitContainer1->SplitterWidth = 2;
			this->splitContainer1->TabIndex = 5;
			this->splitContainer1->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &OsnMirrorCopyUI::splitContainer1_SplitterMoved);
			// 
			// treeView1
			// 
			this->treeView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->treeView1->ImageIndex = 0;
			this->treeView1->ImageList = this->imageList1;
			this->treeView1->Location = System::Drawing::Point(1, 1);
			this->treeView1->Name = L"treeView1";
			treeNode1->ImageIndex = 1;
			treeNode1->Name = L"Node1";
			treeNode1->SelectedImageIndex = 1;
			treeNode1->Text = L"本地卷";
			treeNode2->ImageIndex = 2;
			treeNode2->Name = L"Node0";
			treeNode2->SelectedImageIndex = 2;
			treeNode2->Text = L"本地磁盘";
			treeNode3->ImageIndex = 3;
			treeNode3->Name = L"Node3";
			treeNode3->SelectedImageIndex = 3;
			treeNode3->Text = L"镜像关系";
			treeNode4->ImageIndex = 0;
			treeNode4->Name = L"Disk";
			treeNode4->SelectedImageIndex = 0;
			treeNode4->Text = L"OSN Node";
			this->treeView1->Nodes->AddRange(gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) {treeNode4});
			this->treeView1->SelectedImageIndex = 0;
			this->treeView1->Size = System::Drawing::Size(157, 278);
			this->treeView1->TabIndex = 0;
			this->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &OsnMirrorCopyUI::treeView1_AfterSelect);
			// 
			// imageList1
			// 
			this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList1.ImageStream")));
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList1->Images->SetKeyName(0, L"computer.bmp");
			this->imageList1->Images->SetKeyName(1, L"volume.bmp");
			this->imageList1->Images->SetKeyName(2, L"disk.bmp");
			this->imageList1->Images->SetKeyName(3, L"mirror.bmp");
			// 
			// listView1
			// 
			this->listView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->listView1->FullRowSelect = true;
			this->listView1->Location = System::Drawing::Point(1, 1);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(577, 278);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			this->listView1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &OsnMirrorCopyUI::listView1_MouseDown);
			// 
			// statusStrip1
			// 
			this->statusStrip1->AllowMerge = false;
			this->statusStrip1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->statusStrip1->AutoSize = false;
			this->statusStrip1->BackColor = System::Drawing::SystemColors::Control;
			this->statusStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2, this->toolStripStatusLabel3});
			this->statusStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->statusStrip1->Location = System::Drawing::Point(1, 323);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->ShowItemToolTips = true;
			this->statusStrip1->Size = System::Drawing::Size(735, 18);
			this->statusStrip1->TabIndex = 4;
			this->statusStrip1->TabStop = true;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->AutoSize = false;
			this->toolStripStatusLabel1->BorderSides = System::Windows::Forms::ToolStripStatusLabelBorderSides::Right;
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(130, 13);
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->AutoSize = false;
			this->toolStripStatusLabel2->BorderSides = System::Windows::Forms::ToolStripStatusLabelBorderSides::Left;
			this->toolStripStatusLabel2->BorderStyle = System::Windows::Forms::Border3DStyle::RaisedOuter;
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(331, 14);
			// 
			// toolStripStatusLabel3
			// 
			this->toolStripStatusLabel3->BorderSides = System::Windows::Forms::ToolStripStatusLabelBorderSides::Left;
			this->toolStripStatusLabel3->BorderStyle = System::Windows::Forms::Border3DStyle::Bump;
			this->toolStripStatusLabel3->Name = L"toolStripStatusLabel3";
			this->toolStripStatusLabel3->Size = System::Drawing::Size(4, 13);
			this->toolStripStatusLabel3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// ToolStrip1
			// 
			this->ToolStrip1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ToolStrip1->AutoSize = false;
			this->ToolStrip1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->ToolStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->ToolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ToolStripRefreshButton, 
				this->ToolStripSettingButton, this->ToolStripHelpButton});
			this->ToolStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->ToolStrip1->Location = System::Drawing::Point(-1, 21);
			this->ToolStrip1->Name = L"ToolStrip1";
			this->ToolStrip1->Size = System::Drawing::Size(737, 22);
			this->ToolStrip1->TabIndex = 7;
			this->ToolStrip1->Text = L"toolStrip1";
			// 
			// ToolStripRefreshButton
			// 
			this->ToolStripRefreshButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->ToolStripRefreshButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ToolStripRefreshButton.Image")));
			this->ToolStripRefreshButton->ImageAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->ToolStripRefreshButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->ToolStripRefreshButton->Name = L"ToolStripRefreshButton";
			this->ToolStripRefreshButton->Size = System::Drawing::Size(23, 19);
			this->ToolStripRefreshButton->Text = L"刷新";
			this->ToolStripRefreshButton->TextDirection = System::Windows::Forms::ToolStripTextDirection::Horizontal;
			this->ToolStripRefreshButton->ToolTipText = L"刷新 Alt+F5";
			this->ToolStripRefreshButton->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::RefreshToolStripMenuItem_Click);
			// 
			// ToolStripSettingButton
			// 
			this->ToolStripSettingButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->ToolStripSettingButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ToolStripSettingButton.Image")));
			this->ToolStripSettingButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->ToolStripSettingButton->Name = L"ToolStripSettingButton";
			this->ToolStripSettingButton->Size = System::Drawing::Size(23, 19);
			this->ToolStripSettingButton->Text = L"系统设置";
			this->ToolStripSettingButton->ToolTipText = L"系统设置 Alt+F3";
			this->ToolStripSettingButton->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::SystemSetting_Click);
			// 
			// ToolStripHelpButton
			// 
			this->ToolStripHelpButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->ToolStripHelpButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ToolStripHelpButton.Image")));
			this->ToolStripHelpButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->ToolStripHelpButton->Name = L"ToolStripHelpButton";
			this->ToolStripHelpButton->Size = System::Drawing::Size(23, 19);
			this->ToolStripHelpButton->ToolTipText = L"帮助 F1";
			this->ToolStripHelpButton->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::helpToolStripMenuItem_Click);
			// 
			// NewMirror
			// 
			this->NewMirror->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->NewMirrorMenuItem});
			this->NewMirror->Name = L"NewMirror";
			this->NewMirror->Size = System::Drawing::Size(95, 26);
			this->NewMirror->Click += gcnew System::EventHandler(this, &OsnMirrorCopyUI::newMirrorToolStripMenuItem_Click);
			// 
			// NewMirrorMenuItem
			// 
			this->NewMirrorMenuItem->Name = L"NewMirrorMenuItem";
			this->NewMirrorMenuItem->Size = System::Drawing::Size(94, 22);
			this->NewMirrorMenuItem->Text = L"新建";
			// 
			// backgroundCDPSchedule
			// 
			this->backgroundCDPSchedule->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &OsnMirrorCopyUI::backgroundCDPSchedule_DoWork);
			this->backgroundCDPSchedule->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &OsnMirrorCopyUI::backgroundCDPSchedule_RunWorkerCompleted);
			// 
			// backgroundCreateCDP
			// 
			this->backgroundCreateCDP->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &OsnMirrorCopyUI::backgroundCreateCDP_DoWork);
			// 
			// backgroundDBConf
			// 
			this->backgroundDBConf->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &OsnMirrorCopyUI::backgroundDBConf_DoWork);
			// 
			// OsnMirrorCopyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ClientSize = System::Drawing::Size(735, 339);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->ToolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->IsMdiContainer = true;
			this->KeyPreview = true;
			this->Name = L"OsnMirrorCopyUI";
			this->ShowIcon = false;
			this->Text = L"复制管理软件";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &OsnMirrorCopyUI::OsnMirrorCopyUI_FormClosed);
			this->Load += gcnew System::EventHandler(this, &OsnMirrorCopyUI::OsnMirrorCopyUI_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &OsnMirrorCopyUI::OsnMirrorCopyUI_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ToolStrip1->ResumeLayout(false);
			this->ToolStrip1->PerformLayout();
			this->NewMirror->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
			 {
				 TreeNode ^CurrentNode = e->Node;

				 this->listView1->Items->Clear();
				 this->listView1->Columns->Clear();

				 if(CurrentNode== this->treeView1->Nodes[0])
				 {
					 RefreshComputerNode();

				 }
				 else if(CurrentNode == this->treeView1->Nodes[0]->Nodes[0]) // volume node
				 {
					 //RefreshDriverNode();
					 RefreshVolumeListView();
				 }
				 else if(CurrentNode == this->treeView1->Nodes[0]->Nodes[1]) // disk node
				 {
					 RefreshDiskListView();
				 }
				 else if(CurrentNode == this->treeView1->Nodes[0]->Nodes[2]) // mirror mode
				 {
					 RefreshMirrorNode();

				 }

			 }

	private:System::Void RefreshComputerNode()
		 {
			 /*String ^ComputerName = "ComputerName";
			 String ^IPAddress    =  "IP Address";*/

			 NewToolStripMenuItem->Enabled = false;
			 DeleteToolStripMenuItem->Enabled = false;
			 InitializeToolStripMenuItem->Enabled = false;
			 NormalInitializeToolStripMenuItem->Enabled = false;
			 NTFSInitializeToolStripMenuItem->Enabled = false;
			 StopInitializeToolStripMenuItem->Enabled = false;
			 CreateCDPToolStripMenu->Enabled = false;
			 CreateCDPScheduleToolStripMenu->Enabled = false;
			 ViewCDPScheduleToolStripMenu->Enabled = false;
			 DelCDPScheuleToolStripMenu->Enabled = false;
			 transferModeToolStripMenuItem->Enabled = false;

			 String ^ComputerName = "计算机名";
			 String ^IPAddressstr    =  "IP 地址";
			 // m_ListViewMutex->WaitOne();

			 this->listView1->Columns->Add(ComputerName,140,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(IPAddressstr,160,HorizontalAlignment::Left);

			 //this->listView1->Items->Add(computerName);
			 //this->listView1->Items[0]->SubItems->Add(this->LocalIPAddress->ToString());
			 int count=hostInfo->AddressList->Length;
			 for(int i=0;i<count;i++)
			 {
				 ListViewItem^ item=gcnew ListViewItem(computerName,0);
				 IPAddress^ ip=static_cast<IPAddress^>(hostInfo->AddressList->GetValue(i));
				 item->SubItems->Add(ip->ToString());
				 this->listView1->Items->Add(item);
			 }
			 // m_ListViewMutex->ReleaseMutex();





		 }


			void RefreshVolumeListView()
		 {
			 NewToolStripMenuItem->Enabled = true;
			 DeleteToolStripMenuItem->Enabled = false;
			 InitializeToolStripMenuItem->Enabled = false;
			 StopInitializeToolStripMenuItem->Enabled = false;
			 CreateCDPToolStripMenu->Enabled = false;
			 CreateCDPScheduleToolStripMenu->Enabled = false;
			 ViewCDPScheduleToolStripMenu->Enabled = false;
			 DelCDPScheuleToolStripMenu->Enabled = false;
			 transferModeToolStripMenuItem->Enabled = false;

			 // m_ListViewMutex->WaitOne();
			 this->listView1->Items->Clear();
			 this->listView1->Columns->Clear();

			 /* String ^DriverLabel = "DriveName";
			 String ^Signuare = "Signature";
			 String ^Size     = "Size(MB)";
			 String ^Property = "Property";*/

			 String ^DriverLabel = "驱动器";
			 String ^Signuare = "签名";
			 String ^Size     = "大小(MB)";
			 String ^Property = "属性";

			 this->listView1->Columns->Add(DriverLabel,80,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Signuare,160,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Size,100,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Property,100,HorizontalAlignment::Left);

			 int index=0;
			 for( int i=0;i<this->pDriverList->GetLength();i++)
			 {
				 CDriverInfo ^pDriverInfo =static_cast<CDriverInfo ^>(this->pDriverList->GetItem(i));

				 // 判断如果卷所在磁盘是镜像副本，那就过滤
				 if(pDiskList->HideDriver(pDriverInfo->m_GUID))
				 {
					 continue;
				 }

				 this->listView1->Items->Add(pDriverInfo->m_VolumeLable);
				 // String ^SignatureAndOffset = pDriverInfo->m_DiskSignature.ToString("X8");
				 // SignatureAndOffset = String::Concat(SignatureAndOffset,"_");
				 // SignatureAndOffset = String::Concat(SignatureAndOffset,pDriverInfo->m_StartOffset.ToString("X8"));
				 this->listView1->Items[index]->SubItems->Add(pDriverInfo->m_GUID);
				 this->listView1->Items[index]->SubItems->Add((pDriverInfo->m_DriverSize/(2*1024)).ToString());
				 if(pDriverInfo->m_Role ==Free)
					 this->listView1->Items[index]->SubItems->Add("");
				 else if(pDriverInfo->m_Role==Mirror_Source)
				 {
					 this->listView1->Items[index]->SubItems->Add("镜像源");
				 }
				 else 
				 {
					 this->listView1->Items[index]->SubItems->Add("副本");
				 }

				 index++;

			 }
			 // m_ListViewMutex->ReleaseMutex();

		 }

			void RefreshDiskListView()
		 {
			 NewToolStripMenuItem->Enabled = true;
			 DeleteToolStripMenuItem->Enabled = false;
			 InitializeToolStripMenuItem->Enabled = false;
			 NormalInitializeToolStripMenuItem->Enabled = false;
			 NTFSInitializeToolStripMenuItem->Enabled = false;
			 StopInitializeToolStripMenuItem->Enabled = false;
			 CreateCDPToolStripMenu->Enabled = false;
			 CreateCDPScheduleToolStripMenu->Enabled = false;
			 ViewCDPScheduleToolStripMenu->Enabled = false;
			 DelCDPScheuleToolStripMenu->Enabled = false;
			 transferModeToolStripMenuItem->Enabled = false;

			 // m_ListViewMutex->WaitOne();
			 this->listView1->Items->Clear();
			 this->listView1->Columns->Clear();

			 /*String ^DiskIndex = "DiskIndex";
			 String ^Signuare = "Signature";
			 String ^Size     = "Size(MB)";
			 String ^Property = "Property";*/

			 String ^DiskIndex = "磁盘索引";
			 String ^Signuare = "签名";
			 String ^Size     = "大小(MB)";
			 String ^Property = "属性";


			 this->listView1->Columns->Add(DiskIndex,80,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Signuare,160,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Size,100,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Property,100,HorizontalAlignment::Left);

			 for( int i=0;i<this->pDiskList->GetLength();i++)
			 {
				 CDiskInfo ^pDiskInfo =static_cast<CDiskInfo ^>(this->pDiskList->GetItem(i));

				 this->listView1->Items->Add(pDiskInfo->m_DiskIndex.ToString());
				 this->listView1->Items[i]->SubItems->Add(pDiskInfo->m_Guid);

				 this->listView1->Items[i]->SubItems->Add((pDiskInfo->m_DiskSize/2048).ToString());
				 if(pDiskInfo->m_Role ==Free)
					 this->listView1->Items[i]->SubItems->Add("");
				 else if(pDiskInfo->m_Role==Mirror_Source)
					 this->listView1->Items[i]->SubItems->Add("镜像源");
				 else 
					 this->listView1->Items[i]->SubItems->Add("副本");

			 }
			 // m_ListViewMutex->ReleaseMutex();

		 }

	private:System::Void RefreshDriverNode()
		 {

			 if(this->m_InstallType == VolumeCopy)
				 RefreshVolumeListView();
			 else 
				 RefreshDiskListView();

		 }


			int GetClusterActiveNode(String ^name)
			{
				
				int retvalue = 0;// o success,1 failed,2 cluster is failed
				LPCLUSTERVERSIONINFO lpClusterInfo;
				HCLUSENUM hclusenum =NULL;
				HRESOURCE hresource = NULL;
				HCLUSTER hcluster =NULL;
				WCHAR	clusterName[128] ;	
				WCHAR	nodeName[10][128];
				WCHAR	*pResourceName;
				DWORD	chClusterName =128*sizeof(WCHAR);
				DWORD	dwtype;
				DWORD	enumCount;
				DWORD	cchNodeName;
				DWORD	chNodeNameLen;
				LPWSTR  lpszGroupName;
				WCHAR	*pszNodeName;
				DWORD	lpcchGroupName;
				WCHAR *szbuffer;
				DWORD ret = 0;
				memset(clusterName,0,128*sizeof(WCHAR));
				for(int i = 0;i<10;i++)
				{
					memset(nodeName[i],0,128*sizeof(WCHAR));
				}
				DWORD nodeState = ClusterNodeStateUnknown;

				lpClusterInfo = (LPCLUSTERVERSIONINFO)malloc(sizeof(CLUSTERVERSIONINFO));
				try
				{
					hcluster = OpenCluster(NULL);//name is NULL ,through LPC get the local cluster handle
					if(hcluster == NULL)
					{
						free(lpClusterInfo);
						return  2;
					}
					lpClusterInfo->dwVersionInfoSize = sizeof(CLUSTERVERSIONINFO);
					ret = GetClusterInformation(hcluster,clusterName,&chClusterName,lpClusterInfo);
					if(ret !=ERROR_SUCCESS)
					{
						myEventLog->OSNWriteEventLog(String::Concat("GetClusterInformation,ERROR:",ret.ToString()),EventLogEntryType::Error,024);
						free(lpClusterInfo);
						CloseCluster(hcluster);
						return 2;
					}

					hclusenum = ClusterOpenEnum(hcluster,CLUSTER_ENUM_RESOURCE);
					if(!hclusenum)
					{
						DWORD dd=GetLastError();
						myEventLog->OSNWriteEventLog(String::Concat("ClusterOpenEnum,ERROR:",dd.ToString()),EventLogEntryType::Error,024);
						free(lpClusterInfo);
						CloseCluster(hcluster);
						return 2;
					}

					dwtype = CLUSTER_ENUM_RESOURCE;
					pResourceName = (LPWSTR)malloc(256*sizeof(WCHAR));
					szbuffer = (WCHAR *)malloc(256*sizeof(WCHAR));
					enumCount = ClusterGetEnumCount(hclusenum);
					for(DWORD i= 0 ;i<enumCount ;++i)
					{
						chNodeNameLen = 256*sizeof(WCHAR);
						memset(szbuffer,0,sizeof(szbuffer));

						ret = ClusterEnum(hclusenum,i,&dwtype,pResourceName,&chNodeNameLen);//get enum something
						if(ret == ERROR_SUCCESS)
						{
							String ^ ss = name;
							setlocale(LC_CTYPE, "chs");
							String ^dd = Marshal::PtrToStringUni((IntPtr)pResourceName);
							if(ss == dd )
							{
								hresource = OpenClusterResource(hcluster,pResourceName);
								pszNodeName = (LPWSTR)malloc(32);
								lpszGroupName = (LPWSTR)malloc(32);
								cchNodeName = 32;
								lpcchGroupName = 32;
								CLUSTER_RESOURCE_STATE  resState = GetClusterResourceState(hresource,pszNodeName,&cchNodeName,lpszGroupName,&lpcchGroupName);
								if(resState == ClusterResourceStateUnknown)
								{
									myEventLog->OSNWriteEventLog(String::Concat("gFailed to get res state!,errrorCode = ",GetLastError().ToString()),EventLogEntryType::Error,024);
								}
								else
								{

									String ^tempstr = Marshal::PtrToStringUni((IntPtr)pszNodeName);

									if(String::Compare(tempstr,computerName,true) == 0)
									{
										retvalue = 0;
										CloseClusterResource(hresource);
										setlocale(LC_CTYPE, "C");
										break;
									}
									else 
									{
										retvalue = 1;
										CloseClusterResource(hresource);
										setlocale(LC_CTYPE, "C");
										break;
									}
								}
							}
							setlocale(LC_CTYPE, "C");
						}
						else
						{
							myEventLog->OSNWriteEventLog(String::Concat("get ClusterEnum failed,errorCode = ",ret.ToString()),EventLogEntryType::Error,024);
							retvalue = 2;
							break;
						}

					}
				}catch(Exception^ exx)
				{
					myEventLog->OSNWriteEventLog(String::Concat("Getactivenode,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
				}		
				free(szbuffer);
				free(pResourceName);

				
				//CloseClusterNode(hnode);
				ClusterCloseEnum(hclusenum);
				CloseCluster(hcluster);
				free(lpClusterInfo);
				return retvalue;
			}
			void RefreshVolumeMirrorListView()
		 {
			 // this->listView1->Items->Clear();
			 //this->listView1->Columns->Clear();

			 /*String ^ Source ="SourceDrive";
			 String ^ Target ="TargetDrive";
			 String ^ State = "State";*/

			 /* String ^ Source ="源卷";
			 String ^ Target ="镜像卷";
			 String ^ State = "状态";*/


			 MIRROR_INFO MirrorInfo;
			 QUERY_MIRROR_STATE MirrorState;
			 //QUERY_MIRROR_STATE MirrorRemoteState;

			 /*this->listView1->Columns->Add(Source,120,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(Target,120,HorizontalAlignment::Left);
			 this->listView1->Columns->Add(State,140,HorizontalAlignment::Left);*/

			 // m_ListViewMutex->WaitOne();

			 for(int i=0;i<this->pVolumeMirrorList->GetLength();i++)
			 {
				 CMirrorInfo ^pMirrorInfo = static_cast<CMirrorInfo ^>(pVolumeMirrorList->GetItem(i));
				 CDriverInfo ^pSourceDriver = this->pDriverList->GetDriverInfo(pMirrorInfo->m_SourceGuid);
				 CDriverInfo ^pTargetDriver = this->pDriverList->GetDriverInfo(pMirrorInfo->m_TargetGuid);

				 // String ^SourceString = pMirrorInfo->m_SourceDiskSignature.ToString("X8");
				 // SourceString = String::Concat(SourceString,"_");
				 // SourceString = String::Concat(SourceString,pMirrorInfo->m_SourceOffset.ToString("X8"));

				 this->listView1->Items->Add(pMirrorInfo->m_SourceGuid);

				 //String ^TargetString = pMirrorInfo->m_TargetDiskSignature.ToString("X8");
				 //TargetString = String::Concat(TargetString,"_");
				 //TargetString = String::Concat(TargetString,pMirrorInfo->m_TargetOffset.ToString("X8"));

				 this->listView1->Items[i]->SubItems->Add(pMirrorInfo->m_TargetGuid);

				 if(pSourceDriver)
					 this->listView1->Items[i]->SubItems->Add(pSourceDriver->m_VolumeLable);
				 else
					 this->listView1->Items[i]->SubItems->Add("");

				 if(pTargetDriver)
					 this->listView1->Items[i]->SubItems->Add(pTargetDriver->m_VolumeLable);
				 else
					 this->listView1->Items[i]->SubItems->Add("");

				 this->listView1->Items[i]->SubItems->Add("卷复制");



				 memset(&MirrorInfo,0,sizeof(MIRROR_INFO));
				 memset(&MirrorState,0,sizeof(MirrorState));


				 GUID Srcguid0;
				 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
				 char* chguid = static_cast<char*>(p.ToPointer());			
				 OsnGUIDFromString(chguid,&Srcguid0);
				 Marshal::FreeHGlobal(p);
				 GUID Tgtguid0;

				 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
				 char* chguid0 = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid0,&Tgtguid0);
				 Marshal::FreeHGlobal(p1);


				 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
				 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset =pMirrorInfo->m_SourceOffset;
				 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
				 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset  =pMirrorInfo->m_TargetOffset;
				 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
				 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;



				 unsigned int ErrorCode = OsnGetMirrorState(&MirrorInfo,&MirrorState);		 
				 if(pMirrorInfo->m_IsCluster == 1)
				 {
					 int activeFlag = GetClusterActiveNode(pMirrorInfo->m_ClusterResourceName);
					 if(activeFlag == 0)
					 {
						 if(ErrorCode != 0)
						 {
							 //this->listView1->Items[i]->SubItems->Add("Unhealthy");
							 this->listView1->Items[i]->SubItems->Add("不正常");
							 myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",ErrorCode.ToString()),EventLogEntryType::Error,024);
						 }
						 else
						 {
							 if(MirrorState.m_State==DOWN)
							 {
								 //this->listView1->Items[i]->SubItems->Add("MirrorDown");
								 this->listView1->Items[i]->SubItems->Add("镜像Down");

							 }
							 else if(MirrorState.m_State==UP)
							 {
								 // this->listView1->Items[i]->SubItems->Add("MirrorUP");
								 this->listView1->Items[i]->SubItems->Add("正常");

							 }
							 else if(MirrorState.m_State==DIRTY)
							 {
								 //this->listView1->Items[i]->SubItems->Add("MirrorDirty");
								 this->listView1->Items[i]->SubItems->Add("数据不同步");
							 }
							 else if(MirrorState.m_State== MISSING)
							 {
								 //this->listView1->Items[i]->SubItems->Add("MirrorMissing");
								 this->listView1->Items[i]->SubItems->Add("镜像Down");

							 }
							 else
							 {
								 unsigned int Process=0;
								 ErrorCode = OsnGetRecoveryPercentage(&MirrorInfo,&Process);
								 if(ErrorCode!=0)
								 {
									 // this->listView1->Items[i]->SubItems->Add("Unhealthy");
									 this->listView1->Items[i]->SubItems->Add("不正常");
									 continue;
								 }

								 if(MirrorState.m_State==INIT)
								 {
									 // String ^ProcessPercentage = "Initialize... ";
									 String ^ProcessPercentage = "初始化...";
									 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
									 ProcessPercentage =String::Concat(ProcessPercentage,"%");

									 this->listView1->Items[i]->SubItems->Add(ProcessPercentage);
								 }
								 else
								 {
									 // String ^ProcessPercentage = "Recovery... ";
									 String ^ProcessPercentage;
									 if(SynOrAsyn(false,pMirrorInfo))
									 {
										 ProcessPercentage = "异步传输中...";
									 }
									 else
									 {
										 ProcessPercentage = "正在恢复...";
									 }

									 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
									 ProcessPercentage =String::Concat(ProcessPercentage,"%");

									 this->listView1->Items[i]->SubItems->Add(ProcessPercentage);

								 }

							 }
						 }
						 this->listView1->Items[i]->SubItems->Add("非活动");
					 }
					 else 
					 {
						 this->listView1->Items[i]->SubItems->Add("非活动");
						 this->listView1->Items[i]->SubItems->Add(" ");
						 /*ErrorCode = OsnGetVolumeClusterMirrorRemoteState(&MirrorInfo,&MirrorRemoteState);
						 if(ErrorCode !=0)
						 {
							 this->listView1->Items[i]->SubItems->Add("未获取到");
						 }
						 else
						 {
							 if(MirrorRemoteState.m_RemoteState == DOWN)
							 {
								 this->listView1->Items[i]->SubItems->Add("镜像Down");
							 }
							 else if(MirrorRemoteState.m_RemoteState == UP)
							 {
								 this->listView1->Items[i]->SubItems->Add("正常");
							 }
							 else if(MirrorRemoteState.m_RemoteState == DIRTY)
							 {
								 this->listView1->Items[i]->SubItems->Add("数据不同步");
							 }
							 else if(MirrorRemoteState.m_RemoteState == INIT)
							 {
								 this->listView1->Items[i]->SubItems->Add("初始化...");
							 }
							 else if(MirrorRemoteState.m_RemoteState == RECOVERY)
							 {
								 this->listView1->Items[i]->SubItems->Add("恢复...");
							 }
							 else if(MirrorRemoteState.m_RemoteState = MISSING)
							 {
								 this->listView1->Items[i]->SubItems->Add("镜像Down");
							 }
							 else if(MirrorRemoteState.m_RemoteState == NONE)
							 {
								 this->listView1->Items[i]->SubItems->Add("未获取到");
							 }
						 }*/

					 }
				 }
				 else
				 {
					 if(ErrorCode != 0)
					 {
						 //this->listView1->Items[i]->SubItems->Add("Unhealthy");
						 this->listView1->Items[i]->SubItems->Add("不正常");
						 myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",ErrorCode.ToString()),EventLogEntryType::Error,024);
					 }
					 else
					 {
						 if(MirrorState.m_State==DOWN)
						 {
							 //this->listView1->Items[i]->SubItems->Add("MirrorDown");
							 this->listView1->Items[i]->SubItems->Add("镜像Down");

						 }
						 else if(MirrorState.m_State==UP)
						 {
							 // this->listView1->Items[i]->SubItems->Add("MirrorUP");
							 this->listView1->Items[i]->SubItems->Add("正常");

						 }
						 else if(MirrorState.m_State==DIRTY)
						 {
							 //this->listView1->Items[i]->SubItems->Add("MirrorDirty");
							 this->listView1->Items[i]->SubItems->Add("数据不同步");
						 }
						 else if(MirrorState.m_State== MISSING)
						 {
							 //this->listView1->Items[i]->SubItems->Add("MirrorMissing");
							 this->listView1->Items[i]->SubItems->Add("镜像Down");

						 }
						 else
						 {
							 unsigned int Process=0;
							 ErrorCode = OsnGetRecoveryPercentage(&MirrorInfo,&Process);
							 if(ErrorCode!=0)
							 {
								 // this->listView1->Items[i]->SubItems->Add("Unhealthy");
								 this->listView1->Items[i]->SubItems->Add("不正常");
								 continue;
							 }

							 if(MirrorState.m_State==INIT)
							 {
								 // String ^ProcessPercentage = "Initialize... ";
								 String ^ProcessPercentage = "初始化...";
								 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
								 ProcessPercentage =String::Concat(ProcessPercentage,"%");

								 this->listView1->Items[i]->SubItems->Add(ProcessPercentage);
							 }
							 else
							 {
								 // String ^ProcessPercentage = "Recovery... ";
								 String ^ProcessPercentage;
								 if(SynOrAsyn(false,pMirrorInfo))
								 {
									 ProcessPercentage = "异步传输中...";
								 }
								 else
								 {
									 ProcessPercentage = "正在恢复...";
								 }

								 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
								 ProcessPercentage =String::Concat(ProcessPercentage,"%");

								 this->listView1->Items[i]->SubItems->Add(ProcessPercentage);

							 }

						 }
					 }
					 this->listView1->Items[i]->SubItems->Add("非集群模式");
				 }

				 if(pSourceDriver)
				 {
					 //Task ^t = GetTaskSchedule(pSourceDriver->m_VolumeLable->Substring(0,1)->ToUpper());	
					 String^ tempsignature=String::Concat(pSourceDriver->m_GUID,"#Volume");
					 if(IsSchedule(tempsignature))
					 {
						 this->listView1->Items[i]->SubItems->Add("Schedule");
					 }
					 else
					 {
						 this->listView1->Items[i]->SubItems->Add("Manual");
					 }
				 }
				 else
				 {
					 listView1->Items[i]->SubItems->Add(" ");
				 }

				 // display the system reboot
				 if(MirrorState.m_HealthyBoot)
				 {
					 listView1->Items[i]->SubItems->Add("Yes");
				 }
				 else
				 {
					 listView1->Items[i]->SubItems->Add("No");
				 }

				 String^ tranDiskroot = "System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\AsynVolume\\";
				 RegistryKey ^pRegKey = Registry::LocalMachine;
				 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey(tranDiskroot,true);
				 if(pParmeterKey != nullptr)
				 {
					String^trankey = String::Concat(String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),"Asyn");
					
					if(0 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
					{
						 listView1->Items[i]->SubItems->Add("同步传输");
					}
					else if(1 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
					{
						 listView1->Items[i]->SubItems->Add("异步传输");
					}
					pParmeterKey->Close();
				 }
				 pRegKey->Close();
			 }
			 // m_ListViewMutex->ReleaseMutex();

		 }
			void RefreshDiskMirrorListView()
		 {

			
			 MIRROR_INFO MirrorInfo;
			 QUERY_MIRROR_STATE MirrorState;
			 QUERY_MIRROR_STATE MirrorRemoteState;


			 int index = this->pVolumeMirrorList->GetLength();
			 // m_ListViewMutex->WaitOne();

			 for(int i=0;i<this->pDiskMirrorList->GetLength();i++)
			 {
				 CMirrorInfo ^pMirrorInfo = static_cast<CMirrorInfo ^>(pDiskMirrorList->GetItem(i));

				 //String ^SourceString = pMirrorInfo->m_SourceDiskSignature.ToString("X8");
				 CDiskInfo ^pSourceDisk = this->pDiskList->GetDiskInfo(pMirrorInfo->m_SourceGuid);
				 CDiskInfo ^pTargetDisk = this->pDiskList->GetDiskInfo(pMirrorInfo->m_TargetGuid);

				 this->listView1->Items->Add(pMirrorInfo->m_SourceGuid);

				 //String ^TargetString = pMirrorInfo->m_TargetDiskSignature.ToString("X8");

				 this->listView1->Items[i+index]->SubItems->Add(pMirrorInfo->m_TargetGuid);

				 if(pSourceDisk)
					 this->listView1->Items[i+index]->SubItems->Add(String::Concat("磁盘 ",pSourceDisk->m_DiskIndex.ToString()));
				 else
					 this->listView1->Items[i+index]->SubItems->Add("");

				 if(pTargetDisk)
					 this->listView1->Items[i+index]->SubItems->Add(String::Concat("磁盘 ",pTargetDisk->m_DiskIndex.ToString()));
				 else
					 this->listView1->Items[i+index]->SubItems->Add("");

				 this->listView1->Items[i+index]->SubItems->Add("磁盘复制");

				 memset(&MirrorInfo,0,sizeof(MIRROR_INFO));


				 GUID Srcguid0;
				 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
				 char* chguid = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid,&Srcguid0);
				 Marshal::FreeHGlobal(p);
				 GUID Tgtguid0;
				 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
				 char* chguid0 = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid0,&Tgtguid0);
				 Marshal::FreeHGlobal(p1);

				 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
				 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
				 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
				 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset =pMirrorInfo->m_SourceOffset;
				 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
				 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset  =pMirrorInfo->m_TargetOffset;

				 unsigned int ErrorCode = OsnGetDiskCopyMirrorState(&MirrorInfo,&MirrorState);
				 
				 if(pMirrorInfo->m_IsCluster == 1)
				 {
					 int activeFlag = GetClusterActiveNode(pMirrorInfo->m_ClusterResourceName);
					 if(activeFlag == 0)
					 {
						 if(ErrorCode != 0)
						 {
							 //this->listView1->Items[i+index]->SubItems->Add("Unhealthy");
							 this->listView1->Items[i+index]->SubItems->Add("不正常");
							 myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",ErrorCode.ToString()),EventLogEntryType::Error,024);
						 }
						 else
						 {
							 if(MirrorState.m_State==DOWN)
							 {
								 //this->listView1->Items[i+index]->SubItems->Add("MirrorDown");
								 this->listView1->Items[i+index]->SubItems->Add("镜像Down");

							 }
							 else if(MirrorState.m_State==UP)
							 {
								 //this->listView1->Items[i+index]->SubItems->Add("MirrorUP");
								 this->listView1->Items[i+index]->SubItems->Add("正常");
							 }
							 else if(MirrorState.m_State==DIRTY)
							 {
								 //this->listView1->Items[i+index]->SubItems->Add("MirrorDirty");
								 this->listView1->Items[i+index]->SubItems->Add("数据不同步");
							 }
							 else if(MirrorState.m_State == MISSING)
							 {
								 //this->listView1->Items[i+index]->SubItems->Add("MirrorMissing");
								 this->listView1->Items[i+index]->SubItems->Add("镜像Down");

							 }
							 else
							 {
								 unsigned int Process=0;
								 ErrorCode = OsnGetDiskCopyRecoveryPercentage(&MirrorInfo,&Process);
								 if(ErrorCode!=0)
								 {
									 //this->listView1->Items[i+index]->SubItems->Add("Unhealthy");
									 this->listView1->Items[i+index]->SubItems->Add("不正常");
									 continue;
								 }

								 if(MirrorState.m_State==INIT)
								 {
									 // String ^ProcessPercentage = "Initialize... ";
									 String ^ProcessPercentage = "初始化...";
									 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
									 ProcessPercentage =String::Concat(ProcessPercentage,"%");

									 this->listView1->Items[i+index]->SubItems->Add(ProcessPercentage);
								 }
								 else
								 {
									 //String ^ProcessPercentage = "Recovery... ";
									 String ^ProcessPercentage;
									 if(SynOrAsyn(true,pMirrorInfo))
									 {
										 ProcessPercentage = "异步传输中...";
									 }
									 else
									 {
										 ProcessPercentage = "正在恢复...";
									 }

									 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
									 ProcessPercentage =String::Concat(ProcessPercentage,"%");

									 this->listView1->Items[i+index]->SubItems->Add(ProcessPercentage);

								 }

							 }
						 }
						 this->listView1->Items[i+index]->SubItems->Add("非活动");
					 }
					 else 
					 {
						ErrorCode = OsnGetDiskClusterMirrorRemoteState(&MirrorInfo,&MirrorRemoteState);
						 this->listView1->Items[i+index]->SubItems->Add("非活动");
						 this->listView1->Items[i+index]->SubItems->Add(" ");
						/* if(ErrorCode !=0)
						 {
							 this->listView1->Items[i+index]->SubItems->Add("未获取到");
						 }
						 else
						 {
							 if(MirrorRemoteState.m_RemoteState == DOWN)
							 {
								 this->listView1->Items[i+index]->SubItems->Add("镜像Down");
							 }
							 else if(MirrorRemoteState.m_RemoteState == UP)
							 {
								 this->listView1->Items[i+index]->SubItems->Add("正常");
							 }
							 else if(MirrorRemoteState.m_RemoteState == DIRTY)
							 {
								  this->listView1->Items[i+index]->SubItems->Add("数据不同步");
							 }
							 else if(MirrorRemoteState.m_RemoteState == INIT)
							 {
								 this->listView1->Items[i+index]->SubItems->Add("初始化...");
							 }
							  else if(MirrorRemoteState.m_RemoteState == RECOVERY)
							 {
								 this->listView1->Items[i+index]->SubItems->Add("恢复...");
							 }
							  else if(MirrorRemoteState.m_RemoteState = MISSING)
							 {
								  this->listView1->Items[i+index]->SubItems->Add("镜像Down");
							 }
							  else if(MirrorRemoteState.m_RemoteState == NONE)
							  {
								  this->listView1->Items[i+index]->SubItems->Add("未获取到");
							  }
						 }*/
					 }
				 }
				 else
				 {
					 if(ErrorCode != 0)
					 {
						 //this->listView1->Items[i+index]->SubItems->Add("Unhealthy");
						 this->listView1->Items[i+index]->SubItems->Add("不正常");
						 myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",ErrorCode.ToString()),EventLogEntryType::Error,024);
					 }
					 else
					 {
						 if(MirrorState.m_State==DOWN)
						 {
							 //this->listView1->Items[i+index]->SubItems->Add("MirrorDown");
							 this->listView1->Items[i+index]->SubItems->Add("镜像Down");

						 }
						 else if(MirrorState.m_State==UP)
						 {
							 //this->listView1->Items[i+index]->SubItems->Add("MirrorUP");
							 this->listView1->Items[i+index]->SubItems->Add("正常");
						 }
						 else if(MirrorState.m_State==DIRTY)
						 {
							 //this->listView1->Items[i+index]->SubItems->Add("MirrorDirty");
							 this->listView1->Items[i+index]->SubItems->Add("数据不同步");
						 }
						 else if(MirrorState.m_State == MISSING)
						 {
							 //this->listView1->Items[i+index]->SubItems->Add("MirrorMissing");
							 this->listView1->Items[i+index]->SubItems->Add("镜像Down");

						 }
						 else
						 {
							 unsigned int Process=0;
							 ErrorCode = OsnGetDiskCopyRecoveryPercentage(&MirrorInfo,&Process);
							 if(ErrorCode!=0)
							 {
								 //this->listView1->Items[i+index]->SubItems->Add("Unhealthy");
								 this->listView1->Items[i+index]->SubItems->Add("不正常");
								 continue;
							 }

							 if(MirrorState.m_State==INIT)
							 {
								 // String ^ProcessPercentage = "Initialize... ";
								 String ^ProcessPercentage = "初始化...";
								 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
								 ProcessPercentage =String::Concat(ProcessPercentage,"%");

								 this->listView1->Items[i+index]->SubItems->Add(ProcessPercentage);
							 }
							 else
							 {
								 //String ^ProcessPercentage = "Recovery... ";
								 String ^ProcessPercentage;
								 if(SynOrAsyn(true,pMirrorInfo))
								 {
									 ProcessPercentage = "异步传输中...";
								 }
								 else
								 {
									 ProcessPercentage = "正在恢复...";
								 }
								 ProcessPercentage = String::Concat(ProcessPercentage,Process.ToString());
								 ProcessPercentage =String::Concat(ProcessPercentage,"%");

								 this->listView1->Items[i+index]->SubItems->Add(ProcessPercentage);

							 }

						 }

					 }
					 this->listView1->Items[i+index]->SubItems->Add("非集群模式");
				 }

				 if(pSourceDisk)
				 {

					 //Task ^t = GetTaskSchedule(pSourceDisk->m_DiskIndex.ToString());
					 String^ tempSignature;
					 tempSignature=String::Concat(pSourceDisk->m_Guid,"#Disk");
					 if(IsSchedule(tempSignature))
					 {
						 listView1->Items[i+index]->SubItems->Add("Schedule");
					 }
					 else
					 {
						 listView1->Items[i+index]->SubItems->Add("Manual");
					 }
				 }
				 else
				 {
					 listView1->Items[i+index]->SubItems->Add(" ");
				 }

				 // display the system reboot
				 if(MirrorState.m_HealthyBoot)
				 {
					 listView1->Items[i+index]->SubItems->Add("Yes");

				 }
				 else
				 {
					 listView1->Items[i+index]->SubItems->Add("No");
				 }

				 String^ tranDiskroot = "System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\AsynDisk\\";
				 RegistryKey ^pRegKey = Registry::LocalMachine;
				 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey(tranDiskroot,true);
				 if(pParmeterKey != nullptr)
				 {
					String^trankey = String::Concat(String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),"Asyn");
					
					if(0 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
					{
						 listView1->Items[i+index]->SubItems->Add("同步传输");
					}
					else if(1 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
					{
						 listView1->Items[i+index]->SubItems->Add("异步传输");
					}
					pParmeterKey->Close();
				 }
				 pRegKey->Close();
			 }
			 // m_ListViewMutex->ReleaseMutex();

		 }

	private: System::Void RefreshMirrorNode()
			 {
				 NewToolStripMenuItem->Enabled = true;
				 DeleteToolStripMenuItem->Enabled = true;
				 InitializeToolStripMenuItem->Enabled = true;
				 NormalInitializeToolStripMenuItem->Enabled = true;
				 NTFSInitializeToolStripMenuItem->Enabled = true;
				 StopInitializeToolStripMenuItem->Enabled = true;
				 CreateCDPToolStripMenu->Enabled = true;
				 CreateCDPScheduleToolStripMenu->Enabled = true;
				 ViewCDPScheduleToolStripMenu->Enabled = true;
				 DelCDPScheuleToolStripMenu->Enabled = true;
				 transferModeToolStripMenuItem->Enabled = true;

				 this->listView1->Items->Clear();
				 this->listView1->Columns->Clear();

				 String ^ Source ="源签名";
				 String ^ SourceLabel ="镜像源";
				 String ^ Target = "镜像签名";
				 String ^ TargetLabel ="镜像";
				 String ^ State = "状态";
				 String ^RemoteState="远程状态";
				 String ^ Property ="属性";
				 String ^ CDPMode = "CDP方式";
				 String ^ strRebootFlag="正常启动";
				 String ^ transferMode="传输方式";

				 this->listView1->Columns->Add(Source,110,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(Target,115,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(SourceLabel,60,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(TargetLabel,50,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(Property,60,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(State,85,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(RemoteState,85,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(CDPMode,85,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(strRebootFlag,85,HorizontalAlignment::Left);
				 this->listView1->Columns->Add(transferMode,85,HorizontalAlignment::Left);

				 RefreshVolumeMirrorListView();
				 RefreshDiskMirrorListView();


			 }

	private:System::Void InitializeMembers()
			{
				this->computerName = System::Windows::Forms::SystemInformation::ComputerName;
				this->m_strVSSParaRegPath = L"System\\CurrentControlSet\\Services\\OsnVSS\\Parameters";

				try
				{
					m_strOSVersion = GetCurrentOSVersion();
					//IPHostEntry ^hostInfo=System::Net::Dns::GetHostEntry(computerName);
					// set the working ip
					hostInfo=System::Net::Dns::Resolve(computerName);
					this->LocalIPAddress=static_cast<IPAddress ^>(hostInfo->AddressList->GetValue(0));

				}
				catch(Exception^ exx)
				{
					MessageBox::Show(String::Concat("初始化时出现异常：",exx->Message->ToString()));
				}

				this->treeView1->Nodes[0]->Text = computerName;

				this->pVolumeMirrorList = gcnew CMirrorInfoList(128);
				this->pDiskMirrorList = gcnew CMirrorInfoList(128);
				this->pDriverList = gcnew CDriverInfoList(256);
				this->pDiskList = gcnew CDiskInfoList(256);


				ReadInstallTypeFromRegistry();
				CheckRegistryKey();


				GetSystemDriversInfo();
				GetSystemDisksInfo();

				ReadConfigurationFile();
				try
				{
					GetSystemMirrorInfo();
					this->treeView1->SelectedNode=this->treeView1->Nodes[0];
					RefreshComputerNode();
				}
				catch(Exception^ exx)
				{
					myEventLog->OSNWriteEventLog(String::Concat("获取基本信息时出现异常：",exx->Message->ToString()),EventLogEntryType::Error,024);
				}

			}
	private: void ReadConfigurationFile()
			 {
				 String ^strLine = nullptr;
				 array<String^>^split= nullptr;
				 array<Char>^chars = {';'};

				 //read initiator information info
				 String ^strFileName = String::Concat(ImagePath, L"\\OSNHostMirror.cfg");
				 FileInfo ^fiHost = gcnew FileInfo(strFileName);
				 if(!fiHost->Exists)
				 {
					 return;
				 }

				 //String ^strMsg = "Illegal line found in Configuration file.";

				 StreamReader ^srHost = fiHost->OpenText();
				 while((strLine = srHost->ReadLine()))
				 {
					 if(strLine->StartsWith("DiskMirrorInfo"))
					 {
						 //DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
						 strLine = strLine->Substring(strlen("DiskMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;
						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode = 2;
						 unsigned int isCluster=0;
						 String ^clusterResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster=Convert::ToInt32(s);
					   }
					   else if(i == 4)
					   {
						   clusterResourceName = s;
					   }
					   ++i;
				   }

						 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
						 this->pDiskMirrorList->AddItem(pMirrorInfo);
					 }

					 if(strLine->StartsWith("VolumeMirrorInfo"))
					 {
						 //VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
						 strLine = strLine->Substring(strlen("VolumeMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;
						 //unsigned int uiSrcDiskSignature = 0;
						 //unsigned int uiSrcDiskOffset = 0;
						 //unsigned int uiDesDiskSignature = 0;
						 //unsigned int uiDesDiskOffset = 0;
						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode = 2;
						 unsigned int isCluster = 0;
						 String		^clusterResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid =s; //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   /*else if(i == 1)
					   {
					   uiSrcDiskOffset = UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }*/
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   /* else if(i == 3)
					   {
					   uiDesDiskOffset = UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }*/
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster = Convert::ToInt32(s);
					   }
					   else if(i == 4)
					   {
						   clusterResourceName = s;
					   }
					   ++i;
				   }

						 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(SrcGuid,DesGuid,uiEIMMode,isCluster,clusterResourceName);
						 this->pVolumeMirrorList->AddItem(pMirrorInfo);
					 }


				 }
				 srHost->Close();
			 }


	private: void WriteConfigurationFile()
			 {
				 try
				 {
					 FileInfo ^fi = gcnew FileInfo(String::Concat(ImagePath, L"\\OSNHostMirror.cfg"));
					 if(!fi->Exists)
					 {
						 return;
					 }

					 fi->CopyTo(String::Concat(ImagePath, L"\\OSNHostMirror.cfg.bak"), true);

					 FileStream	^fs = fi->Open(FileMode::Create); 
					 StreamWriter	^sw = gcnew StreamWriter(fs); 
					 sw->AutoFlush = true;

					 int i;
					 for(i=0; i<this->pDiskMirrorList->GetLength(); i++)
					 {
						 CMirrorInfo ^pMirrorInfo = static_cast<CMirrorInfo ^>(pDiskMirrorList->GetItem(i));
						 /*sw->WriteLine("DiskMirrorInfo={0:G};{1:G};{3:G}", pMirrorInfo->m_SourceDiskSignature.ToString("X8"),
						 pMirrorInfo->m_TargetDiskSignature.ToString("X8"), pMirrorInfo->m_EIMMode.ToString());*/
						 String ^strLine = String::Concat("DiskMirrorInfo=", pMirrorInfo->m_SourceGuid, ";");
						 strLine = String::Concat(strLine, pMirrorInfo->m_TargetGuid, ";");
						 strLine = String::Concat(strLine, pMirrorInfo->m_EIMMode.ToString(),";");
						 strLine = String::Concat(strLine,pMirrorInfo->m_IsCluster.ToString(),";");
						 strLine = String::Concat(strLine,pMirrorInfo->m_ClusterResourceName);
						 sw->WriteLine(strLine);
					 }

					 for(i=0; i<this->pVolumeMirrorList->GetLength(); i++)
					 {
						 CMirrorInfo ^pMirrorInfo = static_cast<CMirrorInfo^>(pVolumeMirrorList->GetItem(i));

						 String ^strLine = String::Concat("VolumeMirrorInfo=", pMirrorInfo->m_SourceGuid, ";");
						 //strLine = String::Concat(strLine, pMirrorInfo->m_SourceOffset.ToString("X8"), ";");
						 strLine = String::Concat(strLine, pMirrorInfo->m_TargetGuid, ";");
						 //strLine = String::Concat(strLine, pMirrorInfo->m_TargetOffset.ToString("X8"), ";");
						 strLine = String::Concat(strLine, pMirrorInfo->m_EIMMode.ToString(),";");
						 strLine = String::Concat(strLine,pMirrorInfo->m_IsCluster.ToString(),";");
						 strLine = String::Concat(strLine,pMirrorInfo->m_ClusterResourceName);
						 sw->WriteLine(strLine);
						 /*sw->WriteLine("VolumeMirrorInfo={0:G};{1:G};{2:G};{3:G};{4:G}", pMirrorInfo->m_SourceDiskSignature.ToString("X8"),
						 pMirrorInfo->m_SourceOffset.ToString("X8"), pMirrorInfo->m_TargetDiskSignature.ToString("X8"), 
						 pMirrorInfo->m_TargetOffset.ToString("X8"), pMirrorInfo->m_EIMMode.ToString());*/
					 }
					 sw->Close();
				 }
				 catch(Exception^ exx)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("WriteConfigurationFile,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
				 }

			 }
			 bool IsSchedule(String^ guid)
			 {
				 String^ xmlFileName = String::Concat(AppDomain::CurrentDomain->BaseDirectory,"\\HostMirrorCDPSchedule.xml");
				 XmlDocument^ xmlDoc = gcnew XmlDocument();
				 if(File::Exists(xmlFileName)==true)
				 {
					 xmlDoc->Load(xmlFileName);
					 String^ strExp = String::Concat("//CHostMirrorCDPSchedule[m_Guid='",guid,"']");
					 XmlNode^ xNode = xmlDoc->SelectSingleNode(strExp);
					 if(xNode)
					 {
						 return true;
					 }
				 }
				 return false;
			 }
			 /*
			 private: Task^ GetTaskSchedule(String ^strSrcVolume)
			 {
			 Task ^t = nullptr;
			 t = OSNTaskSchedule::GetTaskByTaskName(strSrcVolume);
			 if(t)
			 {
			 t->Close();
			 }
			 return t;
			 }*/
	private: String^ GetCurrentOSVersion()
			 {
				 String^ strOSVersion = "";
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
					 MessageBox::Show(ex->ToString());
				 }
				 return strOSVersion;
			 }
			 void GetSystemDisksInfo()
			 {


				 this->pDiskList ->Clear();
				 /* Object	^object;


				 // clear the previous list

				 try{

				 ManagementObjectSearcher ^query = gcnew ManagementObjectSearcher(
				 "SELECT * FROM Win32_DiskDrive");
				 ManagementObjectCollection ^queryCollection = query->Get();
				 IEnumerator	^loopMC = queryCollection->GetEnumerator();
				 while(loopMC->MoveNext())
				 {
				 ManagementObject ^mo = static_cast<ManagementObject ^> (loopMC->Current);
				 object		= mo->GetPropertyValue("Index");
				 unsigned int Index =System::Convert::ToUInt32(object);

				 object = mo->GetPropertyValue("Signature");
				 unsigned int Signature = System::Convert::ToUInt32(object);

				 object = mo->GetPropertyValue("Size");
				 unsigned __int64 Size = System::Convert::ToUInt64(object);

				 Size = Size/BLOCK_SIZE; // Convert to blocks

				 unsigned int UseSize = this->pDriverList->GetBlocksBySignature(Signature);

				 CDiskInfo ^pDisk = gcnew CDiskInfo(Index,Signature,Size,(Size-UseSize));
				 if(pDiskList->GetDiskInfo(Signature)==nullptr)
				 pDiskList->AddItem(pDisk);


				 }
				 }
				 catch(...)
				 {
				 MessageBox::Show("获取磁盘出现异常");
				 }*/

				 //get system disks

				 try
				 {
					 DISK_INFO pDisk;
					 int ErrorCode=0;
					 for(int i=0;i<64;i++)
					 {
						 memset(&pDisk,0,sizeof(DISK_INFO));
						 pDisk.m_DiskIndex=i;
						 ErrorCode=OsnGetDisk(&pDisk);
						 if(ErrorCode==0)
						 {     
							 /*if(pDisk.m_DiskSignature==0||pDisk.m_DiskSize==0)
							 continue;*/
							 String^ chguid=nullptr;
							 char guid[128];
							 OsnGUIDToString(guid,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
							 chguid=gcnew String(guid);
							 unsigned __int64 UseSize = this->pDriverList->GetBlocksBySignature(chguid);

							 CDiskInfo ^pNewDisk = gcnew CDiskInfo(pDisk.m_DiskIndex,chguid,pDisk.m_DiskSize,(pDisk.m_DiskSize-UseSize));
							 if(pNewDisk!=nullptr)
							 {
								 if(pDiskList->GetDiskInfo(pNewDisk->m_Guid)==nullptr ||pDiskList->GetDiskByIndex(pNewDisk->m_DiskIndex) == nullptr)
									 pDiskList->AddItem(pNewDisk);
							 }
						 }
					 }
				 }
				 catch(Exception^ exx)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("GetSystemDisksInfo,ERROR:",exx->Message->ToString()),EventLogEntryType::Error,024);
				 }


			 }

			 void GetSystemDriversInfo()
			 {

				 Object	^object;
				 unsigned int ErrorCode;
				 unsigned __int64		size;

				 // clear the previous list
				 try{

					 ManagementObjectSearcher ^query = gcnew ManagementObjectSearcher(
						 "SELECT * FROM Win32_LogicalDisk WHERE DriveType=3");
					 ManagementObjectCollection ^queryCollection = query->Get();
					 System::Collections::IEnumerator	^loopMC = queryCollection->GetEnumerator();
					 while(loopMC->MoveNext())
					 {
						 ManagementObject ^mo = static_cast<ManagementObject ^> (loopMC->Current);
						 object		= mo->GetPropertyValue("Name");
						 size      =Convert::ToUInt64(mo->GetPropertyValue("Size"));

						 String ^VolumeLabel	= static_cast<String ^> (object);	//C:, D:, E:, etc.

						 unsigned int	signature = 0;	
						 unsigned int	startingOffset = 0;

						 Char Name=VolumeLabel[0];
						 /*int ret=OSNGetVolumeSize(Name,size);
						 if(0!=ret)
						 {
						 myEventLog->OsnHostMirrorLog("获取到的volume的size出现错误");
						 }
						 else
						 {
						 //size=(unsigned __int64)(tempsize);
						 myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume:",VolumeLabel,"的size=",Convert::ToString(size)));
						 }*/

						 ErrorCode = OSNGetVolumeID(Name, signature, startingOffset);
						 if(ErrorCode==0)
						 {
							 //MessageBox::Show(String::Concat("签名：",signature.ToString("X8"),"偏移量：",startingOffset.ToString("X8")));
							 //ErrorCode = OsnVolumeCopyGetVolumeSize(signature, startingOffset, &size);
							 //if(ErrorCode ==0)
							 //{
							 size=0;
							 char char_guid[128];
							 bool ret=OsnCHeckIsGPTVolume((const char)VolumeLabel[0],char_guid);
							 String^ guid=nullptr;
							 if(ret)
							 {
 								 guid=gcnew String(char_guid);
								 GUID guid0;
								 IntPtr p = Marshal::StringToHGlobalAnsi(guid);
								 char* chguid = static_cast<char*>(p.ToPointer());
								 OsnGUIDFromString(chguid,&guid0);
								 Marshal::FreeHGlobal(p);
								 int ret=OsnVolumeCopyGetVolumeSize(guid0,size);
								 if(0!=ret)
								 {
									 myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume的size出现错误,errorcode=",ret.ToString()));
								 }
								 else
								 {
									 myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume:",VolumeLabel,"的size=",Convert::ToString(size)));
								 }
								
								 //return guid;
							 }
							 else
							 {
								 VOLUMEID volume;
								 volume.MBP_VolumeID.m_BlockOffset=startingOffset;
								 volume.MBP_VolumeID.m_DiskSignature=signature;
								 volume.MBP_VolumeID.m_NotUsed1=0;
								 OsnGUIDToString(char_guid,volume.SAN_VolumeID.m_VolumeGuid);
								 guid=gcnew String(char_guid);		

								 int ret=OsnVolumeCopyGetVolumeSize(volume.SAN_VolumeID.m_VolumeGuid,size);
								 if(0!=ret)
								 {
									 myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume的size出现错误,errorcode=",ret.ToString()));
								 }
								 else
								 {
									 myEventLog->OsnHostMirrorLog(String::Concat("获取到的volume:",VolumeLabel,"的size=",Convert::ToString(size)));
								 }
							 }

							 CDriverInfo	^pDriverInfo = gcnew CDriverInfo(VolumeLabel, size, guid);
							 if(this->pDriverList->GetDriverInfo(guid)== nullptr)
								 this->pDriverList->AddItem(pDriverInfo);
							 //}
							 /*else
							 {
							 MessageBox::Show(String::Concat("获取卷容量异常，出错代码",ErrorCode.ToString()));
							 }*/
						 }
						 /*else
						 {
						 MessageBox::Show(String::Concat("获取卷签名异常,出错代码",ErrorCode.ToString()));
						 }*/
					 }
				 }
				 catch(Exception ^e)
				 {
					 MessageBox::Show("获取分区出异常.");
					 MessageBox::Show(e->Message);
				 }

			 }

			 int NewVolumeMirror()
			 {
				 try
				 {
					 if(this->listView1->SelectedItems->Count!=1)
					 {
						 MessageBox::Show("请选择驱动器");
						 return 1;
					 }

					 ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

					 String ^guid  = SelectItem->SubItems[1]->Text;

					 CDriverInfo ^pSelectDriver = this->pDriverList->GetDriverInfoByString(guid);
					 if(pSelectDriver==nullptr)
					 {
						 MessageBox::Show("无法获取驱动器信息");
						 return 1;
					 }

					 if(pSelectDriver->m_Role!=Free)
					 {
						 MessageBox::Show("当前驱动器已经有镜像");
						 return 1;
					 }

					 CNewMirror ^pNewMirror = gcnew CNewMirror(pSelectDriver,
						 this->pDriverList,
						 VolumeCopy,
						 this->pDiskList);
					 if(pNewMirror->ShowDialog()!=System::Windows::Forms::DialogResult::Yes)
					 { 
						 return 1;
					 }

					 MIRROR_INFO MirrorInfo;


					 GUID Srcguid0;
					 IntPtr p = Marshal::StringToHGlobalAnsi(pNewMirror->pSourceDriver->m_GUID);
					 char* chguid = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid,&Srcguid0);
					 Marshal::FreeHGlobal(p);
					 GUID Tgtguid0;
					 IntPtr p1 = Marshal::StringToHGlobalAnsi(pNewMirror->pTargetDriver->m_GUID);
					 char* chguid0 = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid0,&Tgtguid0);
					 Marshal::FreeHGlobal(p1);

					 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pNewMirror->pSourceDriver->m_DiskSignature;
					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset = pNewMirror->pSourceDriver->m_StartOffset;

					 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature= pNewMirror->pTargetDriver->m_DiskSignature;
					 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset = pNewMirror->pTargetDriver->m_StartOffset;
					 unsigned int ErrorCode;
					 if(pNewMirror->m_IsCluster==0)
					 {
						 ErrorCode  =  OsnSetVolumeCopy(&MirrorInfo);
					 }
					 else
					 {
						 ErrorCode = OsnSetClusterVolumeCopy(&MirrorInfo);
					 }
					 if(ErrorCode!=0)
					 {
						 MessageBox::Show(String::Concat("设置镜像出错,代码=",ErrorCode.ToString()));
						 return 1;
					 }

					 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(pNewMirror->pSourceDriver->m_GUID,
						 pNewMirror->pTargetDriver->m_GUID,pNewMirror->m_EIMMode,
						 pNewMirror->m_IsCluster,pNewMirror->m_ClusterResourceName);

					 pNewMirror->pSourceDriver->m_Role = Mirror_Source;
					 pNewMirror->pTargetDriver->m_Role = Mirror_Target;

					 //String ^pSourceString = pNewMirror->pSourceDriver->m_DiskSignature.ToString("X8");
					 //pSourceString = String::Concat(pSourceString,"_");
					 //pSourceString = String ::Concat(pSourceString,pNewMirror->pSourceDriver->m_StartOffset.ToString("X8"));

					 // String ^pTargetString = pNewMirror->pTargetDriver->m_DiskSignature.ToString("X8");
					 // pTargetString = String::Concat(pTargetString,"_");
					 // pTargetString = String ::Concat(pTargetString,pNewMirror->pTargetDriver->m_StartOffset.ToString("X8"));


					 if(pNewMirror->m_IsCluster==0)
					 {
						 ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pNewMirror->pSourceDriver->m_GUID,"}"),String::Concat("{",pNewMirror->pTargetDriver->m_GUID,"}"),true,2,0);
					 }
					 else
					 {
						 ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pNewMirror->pSourceDriver->m_GUID,"}"),String::Concat("{",pNewMirror->pTargetDriver->m_GUID,"}"),true,2,1);
						 if(ErrorCode==0)
						 ErrorCode = SetClusterKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pNewMirror->pSourceDriver->m_GUID,"}"),true,DIRTY,DIRTY);
					 }
					 if(ErrorCode!=0)
					 {
						 MessageBox::Show("设置注册表失败.");
						 OsnRemoveVolumeCopy(&MirrorInfo);
						 return 1;
					 }

					 this->pVolumeMirrorList->AddItem(pMirrorInfo);
					 WriteConfigurationFile();

					 return 0;
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("新建镜像过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("新建镜像过程中出现异常，异常信息：",exx->Message->ToString()));
				 }
				 return 1;

			 }

			 int NewDiskMirror()
			 {

				 try
				 {
					 if(this->listView1->SelectedItems->Count!=1)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("请选择驱动器.");
						 box->ShowDialog();
						 return 1;
					 }

					 ListViewItem ^SelectItem =nullptr;
					 try
					 {
						 SelectItem=this->listView1->SelectedItems[0];
					 }
					 catch(...)
					 {
						 MessageBox::Show("您未选中镜像关系，请选择镜像关系");
						 return 0;
					 }

					 String ^guid  = SelectItem->SubItems[1]->Text;

					 CDiskInfo ^pSelectDisk = this->pDiskList->GetDiskInfo(guid);
					 if(pSelectDisk==nullptr)
					 {
						 MessageBox::Show("无法获取驱动器信息");
						 return 1;
					 }

					 if(pSelectDisk->m_Role!=Free)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("当前驱动器已经有镜像.");
						 box->ShowDialog();
						 return 1;
					 }

					 CNewMirror ^pNewMirror = gcnew CNewMirror(pSelectDisk,this->pDiskList,pDriverList);
					 if(pNewMirror->ShowDialog()!=System::Windows::Forms::DialogResult::Yes)
					 { 
						 return 1;
					 }

					 MIRROR_INFO MirrorInfo;

					 try
					 {
						 GUID Srcguid0;
						 IntPtr p = Marshal::StringToHGlobalAnsi(pNewMirror->pSourceDisk->m_Guid);
						 char* chguid = static_cast<char*>(p.ToPointer());
						 OsnGUIDFromString(chguid,&Srcguid0);
						 Marshal::FreeHGlobal(p);
						 GUID Tgtguid0;
						 IntPtr p1 = Marshal::StringToHGlobalAnsi(pNewMirror->pTargetDisk->m_Guid);
						 char* chguid0 = static_cast<char*>(p.ToPointer());
						 OsnGUIDFromString(chguid0,&Tgtguid0);
						 Marshal::FreeHGlobal(p1);

						 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
						 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
					 }
					 catch(Exception^ exx)
					 {
						 MessageBox::Show(String::Concat("new disk mirror error:",exx->Message->ToString()));
						 return 1;
					 }

					  String^  ddGuid = "00000000-0000-0000-0000-000000000000";
					  if(pNewMirror->pSourceDisk->m_Guid == ddGuid ||
						  pNewMirror->pTargetDisk->m_Guid == ddGuid)
					  {
						  MessageBox::Show("源盘或目标盘没有初始化，请初始化！");
						  return 1;
					  }

					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pNewMirror->pSourceDisk->m_DiskSignature;
					 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset = 0;

					 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature= pNewMirror->pTargetDisk->m_DiskSignature;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset = 0;


					 unsigned int ErrorCode;
					 if(pNewMirror->m_IsCluster==0)
					 {
						 ErrorCode =  OsnSetDiskCopy(&MirrorInfo);
					 }
					 else 
					 {
						 ErrorCode = OsnSetClusterDiskCopy(&MirrorInfo);
					 }
					 if(ErrorCode!=0)
					 {
						 MessageBox::Show(String::Concat("设置镜像出错,代码=",ErrorCode.ToString()));
						 return 1;
					 }

					 CMirrorInfo ^pMirrorInfo = gcnew CMirrorInfo(pNewMirror->pSourceDisk->m_Guid,
						 pNewMirror->pTargetDisk->m_Guid,pNewMirror->m_EIMMode,
						 pNewMirror->m_IsCluster,pNewMirror->m_ClusterResourceName);

					 pNewMirror->pTargetDisk->m_Role = Mirror_Target;
					 pNewMirror->pSourceDisk->m_Role = Mirror_Source;

					 //String ^pSourceString = pNewMirror->pSourceDisk->m_DiskSignature.ToString("X8");
					 //String ^pTargetString = pNewMirror->pTargetDisk->m_DiskSignature.ToString("X8");


					 if(pNewMirror->m_IsCluster==0)
					 {
						 ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pNewMirror->pSourceDisk->m_Guid,"}"),String::Concat("{",pNewMirror->pTargetDisk->m_Guid,"}"),true,2,0);
					 }
					 else
					 {
						 ErrorCode = SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pNewMirror->pSourceDisk->m_Guid,"}"),String::Concat("{",pNewMirror->pTargetDisk->m_Guid,"}"),true,2,1);
						 if(ErrorCode==0)
							 SetClusterKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pNewMirror->pSourceDisk->m_Guid,"}"),true,DIRTY,DIRTY);
					 }
					 if(ErrorCode!=0)
					 {
						 MessageBox::Show("设置注册表失败.");
						 OsnRemoveDiskCopy(&MirrorInfo);
						 return 1;
					 }

					 this->pDiskMirrorList->AddItem(pMirrorInfo);
					 WriteConfigurationFile();

					 return 0;
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("新建镜像过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("新建镜像过程中出现异常，异常信息：",exx->Message->ToString()));
				 }
				 return 1;
			 }
			 int FlushRegister()
			 {
				 try
				 {
					 LONG ret=0;
					 ret=RegFlushKey(HKEY_LOCAL_MACHINE);
					 if(ERROR_SUCCESS!=ret)
					 {
						 myEventLog->OSNWriteEventLog("刷新HKEY_LOCAL_MACHINE注册表失败",EventLogEntryType::Error,024);
					 }
					 ret=RegFlushKey(HKEY_CURRENT_USER);
					 if(ERROR_SUCCESS!=ret)
					 {
						 myEventLog->OSNWriteEventLog("刷新HKEY_CURRENT_USER注册表失败",EventLogEntryType::Error,024);
					 }
					 ret=RegFlushKey(HKEY_CURRENT_CONFIG);
					 if(ERROR_SUCCESS!=ret)
					 {
						 myEventLog->OSNWriteEventLog("刷新HKEY_CURRENT_CONFIG注册表失败",EventLogEntryType::Error,024);
					 }
				 }
				 catch(Exception^ ex)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("刷新注册表出现异常=",ex->Message->ToString()),EventLogEntryType::Error,024);

				 }
				 return 0;

			 }

			 int FlushBuffer(const wchar_t drive)
			 {

				 HANDLE handle;
				 char driveName[MAX_PATH];
				 sprintf_s(driveName,"\\\\.\\%c:",drive);
				 handle=CreateFile(driveName,
					 GENERIC_WRITE,
					 FILE_SHARE_READ|FILE_SHARE_WRITE,
					 NULL,
					 OPEN_EXISTING,
					 NULL,
					 NULL);
				 if(NULL==handle)
				 {
					 int ret=GetLastError();
					 return ret;
				 }
				 if(!FlushFileBuffers(handle))
				 {
					 int ret=GetLastError();
					 return ret;
				 }
				 return 0;
			 }
			 bool CheckVolIsBootable(String^ label)
			 {
				 try
				 {

					 String ^WQL = "Associators   of   {win32_LogicalDisk='%s'}   where   resultClass   =   Win32_DiskPartition";

					 String ^NewLine = WQL->Replace("%s", label);

					 // obtain the disk 
					 ManagementObjectSearcher ^DiskPartitionSearcher =
						 gcnew ManagementObjectSearcher("root\\CIMV2",
						 NewLine);

					 ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();

					 System::Collections::IEnumerator	^objEnum1= DiskCollection->GetEnumerator();
					 while(objEnum1->MoveNext())
					 {
						 ManagementObject ^queryObj1=static_cast<ManagementObject ^>(objEnum1->Current);                       
						 bool ret=Convert::ToBoolean(queryObj1->GetPropertyValue("Bootable"));
						 return ret;
					 }
					 return false;
				 }
				 catch(Exception^ ex)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("CheckVolIsBootable: ",ex->ToString()),EventLogEntryType::Error,011);
				 }
				 return false;

			 }
			 String^ GetVolListByDiskIndexAndFlushBuffer(unsigned int diskIndex)
			 {

				 try
				 {
					 String ^volList=nullptr;
					 // obtain the group
					 String^ strQuery="SELECT * FROM Win32_LogicalDisk WHERE DriveType=3";
					 ManagementObjectSearcher^ LogicalDiskSearch =gcnew ManagementObjectSearcher("root\\CIMV2",strQuery);

					 ManagementObjectCollection^ LogicalCollection = LogicalDiskSearch->Get();

					 System::Collections::IEnumerator^ objEnum1= LogicalCollection->GetEnumerator();
					 while(objEnum1->MoveNext())
					 {
						 ManagementObject^ queryObj1 = safe_cast<ManagementObject ^>(objEnum1->Current);

						 String ^Name = Convert::ToString(queryObj1->GetPropertyValue("Name"));

						 String ^WQL = "Associators of {win32_LogicalDisk='%s'} where resultClass = Win32_DiskPartition";

						 String ^NewLine = WQL->Replace("%s", Name);

						 // obtain the disk 
						 ManagementObjectSearcher ^DiskPartitionSearcher =gcnew ManagementObjectSearcher("root\\CIMV2",NewLine);

						 ManagementObjectCollection ^DiskCollection = DiskPartitionSearcher->Get();

						 System::Collections::IEnumerator	^objEnum= DiskCollection->GetEnumerator();
						 while(objEnum->MoveNext())
						 {
							 ManagementObject ^queryObj=safe_cast<ManagementObject ^>(objEnum->Current);                       
							 UInt32 Index2 = Convert::ToUInt32(queryObj->GetPropertyValue("DiskIndex"));
							 if(diskIndex == Index2)
							 {
								 FlushBuffer(Name[0]);
								 volList=String::Concat(volList,Name,"@");
							 }
						 }
					 }
					 return volList;
				 }
				 catch (Exception^ exx)
				 {
					 myEventLog->OsnHostMirrorLog(String::Concat("获取磁盘中的卷列表时出现异常，异常信息：",exx->Message->ToString()));
					 return nullptr;
				 }
			 }
	private: System::Void newMirrorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int ErrorCode =0;
				 if(treeView1->SelectedNode == this->treeView1->Nodes[0]->Nodes[0]) // volume node
				 {
					 if(this->m_InstallType == DiskCopy)
					 {
						 MessageBox::Show("当前安装版本不支持卷复制");
						 return;
					 }
					 if(this->pVolumeMirrorList->GetLength()>128)
					 {
						 MessageBox::Show("You have exceeded the maximum number of mirrors ");
						 return ;
					 }

					 if(this->listView1->SelectedItems->Count!=1)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("请选择驱动器.");
						 box->ShowDialog();
						 return ;
					 }
					 ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

					 String ^guid  = SelectItem->SubItems[1]->Text;
					 CDriverInfo ^pSelectDriver = this->pDriverList->GetDriverInfoByString(guid);
					 if(pSelectDriver==nullptr)
					 {
						 MessageBox::Show("无法获取驱动器信息");
						 return;
					 }

					 if(CheckVolIsBootable(pSelectDriver->m_VolumeLable))
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("不支持对系统卷做镜像,请选择非系统卷.");
						 box->ShowDialog();
						 return;
					 }
					 if(pSelectDriver->m_Role!=Free)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("该卷已经存在镜像关系中，不能再创建镜像关系.");
						 box->ShowDialog();
						 return;
					 }
					 /* String^ sysvol=(String ^)System::Environment::SystemDirectory;
					 sysvol=sysvol->Substring(0,2);
					 bool issysvol=false;
					 if(sysvol->Equals(pSelectDriver->m_VolumeLable))
					 {
					 if( MessageBox::Show("此卷是启动卷，操作系统安装在该卷上，是否任然继续？","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes))
					 {
					 return;
					 }

					 }*/
					 ErrorCode=NewVolumeMirror();
				 }
				 else if(treeView1->SelectedNode == this->treeView1->Nodes[0]->Nodes[1]) // disk node
				 {

					 if(this->m_InstallType == VolumeCopy)
					 {
						 MessageBox::Show("当前安装版本不支持磁盘复制");
						 return;
					 }
					 if(this->pDiskMirrorList->GetLength()>128)
					 {
						 MessageBox::Show("You have exceeded the maximum number of mirrors ");
						 return ;

					 }

					 if(this->listView1->SelectedItems->Count!=1)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("请选择磁盘.");
						 box->ShowDialog();
						 return ;
					 }

					 ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

					 String ^guid  = SelectItem->SubItems[0]->Text;

					 CDiskInfo ^pSelectDisk = this->pDiskList->GetDiskByIndex(Convert::ToUInt32(guid));
					 if(pSelectDisk == nullptr)
					 {
						 MessageBox::Show("无法获取磁盘信息信息");
						 return;
					 }

					 if(pSelectDisk->m_Role!=Free)
					 {
						 MessageBoxShow ^box = gcnew MessageBoxShow("该卷已经存在镜像关系中，不能再创建镜像关系.");
						 box->ShowDialog();
						 return;
					 }

					 ErrorCode = NewDiskMirror();
					// GetSystemDisksInfo();//diskmirror创建后，有可能有初始化后的操作，所以要刷新下disk信息
				 }
				 else
				 {
					 return;
				 }

				 if(ErrorCode ==0 )
				 { 
					 this->treeView1->SelectedNode = this->treeView1->Nodes[0]->Nodes[2];
					 this->RefreshMirrorNode();
				 }



			 }

			 bool DeleteVolumeMirror(ListViewItem ^DeleteItem)
			 {
				 String ^strSrcguid  = DeleteItem->SubItems[0]->Text;
				 String ^strDesguid = DeleteItem->SubItems[1]->Text;
				 String^ Property   =DeleteItem->SubItems[4]->Text;
				 CMirrorInfo ^pMirrorInfo= nullptr;
				 CDriverInfo ^pSourceInfo =nullptr;
				 CDriverInfo ^pTargetInfo = nullptr;

				 if(Property->Equals("卷复制"))
				 {
					 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcguid,strDesguid);
					 pSourceInfo = this->pDriverList->GetDriverInfoByString(strSrcguid);
					 pTargetInfo = this->pDriverList->GetDriverInfoByString(strDesguid);
				 }
				 else
				 {
					 pSourceInfo = this->pDriverList->GetDriverInfo(strSrcguid);
					 pTargetInfo = this->pDriverList->GetDriverInfo(strDesguid);

					 if(pSourceInfo==nullptr&&pTargetInfo == nullptr)
					 {
						 return false;
					 }
					 else
					 {
						 if(pSourceInfo!=nullptr)
							 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSourceInfo->m_GUID,true);
						 else
							 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pTargetInfo->m_GUID,false);

					 }

				 }

				 if(pMirrorInfo == nullptr)
				 {
					 MessageBox::Show("无法获取镜像信息");
					 return false;
				 }

				 MIRROR_INFO MirrorInfo;


				 GUID Srcguid0;
				 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
				 char* chguid = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid,&Srcguid0);
				 Marshal::FreeHGlobal(p);
				 GUID Tgtguid0;
				 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
				 char* chguid0 = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid0,&Tgtguid0);
				 Marshal::FreeHGlobal(p1);

				 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
				 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

				 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
				 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
				 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
				 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_TargetOffset;

				 // String ^pSourceString = pMirrorInfo->m_SourceDiskSignature.ToString("X8");
				 //pSourceString = String::Concat(pSourceString,"_");
				 //pSourceString = String ::Concat(pSourceString,pMirrorInfo->m_SourceOffset.ToString("X8"));

				 //String ^pTargetString = pMirrorInfo->m_TargetDiskSignature.ToString("X8");
				 //pTargetString = String::Concat(pTargetString,"_");
				 // pTargetString = String ::Concat(pTargetString,pMirrorInfo->m_TargetOffset.ToString("X8"));
				 unsigned int ErrorCode;
				 QUERY_MIRROR_STATE MirrorState;
				 ErrorCode = OsnGetMirrorState(&MirrorInfo,&MirrorState);	
				 if(ErrorCode == 0)
				 {
					 if(MirrorState.m_State == INIT)
					 {
						 MessageBox::Show("镜像关系正在初始化，请先停止初始化.");
						 return false;
					 }
				 }


				 unsigned int PreviousState = ReadPreviousState(pMirrorInfo->m_SourceGuid,true);

				 if(pMirrorInfo->m_IsCluster==0)
				 {
					 ErrorCode =SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),false,4,0);
				 }
				 else
				 {
					 ErrorCode=SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),false,4,1);
					 if(ErrorCode == 0)
					 {
						 SetClusterKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),false,DIRTY,DIRTY);
					 }
				 }
				 if(ErrorCode!=0)
				 {
					 MessageBox::Show("删除注册表失败");
				 }

				 ErrorCode = OsnRemoveVolumeCopy(&MirrorInfo);
				 if(ErrorCode==0)
				 {
					 if(pSourceInfo!=nullptr)
						 pSourceInfo->m_Role = Free;
					 if(pTargetInfo!=nullptr)
						 pTargetInfo->m_Role = Free;

					 this->pVolumeMirrorList->RemoveItem(pMirrorInfo);

					 WriteConfigurationFile();
				 }
				 else
				 {	
					 if(pMirrorInfo->m_IsCluster==0)
					 {
						 SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),true,PreviousState,0);
					 }
					 else
					 {
						 SetRegistryKey("System\\CurrentControlSet\\Services\\OsnVSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),true,PreviousState,1);
					 }
					 MessageBox::Show(String::Concat("删除镜像出错 ,代码 =",ErrorCode.ToString()));
					 return false;
				 }
				 return true;

			 }

			 bool DeleteDiskMirror(ListViewItem ^DeleteItem)
			 {

				 // ListViewItem ^SelectItem = this->listView1->SelectedItems[0];
				 String ^Srcguid  = DeleteItem->SubItems[0]->Text;
				 String ^ Tgtguid= DeleteItem->SubItems[1]->Text;

				 CMirrorInfo ^pMirrorInfo= nullptr;
				 CDiskInfo ^pSourceDiskInfo =nullptr;
				 CDiskInfo ^pTargetDiskInfo = nullptr;


				 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(Srcguid,true);

				 pSourceDiskInfo = this->pDiskList->GetDiskInfo(Srcguid);
				 pTargetDiskInfo = this->pDiskList->GetDiskInfo(Tgtguid);


				 if(pMirrorInfo == nullptr)
				 {
					 MessageBox::Show("无法获取镜像信息");
					 return false;
				 }

				 MIRROR_INFO MirrorInfo;

				 GUID Srcguid0;
				 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
				 char* chguid = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid,&Srcguid0);
				 Marshal::FreeHGlobal(p);
				 GUID Tgtguid0;
				 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
				 char* chguid0 = static_cast<char*>(p.ToPointer());
				 OsnGUIDFromString(chguid0,&Tgtguid0);
				 Marshal::FreeHGlobal(p1);

				 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
				 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

				 ///MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
				 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
				 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
				 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_TargetOffset;


				 // String ^pSourceString = pMirrorInfo->m_SourceDiskSignature.ToString("X8");
				 // String ^pTargetString = pMirrorInfo->m_TargetDiskSignature.ToString("X8");

				 unsigned int ErrorCode;
				 QUERY_MIRROR_STATE MirrorState;
				 ErrorCode= OsnGetDiskCopyMirrorState(&MirrorInfo,&MirrorState);
				 if(ErrorCode == 0)
				 {
					 if(MirrorState.m_State == INIT)
					 {
						 MessageBox::Show("镜像关系正在初始化，请先停止初始化.");
						 return false;
					 }
				 }

				 unsigned int PreviousState = ReadPreviousState(pMirrorInfo->m_SourceGuid,true);

				 if(pMirrorInfo->m_IsCluster==0)
				 {
					 ErrorCode =SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),false,4,0);
				 }
				 else
				 {
					 ErrorCode=SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),false,4,1);
					 if(ErrorCode ==0)
					 {
						 SetClusterKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),false,DIRTY,DIRTY);
					 }
				 }
				 if(ErrorCode!=0)
				 {
					 MessageBox::Show("删除注册表失败");
				 }


				 ErrorCode = OsnRemoveDiskCopy(&MirrorInfo);
				 if(ErrorCode==0)
				 {

					 if(pSourceDiskInfo!=nullptr)
						 pSourceDiskInfo->m_Role = Free;
					 if(pTargetDiskInfo!=nullptr)
						 pTargetDiskInfo->m_Role = Free;

					 this->pDiskMirrorList->RemoveItem(pMirrorInfo);

					 WriteConfigurationFile();
				 }
				 else
				 {
					 if(pMirrorInfo->m_IsCluster==0)
					 {
						 SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),true,PreviousState,0);
					 }
					 else
					 {
						 SetRegistryKey("System\\CurrentControlSet\\Services\\OsnDSS",String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),String::Concat("{",pMirrorInfo->m_TargetGuid,"}"),true,PreviousState,1);
					 }
					 MessageBox::Show(String::Concat("删除镜像出错 ,代码 =",ErrorCode.ToString()));
					 return false;

				 }
				 return true;
			 }
	private: System::Void deleteMirrorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 String ^Property = nullptr;
				 String ^strSrcSignOffset = nullptr;
				 String ^strDesSignOffset = nullptr;
				 String ^strSrcLable = nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;
				 String^ strParaMeter=nullptr;

				 if(this->listView1->SelectedItems->Count==0)
				 {
					 MessageBox::Show("请选择镜像关系.");
					 return;
				 }

				 this->listView1->HideSelection=false;

				 if(MessageBox::Show("确定需要删除吗?", "", MessageBoxButtons::YesNo) != System::Windows::Forms::DialogResult::Yes)
				 {
					 return;
				 } 

				 for(int i=0;i<this->listView1->SelectedItems->Count;i++)
				 {
					 ListViewItem ^SelectItem = this->listView1->SelectedItems[i];
					 strSrcSignOffset  = SelectItem->SubItems[0]->Text;
					 strDesSignOffset = SelectItem->SubItems[1]->Text;
					 String^ state  = SelectItem->SubItems[5]->Text;
					 // String^ strschedule=SelectItem->SubItems[3]->Text;
					 Property             = SelectItem->SubItems[4]->Text;
					 String^ sch        =SelectItem->SubItems[7]->Text;

					 if(Property->Equals("卷复制"))
					 {
						bool delflag = DeleteVolumeMirror(SelectItem);

						 pVolumeInfo = this->pDriverList->GetDriverInfoByString(strSrcSignOffset);
						 if(!pVolumeInfo)
						 {
							 MessageBox::Show("无法获取此卷信息。");
							 if(delflag)
								 MessageBox::Show("镜像CDP计划删除失败，请手动删除！");
							 return;
						 }

						 strParaMeter = String::Concat(strSrcSignOffset,"#");
						 strParaMeter = String::Concat(strParaMeter,"Volume ");
					 }
					 else if(Property->Equals("磁盘复制"))
					 {
						bool delflag = DeleteDiskMirror(SelectItem);

						 pDiskInfo = this->pDiskList->GetDiskInfo(strSrcSignOffset);
						 if(!pDiskInfo)
						 {
							 MessageBox::Show("无法获取此磁盘信息。");
							 if(delflag)
								 MessageBox::Show("镜像CDP计划删除失败，请手动删除！");
							 return;
						 }

						 strParaMeter = String::Concat(strSrcSignOffset,"#");
						 strParaMeter = String::Concat(strParaMeter,"Disk ");
					 }
					 else
					 {
						 MessageBox::Show("选择无效.");
					 }

					 if(sch->Equals("Schedule"))
					 {
						 try
						 {
							 String^ temppara=String::Concat("deletesilent","@",strParaMeter);
							 backgroundCDPSchedule->RunWorkerAsync(temppara);
						 }
						 catch(Exception^ exx)
						 {
							 MessageBox::Show("有些镜像的CDP计划删除失败，请手动删除");
							 myEventLog->OsnHostMirrorLog(String::Concat("删除CDP计划出现异常,error=",exx->Message->ToString()));
						 }
					 }
					 /* Task ^t = OSNTaskSchedule::GetTaskByTaskName(strSrcLable);
					 if(t)
					 {
					 bool bRet = OSNTaskSchedule::DeleteTaskSchedule(t->Name);
					 if(!bRet)
					 {
					 MessageBox::Show("删除CDP任务计划失败。");
					 }
					 }*/
				 }
				 this->RefreshMirrorNode();		 
			 }

			 void InitializeVolumeMirror(INIT_TYPE type)
			 {
				 try
				 {
					 // m_ListViewMutex->WaitOne();
					 ListViewItem ^SelectItem =nullptr; 
					 try
					 {
						 SelectItem=this->listView1->SelectedItems[0];
					 }
					 catch(...)
					 {
						 MessageBox::Show("您未选中镜像关系，请选择镜像关系");
						 // m_ListViewMutex->ReleaseMutex();
						 return;
					 }

					 String ^Srcguid  = SelectItem->SubItems[0]->Text;
					 String ^Desguid    = SelectItem ->SubItems[1]->Text;

					 // m_ListViewMutex->ReleaseMutex();

					 CMirrorInfo ^pMirrorInfo= nullptr;
					 CDriverInfo ^pSourceInfo =nullptr;
					 CDriverInfo ^pTargetInfo = nullptr;

					 if(Srcguid->Length>8)
					 {
						 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(Srcguid,Desguid);
						 pSourceInfo = this->pDriverList->GetDriverInfoByString(Srcguid);
						 pTargetInfo = this->pDriverList->GetDriverInfoByString(Desguid);

					 }
					 else
					 {
						 pSourceInfo = this->pDriverList->GetDriverInfo(Srcguid);
						 pTargetInfo = this->pDriverList->GetDriverInfo(Desguid);

						 if(pSourceInfo==nullptr&&pTargetInfo == nullptr)
						 {
							 MessageBox::Show("无法获取驱动器信息");
							 return;
						 }
						 else
						 {
							 if(pSourceInfo!=nullptr)
								 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSourceInfo->m_GUID,true);
							 else
								 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pTargetInfo->m_GUID,false);

						 }

					 }
					 if(CheckFileSystem(pSourceInfo->m_VolumeLable) != 2)
					 {
						 MessageBox::Show("源卷不是NTFS文件系统，不支持自动精简初始化.");
						 return ;
					 }

					 if(pSourceInfo->m_DriverSize>pTargetInfo->m_DriverSize)
					 {
						 MessageBox::Show("源卷比目标卷小，请重新设置镜像关系,否则数据不能一致");
						 return ;
					 }

					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息");
						 return;
					 }

					 MIRROR_INFO MirrorInfo;


					 GUID Srcguid0;
					 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
					 char* chguid = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid,&Srcguid0);
					 Marshal::FreeHGlobal(p);
					 GUID Tgtguid0;
					 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
					 char* chguid0 = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid0,&Tgtguid0);
					 Marshal::FreeHGlobal(p1);

					 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
					 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
					 //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_TargetOffset;

					 /*VOLUME_MARKER marker;
					 memset(&marker,0,sizeof(VOLUME_MARKER));

					 marker.m_MarkerBit =1;
					 marker.m_VolumeID.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 marker.m_VolumeID.MBP_VolumeID.m_BlockOffset=pMirrorInfo->m_SourceOffset;
					 memcpy(marker.m_Marker,"_MAKECDPSNAP_",sizeof("_MAKECDPSNAP_"));


					 unsigned int ErrorCode=OsnVolumeCopyMarkCDP(&marker);*/
					 unsigned int ErrorCode = OsnStartVolumeCopyInitialization(&MirrorInfo,type);
					 if(ErrorCode !=0)
					 {
						 MessageBox::Show(String::Concat("初始化失败,代码=",ErrorCode.ToString()));
						 return;
					 }
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("初始化过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("初始化过程中出现异常，异常信息：",exx->Message->ToString()));
					 //m_ListViewMutex->ReleaseMutex();
				 }

			 }

			 void InitializeDiskMirror()
			 {
				 try
				 {
					 //m_ListViewMutex->WaitOne();
					 ListViewItem ^SelectItem =nullptr;
					 try
					 {
						 SelectItem=this->listView1->SelectedItems[0];
					 }
					 catch(Exception^ exx)
					 {
						 MessageBox::Show("您未选中镜像关系，请选择镜像关系");
						 myEventLog->OsnHostMirrorLog(String::Concat("InitializeDiskMirror error=",exx->Message->ToString()));
						 // m_ListViewMutex->ReleaseMutex();
						 return;
					 }

					 String ^Srcguid  = SelectItem->SubItems[0]->Text;
					 String ^Tgtguid     = SelectItem ->SubItems[1]->Text;

					 // m_ListViewMutex->ReleaseMutex();

					 CMirrorInfo ^pMirrorInfo= nullptr;
					 CDiskInfo ^pSourceInfo =nullptr;
					 CDiskInfo ^pTargetInfo = nullptr;


					 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(Srcguid,true);

					 pSourceInfo = this->pDiskList->GetDiskInfo(Srcguid);
					 pTargetInfo = this->pDiskList->GetDiskInfo(Tgtguid);


					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息");
						 return;
					 }
					 //重新获取disk的容量的大小，然后判断源卷和目标卷的大小是否一样，map卷可以扩容会导致大小不一样
					 DISK_INFO pSourceDisk;
					 int ErrorCode=0;
					 memset(&pSourceDisk,0,sizeof(DISK_INFO));
					 pSourceDisk.m_DiskIndex=pSourceInfo->m_DiskIndex;
					 ErrorCode=OsnGetDisk(&pSourceDisk);
					 if(ErrorCode!=0)
					 {
						 myEventLog->OsnHostMirrorLog("获取disk的容量失败");
					 }
					 else
					 {
						 DISK_INFO pTargetDisk;
						 int ErrorCode=0;
						 memset(&pTargetDisk,0,sizeof(DISK_INFO));
						 pTargetDisk.m_DiskIndex=pTargetInfo->m_DiskIndex;
						 ErrorCode=OsnGetDisk(&pTargetDisk);
						 if(0==ErrorCode)
						 {
							 if(pSourceDisk.m_DiskSize>pTargetDisk.m_DiskSize)
							 {
								 MessageBox::Show("源盘比目标盘的容量大，请重新设置镜像关系,否则数据不能一致");
								 return;
							 }
						 }
					 }

					 MIRROR_INFO MirrorInfo;


					 GUID Srcguid0;
					 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
					 char* chguid = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid,&Srcguid0);
					 Marshal::FreeHGlobal(p);
					 GUID Tgtguid0;
					 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
					 char* chguid0 = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid0,&Tgtguid0);
					 Marshal::FreeHGlobal(p1);

					 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_TargetOffset;

					 /* VOLUME_MARKER marker;
					 memset(&marker,0,sizeof(VOLUME_MARKER));

					 marker.m_MarkerBit =1;
					 marker.m_VolumeID.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 marker.m_VolumeID.MBP_VolumeID.m_BlockOffset=pMirrorInfo->m_SourceOffset;
					 memcpy(marker.m_Marker,"_MAKECDPSNAP_",sizeof("_MAKECDPSNAP_"));


					 unsigned int ErrorCode=OsnDiskCopyMarkCDP(&marker);*/

					 unsigned int Error = OsnStartDiskCopyInitialization(&MirrorInfo);
					 if(Error !=0)
					 {
						 MessageBox::Show(String::Concat("初始化失败,代码=",Error.ToString()));
						 return;
					 }
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("初始化过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("初始化过程中出现异常，异常信息：",exx->Message->ToString()));
					 // m_ListViewMutex->ReleaseMutex();
				 }

			 }
private: System::Void NormalInitializeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 // m_ListViewMutex->WaitOne();
			 if(this->listView1->SelectedItems->Count!=1)
			 {
				 MessageBox::Show("请选择驱动器.");
				 // m_ListViewMutex->ReleaseMutex();
				 return;
			 }

			 if(MessageBox::Show("确认需要初始化?", "", MessageBoxButtons::YesNo) != System::Windows::Forms::DialogResult::Yes)
			 {
				 //m_ListViewMutex->ReleaseMutex();
				 return;
			 } 


			 ListViewItem ^SelectItem =nullptr;
			 try
			 {
				 SelectItem=this->listView1->SelectedItems[0];
			 }
			 catch(Exception^ exx)
			 {
				 MessageBox::Show("请选择初始化的镜像关系");
				 myEventLog->OsnHostMirrorLog(String::Concat("initializeToolStripMenuItem_Click error=",exx->Message->ToString()));
				 return;
			 }
			 String ^Property  = SelectItem->SubItems[4]->Text;
			 String^ isInit= SelectItem->SubItems[5]->Text;


			 // m_ListViewMutex->ReleaseMutex();
			 if(isInit->Contains("初始化"))
			 {
				 MessageBox::Show("该镜像关系已经在初始化");
				 return;
			 }

			 if(Property->Equals("卷复制"))
				 InitializeVolumeMirror(BASIC_INIT);
			 else if(Property->Equals("磁盘复制"))
				 InitializeDiskMirror();

			 Sleep(700);
			 this->RefreshMirrorNode();
		 }
private: System::Void NTFSInitializeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  // m_ListViewMutex->WaitOne();
			 if(this->listView1->SelectedItems->Count!=1)
			 {
				 MessageBox::Show("请选择驱动器.");
				 // m_ListViewMutex->ReleaseMutex();
				 return;
			 }

			 if(MessageBox::Show("确认需要初始化?", "", MessageBoxButtons::YesNo) != System::Windows::Forms::DialogResult::Yes)
			 {
				 //m_ListViewMutex->ReleaseMutex();
				 return;
			 } 


			 ListViewItem ^SelectItem =nullptr;
			 try
			 {
				 SelectItem=this->listView1->SelectedItems[0];
			 }
			 catch(Exception^ exx)
			 {
				 MessageBox::Show("请选择初始化的镜像关系");
				 myEventLog->OsnHostMirrorLog(String::Concat("initializeToolStripMenuItem_Click error=",exx->Message->ToString()));
				 return;
			 }
			 String ^Property  = SelectItem->SubItems[4]->Text;
			 String^ isInit= SelectItem->SubItems[5]->Text;


			 // m_ListViewMutex->ReleaseMutex();
			 if(isInit->Contains("初始化"))
			 {
				 MessageBox::Show("该镜像关系已经在初始化");
				 return;
			 }

			 if(Property->Equals("卷复制"))
				 InitializeVolumeMirror(NTFS_INIT);
			 else if(Property->Equals("磁盘复制"))
			 {
				 //InitializeDiskMirror();
				 MessageBox::Show("磁盘镜像不支持自动精简初始化.");
			 }

			 Sleep(700);
			 this->RefreshMirrorNode();
		 }


			 void StopVolumeMirrorInit()
			 {
				 try
				 {
					 // m_ListViewMutex->WaitOne();
					 ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

					 String ^Srcguid  = SelectItem->SubItems[0]->Text;
					 String ^Desguid    = SelectItem ->SubItems[1]->Text;

					 // m_ListViewMutex->ReleaseMutex();

					 CMirrorInfo ^pMirrorInfo= nullptr;
					 CDriverInfo ^pSourceInfo =nullptr;
					 CDriverInfo ^pTargetInfo = nullptr;

					 if(Srcguid->Length>8)
					 {
						 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(Srcguid,Desguid);
						 pSourceInfo = this->pDriverList->GetDriverInfoByString(Srcguid);
						 pTargetInfo = this->pDriverList->GetDriverInfoByString(Desguid);

						 //MessageBox::Show("Device offline,can not do initialize");
						 //return;
					 }
					 else
					 {
						 pSourceInfo = this->pDriverList->GetDriverInfo(Srcguid);
						 pTargetInfo = this->pDriverList->GetDriverInfo(Desguid);

						 if(pSourceInfo==nullptr&&pTargetInfo == nullptr)
						 {
							 MessageBox::Show("初始化失败，无法获取驱动器");
							 return;
						 }
						 else
						 {
							 if(pSourceInfo!=nullptr)
								 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pSourceInfo->m_GUID,true);
							 else
								 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pTargetInfo->m_GUID,false);

						 }

					 }

					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息");
						 return ;
					 }

					 MIRROR_INFO MirrorInfo;


					 GUID Srcguid0;
					 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
					 char* chguid = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid,&Srcguid0);
					 Marshal::FreeHGlobal(p);
					 GUID Tgtguid0;
					 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
					 char* chguid0 = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid0,&Tgtguid0);
					 Marshal::FreeHGlobal(p1);

					 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

					 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_SourceOffset;

					 unsigned int Process=-1;
					 unsigned int ErrorCode = OsnGetRecoveryPercentage(&MirrorInfo,&Process);
					 if(-1==Process&&ErrorCode==0)
					 {
						 MessageBox::Show("该镜像没有做初始化.");
						 return;
					 }
					 ErrorCode = OsnStopVolumeCopyInitialization(&MirrorInfo);
					 if(ErrorCode !=0)
					 {
						 MessageBox::Show(String::Concat("停止初始化失败,代码=",ErrorCode.ToString()));
						 return ;
					 }
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("停止初始化过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("停止初始化过程中出现异常，异常信息：",exx->Message->ToString()));
					 // m_ListViewMutex->ReleaseMutex();
				 }

			 }

			 void StopDiskMirrorInit()
			 {
				 try
				 {
					 //m_ListViewMutex->WaitOne();
					 ListViewItem ^SelectItem = this->listView1->SelectedItems[0];

					 String ^Srcguid  = SelectItem->SubItems[0]->Text;
					 String ^Tgtguid     = SelectItem ->SubItems[1]->Text;

					 // m_ListViewMutex->ReleaseMutex();

					 CMirrorInfo ^pMirrorInfo= nullptr;
					 CDiskInfo ^pSourceInfo =nullptr;
					 CDiskInfo ^pTargetInfo = nullptr;


					 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(Srcguid,true);

					 pSourceInfo = this->pDiskList->GetDiskInfo(Srcguid);
					 pTargetInfo = this->pDiskList->GetDiskInfo(Tgtguid);


					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息");
						 return;
					 }


					 MIRROR_INFO MirrorInfo;


					 GUID Srcguid0;
					 IntPtr p = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_SourceGuid);
					 char* chguid = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid,&Srcguid0);
					 Marshal::FreeHGlobal(p);
					 GUID Tgtguid0;
					 IntPtr p1 = Marshal::StringToHGlobalAnsi(pMirrorInfo->m_TargetGuid);
					 char* chguid0 = static_cast<char*>(p.ToPointer());
					 OsnGUIDFromString(chguid0,&Tgtguid0);
					 Marshal::FreeHGlobal(p1);

					 MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					 MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;

					 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_SourceDiskSignature;
					 // MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pMirrorInfo->m_SourceOffset;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature = pMirrorInfo->m_TargetDiskSignature;
					 // MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset   = pMirrorInfo->m_TargetOffset;

					 unsigned int Process=-1;
					 unsigned int ErrorCode = OsnGetDiskCopyRecoveryPercentage(&MirrorInfo,&Process);
					 if(-1==Process&&ErrorCode==0)
					 {
						 MessageBox::Show("该镜像没有做初始化.");
						 return;
					 }

					 ErrorCode = OsnStopDiskCopyInitialization(&MirrorInfo);
					 if(ErrorCode !=0)
					 {
						 MessageBox::Show(String::Concat("停止初始化失败,代码=",ErrorCode.ToString()));
						 return;
					 }
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("停止初始化过程中出现异常，请稍后重试");
					 myEventLog->OsnHostMirrorLog(String::Concat("停止初始化过程中出现异常，异常信息：",exx->Message->ToString()));
					 // m_ListViewMutex->ReleaseMutex();
				 }

			 }
	private: System::Void stopInitializeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count!=1)
				 {
					 MessageBox::Show("请选择驱动器.");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }

				 ListViewItem ^SelectItem = nullptr;
				 try
				 {
					 SelectItem=this->listView1->SelectedItems[0];
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("您未选中镜像关系，请选择镜像关系");
					 myEventLog->OsnHostMirrorLog(String::Concat("stopInitializeToolStripMenuItem_Click error=",exx->Message->ToString()));
					 return;
				 }
				 String ^Property  = SelectItem->SubItems[4]->Text;
				 // m_ListViewMutex->ReleaseMutex();

				 if(Property->Equals("卷复制"))
					 StopVolumeMirrorInit();
				 else if(Property->Equals("磁盘复制"))
					 StopDiskMirrorInit();


				 this->RefreshMirrorNode();


			 }
	private: System::Void listView1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
			 {
				 TreeNode ^CurrentNode = this->treeView1->SelectedNode;

				 if(e->Button==System::Windows::Forms::MouseButtons::Right)
				 {
					 this->NewMirrorMenuItem->Enabled =false;
					 this->DeleteMirror->Enabled = false;
					 this->Initialize->Enabled = false;
					 this->StopInitialize->Enabled= false;
					 this->CreateCDPToolStripMenuItem->Enabled = false;
					 this->transferMode1ToolStripMenuItem->Enabled = false;

					 this->DelCDPScheduleToolStripMenuItem->Enabled = true;
					 this->CreateCDPScheduleToolStripMenuItem->Enabled = true;
					 this->ViewCDPScheduleToolStripMenuItem->Enabled = true;
					 this->contextToolStripMenuItem->Enabled = true;
					 // this->listView1->ContextMenuStrip = this->contextMenuStrip1;
					 ListViewItem ^Item = this->listView1->GetItemAt(e->X,e->Y);
					
					 if(CurrentNode==this->treeView1->Nodes[0]->Nodes[0])       // driver node
					 {
						 this->listView1->ContextMenuStrip= this->NewMirror;
						 if(Item!=nullptr)
						 {
							 this->NewMirrorMenuItem->Enabled=true;
						 }

					 }
					 else if(CurrentNode == this->treeView1->Nodes[0]->Nodes[1]) // disk node
					 {
						 this->listView1->ContextMenuStrip= this->NewMirror;
						 if(Item != nullptr)
						 {
							 this->NewMirrorMenuItem->Enabled=true;
						 }

					 }
					 else if(CurrentNode == this->treeView1->Nodes[0]->Nodes[2]) // mirror node
					 {
						 this->listView1->ContextMenuStrip = this->contextMenuStrip1;
						 if(Item!=nullptr)
						 {
							 this->DeleteMirror->Enabled=true;
							 this->Initialize->Enabled=true;
							 this->StopInitialize->Enabled=true;
							 this->CreateCDPToolStripMenuItem->Enabled = true;
							 this->transferMode1ToolStripMenuItem->Enabled = true;
						 }
					 }
					 else
					 {
						 this->DelCDPScheduleToolStripMenuItem->Enabled = false;
						 this->CreateCDPScheduleToolStripMenuItem->Enabled = false;
						 this->ViewCDPScheduleToolStripMenuItem->Enabled = false;
						 this->contextToolStripMenuItem->Enabled = false;
					 }
				 }
			 }

			 unsigned int ReadPreviousState(String ^Key,bool Flag)
			 {
				 if(Flag) // VolumeCopy
				 {
					 RegistryKey	^pRegkey = Registry::LocalMachine;
					 RegistryKey ^pParameter = pRegkey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\State");
					 if(pParameter)
					 {
						 Object      ^pState=nullptr;
						 pState=pParameter->GetValue(Key);
						 int state = Convert::ToUInt32(pState);
						 if(state>=0&&state<=6)
							 return state;
						 else 
							 return 2;

					 }
					 else
					 {
						 return 2;
					 }


				 }
				 else //DiskCopy
				 {
					 RegistryKey	^pRegkey = Registry::LocalMachine;
					 RegistryKey ^pParameter = pRegkey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDiskCopy\\Parameters\\State");

					 if(pParameter)
					 {
						 Object      ^pState=nullptr;
						 pState=pParameter->GetValue(Key);
						 int state = Convert::ToUInt32(pState);
						 if(state>=0&&state<=6)
							 return state;
						 else 
							 return 2;

					 }
					 else
					 {
						 return 2;
					 }


				 }

			 }
			 
			 unsigned int SetClusterKey(String ^ServiceKeyName,String ^SourceString,bool Flag,int remoteState,int localState)
			 {
				 RegistryKey ^pRegKey = Registry::LocalMachine;
				 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters"),true);
				 if(pParmeterKey == nullptr)
				 {
					 pParmeterKey=pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters"));
					 if(pParmeterKey==nullptr)
					 {
						 return 1;
					 }

				 }

				 RegistryKey ^pClusterLocalState = nullptr;
				 pClusterLocalState = pParmeterKey->OpenSubKey((ServiceKeyName,"\\Parameters\\ClusterLocalState"),true);
				 if(pClusterLocalState == nullptr)
				 {
					 pClusterLocalState =pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\ClusterLocalState"));
					 if(pClusterLocalState == nullptr)
					 {
						
						 pRegKey->Close();
						 return 1;
					 }
				 }

				 RegistryKey ^pClusterRemoteState = nullptr;
				 pClusterRemoteState = pParmeterKey->OpenSubKey((ServiceKeyName,"\\Parameters\\ClusterRemoteState"),true);
				 if(pClusterRemoteState == nullptr)
				 {
					 pClusterRemoteState =pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\ClusterRemoteState"));
					 if(pClusterRemoteState == nullptr)
					 {
						 pClusterLocalState->Close();
						 pRegKey->Close();
						 return 1;
					 }
				 }

				 if(!Flag)
				 {
					 pClusterRemoteState->DeleteValue(SourceString,false);
					 pClusterLocalState->DeleteValue(SourceString,false);
				 }
				 else
				 {
					 pClusterRemoteState->SetValue(SourceString,remoteState);
					 pClusterLocalState->SetValue(SourceString,localState);
				 }

				 pClusterRemoteState->Close();
				 pClusterLocalState->Close();
				 pRegKey->Close();

				 return 0;
			 }

			 unsigned int SetRegistryKey(String ^ServiceKeyName,String ^SourceString,String ^TargetString,bool Flag,int State,int IsCluster)
			 {
				 RegistryKey ^pRegKey = Registry::LocalMachine;
				 // RegistryKey ^pParmeterKey = pRegKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVolumeCopy\\Paramters",true);

				 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters"),true);
				 if(pParmeterKey == nullptr)
				 {
					 pParmeterKey=pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters"));
					 if(pParmeterKey==nullptr)
					 {
						 return 1;
					 }

				 }

				 RegistryKey ^pStateKey = pParmeterKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters\\State"),true);
				 if(pStateKey==nullptr)
				 {
					 pStateKey = pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\State"));
					 if(pStateKey==nullptr)
					 {
						 pRegKey->Close();
						 return 1;
					 }
				 }

				 RegistryKey ^pMirrorKey=nullptr;
				 if(IsCluster==0)
				 {
					 pMirrorKey = pParmeterKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters\\Mirrors"),true);
					 if(pMirrorKey==nullptr)
					 {
						 pMirrorKey = pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\Mirrors"));
						 if(pMirrorKey==nullptr)
						 {
							 pStateKey->Close();
							 pRegKey->Close();
							 return 1;
						 }
					 }
				 }
				 else
				 {
					 pMirrorKey = pParmeterKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters\\ClusterMirrors"),true);
					 if(pMirrorKey==nullptr)
					 {
						 pMirrorKey = pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\ClusterMirrors"));
						 if(pMirrorKey==nullptr)
						 {
							 pStateKey->Close();
							 pRegKey->Close();
							 return 1;
						 }
					 }
				 }


				 RegistryKey ^pRebootKey = pParmeterKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters\\Reboot"),true);
				 if(pRebootKey==nullptr)
			  {
				  pRebootKey = pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters\\Reboot"));
				  if(pRebootKey==nullptr)
				  {
					  pStateKey->Close();
					  pRegKey->Close();
					  pMirrorKey->Close();
					  return 1;
				  }
			  }


				 if(Flag)
			  {
				  //int State = 2;

				  pStateKey->SetValue(SourceString, State);
				  pMirrorKey->SetValue(SourceString,TargetString);

				  int Reboot = 0;
				  pRebootKey->SetValue(SourceString,Reboot);
			  }
				 else
			  {
				  pStateKey->DeleteValue(SourceString,false);
				  pMirrorKey->DeleteValue(SourceString,false);
				  pRebootKey->DeleteValue(SourceString,false);

			  }

				 pStateKey->Close();
				 pMirrorKey->Close();
				 pRegKey->Close();
				 pRebootKey->Close();


				 return 0;

			 }


			 void ReadInstallTypeFromRegistry()
			 {
				 try
				 {

					 this->m_InstallType = VolumeCopy; // default to VolumeCopy
					 RegistryKey	^pRegkey = Registry::LocalMachine;
					 RegistryKey ^pVSSParameter = nullptr;
					 RegistryKey ^pDSSParameter=nullptr;

					 pVSSParameter = pRegkey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS");
					 pDSSParameter = pRegkey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS");

					 if(pVSSParameter!=nullptr&&pDSSParameter!=nullptr)
					 {
						 this->m_InstallType = Both;

					 }
					 else if(pDSSParameter!=nullptr)
					 {
						 this->m_InstallType = DiskCopy;
					 }
					 else
					 {
						 this->m_InstallType = VolumeCopy;
					 }

					 if(pVSSParameter)
						 pVSSParameter->Close();

					 if(pDSSParameter)
						 pDSSParameter->Close();

					 if(pRegkey)
						 pRegkey->Close();
				 }
				 catch(Exception^ exx)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("ReadInstallTypeFromRegistry error:",exx->Message->ToString()),EventLogEntryType::Error,024);
				 }

			 }

			 void RegistryKeyVolumeCopy()
			 {
				 try
				 {
					 RegistryKey ^pRegKey = Registry::LocalMachine;
					 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters",true);
					 if(pParmeterKey == nullptr)
					 {
						 pParmeterKey=pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters");
						 if(pParmeterKey==nullptr)
						 {
							 return;
						 }

					 }

					 RegistryKey ^pStateKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\State",true);
					 if(pStateKey==nullptr)
					 {
						 pStateKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\State");
						 if(pStateKey==nullptr)
						 {
							 pRegKey->Close();
							 return;
						 }
					 }

					 RegistryKey ^pMirrorKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\Mirrors",true);
					 if(pMirrorKey==nullptr)
					 {
						 pMirrorKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\Mirrors");
						 if(pMirrorKey==nullptr)
						 {
							 pRegKey->Close();
							 pStateKey->Close();
							 return;
						 }
					 }

					 RegistryKey ^pProcessKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\Process",true);
					 if(pProcessKey==nullptr)
					 {
						 pProcessKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\Process");
						 if(pProcessKey==nullptr)
						 {
							 pProcessKey->Close();
							 pRegKey->Close();
							 pStateKey->Close();
							 return;
						 }
					 }

					 RegistryKey ^pQueueKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\QueueIOs",true);
					 if(pQueueKey==nullptr)
					 {
						 pQueueKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\QueueIOs");
						 if(pQueueKey==nullptr)
						 {
							 pProcessKey->Close();
							 pRegKey->Close();
							 pStateKey->Close();
							 pQueueKey->Close();
							 return;
						 }
					 }

					 pQueueKey->Close();
					 pMirrorKey->Close();
					 pProcessKey->Close();
					 pStateKey->Close();
					 pRegKey->Close();
				 }
				 catch(Exception^ exx)
				 {
					 myEventLog->OSNWriteEventLog(String::Concat("RegistryKeyVolumeCopy,error:",exx->Message->ToString()),EventLogEntryType::Error,024);
				 }
				 return;

			 }
			 void RegistryKeyDiskCopy()
			 {
				 RegistryKey ^pRegKey = Registry::LocalMachine;
				 RegistryKey ^pParmeterKey = pRegKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters",true);
				 if(pParmeterKey == nullptr)
				 {
					 pParmeterKey=pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters");
					 if(pParmeterKey==nullptr)
					 {
						 return;
					 }

				 }

				 RegistryKey ^pStateKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\State",true);
				 if(pStateKey==nullptr)
				 {
					 pStateKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\State");
					 if(pStateKey==nullptr)
					 {
						 pRegKey->Close();
						 return;
					 }
				 }

				 RegistryKey ^pMirrorKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\Mirrors",true);
				 if(pMirrorKey==nullptr)
			  {
				  pMirrorKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\Mirrors");
				  if(pMirrorKey==nullptr)
				  {
					  pRegKey->Close();
					  pStateKey->Close();
					  return;
				  }
			  }

				 RegistryKey ^pProcessKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\Process",true);
				 if(pProcessKey==nullptr)
			  {
				  pProcessKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\Process");
				  if(pProcessKey==nullptr)
				  {
					  pProcessKey->Close();
					  pRegKey->Close();
					  pStateKey->Close();
					  return;
				  }
			  }

				 RegistryKey ^pQueueKey = pParmeterKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\QueueIOs",true);
				 if(pQueueKey==nullptr)
			  {
				  pQueueKey = pRegKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\QueueIOs");
				  if(pQueueKey==nullptr)
				  {
					  pProcessKey->Close();
					  pRegKey->Close();
					  pStateKey->Close();
					  pQueueKey->Close();
					  return;
				  }
			  }

				 pQueueKey->Close();
				 pProcessKey->Close();
				 pMirrorKey->Close();
				 pStateKey->Close();
				 pRegKey->Close();
				 return;

			 }
			 void CheckRegistryKey()
			 {
				 if(this->m_InstallType== VolumeCopy)
					 RegistryKeyVolumeCopy();
				 else if(this->m_InstallType == DiskCopy)
					 RegistryKeyDiskCopy();
				 else
				 {
					 RegistryKeyVolumeCopy();
					 RegistryKeyDiskCopy();

				 }


			 }

			 int GetIsClusterByGUID(String^ srcguid,String^dstguid)
			 {
				 String ^strLine = nullptr;
				 array<String^>^split= nullptr;
				 array<Char>^chars = {';'};

				 //read initiator information info
				 String ^strFileName = String::Concat(ImagePath, L"\\OSNHostMirror.cfg");
				 FileInfo ^fiHost = gcnew FileInfo(strFileName);
				 if(!fiHost->Exists)
				 {
					 return 0;
				 }

				 //String ^strMsg = "Illegal line found in Configuration file.";

				 StreamReader ^srHost = fiHost->OpenText();
				 while((strLine = srHost->ReadLine()))
				 {
					 if(strLine->StartsWith("DiskMirrorInfo"))
					 {
						 //DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
						 strLine = strLine->Substring(strlen("DiskMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;
						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode=2;
						 unsigned int isCluster = 0;
						 String ^ResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster= Convert::ToInt32(s);
					   }
					   else if(i == 4)
					   {
						   ResourceName = s;
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return isCluster;
				   }

					 }

					 if(strLine->StartsWith("VolumeMirrorInfo"))
					 {
						 //VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
						 strLine = strLine->Substring(strlen("VolumeMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;

						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode=2;
						 unsigned int isCluster = 0;
						 String ^ResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid =s; //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster = Convert::ToInt32(s);
					   }
					   else if(i == 4)
					   {
						   ResourceName = s;
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return isCluster;
				   }

					 }


				 }
				 srHost->Close();
				 return 0;
			 }

			 String ^GetClusterResourceName(String^ srcguid,String^ dstguid)
			 {
				 String ^strLine = nullptr;
				 array<String^>^split= nullptr;
				 array<Char>^chars = {';'};

				 //read initiator information info
				 String ^strFileName = String::Concat(ImagePath, L"\\OSNHostMirror.cfg");
				 FileInfo ^fiHost = gcnew FileInfo(strFileName);
				 if(!fiHost->Exists)
				 {
					 return "NONE";
				 }

				 //String ^strMsg = "Illegal line found in Configuration file.";

				 StreamReader ^srHost = fiHost->OpenText();
				 while((strLine = srHost->ReadLine()))
				 {
					 if(strLine->StartsWith("DiskMirrorInfo"))
					 {
						 //DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
						 strLine = strLine->Substring(strlen("DiskMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;
						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode=2;
						 unsigned int isCluster = 0;
						 String ^ResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster= Convert::ToInt32(s);
					   }
					   else if( i == 4)
					   {
						   ResourceName = s;
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return ResourceName;
				   }

					 }

					 if(strLine->StartsWith("VolumeMirrorInfo"))
					 {
						 //VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
						 strLine = strLine->Substring(strlen("VolumeMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;

						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode=2;
						 unsigned int isCluster = 0;
						 String ^ResourceName = "NONE";

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid =s; //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   else if(i == 3)
					   {
						   isCluster = Convert::ToInt32(s);
					   }
					   else if(i == 4)
					   {
						   ResourceName = s;
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return ResourceName;
				   }

					 }


				 }
				 srHost->Close();
				 return "NONE";
			 }


			 int GetEimModebyGUID(String^ srcguid,String^ dstguid)
			 {
				 String ^strLine = nullptr;
				 array<String^>^split= nullptr;
				 array<Char>^chars = {';'};

				 //read initiator information info
				 String ^strFileName = String::Concat(ImagePath, L"\\OSNHostMirror.cfg");
				 FileInfo ^fiHost = gcnew FileInfo(strFileName);
				 if(!fiHost->Exists)
				 {
					 return 2;
				 }

				 //String ^strMsg = "Illegal line found in Configuration file.";

				 StreamReader ^srHost = fiHost->OpenText();
				 while((strLine = srHost->ReadLine()))
				 {
					 if(strLine->StartsWith("DiskMirrorInfo"))
					 {
						 //DiskMirrorInfo=SrcSignature;DesSignature;EIMMode
						 strLine = strLine->Substring(strlen("DiskMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;
						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode = 2;

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return uiEIMMode;
				   }

					 }

					 if(strLine->StartsWith("VolumeMirrorInfo"))
					 {
						 //VolumeMirrorInfo=SrcSignature;SrcOffset;DesSignature;DesOffset;EIMMode
						 strLine = strLine->Substring(strlen("VolumeMirrorInfo") + 1);
						 split = strLine->Split( chars );

						 int i=0;

						 String^  SrcGuid = "00000000-0000-0000-0000-000000000000";
						 String^  DesGuid = "00000000-0000-0000-0000-000000000000";
						 unsigned int uiEIMMode = 2;

						 System::Collections::IEnumerator	^myEnum = split->GetEnumerator();
						 while ( myEnum->MoveNext() )
				   {
					   String^ s = safe_cast<String^>(myEnum->Current);

					   if(i == 0)
					   {
						   SrcGuid =s; //UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 1)
					   {
						   DesGuid = s;//UInt32::Parse(s,System::Globalization::NumberStyles::HexNumber);
					   }
					   else if(i == 2)
					   {
						   uiEIMMode = Convert::ToInt32(s);
					   }
					   ++i;
				   }

						 if(SrcGuid->Equals(srcguid)&&DesGuid->Equals(dstguid))
				   {
					   srHost->Close();
					   return uiEIMMode;
				   }

					 }


				 }
				 srHost->Close();
				 return 2;

			 }
			 void GetVolumeCopyMirrorInfo()
			 {
				 try
			  {
				  MIRROR_INFO MirrorInfo ;
				  for( int i=0;i<this->pDriverList->GetLength();i++)
				  {
					  CDriverInfo ^pDriverInfo = static_cast<CDriverInfo ^>(this->pDriverList->GetItem(i));
					  if(pDriverInfo->m_Role!=Free)
						  continue;



					  GUID Srcguid0;
					  //array<char>^ chguid=gcnew array<char>(128);
					  try
					  {
						  IntPtr p = Marshal::StringToHGlobalAnsi(pDriverInfo->m_GUID);
						  char* chguid = static_cast<char*>(p.ToPointer());


						  OsnGUIDFromString(chguid,&Srcguid0);
						  Marshal::FreeHGlobal(p);
						  GUID Tgtguid0;

						  IntPtr p1 = Marshal::StringToHGlobalAnsi("00000000-0000-0000-0000-000000000000");
						  char* chguid0 = static_cast<char*>(p.ToPointer());
						  OsnGUIDFromString(chguid0,&Tgtguid0);
						  Marshal::FreeHGlobal(p1);

						  MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
						  MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;
						  //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset=0;
						  //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature=0;
						  //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_NotUsed1=0;
					  }
					  catch(Exception^ ee)
					  {
						  myEventLog->OSNWriteEventLog(String::Concat("Osnguid from string,error：",ee->Message->ToString()),EventLogEntryType::Error,024);
					  }
					  //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_DiskSignature = pDriverInfo->m_DiskSignature;
					  //MirrorInfo.m_SourceVolume.MBP_VolumeID.m_BlockOffset  = pDriverInfo->m_StartOffset;
					  //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset=0;
					  //MirrorInfo.m_TargetVolume.MBP_VolumeID.m_DiskSignature=0;

					  try
					  {

						  int ErrorCode = OsnVolumeCopyGetMirrorInfo(&MirrorInfo);
						  if(ErrorCode==0)
						  {   
							  //ULONG offset = MirrorInfo.m_TargetVolume.MBP_VolumeID.m_BlockOffset;
							  String^ strguid=nullptr;
							  char guid[128];
							  OsnGUIDToString(guid,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
							  strguid=gcnew String(guid);
							  CDriverInfo ^pTargetDriver = this->pDriverList->GetDriverInfo(strguid);
							  if(pTargetDriver!=nullptr)
							  {
								  pTargetDriver->m_Role =Mirror_Target;
							  }
							  pDriverInfo->m_Role=Mirror_Source;

							  CMirrorInfo ^pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(pDriverInfo->m_GUID,true);
							  if(pMirrorInfo==nullptr)
							  {
								  String^ Srcguid=nullptr;
								  char guid[128];
								  OsnGUIDToString(guid,MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid);
								  Srcguid=gcnew String(guid);
								  String^ Desguid=nullptr;
								  OsnGUIDToString(guid,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
								  Desguid=gcnew String(guid);
								  unsigned int eimmode=GetEimModebyGUID(Srcguid,Desguid);
								  unsigned int isCluster=GetIsClusterByGUID(Srcguid,Desguid);
								  String ^ResourceName = GetClusterResourceName(Srcguid,Desguid);
								  pMirrorInfo = gcnew CMirrorInfo(Srcguid,Desguid,eimmode,isCluster,ResourceName);

								  this->pVolumeMirrorList->AddItem(pMirrorInfo);
							  }


						  }
						  //else
						  //{
						  //myEventLog->OSNWriteEventLog(String::Concat("OsnVolumeCopyGetMirrorInfo获取镜像信息失败，错误代码是：",Convert::ToString(ErrorCode),"for volume ",pDriverInfo->m_VolumeLable),EventLogEntryType::Error,024);

						  //}
					  }
					  catch(Exception^ exxx)
					  {
						  myEventLog->OSNWriteEventLog(String::Concat("OsnVolumeCopyGetMirrorInfo,error：",exxx->Message->ToString()),EventLogEntryType::Error,024);
					  }

				  }
				  WriteConfigurationFile();
			  }
				 catch(Exception^ exx)
			  {
				  myEventLog->OSNWriteEventLog(String::Concat("GetVolumeCopyMirrorInfo,error：",exx->Message->ToString()),EventLogEntryType::Error,024);

			  }

			 }

			 void GetDiskCopyMirrorInfo()
			 {
				 try
			  {
				  MIRROR_INFO MirrorInfo ;
				  for( int i=0;i<this->pDiskList->GetLength();i++)
				  {
					  CDiskInfo ^pDiskInfo = static_cast<CDiskInfo ^>(this->pDiskList->GetItem(i));
					  if(pDiskInfo->m_Role!=Free)
						  continue;

					  GUID Srcguid0;
					  IntPtr p = Marshal::StringToHGlobalAnsi(pDiskInfo->m_Guid);
					  char* chguid = static_cast<char*>(p.ToPointer());


					  OsnGUIDFromString(chguid,&Srcguid0);
					  Marshal::FreeHGlobal(p);
					  GUID Tgtguid0;

					  IntPtr p1 = Marshal::StringToHGlobalAnsi("00000000-0000-0000-0000-000000000000");
					  char* chguid0 = static_cast<char*>(p.ToPointer());
					  OsnGUIDFromString(chguid0,&Tgtguid0);
					  Marshal::FreeHGlobal(p1);

					  MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid=Srcguid0;
					  MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid=Tgtguid0;


					  int ErrorCode = OsnDiskCopyGetMirrorInfo(&MirrorInfo);
					  if(ErrorCode==0)
					  {   
						  String^ strguid=nullptr;
						  char guid[128];
						  OsnGUIDToString(guid,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
						  strguid=gcnew String(guid);

						  CDiskInfo ^pTargetDisk= this->pDiskList->GetDiskInfo(strguid);
						  if(pTargetDisk!=nullptr)
								{
									pTargetDisk->m_Role =Mirror_Target;
								}
						  pDiskInfo->m_Role=Mirror_Source;

						  CMirrorInfo ^pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(pDiskInfo->m_Guid,true);
						  if(pMirrorInfo==nullptr)
								{
									String^ strSrcguid=nullptr;
									char guid[128];
									OsnGUIDToString(guid,MirrorInfo.m_SourceVolume.SAN_VolumeID.m_VolumeGuid);
									strSrcguid=gcnew String(guid);
									String^ strDesguid=nullptr;
									OsnGUIDToString(guid,MirrorInfo.m_TargetVolume.SAN_VolumeID.m_VolumeGuid);
									strDesguid=gcnew String(guid);

									unsigned int eimmode=GetEimModebyGUID(strSrcguid,strDesguid);
									unsigned int isCluster = GetIsClusterByGUID(strSrcguid,strDesguid);
									String ^ResourceName = GetClusterResourceName(strSrcguid,strDesguid);
									pMirrorInfo = gcnew CMirrorInfo(strSrcguid,strDesguid,eimmode,isCluster,ResourceName);

									this->pDiskMirrorList->AddItem(pMirrorInfo);
								}
							}
					  //else
							//{
					  //myEventLog->OSNWriteEventLog(String::Concat("OsnDiskCopyGetMirrorInfo获取镜像信息失败，错误代码是：",Convert::ToString(ErrorCode),"for disk ",Convert::ToString(pDiskInfo->m_DiskIndex)),EventLogEntryType::Error,024);

							//}
					 }
				  WriteConfigurationFile();
			  }
				 catch(Exception^ exx)
			  {
				  myEventLog->OSNWriteEventLog(String::Concat("GetDiskCopyMirrorInfo,error：",exx->Message->ToString()),EventLogEntryType::Error,024);
			  }
			 }

			 void GetSystemMirrorInfo()
			 {
				 
				 if(this->m_InstallType == VolumeCopy)
					 GetVolumeCopyMirrorInfo();
				 else if(this->m_InstallType == DiskCopy)
					 GetDiskCopyMirrorInfo();
				 else
				 {
					 GetVolumeCopyMirrorInfo();
					 GetDiskCopyMirrorInfo();

				 }
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->pDriverList->Clear();
				 this->pVolumeMirrorList->Clear();
				 this->pDiskMirrorList->Clear();
				 this->pDiskList->Clear();     

				 GetSystemDriversInfo();
				 GetSystemDisksInfo(); 

				 ReadConfigurationFile();
				 GetSystemMirrorInfo();
				 WriteConfigurationFile();

				 if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[0])
			  {
				  //this->RefreshDriverNode();
				  RefreshVolumeListView();
			  }
				 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[1])
			  {
				  this->RefreshDiskListView();
			  }
				 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[2])
			  {
				  this->RefreshMirrorNode();
			  }

				 this->RefreshStatusBar();

			 }
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Application::Exit();
			 }
	private: System::Void RefreshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {		 
				 this->pDriverList->Clear();
				 this->pVolumeMirrorList->Clear();
				 this->pDiskMirrorList->Clear();
				 this->pDiskList->Clear();     

				 GetSystemDriversInfo();
				 GetSystemDisksInfo(); 

				 ReadConfigurationFile();
				 GetSystemMirrorInfo();
				 WriteConfigurationFile();

				 if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[0])
			  {
				  this-> RefreshVolumeListView();
			  }
				 else   if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[1])
			  {
				  this->RefreshDiskListView();
			  }
				 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[2])
			  {
				  this->RefreshMirrorNode();
			  }

			 }


	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 /* if(!ImagePath)
				 return;

				 System::Diagnostics::Process	^help = gcnew System::Diagnostics::Process();
				 help->StartInfo->RedirectStandardOutput = false;
				 help->StartInfo->FileName = String::Concat(ImagePath,"\\OSNMirrorCopy.chm");
				 help->StartInfo->UseShellExecute = true;
				 help->Start();*/
				 About ^about = gcnew About(this->m_InstallType);
				 about->ShowDialog();


			 }
	private: System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if(!ImagePath)
					 return;

				 System::Diagnostics::Process	^help = gcnew System::Diagnostics::Process();
				 help->StartInfo->RedirectStandardOutput = false;
				 help->StartInfo->FileName = String::Concat(ImagePath,"\\HostMirrorHandBook(x86).chm");
				 help->StartInfo->UseShellExecute = true;
				 help->Start();
			 }

	private: System::Void SystemSetting_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

				 Setting ^pSetting = gcnew Setting(m_InstallType);
				 pSetting->ShowDialog();

			 }

	private: System::Void splitContainer1_SplitterMoved(System::Object^  sender, System::Windows::Forms::SplitterEventArgs^  e) 
			 {
				 int a=this->splitContainer1->Panel1->Size.Width;
				 // this->toolStripStatusLabel1->Size.Width=a;

				 int b= (this->splitContainer1->Panel2->Size.Width*7)/10;
				 //this->toolStripStatusLabel2->Size.Width=b;

			 }
			 void RefreshStatusBar()
			 {
				 SYSTEMTIME systemTime;	
				 GetLocalTime(&systemTime);
				 String ^sDay = systemTime.wYear.ToString();
				 sDay = String::Concat(sDay, ".");
				 sDay = String::Concat(sDay,systemTime.wMonth.ToString("d2"));
				 sDay = String::Concat(sDay, ".");
				 sDay = String::Concat(sDay,systemTime.wDay.ToString("d2"));
				 sDay = String::Concat(sDay," ");
				 sDay = String::Concat(sDay,systemTime.wHour.ToString("d2"));
				 sDay = String::Concat(sDay,":");
				 sDay = String::Concat(sDay,systemTime.wMinute.ToString("d2"));
				 sDay = String::Concat(sDay,":");
				 sDay = String::Concat(sDay,systemTime.wSecond.ToString("d2"));

				 this->toolStripStatusLabel3->Text = sDay;


			 }

	private: System::Void OsnMirrorCopyUI_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
			 {
				 if(e->KeyCode==Keys::F5) // refresh
				 {

					 if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[0])
					 {
						 //this->RefreshDriverNode();
						 RefreshVolumeListView();
					 }
					 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[1])
					 {
						 this->RefreshDiskListView();
					 }
					 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[2])
					 {
						 this->RefreshMirrorNode();
					 }

					 this->RefreshStatusBar();
				 }

				 if(e->KeyCode == Keys::F1) // help
				 {
					 if(!ImagePath)
						 return;

					 System::Diagnostics::Process	^help = gcnew System::Diagnostics::Process();
					 help->StartInfo->RedirectStandardOutput = false;
					 help->StartInfo->FileName = String::Concat(ImagePath,"\\HostMirrorHandBook(x86).chm");
					 help->StartInfo->UseShellExecute = true;
					 help->Start();

				 }

				 if(e->KeyCode == Keys::F3) // setting
				 {
					 Setting ^pSetting = gcnew Setting(m_InstallType);
					 pSetting->ShowDialog();

				 }

				 if(e->KeyCode == Keys::F4)
				 {
					 // DatabaseCfg ^pDatabaseCfg = gcnew DatabaseCfg();
					 // pDatabaseCfg->ShowDialog();
					 Process^ proc=gcnew Process();
					 try
					 {
						 String^ filepath=String::Concat(ImagePath,"\\OSNDbConf.exe");
						 proc->StartInfo->FileName=filepath;
						 proc->StartInfo->RedirectStandardError=true;
						 proc->StartInfo->RedirectStandardInput=true;
						 proc->StartInfo->RedirectStandardOutput=true;
						 proc->StartInfo->UseShellExecute=false;
						 proc->StartInfo->CreateNoWindow=false;
						 proc->Start();
						 proc->WaitForExit();
					 }
					 catch(Exception^ ex)
					 {
						 MessageBox::Show(String::Concat("打开数据库代理设置界面出现异常:",ex->Message->ToString()));
					 }
					 finally
					 {
						 if(proc!=nullptr)
						 {
							 proc->Close();
						 }
					 }
				 }
			 }

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

	private: array<Byte>^ GetLegalKey()
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
	private: array<Byte>^ GetLegalIV()
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
	private: String^ OSNDecry(String^ msg)
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
					 myEventLog->OSNWriteEventLog(String::Concat("解密时出现异常;",ex->Message->ToString()),EventLogEntryType::Error,0);
					 MessageBox::Show("解密时出现异常");
				 }
				 return nullptr;
			 }
			 /**************************************************************************************************
			 *    FunctionName  : UnSetHotBackupModeAndResumeOracle 
			 *    Input         : NULL
			 *    ReturnValue   : success:0, error:-1
			 *    Summary       : Resume Oracle from suspend state and unset it from hot backup mode  
			 ****************************************************************************************************/
			 int UnSetHotBackupModeAndResumeOracle()
			 {
				 //--Flush cache of oracle
				 Process ^pCmdProcess = gcnew Process();
				 pCmdProcess->StartInfo->FileName = "sqlplus.exe";

				 pCmdProcess->StartInfo->UseShellExecute = false;
				 pCmdProcess->StartInfo->RedirectStandardInput = true;
				 pCmdProcess->StartInfo->RedirectStandardOutput = true;
				 pCmdProcess->StartInfo->RedirectStandardError = true;
				 pCmdProcess->StartInfo->CreateNoWindow = true;

				 pCmdProcess->StartInfo->Arguments = "/nolog";

				 try
				 {
					 pCmdProcess->Start();
					 StreamWriter ^input = pCmdProcess->StandardInput;

					 input->WriteLine("connect /as sysdba");
					 input->Flush();

					 ////*********flush and suspend oracle***********

					 //input->WriteLine("ALTER SYSTEM RESUME;");
					 //input->Flush();
					 input->WriteLine("ALTER TABLESPACE system END BACKUP;");
					 input->Flush();

					 input->Close();

					 pCmdProcess->WaitForExit();
					 ////*********exit flush and suspend oracle***********
				 }
				 catch(...)
				 {
					 if(pCmdProcess)
					 {
						 pCmdProcess->Close();
					 }

					 return -1;
				 }
				 __finally
				 {
					 if(pCmdProcess)
					 {
						 pCmdProcess->Close();
					 }
				 }
				 return 0;
			 }



			 void CreateCDPSchedule(String^ type,String^ parameter)
			 {
				 String ^strRet = nullptr;
				 System::Diagnostics::Process ^newProcess = gcnew System::Diagnostics::Process();
				 try
				 {
					 String^ paralist=String::Concat(type,"@",parameter);
					 String^ filePath=Application::StartupPath+"\\HostMirrorCDPSchedule.exe";
					 newProcess->StartInfo->RedirectStandardOutput = true;
					 newProcess->StartInfo->RedirectStandardInput = true;
					 newProcess->StartInfo->CreateNoWindow = true;
					 newProcess->StartInfo->UseShellExecute = false;
					 newProcess->StartInfo->Arguments=paralist;

					 newProcess->StartInfo->FileName = filePath;
					 newProcess->Start();

					 strRet = newProcess->StandardOutput->ReadToEnd();

					 newProcess->WaitForExit();

				 }
				 catch(...)
				 {
					 return ;
				 }
				 finally
				 {
					 if(newProcess && !newProcess->HasExited)
					 {
						 newProcess->Kill();
					 }
					 if(newProcess)
					 {
						 newProcess->Close();
					 }
				 }
				 return ;

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
			 
	private: System::Void CreateCDPToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count != 1)
				 {
					 MessageBox::Show("请选择一个要建立CDP快照的镜像组。");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }

				 String ^strSrcVolSignOffset = nullptr;
				 String ^strDesVolSignOffset = nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;

				 ListViewItem^ selectItem = this->listView1->SelectedItems[0];
				 strSrcVolSignOffset  = selectItem->SubItems[0]->Text;
				 strDesVolSignOffset = selectItem->SubItems[1]->Text;
				 String ^Property  = selectItem->SubItems[4]->Text;

				 // m_ListViewMutex->ReleaseMutex();
				 //int OracleFlag=2;//2 为非归档模式，0为归档模式
				 try
				 {
					 if(Property->Equals("卷复制"))//(strSrcVolSignOffset->Length > 8)
					 {
						 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcVolSignOffset, strDesVolSignOffset);
						 if(pMirrorInfo == nullptr)
						 {
							 MessageBox::Show("无法获取镜像信息。");
							 return;
						 }

						 pVolumeInfo = this->pDriverList->GetDriverInfoByString(strSrcVolSignOffset);
						 if(!pVolumeInfo)
						 {
							 MessageBox::Show("无法获取此卷信息。");
							 return;
						 }
						 if( MessageBox::Show("创建CDP需要先设置OSNWebmanger上的CDP保存卷，否则会创建CDP失败，是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
						 {
							 return;
						 }

						 String^ para="volumehostmirror,";// String^ para="-para=volumehostmirror,";
						 para=String::Concat(para,pVolumeInfo->m_VolumeLable);
						 CDPArg^ xuarg=gcnew CDPArg();
						 xuarg->argstr=para;
						 try
						 {
							 backgroundCreateCDP->RunWorkerAsync(xuarg);
						 }
						 catch(Exception^ exx)
						 {
							 MessageBox::Show("创建CDP出现异常或者已经有一个在CDP正在创建，请稍后重试!");
							 myEventLog->OsnHostMirrorLog(exx->Message->ToString());
							 return;

						 }
					 }
					 else //disk volume hostmirror
					 {
						 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcVolSignOffset,true);
						 if(pMirrorInfo == nullptr)
						 {
							 MessageBox::Show("无法获取镜像信息。");
							 return;
						 }

						 pDiskInfo = this->pDiskList->GetDiskInfo(strSrcVolSignOffset);
						 if(!pDiskInfo)
						 {
							 MessageBox::Show("无法获取此磁盘信息。");
							 return;
						 }
						 if( MessageBox::Show("创建CDP需要先设置OSNWebmanger上的CDP保存卷，否则会创建CDP失败，是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
						 {
							 return;
						 }

						 String^ para="diskhostmirror,";//String^ para="-para=diskhostmirror,";
						 para=String::Concat(para,Convert::ToString(pDiskInfo->m_DiskIndex));

						 CDPArg^ xuarg=gcnew CDPArg();
						 xuarg->argstr=para;
						 try
						 {
							 backgroundCreateCDP->RunWorkerAsync(xuarg);
						 }
						 catch(Exception^ exx)
						 {
							 MessageBox::Show("创建CDP出现异常或者已经有一个在CDP正在创建，请稍后重试!");
							 myEventLog->OsnHostMirrorLog(exx->Message->ToString());
							 return;
						 }

					 }
				 }
				 catch(Exception ^err)
				 {
					 String ^str = "创建CDP 失败,错误信息";
					 str =String::Concat(str,err);
					 MessageBox::Show(str);
				 }

			 }
	private: System::Void DelCDPScheuleToolStripMenu_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 //m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count == 0)
				 {
					 MessageBox::Show("请先选择创建了CDP任务计划的镜像组。");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }
				 // m_ListViewMutex->ReleaseMutex();

				 String ^strSrcguid = nullptr;
				 String ^strDesguid = nullptr;
				 String ^strSrcLable = nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;
				 String ^strParaMeter=nullptr;

				 if(MessageBox::Show("您确定要删除CDP任务计划？","",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
				 {
					 bool bFindNoSch = false;

					 // m_ListViewMutex->WaitOne();

					 try
					 {
						 for(int i=0; i<listView1->SelectedItems->Count; i++)
						 {
							 strSrcguid = listView1->SelectedItems[i]->SubItems[0]->Text;
							 strDesguid = listView1->SelectedItems[i]->SubItems[1]->Text;
							 String^ Property=listView1->SelectedItems[i]->SubItems[4]->Text;
							 String^ isSchedule=listView1->SelectedItems[i]->SubItems[7]->Text;
							 if(isSchedule->Equals("Manual"))
							 {
								 MessageBox::Show("该镜像关系没有设置CDP计划，不用删除CDP计划");
								 continue;
							 }
							 if(Property->Equals("卷复制"))
							 {
								 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcguid, strDesguid);
								 if(pMirrorInfo == nullptr)
								 {
									 MessageBox::Show("无法获取镜像信息。");
									 continue;
								 }

								 pVolumeInfo = this->pDriverList->GetDriverInfoByString(strSrcguid);
								 if(!pVolumeInfo)
								 {
									 MessageBox::Show("无法获取此卷信息。");
									 continue;
								 }

								 strSrcLable = pVolumeInfo->m_VolumeLable->Substring(0,1)->ToUpper();

								 strParaMeter = String::Concat(strSrcguid,"#");
								 strParaMeter = String::Concat(strParaMeter,"Volume ");
							 }
							 else
							 {
								 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcguid,true);
								 if(pMirrorInfo == nullptr)
								 {
									 MessageBox::Show("无法获取镜像信息。");
									 continue;
								 }

								 pDiskInfo = this->pDiskList->GetDiskInfo(strSrcguid);
								 if(!pDiskInfo)
								 {
									 MessageBox::Show("无法获取此磁盘信息。");
									 continue;
								 }

								 // strSrcLable = pDiskInfo->m_DiskIndex.ToString();
								 // String ^strSrcDiskSignOffset = String::Concat(strSrcSignOffset,"_00000000");
								 strParaMeter = String::Concat(pDiskInfo->m_Guid,"#");
								 strParaMeter = String::Concat(strParaMeter,"Disk ");
							 }

							 /*Task ^t = OSNTaskSchedule::GetTaskByTaskName(strSrcLable);
							 if(t)
							 {
							 bool bRet = OSNTaskSchedule::DeleteTaskSchedule(t->Name);
							 if(!bRet)
							 {
							 bFindNoSch = true;
							 //String* strMsg = "Failed to delete taskSchedule.";
							 MessageBox::Show("删除CDP任务计划失败。");
							 continue;
							 }
							 }*/
							 try
							 {
								 String^ temppara=String::Concat("delete","@",strParaMeter);
								 backgroundCDPSchedule->RunWorkerAsync(temppara);
							 }
							 catch(Exception^ exx)
							 {
								 MessageBox::Show("有些镜像的CDP计划删除失败，请手动删除");
								 myEventLog->OsnHostMirrorLog(String::Concat("删除CDP计划出现异常,error=",exx->Message->ToString()));
							 }
						 }
						 // m_ListViewMutex->ReleaseMutex();
						 //CreateCDPScheduleDelegate^ schedule=gcnew CreateCDPScheduleDelegate(this,&VolumeCopyUI::OsnMirrorCopyUI::CreateCDPSchedule);
						 //schedule->Invoke("delete",strParaMeter);

					 }
					 catch(Exception^ exx)
					 {
						 MessageBox::Show("删除镜像关系时出现异常，有些镜像可能没有删除成功，请重新删除");
						 myEventLog->OsnHostMirrorLog(String::Concat("DelCDPScheuleToolStripMenu_Click error=",exx->Message->ToString()));
						 return;
					 }
					 //CreateCDPSchedule("delete",strParaMeter);
					 /* if(!bFindNoSch)
					 {
					 //String* strMsg = "Succeed to delete the taskSchedules which you choose.";
					 MessageBox::Show("成功删除您所选择的CDP任务计划。");
					 }*/

					 // RefreshMirrorNode();
				 }
			 }
	private: System::Void ViewCDPScheduleToolStripMenu_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count != 1)
				 {
					 MessageBox::Show("请先选择一个创建了CDP任务计划的镜像组。");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }

				 String ^strSrcLable = nullptr;
				 String ^strSrcVolSignOffset = nullptr;
				 String ^strDesVolSignOffset = nullptr;
				 String ^strParaMeter=nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;

				 ListViewItem^ selectItem = this->listView1->SelectedItems[0];
				 strSrcVolSignOffset  = selectItem->SubItems[0]->Text;
				 strDesVolSignOffset = selectItem->SubItems[1]->Text;
				 String^ Property   =selectItem->SubItems[4]->Text;
				 String^ isSchedule=selectItem->SubItems[7]->Text;
				 // m_ListViewMutex->ReleaseMutex();

				 if(Property->Equals("卷复制"))
				 {
					 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcVolSignOffset, strDesVolSignOffset);
					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息。");
						 return;
					 }

					 pVolumeInfo = this->pDriverList->GetDriverInfoByString(strSrcVolSignOffset);
					 if(!pVolumeInfo)
					 {
						 MessageBox::Show("无法获取此卷信息。");
						 return;
					 }

					 strSrcLable = pVolumeInfo->m_VolumeLable->Substring(0,1)->ToUpper();
					 strParaMeter = String::Concat(pVolumeInfo->m_GUID,"#");
					 strParaMeter = String::Concat(strParaMeter,"Volume ");
				 }
				 else
				 {
					 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcVolSignOffset,true);
					 if(pMirrorInfo == nullptr)
				  {
					  MessageBox::Show("无法获取镜像信息。");
					  return;
					 }

					 pDiskInfo = this->pDiskList->GetDiskInfo(strSrcVolSignOffset);
					 if(!pDiskInfo)
					 {
						 MessageBox::Show("无法获取此磁盘信息。");
						 return;
					 }

					 strSrcLable = pDiskInfo->m_DiskIndex.ToString();
					 //String ^strSrcDiskSignOffset = String::Concat(strSrcVolSignOffset,"_00000000");
					 strParaMeter = String::Concat(pDiskInfo->m_Guid,"#");
					 strParaMeter = String::Concat(strParaMeter,"Disk ");
				 }
				 if(isSchedule->Equals("Schedule"))
				 {
					 try
					 {
						 String^ temppara=String::Concat("view","@",strParaMeter);
						 backgroundCDPSchedule->RunWorkerAsync(temppara);
					 }
					 catch(Exception^ exx)
					 {
						 MessageBox::Show("已经有一个添加，删除或者是查看CDP计划的进程在执行，请稍后重试");
						 myEventLog->OsnHostMirrorLog(String::Concat("查看CDP计划出现异常,error=",exx->Message->ToString()));
					 }
				 }
				 else 
				 {
					 MessageBox::Show("该镜像关系没有设置CDP计划");
				 }
				 /*if(m_strOSVersion->Contains("2003") || m_strOSVersion->ToUpper()->Contains("XP") || m_strOSVersion->Contains("2000"))
				 {
				 Task ^t = OSNTaskSchedule::GetTaskByTaskName(strSrcLable);
				 if(t)
				 {
				 bool bOK = t->DisplayPropertySheet();
				 if(bOK)
					{
					t->Save();
					}
					t->Close();
				 }
				 else
				 {
				 //String* strMsg = "Could not find the taskSchedule.";
				 MessageBox::Show("此任务计划不存在。");
				 return;
				 }
				 }*/
			 }
	private: System::Void CreateCDPScheduleToolStripMenu_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count != 1)
				 {
					 MessageBox::Show("请选择一个要建立CDP任务计划的镜像组。");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }

				 String ^strSrcVolSignOffset = nullptr;
				 String ^strDesVolSignOffset = nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;

				 String ^strTaskName = "";
				 String ^strAppName  = String::Concat(this->ImagePath,"\\OSNHostMirrorCDPAgent.exe");
				 String ^strParaMeter  = "";
				 String ^strWorkDir     = ImagePath;
				 String ^strComment   = "";

				 ListViewItem^ selectItem =nullptr;
				 try
				 {
					 selectItem=this->listView1->SelectedItems[0];
				 }
				 catch(Exception^ exx)
				 {
					 MessageBox::Show("您未选中镜像关系，请选择镜像关系");
					 myEventLog->OsnHostMirrorLog(String::Concat("CreateCDPScheduleToolStripMenu_Click error=",exx->Message->ToString()));
					 return;
				 }
				 strSrcVolSignOffset  = selectItem->SubItems[0]->Text;
				 strDesVolSignOffset = selectItem->SubItems[1]->Text;
				 String^ Property   = selectItem->SubItems[4]->Text;

				 // m_ListViewMutex->ReleaseMutex();

				 try
				 {
					 if(Property->Equals("卷复制"))
					 {
						 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcVolSignOffset, strDesVolSignOffset);
						 if(pMirrorInfo == nullptr)
						 {
							 MessageBox::Show("无法获取镜像信息。");
							 return;
						 }

						 pVolumeInfo = this->pDriverList->GetDriverInfoByString(strSrcVolSignOffset);
						 if(!pVolumeInfo)
						 {
							 MessageBox::Show("无法获取此卷信息。");
							 return;
						 }

						 if( MessageBox::Show("创建CDP需要先设置OSNWebmanger上的CDP保存卷，否则会创建CDP失败，是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
						 {
							 return;
						 }
						 String ^strSrcVolumeLable = pVolumeInfo->m_VolumeLable->Substring(0,1)->ToUpper();
						 ////The taskName can not contain  '<', '>', ':', '/', '\', '|'
						 //strTaskName = String::Concat("OSNHostMirrorCDPAgent_Volume_",strSrcVolumeLable);
						 // strTaskName = strTaskName->Replace(":","");

						 // strParaMeter = String::Concat(strSrcVolumeLable," ",strSrcVolSignOffset," ");
						 // strParaMeter = String::Concat(strParaMeter,"Volume ",pMirrorInfo->m_EIMMode.ToString());
						 strParaMeter = String::Concat(pVolumeInfo->m_GUID,"#");
						 strParaMeter = String::Concat(strParaMeter,"Volume ");
					 }
					 else
					 {
						 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcVolSignOffset,true);
						 if(pMirrorInfo == nullptr)
						 {
							 MessageBox::Show("无法获取镜像信息。");
							 return;
						 }

						 pDiskInfo = this->pDiskList->GetDiskInfo(strSrcVolSignOffset);
						 if(!pDiskInfo)
						 {
							 MessageBox::Show("无法获取此磁盘信息。");
							 return;
						 }

						 if( MessageBox::Show("创建CDP需要先设置OSNWebmanger上的CDP保存卷，否则会创建CDP失败，是否继续","",MessageBoxButtons::YesNo)!=System::Windows::Forms::DialogResult::Yes)
						 {
							 return;
						 }
						 // strTaskName = String::Concat("OSNHostMirrorCDPAgent_Disk_",pDiskInfo->m_DiskIndex.ToString());

						 // String ^strSrcDiskSignOffset = String::Concat(strSrcVolSignOffset,"_00000000");
						 //strParaMeter = String::Concat(pDiskInfo->m_DiskIndex.ToString()," ",strSrcDiskSignOffset," ");
						 //strParaMeter = String::Concat(strParaMeter,"Disk ",pMirrorInfo->m_EIMMode.ToString());
						 strParaMeter = String::Concat(pDiskInfo->m_Guid,"#");
						 strParaMeter = String::Concat(strParaMeter,"Disk ");
					 }

					 // int iRetCode = CreateScheduleTask(strTaskName,strAppName,strParaMeter,
					 // strComment, strWorkDir);
					 //CreateCDPScheduleDelegate^ schedule=gcnew CreateCDPScheduleDelegate(this,&VolumeCopyUI::OsnMirrorCopyUI::CreateCDPSchedule);
					 //schedule->Invoke("add",strParaMeter);
					 try
					 {
						 String^ temppara=String::Concat("add","@",strParaMeter);
						 backgroundCDPSchedule->RunWorkerAsync(temppara);
					 }
					 catch(Exception^ exx)
					 {
						 MessageBox::Show("已经有一个添加，删除或者是查看CDP计划的进程在执行，请稍后重试");
						 myEventLog->OsnHostMirrorLog(String::Concat("添加CDP计划出现异常,error=",exx->Message->ToString()));
					 }
					 // int iRetCode=CreateCDPSchedule();

					 //RefreshMirrorNode();
				 }
				 catch(Exception ^ex)
				 {
					 MessageBox::Show(ex->ToString());
				 }
			 }


	private: System::Void DatabaseConfigToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 // DatabaseCfg ^pDatabaseCfg = gcnew DatabaseCfg();
				 //pDatabaseCfg->ShowDialog();
				 try
			  {
				  backgroundDBConf->RunWorkerAsync();
			  }
				 catch(Exception^ exx)
			  {
				  MessageBox::Show("已经有一个数据库设置界面已经打开，请先关闭打开的数据库设置界面");
				  myEventLog->OsnHostMirrorLog(exx->Message->ToString());
				  return;
			  }


			 }
	private:System::Void ClusterIPToolStripMenuItem_Click(System::Object^ sender,System::EventArgs^ e)
		 {
			 SetClusterIP ^setClusterIP=gcnew SetClusterIP();
			 if(setClusterIP->ShowDialog()!=System::Windows::Forms::DialogResult::Yes)
			 {
				 return ;
			 }
		 }
	private: System::Void backgroundCDPSchedule_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
			 {
				 String^ arg=(String^)e->Argument;
				 array<Char>^chars = {'@'};
				 array<String^>^ args=arg->Split(chars);
				 CreateCDPSchedule(args[0],args[1]);
			 }
	private: System::Void backgroundCDPSchedule_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
			 {
				 RefreshMirrorNode();
			 }


	private: System::Void backgroundCreateCDP_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 CDPArg^ agr=(CDPArg^)(e->Argument);

				 Process^ proc=gcnew Process();
				 try
				 {
					 String^ filepath=String::Concat(ImagePath,"\\OSNDBProxy.exe");
					 proc->StartInfo->FileName=filepath;
					 proc->StartInfo->RedirectStandardError=true;
					 proc->StartInfo->RedirectStandardInput=true;
					 proc->StartInfo->RedirectStandardOutput=true;
					 proc->StartInfo->UseShellExecute=false;
					 proc->StartInfo->CreateNoWindow=true;
					 proc->StartInfo->Arguments = agr->argstr;
					 proc->Start();
					 proc->WaitForExit();
					 
					 if(proc->ExitCode == 0)
						 MessageBox::Show("创建CDP快照成功");
					 else
						 MessageBox::Show("创建CDP快照失败或正在创建第一个CDP快照");
				 }
				 catch(Exception^ ex)
				 {
					 MessageBox::Show(String::Concat("创建cdp快照失败:",ex->Message->ToString()));
				 }
				 finally
				 {
					 if(proc!=nullptr)
					 {
						 proc->Close();
					 }
				 }
			 }
	private: System::Void FileToolStripMenu_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

			 }


	private: System::Void OsnMirrorCopyUI_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 RegisterHotKey(this->Handle,8880,Alt,Keys::F3);
				 RegisterHotKey(this->Handle,8881,Alt,Keys::F4);
				 RegisterHotKey(this->Handle,8882,Alt,Keys::F5);

			 }
	private: System::Void OsnMirrorCopyUI_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
			 {
				 UnregisterHotKey(this->Handle,8880);
				 UnregisterHotKey(this->Handle,8881);
				 UnregisterHotKey(this->Handle,8882);
			 }
	private: void ProcessHotkey(System::Windows::Forms::Message^ m)
			 {
				 IntPtr id= m->WParam;
				 int sid = id.ToInt32();
				 switch(sid)
				 {
				 case 8880:
					 {
						 Setting ^pSetting = gcnew Setting(m_InstallType);
						 pSetting->ShowDialog();
						 break;
					 }
				 case 8881:
					 {
						 Process^ proc=gcnew Process();
						 try
						 {
							 String^ filepath=String::Concat(ImagePath,"\\OSNDbConf.exe");
							 proc->StartInfo->FileName=filepath;
							 proc->StartInfo->RedirectStandardError=true;
							 proc->StartInfo->RedirectStandardInput=true;
							 proc->StartInfo->RedirectStandardOutput=true;
							 proc->StartInfo->UseShellExecute=false;
							 proc->StartInfo->CreateNoWindow=false;
							 proc->Start();
							 proc->WaitForExit();
						 }
						 catch(Exception^ ex)
						 {
							 MessageBox::Show(String::Concat("打开数据库代理设置界面出现异常:",ex->Message->ToString()));
						 }
						 finally
						 {
							 if(proc!=nullptr)
							 {
								 proc->Close();
							 }
						 }
						 break;
					 }
				 case 8882:
					 {
						 this->pDriverList->Clear();
						 this->pVolumeMirrorList->Clear();
						 this->pDiskMirrorList->Clear();
						 this->pDiskList->Clear();     

						 GetSystemDriversInfo();
						 GetSystemDisksInfo(); 

						 ReadConfigurationFile();
						 GetSystemMirrorInfo();
						 WriteConfigurationFile();

						 if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[0])
						 {
							 this-> RefreshVolumeListView();
						 }
						 else   if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[1])
						 {
							 this->RefreshDiskListView();
						 }
						 else if(this->treeView1->SelectedNode == treeView1->Nodes[0]->Nodes[2])
						 {
							 this->RefreshMirrorNode();
						 }
						 break;
					 }
				 }

			 }
	protected:
		[SecurityPermission(SecurityAction::Demand, Flags=SecurityPermissionFlag::UnmanagedCode)]
		virtual void WndProc( Message% m ) override

		{
			//const int WM_HOTKEY = 0x0312;//如果m.Msg的值为0x0312那么表示用户按下了热键
			switch (m.Msg)
			{
			case WM_HOTKEY:
				ProcessHotkey(m);//按下热键时调用ProcessHotkey()函数
				break;
			}
			Form::WndProc(m); //将系统消息传递自父类的WndProc

		}
	private: System::Void backgroundDBConf_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 Process^ proc=gcnew Process();
				 try
				 {
					 String^ filepath=String::Concat(ImagePath,"\\OSNDbConf.exe");
					 proc->StartInfo->FileName=filepath;
					 proc->StartInfo->RedirectStandardError=true;
					 proc->StartInfo->RedirectStandardInput=true;
					 proc->StartInfo->RedirectStandardOutput=true;
					 proc->StartInfo->UseShellExecute=false;
					 proc->StartInfo->CreateNoWindow=false;
					 proc->Start();
					 proc->WaitForExit();
				 }
				 catch(Exception^ ex)
				 {
					 MessageBox::Show(String::Concat("打开数据库代理设置界面出现异常:",ex->Message->ToString()));
				 }
				 finally
				 {
					 if(proc!=nullptr)
					 {
						 proc->Close();
					 }
				 }
			 }
	private: System::Void ToolStripResourceName_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 				 // m_ListViewMutex->WaitOne();
				 if(this->listView1->SelectedItems->Count != 1)
				 {
					 MessageBox::Show("请先选择一个镜像关系。");
					 // m_ListViewMutex->ReleaseMutex();
					 return;
				 }

				 String ^strSrcLable = nullptr;
				 String ^strSrcVolSignOffset = nullptr;
				 String ^strDesVolSignOffset = nullptr;
				 String ^strParaMeter=nullptr;
				 CMirrorInfo ^pMirrorInfo = nullptr;
				 CDriverInfo ^pVolumeInfo = nullptr;
				 CDiskInfo ^pDiskInfo = nullptr;

				 ListViewItem^ selectItem = this->listView1->SelectedItems[0];
				 strSrcVolSignOffset  = selectItem->SubItems[0]->Text;
				 strDesVolSignOffset = selectItem->SubItems[1]->Text;
				 String^ Property   =selectItem->SubItems[4]->Text;
				 // m_ListViewMutex->ReleaseMutex();

				 if(Property->Equals("卷复制"))
				 {
					 pMirrorInfo = this->pVolumeMirrorList->GetMirrorInfo(strSrcVolSignOffset, strDesVolSignOffset);
					 if(pMirrorInfo == nullptr)
					 {
						 MessageBox::Show("无法获取镜像信息。");
						 return;
					 }
					 if(pMirrorInfo->m_IsCluster == 0)
					 {
						 MessageBox::Show("该镜像关系不是集群模式！");
						 return ;
					 }
				 }
				 else
				 {
					 pMirrorInfo = this->pDiskMirrorList->GetDiskMirrorInfo(strSrcVolSignOffset,true);
					 if(pMirrorInfo == nullptr)
				  {
					  MessageBox::Show("无法获取镜像信息。");
					  return;
				  }
					 if(pMirrorInfo->m_IsCluster == 0)
					 {
						 MessageBox::Show("该镜像关系不是集群模式!");
						 return ;
					 }
				 }
				 ClusterResourceName ^name = gcnew ClusterResourceName(pMirrorInfo->m_ClusterResourceName);
				 if(name->ShowDialog() == System::Windows::Forms::DialogResult::Yes)
				 {
					 pMirrorInfo->m_ClusterResourceName = name->ResourceName;
					 WriteConfigurationFile();
					 MessageBox::Show("设置成功");
				 }
			 }
private: System::Void transferModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(this->listView1->SelectedItems->Count!=1)
			 {
				 MessageBox::Show("请选择驱动器.");
				 return;
			 }
			 ListViewItem ^SelectItem =nullptr;
			 try
			 {
				 SelectItem=this->listView1->SelectedItems[0];
			 }
			 catch(Exception^ exx)
			 {
				 MessageBox::Show("请选择镜像关系");
				 myEventLog->OsnHostMirrorLog(String::Concat("transferMode_Click error=",exx->Message->ToString()));
				 return;
			 }
			 String ^Srcguid  = SelectItem->SubItems[0]->Text;
			 String ^Property  = SelectItem->SubItems[4]->Text;

			 RegistryKey ^pRegKey = Registry::LocalMachine;
			 RegistryKey ^pParmeterKey;

			 if(Property->Equals("卷复制"))
			 {
				 String^ tranVolumeRoot = "System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\AsynVolume\\";
				 pParmeterKey = pRegKey->OpenSubKey(tranVolumeRoot,true);
			 }
			 else if(Property->Equals("磁盘复制"))
			 {
				 String^ tranDiskRoot = "System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\AsynDisk\\";
				 pParmeterKey = pRegKey->OpenSubKey(tranDiskRoot,true);
			 }

			 if(pParmeterKey != nullptr)
			 {
				 String^trankey = String::Concat(String::Concat("{",Srcguid,"}"),"Asyn");
					
				 if(0 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				 {
					  pParmeterKey->SetValue(trankey,1);
				 }
				 else if(1 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				 {
					  pParmeterKey->SetValue(trankey,0);
				 }
				 pParmeterKey->Close();
			 }
			 pRegKey->Close();
		 }
private: System::Void transferMode1ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 bool diskorvolume;
			 ASYN_MARKER buffer;
			 int status;

		     memset(&buffer, 0, sizeof(ASYN_MARKER));

			 if(this->listView1->SelectedItems->Count!=1)
			 {
				 MessageBox::Show("请选择驱动器.");
				 return;
			 }
			 ListViewItem ^SelectItem =nullptr;
			 try
			 {
				 SelectItem=this->listView1->SelectedItems[0];
			 }
			 catch(Exception^ exx)
			 {
				 MessageBox::Show("请选择镜像关系");
				 myEventLog->OsnHostMirrorLog(String::Concat("transferMode_Click error=",exx->Message->ToString()));
				 return;
			 }
			 String ^Srcguid  = SelectItem->SubItems[0]->Text;
			 String ^Property  = SelectItem->SubItems[4]->Text;

			 RegistryKey ^pRegKey = Registry::LocalMachine;
			 RegistryKey ^pParmeterKey;
			 char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(Srcguid);
			 OsnGUIDFromString(str2,&buffer.m_VolumeID.SAN_VolumeID.m_VolumeGuid);

			 if(Property->Equals("卷复制"))
			 {
				 String^ tranVolumeRoot = "System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\AsynVolume\\";
				 pParmeterKey = pRegKey->OpenSubKey(tranVolumeRoot,true);
				 diskorvolume = false;
			 }
			 else if(Property->Equals("磁盘复制"))
			 {
				 String^ tranDiskRoot = "System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\AsynDisk\\";
				 pParmeterKey = pRegKey->OpenSubKey(tranDiskRoot,true);
				 diskorvolume = true;
			 }

			 
			 if(pParmeterKey != nullptr)
			 {
				 String^trankey = String::Concat(String::Concat("{",Srcguid,"}"),"Asyn");
					
				 if(0 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				 {
					 memcpy(buffer.m_Marker, "_ASYN_",sizeof("_ASYN_"));
					  try
					  {
						  if(diskorvolume == false)
						  {
							  status = OsnVolumeMark(&buffer);
							  if(status == 0)
							  {
								  pParmeterKey->SetValue(trankey,1);
							  }
						  }
						  else
						  {
							 status = OsnDiskMark(&buffer);
							 if(status == 0)
							 {
								 pParmeterKey->SetValue(trankey,1);
							 }
						  }
					  }
					  catch(Exception ^ex0)
				      {
							Console::WriteLine(String::Concat("ASYN error=",ex0->Message->ToString()));
					  }
				 }
				 else if(1 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				 {
					 memcpy(buffer.m_Marker, "_SYN_",sizeof("_SYN_"));
					  try
					  {
						  if(diskorvolume == false)
						  {
							  status = OsnVolumeMark(&buffer);
							  if(status == 0)
							  {
								   pParmeterKey->SetValue(trankey,0);
							  }
						  }
						  else
						  {
							  status = OsnDiskMark(&buffer);
							  if(status == 0)
							  {
								   pParmeterKey->SetValue(trankey,0);
							  }
						  }
					  }
					  catch(Exception ^ex0)
				      {
							Console::WriteLine(String::Concat("SYN error=",ex0->Message->ToString()));
					  }
				 }
				 pParmeterKey->Close();
			 }
			 pRegKey->Close();
		 }

		 bool SynOrAsyn(bool volumeordisk,CMirrorInfo ^pMirrorInfo)
		 {
			 bool synorasyn;
			 RegistryKey ^pParmeterKey;
			 RegistryKey ^pRegKey = Registry::LocalMachine;

			 if(volumeordisk == true)
			 {
				String^ tranDiskroot = "System\\CurrentControlSet\\Services\\OsnDSS\\Parameters\\AsynDisk\\";
				pParmeterKey = pRegKey->OpenSubKey(tranDiskroot,true);
			 }
			 else
			 {
				String^ tranVolumeroot = "System\\CurrentControlSet\\Services\\OsnVSS\\Parameters\\AsynVolume\\";
				pParmeterKey = pRegKey->OpenSubKey(tranVolumeroot,true);
			 }
			 
			 if(pParmeterKey != nullptr)
			 {
				String^trankey = String::Concat(String::Concat("{",pMirrorInfo->m_SourceGuid,"}"),"Asyn");
				
				if(0 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				{
					synorasyn = false;
				}
				else if(1 == Convert::ToUInt32(pParmeterKey->GetValue(trankey)))
				{
					synorasyn = true;
				}
				pParmeterKey->Close();
		     }
			 pRegKey->Close();
			 return synorasyn;
		 }

};
}

