using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    public partial class StereoButtonsPanel : UserControl
    {
        #region Enums
        public enum eLRBothButtonsStates
        {
            Both,
            LeftOnly,
            RightOnly
        }
        public enum eSwapButtonStates
        {
            SwapOn,
            SwapOff
        }
        public enum eHVButtonStates
        {
            Vertical,
            Horizontal
        }
        #endregion

        #region Variables
        private eLRBothButtonsStates mLRBothButtonsState = eLRBothButtonsStates.Both;
        private eSwapButtonStates mSwapButtonState = eSwapButtonStates.SwapOff;
        private eHVButtonStates mHVButtonState = eHVButtonStates.Horizontal;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eLRBothButtonsStates LRBothButtonsState
        {
            get
            {
                return mLRBothButtonsState;
            }
            set
            {
                mLRBothButtonsState = value;
                switch (mLRBothButtonsState)
                {
                    case eLRBothButtonsStates.Both:
                        bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_selected;
                        bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
                        bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
                        OnBoth?.Invoke(this, EventArgs.Empty);
                        break;
                    case eLRBothButtonsStates.LeftOnly:
                        bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
                        bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_selected;
                        bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_unselected;
                        OnLeftOnly?.Invoke(this, EventArgs.Empty);
                        break;
                    case eLRBothButtonsStates.RightOnly:
                        bBoth.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.both_unselected;
                        bLeftOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.left_only_unselected;
                        bRightOnly.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.right_only_selected;
                        OnRightOnly?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eSwapButtonStates SwapButtonState
        {
            get
            {
                return mSwapButtonState;
            }
            set
            {
                mSwapButtonState = value;
                switch (mSwapButtonState)
                {
                    case eSwapButtonStates.SwapOff:
                        bSwap.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.swap_unselected;
                        OnNoSwap?.Invoke(this, EventArgs.Empty);
                        break;
                    case eSwapButtonStates.SwapOn:
                        bSwap.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.swap_selected;
                        OnSwap?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eHVButtonStates HVButtonState
        {
            get
            {
                return mHVButtonState;
            }
            set
            {
                mHVButtonState = value;
                switch (mHVButtonState)
                {
                    case eHVButtonStates.Vertical:
                        bVertical.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.vertical;
                        OnVertical?.Invoke(this, EventArgs.Empty);
                        break;
                    case eHVButtonStates.Horizontal:
                        bVertical.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.horizontal;
                        OnHorizontal?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnBoth;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnLeftOnly;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnRightOnly;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnNoSwap;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnSwap;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnHorizontal;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnVertical;
        #endregion

        #region Constructor
        public StereoButtonsPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        #endregion

        #region Event Methods
        private void bBoth_Click(object sender, EventArgs e)
        {
            LRBothButtonsState = eLRBothButtonsStates.Both;
        }
        private void bLeftOnly_Click(object sender, EventArgs e)
        {
            LRBothButtonsState = eLRBothButtonsStates.LeftOnly;
        }
        private void bRightOnly_Click(object sender, EventArgs e)
        {
            LRBothButtonsState = eLRBothButtonsStates.RightOnly;
        }
        private void bSwap_Click(object sender, EventArgs e)
        {
            if (SwapButtonState == eSwapButtonStates.SwapOff)
            {
                SwapButtonState = eSwapButtonStates.SwapOn;
            }
            else if (SwapButtonState == eSwapButtonStates.SwapOn)
            {
                SwapButtonState = eSwapButtonStates.SwapOff;
            }
        }
        private void bVertical_Click(object sender, EventArgs e)
        {
            if (HVButtonState == eHVButtonStates.Vertical)
            {
                HVButtonState = eHVButtonStates.Horizontal;
            }
            else if (HVButtonState == eHVButtonStates.Horizontal)
            {
                HVButtonState = eHVButtonStates.Vertical;
            }
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
        #endregion
    }
}