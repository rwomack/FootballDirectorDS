namespace Data_Builder
{
	partial class FormMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.StatusLabel = new System.Windows.Forms.Label();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Java_Button = new System.Windows.Forms.RadioButton();
            this.C_Button = new System.Windows.Forms.RadioButton();
            this.button3 = new System.Windows.Forms.Button();
            this.Series60_Button = new System.Windows.Forms.RadioButton();
            this.PocketPC_Button = new System.Windows.Forms.RadioButton();
            this.button4 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 16F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.label1.Location = new System.Drawing.Point(298, 191);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(196, 26);
            this.label1.TabIndex = 0;
            this.label1.Text = "Data Builder v1.2";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // button1
            // 
            this.button1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.button1.Location = new System.Drawing.Point(488, 508);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(145, 26);
            this.button1.TabIndex = 1;
            this.button1.Text = "Generate PocketPC Data";
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button2.FlatAppearance.BorderSize = 2;
            this.button2.Location = new System.Drawing.Point(720, 507);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(55, 27);
            this.button2.TabIndex = 2;
            this.button2.Text = "Exit";
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(-1, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(789, 167);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // StatusLabel
            // 
            this.StatusLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StatusLabel.Location = new System.Drawing.Point(12, 221);
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(761, 206);
            this.StatusLabel.TabIndex = 4;
            this.StatusLabel.Text = "label2";
            this.StatusLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.pictureBox2.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox2.Image")));
            this.pictureBox2.Location = new System.Drawing.Point(12, 406);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(114, 128);
            this.pictureBox2.TabIndex = 5;
            this.pictureBox2.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Java_Button);
            this.groupBox1.Controls.Add(this.C_Button);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(180, 449);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(126, 75);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "String Generation";
            // 
            // Java_Button
            // 
            this.Java_Button.AccessibleName = "J_button";
            this.Java_Button.AutoSize = true;
            this.Java_Button.Checked = true;
            this.Java_Button.Location = new System.Drawing.Point(16, 46);
            this.Java_Button.Name = "Java_Button";
            this.Java_Button.Size = new System.Drawing.Size(52, 17);
            this.Java_Button.TabIndex = 1;
            this.Java_Button.TabStop = true;
            this.Java_Button.Text = "Java";
            this.Java_Button.CheckedChanged += new System.EventHandler(this.Java_Button_CheckedChanged);
            // 
            // C_Button
            // 
            this.C_Button.AutoSize = true;
            this.C_Button.Location = new System.Drawing.Point(16, 23);
            this.C_Button.Name = "C_Button";
            this.C_Button.Size = new System.Drawing.Size(47, 17);
            this.C_Button.TabIndex = 0;
            this.C_Button.Text = "C++";
            this.C_Button.CheckedChanged += new System.EventHandler(this.C_Button_CheckedChanged);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.Location = new System.Drawing.Point(213, 546);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(134, 26);
            this.button3.TabIndex = 2;
            this.button3.Text = "Generate Strings";
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Series60_Button
            // 
            this.Series60_Button.AutoSize = true;
            this.Series60_Button.Checked = true;
            this.Series60_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Series60_Button.Location = new System.Drawing.Point(312, 495);
            this.Series60_Button.Name = "Series60_Button";
            this.Series60_Button.Size = new System.Drawing.Size(74, 17);
            this.Series60_Button.TabIndex = 4;
            this.Series60_Button.TabStop = true;
            this.Series60_Button.Text = "Series60";
            this.Series60_Button.CheckedChanged += new System.EventHandler(this.Series60_Button_CheckedChanged);
            // 
            // PocketPC_Button
            // 
            this.PocketPC_Button.AccessibleDescription = "Generate PocketPC data";
            this.PocketPC_Button.AutoSize = true;
            this.PocketPC_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PocketPC_Button.Location = new System.Drawing.Point(312, 472);
            this.PocketPC_Button.Name = "PocketPC_Button";
            this.PocketPC_Button.Size = new System.Drawing.Size(81, 17);
            this.PocketPC_Button.TabIndex = 3;
            this.PocketPC_Button.Text = "PocketPC";
            this.PocketPC_Button.CheckedChanged += new System.EventHandler(this.PocketPC_Button_CheckedChanged);
            // 
            // button4
            // 
            this.button4.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.button4.Location = new System.Drawing.Point(488, 452);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(145, 26);
            this.button4.TabIndex = 7;
            this.button4.Text = "Generate Series60 Data";
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(787, 546);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.Series60_Button);
            this.Controls.Add(this.PocketPC_Button);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.StatusLabel);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Name = "FormMain";
            this.Text = "Football Director ";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.PictureBox pictureBox2;
		public System.Windows.Forms.Label StatusLabel;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton Java_Button;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.RadioButton Series60_Button;
		private System.Windows.Forms.RadioButton PocketPC_Button;
		public System.Windows.Forms.RadioButton C_Button;
        private System.Windows.Forms.Button button4;
	}
}

