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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.timerSendSettings = new System.Windows.Forms.Timer(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.bOpen = new System.Windows.Forms.Button();
            this.dlgOpen = new System.Windows.Forms.OpenFileDialog();
            this.timerGUIStereoPlayer = new System.Windows.Forms.Timer(this.components);
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.ptpPlayTimePanel = new StereoscopicMoviePlayer.PlayTimePanel();
            this.ppPlayerPanel = new StereoscopicMoviePlayer.PlayerPanel();
            this.spSoundPanel = new StereoscopicMoviePlayer.SoundPanel();
            this.cppComPortPanel = new StereoscopicMoviePlayer.COMPortPanel();
            this.sbpStereoButtonsPanel = new StereoscopicMoviePlayer.StereoButtonsPanel();
            this.gtopGlassesTimeOffsetPanel = new StereoscopicMoviePlayer.GlassesTimeOffsetPanel();
            this.popUpMenu.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
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
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.gtopGlassesTimeOffsetPanel);
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
            this.groupBox3.Controls.Add(this.ptpPlayTimePanel);
            this.groupBox3.Controls.Add(this.ppPlayerPanel);
            this.groupBox3.Controls.Add(this.spSoundPanel);
            this.groupBox3.Controls.Add(this.cppComPortPanel);
            this.groupBox3.Controls.Add(this.sbpStereoButtonsPanel);
            this.groupBox3.Controls.Add(this.bOpen);
            this.groupBox3.Location = new System.Drawing.Point(5, 327);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(0);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(0);
            this.groupBox3.Size = new System.Drawing.Size(593, 85);
            this.groupBox3.TabIndex = 35;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Player Controls";
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
            // ptpPlayTimePanel
            // 
            this.ptpPlayTimePanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ptpPlayTimePanel.CurrentPlayingTime = ((long)(0));
            this.ptpPlayTimePanel.Duration = ((long)(0));
            this.ptpPlayTimePanel.Location = new System.Drawing.Point(7, 19);
            this.ptpPlayTimePanel.Name = "ptpPlayTimePanel";
            this.ptpPlayTimePanel.Size = new System.Drawing.Size(580, 20);
            this.ptpPlayTimePanel.TabIndex = 47;
            this.ptpPlayTimePanel.OnSeek += new System.EventHandler<StereoscopicMoviePlayer.PlayTimePanel.SeekEventArgs>(this.ptpPlayTimePanel_OnSeek);
            this.ptpPlayTimePanel.OnValueUpdateRequest += new System.EventHandler(this.ptpPlayTimePanel_OnValueUpdateRequest);
            // 
            // ppPlayerPanel
            // 
            this.ppPlayerPanel.ButtonsState = StereoscopicMoviePlayer.PlayerPanel.eButtonsStates.Stopped;
            this.ppPlayerPanel.Location = new System.Drawing.Point(36, 45);
            this.ppPlayerPanel.Name = "ppPlayerPanel";
            this.ppPlayerPanel.Size = new System.Drawing.Size(55, 26);
            this.ppPlayerPanel.TabIndex = 46;
            this.ppPlayerPanel.OnPlay += new System.EventHandler(this.ppPlayerPanel_OnPlay);
            this.ppPlayerPanel.OnPlayAfterPause += new System.EventHandler(this.ppPlayerPanel_OnPlayAfterPause);
            this.ppPlayerPanel.OnPause += new System.EventHandler(this.ppPlayerPanel_OnPause);
            this.ppPlayerPanel.OnStop += new System.EventHandler(this.ppPlayerPanel_OnStop);
            // 
            // spSoundPanel
            // 
            this.spSoundPanel.Location = new System.Drawing.Point(95, 45);
            this.spSoundPanel.Name = "spSoundPanel";
            this.spSoundPanel.SelectedSoundTrack = 0;
            this.spSoundPanel.Size = new System.Drawing.Size(213, 26);
            this.spSoundPanel.SoundState = StereoscopicMoviePlayer.SoundPanel.eSoundStates.SoundOn;
            this.spSoundPanel.SoundVolume = ((ushort)(0));
            this.spSoundPanel.TabIndex = 36;
            this.spSoundPanel.OnSoundOff += new System.EventHandler(this.spSoundPanel_OnSoundOff);
            this.spSoundPanel.OnSoundOn += new System.EventHandler(this.spSoundPanel_OnSoundOn);
            this.spSoundPanel.OnSelectedSoundTrackChanged += new System.EventHandler<StereoscopicMoviePlayer.SoundPanel.SelectedSoundTrackEventArgs>(this.spSoundPanel_OnSelectedSoundTrackChanged);
            this.spSoundPanel.OnSoundVolumeChanged += new System.EventHandler<StereoscopicMoviePlayer.SoundPanel.SoundVolumeEventArgs>(this.spSoundPanel_OnSoundVolumeChanged);
            // 
            // cppComPortPanel
            // 
            this.cppComPortPanel.Location = new System.Drawing.Point(309, 45);
            this.cppComPortPanel.Name = "cppComPortPanel";
            this.cppComPortPanel.SelectedCOMPort = "";
            this.cppComPortPanel.Size = new System.Drawing.Size(132, 26);
            this.cppComPortPanel.TabIndex = 45;
            this.cppComPortPanel.OnCOMPortSelected += new System.EventHandler<StereoscopicMoviePlayer.COMPortPanel.COMPortEventArgs>(this.cppComPortPanel_OnCOMPortSelected);
            // 
            // sbpStereoButtonsPanel
            // 
            this.sbpStereoButtonsPanel.HVButtonState = StereoscopicMoviePlayer.StereoButtonsPanel.eHVButtonStates.Horizontal;
            this.sbpStereoButtonsPanel.Location = new System.Drawing.Point(445, 45);
            this.sbpStereoButtonsPanel.LRBothButtonsState = StereoscopicMoviePlayer.StereoButtonsPanel.eLRBothButtonsStates.Both;
            this.sbpStereoButtonsPanel.Name = "sbpStereoButtonsPanel";
            this.sbpStereoButtonsPanel.Size = new System.Drawing.Size(142, 26);
            this.sbpStereoButtonsPanel.SwapButtonState = StereoscopicMoviePlayer.StereoButtonsPanel.eSwapButtonStates.SwapOff;
            this.sbpStereoButtonsPanel.TabIndex = 36;
            this.sbpStereoButtonsPanel.OnBoth += new System.EventHandler(this.sbpStereoButtonsPanel_OnBoth);
            this.sbpStereoButtonsPanel.OnLeftOnly += new System.EventHandler(this.sbpStereoButtonsPanel_OnLeftOnly);
            this.sbpStereoButtonsPanel.OnRightOnly += new System.EventHandler(this.sbpStereoButtonsPanel_OnRightOnly);
            this.sbpStereoButtonsPanel.OnNoSwap += new System.EventHandler(this.sbpStereoButtonsPanel_OnNoSwap);
            this.sbpStereoButtonsPanel.OnSwap += new System.EventHandler(this.sbpStereoButtonsPanel_OnSwap);
            this.sbpStereoButtonsPanel.OnHorizontal += new System.EventHandler(this.sbpStereoButtonsPanel_OnHorizontal);
            this.sbpStereoButtonsPanel.OnVertical += new System.EventHandler(this.sbpStereoButtonsPanel_OnVertical);
            // 
            // gtopGlassesTimeOffsetPanel
            // 
            this.gtopGlassesTimeOffsetPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gtopGlassesTimeOffsetPanel.Location = new System.Drawing.Point(7, 20);
            this.gtopGlassesTimeOffsetPanel.Name = "gtopGlassesTimeOffsetPanel";
            this.gtopGlassesTimeOffsetPanel.Size = new System.Drawing.Size(580, 24);
            this.gtopGlassesTimeOffsetPanel.TabIndex = 0;
            this.gtopGlassesTimeOffsetPanel.TimeOffset = 0;
            this.gtopGlassesTimeOffsetPanel.OnTimeOffset += new System.EventHandler<StereoscopicMoviePlayer.GlassesTimeOffsetPanel.TimeOffsetEventArgs>(this.gtopGlassesTimeOffsetPanel_OnTimeOffset);
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
            this.groupBox1.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Timer timerSendSettings;
        private System.Windows.Forms.ToolStripMenuItem gitHubToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem arduinoProjectToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button bOpen;
        private System.Windows.Forms.OpenFileDialog dlgOpen;
        private System.Windows.Forms.Timer timerGUIStereoPlayer;
        private System.Windows.Forms.ToolTip ttControls;
        private StereoButtonsPanel sbpStereoButtonsPanel;
        private COMPortPanel cppComPortPanel;
        private SoundPanel spSoundPanel;
        private PlayerPanel ppPlayerPanel;
        private PlayTimePanel ptpPlayTimePanel;
        private GlassesTimeOffsetPanel gtopGlassesTimeOffsetPanel;
    }
}

