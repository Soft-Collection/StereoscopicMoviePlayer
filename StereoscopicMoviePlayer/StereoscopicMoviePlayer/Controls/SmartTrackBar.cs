using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    class SmartTrackBar : TrackBar
    {
        #region Variables
        private long mLastSWTime = 0;
        private bool mAlreadyDone1 = true;
        private bool mAlreadyDone2 = true;
        private bool mUserChanging = false;
        private int mInterval1 = 200;
        private int mInterval2 = 400;
        private Timer mTimer = null;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("Time in milliseconds after last move to get scroll event.")]
        public int Interval1
        {
            get
            {
                return mInterval1;
            }
            set
            {
                mInterval1 = value;
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("Time in milliseconds after last move until value update is permitted and smartscroll is prohibited.")]
        public int Interval2
        {
            get
            {
                return mInterval2;
            }
            set
            {
                mInterval2 = value;
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public new int Value
        {
            get
            {
                return base.Value;
            }
            set
            {
                if (mAlreadyDone2) base.Value = value;
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("Occurs after enough time of not moving.")]
        public event EventHandler OnSmartScroll;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("Occurs on update request.")]
        public event EventHandler OnValueUpdateRequest;
        #endregion

        #region Constructor
        public SmartTrackBar()
        {
            this.ValueChanged += SmartTrackBar_Scroll;
            this.MouseDown += SmartTrackBar_MouseDown;
            this.MouseUp += SmartTrackBar_MouseUp;
            this.KeyDown += SmartTrackBar_KeyDown;
            this.KeyUp += SmartTrackBar_KeyUp;
            //----------------------------------------
            mTimer = new Timer();
            mTimer.Tick += Timer_Tick;
            mTimer.Enabled = true;
            mTimer.Interval = 10;
        }
        #endregion

        #region Methods
        private void Timer_Tick(object sender, EventArgs e)
        {
            if (((double)(Stopwatch.GetTimestamp() - mLastSWTime)) / ((double)Stopwatch.Frequency / 1000.0) > mInterval2)
            {
                if (!mAlreadyDone2) mAlreadyDone2 = true;
            }
            //-----------------------------------------------------
            if (((double)(Stopwatch.GetTimestamp() - mLastSWTime)) / ((double)Stopwatch.Frequency / 1000.0) > mInterval1)
            {
                if (!mAlreadyDone1)
                {
                    OnSmartScroll?.Invoke(this, EventArgs.Empty);
                    mAlreadyDone1 = true;
                }
            }
            //-----------------------------------------------------
            OnValueUpdateRequest?.Invoke(this, EventArgs.Empty);
        }
        private void SmartTrackBar_Scroll(object sender, EventArgs e)
        {
            if (mUserChanging)
            {
                mLastSWTime = Stopwatch.GetTimestamp();
                mAlreadyDone2 = false;
                mAlreadyDone1 = false;
            }
        }
        private void SmartTrackBar_MouseDown(object sender, MouseEventArgs e)
        {
            mUserChanging = true;
        }
        private void SmartTrackBar_MouseUp(object sender, MouseEventArgs e)
        {
            mUserChanging = false;
        }
        private void SmartTrackBar_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Left:
                case Keys.Right:
                case Keys.Up:
                case Keys.Down:
                case Keys.PageUp:
                case Keys.PageDown:
                case Keys.Home:
                case Keys.End:
                    mUserChanging = true;
                    break;
            }
        }
        private void SmartTrackBar_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Left:
                case Keys.Right:
                case Keys.Up:
                case Keys.Down:
                case Keys.PageUp:
                case Keys.PageDown:
                case Keys.Home:
                case Keys.End:
                    mUserChanging = false;
                    break;
            }
        }
        #endregion
    }
}