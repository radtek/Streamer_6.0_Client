using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Xml.Serialization;
using System.Windows.Forms;


namespace HostMirrorCDPSchedule
{
    public partial class CDPScheduleForm : Form
    {
        private readonly String m_arg;
        protected int m_Step;

        public CDPScheduleForm()
        {
            InitializeComponent();
        }
        public CDPScheduleForm(String type,String args)
        {
          
            InitializeComponent();
            m_arg = args;
            m_Step = 0;//defaul 0 indicate the first step
            if (type.ToLower().Equals("add"))
            {
                splitContainer1.Panel2Collapsed = true;
            }
            this.btnBack.Visible = false;
            this.btnBack.Enabled = false;
        }
        /// <summary>
        /// 转化为字符串形式
        /// </summary>
        /// <returns>NULL表示格式不对</returns>
        public  static String FormatCdpSchedule(CDPSchedule schedule)
        {
            if (ScheduleMode.MODE_NONE == schedule.Mode)
                return null;
            if (ScheduleMode.MODE_MINUTES == schedule.Mode &&
                (schedule.StartDates == null || schedule.StartTime == null || schedule.Modifier == null))
                return null;
            if (ScheduleMode.MODE_DAYLY == schedule.Mode &&
                (schedule.StartDates == null || schedule.StartTime == null || schedule.Modifier == null))
                return null;
            if (ScheduleMode.MODE_WEEKLY == schedule.Mode &&
                (schedule.StartTime == null || schedule.Modifier == null || schedule.Modifier.Week.Days == null))
                return null;
            if (ScheduleMode.MODE_MONTHLY == schedule.Mode &&
               (schedule.StartDates == null || schedule.StartTime == null))
                return null;
            String strSchedule = null;
            String[] weekDayArray = null;
            Date start = null;
            switch (schedule.Mode)
            {
                case ScheduleMode.MODE_MINUTES:
                    start = schedule.StartDates[0];
                    strSchedule = String.Format("Year:{0};Month:{1};Day:{2};WeekNum:ALL;DayOfWeek:ALL",
                                                start.Year, start.Month, start.Day);
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                schedule.StartTime.Hour, schedule.StartTime.Minute, schedule.StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:{1}", strSchedule,
                                                schedule.Modifier.IntervalMinutes);
                    break;
                case ScheduleMode.MODE_DAYLY:
                    start = schedule.StartDates[0];
                    strSchedule = String.Format("Year:{0};Month:{1};Day:{2};WeekNum:ALL;DayOfWeek:ALL",
                                                start.Year, start.Month, start.Day);
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                schedule.StartTime.Hour, schedule.StartTime.Minute, schedule.StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:{1};IntervalMinutes:ALL", strSchedule,
                                                schedule.Modifier.IntervalDays);
                    break;
                case ScheduleMode.MODE_WEEKLY:
                    weekDayArray = new String[schedule.Modifier.Week.Days.Length];
                    for (int index = 0; index < schedule.Modifier.Week.Days.Length; ++index)
                        weekDayArray[index] = ((int)(schedule.Modifier.Week.Days[index])).ToString();
                    strSchedule = String.Format("Year:ALL;Month:ALL;Day:ALL;WeekNum:{0};DayOfWeek:{1}",
                                                schedule.Modifier.Week.Base, String.Join(",", weekDayArray));
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                schedule.StartTime.Hour, schedule.StartTime.Minute, schedule.StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:ALL", strSchedule);
                    break;
                case ScheduleMode.MODE_MONTHLY:
                    String[] monthArray = new String[schedule.StartDates.Length];
                    for (int index = 0; index < schedule.StartDates.Length; ++index)
                        monthArray[index] = schedule.StartDates[index].Month.ToString();
                    bool dayFlag = true;//default to indicate specific day
                    if (schedule.StartDates[0].Day == 0)
                        dayFlag = false;
                    if (schedule.Modifier != null)
                    {
                        weekDayArray = new String[schedule.Modifier.Week.Days.Length];
                        for (int index = 0; index < schedule.Modifier.Week.Days.Length; ++index)
                            weekDayArray[index] = ((int)(schedule.Modifier.Week.Days[index])).ToString();
                    }
                    strSchedule = String.Format("Year:ALL;Month:{0};Day:{1};WeekNum:{2};DayOfWeek:{3}",
                                                String.Join(",", monthArray), dayFlag ? ((int)(schedule.StartDates[0].Day)).ToString() : "ALL",
                                                (dayFlag || null == schedule.Modifier) ? "ALL" : ((int)(schedule.Modifier.Week.Base)).ToString(),
                                                (dayFlag || null == schedule.Modifier) ? "ALL" : String.Join(",", weekDayArray));
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                schedule.StartTime.Hour, schedule.StartTime.Minute, schedule.StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:ALL", strSchedule);
                    break;
                default: break;
            }
            return strSchedule;
        }
        public static int SendCommandToService(RpcServiceCommand cmd, String[] paras, out CRetMsgInfo result)
        {
            result = null;
            String clientIpAddr = "127.0.0.1";
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                MemoryStream ms = new MemoryStream();
                System.Xml.XmlTextWriter txtWriter = new System.Xml.XmlTextWriter(ms, Encoding.UTF8);
                UInt32 paraNum = (UInt32)((null == paras) ? 0 : paras.Length);
                CSocketMsgInfo command = new CSocketMsgInfo(cmd, paraNum, paras);
                XmlSerializer xs = new XmlSerializer(command.GetType());
                xs.Serialize(txtWriter, command);
                Byte[] cmdBuffer = ms.GetBuffer();
                ms.Close();
                Byte[] receivedDataBuffer = new Byte[4096];
                clientSocket.ReceiveTimeout = 15000;
                IPAddress clientAddr = IPAddress.Parse(clientIpAddr);
                int listenPort = 59292;
                clientSocket.Connect(clientAddr, listenPort);
                if (!clientSocket.Connected)
                    return 1;//failed to connect remote
                clientSocket.Send(cmdBuffer, SocketFlags.None);
                clientSocket.Receive(receivedDataBuffer, SocketFlags.None);
                xs = new XmlSerializer(typeof(CRetMsgInfo));
                StreamReader sr = new StreamReader(new MemoryStream(receivedDataBuffer), Encoding.UTF8);
                result = (CRetMsgInfo)(xs.Deserialize(sr));
                sr.Close();
                return 0;
            }
            catch
            {
                result = null;
                return -1;
            }
            finally
            {
                if (clientSocket != null)
                    clientSocket.Close();
            }
        }

        private int CreateCDPSchedule(HostMirrorCDPSchedule.CDPSchedule schedule)
        {
            int ret = 0;
          
            CRetMsgInfo rmi;
            RpcServiceCommand cmd = RpcServiceCommand.OSN_RC_CMD_CREATE_CDP_SCHEDULE;
            CDP_SCHEDULE_MODE mode = CDP_SCHEDULE_MODE.MODE_DAY;
            switch (schedule.Mode)
            {
                case HostMirrorCDPSchedule.ScheduleMode.MODE_MINUTES:
                    mode = CDP_SCHEDULE_MODE.MODE_MINUTE; break;
                case HostMirrorCDPSchedule.ScheduleMode.MODE_DAYLY:
                    mode = CDP_SCHEDULE_MODE.MODE_DAY; break;
                case HostMirrorCDPSchedule.ScheduleMode.MODE_WEEKLY:
                    mode = CDP_SCHEDULE_MODE.MODE_WEEK; break;
                case HostMirrorCDPSchedule.ScheduleMode.MODE_MONTHLY:
                    mode = CDP_SCHEDULE_MODE.MODE_MONTH; break;
                default: break;
            }
            String strStartTime = FormatCdpSchedule(schedule);
            String[] paras = new String[] { m_arg, strStartTime,null,((int)mode).ToString() };
            ret = SendCommandToService(cmd, paras, out rmi);
            if (ret != 0 || null == rmi)
                ret = 5;
            if (rmi.m_RepServiceStatus == RepServiceStatus.RS_ERROR_NOT_FIND_DISK)
                ret = 8;
            else if (rmi.m_RepServiceStatus == RepServiceStatus.RS_SUCCESS)
                ret = 0;
            else
                ret=(int)(rmi.m_ErrorCode);
           if (ret == 5)
            {
                MessageBox.Show("请确认客户端是否支持CDP并运行正常.", "提示",
                                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
            else if (ret == 8)
            {
                MessageBox.Show("客户端未找到此卷.", "错误",
                                 MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (ret != 0)
            {
                MessageBox.Show("设置CDP计划失败,错误代码0x" + Convert.ToString(ret, 16),
                                 "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                MessageBox.Show("成功设置镜像的CDP计划.", "提示",
                                 MessageBoxButtons.OK, MessageBoxIcon.None);
            }
            return ret;
        }

        private void SetCheckBox(bool isVisible, bool isWeekly)
        {
            label5.Visible = isVisible;
            checkBox1.Visible = isVisible;
            checkBox2.Visible = isVisible;
            checkBox3.Visible = isVisible;
            checkBox4.Visible = isVisible;
            checkBox5.Visible = isVisible;
            checkBox6.Visible = isVisible;
            checkBox7.Visible = isVisible;
            checkBox8.Visible = (isVisible) && (!isWeekly);
            checkBox9.Visible = (isVisible) && (!isWeekly);
            checkBox10.Visible = (isVisible) && (!isWeekly);
            checkBox11.Visible = (isVisible) && (!isWeekly);
            checkBox12.Visible = (isVisible) && (!isWeekly);
            if (isVisible && isWeekly)
            {
                checkBox1.Text = "星期一";
                checkBox2.Text = "星期二";
                checkBox3.Text = "星期三";
                checkBox4.Text = "星期四";
                checkBox5.Text = "星期五";
                checkBox6.Text = "星期六";
                checkBox7.Text = "星期日";
            }
            else if (isVisible)
            {
                checkBox1.Text = "一月";
                checkBox2.Text = "二月"; ;
                checkBox3.Text = "三月"; ;
                checkBox4.Text = "四月"; ;
                checkBox5.Text = "五月"; ;
                checkBox6.Text = "六月"; ;
                checkBox7.Text = "七月"; ;
                checkBox8.Text = "八月";
                checkBox9.Text = "九月";
                checkBox10.Text = "十月";
                checkBox11.Text = "十一月";
                checkBox12.Text = "十二月";
            }
        }

        private void btnBack_Click(object sender, EventArgs e)
        {
            if (m_Step == 0)
                return;
            --m_Step;
            btnNext.Text = "下一步";
            splitContainer1.Panel2Collapsed = true;
            this.btnBack.Visible = false;
            this.btnBack.Enabled = false;
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            if (m_Step == 0)
            {
                rbChoose1.Text = "每";
                if (rbModeMinutes.Checked)
                {
                    lbUnit.Text = "分钟";
                    nudModifier.Maximum = 1440;
                    rbChoose2.Visible = false;
                    cbWeekBase.Visible = false;
                    cbDayOfWeek.Visible = false;
                    startDatePicker.Enabled = true;
                    SetCheckBox(false, false);
                }
                else if (rbModeDayly.Checked)
                {
                    lbUnit.Text = "天";
                    nudModifier.Maximum = 365;
                    rbChoose2.Visible = false;
                    cbWeekBase.Visible = false;
                    cbDayOfWeek.Visible = false;
                    startDatePicker.Enabled = true;
                    SetCheckBox(false, false);
                }
                else if (rbModeWeekly.Checked)
                {
                    lbUnit.Text = "周";
                    nudModifier.Maximum = 1;
                    rbChoose2.Visible = false;
                    cbWeekBase.Visible = false;
                    cbDayOfWeek.Visible = false;
                    startDatePicker.Enabled = false;
                    SetCheckBox(true, true);
                }
                else if (rbModeMonthly.Checked)
                {
                    rbChoose1.Text = "第";
                    lbUnit.Text = "天";
                    nudModifier.Maximum = 31;
                    rbChoose2.Visible = true;
                    cbWeekBase.Visible = true;
                    cbDayOfWeek.Visible = true;
                    startDatePicker.Enabled = false;
                    SetCheckBox(true, false);
                    cbWeekBase.SelectedIndex = 0;
                    cbDayOfWeek.SelectedIndex = 0;
                }
                else
                {
                    return;
                }
                splitContainer1.Panel1Collapsed = true;
                ++m_Step;
                this.btnBack.Visible = true;
                this.btnBack.Enabled = true;
                btnNext.Text = "确定";
            }
            else
            {
                HostMirrorCDPSchedule.Date[] startDates = null;
                HostMirrorCDPSchedule.ScheduleModifier modifier = new HostMirrorCDPSchedule.ScheduleModifier();
                HostMirrorCDPSchedule.Time startTime = new HostMirrorCDPSchedule.Time();
                String[] temp = startTimePicker.Text.Split(new String[] { ":" },
                                                           StringSplitOptions.RemoveEmptyEntries);
                startTime.Hour = Convert.ToUInt32(temp[0]);
                startTime.Minute = Convert.ToUInt32(temp[1]);
                startTime.Sencond = 0;
                HostMirrorCDPSchedule.CDPSchedule schedule = new HostMirrorCDPSchedule.CDPSchedule();
                schedule.Mode = HostMirrorCDPSchedule.ScheduleMode.MODE_NONE;
                if (rbModeMinutes.Checked)
                {
                    startDates = new HostMirrorCDPSchedule.Date[1];
                    startDates[0] = new HostMirrorCDPSchedule.Date();
                    startDates[0].Year = (UInt32)(startDatePicker.Value.Year);
                    startDates[0].Month = (UInt32)(startDatePicker.Value.Month);
                    startDates[0].Day = (UInt32)(startDatePicker.Value.Day);
                    schedule.Mode = HostMirrorCDPSchedule.ScheduleMode.MODE_MINUTES;
                    modifier.IntervalMinutes = Convert.ToUInt32(nudModifier.Value);
                }
                else if (rbModeDayly.Checked)
                {
                    startDates = new HostMirrorCDPSchedule.Date[1];
                    startDates[0] = new HostMirrorCDPSchedule.Date();
                    startDates[0].Year = (UInt32)(startDatePicker.Value.Year);
                    startDates[0].Month = (UInt32)(startDatePicker.Value.Month);
                    startDates[0].Day = (UInt32)(startDatePicker.Value.Day);
                    schedule.Mode = HostMirrorCDPSchedule.ScheduleMode.MODE_DAYLY;
                    modifier.IntervalDays = Convert.ToUInt32(nudModifier.Value);
                }
                else if (rbModeWeekly.Checked)
                {
                    schedule.Mode = HostMirrorCDPSchedule.ScheduleMode.MODE_WEEKLY;
                    modifier.Week = new HostMirrorCDPSchedule.WeekModifier();
                    modifier.Week.Base = HostMirrorCDPSchedule.WeekBase.NONE;
                    List<HostMirrorCDPSchedule.DayOfWeek> dayOfWeekList = new List<HostMirrorCDPSchedule.DayOfWeek>();
                    if (checkBox1.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Monday);
                    if (checkBox2.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Tuesday);
                    if (checkBox3.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Wednesday);
                    if (checkBox4.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Thursday);
                    if (checkBox5.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Friday);
                    if (checkBox6.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Saturday);
                    if (checkBox7.Checked)
                        dayOfWeekList.Add(HostMirrorCDPSchedule.DayOfWeek.Sunday);
                    if (dayOfWeekList.Count == 0)
                        return;
                    modifier.Week.Days = dayOfWeekList.ToArray();
                }
                else if (rbModeMonthly.Checked)
                {
                    bool[] yearsignal = new bool[13] { false,false,false,false,false,false,
                        false,false,false,false,false,false,false};

                    schedule.Mode = HostMirrorCDPSchedule.ScheduleMode.MODE_MONTHLY;
                    List<UInt32> monthList = new List<UInt32>();
                    if (checkBox1.Checked)
                    {
                        monthList.Add(1);
                        yearsignal[1]=true;
                    }
                    if (checkBox2.Checked)
                    {
                        monthList.Add(2);
                        yearsignal[2] = true;
                    }
                    if (checkBox3.Checked)
                    {
                        monthList.Add(3);
                        yearsignal[3] = true;
                    }
                    if (checkBox4.Checked)
                    {
                        monthList.Add(4);
                        yearsignal[4] = true;
                    }
                    if (checkBox5.Checked)
                    {
                        monthList.Add(5);
                        yearsignal[5] = true;
                    }
                    if (checkBox6.Checked)
                    {
                        monthList.Add(6);
                        yearsignal[6] = true;
                    }
                    if (checkBox7.Checked)
                    {
                        monthList.Add(7);
                        yearsignal[7] = true;
                    }
                    if (checkBox8.Checked)
                    {
                        monthList.Add(8);
                        yearsignal[8] = true;
                    }
                    if (checkBox9.Checked)
                    {
                        monthList.Add(9);
                        yearsignal[9] = true;
                    }
                    if (checkBox10.Checked)
                    {
                        monthList.Add(10);
                        yearsignal[10] = true;
                    }
                    if (checkBox11.Checked)
                    {
                        monthList.Add(11);
                        yearsignal[11] = true;
                    }
                    if (checkBox12.Checked)
                    {
                        monthList.Add(12);
                        yearsignal[12] = true;
                    }
                    if (monthList.Count == 0)
                        return;
                    startDates = new HostMirrorCDPSchedule.Date[monthList.Count];
                    for (int i = 0; i < monthList.Count; ++i)
                    {
                        startDates[i] = new HostMirrorCDPSchedule.Date();
                        startDates[i].Year = 0;
                        startDates[i].Month = monthList[i];
                        startDates[i].Day = 0;
                    }
                    if (rbChoose1.Checked)
                    {
                        startDates[0].Day = Convert.ToUInt32(nudModifier.Value);
                        int year = DateTime.Now.Year;

                        for (int i = 1; i <= 12;i++ )
                        {
                            if (yearsignal[i] == true)
                            {
                                switch (i)
                                {
                                    case 2:
                                        {
                                            if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
                                            {
                                                if (startDates[0].Day > 29)
                                                {
                                                    MessageBox.Show("二月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                                    return;
                                                }
                                            }
                                            else
                                            {
                                                if (startDates[0].Day > 28)
                                                {
                                                    MessageBox.Show("二月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                                    return;
                                                }
                                            }
                                        };
                                        break;

                                    case 4:
                                        if (startDates[0].Day > 30)
                                        {
                                            MessageBox.Show("四月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                            return;
                                        };
                                        break;
  
                                    case 6:
                                        if (startDates[0].Day > 30)
                                        {
                                            MessageBox.Show("六月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                            return;
                                        };
                                        break;

                                    case 9:
                                        if (startDates[0].Day > 30)
                                        {
                                            MessageBox.Show("九月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                            return;
                                        };
                                        break;

                                    case 11:
                                        if (startDates[0].Day > 30)
                                        {
                                            MessageBox.Show("十一月份天数设置有误", "天数设置", MessageBoxButtons.OK);
                                            return;
                                        };
                                        break;
                                }

                            }
                        }
                        modifier = null;
                    }
                    else if (rbChoose2.Checked)
                    {
                        startDates[0].Day = 0;
                        modifier.Week = new HostMirrorCDPSchedule.WeekModifier();
                        modifier.Week.Base = (HostMirrorCDPSchedule.WeekBase)(cbWeekBase.SelectedIndex + 1);
                        modifier.Week.Days = new HostMirrorCDPSchedule.DayOfWeek[1] { (HostMirrorCDPSchedule.DayOfWeek)(cbDayOfWeek.SelectedIndex) };
                    }
                }
                schedule.StartDates = startDates;
                schedule.StartTime = startTime;
                schedule.EndDates = null;
                schedule.EndTime = null;
                schedule.Modifier = modifier;
                this.Cursor = Cursors.WaitCursor;
                try
                {
                    int ret = CreateCDPSchedule(schedule);
                    if(ret==0)
                        this.DialogResult = DialogResult.OK;
                }
                catch
                {
                    MessageBox.Show("设置CDP计划发生异常.", "警告",
                                     MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                Application.Exit();
            }
        }
    }
}