namespace ImageConverter
{
    partial class frmImageConverter
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
            this.txtHContent = new System.Windows.Forms.TextBox();
            this.cmdBrowseDir = new System.Windows.Forms.Button();
            this.pnlTools = new System.Windows.Forms.Panel();
            this.cmdStart = new System.Windows.Forms.Button();
            this.pnlColor = new System.Windows.Forms.Panel();
            this.lbl16bit = new System.Windows.Forms.Label();
            this.lbl32bit = new System.Windows.Forms.Label();
            this.txt16bitColor = new System.Windows.Forms.TextBox();
            this.txt32bitColor = new System.Windows.Forms.TextBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.txtFolder = new System.Windows.Forms.TextBox();
            this.cmdColorDialog = new System.Windows.Forms.Button();
            this.txtCPPContent = new System.Windows.Forms.TextBox();
            this.colorPicker = new System.Windows.Forms.ColorDialog();
            this.folderDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.chkSystemImages = new System.Windows.Forms.CheckBox();
            this.pnlTools.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtHContent
            // 
            this.txtHContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtHContent.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.txtHContent.Location = new System.Drawing.Point(0, 0);
            this.txtHContent.Multiline = true;
            this.txtHContent.Name = "txtHContent";
            this.txtHContent.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtHContent.Size = new System.Drawing.Size(638, 355);
            this.txtHContent.TabIndex = 1;
            this.txtHContent.WordWrap = false;
            // 
            // cmdBrowseDir
            // 
            this.cmdBrowseDir.Location = new System.Drawing.Point(446, 9);
            this.cmdBrowseDir.Name = "cmdBrowseDir";
            this.cmdBrowseDir.Size = new System.Drawing.Size(26, 23);
            this.cmdBrowseDir.TabIndex = 6;
            this.cmdBrowseDir.Text = "...";
            this.cmdBrowseDir.UseVisualStyleBackColor = true;
            this.cmdBrowseDir.Click += new System.EventHandler(this.cmdBrowseDir_Click);
            // 
            // pnlTools
            // 
            this.pnlTools.Controls.Add(this.chkSystemImages);
            this.pnlTools.Controls.Add(this.label2);
            this.pnlTools.Controls.Add(this.label1);
            this.pnlTools.Controls.Add(this.cmdStart);
            this.pnlTools.Controls.Add(this.pnlColor);
            this.pnlTools.Controls.Add(this.lbl16bit);
            this.pnlTools.Controls.Add(this.lbl32bit);
            this.pnlTools.Controls.Add(this.txt16bitColor);
            this.pnlTools.Controls.Add(this.txt32bitColor);
            this.pnlTools.Controls.Add(this.progressBar1);
            this.pnlTools.Controls.Add(this.txtFolder);
            this.pnlTools.Controls.Add(this.cmdColorDialog);
            this.pnlTools.Controls.Add(this.cmdBrowseDir);
            this.pnlTools.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTools.Location = new System.Drawing.Point(0, 0);
            this.pnlTools.Name = "pnlTools";
            this.pnlTools.Size = new System.Drawing.Size(1369, 100);
            this.pnlTools.TabIndex = 7;
            // 
            // cmdStart
            // 
            this.cmdStart.Location = new System.Drawing.Point(446, 38);
            this.cmdStart.Name = "cmdStart";
            this.cmdStart.Size = new System.Drawing.Size(75, 23);
            this.cmdStart.TabIndex = 13;
            this.cmdStart.Text = "Start!";
            this.cmdStart.UseVisualStyleBackColor = true;
            this.cmdStart.Click += new System.EventHandler(this.cmdStart_Click);
            // 
            // pnlColor
            // 
            this.pnlColor.Location = new System.Drawing.Point(630, 65);
            this.pnlColor.Name = "pnlColor";
            this.pnlColor.Size = new System.Drawing.Size(100, 26);
            this.pnlColor.TabIndex = 12;
            // 
            // lbl16bit
            // 
            this.lbl16bit.AutoSize = true;
            this.lbl16bit.Location = new System.Drawing.Point(799, 22);
            this.lbl16bit.Name = "lbl16bit";
            this.lbl16bit.Size = new System.Drawing.Size(59, 13);
            this.lbl16bit.TabIndex = 11;
            this.lbl16bit.Text = "16 bit color";
            // 
            // lbl32bit
            // 
            this.lbl32bit.AutoSize = true;
            this.lbl32bit.Location = new System.Drawing.Point(644, 22);
            this.lbl32bit.Name = "lbl32bit";
            this.lbl32bit.Size = new System.Drawing.Size(59, 13);
            this.lbl32bit.TabIndex = 10;
            this.lbl32bit.Text = "32 bit color";
            // 
            // txt16bitColor
            // 
            this.txt16bitColor.Location = new System.Drawing.Point(778, 38);
            this.txt16bitColor.Name = "txt16bitColor";
            this.txt16bitColor.ReadOnly = true;
            this.txt16bitColor.Size = new System.Drawing.Size(100, 20);
            this.txt16bitColor.TabIndex = 9;
            // 
            // txt32bitColor
            // 
            this.txt32bitColor.Location = new System.Drawing.Point(630, 38);
            this.txt32bitColor.Name = "txt32bitColor";
            this.txt32bitColor.Size = new System.Drawing.Size(100, 20);
            this.txt32bitColor.TabIndex = 9;
            this.txt32bitColor.TextChanged += new System.EventHandler(this.txt32bitColor_TextChanged);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(4, 38);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(436, 23);
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar1.TabIndex = 8;
            // 
            // txtFolder
            // 
            this.txtFolder.Location = new System.Drawing.Point(4, 12);
            this.txtFolder.Name = "txtFolder";
            this.txtFolder.Size = new System.Drawing.Size(436, 20);
            this.txtFolder.TabIndex = 7;
            this.txtFolder.Text = "d:\\Projects\\KreylSVN\\OpenSpace\\Utils\\ImageConverter\\Icons\\ToConvert\\test\\";
            // 
            // cmdColorDialog
            // 
            this.cmdColorDialog.Location = new System.Drawing.Point(736, 35);
            this.cmdColorDialog.Name = "cmdColorDialog";
            this.cmdColorDialog.Size = new System.Drawing.Size(26, 23);
            this.cmdColorDialog.TabIndex = 6;
            this.cmdColorDialog.Text = "...";
            this.cmdColorDialog.UseVisualStyleBackColor = true;
            this.cmdColorDialog.Click += new System.EventHandler(this.cmdColorDialog_Click);
            // 
            // txtCPPContent
            // 
            this.txtCPPContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtCPPContent.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.txtCPPContent.Location = new System.Drawing.Point(0, 0);
            this.txtCPPContent.Multiline = true;
            this.txtCPPContent.Name = "txtCPPContent";
            this.txtCPPContent.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtCPPContent.Size = new System.Drawing.Size(727, 355);
            this.txtCPPContent.TabIndex = 1;
            this.txtCPPContent.WordWrap = false;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 100);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.txtHContent);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.txtCPPContent);
            this.splitContainer1.Size = new System.Drawing.Size(1369, 355);
            this.splitContainer1.SplitterDistance = 638;
            this.splitContainer1.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(233, 77);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 16);
            this.label1.TabIndex = 14;
            this.label1.Text = "images.h";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label2.Location = new System.Drawing.Point(966, 77);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 16);
            this.label2.TabIndex = 14;
            this.label2.Text = "images.cpp";
            // 
            // chkSystemImages
            // 
            this.chkSystemImages.AutoSize = true;
            this.chkSystemImages.Checked = true;
            this.chkSystemImages.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSystemImages.Location = new System.Drawing.Point(12, 74);
            this.chkSystemImages.Name = "chkSystemImages";
            this.chkSystemImages.Size = new System.Drawing.Size(94, 17);
            this.chkSystemImages.TabIndex = 15;
            this.chkSystemImages.Text = "SystemImages";
            this.chkSystemImages.UseVisualStyleBackColor = true;
            // 
            // frmImageConverter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1369, 455);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.pnlTools);
            this.Name = "frmImageConverter";
            this.Text = "ImageConverter";
            this.Load += new System.EventHandler(this.frmImageConverter_Load);
            this.pnlTools.ResumeLayout(false);
            this.pnlTools.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox txtHContent;
        private System.Windows.Forms.Button cmdBrowseDir;
        private System.Windows.Forms.Panel pnlTools;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.TextBox txtFolder;
        private System.Windows.Forms.TextBox txt32bitColor;
        private System.Windows.Forms.TextBox txtCPPContent;
        private System.Windows.Forms.Label lbl16bit;
        private System.Windows.Forms.Label lbl32bit;
        private System.Windows.Forms.TextBox txt16bitColor;
        private System.Windows.Forms.Button cmdColorDialog;
        private System.Windows.Forms.ColorDialog colorPicker;
        private System.Windows.Forms.Panel pnlColor;
        private System.Windows.Forms.Button cmdStart;
        private System.Windows.Forms.FolderBrowserDialog folderDialog;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox chkSystemImages;
    }
}

