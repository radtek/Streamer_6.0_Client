using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace HostMirrorCDPSchedule
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(String[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            if (null != args && args.Length > 0)//delete srclabel_srcoffset_volume;add srclabel_srcoffset_volume
            {
                String[] paralist = args[0].Split('@');
                if (paralist[0].ToLower().Equals("delete") || paralist[0].ToLower().Equals("deletesilent"))
                {
                    try
                    {
                        CRetMsgInfo rmi;
                        RpcServiceCommand cmd = RpcServiceCommand.OSN_RC_CMD_DELETE_CDP_SCHEDULE;
                        String[] para = new String[1];
                        para[0] = paralist[1];
                        int ret = CDPScheduleForm.SendCommandToService(cmd, para, out rmi);
                        if (ret != 0 || null == rmi)
                            ret = 5;
                        if (rmi.m_RepServiceStatus == RepServiceStatus.RS_ERROR_NOT_FIND_DISK)
                            ret = 8;
                        else if (rmi.m_RepServiceStatus == RepServiceStatus.RS_SUCCESS)
                            ret = 0;
                        else
                            ret = (int)rmi.m_ErrorCode;
                        if (paralist[0].ToLower().Equals("delete"))
                        {

                            if (ret == 5)
                            {
                                MessageBox.Show("��ȷ�Ͽͻ����Ƿ�֧��CDP����������.", "��ʾ",
                                                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                            }
                            else if (ret != 0)
                            {
                                MessageBox.Show("ɾ�������CDP�ƻ�ʧ��,�������0x" + System.Convert.ToString(ret, 16),
                                                "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (0 == ret)
                            {
                                MessageBox.Show("�ɹ�ɾ��CDP�ƻ�.", "��ʾ",
                                                 MessageBoxButtons.OK, MessageBoxIcon.None);
                            }
                        }
                        else//
                        {
                            if (0 != ret)
                            {
                                MessageBox.Show("ɾ��CDP�ƻ�ʧ��,error code:"+ret.ToString());
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("ɾ��CDP�ƻ�ʱ�����쳣��" + "��������CDP�ƻ�����δ����!");
                    }
                    Application.Exit();
                    Application.ExitThread();
                }
                else if (paralist[0].ToLower().Equals("view"))
                {
                    try
                    {
                        CRetMsgInfo rmi;
                        RpcServiceCommand cmd = RpcServiceCommand.OSN_RC_CMD_GET_CDP_SCHEDULE;
                        String[] para = new String[1];
                        para[0] = paralist[1];
                        int ret = CDPScheduleForm.SendCommandToService(cmd, para, out rmi);
                        if (ret != 0 || null == rmi)
                            ret = 5;
                        if (rmi.m_RepServiceStatus == RepServiceStatus.RS_ERROR_NOT_FIND_DISK)
                            ret = 8;
                        else if (rmi.m_RepServiceStatus == RepServiceStatus.RS_SUCCESS)
                            ret = 0;
                        else
                            ret = (int)rmi.m_ErrorCode;

                        if (ret == 5)
                        {
                            MessageBox.Show("��ȷ�Ͽͻ����Ƿ�֧��CDP����������.", "��ʾ",
                                             MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        }
                        else if (ret != 0)
                        {
                            MessageBox.Show("�鿴������CDP�ƻ�ʧ��,�������0x" + System.Convert.ToString(ret, 16),
                                            "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        else if(0==ret)
                        {
                            MessageBox.Show(rmi.m_RetMsgInfo, "��ʾ",
                                             MessageBoxButtons.OK, MessageBoxIcon.None);
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("��ȡCDP�ƻ�ʱ�����쳣����������CDP�ƻ�����δ����!");
                    }
                    Application.Exit();
                    Application.ExitThread();
                }
                else
                {
                    Application.Run(new CDPScheduleForm(paralist[0], paralist[1]));
                }
            }
            else
            {
                Application.Run(new CDPScheduleForm());
            }
        }
    }
}