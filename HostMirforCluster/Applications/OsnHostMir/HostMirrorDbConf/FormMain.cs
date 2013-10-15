using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Management;
using Microsoft.Win32;
using System.Security.Cryptography;
using System.IO;
using System.Diagnostics;//Conditional
using OSNDatabaseApi;

namespace ApplicationCfg
{
    public partial class FormMain : Form
    {
        private const string strVSSRegPath = @"System\CurrentControlSet\Services\OsnVSS";
        private const string strVSSParameterRegPath = @"System\CurrentControlSet\Services\OsnVSS\Parameters";

        public FormMain()
        {
            InitializeComponent();

           
            radioBtnOracleNo.Checked = true;
            labelChooseOracleVol.Enabled = false;
            comboBoxOracleVolumes.Enabled = false;
            checkForceOracle.Enabled = false;

            //SQLServer
            radioBtnSqlNo.Checked = true;
            labelChooseSqlVol.Enabled = false;
            comboBoxSqlVolumes.Enabled = false;
            checkForceSQLServer.Enabled = false;

            //Sybase
            rdBtnSybaseNo.Checked = true;
            labelChooseSybaseVol.Enabled = false;
            comboBoxSybaseVolumes.Enabled = false;
            labelSybaseAdmin.Enabled = false;//sybase用户名
            txtSybaseAdmin.Enabled = false;
            labelSybasePass.Enabled = false;
            txtSybasePassword.Enabled = false;
            checkForceSybase.Enabled = false;

            //MySQL
            rdBtnMySQLNo.Checked = true;
            labelChooseMysql.Enabled = false;
            comboBoxMySQLVolumes.Enabled = false;
            labelMySQLAdmin.Enabled = false;
            txtMySQLAdmin.Enabled = false;
            labelMySQLPass.Enabled = false;
            txtMySQLPassword.Enabled = false;
            checkForceMySQL.Enabled = false;

            //Domino
            rdBtnDominoNO.Checked = true;
            //ExChange
            radioBtnExNo.Checked = true;
            labelChooseEx.Enabled = false;
            comboBoxExVolumes.Enabled = false;
            checkForceEx.Enabled = false;

            //informix
            radioBtnInformixNo.Checked = true;
            label6.Enabled = false;
            comboBoxInformixVolumes.Enabled = false;
            checkForceInofrmix.Enabled = false;


            m_VolumeLabelList = new List<string>();

            OSNStartService("winmgmt");
            OSNStartService("Wmi");

            RefreshComboBoxVolumes();
        }

