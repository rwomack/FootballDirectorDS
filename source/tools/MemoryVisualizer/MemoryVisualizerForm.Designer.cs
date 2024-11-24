namespace MemoryVisualizer
{
    partial class MemoryVisualizerForm
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.compareAgainstToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.compareAgainstbytesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.listViewFileSelection = new System.Windows.Forms.ListView();
            this.dumpView1 = new MemoryVisualizer.DumpView();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(503, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.compareAgainstToolStripMenuItem,
            this.compareAgainstbytesToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(266, 22);
            this.loadToolStripMenuItem.Text = "&Open baseline...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // compareAgainstToolStripMenuItem
            // 
            this.compareAgainstToolStripMenuItem.Name = "compareAgainstToolStripMenuItem";
            this.compareAgainstToolStripMenuItem.Size = new System.Drawing.Size(266, 22);
            this.compareAgainstToolStripMenuItem.Text = "&Compare against (unmatched/new)...";
            this.compareAgainstToolStripMenuItem.Click += new System.EventHandler(this.compareAgainstToolStripMenuItem_Click);
            // 
            // compareAgainstbytesToolStripMenuItem
            // 
            this.compareAgainstbytesToolStripMenuItem.Name = "compareAgainstbytesToolStripMenuItem";
            this.compareAgainstbytesToolStripMenuItem.Size = new System.Drawing.Size(266, 22);
            this.compareAgainstbytesToolStripMenuItem.Text = "Compare against (&bytes)...";
            this.compareAgainstbytesToolStripMenuItem.Click += new System.EventHandler(this.compareAgainstbytesToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(263, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(266, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.listViewFileSelection);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.dumpView1);
            this.splitContainer1.Size = new System.Drawing.Size(503, 384);
            this.splitContainer1.SplitterDistance = 167;
            this.splitContainer1.TabIndex = 2;
            // 
            // listViewFileSelection
            // 
            this.listViewFileSelection.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewFileSelection.Location = new System.Drawing.Point(0, 0);
            this.listViewFileSelection.Name = "listViewFileSelection";
            this.listViewFileSelection.Size = new System.Drawing.Size(167, 384);
            this.listViewFileSelection.TabIndex = 0;
            this.listViewFileSelection.UseCompatibleStateImageBehavior = false;
            this.listViewFileSelection.View = System.Windows.Forms.View.List;
            this.listViewFileSelection.SelectedIndexChanged += new System.EventHandler(this.listViewFileSelection_SelectedIndexChanged);
            // 
            // dumpView1
            // 
            this.dumpView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dumpView1.Location = new System.Drawing.Point(0, 0);
            this.dumpView1.Name = "dumpView1";
            this.dumpView1.Size = new System.Drawing.Size(332, 384);
            this.dumpView1.TabIndex = 0;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "XML files|*.xml";
            // 
            // MemoryVisualizerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(503, 408);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MemoryVisualizerForm";
            this.Text = "Memory Visualizer";
            this.Load += new System.EventHandler(this.MemoryVisualizer_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem compareAgainstToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView listViewFileSelection;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private DumpView dumpView1;
        private System.Windows.Forms.ToolStripMenuItem compareAgainstbytesToolStripMenuItem;
    }
}

