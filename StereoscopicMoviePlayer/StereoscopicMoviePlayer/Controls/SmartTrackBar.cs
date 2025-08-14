using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Forms;

class SmartTrackBar : TrackBar
{
    #region Variables
    private long mLastSWTime = 0;
    private bool mAlreadyApplied = true;
    private bool mDisableScroll = false;
    private int mTimeAfterLastMoveToGetScrollEvent = 0;
    private int mTimeAfterLastMoveUntilValueUpdateIsPermitted = 0;
    private Timer mTimer = null;
    #endregion

    #region Properties
    [Browsable(true)]
    [Category("Behavior")]
    [Description("Time in milliseconds after last move to get scroll event.")]
    public int TimeAfterLastMoveToGetScrollEvent
    {
        get
        {
            return mTimeAfterLastMoveToGetScrollEvent;
        }
        set
        {
            mTimeAfterLastMoveToGetScrollEvent = value;
        }
    }
    [Browsable(true)]
    [Category("Behavior")]
    [Description("Time in milliseconds after last move until value update is permitted.")]
    public int TimeAfterLastMoveUntilValueUpdateIsPermitted
    {
        get
        {
            return mTimeAfterLastMoveUntilValueUpdateIsPermitted;
        }
        set
        {
            mTimeAfterLastMoveUntilValueUpdateIsPermitted = value;
        }
    }
    #endregion

    #region Events
    [Browsable(true)]
    [Category("Behavior")]
    [Description("Occurs after enough time of not moving.")]
    public event EventHandler SmartScroll;
    [Browsable(true)]
    [Category("Behavior")]
    [Description("Occurs when value update permitted.")]
    public event EventHandler ValueUpdatePermitted;
    #endregion

    #region Constructor
    public SmartTrackBar()
    {
        this.ValueChanged += SmartTrackBar_Scroll;
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
        if (((double)(Stopwatch.GetTimestamp() - mLastSWTime)) / ((double)Stopwatch.Frequency / 1000.0) > mTimeAfterLastMoveUntilValueUpdateIsPermitted)
        {
            mDisableScroll = true;
            ValueUpdatePermitted?.Invoke(this, e);
            mDisableScroll = false;
        }
        //-----------------------------------------------------
        if (((double)(Stopwatch.GetTimestamp() - mLastSWTime)) / ((double)Stopwatch.Frequency / 1000.0) > mTimeAfterLastMoveToGetScrollEvent)
        {
            if (!mAlreadyApplied)
            {
                SmartScroll?.Invoke(this, e);
                mAlreadyApplied = true;
            }
        }
    }
    private void SmartTrackBar_Scroll(object sender, EventArgs e)
    {
        if (!mDisableScroll)
        {
            mLastSWTime = Stopwatch.GetTimestamp();
            mAlreadyApplied = false;
        }
    }
    #endregion
}