#pragma once
//#include "CreateCDPApi.h"
#include <stdio.h>
#include <windows.h>
#include "eventlog.h"
#include "..\OsnVolumeCopyApi\Configure.h"
#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"
#include "CreateCDP.h"
namespace HostmirrorScheduleService
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Data::SqlClient;
	using namespace System::Net;
	using namespace System::Net::Sockets;
    using namespace System::Runtime::InteropServices;
	using namespace System::Timers;
	using namespace System::Management;
	using namespace System::Collections::Generic;
	using namespace System::Xml;
	using namespace System::Xml::Serialization;
	using namespace System::Text;
	using namespace System::IO;
	using namespace Microsoft::Win32;
	using namespace System::Threading;
	using namespace OSNRpcCommonCfg;

	public ref class CHostMirrorCDPScheduleThread
	{
	private: 
		static const int MONTH_MODE_INTERVAL = 9 * 60 * 1000;   ////9分钟,the timespan for MONTH_MODE 
		static const int DEFAULT_MODE_INTERVAL = 5 * 60 * 1000;  ////5分钟
		Mutex^ m_Mutex;
		Mutex^ m_RegMutex;
		String^ m_XmlFileName;                                                                 
		System::Timers::Timer^ m_CdpScheduleTimer;
		bool flag;
		//List<OSNRpcCommonCfg::CDiskInfo*>*  m_DiskList;
		CHostMirrorCDPSchedule  ^m_CDPSchedule;
		bool m_bChangeInterval; 
		XmlDocument^  xmlDoc;
		CEventLog^ myEventLog;
		
    public:   
		ManualResetEvent^ m_ScheduleDeleteEvent;  
		String^ m_Guid;

	public: 
		CHostMirrorCDPScheduleThread()
		{
			
		}

		CHostMirrorCDPScheduleThread(Mutex^ mutex,
							  Mutex^ Regmutx,
							  String^ guid,
							  String^ strXmlFileName)
		{
			m_Mutex = mutex;
			m_RegMutex=Regmutx;
			m_Guid = guid;
			m_XmlFileName = strXmlFileName;
			m_ScheduleDeleteEvent = gcnew ManualResetEvent(false);
			xmlDoc = gcnew XmlDocument();
			myEventLog=gcnew CEventLog();
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

	
		int  OsnWeekOfMonth(DateTime dateTime, int WeekStart)
		{
			//WeekStart
			//1表示 周一至周日 为一周
			//2表示 周日至周六 为一周
			String^ strFirstOfMonth = String::Concat(dateTime.Date.Year.ToString(), "-", dateTime.Date.Month.ToString(), "-1");
			DateTime FirstOfMonth = Convert::ToDateTime(strFirstOfMonth);
			int i = (int) FirstOfMonth.Date.DayOfWeek;
			if(i == 0)
			{
				i = 7;
			}
			if(WeekStart == 1)
			{
				return (dateTime.Date.Day + i - 2) / 7 + 1;
			}
			if(WeekStart == 2)
			{
				return (dateTime.Date.Day + i - 1) / 7;
			}
			return 0;
		}
		
		String^ GetCurrentOSVersion()
		{
			 String^ strOSVersion = "";
			 try
			 { 
				 ManagementObjectSearcher^ OSVersionSearcher = gcnew 
										   ManagementObjectSearcher("Select * from Win32_OperatingSystem");
				 ManagementObjectCollection::ManagementObjectEnumerator^ myOSData = OSVersionSearcher->Get()->GetEnumerator();
				 myOSData->MoveNext();
				 ManagementBaseObject^ mboOS = myOSData->Current;
				 strOSVersion = mboOS->GetPropertyValue("Caption")->ToString(); 
			 }
			 catch(Exception^ ex)
			 {
				 CEventLog::OSNWriteEventLog(ex->Message,EventLogEntryType::Error,021);
			 }
			 return strOSVersion;
		}
		
		void OnCDPScheduleMonthTimerElapsed(System::Object^ sender,ElapsedEventArgs^ e)
		{
				
			if(m_CDPSchedule == nullptr)
			{
				return;
			}
			
			if(!CheckVolAndDisk(m_Guid))
			{
				return;
			}

			DateTime timeNow = DateTime::Now;    

			if(m_CDPSchedule->m_StartTime->m_Month->Equals("ALL")||  
			   IsStringContained(m_CDPSchedule->m_StartTime->m_Month, timeNow.Month.ToString()))  ////should do cdp in this month  
			{
				if(m_CDPSchedule->m_StartTime->m_Day->Equals("ALL"))  ////give a day by m_WeekNum and m_DayOfWeek
				{
					int iNowWeekOfMonth   = OsnWeekOfMonth(timeNow,2);
					int iNowDayOfWeek     = Convert::ToInt32(timeNow.DayOfWeek);
					int iStartWeekOfMonth = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_WeekNum);
					int iStartDayOfWeek   = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_DayOfWeek);

					if((iNowWeekOfMonth == iStartWeekOfMonth) && (iNowDayOfWeek == iStartDayOfWeek))
					{
						int iStartHour   = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Hour);
						int iStartMinute = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Minute);

						if(timeNow.Hour == iStartHour + 1)
						{
							if(60 - iStartMinute + timeNow.Minute <= MONTH_MODE_INTERVAL/1000/60/2)
							{
								OsnDoCDPSnapshot();
							}
						}
						else if(timeNow.Hour == iStartHour)
						{
							if(((timeNow.Minute >= iStartMinute) && (timeNow.Minute - iStartMinute <= MONTH_MODE_INTERVAL/1000/60/2))
								||((iStartMinute >= timeNow.Minute) && (iStartMinute - timeNow.Minute <= MONTH_MODE_INTERVAL/1000/60/2)))
							{
								OsnDoCDPSnapshot();
							}
						}
						else if(timeNow.Hour == iStartHour - 1)
						{
							if(60 - timeNow.Minute + iStartMinute <= MONTH_MODE_INTERVAL/1000/60/2)
							{
								OsnDoCDPSnapshot();
							}
						}
					}
				}
				else  ////give a day by m_Day
				{
					if(m_CDPSchedule->m_StartTime->m_Day->Equals(timeNow.Day.ToString()))
					{
						int iStartHour   = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Hour);
						int iStartMinute = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Minute);
					
						if(timeNow.Hour == iStartHour + 1)
						{
							if(60 - iStartMinute + timeNow.Minute <= MONTH_MODE_INTERVAL/1000/60/2)
							{
								OsnDoCDPSnapshot();
							}
						}
						else if(timeNow.Hour == iStartHour)
						{
							if(((timeNow.Minute >= iStartMinute) && (timeNow.Minute - iStartMinute <= MONTH_MODE_INTERVAL/1000/60/2))
								||((iStartMinute >= timeNow.Minute) && (iStartMinute - timeNow.Minute <= MONTH_MODE_INTERVAL/1000/60/2)))
							{
								OsnDoCDPSnapshot();
							}
						}
						else if(timeNow.Hour == iStartHour - 1)
						{
							if(60 - timeNow.Minute + iStartMinute <= MONTH_MODE_INTERVAL/1000/60/2)
							{
								OsnDoCDPSnapshot();
							}
						}
					}
				}
			}
		
		}
		
		void OnCDPScheduleWeekTimerElapsed(System::Object^ sender,ElapsedEventArgs^ e)
		{
				if(m_CDPSchedule == nullptr)
				{
					return;
				}
                
				if(!CheckVolAndDisk(m_Guid))
				{
					return;
				}
				DateTime timeNow = DateTime::Now;  
				String ^strNowDayOfWeek = nullptr;
				switch(timeNow.DayOfWeek)
				{
				case DayOfWeek::Monday:
					strNowDayOfWeek = "1";
					break;

				case DayOfWeek::Tuesday:
					strNowDayOfWeek = "2";
					break;
					
				case DayOfWeek::Wednesday:
					strNowDayOfWeek = "3";
					break;

				case DayOfWeek::Thursday:
					strNowDayOfWeek = "4";
					break;

				case DayOfWeek::Friday:
					strNowDayOfWeek = "5";
					break;

				case DayOfWeek::Saturday:
					strNowDayOfWeek = "6";
					break;

				case DayOfWeek::Sunday:
					strNowDayOfWeek = "0";
					break;

				default:
					break;

				}

				if(m_CDPSchedule->m_StartTime->m_DayOfWeek->Equals("ALL")
				   || IsStringContained(m_CDPSchedule->m_StartTime->m_DayOfWeek,strNowDayOfWeek)) 
				{
					int iStartHour   = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Hour);
					int iStartMinute = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_Minute);
					//if(iStartHour < timeNow.Hour)  ////当前时间已经超过要求开始时间
					//{
					//	OsnDoCDPSnapshot();
					//}
					//else if((iStartHour == timeNow.Hour) && (timeNow.Minute - iStartMinute >= MONTH_MODE_INTERVAL/1000/60))  ////两者小时相等，分钟差别在10分钟之内
					//{
					//	OsnDoCDPSnapshot();
					//}
					//else if((iStartHour == timeNow.Hour + 1) && (60 - timeNow.Minute + iStartMinute <= MONTH_MODE_INTERVAL/1000/60)) ////当前时间小时比要求时间少一，分钟差别在10分钟之内
					//{
					//	OsnDoCDPSnapshot();
					//}
					if(timeNow.Hour == iStartHour + 1)
					{
						if(60 - iStartMinute + timeNow.Minute <= DEFAULT_MODE_INTERVAL/1000/60/2)
						{
							OsnDoCDPSnapshot();
						}
					}
					else if(timeNow.Hour == iStartHour)
					{
						if(((timeNow.Minute >= iStartMinute) && (timeNow.Minute - iStartMinute <= MONTH_MODE_INTERVAL/1000/60/2))
									||((iStartMinute >= timeNow.Minute) && (iStartMinute - timeNow.Minute <= MONTH_MODE_INTERVAL/1000/60/2)))
						{
							OsnDoCDPSnapshot();
						}
					}
					else if(timeNow.Hour == iStartHour - 1)
					{
						if(60 - timeNow.Minute + iStartMinute <= DEFAULT_MODE_INTERVAL/1000/60/2)
						{
							OsnDoCDPSnapshot();
						}
					}
				}
			}
		
		void OnCDPScheduleDayTimerElapsed(System::Object^ sender,ElapsedEventArgs^ e)
		{
			if(!m_bChangeInterval)
			{
				if(m_CDPSchedule == nullptr)
				{
					return;
				}
                
				if(!CheckVolAndDisk(m_Guid))
				{
					return;
				}
				OsnDoCDPSnapshot();

				int iIntervalDays = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_IntervalDays);
				m_CdpScheduleTimer->Interval = iIntervalDays *24* 60 *60* 1000;
				m_bChangeInterval = true;
			}
			else
			{
				OsnDoCDPSnapshot();
			}
		}
		
		void OnCDPScheduleMinuteTimerElapsed(System::Object^ sender,ElapsedEventArgs^ e)
			{
#ifdef TRACE
				System::Diagnostics::Trace::WriteLine("SCA:Enter OnCDPScheduleMinuteTimerElapsed Routine.");
#endif
				if(!m_bChangeInterval)
				{
					if(m_CDPSchedule == nullptr)
					{
						return;
					}
					
					if(!CheckVolAndDisk(m_Guid))
					{
						return;
					}
					OsnDoCDPSnapshot();

					int iIntervalMinutes = Convert::ToInt32(m_CDPSchedule->m_StartTime->m_IntervalMinutes);
					m_CdpScheduleTimer->Interval = iIntervalMinutes * 60 * 1000;
					m_bChangeInterval = true;
				}
				else
				{
					OsnDoCDPSnapshot();
				}
			}
		
		Int32 GetIntervalInMinute(OSNRpcCommonCfg::CCDPStartTime^ st)
		{
			Int32 Interval=0;
			Int32 ScheduleInterval;
			CDP_SCHEDULE_MODE mode=(CDP_SCHEDULE_MODE)Convert::ToInt16(m_CDPSchedule->m_Mode);
			if(mode==CDP_SCHEDULE_MODE::MODE_DAY)
			{
				 ScheduleInterval=Convert::ToInt32(st->m_IntervalDays)*24*60;
			}
			else
			{
				if(mode==CDP_SCHEDULE_MODE::MODE_MINUTE)
				{
					ScheduleInterval=Convert::ToInt32(st->m_IntervalMinutes);
				}
			}
			String^ strst=st->m_Year;
			strst=String::Concat(strst,".");
			strst=String::Concat(strst,st->m_Month);
			strst=String::Concat(strst,".");
			strst=String::Concat(strst,st->m_Day);
			strst=String::Concat(strst," ");
			strst=String::Concat(strst,st->m_Hour);
			strst=String::Concat(strst,":");
			strst=String::Concat(strst,st->m_Minute);
			strst=String::Concat(strst,":");
			strst=String::Concat(strst,st->m_Second);
			strst=String::Concat(strst,".");
			strst=String::Concat(strst,st->m_Month);
			DateTime startTime=DateTime::Parse(strst);
			DateTime now=DateTime::Now;
			if(startTime>now)
			{
				TimeSpan myspan=startTime-now;
			    Interval=Convert::ToInt32(myspan.TotalMinutes);
			}
			else
			{
				TimeSpan myspan=now-startTime;
				Int32 n=Convert::ToInt32(myspan.TotalMinutes)%(ScheduleInterval*24*60);
                Interval = ScheduleInterval*24*60 - n;
			}
			return Interval;
		}
		
		void CDPSchedule()
		{
			m_bChangeInterval = false;
			
			m_CdpScheduleTimer = gcnew System::Timers::Timer();
			try
			{
				m_Mutex->WaitOne();
				xmlDoc->Load(m_XmlFileName);
				String^ strExp = String::Concat("//CHostMirrorCDPSchedule[m_Guid='",m_Guid,"']");
				XmlNode^ xNode = xmlDoc->SelectSingleNode(strExp);
				if(!xNode)
				{
					m_Mutex->ReleaseMutex();

						
					CEventLog::OSNWriteEventLog("Error: Failed to find the source disk in file CDPSchedule.xml.",EventLogEntryType::Error,017);
					m_ScheduleDeleteEvent->Reset();
					m_ScheduleDeleteEvent->WaitOne();
					
					m_Mutex->WaitOne();
					////read CDP Schedule info from CDPSchedule.xml
					xmlDoc->Load(m_XmlFileName);
					xNode = xmlDoc->SelectSingleNode(strExp);		
					if(!xNode)
					{
						 m_CdpScheduleTimer->Enabled = false;
			             m_CdpScheduleTimer->Close();
			             CDPSchedule();
						 return;
					}
				}
				//String* strCDPVolumeName    = xNode->ChildNodes->Item(0)->InnerText;
				//String* strServerIP      = xNode->ChildNodes->Item(1)->InnerText;
				String^ strSrcsiguature = xNode->ChildNodes->Item(0)->InnerText;

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

				String^ strScheduleMode = xNode->ChildNodes->Item(2)->InnerText;
				m_Mutex->ReleaseMutex();
					
				CCDPStartTime^ startTime = gcnew CCDPStartTime();
				startTime->m_Day       = strStartDay;
				startTime->m_DayOfWeek = strStartDayOfWeek;
				startTime->m_Hour      = strStartHour;
				startTime->m_IntervalDays = strStartIntervalDays;
				startTime->m_Minute    = strStartMinute;
				startTime->m_Month     = strStartMonth;
				startTime->m_Second    = strStartSecond;
				startTime->m_WeekNum   = strStartWeekNum;
				startTime->m_Year      = strStartYear;
				startTime->m_IntervalMinutes = strStartIntervalMinutes;

				CDP_SCHEDULE_MODE scheduleMode = (CDP_SCHEDULE_MODE)(Convert::ToInt16(strScheduleMode));
				m_CDPSchedule = gcnew CHostMirrorCDPSchedule(strSrcsiguature,
					startTime, nullptr, Convert::ToInt16(scheduleMode));
		
				switch(scheduleMode)
				{
				case CDP_SCHEDULE_MODE::MODE_MONTH:
					{
						m_CdpScheduleTimer->Elapsed += gcnew ElapsedEventHandler(this,&HostmirrorScheduleService::CHostMirrorCDPScheduleThread::OnCDPScheduleMonthTimerElapsed);
						m_CdpScheduleTimer->Interval = MONTH_MODE_INTERVAL;
						break;
					}
				case CDP_SCHEDULE_MODE::MODE_WEEK:
					{
						m_CdpScheduleTimer->Elapsed += gcnew ElapsedEventHandler(this,&HostmirrorScheduleService::CHostMirrorCDPScheduleThread::OnCDPScheduleWeekTimerElapsed);
						m_CdpScheduleTimer->Interval = DEFAULT_MODE_INTERVAL;
						break;
					}
				case CDP_SCHEDULE_MODE::MODE_DAY:
					{
						Int32 iStartYear         = Convert::ToInt32(strStartYear);
						Int32 iStartMonth        = Convert::ToInt32(strStartMonth);
						Int32 iStartDay          = Convert::ToInt32(strStartDay);
						Int32 iStartHour         = Convert::ToInt32(strStartHour);
						Int32 iStartMinute       = Convert::ToInt32(strStartMinute);
						Int32 iStartSecond       = Convert::ToInt32(strStartSecond);
						Int32 iStartIntervalDays = Convert::ToInt32(strStartIntervalDays);

						DateTime dateStartCDP = System::DateTime(iStartYear,iStartMonth,iStartDay,
							                                  iStartHour,iStartMinute,iStartSecond);
						DateTime timeNow = DateTime::Now;   
						
						System::TimeSpan diff = timeNow.Subtract(dateStartCDP);
						if(diff.TotalMinutes < 0)
						{
							diff = dateStartCDP.Subtract(timeNow);
							m_CdpScheduleTimer->Interval = (double)diff.TotalMinutes * 60 * 1000;
						}
						else
						{
							Int64 modMinutes = (Int64)diff.TotalMinutes % (iStartIntervalDays*24*60);
							m_CdpScheduleTimer->Interval = (double)(iStartIntervalDays*24*60 - modMinutes) * 60 * 1000;
						}


						m_CdpScheduleTimer->Elapsed += gcnew ElapsedEventHandler(this,&HostmirrorScheduleService::CHostMirrorCDPScheduleThread::OnCDPScheduleDayTimerElapsed);
						break;
					}
				case CDP_SCHEDULE_MODE::MODE_MINUTE:
					{
						Int32 iStartYear         = Convert::ToInt32(strStartYear);
						Int32 iStartMonth        = Convert::ToInt32(strStartMonth);
						Int32 iStartDay          = Convert::ToInt32(strStartDay);
						Int32 iStartHour         = Convert::ToInt32(strStartHour);
						Int32 iStartMinute       = Convert::ToInt32(strStartMinute);
						Int32 iStartSecond       = Convert::ToInt32(strStartSecond);
						Int32 iStartIntervalMinutes = Convert::ToInt32(strStartIntervalMinutes);

						DateTime dateStartCDP = System::DateTime(iStartYear,iStartMonth,iStartDay,
							                                  iStartHour,iStartMinute,iStartSecond);
						DateTime timeNow = DateTime::Now;
						
						System::TimeSpan diff = timeNow.Subtract(dateStartCDP);
						if(diff.TotalMinutes < 0)
						{
							diff = dateStartCDP.Subtract(timeNow);
							m_CdpScheduleTimer->Interval = (double)diff.TotalMinutes * 60 * 1000;
						}
						else
						{
							Int64 modMinutes = (Int64)diff.TotalMinutes % iStartIntervalMinutes;
							m_CdpScheduleTimer->Interval = (double)(iStartIntervalMinutes - modMinutes) * 60 * 1000;
						}

						m_CdpScheduleTimer->Elapsed += gcnew ElapsedEventHandler(this,&HostmirrorScheduleService::CHostMirrorCDPScheduleThread::OnCDPScheduleMinuteTimerElapsed);
						break;
					}
				default:
					break;
				}

				m_CdpScheduleTimer->Enabled = true;
				GC::KeepAlive(m_CdpScheduleTimer);


			   m_ScheduleDeleteEvent->Reset();
			   m_ScheduleDeleteEvent->WaitOne();

			   m_CdpScheduleTimer->Enabled = false;
			   m_CdpScheduleTimer->Close();
			   CDPSchedule();
			}
			catch(Exception ^e)
			{
				CEventLog::OSNWriteEventLog(e->Message,EventLogEntryType::Error,017);
				return;
			}
		}

		
	private:
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
		String^ GetGuidByLabel(String^ label)
		{
			Char Name=label[0];
			unsigned int	signature = 0;	
			unsigned int	startingOffset = 0;
			int ErrorCode = OSNGetVolumeID(Name, signature, startingOffset);
			if(ErrorCode==0)
			{
				String^ tempsignature=nullptr;
				char char_guid[128];
				bool ret=OsnCHeckIsGPTVolume((const char)Name,char_guid);
				if(ret)
				{
				  String^ guid=gcnew String(char_guid);
				  return guid;
				}
				else
				{
					VOLUMEID volume;
					volume.MBP_VolumeID.m_BlockOffset=startingOffset;
					volume.MBP_VolumeID.m_DiskSignature=signature;
					volume.MBP_VolumeID.m_NotUsed1=0;
				    OsnGUIDToString(char_guid,volume.SAN_VolumeID.m_VolumeGuid);
					tempsignature=gcnew String(char_guid);
				}
				return tempsignature;
			}
			return nullptr;
		}
		String^ GetGuidByIndex(int index)
		{
			 DISK_INFO pDisk;
			 memset(&pDisk,0,sizeof(DISK_INFO));
			 pDisk.m_DiskIndex=index;
			int  ErrorCode=OsnGetDisk(&pDisk);
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
						// CEventLog::OSNWriteEventLog(String::Concat(guid,"index=",Convert::ToString(i)),EventLogEntryType::Error,017);
						 return i;
					 }
				 }
			 }
			 return -1;
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
		bool IsStringContained(String ^strSrc, String ^strDes)
		{
			array<String^>^ strArray = nullptr;
			array<Char>^chars = {','};
			strArray = strSrc->Split(chars);
			for(int i=0; i<strArray->Length; i++)
			{
				if(strArray[i]->Equals(strDes))
				{
					return true;
				}
			}
			return false;
		}

		inline void OsnDoCDPSnapshot()
		{
			array<Char>^chars = {'#'};
			array<String^>^ paralist=m_Guid->Split(chars);
			if(nullptr!=paralist&&paralist->Length==2)
			{
				if(paralist[1]->Equals("Volume"))
				{
					String^ volumeLabel=GetVolumeLabelBySignature(paralist[0]);
					MakeVolumeCDP(volumeLabel);
				}
				else if(paralist[1]->Equals("Disk"))
				{
					int index=GetDiskIndexBySignature(paralist[0]);
					//CEventLog::OSNWriteEventLog(paralist[0],EventLogEntryType::Error,017);
					if(-1==index)
					{
						CEventLog::OSNWriteEventLog("找不到该磁盘",EventLogEntryType::Error,017);
						return;
					}
					MakeDiskCDP(index);
				}
			}
			else
			{
				CEventLog::OSNWriteEventLog("参数不对，不创建CDP",EventLogEntryType::Error,017);
			}
        }
	};
}