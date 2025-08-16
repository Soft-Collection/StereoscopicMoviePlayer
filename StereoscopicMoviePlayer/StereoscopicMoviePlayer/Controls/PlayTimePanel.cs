using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    public partial class PlayTimePanel : UserControl
    {
        #region Classes
        public class SeekEventArgs : EventArgs
        {
            public int Seek { get; }
            public SeekEventArgs(int seek)
            {
                Seek = seek;
            }
        }
        #endregion

        #region Enums
        #endregion

        #region Variables
        private Int64 mDuration = 0;
        private Int64 mCurrentPlayingTime = 0;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public Int64 Duration
        {
            get
            {
                return mDuration;
            }
            set
            {
                mDuration = value;
                int duration_1000 = (int)Duration / 1000;
                tbMovieTime.Maximum = duration_1000;
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public Int64 CurrentPlayingTime
        {
            get
            {
                return mCurrentPlayingTime;
            }
            set
            {
                mCurrentPlayingTime = value;
                int currentPlayingTime_1000 = (int)mCurrentPlayingTime / 1000;
                //------------------------------------------
                if (currentPlayingTime_1000 > tbMovieTime.Maximum) mCurrentPlayingTime = 0;
                tbMovieTime.Value = currentPlayingTime_1000;
                //------------------------------------------
                int hours = currentPlayingTime_1000 / 3600;
                int minutes = currentPlayingTime_1000 / 60 % 60;
                int seconds = currentPlayingTime_1000 % 60;
                lblMovieTime.Text = $"{hours:D2}:{minutes:D2}:{seconds:D2}";
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler<SeekEventArgs> OnSeek;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("Occurs when value update permitted.")]
        public event EventHandler OnValueUpdateRequest;
        #endregion

        #region Constructor
        public PlayTimePanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        #endregion

        #region Event Methods
        private void tbMovieTime_OnSmartScroll(object sender, EventArgs e)
        {
            OnSeek?.Invoke(this, new SeekEventArgs(tbMovieTime.Value * 1000));
        }
        private void tbMovieTime_OnValueUpdateRequest(object sender, EventArgs e)
        {
            OnValueUpdateRequest?.Invoke(this, EventArgs.Empty);
        }
        private void tbMovieTime_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Rewind or go forward in the movie", (IWin32Window)sender, 5000);
        }
        #endregion
    }
}