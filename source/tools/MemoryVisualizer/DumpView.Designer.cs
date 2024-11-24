namespace MemoryVisualizer
{
    partial class DumpView
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.listViewHeaps = new System.Windows.Forms.ListView();
            this.heapView1 = new MemoryVisualizer.HeapView();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.listViewHeaps);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.heapView1);
            this.splitContainer1.Size = new System.Drawing.Size(553, 397);
            this.splitContainer1.SplitterDistance = 184;
            this.splitContainer1.TabIndex = 0;
            // 
            // listViewHeaps
            // 
            this.listViewHeaps.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewHeaps.Location = new System.Drawing.Point(0, 0);
            this.listViewHeaps.Name = "listViewHeaps";
            this.listViewHeaps.Size = new System.Drawing.Size(184, 397);
            this.listViewHeaps.TabIndex = 0;
            this.listViewHeaps.UseCompatibleStateImageBehavior = false;
            this.listViewHeaps.View = System.Windows.Forms.View.List;
            // 
            // heapView1
            // 
            this.heapView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.heapView1.Location = new System.Drawing.Point(0, 0);
            this.heapView1.Name = "heapView1";
            this.heapView1.Size = new System.Drawing.Size(365, 397);
            this.heapView1.TabIndex = 0;
            // 
            // DumpView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "DumpView";
            this.Size = new System.Drawing.Size(553, 397);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView listViewHeaps;
        private HeapView heapView1;
    }
}
