#pragma once

#include "MyClusApi.h"
#include <locale.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;
using namespace System::Runtime::InteropServices;
using namespace System::Management;

namespace VolumeCopyUI {

	/// <summary>
	/// Summary for ClusterResourceName
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ClusterResourceName : public System::Windows::Forms::Form
	{
	public:
		ClusterResourceName(String ^name)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			ResourceName = name;
			arraylist =gcnew ArrayList();
			if(CheckClusterResourceNameBind())
			{
				this->cmbResource->DataSource = arraylist;
				for(int i = 0;i<arraylist->Count;i++)
				{
					if(ResourceName == arraylist[i]->ToString())
						this->cmbResource->SelectedIndex = i;
				}
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ClusterResourceName()
		{
			if (components)
			{
				delete components;
			}
		}

	public: String ^ResourceName;
	public: ArrayList ^arraylist;
	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::ComboBox^  cmbResource;
	public: 
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
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->cmbResource = (gcnew System::Windows::Forms::ComboBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(45, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(173, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"配置源盘在集群资源中的名称：";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(29, 113);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ClusterResourceName::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(143, 113);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 3;
			this->btnCancel->Text = L"取消";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &ClusterResourceName::btnCancel_Click);
			// 
			// cmbResource
			// 
			this->cmbResource->FormattingEnabled = true;
			this->cmbResource->Location = System::Drawing::Point(47, 57);
			this->cmbResource->Name = L"cmbResource";
			this->cmbResource->Size = System::Drawing::Size(148, 20);
			this->cmbResource->TabIndex = 4;
			// 
			// ClusterResourceName
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(291, 148);
			this->Controls->Add(this->cmbResource);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->label1);
			this->Name = L"ClusterResourceName";
			this->Text = L"ClusterResourceName";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if(cmbResource->SelectedValue->ToString() == "")
				 {
					 MessageBox::Show("请配置源盘在集群资源中的名称！");
					 return ;
				 }

				 ResourceName = this->cmbResource->SelectedValue->ToString();
				 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
				 return ;
			 }
	private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->DialogResult = System::Windows::Forms::DialogResult::No;
				 return ;
			 }
			 private: bool CheckClusterResourceNameBind()
			 {
				LPCLUSTERVERSIONINFO lpClusterInfo;
				HCLUSENUM hclusenum=NULL;
				HCLUSTER hcluster=NULL;
				WCHAR	clusterName[128] ;	
				WCHAR	nodeName[10][128];
				WCHAR	*pResourceName;
				DWORD	chClusterName =128*sizeof(WCHAR);
				DWORD	dwtype;
				DWORD	enumCount;
				DWORD	chNodeNameLen;
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
						return  false;
					}
					lpClusterInfo->dwVersionInfoSize = sizeof(CLUSTERVERSIONINFO);
					ret = GetClusterInformation(hcluster,clusterName,&chClusterName,lpClusterInfo);
					
					if(ret !=ERROR_SUCCESS)
					{
						free(lpClusterInfo);
						CloseCluster(hcluster);
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

				
				//CloseClusterNode(hnode);
				ClusterCloseEnum(hclusenum);
				CloseCluster(hcluster);
				free(lpClusterInfo);
				return true;
			 }
};
}