        private String OSNStartService(String serviceName)
        {
            String strRet = String.Empty;
            System.Diagnostics.Process newProcess = new System.Diagnostics.Process();
            try
            {
                newProcess.StartInfo.RedirectStandardOutput = true;
                newProcess.StartInfo.RedirectStandardInput = true;
                newProcess.StartInfo.CreateNoWindow = true;
                newProcess.StartInfo.UseShellExecute = false;

                newProcess.StartInfo.FileName = "cmd.exe";
                newProcess.Start();

                StreamWriter input = newProcess.StandardInput;
                input.WriteLine(String.Concat("net start ", serviceName));
                input.Flush();
                input.Close();

                strRet = newProcess.StandardOutput.ReadToEnd();

                newProcess.WaitForExit();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            finally
            {
                if (newProcess != null && !newProcess.HasExited)
                {
                    newProcess.Kill();
                }
                if (newProcess != null)
                {
                    newProcess.Close();
                }
            }

            return strRet;
        }

        private void RefreshVolumeList()
        {
            m_VolumeLabelList.Clear();
            try
            {
                ManagementObjectSearcher myQuery = new ManagementObjectSearcher(
                                                       "SELECT * FROM Win32_LogicalDisk WHERE DriveType=3");
                ManagementObjectCollection myQueryCollection = myQuery.Get();
                IEnumerator loopMC = myQueryCollection.GetEnumerator();
                while (loopMC.MoveNext())
                {
                    ManagementObject myMO = (ManagementObject)(loopMC.Current);
                    string strVolLabel = (string)(myMO.GetPropertyValue("Name"));
                    m_VolumeLabelList.Add(strVolLabel);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void RefreshComboBoxVolumes()
        {
            RefreshVolumeList();
            foreach (string strVolLabel in m_VolumeLabelList)
            {
                comboBoxOracleVolumes.Items.Add(strVolLabel);
                comboBoxSqlVolumes.Items.Add(strVolLabel);
                comboBoxSybaseVolumes.Items.Add(strVolLabel);
                comboBoxMySQLVolumes.Items.Add(strVolLabel);
                comboBoxExVolumes.Items.Add(strVolLabel);
                comboBoxInformixVolumes.Items.Add(strVolLabel);
            }

            radioBtnOracleNo.Checked = true;
            radioBtnSqlNo.Checked = true;
            rdBtnMySQLNo.Checked = true;
            rdBtnSybaseNo.Checked = true;
            rdBtnDominoNO.Checked = true;
            radioBtnExNo.Checked = true;
            radioBtnInformixNo.Checked = true;

            RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
            if (serviceKey != null)
            {
                ////SQL Server configuration
                try
                {
                    String strSqlVol = serviceKey.GetValue("SQLServerDataVolume").ToString();
                    if (strSqlVol != null)
                    {
                        comboBoxSqlVolumes.Text = strSqlVol;
                        radioBtnSqlYes.Checked = true;
                        labelChooseSqlVol.Enabled = true;
                        checkForceSQLServer.Enabled = true;
                        int forcesql = Convert.ToInt16(serviceKey.GetValue("SQLServerForce").ToString());
                        if (0 == forcesql)
                        {
                            checkForceSQLServer.Checked = false;
                        }
                        else
                        {
                            checkForceSQLServer.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxSqlVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxSqlVolumes.Text = String.Empty;
                }
                ////Oracle configuration
                try
                {
                    String strOracleVol = serviceKey.GetValue("OracleDataVolume").ToString();
                    if (strOracleVol != null)
                    {
                        comboBoxOracleVolumes.Text = strOracleVol;
                        radioBtnOracleYes.Checked = true;
                        labelChooseOracleVol.Enabled = true;
                        checkForceOracle.Enabled = true;
                       /* string strOracleInstance = serviceKey.GetValue("OracleInstance").ToString();
                        if (strOracleInstance != null)
                        {
                            txtOracleInstance.Text = strOracleInstance;
                        }*/
                        int forceoracle = Convert.ToInt16(serviceKey.GetValue("OracleForce").ToString());
                        if (0 == forceoracle)
                        {
                            checkForceOracle.Checked = false;
                        }
                        else
                        {
                            checkForceOracle.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxOracleVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxOracleVolumes.Text = String.Empty;
                }

                ////Sybase configuration
                try
                {
                    String strSybaseVol = serviceKey.GetValue("SybaseDataVolume").ToString();
                    if (strSybaseVol != null)
                    {
                        this.comboBoxSybaseVolumes.Text = strSybaseVol;
                        rdBtnSybaseYes.Checked = true;
                        labelSybaseAdmin.Enabled = true;
                        labelSybasePass.Enabled = true;
                        labelChooseSybaseVol.Enabled = true;
                        string strSybaseAdmin = serviceKey.GetValue("SybaseAdmin").ToString();
                        if (strSybaseAdmin != null)
                        {
                            string admin = OSNDecry(strSybaseAdmin);
                            txtSybaseAdmin.Text = admin;
                        }
                        else
                        {
                            txtSybaseAdmin.Text = string.Empty;
                        }
                        string strSybasePass = serviceKey.GetValue("SybasePass").ToString();
                        if (strSybasePass != null)
                        {
                            string admin = OSNDecry(strSybasePass);
                            txtSybasePassword.Text = admin;
                        }
                        else
                        {
                            txtSybasePassword.Text = string.Empty;
                        }
                        string strSybaseInstance = serviceKey.GetValue("Sybaseinstance").ToString();
                        if (strSybaseInstance != null)
                        {
                            txtSybaseInstance.Text = strSybaseInstance;
                        }
                        checkForceSybase.Enabled = true;
                        int forcesybase = Convert.ToInt16(serviceKey.GetValue("SybaseForce").ToString());
                        if (0 == forcesybase)
                        {
                            checkForceSybase.Checked = false;
                        }
                        else
                        {
                            checkForceSybase.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxSybaseVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxSybaseVolumes.Text = String.Empty;
                    txtSybaseAdmin.Text = string.Empty;
                }
                ////MySQL configuration
                try
                {
                    String strMySQLVol = serviceKey.GetValue("MySQLDataVolume").ToString();
                    if (strMySQLVol != null)
                    {
                        this.comboBoxMySQLVolumes.Text = strMySQLVol;
                        rdBtnMySQLYes.Checked = true;
                        labelMySQLAdmin.Enabled = true;
                        labelMySQLPass.Enabled = true;
                        string strMySQLAdmin = serviceKey.GetValue("MySQLAdmin").ToString();
                        if (strMySQLAdmin != null)
                        {
                            string admin = OSNDecry(strMySQLAdmin);
                            txtMySQLAdmin.Text = admin;
                        }
                        else
                        {
                            txtMySQLAdmin.Text = string.Empty;
                        }
                        txtMySQLPassword.Text = "******";
                        checkForceMySQL.Enabled = true;
                        int forcemysql = Convert.ToInt16(serviceKey.GetValue("MySQLForce").ToString());
                        if (0 == forcemysql)
                        {
                            checkForceMySQL.Checked = false;
                        }
                        else
                        {
                            checkForceMySQL.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxMySQLVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxMySQLVolumes.Text = String.Empty;
                    txtMySQLAdmin.Text = string.Empty;
                }
                ////Domino configuretion
                try
                {
                    String strDominoLVol = serviceKey.GetValue("DominoDataVolume").ToString();
                    if (strDominoLVol != null)
                    {
                        rdBtnDominoYES.Checked= true;
                        checkForceDomino.Enabled = true;
                        int forcedomino = Convert.ToInt16(serviceKey.GetValue("DominoForce").ToString());
                        if (0 == forcedomino)
                        {
                            checkForceDomino.Checked = false;
                        }
                        else
                        {
                            checkForceDomino.Checked = true;
                        }
                    }
                }
                catch (Exception ex)
                {
                    checkForceDomino.Enabled = false;
                }
                ////ExChange configuration
                try
                {
                    String strSqlVol = serviceKey.GetValue("ExChangeDataVolume").ToString();
                    if (strSqlVol != null)
                    {
                        comboBoxExVolumes.Text = strSqlVol;
                        radioBtnExYes.Checked = true;
                        labelChooseEx.Enabled = true;
                        checkForceEx.Enabled = true;
                        int forceEx = Convert.ToInt16(serviceKey.GetValue("ExChangeForce").ToString());
                        if (0 == forceEx)
                        {
                            checkForceEx.Checked = false;
                        }
                        else
                        {
                            checkForceEx.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxExVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxExVolumes.Text = String.Empty;
                }


                ////informix configuration
                try
                {
                    String strinformixVol = serviceKey.GetValue("InformixDataVolume").ToString();
                    if (strinformixVol != null)
                    {
                        comboBoxInformixVolumes.Text = strinformixVol;
                        radioBtnInformixYes.Checked = true;
                       label6.Enabled = true;
                       checkForceInofrmix.Enabled = true;
                        int forceinformix= Convert.ToInt16(serviceKey.GetValue("InformixForce").ToString());
                        if (0 == forceinformix)
                        {
                            checkForceInofrmix.Checked = false;
                        }
                        else
                        {
                            checkForceInofrmix.Checked = true;
                        }
                    }
                    else
                    {
                        comboBoxInformixVolumes.Text = String.Empty;
                    }
                }
                catch (Exception)
                {
                    comboBoxInformixVolumes.Text = String.Empty;
                }
                serviceKey.Close();
            }
            else
            {
                comboBoxOracleVolumes.Text = String.Empty;
                comboBoxSqlVolumes.Text = String.Empty;
                comboBoxMySQLVolumes.Text = string.Empty;
                comboBoxSybaseVolumes.Text = string.Empty;
                comboBoxExVolumes.Text = string.Empty;
                comboBoxInformixVolumes.Text = string.Empty;
            }

        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            ////Sql Server configuration
            if (radioBtnSqlNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("SQLServerDataVolume", false);
                    serviceKey.DeleteValue("SQLServerForce", false);
                    serviceKey.Close();
                }
            }
            else
            {
                if (!SQLServerConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }


            ////Oracle configuration
            if (radioBtnOracleNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("OracleDataVolume", false);
                    serviceKey.DeleteValue("OracleForce", false);
                    serviceKey.Close();
                }

                //this.Dispose();
               // return;
            }
            else if (radioBtnOracleYes.Checked)
            {
                if (!OracleConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            //// Sybase configuration
            if (rdBtnSybaseNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("SybaseDataVolume", false);
                    serviceKey.DeleteValue("SybaseAdmin", false);
                    serviceKey.DeleteValue("SybasePass", false);
                    serviceKey.DeleteValue("SybaseForce", false);
                    serviceKey.Close();
                }
            }
            else if (rdBtnSybaseYes.Checked)
            {
                if (!SybaseConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            ////MYSQL configuration
            if (rdBtnMySQLNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("MySQLDataVolume", false);
                    serviceKey.DeleteValue("MySQLAdmin", false);
                    serviceKey.DeleteValue("MySQLPass", false);
                    serviceKey.DeleteValue("MySQLForce", false);
                    serviceKey.Close();
                }
            }
            else if (rdBtnMySQLYes.Checked)
            {
                if (!MySQLConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            ////Domino  configuration
            if (rdBtnDominoNO.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("DominoDataVolume", false);
                    serviceKey.DeleteValue("DominoForce", false);
                    serviceKey.Close();
                }
            }
            else if (rdBtnDominoYES.Checked)
            {
                if (!DominoConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            //// ExChange configuration
            if (radioBtnExNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("ExchangeDataVolume", false);
                    serviceKey.DeleteValue("ExchangeForce", false);
                    serviceKey.Close();
                }
            }
            else
            {
                if (!ExchangeConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            //// informix configuration
            if (radioBtnInformixNo.Checked)
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.DeleteValue("InformixDataVolume", false);
                    serviceKey.DeleteValue("InformixForce", false);
                    serviceKey.Close();
                }
            }
            else
            {
                if (!InformixConfig())
                {
                    MessageBox.Show("请重新配置数据库信息");
                    return;
                }
            }
            this.Dispose();
            return;


        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Dispose();
        }

        private List<string> m_VolumeLabelList;

        private bool OracleConfig()
        {
            if (comboBoxOracleVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择Oracle数据存放目录。");
                return false;
            }
            /*else if (txtOracleInstance.Text.Equals(String.Empty))
            {
                MessageBox.Show("请填写Oracle的数据库实例名");
                return false;
            }*/
            else
            {
                //bool isOracle8i = CheckOracle8i();
                //是Oracle8i
                /* string adminName="";
                 string adminPass="";
                 if (isOracle8i)
                 {
                     Oracle8i oracle = new Oracle8i();
                     if (DialogResult.Yes == oracle.ShowDialog())
                     {
                         string Name = oracle.adminName;
                         string Pass = oracle.adminPass;
                         adminName = OSNEncry(Name);
                         adminPass = OSNEncry(Pass);
                     }
                     oracle.Dispose();
                 }*/
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("OracleDataVolume", comboBoxOracleVolumes.Text);
                   // serviceKey.SetValue("OracleInstance", txtOracleInstance.Text);
                    /* if (isOracle8i)
                     {
                         serviceKey.SetValue("OracleAdmin", adminName);
                         serviceKey.SetValue("OraclePass", adminPass);
                     }*/
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("OracleDataVolume", comboBoxOracleVolumes.Text);
                        //serviceKey.SetValue("OracleInstance", txtOracleInstance.Text);
                        /* if (isOracle8i)
                         {
                             serviceKey.SetValue("OracleAdmin", adminName);
                             serviceKey.SetValue("OraclePass", adminPass);
                         }*/
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceOracle.Checked)
                {
                    serviceKey.SetValue("OracleForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("OracleForce", 0);
                    serviceKey.Close();
                }
                //this.Dispose();
            }
            return true;
        }
        private bool SQLServerConfig()
        {
            if (comboBoxSqlVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择SQL Server数据存放目录。");
                return false;
            }
            else
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("SQLServerDataVolume", comboBoxSqlVolumes.Text);
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("SQLServerDataVolume", comboBoxSqlVolumes.Text);
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceSQLServer.Checked)
                {
                    serviceKey.SetValue("SQLServerForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("SQLServerForce", 0);
                    serviceKey.Close();
                }
            }
            return true;
        }
        private bool SybaseConfig()
        {
            if (comboBoxSybaseVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择Sybase数据存放目录。");
                return false;
            }
            else if (txtSybaseInstance.Text.Equals(String.Empty))
            {
                MessageBox.Show("请设置Sybase数据库名.");
                return false;
            }
            else
            {

                if (string.Empty == this.txtSybaseAdmin.Text.Trim())
                {
                    return false;
                }
                string admin = OSNEncry(this.txtSybaseAdmin.Text.Trim());
                string pass = OSNEncry(this.txtSybasePassword.Text.Trim());
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("SybaseDataVolume", comboBoxSybaseVolumes.Text);
                    serviceKey.SetValue("SybaseAdmin", admin);
                    serviceKey.SetValue("SybasePass", pass);
                    serviceKey.SetValue("SybaseInstance", txtSybaseInstance.Text);
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("SybaseDataVolume", comboBoxSybaseVolumes.Text);
                        serviceKey.SetValue("SybaseAdmin", admin);
                        serviceKey.SetValue("SybasePass", pass);
                        serviceKey.SetValue("SybaseInstance", txtSybaseInstance.Text);
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceSybase.Checked)
                {
                    serviceKey.SetValue("SybaseForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("SybaseForce", 0);
                    serviceKey.Close();
                }
            }
            return true;
        }
        private bool MySQLConfig()
        {
            if (this.comboBoxMySQLVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择SQL Server数据存放目录。");
                return false;
            }
            else
            {

                if (string.Empty == this.txtMySQLAdmin.Text.Trim())
                {
                    return false;
                }  
                string admin = OSNEncry(this.txtMySQLAdmin.Text.Trim());
                string pass = OSNEncry(this.txtMySQLPassword.Text.Trim());
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("MySQLDataVolume", comboBoxMySQLVolumes.Text);
                    serviceKey.SetValue("MySQLAdmin", admin);
                    serviceKey.SetValue("MySQLPass", pass);
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("MySQLDataVolume", comboBoxMySQLVolumes.Text);
                        serviceKey.SetValue("MySQLAdmin", admin);
                        serviceKey.SetValue("MySQLPass", pass);
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceMySQL.Checked)
                {
                    serviceKey.SetValue("MySQLForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("MySQLForce", 0);
                    serviceKey.Close();
                }
            }
            return true;
        }
        private bool DominoConfig()
        {
            String DominoVol = null;
            String DominoPath = null;
            try
            {
                RegistryKey RegKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Lotus\\Domino");
                if (RegKey != null)
                {
                    DominoPath = RegKey.GetValue("Path").ToString();
                    if (null != DominoPath)
                    {
                        DominoVol = DominoPath.Substring(0, 2);
                    }
                }
                RegKey.Close();
            }
            catch (Exception ex)
            {
                return false;

            }

            
            String localPath = AppDomain.CurrentDomain.BaseDirectory + "Dmino.dll";
            string localPath1 = AppDomain.CurrentDomain.BaseDirectory + "OSNDminoAgent.exe";
            String localPath2 = AppDomain.CurrentDomain.BaseDirectory + "OSNApi.dll";
            String desPath = DominoPath + @"\Dmino.dll";
            String desPath1 = DominoPath + @"\OSNDminoAgent.exe";
            String desPath2 = DominoPath + @"\OSNApi.dll";
            try
            {
                File.Copy(localPath, desPath, true);
                File.Copy(localPath1, desPath1, true);
                File.Copy(localPath2, desPath2, true);
                CopyFile(DominoPath);
            }
            catch (Exception ee)
            {
                MessageBox.Show(ee.Message.ToString());
                return false;
            }


            RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
            if (serviceKey != null)
            {
                serviceKey.SetValue("DominoDataVolume", DominoVol);
            }
            else
            {
                serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("DominoDataVolume", DominoVol);
                }
            }
            //执行过上面的操作后一定已经有该注册表项，所以不判断
            if (checkForceDomino.Checked)
            {
                serviceKey.SetValue("DominoForce", 1);
                serviceKey.Close();
            }
            else
            {
                serviceKey.SetValue("DominoForce", 0);
                serviceKey.Close();
            }
            return true;
        }
        private bool ExchangeConfig()
        {
            if (comboBoxExVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择Exchange数据存放目录。");
                return false;
            }
            else
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("ExChangeDataVolume", comboBoxExVolumes.Text);
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("ExChangeDataVolume", comboBoxExVolumes.Text);
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceEx.Checked)
                {
                    serviceKey.SetValue("ExChangeForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("ExChangeForce", 0);
                    serviceKey.Close();
                }
            }
            return true;
        }
        private bool InformixConfig()
        {
            if (comboBoxInformixVolumes.Text.Equals(string.Empty))
            {
                MessageBox.Show("请先选择Informix数据存放目录。");
                return false;
            }
            else
            {
                RegistryKey serviceKey = Registry.LocalMachine.OpenSubKey(strVSSParameterRegPath, true);
                if (serviceKey != null)
                {
                    serviceKey.SetValue("InformixDataVolume", comboBoxInformixVolumes.Text);
                }
                else
                {
                    serviceKey = Registry.LocalMachine.CreateSubKey(strVSSParameterRegPath);
                    if (serviceKey != null)
                    {
                        serviceKey.SetValue("InformixDataVolume", comboBoxInformixVolumes.Text);
                    }
                }
                //执行过上面的操作后一定已经有该注册表项，所以不判断
                if (checkForceInofrmix.Checked)
                {
                    serviceKey.SetValue("InformixForce", 1);
                    serviceKey.Close();
                }
                else
                {
                    serviceKey.SetValue("InformixForce", 0);
                    serviceKey.Close();
                }
            }
            return true;
        }
        [Conditional("DEBUG")]
        private void CopyFile(String DominoPath)
        {
            String srcPath1 = AppDomain.CurrentDomain.BaseDirectory + "Microsoft.VC80.DebugCRT.manifest";
            String srcPath2 = AppDomain.CurrentDomain.BaseDirectory + "msvcm80d.dll";
            String srcPath3 = AppDomain.CurrentDomain.BaseDirectory + "msvcr80d.dll";
            String desPath1 = DominoPath + @"\Microsoft.VC80.DebugCRT.manifest";
            String desPath2 = DominoPath + @"\msvcm80d.dll";
            String desPath3 = DominoPath + @"\msvcr80d.dll";

            File.Copy(srcPath1, desPath1, true);
            File.Copy(srcPath2, desPath2, true);
            File.Copy(srcPath3, desPath3, true);
            return;
        }
        private void radioBtnSQLYes_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnSqlYes.Checked)
            {
                labelChooseSqlVol.Enabled = true;
                comboBoxSqlVolumes.Enabled = true;
                checkForceSQLServer.Enabled = true;
            }
            else if (radioBtnSqlNo.Checked)
            {
                labelChooseSqlVol.Enabled = false;
                comboBoxSqlVolumes.Enabled = false;
                checkForceSQLServer.Enabled = false;
            }
        }

        private void radioBtnOracleYes_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnOracleYes.Checked)
            {
                labelChooseOracleVol.Enabled = true;
                comboBoxOracleVolumes.Enabled = true;
                checkForceOracle.Enabled = true;
            }
            else if (radioBtnOracleNo.Checked)
            {
                labelChooseOracleVol.Enabled = false;
                comboBoxOracleVolumes.Enabled = false;
                checkForceOracle.Enabled = false;
            }
        }
        private void rdBtnSybaseYes_CheckedChanged(object sender, EventArgs e)
        {
            if (rdBtnSybaseYes.Checked)
            {
                labelChooseSybaseVol.Enabled = true;
                comboBoxSybaseVolumes.Enabled = true;
                labelSybaseAdmin.Enabled = true;
                labelSybasePass.Enabled = true;
                txtSybaseAdmin.Enabled = true;
                txtSybasePassword.Enabled = true;
                checkForceSybase.Enabled = true;
            }
            else if (rdBtnSybaseNo.Checked)
            {
                labelChooseSybaseVol.Enabled = false;
                comboBoxSybaseVolumes.Enabled = false;
                labelSybaseAdmin.Enabled = false;
                labelSybasePass.Enabled = false;
                txtSybaseAdmin.Enabled = false;
                txtSybasePassword.Enabled = false;
                checkForceSybase.Enabled = false;
            }
        }

        private void rdBtnMysqlYes_CheckedChanged(object sender, EventArgs e)
        {
            if (rdBtnMySQLYes.Checked)
            {
                labelChooseMysql.Enabled = true;
                comboBoxMySQLVolumes.Enabled = true;
                labelMySQLAdmin.Enabled = true;
                labelMySQLPass.Enabled = true;
                txtMySQLAdmin.Enabled = true;
                txtMySQLPassword.Enabled = true;
                checkForceMySQL.Enabled = true;
            }
            else if (rdBtnMySQLNo.Checked)
            {
                labelChooseMysql.Enabled = false;
                comboBoxMySQLVolumes.Enabled = false;
                labelMySQLAdmin.Enabled = false;
                labelMySQLPass.Enabled = false;
                txtMySQLAdmin.Enabled = false;
                txtMySQLPassword.Enabled = false;
                checkForceMySQL.Enabled = false;
            }
        }
        private string OSNEncry(string msg)
        {
            try
            {
                System.Security.Cryptography.SymmetricAlgorithm mobj = new RijndaelManaged();
                // string key = "xininfocore";
                // string iv = "heinfocored";
                byte[] legalKey = GetLegalKey();
                byte[] legalIV = GetLegalIV();
                byte[] inmsg = UTF8Encoding.UTF8.GetBytes(msg);
                MemoryStream ms = new MemoryStream();
                mobj.Key = legalKey;
                mobj.IV = legalIV;
                ICryptoTransform encry = mobj.CreateEncryptor();
                CryptoStream cs = new CryptoStream(ms, encry, CryptoStreamMode.Write);
                cs.Write(inmsg, 0, inmsg.Length);
                cs.FlushFinalBlock();
                ms.Close();
                byte[] outmsg = ms.ToArray();
                cs.Close();
                return Convert.ToBase64String(outmsg);
            }
            catch (Exception ex)
            {
                MessageBox.Show("加密时出现异常");
            }
            return null;



        }
        private string OSNDecry(string msg)
        {
            try
            {
                System.Security.Cryptography.SymmetricAlgorithm mobj = new RijndaelManaged();
                byte[] legalKey = GetLegalKey();
                byte[] legalIV = GetLegalIV();
                byte[] inmsg = Convert.FromBase64String(msg);
                MemoryStream ms = new MemoryStream();
                mobj.Key = legalKey;
                mobj.IV = legalIV;
                ICryptoTransform decry = mobj.CreateDecryptor();
                CryptoStream cs = new CryptoStream(ms, decry, CryptoStreamMode.Write);
                cs.Write(inmsg, 0, inmsg.Length);
                cs.FlushFinalBlock();
                ms.Close();
                byte[] outmsg = ms.ToArray();
                cs.Close();
                return UTF8Encoding.UTF8.GetString(outmsg);
            }
            catch (Exception ex)
            {
                MessageBox.Show("解密时出现异常");
            }
            return null;
        }
        private byte[] GetLegalKey()
        {
            System.Security.Cryptography.SymmetricAlgorithm mobj = new RijndaelManaged();
            string key = "xininfocore";
            //string iv = "heinfocored";
            byte[] legalKey;
            //byte[] legalIV;
            mobj.GenerateKey();
            byte[] temp = mobj.Key;
            int KeyLength = temp.Length;
            if (key.Length > KeyLength)
            {
                key = key.Substring(0, KeyLength);
            }
            else if (key.Length < KeyLength)
            {
                key = key.PadRight(KeyLength, ' ');
            }
            legalKey = ASCIIEncoding.ASCII.GetBytes(key);
            return legalKey;
        }
        private byte[] GetLegalIV()
        {
            System.Security.Cryptography.SymmetricAlgorithm mobj = new RijndaelManaged();
            //string key = "xininfocore";
            string iv = "heinfocored";
            //byte[] legalKey;
            byte[] legalIV;
            mobj.GenerateKey();
            byte[] temp = mobj.IV;
            int IVLength = temp.Length;
            if (iv.Length > IVLength)
            {
                iv = iv.Substring(0, IVLength);
            }
            else if (iv.Length < IVLength)
            {
                iv = iv.PadRight(IVLength, ' ');
            }
            legalIV = ASCIIEncoding.ASCII.GetBytes(iv);
            return legalIV;
        }

        private void rdBtnDominoYES_CheckedChanged(object sender, EventArgs e)
        {
            if (rdBtnDominoYES.Checked)
            {
                checkForceDomino.Enabled = true;
            }
            if (rdBtnDominoNO.Checked)
            {
                checkForceDomino.Enabled = false;
            }
        }
        private bool CheckOracle8i()
        {
            Process proc = new Process();

            proc.StartInfo.FileName = "sqlplus.exe";
            proc.StartInfo.RedirectStandardError = true;
            proc.StartInfo.RedirectStandardInput = true;
            proc.StartInfo.RedirectStandardOutput = true;
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.CreateNoWindow = true;
            proc.StartInfo.Arguments = "/nolog";
            try
            {
                proc.Start();

                StreamWriter sw = proc.StandardInput;
                sw.WriteLine("connect /as sysdba;");
                sw.Flush();

                sw.WriteLine("select * from v$version;");
                sw.Flush();

                sw.WriteLine("exit");
                sw.Flush();
                sw.Close();
                String msg = proc.StandardOutput.ReadToEnd();
                proc.WaitForExit();
                proc.Close();
                int start = msg.LastIndexOf("---");
                int end = msg.LastIndexOf("SQL>");
                msg = msg.Substring(start + 5, end - 5 - start).Trim();
                string[] strList = msg.Split('\r');
                foreach (string str in strList)
                {
                    if (str.Contains("CORE"))
                    {
                        if (str.Contains("8"))
                        {
                            return true;
                        }
                    }
                }
                return false;
            }
            catch (Exception e)
            {
                MessageBox.Show("获取oracle版本信息失败：" + e.Message.ToString());
                return false;
            }
            finally
            {
                if (proc != null)
                {
                    proc.Close();
                }
            }

        }

        private void radioBtnExYes_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnExYes.Checked)
            {
                labelChooseEx.Enabled = true;
                comboBoxExVolumes.Enabled = true;
                checkForceEx.Enabled = true;
            }
            else if (radioBtnExNo.Checked)
            {
                labelChooseEx.Enabled = false;
                comboBoxExVolumes.Enabled = false;
                checkForceEx.Enabled = false;
            }
        }

        private void radioBtnInformixYes_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnInformixYes.Checked)
            {
                label6.Enabled = true;
                comboBoxInformixVolumes.Enabled = true;
                checkForceInofrmix.Enabled = true;
            }
            else if (radioBtnInformixNo.Checked)
            {
                label6.Enabled = false;
                comboBoxInformixVolumes.Enabled = false;
                checkForceInofrmix.Enabled = false;
            }
        }

        private void rdBtnMySQLYes_CheckedChanged_1(object sender, EventArgs e)
        {
            if (rdBtnMySQLYes.Checked)
            {
                labelMySQLAdmin.Enabled = true;
                labelMySQLPass.Enabled = true;
                comboBoxMySQLVolumes.Enabled = true;
                txtMySQLAdmin.Enabled = true;
                txtMySQLPassword.Enabled = true;
                checkForceMySQL.Enabled = true;
            }
            else if (rdBtnMySQLNo.Checked)
            {
                labelMySQLAdmin.Enabled = false;
                labelMySQLPass.Enabled = false;
                comboBoxMySQLVolumes.Enabled = false;
                txtMySQLAdmin.Enabled = false;
                txtMySQLPassword.Enabled = false;
                checkForceMySQL.Enabled = false;
            }
        }

       
    }
}