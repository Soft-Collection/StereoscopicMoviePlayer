﻿namespace StereoscopicMoviePlayer
{
    partial class MainFrm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainFrm));
            this.popUpMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.hideOrShowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.runAtStartupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.alwaysOnTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.trayNotifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.visitWebSiteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gitHubToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.arduinoProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.tsslFrequencyLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tsslFrequency = new System.Windows.Forms.ToolStripStatusLabel();
            this.timerFrequency = new System.Windows.Forms.Timer(this.components);
            this.pbVideoPanel = new System.Windows.Forms.PictureBox();
            this.tbGlassesTimeOffset = new System.Windows.Forms.TrackBar();
            this.lblGlassesTimeOffset = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.timerSendSettings = new System.Windows.Forms.Timer(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.bVertical = new System.Windows.Forms.Button();
            this.tbVolume = new System.Windows.Forms.TrackBar();
            this.bSound = new System.Windows.Forms.Button();
            this.bRefresh = new System.Windows.Forms.Button();
            this.cbComPort = new System.Windows.Forms.ComboBox();
            this.bSwap = new System.Windows.Forms.Button();
            this.bBoth = new System.Windows.Forms.Button();
            this.bRightOnly = new System.Windows.Forms.Button();
            this.bLeftOnly = new System.Windows.Forms.Button();
            this.cbTracks = new System.Windows.Forms.ComboBox();
            this.bOpen = new System.Windows.Forms.Button();
            this.bStop = new System.Windows.Forms.Button();
            this.bPlayPause = new System.Windows.Forms.Button();
            this.tbMovieTime = new System.Windows.Forms.TrackBar();
            this.lblMovieTime = new System.Windows.Forms.Label();
            this.dlgOpen = new System.Windows.Forms.OpenFileDialog();
            this.timerGUIStereoPlayer = new System.Windows.Forms.Timer(this.components);
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.popUpMenu.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMovieTime)).BeginInit();
            this.SuspendLayout();
            // 
            // popUpMenu
            // 
            this.popUpMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator1,
            this.hideOrShowToolStripMenuItem,
            this.runAtStartupToolStripMenuItem,
            this.alwaysOnTopToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.popUpMenu.Name = "popUpMenu";
            this.popUpMenu.Size = new System.Drawing.Size(152, 120);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(148, 6);
            // 
            // hideOrShowToolStripMenuItem
            // 
            this.hideOrShowToolStripMenuItem.Name = "hideOrShowToolStripMenuItem";
            this.hideOrShowToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.hideOrShowToolStripMenuItem.Click += new System.EventHandler(this.hideOrShowToolStripMenuItem_Click);
            // 
            // runAtStartupToolStripMenuItem
            // 
            this.runAtStartupToolStripMenuItem.Name = "runAtStartupToolStripMenuItem";
            this.runAtStartupToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.runAtStartupToolStripMenuItem.Text = "Run at Startup";
            this.runAtStartupToolStripMenuItem.Click += new System.EventHandler(this.runAtStartupToolStripMenuItem_Click);
            // 
            // alwaysOnTopToolStripMenuItem
            // 
            this.alwaysOnTopToolStripMenuItem.Name = "alwaysOnTopToolStripMenuItem";
            this.alwaysOnTopToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.alwaysOnTopToolStripMenuItem.Text = "Always on Top";
            this.alwaysOnTopToolStripMenuItem.Click += new System.EventHandler(this.alwaysOnTopToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // trayNotifyIcon
            // 
            this.trayNotifyIcon.ContextMenuStrip = this.popUpMenu;
            this.trayNotifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("trayNotifyIcon.Icon")));
            this.trayNotifyIcon.Text = "Show/Hide Alpha Stopper";
            this.trayNotifyIcon.Visible = true;
            this.trayNotifyIcon.MouseClick += new System.Windows.Forms.MouseEventHandler(this.trayNotifyIcon_MouseClick);
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem1});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(604, 24);
            this.menuStrip.TabIndex = 26;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem1});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem1
            // 
            this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
            this.exitToolStripMenuItem1.Size = new System.Drawing.Size(92, 22);
            this.exitToolStripMenuItem1.Text = "Exit";
            this.exitToolStripMenuItem1.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem1
            // 
            this.helpToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.visitWebSiteToolStripMenuItem,
            this.gitHubToolStripMenuItem,
            this.arduinoProjectToolStripMenuItem,
            this.aboutToolStripMenuItem1});
            this.helpToolStripMenuItem1.Name = "helpToolStripMenuItem1";
            this.helpToolStripMenuItem1.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem1.Text = "Help";
            // 
            // visitWebSiteToolStripMenuItem
            // 
            this.visitWebSiteToolStripMenuItem.Name = "visitWebSiteToolStripMenuItem";
            this.visitWebSiteToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.visitWebSiteToolStripMenuItem.Text = "Visit Web Site";
            this.visitWebSiteToolStripMenuItem.Click += new System.EventHandler(this.visitWebSiteToolStripMenuItem_Click);
            // 
            // gitHubToolStripMenuItem
            // 
            this.gitHubToolStripMenuItem.Name = "gitHubToolStripMenuItem";
            this.gitHubToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.gitHubToolStripMenuItem.Text = "GitHub";
            this.gitHubToolStripMenuItem.Click += new System.EventHandler(this.gitHubToolStripMenuItem_Click);
            // 
            // arduinoProjectToolStripMenuItem
            // 
            this.arduinoProjectToolStripMenuItem.Name = "arduinoProjectToolStripMenuItem";
            this.arduinoProjectToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.arduinoProjectToolStripMenuItem.Text = "Arduino Project";
            this.arduinoProjectToolStripMenuItem.Click += new System.EventHandler(this.arduinoProjectToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(157, 22);
            this.aboutToolStripMenuItem1.Text = "About";
            this.aboutToolStripMenuItem1.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsslFrequencyLabel,
            this.tsslFrequency});
            this.statusStrip.Location = new System.Drawing.Point(0, 473);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(604, 22);
            this.statusStrip.TabIndex = 29;
            this.statusStrip.Text = "statusStrip1";
            // 
            // tsslFrequencyLabel
            // 
            this.tsslFrequencyLabel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tsslFrequencyLabel.Name = "tsslFrequencyLabel";
            this.tsslFrequencyLabel.Size = new System.Drawing.Size(68, 17);
            this.tsslFrequencyLabel.Text = "Frequency:";
            // 
            // tsslFrequency
            // 
            this.tsslFrequency.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tsslFrequency.Name = "tsslFrequency";
            this.tsslFrequency.Size = new System.Drawing.Size(29, 17);
            this.tsslFrequency.Text = "0Hz";
            // 
            // timerFrequency
            // 
            this.timerFrequency.Tick += new System.EventHandler(this.timerFrequency_Tick);
            // 
            // pbVideoPanel
            // 
            this.pbVideoPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pbVideoPanel.BackColor = System.Drawing.Color.Gray;
            this.pbVideoPanel.Location = new System.Drawing.Point(0, 27);
            this.pbVideoPanel.Name = "pbVideoPanel";
            this.pbVideoPanel.Size = new System.Drawing.Size(604, 297);
            this.pbVideoPanel.TabIndex = 28;
            this.pbVideoPanel.TabStop = false;
            this.pbVideoPanel.SizeChanged += new System.EventHandler(this.pbVideoPanel_SizeChanged);
            // 
            // tbGlassesTimeOffset
            // 
            this.tbGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbGlassesTimeOffset.AutoSize = false;
            this.tbGlassesTimeOffset.Location = new System.Drawing.Point(6, 19);
            this.tbGlassesTimeOffset.Maximum = 100;
            this.tbGlassesTimeOffset.Name = "tbGlassesTimeOffset";
            this.tbGlassesTimeOffset.Size = new System.Drawing.Size(531, 23);
            this.tbGlassesTimeOffset.TabIndex = 30;
            this.tbGlassesTimeOffset.Scroll += new System.EventHandler(this.tbGlassesTimeOffset_Scroll);
            this.tbGlassesTimeOffset.MouseEnter += new System.EventHandler(this.tbGlassesTimeOffset_MouseEnter);
            // 
            // lblGlassesTimeOffset
            // 
            this.lblGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblGlassesTimeOffset.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGlassesTimeOffset.Location = new System.Drawing.Point(534, 16);
            this.lblGlassesTimeOffset.Name = "lblGlassesTimeOffset";
            this.lblGlassesTimeOffset.Size = new System.Drawing.Size(53, 23);
            this.lblGlassesTimeOffset.TabIndex = 31;
            this.lblGlassesTimeOffset.Text = "0";
            this.lblGlassesTimeOffset.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.tbGlassesTimeOffset);
            this.groupBox1.Controls.Add(this.lblGlassesTimeOffset);
            this.groupBox1.Location = new System.Drawing.Point(5, 418);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(593, 52);
            this.groupBox1.TabIndex = 34;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Glasses Time Offset";
            // 
            // timerSendSettings
            // 
            this.timerSendSettings.Tick += new System.EventHandler(this.timerSendSettings_Tick);
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.bVertical);
            this.groupBox3.Controls.Add(this.tbVolume);
            this.groupBox3.Controls.Add(this.bSound);
            this.groupBox3.Controls.Add(this.bRefresh);
            this.groupBox3.Controls.Add(this.cbComPort);
            this.groupBox3.Controls.Add(this.bSwap);
            this.groupBox3.Controls.Add(this.bBoth);
            this.groupBox3.Controls.Add(this.bRightOnly);
            this.groupBox3.Controls.Add(this.bLeftOnly);
            this.groupBox3.Controls.Add(this.cbTracks);
            this.groupBox3.Controls.Add(this.bOpen);
            this.groupBox3.Controls.Add(this.bStop);
            this.groupBox3.Controls.Add(this.bPlayPause);
            this.groupBox3.Controls.Add(this.tbMovieTime);
            this.groupBox3.Controls.Add(this.lblMovieTime);
            this.groupBox3.Location = new System.Drawing.Point(5, 327);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(0);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(0);
            this.groupBox3.Size = new System.Drawing.Size(593, 85);
            this.groupBox3.TabIndex = 35;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Player Controls";
            // 
            // bVertical
            // 
            this.bVertical.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.horizontal;
            this.bVertical.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bVertical.FlatAppearance.BorderSize = 0;
            this.bVertical.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bVertical.Location = new System.Drawing.Point(560, 45);
            this.bVertical.Name = "bVertical";
            this.bVertical.Size = new System.Drawing.Size(26, 26);
            this.bVertical.TabIndex = 45;
            this.bVertical.UseVisualStyleBackColor = true;
            this.bVertical.Click += new System.EventHandler(this.bVertical_Click);
            this.bVertical.MouseEnter += new System.EventHandler(this.bVertical_MouseEnter);
            // 
            // tbVolume
            // 
            this.tbVolume.AutoSize = false;
            this.tbVolume.Location = new System.Drawing.Point(120, 47);
            this.tbVolume.Maximum = 65535;
            this.tbVolume.Name = "tbVolume";
            this.tbVolume.Size = new System.Drawing.Size(112, 23);
            this.tbVolume.TabIndex = 34;
            this.tbVolume.TickFrequency = 5000;
            this.tbVolume.Value = 70;
            this.tbVolume.Scroll += new System.EventHandler(this.tbVolume_Scroll);
            this.tbVolume.MouseEnter += new System.EventHandler(this.tbVolume_MouseEnter);
            // 
            // bSound
            // 
            this.bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.sound;
            this.bSound.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bSound.FlatAppearance.BorderSize = 0;
            this.bSound.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bSound.Location = new System.Drawing.Point(94, 45);
            this.bSound.Name = "bSound";
            this.bSound.Size = new System.Drawing.Size(26, 26);
            this.bSound.TabIndex = 44;
            this.bSound.UseVisualStyleBackColor = true;
            this.bSound.Click += new System.EventHandler(this.bSound_Click);
            this.bSound.MouseEnter += new System.EventHandler(this.bSound_MouseEnter);
            // 
            // bRefresh
            // 
            this.bRefresh.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.refresh;
            this.bRefresh.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bRefresh.FlatAppearance.BorderSize = 0;
            this.bRefresh.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bRefresh.Location = new System.Drawing.Point(415, 45);
            this.bRefresh.Name = "bRefresh";
            this.bRefresh.Size = new System.Drawing.Size(26, 26);
            this.bRefresh.TabIndex = 42;
            this.bRefresh.UseVisualStyleBackColor = true;
            this.bRefresh.Click += new System.EventHandler(this.bRefresh_Click);
            this.bRefresh.MouseEnter += new System.EventHandler(this.bRefresh_MouseEnter);
            // 
            // cbComPort
            // 
            this.cbComPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbComPort.FormattingEnabled = true;
            this.cbComPort.Location = new System.Drawing.Point(309, 48);
            this.cbComPort.Name = "cbComPort";
            this.cbComPort.Size = new System.Drawing.Size(100, 21);
            this.cbComPort.TabIndex = 41;
            this.cbComPort.SelectedIndexChanged += new System.EventHandler(this.cbComPort_SelectedIndexChanged);
            this.cbComPort.MouseEnter += new System.EventHandler(this.cbComPort_MouseEnter);
            // 
            // bSwap
            // 
            this.bSwap.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.swap_unselected;
            this.bSwap.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bSwap.FlatAppearance.BorderSize = 0;
            this.bSwap.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bSwap.Location = new System.Drawing.Point(531, 45);
            this.bSwap.Name = "bSwap";
            this.bSwap.Size = new System.Drawing.Size(26, 26);
            this.bSwap.TabIndex = 40;
            this.bSwap.UseVisualStyleBackColor = true;
            this.bSwap.Click += new System.EventHandler(this.bSwap_Click);
            this.bSwap.MouseEnter += new System.EventHandler(this.bSwap_MouseEnter);
            // 
            // bBoth
            // 
            this.bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
            this.bBoth.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bBoth.FlatAppearance.BorderSize = 0;
            this.bBoth.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bBoth.Location = new System.Drawing.Point(444, 45);
            this.bBoth.Name = "bBoth";
            this.bBoth.Size = new System.Drawing.Size(26, 26);
            this.bBoth.TabIndex = 39;
            this.bBoth.UseVisualStyleBackColor = true;
            this.bBoth.Click += new System.EventHandler(this.bBoth_Click);
            this.bBoth.MouseEnter += new System.EventHandler(this.bBoth_MouseEnter);
            // 
            // bRightOnly
            // 
            this.bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
            this.bRightOnly.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bRightOnly.FlatAppearance.BorderSize = 0;
            this.bRightOnly.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bRightOnly.Location = new System.Drawing.Point(502, 45);
            this.bRightOnly.Name = "bRightOnly";
            this.bRightOnly.Size = new System.Drawing.Size(26, 26);
            this.bRightOnly.TabIndex = 38;
            this.bRightOnly.UseVisualStyleBackColor = true;
            this.bRightOnly.Click += new System.EventHandler(this.bRightOnly_Click);
            this.bRightOnly.MouseEnter += new System.EventHandler(this.bRightOnly_MouseEnter);
            // 
            // bLeftOnly
            // 
            this.bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
            this.bLeftOnly.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bLeftOnly.FlatAppearance.BorderSize = 0;
            this.bLeftOnly.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bLeftOnly.Location = new System.Drawing.Point(473, 45);
            this.bLeftOnly.Name = "bLeftOnly";
            this.bLeftOnly.Size = new System.Drawing.Size(26, 26);
            this.bLeftOnly.TabIndex = 37;
            this.bLeftOnly.UseVisualStyleBackColor = true;
            this.bLeftOnly.Click += new System.EventHandler(this.bLeftOnly_Click);
            this.bLeftOnly.MouseEnter += new System.EventHandler(this.bLeftOnly_MouseEnter);
            // 
            // cbTracks
            // 
            this.cbTracks.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbTracks.FormattingEnabled = true;
            this.cbTracks.Location = new System.Drawing.Point(233, 48);
            this.cbTracks.Name = "cbTracks";
            this.cbTracks.Size = new System.Drawing.Size(70, 21);
            this.cbTracks.TabIndex = 36;
            this.cbTracks.SelectedIndexChanged += new System.EventHandler(this.cbTracks_SelectedIndexChanged);
            this.cbTracks.MouseEnter += new System.EventHandler(this.cbTracks_MouseEnter);
            // 
            // bOpen
            // 
            this.bOpen.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.open;
            this.bOpen.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bOpen.FlatAppearance.BorderSize = 0;
            this.bOpen.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bOpen.Location = new System.Drawing.Point(7, 45);
            this.bOpen.Name = "bOpen";
            this.bOpen.Size = new System.Drawing.Size(26, 26);
            this.bOpen.TabIndex = 35;
            this.bOpen.UseVisualStyleBackColor = true;
            this.bOpen.Click += new System.EventHandler(this.bOpen_Click);
            this.bOpen.MouseEnter += new System.EventHandler(this.bOpen_MouseEnter);
            // 
            // bStop
            // 
            this.bStop.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.astop;
            this.bStop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bStop.FlatAppearance.BorderSize = 0;
            this.bStop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bStop.Location = new System.Drawing.Point(65, 45);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(26, 26);
            this.bStop.TabIndex = 34;
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.bStop_Click);
            this.bStop.MouseEnter += new System.EventHandler(this.bStop_MouseEnter);
            // 
            // bPlayPause
            // 
            this.bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
            this.bPlayPause.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bPlayPause.FlatAppearance.BorderSize = 0;
            this.bPlayPause.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bPlayPause.Location = new System.Drawing.Point(36, 45);
            this.bPlayPause.Name = "bPlayPause";
            this.bPlayPause.Size = new System.Drawing.Size(26, 26);
            this.bPlayPause.TabIndex = 32;
            this.bPlayPause.UseVisualStyleBackColor = true;
            this.bPlayPause.Click += new System.EventHandler(this.bPlayPause_Click);
            this.bPlayPause.MouseEnter += new System.EventHandler(this.bPlayPause_MouseEnter);
            // 
            // tbMovieTime
            // 
            this.tbMovieTime.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbMovieTime.AutoSize = false;
            this.tbMovieTime.Location = new System.Drawing.Point(3, 18);
            this.tbMovieTime.Maximum = 100;
            this.tbMovieTime.Name = "tbMovieTime";
            this.tbMovieTime.Size = new System.Drawing.Size(504, 21);
            this.tbMovieTime.TabIndex = 30;
            this.tbMovieTime.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbMovieTime.Scroll += new System.EventHandler(this.tbMovieTime_Scroll);
            this.tbMovieTime.MouseEnter += new System.EventHandler(this.tbMovieTime_MouseEnter);
            // 
            // lblMovieTime
            // 
            this.lblMovieTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblMovieTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMovieTime.Location = new System.Drawing.Point(483, 18);
            this.lblMovieTime.Name = "lblMovieTime";
            this.lblMovieTime.Size = new System.Drawing.Size(104, 23);
            this.lblMovieTime.TabIndex = 31;
            this.lblMovieTime.Text = "00:00:00";
            this.lblMovieTime.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // timerGUIStereoPlayer
            // 
            this.timerGUIStereoPlayer.Tick += new System.EventHandler(this.timerGUIStereoPlayer_Tick);
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // MainFrm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(604, 495);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pbVideoPanel);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.menuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(620, 450);
            this.Name = "MainFrm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainFrm_FormClosing);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainFrm_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainFrm_DragEnter);
            this.Move += new System.EventHandler(this.MainFrm_Move);
            this.Resize += new System.EventHandler(this.MainFrm_Resize);
            this.popUpMenu.ResumeLayout(false);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMovieTime)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip popUpMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem hideOrShowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem runAtStartupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem alwaysOnTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.NotifyIcon trayNotifyIcon;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem visitWebSiteToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel tsslFrequencyLabel;
        private System.Windows.Forms.ToolStripStatusLabel tsslFrequency;
        private System.Windows.Forms.Timer timerFrequency;
        private System.Windows.Forms.PictureBox pbVideoPanel;
        private System.Windows.Forms.TrackBar tbGlassesTimeOffset;
        private System.Windows.Forms.Label lblGlassesTimeOffset;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Timer timerSendSettings;
        private System.Windows.Forms.ToolStripMenuItem gitHubToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem arduinoProjectToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TrackBar tbMovieTime;
        private System.Windows.Forms.Label lblMovieTime;
        private System.Windows.Forms.Button bStop;
        private System.Windows.Forms.Button bPlayPause;
        private System.Windows.Forms.Button bOpen;
        private System.Windows.Forms.OpenFileDialog dlgOpen;
        private System.Windows.Forms.ComboBox cbTracks;
        private System.Windows.Forms.Timer timerGUIStereoPlayer;
        private System.Windows.Forms.Button bSwap;
        private System.Windows.Forms.Button bBoth;
        private System.Windows.Forms.Button bRightOnly;
        private System.Windows.Forms.Button bLeftOnly;
        private System.Windows.Forms.Button bRefresh;
        private System.Windows.Forms.ComboBox cbComPort;
        private System.Windows.Forms.Button bSound;
        private System.Windows.Forms.TrackBar tbVolume;
        private System.Windows.Forms.ToolTip ttControls;
        private System.Windows.Forms.Button bVertical;
    }
}

