using System;
using System.IO;
using System.Diagnostics;
using System.Security.Permissions;
using System.Windows.Forms;
using static StereoscopicMoviePlayer.StereoButtonsPanel;

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
        #endregion

        #region Variables
        private bool mAlreadySent = true;
        private int mAlreadySentCounter = 0;
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
            SetMainState(eMainStates.VideoFileNotOpened);
            LoadSettingsToControls();
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
                            SetMainState((cppComPortPanel.IsCOMPortSelected) ? eMainStates.Stopped : eMainStates.COMPortNotSelected);
                            if (mStereoVideoManager != null)
                            {
                                if (mStereoVideoManager.PlayerIsOpened())
                                {
                                    mStereoVideoManager.PlayerClose();
                                }
                                mStereoVideoManager.PlayerOpen(Settings.FilePath);
                                ptpPlayTimePanel.CurrentPlayingTime = 0;
                                ptpPlayTimePanel.Duration = mStereoVideoManager.PlayerGetDuration();
                                spSoundPanel.LoadSoundTracks(mStereoVideoManager.PlayerGetNumberOfAudioTracks());
                            }
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
                        ptpPlayTimePanel.Duration = mStereoVideoManager.PlayerGetDuration();
                        spSoundPanel.LoadSoundTracks(mStereoVideoManager.PlayerGetNumberOfAudioTracks());
                        SetMainState((cppComPortPanel.IsCOMPortSelected) ? eMainStates.Stopped : eMainStates.COMPortNotSelected);
                    }
                }
            }
            else
            {
                SetMainState(eMainStates.VideoFileNotOpened);
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
                            SetMainState(eMainStates.PlayingPaused);
                            ppPlayerPanel.ButtonsState = PlayerPanel.eButtonsStates.Paused;
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
                        mStereoVideoManager.PlayerSetVolume(spSoundPanel.SoundVolume);
                        mStereoVideoManager.StereoLRBoth(Settings.LRBoth);
                        mStereoVideoManager.StereoSwapLR(Settings.SwapLR);
                        mStereoVideoManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
                    }
                    mAlreadySent = true;
                }
                mAlreadySentCounter++;
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
                    SetMainState((cppComPortPanel.IsCOMPortSelected) ? eMainStates.Stopped : eMainStates.COMPortNotSelected);
                    if (mStereoVideoManager != null)
                    {
                        if (mStereoVideoManager.PlayerIsOpened())
                        {
                            mStereoVideoManager.PlayerClose();
                        }
                        mStereoVideoManager.PlayerOpen(Settings.FilePath);
                        ptpPlayTimePanel.CurrentPlayingTime = 0;
                        ptpPlayTimePanel.Duration = mStereoVideoManager.PlayerGetDuration();
                        spSoundPanel.LoadSoundTracks(mStereoVideoManager.PlayerGetNumberOfAudioTracks());
                    }
                }
                else
                {
                    SetMainState(eMainStates.VideoFileNotOpened);
                }
            }
        }
        private void pbVideoPanel_SizeChanged(object sender, EventArgs e)
        {
            mLastWindowResizeTime = Stopwatch.GetTimestamp();
            mResizeAlreadyApplied = false;
        }
        private void ptpPlayTimePanel_OnSeek(object sender, PlayTimePanel.SeekEventArgs e)
        {
            if (ppPlayerPanel.ButtonsState == PlayerPanel.eButtonsStates.Stopped)
            {
                SetMainState(eMainStates.PlayingPaused);
                ppPlayerPanel.ButtonsState = PlayerPanel.eButtonsStates.Paused;
            }
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerSeek(e.Seek);
                    PerformStereoStart();
                    if (ppPlayerPanel.ButtonsState == PlayerPanel.eButtonsStates.Stopped) ppPlayerPanel.ButtonsState = PlayerPanel.eButtonsStates.Paused;
                }
            }
        }
        private void ptpPlayTimePanel_OnValueUpdateRequest(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    ptpPlayTimePanel.CurrentPlayingTime = mStereoVideoManager.PlayerGetCurrentPlayingTime();
                }
            }
        }
        private void ppPlayerPanel_OnPause(object sender, EventArgs e)
        {
            SetMainState(eMainStates.PlayingPaused);
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerPause();
                }
            }
        }
        private void ppPlayerPanel_OnPlay(object sender, EventArgs e)
        {
            SetMainState(eMainStates.PlayingPaused);
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerPlay();
                    PerformStereoStart();
                }
            }
        }
        private void ppPlayerPanel_OnStop(object sender, EventArgs e)
        {
            SetMainState(eMainStates.Stopped);
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerStop();
                    PerformStereoStop();
                }
            }
        }
        private void ppPlayerPanel_OnPlayAfterPause(object sender, EventArgs e)
        {
            mIsEOFApplied = false;
        }
        private void spSoundPanel_OnSoundOff(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerMute(true);
                    Settings.SoundOn = false;
                }
            }
        }
        private void spSoundPanel_OnSoundOn(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerMute(false);
                    Settings.SoundOn = true;
                }
            }
        }
        private void spSoundPanel_OnSoundVolumeChanged(object sender, SoundPanel.SoundVolumeEventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.PlayerSetVolume(e.SoundVolume);
                Settings.Volume = e.SoundVolume;
            }
        }
        private void spSoundPanel_OnSelectedSoundTrackChanged(object sender, SoundPanel.SelectedSoundTrackEventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                if (mStereoVideoManager.PlayerIsOpened())
                {
                    mStereoVideoManager.PlayerSetAudioTrack(e.SelectedSoundTrack);
                }
            }
        }
        private void cppComPortPanel_OnCOMPortSelected(object sender, COMPortPanel.COMPortEventArgs e)
        {
            Settings.ComPort = e.COMPort;
            SetMainState(eMainStates.Stopped);
        }
        private void sbpStereoButtonsPanel_OnBoth(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoLRBoth(0);
                Settings.LRBoth = 0;
            }
        }
        private void sbpStereoButtonsPanel_OnLeftOnly(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoLRBoth(1);
                Settings.LRBoth = 1;
            }
        }
        private void sbpStereoButtonsPanel_OnRightOnly(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoLRBoth(2);
                Settings.LRBoth = 2;
            }
        }
        private void sbpStereoButtonsPanel_OnNoSwap(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoSwapLR(false);
                Settings.SwapLR = false;
            }
        }
        private void sbpStereoButtonsPanel_OnSwap(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoSwapLR(true);
                Settings.SwapLR = true;
            }
        }
        private void sbpStereoButtonsPanel_OnHorizontal(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoVerticalLR(false);
                Settings.VerticalLR = false;
            }
        }
        private void sbpStereoButtonsPanel_OnVertical(object sender, EventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoVerticalLR(true);
                Settings.VerticalLR = true;
            }
        }
        private void gtopGlassesTimeOffsetPanel_OnTimeOffset(object sender, GlassesTimeOffsetPanel.TimeOffsetEventArgs e)
        {
            if (mStereoVideoManager != null)
            {
                mStereoVideoManager.StereoSetGlassesTimeOffset(e.TimeOffset);
                Settings.GlassesTimeOffset = e.TimeOffset;
            }
        }
        private void bOpen_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Open video file", (IWin32Window)sender, 5000);
        }
        #endregion

        #region Methods
        private void SetMainState(eMainStates mainState)
        {
            switch (mainState)
            {
                case eMainStates.VideoFileNotOpened:
                    bOpen.Enabled = true;
                    ppPlayerPanel.Enabled = false;
                    spSoundPanel.Enabled = false;
                    cppComPortPanel.Enabled = false;
                    sbpStereoButtonsPanel.Enabled = false;
                    ptpPlayTimePanel.Enabled = false;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.COMPortNotSelected:
                    bOpen.Enabled = true;
                    ppPlayerPanel.Enabled = false;
                    spSoundPanel.Enabled = false;
                    cppComPortPanel.Enabled = true;
                    sbpStereoButtonsPanel.Enabled = false;
                    ptpPlayTimePanel.Enabled = false;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.Stopped:
                    bOpen.Enabled = true;
                    ppPlayerPanel.Enabled = true;
                    spSoundPanel.Enabled = true;
                    cppComPortPanel.Enabled = true;
                    sbpStereoButtonsPanel.Enabled = false;
                    ptpPlayTimePanel.Enabled = true;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.PlayingPaused:
                    bOpen.Enabled = true;
                    ppPlayerPanel.Enabled = true;
                    spSoundPanel.Enabled = true;
                    cppComPortPanel.Enabled = false;
                    sbpStereoButtonsPanel.Enabled = true;
                    ptpPlayTimePanel.Enabled = true;
                    gtopGlassesTimeOffsetPanel.Enabled = true;
                    tsslFrequencyLabel.Visible = true;
                    tsslFrequency.Visible = true;
                    break;
            }

        }
        private void LoadSettingsToControls()
        {
            spSoundPanel.SoundState = Settings.SoundOn ? SoundPanel.eSoundStates.SoundOn : SoundPanel.eSoundStates.SoundOff;
            spSoundPanel.SoundVolume = (UInt16)Settings.Volume;
            cppComPortPanel.SelectedCOMPort = Settings.ComPort;
            cppComPortPanel.LoadPorts();
            switch (Settings.LRBoth)
            {
                case 0:
                    sbpStereoButtonsPanel.LRBothButtonsState = eLRBothButtonsStates.Both;
                    break;
                case 1:
                    sbpStereoButtonsPanel.LRBothButtonsState = eLRBothButtonsStates.LeftOnly;
                    break;
                case 2:
                    sbpStereoButtonsPanel.LRBothButtonsState = eLRBothButtonsStates.RightOnly;
                    break;
            }
            sbpStereoButtonsPanel.SwapButtonState = (Settings.SwapLR) ? eSwapButtonStates.SwapOn : eSwapButtonStates.SwapOff;
            sbpStereoButtonsPanel.HVButtonState = (Settings.VerticalLR) ? eHVButtonStates.Vertical : eHVButtonStates.Horizontal;
            gtopGlassesTimeOffsetPanel.TimeOffset = Settings.GlassesTimeOffset;
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
        #endregion

        #endregion
    }
}