using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ApplicationCfg
{
    public partial class Oracle8i : Form
    {
        public Oracle8i()
        {
            InitializeComponent();
        }
        public string adminName;
        public string adminPass;

        private void btnConfir_Click(object sender, EventArgs e)
        {
            if (this.txtName.Text.Equals(string.Empty))
            {
                MessageBox.Show("请输入Oracle8的管理员用户名");
                this.txtName.Focus();
                this.txtPass.Text = "";
                return;
            }
            else if (this.txtPass.Text.Equals(string.Empty))
            {
                MessageBox.Show("请输入Oracle8的管理员密码");
                this.txtPass.Focus();
                return;
            }
            this.adminName = this.txtName.Text.Trim();
            this.adminPass = this.txtPass.Text.Trim();
            this.DialogResult = DialogResult.Yes;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.No;
            this.Close();
            return;
        }
    }
}