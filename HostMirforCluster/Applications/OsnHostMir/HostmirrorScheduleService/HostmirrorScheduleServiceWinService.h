#pragma once
#include "MainProcess.h"
#include "CDPScheduleThread.h"
#include "eventlog.h"
using namespace System;
using namespace System::Collections;
using namespace System::ServiceProcess;
using namespace System::ComponentModel;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Threading;
using namespace System::Xml::Serialization;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace OSNRpcCommonCfg;



namespace HostmirrorScheduleService {

	/// <summary>
	/// Summary for HostmirrorScheduleServiceWinService
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class HostmirrorScheduleServiceWinService : public System::ServiceProcess::ServiceBase
	{
	public:
		HostmirrorScheduleServiceWinService()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HostmirrorScheduleServiceWinService()
		{
			if (components)
			{
				delete components;
			}
		}

		/// <summary>
		/// Set things in motion so your service can do its work.
		/// </summary>
		virtual void OnStart(array<String^>^ args) override
		{
			// TODO: Add code here to start your service.
			m_MsgBuffer =gcnew array<Byte>(1024*512);
			m_CDPScheduleXmlMutex = gcnew Mutex(false);
			m_MemoryStreamMutex   = gcnew Mutex(false);
			m_RegMutex=gcnew Mutex(false);
			m_CDPScheduleList=gcnew List<CHostMirrorCDPScheduleThread^>();
			//InitializeLocalInfo();
			InitializeCDPSchedule();
			m_MainProcess = gcnew MainProcess(m_CDPScheduleList);
			try
			{
				m_pSocketMsgInfo = gcnew OSNRpcCommonCfg::CSocketMsgInfo();
				m_pDesSerializer = gcnew XmlSerializer(m_pSocketMsgInfo->GetType());
				m_MemStream      = gcnew MemoryStream(m_MsgBuffer);
				
				OsnListenerDelegate^ listenDelegate = gcnew OsnListenerDelegate(this,&HostmirrorScheduleServiceWinService::ListenProcess); 
				listenDelegate->BeginInvoke(nullptr,nullptr);
			}
			catch(Exception^ ex)
			{
				CEventLog::OSNWriteEventLog(ex->ToString(),EventLogEntryType::Error,002);
			}
		}

		/// <summary>
		/// Stop this service.
		/// </summary>
		virtual void OnStop() override
		{
			// TODO: Add code here to perform any tear-down necessary to stop your service.
		}

		 void InitializeCDPSchedule()
	   {
			m_CDPScheduleList->Clear();

			String^ strXmlFileName = String::Concat(AppDomain::CurrentDomain->BaseDirectory,"\\HostMirrorCDPSchedule.xml");
			if(File::Exists(strXmlFileName))
			{
				//read the MapCDPSchedule.xml
				m_CDPScheduleXmlMutex->WaitOne();
				XmlDocument^ xmlDoc = gcnew XmlDocument();
				xmlDoc->Load(strXmlFileName);

				XmlNodeList^ nodeList = xmlDoc->SelectNodes("//CHostMirrorCDPSchedule");
				for(int i=0; i<nodeList->Count; i++)
				{
					XmlNode^ xmlNode = nodeList->Item(i);
					String^ strSrcDiskSign = xmlNode->ChildNodes->Item(0)->InnerText;
					OsnNewCDPScheduleThread(strSrcDiskSign,strXmlFileName);
				}
				m_CDPScheduleXmlMutex->ReleaseMutex();
			}
		}
	public:void ListenProcess()
		{
			try
			{
				AsyncCallback^ myCallBack = gcnew AsyncCallback(this,&HostmirrorScheduleServiceWinService::OnAcceptSocketCallback);
				m_dwPort=59292;
				m_TcpListener = gcnew TcpListener(System::Net::IPAddress::Any, m_dwPort);
				m_TcpListener->Start();

				while(true)
				{
					m_pConnectEvent->Reset();
					m_TcpListener->BeginAcceptSocket(myCallBack, m_TcpListener);
					m_pConnectEvent->WaitOne();
				}
			}
			catch(Exception^ ex)
			{
#ifdef TRACE
				System::Diagnostics::Trace::WriteLine(String::Format("SCA:Listen Exception:{0}",ex->Message));
#endif
				CEventLog::OSNWriteEventLog(ex->ToString(),EventLogEntryType::Error,002);
			}
			__finally
			{
				if(m_TcpListener != nullptr)
				{
					m_TcpListener->Stop();
				}

				OsnListenerDelegate^ listenerDelagate = gcnew OsnListenerDelegate(this,&HostmirrorScheduleServiceWinService::ListenProcess);
				listenerDelagate->BeginInvoke(nullptr, nullptr);
			}
		}
	   void OnAcceptSocketCallback(IAsyncResult^ pResult)
	   {
			TcpListener^ pListener = nullptr;
			Socket^      pSocket   = nullptr;
			bool         bMutexFlag = false;   ////default is false means not owned
			try
			{
				pListener = static_cast<TcpListener^>(pResult->AsyncState);
				pSocket   = pListener->EndAcceptSocket(pResult);
				m_pConnectEvent->Set();

				m_MemoryStreamMutex->WaitOne();
				bMutexFlag = true;
				m_MsgBuffer->Initialize();
				
				int byteCount = pSocket->Receive(m_MsgBuffer);
				if(byteCount > 0)
				{
					m_MsgBuffer[byteCount] = '\0';
					MemoryStream^ recvStream = gcnew MemoryStream(m_MsgBuffer, 0,byteCount);
					//m_MemStream->Seek(0,SeekOrigin::Begin);
					
					//m_pSocketMsgInfo = static_cast<CSocketMsgInfo^>(m_pDesSerializer->Deserialize(m_MemStream));
					XmlDocument ^recvDoc = gcnew XmlDocument();
					recvDoc->Load(recvStream);
					XmlElement ^recvElement0 = (XmlElement^)recvDoc->SelectSingleNode("CSocketMsgInfo/m_SendCommand");
					if(recvElement0 == nullptr)
					{
						throw("recvElement0 is null");
					}
					XmlElement ^recvElement1 = (XmlElement^)recvDoc->SelectSingleNode("CSocketMsgInfo/m_ParaNum");
					if(recvElement1 == nullptr)
					{
						throw("recvElement1 is null");
					}
					String ^string0 = recvElement0->InnerText;
					String ^string1 = recvElement1->InnerText;
					int int0 = Convert::ToInt32(string0);
					int int1 = Convert::ToInt32(string1);
					
					XmlNode ^recvElement2 = (XmlElement^)recvDoc->SelectSingleNode("CSocketMsgInfo/m_StrParaList");
					if(recvElement2 == nullptr)
					{
						throw("recvElement2 is null");
					}
					array <String^>^ myArray=gcnew array <String^>(4){"","","",""};
					for(int j = 0;j<recvElement2->ChildNodes->Count;j++)
					{
						myArray[j]= recvElement2->ChildNodes[j]->InnerText;
					}
					m_pSocketMsgInfo = gcnew CSocketMsgInfo((RpcServiceCommand)int0,int1,myArray); 
						
					m_MemoryStreamMutex->ReleaseMutex();
					bMutexFlag = false;
                      
					try
					{
					    m_MainProcess->processMsg(pSocket,m_pSocketMsgInfo);
					}
					catch(Exception^ e)
					{
#ifdef TRACE
				        System::Diagnostics::Trace::WriteLine(String::Format("SCA:OnAcceptSocketCallback:{0}",e->Message));
#endif
						CEventLog::OSNWriteEventLog(e->ToString(),EventLogEntryType::Error,002);
					}
				}
				else
				{
					m_MemoryStreamMutex->ReleaseMutex();
					bMutexFlag = false;
					return;
				}
			}
			catch(Exception^ ex)
			{
#ifdef TRACE
				System::Diagnostics::Trace::WriteLine(String::Format("SCA:OnAcceptSocketCallback:{0}",ex->Message));
#endif
				m_pConnectEvent->Set();
				CEventLog::OSNWriteEventLog(ex->ToString(),EventLogEntryType::Error,002);
			}
			__finally
			{
				if(bMutexFlag)
				{
					try
					{
						m_MemoryStreamMutex->ReleaseMutex();
					}
					catch(ApplicationException^)
					{
						m_MemoryStreamMutex->Close();
						m_MemoryStreamMutex = gcnew Mutex(false);
					}
				}
			}
		}

	   CHostMirrorCDPScheduleThread^ GetCDPScheduleFromList(String^ strGuid)
		{
			 //CHostMirrorCDPScheduleThread^ tempCDPSchedule = nullptr;
			 for each (CHostMirrorCDPScheduleThread^ tempCDPSchedule in m_CDPScheduleList )
			 {
				// tempCDPSchedule = static_cast<CHostMirrorCDPScheduleThread^>(m_CDPScheduleList->Item[i]);
				 if(tempCDPSchedule && tempCDPSchedule->m_Guid->Equals(strGuid))
				 {
					 return tempCDPSchedule;
				 }
			 }
			 return nullptr;
		}
		void OsnNewCDPScheduleThread(String^ strSrcDiskSign,String^ strXmlFileName)
		{
			CHostMirrorCDPScheduleThread^ pCDPScheduleThread = gcnew CHostMirrorCDPScheduleThread(m_CDPScheduleXmlMutex,m_RegMutex,strSrcDiskSign,
				                                                                  strXmlFileName);
			Thread^ cdpScheduleThread = gcnew Thread(gcnew ThreadStart(pCDPScheduleThread,&CHostMirrorCDPScheduleThread::CDPSchedule));

			m_CDPScheduleList->Add(pCDPScheduleThread);
			cdpScheduleThread->Start();
		}
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
			this->components = gcnew System::ComponentModel::Container();
			this->CanStop = true;
			this->CanPauseAndContinue = true;
			this->AutoLog = true;
			this->ServiceName = L"HostmirrorScheduleServiceWinService";
		}
#pragma endregion
	private:
		unsigned long    m_dwPort;
		Thread^          m_ListenThread;
		TcpListener^     m_TcpListener;
		Socket^          m_Socket;
		String^          m_OracleVolume;
		Mutex^           m_CDPScheduleXmlMutex;
		Mutex^           m_MemoryStreamMutex;
		Mutex^           m_RegMutex;
		List<CHostMirrorCDPScheduleThread^>^ m_CDPScheduleList;
		array<Byte>^              m_MsgBuffer ;
        CSocketMsgInfo^   m_pSocketMsgInfo;
		XmlSerializer^    m_pDesSerializer;
		MemoryStream^     m_MemStream;
		MainProcess^      m_MainProcess; 
		static ManualResetEvent^ m_pConnectEvent = gcnew ManualResetEvent(false);
		static ManualResetEvent^ m_ScheduleDeleteEvent = gcnew ManualResetEvent(false);

	public:
			delegate void OsnListenerDelegate();
	};
}
