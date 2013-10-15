namespace HostMirrorCDPSchedule
{
    partial class CDPScheduleForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rbModeMonthly = new System.Windows.Forms.RadioButton();
            this.rbModeWeekly = new System.Windows.Forms.RadioButton();
            this.rbModeDayly = new System.Windows.Forms.RadioButton();
            this.rbModeMinutes = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.startDatePicker = new System.Windows.Forms.DateTimePicker();
            this.startTimePicker = new System.Windows.Forms.DateTimePicker();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cbDayOfWeek = new System.Windows.Forms.ComboBox();
            this.cbWeekBase = new System.Windows.Forms.ComboBox();
            this.nudModifier = new System.Windows.Forms.NumericUpDown();
            this.rbChoose2 = new System.Windows.Forms.RadioButton();
            this.rbChoose1 = new System.Windows.Forms.RadioButton();
            this.checkBox12 = new System.Windows.Forms.CheckBox();
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.checkBox10 = new System.Windows.Forms.CheckBox();
            this.checkBox11 = new System.Windows.Forms.CheckBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.checkBox9 = new System.Windows.Forms.CheckBox();
            this.checkBox8 = new System.Windows.Forms.CheckBox();
            this.checkBox7 = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.checkBox6 = new System.Windows.Forms.CheckBox();
            this.lbUnit = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnBack = new System.Windows.Forms.Button();
            this.btnNext = new System.Windows.Forms.Button();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudModifier)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.groupBox1);
            this.splitContainer1.Panel1Collapsed = true;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.groupBox2);
            this.splitContainer1.Size = new System.Drawing.Size(287, 273);
            this.splitContainer1.SplitterDistance = 126;
            this.splitContainer1.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rbModeMonthly);
            this.groupBox1.Controls.Add(this.rbModeWeekly);
            this.groupBox1.Controls.Add(this.rbModeDayly);
            this.groupBox1.Controls.Add(this.rbModeMinutes);
            this.groupBox1.Location = new System.Drawing.Point(3, 5);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(282, 265);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "运行这个任务:";
            // 
            // rbModeMonthly
            // 
            this.rbModeMonthly.AutoSize = true;
            this.rbModeMonthly.Location = new System.Drawing.Point(10, 87);
            this.rbModeMonthly.Name = "rbModeMonthly";
            this.rbModeMonthly.Size = new System.Drawing.Size(47, 16);
            this.rbModeMonthly.TabIndex = 3;
            this.rbModeMonthly.TabStop = true;
            this.rbModeMonthly.Text = "每月";
            this.rbModeMonthly.UseVisualStyleBackColor = true;
            // 
            // rbModeWeekly
            // 
            this.rbModeWeekly.AutoSize = true;
            this.rbModeWeekly.Location = new System.Drawing.Point(10, 65);
            this.rbModeWeekly.Name = "rbModeWeekly";
            this.rbModeWeekly.Size = new System.Drawing.Size(47, 16);
            this.rbModeWeekly.TabIndex = 2;
            this.rbModeWeekly.TabStop = true;
            this.rbModeWeekly.Text = "每周";
            this.rbModeWeekly.UseVisualStyleBackColor = true;
            // 
            // rbModeDayly
            // 
            this.rbModeDayly.AutoSize = true;
            this.rbModeDayly.Location = new System.Drawing.Point(10, 43);
            this.rbModeDayly.Name = "rbModeDayly";
            this.rbModeDayly.Size = new System.Drawing.Size(53, 16);
            this.rbModeDayly.TabIndex = 1;
            this.rbModeDayly.TabStop = true;
            this.rbModeDayly.Text = "每N天";
            this.rbModeDayly.UseVisualStyleBackColor = true;
            // 
            // rbModeMinutes
            // 
            this.rbModeMinutes.AutoSize = true;
            this.rbModeMinutes.Location = new System.Drawing.Point(10, 21);
            this.rbModeMinutes.Name = "rbModeMinutes";
            this.rbModeMinutes.Size = new System.Drawing.Size(65, 16);
            this.rbModeMinutes.TabIndex = 0;
            this.rbModeMinutes.TabStop = true;
            this.rbModeMinutes.Text = "每N分钟";
            this.rbModeMinutes.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.startDatePicker);
            this.groupBox2.Controls.Add(this.startTimePicker);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.cbDayOfWeek);
            this.groupBox2.Controls.Add(this.cbWeekBase);
            this.groupBox2.Controls.Add(this.nudModifier);
            this.groupBox2.Controls.Add(this.rbChoose2);
            this.groupBox2.Controls.Add(this.rbChoose1);
            this.groupBox2.Controls.Add(this.checkBox12);
            this.groupBox2.Controls.Add(this.checkBox4);
            this.groupBox2.Controls.Add(this.checkBox10);
            this.groupBox2.Controls.Add(this.checkBox11);
            this.groupBox2.Controls.Add(this.checkBox3);
            this.groupBox2.Controls.Add(this.checkBox2);
            this.groupBox2.Controls.Add(this.checkBox1);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.checkBox9);
            this.groupBox2.Controls.Add(this.checkBox8);
            this.groupBox2.Controls.Add(this.checkBox7);
            this.groupBox2.Controls.Add(this.checkBox5);
            this.groupBox2.Controls.Add(this.checkBox6);
            this.groupBox2.Controls.Add(this.lbUnit);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(3, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(282, 265);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设置任务运行的起始时间和日期:";
            // 
            // startDatePicker
            // 
            this.startDatePicker.CustomFormat = "yyyy-MM-dd";
            this.startDatePicker.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.startDatePicker.Location = new System.Drawing.Point(6, 135);
            this.startDatePicker.Name = "startDatePicker";
            this.startDatePicker.Size = new System.Drawing.Size(114, 21);
            this.startDatePicker.TabIndex = 3;
            // 
            // startTimePicker
            // 
            this.startTimePicker.CustomFormat = "HH:mm";
            this.startTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.startTimePicker.Location = new System.Drawing.Point(9, 35);
            this.startTimePicker.Name = "startTimePicker";
            this.startTimePicker.ShowUpDown = true;
            this.startTimePicker.Size = new System.Drawing.Size(111, 21);
            this.startTimePicker.TabIndex = 1;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Enabled = false;
            this.label4.ForeColor = System.Drawing.Color.SlateGray;
            this.label4.Location = new System.Drawing.Point(127, 139);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 31;
            this.label4.Text = "YYYY-MM-DD";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Enabled = false;
            this.label3.ForeColor = System.Drawing.Color.SlateGray;
            this.label3.Location = new System.Drawing.Point(127, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 12);
            this.label3.TabIndex = 30;
            this.label3.Text = "HH:MM";
            // 
            // cbDayOfWeek
            // 
            this.cbDayOfWeek.FormattingEnabled = true;
            this.cbDayOfWeek.Items.AddRange(new object[] {
            "星期日",
            "星期一",
            "星期二",
            "星期三",
            "星期四",
            "星期五",
            "星期六"});
            this.cbDayOfWeek.Location = new System.Drawing.Point(155, 93);
            this.cbDayOfWeek.Name = "cbDayOfWeek";
            this.cbDayOfWeek.Size = new System.Drawing.Size(93, 20);
            this.cbDayOfWeek.TabIndex = 29;
            // 
            // cbWeekBase
            // 
            this.cbWeekBase.FormattingEnabled = true;
            this.cbWeekBase.Items.AddRange(new object[] {
            "第一个",
            "第二个",
            "第三个",
            "第四个",
            "最后一个"});
            this.cbWeekBase.Location = new System.Drawing.Point(62, 93);
            this.cbWeekBase.Name = "cbWeekBase";
            this.cbWeekBase.Size = new System.Drawing.Size(87, 20);
            this.cbWeekBase.TabIndex = 28;
            // 
            // nudModifier
            // 
            this.nudModifier.BackColor = System.Drawing.SystemColors.Window;
            this.nudModifier.Location = new System.Drawing.Point(62, 66);
            this.nudModifier.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudModifier.Name = "nudModifier";
            this.nudModifier.Size = new System.Drawing.Size(58, 21);
            this.nudModifier.TabIndex = 26;
            this.nudModifier.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.nudModifier.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // rbChoose2
            // 
            this.rbChoose2.AutoSize = true;
            this.rbChoose2.Location = new System.Drawing.Point(9, 95);
            this.rbChoose2.Name = "rbChoose2";
            this.rbChoose2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.rbChoose2.Size = new System.Drawing.Size(53, 16);
            this.rbChoose2.TabIndex = 25;
            this.rbChoose2.Text = "星期:";
            this.rbChoose2.UseVisualStyleBackColor = true;
            // 
            // rbChoose1
            // 
            this.rbChoose1.AutoSize = true;
            this.rbChoose1.Checked = true;
            this.rbChoose1.Location = new System.Drawing.Point(9, 68);
            this.rbChoose1.Name = "rbChoose1";
            this.rbChoose1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.rbChoose1.Size = new System.Drawing.Size(35, 16);
            this.rbChoose1.TabIndex = 24;
            this.rbChoose1.TabStop = true;
            this.rbChoose1.Text = "每";
            this.rbChoose1.UseVisualStyleBackColor = true;
            // 
            // checkBox12
            // 
            this.checkBox12.AutoSize = true;
            this.checkBox12.Location = new System.Drawing.Point(183, 242);
            this.checkBox12.Name = "checkBox12";
            this.checkBox12.Size = new System.Drawing.Size(84, 16);
            this.checkBox12.TabIndex = 21;
            this.checkBox12.Text = "checkBox12";
            this.checkBox12.UseVisualStyleBackColor = true;
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Location = new System.Drawing.Point(9, 242);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(78, 16);
            this.checkBox4.TabIndex = 17;
            this.checkBox4.Text = "checkBox4";
            this.checkBox4.UseVisualStyleBackColor = true;
            // 
            // checkBox10
            // 
            this.checkBox10.AutoSize = true;
            this.checkBox10.Location = new System.Drawing.Point(183, 200);
            this.checkBox10.Name = "checkBox10";
            this.checkBox10.Size = new System.Drawing.Size(84, 16);
            this.checkBox10.TabIndex = 23;
            this.checkBox10.Text = "checkBox10";
            this.checkBox10.UseVisualStyleBackColor = true;
            // 
            // checkBox11
            // 
            this.checkBox11.AutoSize = true;
            this.checkBox11.Location = new System.Drawing.Point(183, 221);
            this.checkBox11.Name = "checkBox11";
            this.checkBox11.Size = new System.Drawing.Size(84, 16);
            this.checkBox11.TabIndex = 22;
            this.checkBox11.Text = "checkBox11";
            this.checkBox11.UseVisualStyleBackColor = true;
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(9, 221);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(78, 16);
            this.checkBox3.TabIndex = 14;
            this.checkBox3.Text = "checkBox3";
            this.checkBox3.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(9, 200);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(78, 16);
            this.checkBox2.TabIndex = 13;
            this.checkBox2.Text = "checkBox2";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(9, 179);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(78, 16);
            this.checkBox1.TabIndex = 12;
            this.checkBox1.Text = "checkBox1";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 163);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 12);
            this.label5.TabIndex = 11;
            this.label5.Text = "选择:";
            // 
            // checkBox9
            // 
            this.checkBox9.AutoSize = true;
            this.checkBox9.Location = new System.Drawing.Point(183, 179);
            this.checkBox9.Name = "checkBox9";
            this.checkBox9.Size = new System.Drawing.Size(78, 16);
            this.checkBox9.TabIndex = 18;
            this.checkBox9.Text = "checkBox9";
            this.checkBox9.UseVisualStyleBackColor = true;
            // 
            // checkBox8
            // 
            this.checkBox8.AutoSize = true;
            this.checkBox8.Location = new System.Drawing.Point(96, 242);
            this.checkBox8.Name = "checkBox8";
            this.checkBox8.Size = new System.Drawing.Size(78, 16);
            this.checkBox8.TabIndex = 19;
            this.checkBox8.Text = "checkBox8";
            this.checkBox8.UseVisualStyleBackColor = true;
            // 
            // checkBox7
            // 
            this.checkBox7.AutoSize = true;
            this.checkBox7.Location = new System.Drawing.Point(96, 221);
            this.checkBox7.Name = "checkBox7";
            this.checkBox7.Size = new System.Drawing.Size(78, 16);
            this.checkBox7.TabIndex = 20;
            this.checkBox7.Text = "checkBox7";
            this.checkBox7.UseVisualStyleBackColor = true;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(96, 179);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(78, 16);
            this.checkBox5.TabIndex = 16;
            this.checkBox5.Text = "checkBox5";
            this.checkBox5.UseVisualStyleBackColor = true;
            // 
            // checkBox6
            // 
            this.checkBox6.AutoSize = true;
            this.checkBox6.Location = new System.Drawing.Point(96, 200);
            this.checkBox6.Name = "checkBox6";
            this.checkBox6.Size = new System.Drawing.Size(78, 16);
            this.checkBox6.TabIndex = 15;
            this.checkBox6.Text = "checkBox6";
            this.checkBox6.UseVisualStyleBackColor = true;
            // 
            // lbUnit
            // 
            this.lbUnit.AutoSize = true;
            this.lbUnit.Location = new System.Drawing.Point(127, 70);
            this.lbUnit.Name = "lbUnit";
            this.lbUnit.Size = new System.Drawing.Size(41, 12);
            this.lbUnit.TabIndex = 6;
            this.lbUnit.Text = "lbUnit";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 120);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "起始日期:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "起始时间:";
            // 
            // btnBack
            // 
            this.btnBack.Location = new System.Drawing.Point(176, 276);
            this.btnBack.Name = "btnBack";
            this.btnBack.Size = new System.Drawing.Size(52, 23);
            this.btnBack.TabIndex = 16;
            this.btnBack.Text = "上一步";
            this.btnBack.UseVisualStyleBackColor = true;
            this.btnBack.Click += new System.EventHandler(this.btnBack_Click);
            // 
            // btnNext
            // 
            this.btnNext.Location = new System.Drawing.Point(234, 276);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(52, 23);
            this.btnNext.TabIndex = 17;
            this.btnNext.Text = "下一步";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // CDPScheduleForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 299);
            this.Controls.Add(this.btnNext);
            this.Controls.Add(this.btnBack);
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "CDPScheduleForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "CDP计划设置";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudModifier)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rbModeMonthly;
        private System.Windows.Forms.RadioButton rbModeWeekly;
        private System.Windows.Forms.RadioButton rbModeDayly;
        private System.Windows.Forms.RadioButton rbModeMinutes;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox cbDayOfWeek;
        private System.Windows.Forms.ComboBox cbWeekBase;
        private System.Windows.Forms.NumericUpDown nudModifier;
        private System.Windows.Forms.RadioButton rbChoose2;
        private System.Windows.Forms.RadioButton rbChoose1;
        private System.Windows.Forms.CheckBox checkBox12;
        private System.Windows.Forms.CheckBox checkBox4;
        private System.Windows.Forms.CheckBox checkBox10;
        private System.Windows.Forms.CheckBox checkBox11;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox checkBox9;
        private System.Windows.Forms.CheckBox checkBox8;
        private System.Windows.Forms.CheckBox checkBox7;
        private System.Windows.Forms.CheckBox checkBox5;
        private System.Windows.Forms.CheckBox checkBox6;
        private System.Windows.Forms.Label lbUnit;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnBack;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DateTimePicker startTimePicker;
        private System.Windows.Forms.DateTimePicker startDatePicker;
    }
}