#include "CDPScheduleThread.h"
#include "eventlog.h"
namespace HostmirrorScheduleService
{
	/*typedef struct _DISK_INFO
{
	unsigned long m_DiskSignature;
	unsigned long m_DiskSize;
	unsigned long m_DiskIndex;
}DISK_INFO;*/
	using namespace System::Net;
	using namespace System::Runtime::InteropServices;
	using namespace System::Runtime ::CompilerServices;
	using namespace System::Management;
	using namespace System::Threading;
	using namespace System::Diagnostics;
	using namespace System::IO;
	using namespace OSNRpcCommonCfg;
    public ref class MainProcess
   {
   public:
	   Mutex^      m_CDPScheduleXmlMutex;
	   Mutex^      m_Mutex;
	   Mutex^      m_RegMutex;
	   List<CHostMirrorCDPScheduleThread^>^ m_CDPScheduleList;
	  static ManualResetEvent^ m_ScheduleDeleteEvent = gcnew ManualResetEvent(false);
    
	  public:
		MainProcess(List<CHostMirrorCDPScheduleThread^>^ pCDPScheduleList)

		{
			m_CDPScheduleList = pCDPScheduleList;
			m_CDPScheduleXmlMutex=gcnew Mutex(false);
			m_Mutex=gcnew Mutex(false);
			m_RegMutex=gcnew Mutex(false);;
		}
	 
		void processMsg(Socket^ socket, CSocketMsgInfo^ pSocketMsgInfo)
		{
			//CEventLog::OSNWriteEventLog("接收到信息，开始分析",EventLogEntryType::Information,023);
			switch(pSocketMsgInfo->m_SendCommand)
			{
				/*case RpcServiceCommand::OSN_RC_CMD_CREATE_CDP:
					OSNHostMirrorCreateCDP(socket,pSocketMsgInfo);
					break;*/
				case RpcServiceCommand::OSN_RC_CMD_CREATE_CDP_SCHEDULE:
					OSNHostMirrorCreateCDPShedule(socket,pSocketMsgInfo);
					break;
				case RpcServiceCommand::OSN_RC_CMD_DELETE_CDP_SCHEDULE:
					OSNHostMirrorDeleteCDPShedule(socket,pSocketMsgInfo);
					break;
				case RpcServiceCommand::OSN_RC_CMD_GET_CDP_SCHEDULE:
					 OsnHostMirrorGetCDPSchedule(socket,pSocketMsgInfo);
					 break;
				default:break;
			}
		}


		bool OSNHostMirrorCreateCDPShedule(Socket^ socket, CSocketMsgInfo^ pSocketMsgInfo)
		{
			//CEventLog::OSNWriteEventLog("接收到信息，开始分析=OSNHostMirrorCreateCDPShedule",EventLogEntryType::Information,023);
			CRetMsgInfo^ pRespondMsgInfo;
			try
			{
				String^ strGuid = pSocketMsgInfo->m_StrParaList[0];
				String^ strCDPStartTime = pSocketMsgInfo->m_StrParaList[1];
				String^ strCDPStopTime = pSocketMsgInfo->m_StrParaList[2];
				String^ strCDPScheduleMode = pSocketMsgInfo->m_StrParaList[3];

				
				//String* diskname = CheckOsnVolume(strVolumeID);
				
				if(!CheckVolAndDisk(strGuid))
				{
					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_NOT_FIND_DISK,(UInt32)RepServiceStatus::RS_ERROR_NOT_FIND_DISK,nullptr);				
					 OsnRespondSocket(socket,pRespondMsgInfo);
					 return false;
				}

				pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,nullptr);				
				OsnRespondSocket(socket,pRespondMsgInfo);
				int iStartIndex = strCDPStartTime->IndexOf("Year");
				int iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Year:"))->IndexOf(";");
				String^ strStartYear = strCDPStartTime->Substring(iStartIndex + strlen("Year:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("Month");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Month:"))->IndexOf(";");
				String^ strStartMonth = strCDPStartTime->Substring(iStartIndex + strlen("Month:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("Day");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Day:"))->IndexOf(";");
				String^ strStartDay = strCDPStartTime->Substring(iStartIndex + strlen("Day:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("WeekNum");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("WeekNum:"))->IndexOf(";");
				String^ strStartWeekNum = strCDPStartTime->Substring(iStartIndex + strlen("WeekNum:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("DayOfWeek");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("DayOfWeek:"))->IndexOf(";");
				String^ strStartDayOfWeek = strCDPStartTime->Substring(iStartIndex + strlen("DayOfWeek:"))->Substring(0,iStopIndex);
							
				iStartIndex = strCDPStartTime->IndexOf("Hour");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Hour:"))->IndexOf(";");
				String^ strStartHour = strCDPStartTime->Substring(iStartIndex + strlen("Hour:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("Minute");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Minute:"))->IndexOf(";");
				String^ strStartMinute = strCDPStartTime->Substring(iStartIndex + strlen("Minute:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("Second");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("Second:"))->IndexOf(";");
				String^ strStartSecond = strCDPStartTime->Substring(iStartIndex + strlen("Second:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("IntervalDays");
				iStopIndex = strCDPStartTime->Substring(iStartIndex + strlen("IntervalDays:"))->IndexOf(";");
				String^ strIntervalDays = strCDPStartTime->Substring(iStartIndex + strlen("IntervalDays:"))->Substring(0,iStopIndex);

				iStartIndex = strCDPStartTime->IndexOf("IntervalMinutes");
				String^ strIntervalMinutes = strCDPStartTime->Substring(iStartIndex + strlen("IntervalMinutes:"));

				CCDPStartTime^ pCDPStartTime = gcnew CCDPStartTime(strStartYear,strStartMonth,strStartDay,
																 strStartWeekNum,strStartDayOfWeek,strStartHour,
																 strStartMinute,strStartSecond,strIntervalDays,
																 strIntervalMinutes);

				CDP_SCHEDULE_MODE cdpScheduleMode = (CDP_SCHEDULE_MODE)(Convert::ToInt16(strCDPScheduleMode));
				
				CHostMirrorCDPSchedule^ pCDPSchedule = gcnew CHostMirrorCDPSchedule(strGuid,
																	pCDPStartTime,
																	nullptr,
																	Convert::ToInt16(cdpScheduleMode));		
								                                       

				String^ xmlFileName = String::Concat(AppDomain::CurrentDomain->BaseDirectory,"\\HostMirrorCDPSchedule.xml");
				if(File::Exists(xmlFileName) == true)
				{
					m_CDPScheduleXmlMutex->WaitOne();
								
					XmlDocument^ xmlDoc = gcnew XmlDocument();
					xmlDoc->Load(xmlFileName);

					XmlNode^ xRoot = xmlDoc->DocumentElement;
								
					String^ strExp = String::Concat("//CHostMirrorCDPSchedule[m_Guid='",strGuid,"']");	
					XmlNode^ xNode = xmlDoc->SelectSingleNode(strExp);
					if(xNode)  ////the schedule has exited, delete it first
					{			
						xRoot->RemoveChild(xNode);
						XmlElement^ xCDPSchedule = xmlDoc->CreateElement("CHostMirrorCDPSchedule");
						xRoot->AppendChild(xCDPSchedule);

						XmlNode^ xSrcVolumeID = xmlDoc->CreateElement("m_Guid"); 
						xSrcVolumeID->InnerText = strGuid;
						xCDPSchedule->AppendChild(xSrcVolumeID);

						XmlNode^ xStartTime = xmlDoc->CreateElement("m_StartTime"); 
						xCDPSchedule->AppendChild(xStartTime);
								
						XmlNode^ xStartYear = xmlDoc->CreateElement("m_Year");
						xStartYear->InnerText = strStartYear;
						xStartTime->AppendChild(xStartYear);

						XmlNode^ xStartMonth = xmlDoc->CreateElement("m_Month");
						xStartMonth->InnerText = strStartMonth;
						xStartTime->AppendChild(xStartMonth);

						XmlNode^ xStartDay = xmlDoc->CreateElement("m_Day");
						xStartDay->InnerText = strStartDay;
						xStartTime->AppendChild(xStartDay);

						XmlNode^ xStartWeekNum = xmlDoc->CreateElement("m_WeekNum");
						xStartWeekNum->InnerText = strStartWeekNum;
						xStartTime->AppendChild(xStartWeekNum);

						XmlNode^ xStartDayOfWeek = xmlDoc->CreateElement("m_DayOfWeek");
						xStartDayOfWeek->InnerText = strStartDayOfWeek;
						xStartTime->AppendChild(xStartDayOfWeek);

						XmlNode^ xStartHour = xmlDoc->CreateElement("m_Hour");
						xStartHour->InnerText = strStartHour;
						xStartTime->AppendChild(xStartHour);

						XmlNode^ xStartMinute = xmlDoc->CreateElement("m_Minute");
						xStartMinute->InnerText = strStartMinute;
						xStartTime->AppendChild(xStartMinute);

						XmlNode^ xStartSecond = xmlDoc->CreateElement("m_Second");
						xStartSecond->InnerText = strStartSecond;
						xStartTime->AppendChild(xStartSecond);

						XmlNode^ xIntervalDays = xmlDoc->CreateElement("m_IntervalDays");
						xIntervalDays->InnerText = strIntervalDays;
						xStartTime->AppendChild(xIntervalDays);

						XmlNode^ xIntervalMinutes = xmlDoc->CreateElement("m_IntervalMinutes");
						xIntervalMinutes->InnerText = strIntervalMinutes;
						xStartTime->AppendChild(xIntervalMinutes);


						XmlNode^ xScheduleMode = xmlDoc->CreateElement("m_Mode"); 
						xScheduleMode->InnerText = strCDPScheduleMode;
						xCDPSchedule->AppendChild(xScheduleMode);

						xmlDoc->DocumentElement->InsertAfter(xCDPSchedule,xmlDoc->DocumentElement->LastChild);
						xmlDoc->Save(xmlFileName);

						m_CDPScheduleXmlMutex->ReleaseMutex();
						CHostMirrorCDPScheduleThread^ pCdpScheduleThread = GetCDPScheduleFromList(strGuid);
						if(pCdpScheduleThread)
						{
							pCdpScheduleThread->m_ScheduleDeleteEvent->Set();
						}
						else
						{
							OsnNewCDPScheduleThread(strGuid,xmlFileName);
						}
						return true;
					}
								////Add new schedule
					XmlElement^ xCDPSchedule = xmlDoc->CreateElement("CHostMirrorCDPSchedule");
					xRoot->AppendChild(xCDPSchedule);

					XmlNode^ xSrcVolumeID = xmlDoc->CreateElement("m_Guid"); 
					xSrcVolumeID->InnerText = strGuid;
					xCDPSchedule->AppendChild(xSrcVolumeID);

					XmlNode^ xStartTime = xmlDoc->CreateElement("m_StartTime"); 
					xCDPSchedule->AppendChild(xStartTime);
								
					XmlNode^ xStartYear = xmlDoc->CreateElement("m_Year");
					xStartYear->InnerText = strStartYear;
					xStartTime->AppendChild(xStartYear);

					XmlNode^ xStartMonth = xmlDoc->CreateElement("m_Month");
					xStartMonth->InnerText = strStartMonth;
					xStartTime->AppendChild(xStartMonth);

					XmlNode^ xStartDay = xmlDoc->CreateElement("m_Day");
					xStartDay->InnerText = strStartDay;
					xStartTime->AppendChild(xStartDay);

					XmlNode^ xStartWeekNum = xmlDoc->CreateElement("m_WeekNum");
					xStartWeekNum->InnerText = strStartWeekNum;
					xStartTime->AppendChild(xStartWeekNum);

					XmlNode^ xStartDayOfWeek = xmlDoc->CreateElement("m_DayOfWeek");
					xStartDayOfWeek->InnerText = strStartDayOfWeek;
					xStartTime->AppendChild(xStartDayOfWeek);

					XmlNode^ xStartHour = xmlDoc->CreateElement("m_Hour");
					xStartHour->InnerText = strStartHour;
					xStartTime->AppendChild(xStartHour);

					XmlNode^ xStartMinute = xmlDoc->CreateElement("m_Minute");
					xStartMinute->InnerText = strStartMinute;
					xStartTime->AppendChild(xStartMinute);

					XmlNode^ xStartSecond = xmlDoc->CreateElement("m_Second");
					xStartSecond->InnerText = strStartSecond;
					xStartTime->AppendChild(xStartSecond);

					XmlNode^ xIntervalDays = xmlDoc->CreateElement("m_IntervalDays");
					xIntervalDays->InnerText = strIntervalDays;
					xStartTime->AppendChild(xIntervalDays);

					XmlNode^ xIntervalMinutes = xmlDoc->CreateElement("m_IntervalMinutes");
					xIntervalMinutes->InnerText = strIntervalMinutes;
					xStartTime->AppendChild(xIntervalMinutes);


					XmlNode^ xScheduleMode = xmlDoc->CreateElement("m_Mode"); 
					xScheduleMode->InnerText = strCDPScheduleMode;
					xCDPSchedule->AppendChild(xScheduleMode);

					xmlDoc->DocumentElement->InsertAfter(xCDPSchedule,xmlDoc->DocumentElement->LastChild);
					xmlDoc->Save(xmlFileName);

					m_CDPScheduleXmlMutex->ReleaseMutex();
					CHostMirrorCDPScheduleThread^ pCdpScheduleThread = GetCDPScheduleFromList(strGuid);
					if(pCdpScheduleThread)
					{
						pCdpScheduleThread->m_ScheduleDeleteEvent->Set();
					}
					else
					{
						OsnNewCDPScheduleThread(strGuid,xmlFileName);
					}
		                       
					return true;
				}
				else
				{
					XmlRootAttribute^ xRoot = gcnew XmlRootAttribute();
					xRoot->ElementName = "OSNHostMirrorScheduleService";
					List<CHostMirrorCDPSchedule^>^ Schedulelist=gcnew List<CHostMirrorCDPSchedule^>();
					Schedulelist->Add(pCDPSchedule);
					XmlSerializer^ xSerial = gcnew XmlSerializer(Schedulelist->GetType(),xRoot);
					TextWriter^ xWriter = gcnew StreamWriter(xmlFileName);
					xSerial->Serialize(xWriter,Schedulelist);
								
					xWriter->Flush();
					xWriter->Close();

					CHostMirrorCDPScheduleThread^ pCdpScheduleThread = GetCDPScheduleFromList(strGuid);
					if(pCdpScheduleThread)
					{
						pCdpScheduleThread->m_ScheduleDeleteEvent->Set();
					}
					else
					{
						OsnNewCDPScheduleThread(strGuid,xmlFileName);
					}
				}
							
			}
			catch(Exception^ ex)
			{
				pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION,(UInt32)(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION),ex->ToString());
				OsnRespondSocket(socket,pRespondMsgInfo);

				CEventLog::OSNWriteEventLog(ex->Message,EventLogEntryType::Error,021);
				return false;
			}
			return true;
        }
	    
	
	    bool OSNHostMirrorDeleteCDPShedule(Socket^ socket, CSocketMsgInfo^ pSocketMsgInfo)
		{
			//CEventLog::OSNWriteEventLog("接收到信息，开始分析=OSNHostMirrorDeleteCDPShedule",EventLogEntryType::Information,023);
			CRetMsgInfo^ pRespondMsgInfo;
			try
			{
				String^ VolumeID = pSocketMsgInfo->m_StrParaList[0];
				
				if(!CheckVolAndDisk(VolumeID))
				{
					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_NOT_FIND_DISK,(UInt32)RepServiceStatus::RS_ERROR_NOT_FIND_DISK,nullptr);				
					OsnRespondSocket(socket,pRespondMsgInfo);
					return false;
				}
				

							
				String^ xmlFileName = String::Concat(AppDomain::CurrentDomain->BaseDirectory,"\\HostMirrorCDPSchedule.xml");
				if(File::Exists(xmlFileName) == true)
				{
					m_CDPScheduleXmlMutex->WaitOne();
								
					XmlDocument^ xmlDoc = gcnew XmlDocument();
					xmlDoc->Load(xmlFileName);

					XmlNode^ xRoot = xmlDoc->DocumentElement;
									
					String^ strExp = String::Concat("//CHostMirrorCDPSchedule[m_Guid='",VolumeID,"']");	
					XmlNode^ xNode = xmlDoc->SelectSingleNode(strExp);
					if(xNode)  ////the schedule has exited, delete it first
					{			
						xRoot->RemoveChild(xNode);
					}
					else
					{
						String^ msg="未设置CDP任务计划";
					    pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,msg);				
					    OsnRespondSocket(socket,pRespondMsgInfo);
							
					    CEventLog::OSNWriteEventLog(msg,EventLogEntryType::Error,017);
					    return true;
					}
					xmlDoc->Save(xmlFileName);

					m_CDPScheduleXmlMutex->ReleaseMutex();
					CHostMirrorCDPScheduleThread^ pCdpScheduleThread = GetCDPScheduleFromList(VolumeID);
					if(pCdpScheduleThread)
					{
						pCdpScheduleThread->m_ScheduleDeleteEvent->Set();
					}
					DeleteCDPScheduleinList(VolumeID);
				}
				else
				{
					String^ msg="未设置CDP任务计划";
					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,msg);				
					OsnRespondSocket(socket,pRespondMsgInfo);
							
					CEventLog::OSNWriteEventLog(msg,EventLogEntryType::Error,017);
					return true;
				}
							
				pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)(RepServiceStatus::RS_SUCCESS),nullptr);
				OsnRespondSocket(socket,pRespondMsgInfo);
			}
			catch(Exception^ ex)
			{
				pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION,(UInt32)(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION),ex->ToString());
				OsnRespondSocket(socket,pRespondMsgInfo);

				CEventLog::OSNWriteEventLog(ex->Message,EventLogEntryType::Error,021);
				return false;
			}
			return true;
		 }
		bool OsnHostMirrorGetCDPSchedule(Socket^ socket, CSocketMsgInfo^ pSocketMsgInfo)
		{
			//CEventLog::OSNWriteEventLog("接收到信息，开始分析=OsnHostMirrorGetCDPSchedule",EventLogEntryType::Information,023);
			 CRetMsgInfo^ pRespondMsgInfo;
			 try
			 {
				String^ strSignature = pSocketMsgInfo->m_StrParaList[0];

				
				if(!CheckVolAndDisk(strSignature))
				{
					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_NOT_FIND_DISK,(UInt32)RepServiceStatus::RS_ERROR_NOT_FIND_DISK,nullptr);				
					OsnRespondSocket(socket,pRespondMsgInfo);
					return false;
				}


				String^ xmlFileName = String::Concat(AppDomain::CurrentDomain->BaseDirectory,"\\HostMirrorCDPSchedule.xml");
				m_CDPScheduleXmlMutex->WaitOne();
								////read CDP Schedule info from CDPSchedule.xml
				XmlDocument^ xmlDoc = gcnew XmlDocument();
				if(File::Exists(xmlFileName)==true)
				{
					xmlDoc->Load(xmlFileName);
					String^ strExp = String::Concat("//CHostMirrorCDPSchedule[m_Guid='",strSignature,"']");
					XmlNode^ xNode = xmlDoc->SelectSingleNode(strExp);
					if(!xNode)
					{
						m_CDPScheduleXmlMutex->ReleaseMutex();
						String^ msg="未设置CDP任务计划";
						pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,msg);				
						OsnRespondSocket(socket,pRespondMsgInfo);
											
						CEventLog::OSNWriteEventLog(msg,EventLogEntryType::Error,017);
													
						return false;
					}

					String^ strStartYear      = xNode->ChildNodes->Item(1)->ChildNodes->Item(0)->InnerText;
					String^ strStartMonth     = xNode->ChildNodes->Item(1)->ChildNodes->Item(1)->InnerText;
					String^ strStartDay       = xNode->ChildNodes->Item(1)->ChildNodes->Item(2)->InnerText;
					String^ strStartWeekNum   = xNode->ChildNodes->Item(1)->ChildNodes->Item(3)->InnerText;
					String^ strStartDayOfWeek = xNode->ChildNodes->Item(1)->ChildNodes->Item(4)->InnerText;
					String^ strStartHour      = xNode->ChildNodes->Item(1)->ChildNodes->Item(5)->InnerText;
					String^ strStartMinute    = xNode->ChildNodes->Item(1)->ChildNodes->Item(6)->InnerText;
					String^ strStartSecond    = xNode->ChildNodes->Item(1)->ChildNodes->Item(7)->InnerText;
					String^ strStartIntervalDays = xNode->ChildNodes->Item(1)->ChildNodes->Item(8)->InnerText;
					String^ strStartIntervalMinutes = xNode->ChildNodes->Item(1)->ChildNodes->Item(9)->InnerText;
					if(strStartMinute->Length == 1)
					{
						strStartMinute = String::Concat("0",strStartMinute);
					}
					String^ strScheduleMode = xNode->ChildNodes->Item(2)->InnerText;
					m_CDPScheduleXmlMutex->ReleaseMutex();
									
					String^ strCDPScheduleMsg = "";

					CDP_SCHEDULE_MODE scheduleMode = (CDP_SCHEDULE_MODE)(Convert::ToInt16(strScheduleMode));
					switch(scheduleMode)
					{
						case CDP_SCHEDULE_MODE::MODE_MONTH:
							{
								if(strStartMonth->Equals("ALL"))   ////每月
								{ 
									 if(strStartDay->Equals("ALL"))  ////第几个星期的第几天
									 {
										int iDayOfWeek = Convert::ToInt16(strStartDayOfWeek);
										String^ strDayOfWeek = GetStringDayOfWeek(iDayOfWeek);

										strCDPScheduleMsg = String::Concat("在每月的第", strStartWeekNum, "个星期", strDayOfWeek);
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, ",",strStartHour, ":");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartMinute);
									 }
									 else    ////第几天
									 {
										 strCDPScheduleMsg = String::Concat("在每月的第", strStartDay, "天", ",");
										 strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartHour, ":");
										 strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartMinute);
									 }
								}
								else    ////特定几个月
								{
									 if(strStartDay->Equals("ALL"))  ////第几个星期的第几天
									{
										int iDayOfWeek = Convert::ToInt16(strStartDayOfWeek);
										String^ strDayOfWeek = GetStringDayOfWeek(iDayOfWeek);

										strCDPScheduleMsg = String::Concat("在",strStartMonth, "月的第");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartWeekNum, "个星期", strDayOfWeek);
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, ",",strStartHour, ":");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartMinute);
									}
									else    ////第几天
									{
										strCDPScheduleMsg = String::Concat("在",strStartMonth, "月的第");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartDay, "天", ",");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartHour, ":");
										strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartMinute);
									}
								}
								break;
							}
						case CDP_SCHEDULE_MODE::MODE_WEEK:
							{
								if(strStartDayOfWeek->Equals("ALL"))
								{
									strCDPScheduleMsg = String::Concat("在每天的 ", strStartHour, ":", strStartMinute);
								}
								else
								{
									String^ strDayArrayOfWeek = GetStringDayArrayOfWeek(strStartDayOfWeek);
									strCDPScheduleMsg = String::Concat("在每周的", strDayArrayOfWeek, " ", strStartHour);
									strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, ":", strStartMinute);
								}
								break;
							}
						case CDP_SCHEDULE_MODE::MODE_DAY:
							{
								strCDPScheduleMsg = String::Concat("从", strStartYear, "-", strStartMonth);
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, "-", strStartDay, " ");
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartHour, ":", strStartMinute);
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, "开始，每", strStartIntervalDays, "天");
								break;
							}
						case CDP_SCHEDULE_MODE::MODE_MINUTE:
							{
								strCDPScheduleMsg = String::Concat("从", strStartYear, "-", strStartMonth);
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, "-", strStartDay, " ");
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, strStartHour, ":", strStartMinute);
								strCDPScheduleMsg = String::Concat(strCDPScheduleMsg, "开始，每", strStartIntervalMinutes, "分钟");
								break;
							}
						default:
								break;
					}				

					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,strCDPScheduleMsg);				
					OsnRespondSocket(socket,pRespondMsgInfo);
				}
				else
				{
					String^ msg="未设置CDP任务计划";
					pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_SUCCESS,(UInt32)RepServiceStatus::RS_SUCCESS,msg);				
					OsnRespondSocket(socket,pRespondMsgInfo);
							
					CEventLog::OSNWriteEventLog(msg,EventLogEntryType::Error,017);
				}

			}
			catch(Exception^ ex)
			{
				pRespondMsgInfo = gcnew CRetMsgInfo(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION,(UInt32)(RepServiceStatus::RS_ERROR_API_CATCH_EXCEPTION),ex->ToString());
				OsnRespondSocket(socket,pRespondMsgInfo);

				CEventLog::OSNWriteEventLog(ex->Message,EventLogEntryType::Error,021);
				m_CDPScheduleXmlMutex->ReleaseMutex();
				return false;
			}
			return true;
		}
		CHostMirrorCDPScheduleThread^ GetCDPScheduleFromList(String^ strSignature)
		{
			 for each(CHostMirrorCDPScheduleThread^ tempCDPSchedule in m_CDPScheduleList)
			 {
				 // CHostMirrorCDPScheduleThread^ tempCDPSchedule = safe_cast<CHostMirrorCDPScheduleThread^>(m_CDPScheduleList->geItem[i]);
				 if(tempCDPSchedule && tempCDPSchedule->m_Guid->Equals(strSignature))
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
		bool CheckVolAndDisk(String^ signature)
		{
			if(signature->EndsWith("Volume"))//label#signature_offset#Volume
			{
				array<String^>^ info=signature->Split('#');
				//String^ tempsignature=GetSignatureByLabel(info[0]);
				String^ label=GetVolumeLabelBySignature(info[0]);
				if(nullptr!=label)
				{
                   return true;
				}
			}
			else if(signature->EndsWith("Disk"))//index#signature_offset#Disk
			{
				array<String^>^ info=signature->Split('#');
				//int index=Convert::ToInt32(info[0]);
				int index=GetDiskIndexBySignature(info[0]);
				//String^ tempsignature=GetSignatureByIndex(index);
				if(-1!=index)
				{
                   return true;
				}

			}
			return false;

		}
		String^ GetSignatureByLabel(String^ label)
		{
			Char Name=label[0];
			unsigned int	signature = 0;	
			unsigned int	startingOffset = 0;
			int ErrorCode = OSNGetVolumeID(Name, signature, startingOffset);
			if(ErrorCode==0)
			{
				//MessageBox::Show(String::Concat("签名：",signature.ToString("X8"),"偏移量：",startingOffset.ToString("X8")));
				//ErrorCode = OsnVolumeCopyGetVolumeSize(signature, startingOffset, &size);
				//if(ErrorCode ==0)
				//{
					char char_guid[128];
					bool ret=OsnCHeckIsGPTVolume((const char)Name,char_guid);
					String^ guid=nullptr;
					if(ret)
					{
					   guid=gcnew String(char_guid);
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
					}
					return guid;
			}
			return nullptr;
		}
		String^ GetVolumeLabelBySignature(String^ guid)
		{
			Object	^object;
			try
			{
				ManagementObjectSearcher ^query = gcnew ManagementObjectSearcher(
											"SELECT * FROM Win32_LogicalDisk WHERE DriveType=3");
				ManagementObjectCollection ^queryCollection = query->Get();
				System::Collections::IEnumerator	^loopMC = queryCollection->GetEnumerator();
				while(loopMC->MoveNext())
				{
					ManagementObject ^mo = static_cast<ManagementObject ^> (loopMC->Current);
					object		= mo->GetPropertyValue("Name");
					
					String ^VolumeLabel	= static_cast<String ^> (object);	
					Char Name=VolumeLabel[0];
					unsigned int	signature = 0;	
					unsigned int	startingOffset = 0;
					int ErrorCode = OSNGetVolumeID(Name, signature, startingOffset);
					if(ErrorCode==0)
					{
							char char_guid[128];
							bool ret=OsnCHeckIsGPTVolume((const char)Name,char_guid);
							String^ strguid=nullptr;
							if(ret)
							{
							   strguid=gcnew String(char_guid);
							  //return guid;
							}
							else
							{
								VOLUMEID volume;
								volume.MBP_VolumeID.m_BlockOffset=startingOffset;
								volume.MBP_VolumeID.m_DiskSignature=signature;
								volume.MBP_VolumeID.m_NotUsed1=0;
								OsnGUIDToString(char_guid,volume.SAN_VolumeID.m_VolumeGuid);
								strguid=gcnew String(char_guid);								
							}
							if(strguid->Equals(guid))
							{
								return VolumeLabel;
							}
					}
				}
			}
			catch(...)
			{
			}
			return nullptr;
		}
		String^ GetSignatureByIndex(int index)
		{
			 DISK_INFO pDisk;
			 memset(&pDisk,0,sizeof(DISK_INFO));
			 pDisk.m_DiskIndex=index;
			 int ErrorCode=OsnGetDisk(&pDisk);
			 if(ErrorCode==0)
			 {     
				 String^ strguid=nullptr;
				 char guid[128];
				 OsnGUIDToString(guid,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
				 strguid=gcnew String(guid);
				 return strguid;
			 }
			 return nullptr;
		}
		int GetDiskIndexBySignature(String^ guid)
		{
			 DISK_INFO pDisk;
			 for(int i=0;i<64;i++)
			 {
				 memset(&pDisk,0,sizeof(DISK_INFO));
				 pDisk.m_DiskIndex=i;
				 int ErrorCode=OsnGetDisk(&pDisk);
				 if(ErrorCode==0)
				 {     
					 String^ strguid=nullptr;
					 char chguid[128];
					 OsnGUIDToString(chguid,pDisk.m_DiskID.SAN_VolumeID.m_VolumeGuid);
					 strguid=gcnew String(chguid);
					 if(strguid->Equals(guid))
					 {
						 return i;
					 }
				 }
			 }
			 return -1;
		}
		bool DeleteCDPScheduleinList(String^ strSignature)
		 {

			CHostMirrorCDPScheduleThread^ tempCDPSchedule = nullptr;
			for each(CHostMirrorCDPScheduleThread^ tempCDPSchedule in m_CDPScheduleList)
			{
				//tempCDPSchedule = static_cast<CHostMirrorCDPScheduleThread^>(m_CDPScheduleList->Item[i]);
				if(tempCDPSchedule && tempCDPSchedule->m_Guid->Equals(strSignature))
				{
					m_CDPScheduleList->Remove(tempCDPSchedule);
					return true;
				}
			}
			return false;
		
		 }
		String^ OsnXmlSerialize(System::Type^ type, System::Object^ obj)
		{
			XmlSerializer^ pXmlSerial = gcnew XmlSerializer(type);
			MemoryStream^ pMem = gcnew MemoryStream();
			XmlTextWriter^ pWriter = gcnew XmlTextWriter(pMem, Encoding::UTF8);
			pXmlSerial->Serialize(pWriter,obj);
			pWriter->Close();

			UTF8Encoding^ utf8 = gcnew UTF8Encoding(); 
			String^ strRet =  utf8->GetString(pMem->ToArray());
			
			return strRet;
		}
		void OsnRespondSocket(Socket^ socket,CRetMsgInfo^ retMsgInfo)
		{
			XmlDocument ^sendDoc = gcnew XmlDocument();

			XmlDeclaration ^dec = sendDoc->CreateXmlDeclaration("1.0", "utf-8", nullptr);
			sendDoc->AppendChild(dec);

			XmlElement ^root = sendDoc->CreateElement("CRetMsgInfo");  
			sendDoc->AppendChild(root);  

			XmlElement ^element1 = sendDoc->CreateElement("m_RepServiceStatus");
			element1->InnerText = ((int)retMsgInfo->m_RepServiceStatus).ToString(); 
			root->AppendChild(element1);  

			XmlElement ^element2 = sendDoc->CreateElement("m_RetMsgInfo");
			element2->InnerText = retMsgInfo->m_RetMsgInfo;  
			root->AppendChild(element2);  

			XmlElement ^element3 = sendDoc->CreateElement("m_ErrorCode");
			element3->InnerText = retMsgInfo->m_ErrorCode.ToString(); 
			root->AppendChild(element3);  

			MemoryStream^ pMem = gcnew MemoryStream();
			sendDoc->Save(pMem);
			
			array<Byte>^ respondBytes = pMem->ToArray();
			socket->Send(respondBytes);
		}
		String^ GetStringDayOfWeek(int iDayOfWeek)
        {
            String^ strDayOfWeek = "";

            switch (iDayOfWeek)
            {
                case 0:
                    strDayOfWeek = "日";
                    break;
                case 1:
                    strDayOfWeek = "一";
                    break;
                case 2:
                    strDayOfWeek = "二";
                    break;
                case 3:
                    strDayOfWeek = "三";
                    break;
                case 4:
                    strDayOfWeek = "四";
                    break;
                case 5:
                    strDayOfWeek = "五";
                    break;
                case 6:
                    strDayOfWeek = "六";
                    break;
                default:
                    break;
            }
            return strDayOfWeek;
        }

	    String^ GetStringDayArrayOfWeek(String^ strDayOfWeek)
			{
				String^ strDayArrayOfWeek=nullptr;

				
				array<Char>^ chSplit = {','};
				array<String^>^ strArray = strDayOfWeek->Split(chSplit);

				for(int i=0; i<strArray->Length; i++)
				{
					switch(Convert::ToInt16(strArray[i]))
					{
					case 0:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期日,";
						break;
					case 1:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期一,";
						break;
					case 2:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期二,";
						break;
					case 3:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期三,";
						break;
					case 4:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期四,";
						break;
					case 5:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期五,";
						break;
					case 6:
						strDayArrayOfWeek = strDayArrayOfWeek+ "星期六,";
						break;
					default:
						break;
					}
				}
				return strDayArrayOfWeek;
			}
   };
}