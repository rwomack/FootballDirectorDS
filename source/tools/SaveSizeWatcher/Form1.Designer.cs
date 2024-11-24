namespace SaveSizeWatcher
{
    partial class Form1
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
            this.buttonStartWatching = new System.Windows.Forms.Button();
            this.buttonStopWatching = new System.Windows.Forms.Button();
            this.buttonReset = new System.Windows.Forms.Button();
            this.listView1 = new System.Windows.Forms.ListView();
            this.textBoxFileToWatch = new System.Windows.Forms.TextBox();
            this.labelLargest = new System.Windows.Forms.Label();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // buttonStartWatching
            // 
            this.buttonStartWatching.Location = new System.Drawing.Point(12, 12);
            this.buttonStartWatching.Name = "buttonStartWatching";
            this.buttonStartWatching.Size = new System.Drawing.Size(75, 23);
            this.buttonStartWatching.TabIndex = 0;
            this.buttonStartWatching.Text = "Start";
            this.buttonStartWatching.UseVisualStyleBackColor = true;
            this.buttonStartWatching.Click += new System.EventHandler(this.buttonStartWatching_Click);
            // 
            // buttonStopWatching
            // 
            this.buttonStopWatching.Location = new System.Drawing.Point(12, 41);
            this.buttonStopWatching.Name = "buttonStopWatching";
            this.buttonStopWatching.Size = new System.Drawing.Size(75, 23);
            this.buttonStopWatching.TabIndex = 1;
            this.buttonStopWatching.Text = "Stop";
            this.buttonStopWatching.UseVisualStyleBackColor = true;
            this.buttonStopWatching.Click += new System.EventHandler(this.buttonStopWatching_Click);
            // 
            // buttonReset
            // 
            this.buttonReset.Location = new System.Drawing.Point(93, 12);
            this.buttonReset.Name = "buttonReset";
            this.buttonReset.Size = new System.Drawing.Size(75, 23);
            this.buttonReset.TabIndex = 2;
            this.buttonReset.Text = "Reset";
            this.buttonReset.UseVisualStyleBackColor = true;
            this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.listView1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.listView1.Location = new System.Drawing.Point(0, 123);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(621, 374);
            this.listView1.TabIndex = 3;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // textBoxFileToWatch
            // 
            this.textBoxFileToWatch.Location = new System.Drawing.Point(193, 14);
            this.textBoxFileToWatch.Name = "textBoxFileToWatch";
            this.textBoxFileToWatch.Size = new System.Drawing.Size(343, 20);
            this.textBoxFileToWatch.TabIndex = 4;
            this.textBoxFileToWatch.Text = "game.save";
            this.textBoxFileToWatch.TextChanged += new System.EventHandler(this.textBoxFileToWatch_TextChanged);
            // 
            // labelLargest
            // 
            this.labelLargest.AutoSize = true;
            this.labelLargest.Location = new System.Drawing.Point(229, 63);
            this.labelLargest.Name = "labelLargest";
            this.labelLargest.Size = new System.Drawing.Size(48, 13);
            this.labelLargest.TabIndex = 5;
            this.labelLargest.Text = "Largest: ";
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Size";
            this.columnHeader1.Width = 300;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(621, 497);
            this.Controls.Add(this.labelLargest);
            this.Controls.Add(this.textBoxFileToWatch);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.buttonReset);
            this.Controls.Add(this.buttonStopWatching);
            this.Controls.Add(this.buttonStartWatching);
            this.Name = "Form1";
            this.Text = "SaveSizeWatcher";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStartWatching;
        private System.Windows.Forms.Button buttonStopWatching;
        private System.Windows.Forms.Button buttonReset;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.TextBox textBoxFileToWatch;
        private System.Windows.Forms.Label labelLargest;
        private System.Windows.Forms.ColumnHeader columnHeader1;
    }
}

