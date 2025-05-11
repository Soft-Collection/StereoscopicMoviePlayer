using System;
using System.IO;
using System.Diagnostics;
using System.IO.Ports;
using System.Security.Permissions;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    public partial class MainFrm : Form
    {
        #region Main
        public MainFrm()
        {
            InitializeComponent();
            Initialize();
            Init();
            InitStereo();
            InitPlayer();
        }
        #endregion

        #region Template
        #region Initialize
        //Initialize.
        private void Initialize()
        {
            try
            {
                Process currentProcess = Process.GetCurrentProcess();
                currentProcess.PriorityClass = ProcessPriorityClass.RealTime;
            }
            catch (Exception)
            {
            }
            this.Text = GetAssemblyInfo.AssemblyTitle;
            this.Icon = global::StereoscopicMoviePlayer.Properties.Resources.program_icon;
            trayNotifyIcon.Icon = global::StereoscopicMoviePlayer.Properties.Resources.program_icon;
            //Load Settings from Registry.
            this.Location = Settings.Location;
            this.Size = Settings.Size;
            this.Visible = Settings.Visible;
            this.TopMost = Settings.AlwaysOnTop;
            //-----------------------------------
            // Get a list of all available COM ports
            string[] ports = SerialPort.GetPortNames();
            cbComPort.Items.Clear();
            foreach (string port in ports)
            {
                cbComPort.Items.Add(port);
            }
            if (cbComPort.Items.Contains(Settings.ComPort))
            {
                cbComPort.SelectedItem = Settings.ComPort;
            }
            else
            {
                cbComPort.SelectedIndex = 0;
            }
            cbComPort.Visible = true;
            bRefresh.Visible = true;
            //-----------------------------------
            pbVideoPanel.BackColor = System.Drawing.Color.DarkGray;
            //------------------------------------------------------
            bStartStop.Enabled = false;
            tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
            lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
            tbGlassesTimeOffset.Enabled = false;
            tbTransparentTimePercent.Value = Settings.TransparentTimePercent;
            lblTransparentTimePercent.Text = Settings.TransparentTimePercent.ToString();
            tbTransparentTimePercent.Enabled = false;
            tsslFrequencyLabel.Visible = false;
            tsslFrequency.Visible = false;
            //-----------------------------------
            //Apply these settings.
            SetVisible();
            SetAlwaysOnTop();
            SetRunAtStartup();
            //-----------------------------------
            //LoadStereoImages();
            //-----------------------------------
            if (mStereoImageManager == null)
            {
                mStereoImageManager = new clsStereoImageManager(pbVideoPanel.Handle);
            }
        }
        #endregion

        #region Menu Events
        //When clicked on Hide or Show.
        private void hideOrShowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeVisible();
        }
        //When clicked on Run At Startup.
        private void runAtStartupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeRunAtStartup();
        }
        //When clicked on Always On Top.
        private void alwaysOnTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeAlwaysOnTop();
        }
        private void ExitProg()
        {
            PerformStop();
            if (mStereoImageManager != null)
            {
                mStereoImageManager.Dispose();
                mStereoImageManager = null;
            }
            Application.Exit();
        }
        //When clicked on About.
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox about = new AboutBox();
            about.TopMost = true;
            about.ShowDialog();
        }
        //When clicked on Exit.
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExitProg();
        }
        private void trayNotifyIcon_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left) ChangeVisible();
        }
        #endregion

        #region Set Settings and Update Menu Items
        //Changes Visible state.
        private void ChangeVisible()
        {
            if (this.Visible)
            {
                bool isStarted = false;
                if (mStereoImageManager != null)
                {
                    isStarted = mStereoImageManager.StereoIsStarted();
                }
                mWasStartedWhenMinimized = isStarted;
                if (isStarted) PerformStop();
            }
            else
            {
                if (mWasStartedWhenMinimized)
                {
                    bool isStarted = false;
                    if (mStereoImageManager != null)
                    {
                        isStarted = mStereoImageManager.StereoIsStarted();
                    }
                    if (!isStarted) PerformStart();
                }
            }
            this.Visible = !this.Visible;
            SetVisible();
        }
        //Apply current Visible value.
        private void SetVisible()
        {
            Settings.Visible = this.Visible;
            if (this.Visible)
            {
                hideOrShowToolStripMenuItem.Text = "Hide";
                trayNotifyIcon.Text = "Hide " + GetAssemblyInfo.AssemblyProduct;
                this.Opacity = 1;
            }
            else
            {
                hideOrShowToolStripMenuItem.Text = "Show";
                trayNotifyIcon.Text = "Show " + GetAssemblyInfo.AssemblyProduct;
                this.Opacity = 0;

            }
        }
        //Changes Run At Startup state.
        private void ChangeRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = !runAtStartupToolStripMenuItem.Checked;
            Settings.RunAtStartup = runAtStartupToolStripMenuItem.Checked;
        }
        //Apply current Run At Startup value.
        private void SetRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = Settings.RunAtStartup;
        }
        //Changes Always On Top state.
        private void ChangeAlwaysOnTop()
        {
            this.TopMost = !this.TopMost;
            SetAlwaysOnTop();
        }
        //Apply current Always On Top value.
        private void SetAlwaysOnTop()
        {
            Settings.AlwaysOnTop = this.TopMost;
            alwaysOnTopToolStripMenuItem.Checked = this.TopMost;
        }
        #endregion

        #region Main Form Events
        private void MainFrm_Move(object sender, EventArgs e)
        {
            Settings.Location = this.Location;
        }
        private void MainFrm_Resize(object sender, EventArgs e)
        {
            Settings.Size = this.Size;
        }
        private void MainFrm_FormClosing(object sender, FormClosingEventArgs e)
        {
            ExitProg();
        }
        #endregion

        #region WinProc
        private const int WM_SYSCOMMAND = 0x0112;
        private const int SC_MINIMIZE = 0xF020;
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.UnmanagedCode)]
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_SYSCOMMAND:
                    int command = m.WParam.ToInt32() & 0xfff0;
                    if (command == SC_MINIMIZE)
                    {
                        try
                        {
                            ChangeVisible();
                            return;
                        }
                        catch
                        {
                        }
                    }
                    break;
            }
            base.WndProc(ref m);
        }
        #endregion
        #endregion

        #region StereoscopicMoviePlayer

        #region Enums
        #endregion

        #region Variables
        private bool mWasStartedWhenMinimized = false;
        private clsStereoImageManager mStereoImageManager = null;
        #endregion

        #region Initialize
        private void Init()
        {
        }
        #endregion

        #region Menu Events
        private void visitWebSiteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://www.soft-collection.com");
        }
        private void gitHubToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicMoviePlayer");
        }
        private void arduinoProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicMoviePlayer/blob/master/ArduinoProject/CableGlasses/CableGlasses.ino");
        }
        #endregion

        #region Main Form Events
        #endregion

        #region Methods
        #endregion

        #endregion

        #region Stereo

        #region Enums
        private enum eStereoStates
        {
            VideoFileNotOpened = 0,
            VideoFileOpened = 1,
            Stereo = 2,
        }
        #endregion

        #region Variables
        private eStereoStates mStereoState = eStereoStates.VideoFileNotOpened;
        private eStereoStates mLastStereoState = eStereoStates.Stereo;
        private bool mAlreadySent = true;
        private int mAlreadySentCounter = 0;
        #endregion

        #region Initialize
        private void InitStereo()
        {
            timerFrequency.Start();
            timerSendSettings.Start();
            timerGUIStereo.Start();
            if (File.Exists(Settings.FilePath))
            {
                mStereoState = eStereoStates.VideoFileOpened;
            }
            else
            {
                mStereoState = eStereoStates.VideoFileNotOpened;
            }
        }
        #endregion

        #region Menu Events
        #endregion

        #region Main Form Events
        private void timerGUIStereo_Tick(object sender, EventArgs e)
        {
            if (!File.Exists(Settings.FilePath))
            {
                mStereoState = eStereoStates.VideoFileNotOpened;
            }
            if (mLastStereoState != mStereoState)
            {
                SetStereoState(mStereoState);
                mLastStereoState = mStereoState;
            }
        }
        private void cbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.ComPort = (string)cbComPort.SelectedItem;
        }
        private void bRefresh_Click(object sender, EventArgs e)
        {
            // Get a list of all available COM ports
            string[] ports = SerialPort.GetPortNames();
            cbComPort.Items.Clear();
            foreach (string port in ports)
            {
                cbComPort.Items.Add(port);
            }
            if (cbComPort.Items.Contains(Settings.ComPort))
            {
                cbComPort.SelectedItem = Settings.ComPort;
            }
            else
            {
                cbComPort.SelectedIndex = 0;
            }
        }
        private void bStartStop_Click(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                bool isStarted = false;
                isStarted = mStereoImageManager.StereoIsStarted();
                if (isStarted) PerformStop(); else PerformStart();
            }
        }
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            int frequencyInHz = 0;
            if (mStereoImageManager != null)
            {
                frequencyInHz = mStereoImageManager.StereoGetFrequency();
            }
            tsslFrequency.Text = frequencyInHz.ToString() + "Hz";
        }
        private void timerSendSettings_Tick(object sender, EventArgs e)
        {
            if (!mAlreadySent)
            {
                if (mAlreadySentCounter > 5)
                {
                    if (mStereoImageManager != null)
                    {
                        mStereoImageManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
                        mStereoImageManager.StereoSetTransparentTimePercent(Settings.TransparentTimePercent);
                    }
                    mAlreadySent = true;
                }
                mAlreadySentCounter++;
            }
        }
        private void tbGlassesTimeOffset_Scroll(object sender, EventArgs e)
        {
            lblGlassesTimeOffset.Text = tbGlassesTimeOffset.Value.ToString();
            Settings.GlassesTimeOffset = tbGlassesTimeOffset.Value;
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
            }
        }
        private void tbTransparentTimePercent_Scroll(object sender, EventArgs e)
        {
            lblTransparentTimePercent.Text = tbTransparentTimePercent.Value.ToString();
            Settings.TransparentTimePercent = tbTransparentTimePercent.Value;
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSetTransparentTimePercent(Settings.TransparentTimePercent);
            }
        }
        #endregion

        #region Methods
        private void SetStereoState(eStereoStates stereoState)
        {
            switch (stereoState)
            {
                case eStereoStates.VideoFileNotOpened:
                    bStartStop.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.play;
                    bStartStop.Enabled = false;
                    cbComPort.Enabled = true;
                    bRefresh.Enabled = true;
                    bLeftOnly.Enabled = false;
                    bRightOnly.Enabled = false;
                    bBoth.Enabled = false;
                    bSwap.Enabled = false;
                    tbGlassesTimeOffset.Enabled = false;
                    tbTransparentTimePercent.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eStereoStates.VideoFileOpened:
                    bStartStop.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.play;
                    bStartStop.Enabled = true;
                    cbComPort.Enabled = true;
                    bRefresh.Enabled = true;
                    bLeftOnly.Enabled = true;
                    bRightOnly.Enabled = true;
                    bBoth.Enabled = true;
                    bSwap.Enabled = true;
                    tbGlassesTimeOffset.Enabled = false;
                    tbTransparentTimePercent.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eStereoStates.Stereo:
                    bStartStop.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.stop;
                    bStartStop.Enabled = true;
                    cbComPort.Enabled = false;
                    bRefresh.Enabled = false;
                    bLeftOnly.Enabled = true;
                    bRightOnly.Enabled = true;
                    bBoth.Enabled = true;
                    bSwap.Enabled = true;
                    tbGlassesTimeOffset.Enabled = true;
                    tbTransparentTimePercent.Enabled = true;
                    tsslFrequencyLabel.Visible = true;
                    tsslFrequency.Visible = true;
                    break;
            }
        }
        private void PerformStart()
        {
            if (mStereoImageManager != null)
            {
                bool isStarted = false;
                isStarted = mStereoImageManager.StereoIsStarted();
                //To Be Started here.
                if (!isStarted)
                {
                    mStereoState = eStereoStates.Stereo;
                    Start();
                    mAlreadySent = false;
                    mAlreadySentCounter = 0;
                }
            }
        }
        private void PerformStop()
        {
            if (mStereoImageManager != null)
            {
                bool isStarted = false;
                isStarted = mStereoImageManager.StereoIsStarted();
                //To Be Stopped here.
                if (isStarted)
                {
                    Stop();
                    mStereoState = eStereoStates.VideoFileOpened;
                    pbVideoPanel.Refresh();
                }
            }
        }
        private void Start()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSetCOMPort(Settings.ComPort);
                mStereoImageManager.StereoStart();
            }
        }
        private void Stop()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoStop();
            }
        }
        #endregion

        #endregion

        #region Player

        #region Enums
        private enum ePlayerStates
        {
            Disabled = 0,
            Playing = 1,
            Paused = 2,
            Stopped = 3
        }
        #endregion

        #region Variables
        private ePlayerStates mPlayerState = ePlayerStates.Disabled;
        private ePlayerStates mLastPlayerState = ePlayerStates.Playing;
        #endregion

        #region Initialize
        private void InitPlayer()
        {
            timerGUIPlayer.Start();
            if (File.Exists(Settings.FilePath))
            {
                mPlayerState = ePlayerStates.Stopped;
                if (mStereoImageManager != null)
                {
                    mStereoImageManager.PlayerOpen(Settings.FilePath);
                    Int64 duration = mStereoImageManager.PlayerGetDuration();
                    tbMovieTime.Maximum = (int)duration;
                }
                LoadTracks();
            }
            else
            {
                mPlayerState = ePlayerStates.Disabled;
            }
        }
        #endregion

        #region Main Form Events
        private void timerGUIPlayer_Tick(object sender, EventArgs e)
        {
            if (!File.Exists(Settings.FilePath))
            {
                mPlayerState = ePlayerStates.Disabled;
            }
            if (mLastPlayerState != mPlayerState)
            {
                SetPlayerState(mPlayerState);
                mLastPlayerState = mPlayerState;
            }
            if (mStereoImageManager != null)
            {
                Int64 currentPlayingTime = mStereoImageManager.PlayerGetCurrentPlayingTime();
                tbMovieTime.Value = (int)currentPlayingTime;
                int hours = (int)currentPlayingTime / 3600;
                int minutes = (int)currentPlayingTime / 60 % 60;
                int seconds = (int)currentPlayingTime % 60;
                lblMovieTime.Text = $"{hours:D2}:{minutes:D2}:{seconds:D2}";
            }
        }
        private void bPlay_Click(object sender, EventArgs e)
        {
            mPlayerState = ePlayerStates.Playing;
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    mStereoImageManager.PlayerPlay();
                }
            }
        }
        private void bPause_Click(object sender, EventArgs e)
        {
            mPlayerState = ePlayerStates.Paused;
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    mStereoImageManager.PlayerPause();
                }
            }
        }
        private void bStop_Click(object sender, EventArgs e)
        {
            mPlayerState = ePlayerStates.Stopped;
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    mStereoImageManager.PlayerStop();
                }
            }
        }
        private void bOpen_Click(object sender, EventArgs e)
        {
            string tempFilePath = Settings.FilePath;
            string temp = OpenFile();
            Settings.FilePath = (temp == string.Empty) ? Settings.FilePath : temp;
            if (tempFilePath != Settings.FilePath)
            {
                if (File.Exists(Settings.FilePath))
                {
                    mPlayerState = ePlayerStates.Stopped;
                    mStereoState = eStereoStates.VideoFileOpened;
                    if (mStereoImageManager != null)
                    {
                        mStereoImageManager.PlayerOpen(Settings.FilePath);
                        Int64 duration = mStereoImageManager.PlayerGetDuration();
                        tbMovieTime.Maximum = (int)duration;
                    }
                    LoadTracks();
                }
                else
                {
                    mPlayerState = ePlayerStates.Disabled;
                    mStereoState = eStereoStates.VideoFileNotOpened;
                }
            }
        }
        private void cbTracks_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    mStereoImageManager.PlayerSetAudioTrack(cbTracks.SelectedIndex);
                }
            }
        }
        private void tbMovieTime_Scroll(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    mStereoImageManager.PlayerSeek(tbMovieTime.Value);
                }
            }
        }
        #endregion

        #region Methods
        private string OpenFile()
        {
            dlgOpen.Title = "Open Video Movie File";
            dlgOpen.Filter = "List Files(*.mkv;*.mp4;*.wmv;*.avi)|*.mkv;*.mp4;*.wmv;*.avi";
            dlgOpen.AddExtension = true;
            dlgOpen.CheckFileExists = true;
            dlgOpen.FileName = "";
            DialogResult dialogResult = dlgOpen.ShowDialog();
            if (dialogResult == DialogResult.OK)
            {
                return dlgOpen.FileName;
            }
            return string.Empty;
        }
        private void SetPlayerState(ePlayerStates playerState)
        {
            switch (playerState)
            {
                case ePlayerStates.Disabled:
                    bPlay.Enabled = false;
                    bPause.Enabled = false;
                    bStop.Enabled = false;
                    cbTracks.Enabled = false;
                    tbMovieTime.Enabled = false;
                    lblMovieTime.Enabled = false;
                    bPlay.FlatAppearance.BorderSize = 0;
                    bPause.FlatAppearance.BorderSize = 0;
                    bStop.FlatAppearance.BorderSize = 0;
                    cbTracks.Items.Clear();
                    tbMovieTime.Maximum = 0;
                    tbMovieTime.Value = 0;
                    lblMovieTime.Text = "00:00:00";
                    break;
                case ePlayerStates.Playing:
                    bPlay.Enabled = true;
                    bPause.Enabled = true;
                    bStop.Enabled = true;
                    cbTracks.Enabled = true;
                    tbMovieTime.Enabled = true;
                    lblMovieTime.Enabled = true;
                    bPlay.FlatAppearance.BorderSize = 1;
                    bPause.FlatAppearance.BorderSize = 0;
                    bStop.FlatAppearance.BorderSize = 0;
                    break;
                case ePlayerStates.Paused:
                    bPlay.Enabled = true;
                    bPause.Enabled = true;
                    bStop.Enabled = true;
                    cbTracks.Enabled = true;
                    tbMovieTime.Enabled = true;
                    lblMovieTime.Enabled = true;
                    bPlay.FlatAppearance.BorderSize = 0;
                    bPause.FlatAppearance.BorderSize = 1;
                    bStop.FlatAppearance.BorderSize = 0;
                    break;
                case ePlayerStates.Stopped:
                    bPlay.Enabled = true;
                    bPause.Enabled = true;
                    bStop.Enabled = true;
                    cbTracks.Enabled = true;
                    tbMovieTime.Enabled = true;
                    lblMovieTime.Enabled = true;
                    bPlay.FlatAppearance.BorderSize = 0;
                    bPause.FlatAppearance.BorderSize = 0;
                    bStop.FlatAppearance.BorderSize = 1;
                    break;
            }
        }
        private void LoadTracks()
        {
            cbTracks.Items.Clear();
            if (mStereoImageManager != null)
            {
                if (mStereoImageManager.PlayerIsOpened())
                {
                    int numberOfAudioTracks = mStereoImageManager.PlayerGetNumberOfAudioTracks();
                    for (int i = 0; i < numberOfAudioTracks; i++)
                    {
                        cbTracks.Items.Add("Track" + i.ToString());
                    }
                    if (cbTracks.Items.Count > 0)
                    {
                        cbTracks.SelectedIndex = 0;
                    }
                }
            }
        }
        #endregion

        #endregion
    }
}