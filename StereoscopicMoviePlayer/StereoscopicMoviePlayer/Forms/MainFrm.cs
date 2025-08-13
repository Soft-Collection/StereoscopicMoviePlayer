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
            InitStereoPlayer();
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
            if (mStereoVideoManager == null)
            {
                mStereoVideoManager = new clsStereoVideoManager(pbVideoPanel.Handle);
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
            PerformStereoStop();
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.Dispose();
                mStereoVideoManager = null;
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
                if (mStereoVideoManager != null)
                {
                    isStarted = mStereoVideoManager.StereoIsStarted();
                }
                mWasStartedWhenMinimized = isStarted;
                if (isStarted) PerformStereoStop();
            }
            else
            {
                if (mWasStartedWhenMinimized)
                {
                    bool isStarted = false;
                    if (mStereoVideoManager != null)
                    {
                        isStarted = mStereoVideoManager.StereoIsStarted();
                    }
                    if (!isStarted) PerformStereoStart();
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
        private clsStereoVideoManager mStereoVideoManager = null;
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
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicMoviePlayer/tree/master/ArduinoProject/CableGlasses");
        }
        #endregion

        #region Main Form Events
        #endregion

        #region Methods
        #endregion

        #endregion

        #region Stereo Player

        #region Enums
        private enum eMainStates
        {
            NotSetYet = 0,
            VideoFileNotOpened = 1,
            COMPortNotSelected = 2,
            Stopped = 3,
            PlayingPaused = 4
        }
        private enum eEnableDisableStates
        {
            NotSetYet = 0,
            VideoFileNotOpened = 1,
            COMPortNotSelected = 2,
            Stopped = 3,
            PlayingPaused = 4
        }
        private enum ePlayerButtonsStates
        {
            NotSetYet = 0,
            Playing = 1,
            Paused = 2,
            Stopped = 3,
            None = 4
        }
        private enum eStereoButtonsStates
        {
            NotSetYet = 0,
            Both = 1,
            LeftOnly = 2,
            RightOnly = 3,
            None = 4
        }
        private enum eSoundButtonStates
        {
            NotSetYet = 0,
            SoundOn = 1,
            SoundOff = 2
        }
        private enum eSwapButtonStates
        {
            NotSetYet = 0,
            SwapOn = 1,
            SwapOff = 2
        }
        private enum eVerticalButtonStates
        {
            NotSetYet = 0,
            VerticalOn = 1,
            VerticalOff = 2
        }
        #endregion

        #region Variables
        private eMainStates mMainState = eMainStates.VideoFileNotOpened;
        private eMainStates mLastMainState = eMainStates.NotSetYet;
        private eEnableDisableStates mEnableDisableState = eEnableDisableStates.VideoFileNotOpened;
        private eEnableDisableStates mLastEnableDisableState = eEnableDisableStates.NotSetYet;
        private ePlayerButtonsStates mPlayerButtonsState = ePlayerButtonsStates.Stopped;
        private ePlayerButtonsStates mLastPlayerButtonsState = ePlayerButtonsStates.NotSetYet;
        private eStereoButtonsStates mStereoButtonsState = eStereoButtonsStates.Both;
        private eStereoButtonsStates mLastStereoButtonsState = eStereoButtonsStates.NotSetYet;
        private eSoundButtonStates mSoundButtonState = eSoundButtonStates.SoundOn;
        private eSoundButtonStates mLastSoundButtonState = eSoundButtonStates.NotSetYet;
        private eSwapButtonStates mSwapButtonState = eSwapButtonStates.SwapOff;
        private eSwapButtonStates mLastSwapButtonState = eSwapButtonStates.NotSetYet;
        private eVerticalButtonStates mVerticalButtonState = eVerticalButtonStates.VerticalOff;
        private eVerticalButtonStates mLastVerticalButtonState = eVerticalButtonStates.NotSetYet;
        private bool mAlreadySent = true;
        private int mAlreadySentCounter = 0;
        private long mLastSWMovieTime = 0;
        private bool mSeekAlreadyApplied = true;
        private long mLastWindowResizeTime = 0;
        private bool mResizeAlreadyApplied = true;
        private bool mIsEOFApplied = false;
        #endregion

        #region Initialize
        private void InitStereoPlayer()
        {
            timerFrequency.Start();
            timerSendSettings.Start();
            timerGUIStereoPlayer.Start();
            mMainState = eMainStates.VideoFileNotOpened;
        }
        #endregion

        #region Menu Events
        #endregion

        #region Main Form Events
        private void MainFrm_DragEnter(object sender, DragEventArgs e)
        {
            Array data = ((IDataObject)e.Data).GetData("FileDrop") as Array;
            if (data != null)
            {
                if ((data.Length == 1) && (data.GetValue(0) is String))
                {
                    string filepath = ((string[])data)[0];
                    string ext = Path.GetExtension(filepath).ToLower();
                    if ((ext == ".mkv") || (ext == ".mp4") || (ext == ".wmv") || (ext == ".avi"))
                    {
                        e.Effect = DragDropEffects.Copy; //Will show small plus (+)
                    }
                    else
                    {
                        e.Effect = DragDropEffects.None; //Will show small prohibition symbol.
                    }
                }
                else
                {
                    e.Effect = DragDropEffects.None; //Will show small prohibition symbol.
                }
            }
        }
        private void MainFrm_DragDrop(object sender, DragEventArgs e)
        {
            Array data = ((IDataObject)e.Data).GetData("FileDrop") as Array;
            if (data != null)
            {
                if ((data.Length == 1) && (data.GetValue(0) is String))
                {
                    string filepath = ((string[])data)[0];
                    string ext = Path.GetExtension(filepath).ToLower();
                    if ((ext == ".mkv") || (ext == ".mp4") || (ext == ".wmv") || (ext == ".avi"))
                    {
                        if (Settings.FilePath != filepath)
                        {
                            Settings.FilePath = filepath;
                            mMainState = (cbComPort.SelectedIndex >= 0) ? eMainStates.Stopped : eMainStates.COMPortNotSelected;
                            if (mStereoVideoManager != null)
                            {
                                if (mStereoVideoManager.PlayerIsOpened())
                                {
                                    mStereoVideoManager.PlayerClose();
                                }
                                mStereoVideoManager.PlayerOpen(Settings.FilePath);
                                Int64 duration = mStereoVideoManager.PlayerGetDuration() / 1000;
                                tbMovieTime.Value = 0;
                                tbMovieTime.Maximum = (int)duration;
                            }
                            LoadTracks();
                        }
                    }
                }
            }
        }
        private void timerGUIStereoPlayer_Tick(object sender, EventArgs e)
        {
            if (File.Exists(Settings.FilePath))
            {
                if (mStereoVideoManager != null)
                {
                    if (!mStereoVideoManager.PlayerIsOpened())
                    {
                        mStereoVideoManager.PlayerOpen(Settings.FilePath);
                        Int64 duration = mStereoVideoManager.PlayerGetDuration() / 1000;
                        tbMovieTime.Maximum = (int)duration;
                        LoadTracks();
                        mMainState = (cbComPort.SelectedIndex >= 0) ? eMainStates.Stopped : eMainStates.COMPortNotSelected;
                    }
                }
            }
            else
            {
                mMainState = eMainStates.VideoFileNotOpened;
            }
            //-----------------------------------------------------
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    if (mStereoVideoManager.PlayerIsEOF())
                    {
                        if (!mIsEOFApplied)
                        {
                            mMainState = eMainStates.PlayingPaused;
                            mPlayerButtonsState = ePlayerButtonsStates.Paused;
                            mIsEOFApplied = true;
                        }
                    }
                    else
                    {
                        mIsEOFApplied = false;
                    }
                }
            }
            //-----------------------------------------------------
            if (mLastMainState != mMainState)
            {
                SetMainState(mMainState);
                mLastMainState = mMainState;
            }
            //-----------------------------------------------------
            if (mLastPlayerButtonsState != mPlayerButtonsState)
            {
                SetPlayerButtonsState(mPlayerButtonsState);
                if (mStereoVideoManager != null)
                {
                    if (mStereoVideoManager.PlayerIsOpened())
                    {
                        switch (mPlayerButtonsState)
                        {
                            case ePlayerButtonsStates.Playing:
                                mStereoVideoManager.PlayerPlay();
                                PerformStereoStart();
                                break;
                            case ePlayerButtonsStates.Paused:
                                mStereoVideoManager.PlayerPause();
                                break;
                            case ePlayerButtonsStates.Stopped:
                                mStereoVideoManager.PlayerStop();
                                PerformStereoStop();
                                break;
                        }
                    }
                }
                mLastPlayerButtonsState = mPlayerButtonsState;
            }
            //-----------------------------------------------------
            if (mLastStereoButtonsState != mStereoButtonsState)
            {
                SetStereoButtonsState(mStereoButtonsState);
                if (mStereoVideoManager != null)
                {
                    switch (mStereoButtonsState)
                    {
                        case eStereoButtonsStates.Both:
                            mStereoVideoManager.StereoLRBoth(0);
                            break;
                        case eStereoButtonsStates.LeftOnly:
                            mStereoVideoManager.StereoLRBoth(1);
                            break;
                        case eStereoButtonsStates.RightOnly:
                            mStereoVideoManager.StereoLRBoth(2);
                            break;
                    }
                }
                mLastStereoButtonsState = mStereoButtonsState;
            }
            //-----------------------------------------------------
            if (mLastSoundButtonState != mSoundButtonState)
            {
                SetSoundButtonState(mSoundButtonState);
                if (mStereoVideoManager != null)
                {
                    if (mStereoVideoManager.PlayerIsOpened())
                    {
                        switch (mSoundButtonState)
                        {
                            case eSoundButtonStates.SoundOn:
                                mStereoVideoManager.PlayerMute(false);
                                break;
                            case eSoundButtonStates.SoundOff:
                                mStereoVideoManager.PlayerMute(true);
                                break;
                        }
                    }
                }
                mLastSoundButtonState = mSoundButtonState;
            }
            //-----------------------------------------------------
            if (mLastSwapButtonState != mSwapButtonState)
            {
                SetSwapButtonState(mSwapButtonState);
                if (mStereoVideoManager != null)
                {
                    switch (mSwapButtonState)
                    {
                        case eSwapButtonStates.SwapOn:
                            mStereoVideoManager.StereoSwapLR(true);
                            break;
                        case eSwapButtonStates.SwapOff:
                            mStereoVideoManager.StereoSwapLR(false);
                            break;
                    }
                }
                mLastSwapButtonState = mSwapButtonState;
            }
            //-----------------------------------------------------
            if (mLastVerticalButtonState != mVerticalButtonState)
            {
                SetVerticalButtonState(mVerticalButtonState);
                if (mStereoVideoManager != null)
                {
                    switch (mVerticalButtonState)
                    {
                        case eVerticalButtonStates.VerticalOn:
                            mStereoVideoManager.StereoVerticalLR(true);
                            break;
                        case eVerticalButtonStates.VerticalOff:
                            mStereoVideoManager.StereoVerticalLR(false);
                            break;
                    }
                }
                mLastVerticalButtonState = mVerticalButtonState;
            }
            //-----------------------------------------------------
            if (mLastEnableDisableState != mEnableDisableState)
            {
                SetEnableDisableState(mEnableDisableState);
                mLastEnableDisableState = mEnableDisableState;
            }
            //-----------------------------------------------------
            if (mStereoVideoManager != null)
            {
                Int64 currentPlayingTime = mStereoVideoManager.PlayerGetCurrentPlayingTime() / 1000;
                int hours = (int)currentPlayingTime / 3600;
                int minutes = (int)currentPlayingTime / 60 % 60;
                int seconds = (int)currentPlayingTime % 60;
                lblMovieTime.Text = $"{hours:D2}:{minutes:D2}:{seconds:D2}";
            }
            //-----------------------------------------------------
            if (((double)(Stopwatch.GetTimestamp() - mLastSWMovieTime)) / ((double)Stopwatch.Frequency / 1000.0) > 400)
            {
                if (mStereoVideoManager != null)
                {
                    Int64 currentPlayingTime = mStereoVideoManager.PlayerGetCurrentPlayingTime() / 1000;
                    if ((int)currentPlayingTime > tbMovieTime.Maximum) currentPlayingTime = 0;
                    tbMovieTime.Value = (int)currentPlayingTime;
                }
            }
            //-----------------------------------------------------
            if (((double)(Stopwatch.GetTimestamp() - mLastSWMovieTime)) / ((double)Stopwatch.Frequency / 1000.0) > 200)
            {
                if (!mSeekAlreadyApplied)
                {
                    if (mStereoVideoManager != null)
                    {
                        if (mStereoVideoManager.PlayerIsOpened())
                        {
                            mStereoVideoManager.PlayerSeek(tbMovieTime.Value * 1000);
                            PerformStereoStart();
                            if (mPlayerButtonsState == ePlayerButtonsStates.Stopped) mPlayerButtonsState = ePlayerButtonsStates.Paused;
                        }
                    }
                    mSeekAlreadyApplied = true;
                }
            }
            //-----------------------------------------------------
            if (((double)(Stopwatch.GetTimestamp() - mLastWindowResizeTime)) / ((double)Stopwatch.Frequency / 1000.0) > 200)
            {
                if (!mResizeAlreadyApplied)
                {
                    if (mStereoVideoManager != null)
                    {
                        mStereoVideoManager.StereoWindowSizeChanged();
                    }
                    mResizeAlreadyApplied = true;
                }
            }
        }
        private void cbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.ComPort = (string)cbComPort.SelectedItem;
            mMainState = eMainStates.Stopped;
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
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            int frequencyInHz = 0;
            if (mStereoVideoManager != null)
            {
                frequencyInHz = mStereoVideoManager.StereoGetFrequency();
            }
            tsslFrequency.Text = frequencyInHz.ToString() + "Hz";
        }
        private void timerSendSettings_Tick(object sender, EventArgs e)
        {
            if (!mAlreadySent)
            {
                if (mAlreadySentCounter > 5)
                {
                    if (mStereoVideoManager != null)
                    {
                        mStereoVideoManager.PlayerMute(!Settings.SoundOn);
                        mStereoVideoManager.PlayerSetVolume((UInt16)tbVolume.Value);
                        mStereoVideoManager.StereoLRBoth(Settings.LRBoth);
                        mStereoVideoManager.StereoSwapLR(Settings.SwapLR);
                        mStereoVideoManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
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
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
            }
        }
        private void bPlayPause_Click(object sender, EventArgs e)
        {
            mMainState = eMainStates.PlayingPaused;
            if (mPlayerButtonsState == ePlayerButtonsStates.Playing)
            {
                mPlayerButtonsState = ePlayerButtonsStates.Paused;
            }
            else if (mPlayerButtonsState == ePlayerButtonsStates.Paused)
            {
                mIsEOFApplied = false;
                mPlayerButtonsState = ePlayerButtonsStates.Playing;
            }
            else if (mPlayerButtonsState == ePlayerButtonsStates.Stopped)
            {
                mPlayerButtonsState = ePlayerButtonsStates.Playing;
            }
        }
        private void bStop_Click(object sender, EventArgs e)
        {
            mMainState = eMainStates.Stopped;
        }
        private void bSound_Click(object sender, EventArgs e)
        {
            if (mSoundButtonState == eSoundButtonStates.SoundOn)
            {
                mSoundButtonState = eSoundButtonStates.SoundOff;
                Settings.SoundOn = false;
            }
            else if (mSoundButtonState == eSoundButtonStates.SoundOff)
            {
                mSoundButtonState = eSoundButtonStates.SoundOn;
                Settings.SoundOn = true;
            }
        }
        private void tbVolume_Scroll(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.PlayerSetVolume((UInt16)tbVolume.Value);
                Settings.Volume = tbVolume.Value;
            }
        }
        private void bBoth_Click(object sender, EventArgs e)
        {
            mStereoButtonsState = eStereoButtonsStates.Both;
            Settings.LRBoth = 0;
        }
        private void bLeftOnly_Click(object sender, EventArgs e)
        {
            mStereoButtonsState = eStereoButtonsStates.LeftOnly;
            Settings.LRBoth = 1;
        }
        private void bRightOnly_Click(object sender, EventArgs e)
        {
            mStereoButtonsState = eStereoButtonsStates.RightOnly;
            Settings.LRBoth = 2;
        }
        private void bSwap_Click(object sender, EventArgs e)
        {
            if (mSwapButtonState == eSwapButtonStates.SwapOn)
            {
                mSwapButtonState = eSwapButtonStates.SwapOff;
                Settings.SwapLR = false;
            }
            else if (mSwapButtonState == eSwapButtonStates.SwapOff)
            {
                mSwapButtonState = eSwapButtonStates.SwapOn;
                Settings.SwapLR = true;
            }
        }
        private void bVertical_Click(object sender, EventArgs e)
        {
            if (mVerticalButtonState == eVerticalButtonStates.VerticalOn)
            {
                mVerticalButtonState = eVerticalButtonStates.VerticalOff;
                Settings.VerticalLR = false;
            }
            else if (mVerticalButtonState == eVerticalButtonStates.VerticalOff)
            {
                mVerticalButtonState = eVerticalButtonStates.VerticalOn;
                Settings.VerticalLR = true;
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
                    mMainState = (cbComPort.SelectedIndex >= 0) ? eMainStates.Stopped : eMainStates.COMPortNotSelected;
                    if (mStereoVideoManager != null)
                    {
                        if (mStereoVideoManager.PlayerIsOpened())
                        {
                            mStereoVideoManager.PlayerClose();
                        }
                        mStereoVideoManager.PlayerOpen(Settings.FilePath);
                        Int64 duration = mStereoVideoManager.PlayerGetDuration() / 1000;
                        tbMovieTime.Value = 0;
                        tbMovieTime.Maximum = (int)duration;
                    }
                    LoadTracks();
                }
                else
                {
                    mMainState = eMainStates.VideoFileNotOpened;
                }
            }
        }
        private void cbTracks_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerSetAudioTrack(cbTracks.SelectedIndex);
                }
            }
        }
        private void tbMovieTime_Scroll(object sender, EventArgs e)
        {
            if (mMainState == eMainStates.Stopped)
            {
                mMainState = eMainStates.PlayingPaused;
                mPlayerButtonsState = ePlayerButtonsStates.Paused;
            }
            mLastSWMovieTime = Stopwatch.GetTimestamp();
            mSeekAlreadyApplied = false;
        }
        private void pbVideoPanel_SizeChanged(object sender, EventArgs e)
        {
            mLastWindowResizeTime = Stopwatch.GetTimestamp();
            mResizeAlreadyApplied = false;
        }
        private void tbMovieTime_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Rewind or go forward in the movie", (IWin32Window)sender, 5000);
        }
        private void bOpen_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Open video file", (IWin32Window)sender, 5000);
        }
        private void bPlayPause_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Play / pause the video", (IWin32Window)sender, 5000);
        }
        private void bStop_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Stop the video", (IWin32Window)sender, 5000);
        }
        private void bSound_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Sound On / Off", (IWin32Window)sender, 5000);
        }
        private void tbVolume_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Change the volume", (IWin32Window)sender, 5000);
        }
        private void cbTracks_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Select audio track", (IWin32Window)sender, 5000);
        }
        private void cbComPort_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Select COM Port of Arduino glasses controller", (IWin32Window)sender, 5000);
        }
        private void bRefresh_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Refresh COM Ports", (IWin32Window)sender, 5000);
        }
        private void bBoth_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Show pictures for both eyes", (IWin32Window)sender, 5000);
        }
        private void bLeftOnly_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Show picture for left eye only", (IWin32Window)sender, 5000);
        }
        private void bRightOnly_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Show picture for right eye only", (IWin32Window)sender, 5000);
        }
        private void bSwap_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Swap left eye and right eye pictures", (IWin32Window)sender, 5000);
        }
        private void bVertical_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("In original video the left picture is above the right picture or near it", (IWin32Window)sender, 5000);
        }
        private void tbGlassesTimeOffset_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Fine correction for the stereo effect", (IWin32Window)sender, 5000);
        }
        #endregion

        #region Methods
        private void SetMainState(eMainStates mainState)
        {
            switch (mainState)
            {
                case eMainStates.VideoFileNotOpened:
                    mEnableDisableState = eEnableDisableStates.VideoFileNotOpened;
                    mPlayerButtonsState = ePlayerButtonsStates.None;
                    switch (Settings.LRBoth)
                    {
                        case 0:
                            mStereoButtonsState = eStereoButtonsStates.Both;
                            break;
                        case 1:
                            mStereoButtonsState = eStereoButtonsStates.LeftOnly;
                            break;
                        case 2:
                            mStereoButtonsState = eStereoButtonsStates.RightOnly;
                            break;
                    }
                    mSwapButtonState = (Settings.SwapLR) ? eSwapButtonStates.SwapOn : eSwapButtonStates.SwapOff;
                    mVerticalButtonState = (Settings.VerticalLR) ? eVerticalButtonStates.VerticalOn : eVerticalButtonStates.VerticalOff;
                    mSoundButtonState = (Settings.SoundOn) ? eSoundButtonStates.SoundOn : eSoundButtonStates.SoundOff;
                    tbVolume.Value = Settings.Volume;
                    tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
                    lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
                    break;
                case eMainStates.COMPortNotSelected:
                    mEnableDisableState = eEnableDisableStates.COMPortNotSelected;
                    mPlayerButtonsState = ePlayerButtonsStates.None;
                    switch (Settings.LRBoth)
                    {
                        case 0:
                            mStereoButtonsState = eStereoButtonsStates.Both;
                            break;
                        case 1:
                            mStereoButtonsState = eStereoButtonsStates.LeftOnly;
                            break;
                        case 2:
                            mStereoButtonsState = eStereoButtonsStates.RightOnly;
                            break;
                    }
                    mSwapButtonState = (Settings.SwapLR) ? eSwapButtonStates.SwapOn : eSwapButtonStates.SwapOff;
                    mVerticalButtonState = (Settings.VerticalLR) ? eVerticalButtonStates.VerticalOn : eVerticalButtonStates.VerticalOff;
                    mSoundButtonState = (Settings.SoundOn) ? eSoundButtonStates.SoundOn : eSoundButtonStates.SoundOff;
                    tbVolume.Value = Settings.Volume;
                    tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
                    lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
                    break;
                case eMainStates.Stopped:
                    mEnableDisableState = eEnableDisableStates.Stopped;
                    mPlayerButtonsState = ePlayerButtonsStates.Stopped;
                    switch (Settings.LRBoth)
                    {
                        case 0:
                            mStereoButtonsState = eStereoButtonsStates.Both;
                            break;
                        case 1:
                            mStereoButtonsState = eStereoButtonsStates.LeftOnly;
                            break;
                        case 2:
                            mStereoButtonsState = eStereoButtonsStates.RightOnly;
                            break;
                    }
                    mSwapButtonState = (Settings.SwapLR) ? eSwapButtonStates.SwapOn : eSwapButtonStates.SwapOff;
                    mVerticalButtonState = (Settings.VerticalLR) ? eVerticalButtonStates.VerticalOn : eVerticalButtonStates.VerticalOff;
                    mSoundButtonState = (Settings.SoundOn) ? eSoundButtonStates.SoundOn : eSoundButtonStates.SoundOff;
                    tbVolume.Value = Settings.Volume;
                    tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
                    lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
                    break;
                case eMainStates.PlayingPaused:
                    mEnableDisableState = eEnableDisableStates.PlayingPaused;
                    switch (Settings.LRBoth)
                    {
                        case 0:
                            mStereoButtonsState = eStereoButtonsStates.Both;
                            break;
                        case 1:
                            mStereoButtonsState = eStereoButtonsStates.LeftOnly;
                            break;
                        case 2:
                            mStereoButtonsState = eStereoButtonsStates.RightOnly;
                            break;
                    }
                    mSwapButtonState = (Settings.SwapLR) ? eSwapButtonStates.SwapOn : eSwapButtonStates.SwapOff;
                    mVerticalButtonState = (Settings.VerticalLR) ? eVerticalButtonStates.VerticalOn : eVerticalButtonStates.VerticalOff;
                    mSoundButtonState = (Settings.SoundOn) ? eSoundButtonStates.SoundOn : eSoundButtonStates.SoundOff;
                    tbVolume.Value = Settings.Volume;
                    tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
                    lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
                    break;
            }
        }
        private void SetEnableDisableState(eEnableDisableStates enableDisableState)
        {
            switch (enableDisableState)
            {
                case eEnableDisableStates.VideoFileNotOpened:
                    bOpen.Enabled = true;
                    bPlayPause.Enabled = false;
                    bStop.Enabled = false;
                    bSound.Enabled = false;
                    tbVolume.Enabled = false;
                    cbTracks.Enabled = false;
                    cbComPort.Enabled = false;
                    bRefresh.Enabled = false;
                    bBoth.Enabled = false;
                    bLeftOnly.Enabled = false;
                    bRightOnly.Enabled = false;
                    bSwap.Enabled = false;
                    bVertical.Enabled = false;
                    tbMovieTime.Enabled = false;
                    lblMovieTime.Enabled = false;
                    tbGlassesTimeOffset.Enabled = false;
                    lblGlassesTimeOffset.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eEnableDisableStates.COMPortNotSelected:
                    bOpen.Enabled = true;
                    bPlayPause.Enabled = false;
                    bStop.Enabled = false;
                    bSound.Enabled = false;
                    tbVolume.Enabled = false;
                    cbTracks.Enabled = false;
                    cbComPort.Enabled = true;
                    bRefresh.Enabled = true;
                    bBoth.Enabled = false;
                    bLeftOnly.Enabled = false;
                    bRightOnly.Enabled = false;
                    bSwap.Enabled = false;
                    bVertical.Enabled = false;
                    tbMovieTime.Enabled = false;
                    lblMovieTime.Enabled = false;
                    tbGlassesTimeOffset.Enabled = false;
                    lblGlassesTimeOffset.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eEnableDisableStates.Stopped:
                    bOpen.Enabled = true;
                    bPlayPause.Enabled = true;
                    bStop.Enabled = true;
                    bSound.Enabled = true;
                    tbVolume.Enabled = true;
                    cbTracks.Enabled = true;
                    cbComPort.Enabled = true;
                    bRefresh.Enabled = true;
                    bBoth.Enabled = false;
                    bLeftOnly.Enabled = false;
                    bRightOnly.Enabled = false;
                    bSwap.Enabled = false;
                    bVertical.Enabled = false;
                    tbMovieTime.Enabled = true;
                    lblMovieTime.Enabled = true;
                    tbGlassesTimeOffset.Enabled = false;
                    lblGlassesTimeOffset.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eEnableDisableStates.PlayingPaused:
                    bOpen.Enabled = true;
                    bPlayPause.Enabled = true;
                    bStop.Enabled = true;
                    bSound.Enabled = true;
                    tbVolume.Enabled = true;
                    cbTracks.Enabled = true;
                    cbComPort.Enabled = false;
                    bRefresh.Enabled = false;
                    bBoth.Enabled = true;
                    bLeftOnly.Enabled = true;
                    bRightOnly.Enabled = true;
                    bSwap.Enabled = true;
                    bVertical.Enabled = true;
                    tbMovieTime.Enabled = true;
                    lblMovieTime.Enabled = true;
                    tbGlassesTimeOffset.Enabled = true;
                    lblGlassesTimeOffset.Enabled = true;
                    tsslFrequencyLabel.Visible = true;
                    tsslFrequency.Visible = true;
                    break;
            }
        }
        private void SetPlayerButtonsState(ePlayerButtonsStates playerButtonsState)
        {
            switch (playerButtonsState)
            {
                case ePlayerButtonsStates.Playing:
                    bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.apause;
                    break;
                case ePlayerButtonsStates.Paused:
                    bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
                    break;
                case ePlayerButtonsStates.Stopped:
                    bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
                    break;
                case ePlayerButtonsStates.None:
                    bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
                    break;
            }
        }
        private void SetStereoButtonsState(eStereoButtonsStates stereoButtonsState)
        {
            switch (stereoButtonsState)
            {
                case eStereoButtonsStates.Both:
                    bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_selected;
                    bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
                    bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
                    break;
                case eStereoButtonsStates.LeftOnly:
                    bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
                    bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_selected;
                    bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
                    break;
                case eStereoButtonsStates.RightOnly:
                    bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
                    bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
                    bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_selected;
                    break;
                case eStereoButtonsStates.None:
                    bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
                    bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
                    bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
                    break;
            }
        }
        private void SetSoundButtonState(eSoundButtonStates soundButtonStates)
        {
            switch (soundButtonStates)
            {
                case eSoundButtonStates.SoundOn:
                    bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.sound;
                    tbVolume.Enabled = true;
                    break;
                case eSoundButtonStates.SoundOff:
                    bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.no_sound;
                    tbVolume.Enabled = false;
                    break;
            }
        }
        private void SetSwapButtonState(eSwapButtonStates swapButtonStates)
        {
            switch (swapButtonStates)
            {
                case eSwapButtonStates.SwapOn:
                    bSwap.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.swap_selected;
                    break;
                case eSwapButtonStates.SwapOff:
                    bSwap.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.swap_unselected;
                    break;
            }
        }
        private void SetVerticalButtonState(eVerticalButtonStates verticalButtonStates)
        {
            switch (verticalButtonStates)
            {
                case eVerticalButtonStates.VerticalOn:
                    bVertical.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.vertical;
                    break;
                case eVerticalButtonStates.VerticalOff:
                    bVertical.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.horizontal;
                    break;
            }
        }
        private void PerformStereoStart()
        {
            if (mStereoVideoManager != null)
            {
                bool isStereoStarted = false;
                isStereoStarted = mStereoVideoManager.StereoIsStarted();
                //To Be Started here.
                if (!isStereoStarted)
                {
                    StereoStart();
                    mAlreadySent = false;
                    mAlreadySentCounter = 0;
                }
            }
        }
        private void PerformStereoStop()
        {
            if (mStereoVideoManager != null)
            {
                bool isStarted = false;
                isStarted = mStereoVideoManager.StereoIsStarted();
                //To Be Stopped here.
                if (isStarted)
                {
                    StereoStop();
                    pbVideoPanel.Refresh();
                }
            }
        }
        private void StereoStart()
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoSetCOMPort(Settings.ComPort);
                mStereoVideoManager.StereoStart();
            }
        }
        private void StereoStop()
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoStop();
            }
        }
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
        private void LoadTracks()
        {
            cbTracks.Items.Clear();
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    int numberOfAudioTracks = mStereoVideoManager.PlayerGetNumberOfAudioTracks();
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