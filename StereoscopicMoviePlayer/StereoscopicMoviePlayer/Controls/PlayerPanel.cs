using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    public partial class PlayerPanel : UserControl
    {
        #region Enums
        public enum eButtonsStates
        {
            Playing,
            Paused,
            Stopped
        }
        #endregion

        #region Variables
        private eButtonsStates mButtonsState = eButtonsStates.Stopped;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eButtonsStates ButtonsState
        {
            get
            {
                return mButtonsState;
            }
            set
            {
                eButtonsStates previousButtonsState = mButtonsState;
                mButtonsState = value;
                switch (mButtonsState)
                {
                    case eButtonsStates.Playing:
                        bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.apause;
                        if (previousButtonsState == eButtonsStates.Paused) OnPlayAfterPause?.Invoke(this, EventArgs.Empty);
                        OnPlay?.Invoke(this, EventArgs.Empty);
                        break;
                    case eButtonsStates.Paused:
                        bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
                        OnPause?.Invoke(this, EventArgs.Empty);
                        break;
                    case eButtonsStates.Stopped:
                        bPlayPause.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.aplay;
                        OnStop?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnPlay;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnPlayAfterPause;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnPause;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnStop;
        #endregion

        #region Constructor
        public PlayerPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        #endregion

        #region Event Methods
        private void bPlayPause_Click(object sender, EventArgs e)
        {
            if (ButtonsState == eButtonsStates.Stopped)
            {
                ButtonsState = eButtonsStates.Playing;
            }
            else if (ButtonsState == eButtonsStates.Playing)
            {
                ButtonsState = eButtonsStates.Paused;
            }
            else if (ButtonsState == eButtonsStates.Paused)
            {
                ButtonsState = eButtonsStates.Playing;
            }
        }
        private void bStop_Click(object sender, EventArgs e)
        {
            ButtonsState = eButtonsStates.Stopped;
        }
        private void bPlayPause_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Play / pause the video", (IWin32Window)sender, 5000);
        }
        private void bStop_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Stop the video", (IWin32Window)sender, 5000);
        }
        #endregion
    }
}