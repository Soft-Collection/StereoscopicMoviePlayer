using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    public partial class SoundPanel : UserControl
    {
        #region Classes
        public class SoundVolumeEventArgs : EventArgs
        {
            public UInt16 SoundVolume { get; }
            public SoundVolumeEventArgs(UInt16 soundVolume)
            {
                SoundVolume = soundVolume;
            }
        }
        public class SelectedSoundTrackEventArgs : EventArgs
        {
            public int SelectedSoundTrack { get; }
            public SelectedSoundTrackEventArgs(int selectedSoundTrack)
            {
                SelectedSoundTrack = selectedSoundTrack;
            }
        }
        #endregion

        #region Enums
        public enum eSoundStates
        {
            SoundOff,
            SoundOn
        }
        #endregion

        #region Variables
        private eSoundStates mSoundState = eSoundStates.SoundOn;
        private UInt16 mSoundVolume = 0;
        private int mSelectedSoundTrack = 0;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eSoundStates SoundState
        {
            get
            {
                return mSoundState;
            }
            set
            {
                mSoundState = value;
                switch (mSoundState)
                {
                    case eSoundStates.SoundOff:
                        bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.no_sound;
                        tbVolume.Enabled = false;
                        OnSoundOff?.Invoke(this, EventArgs.Empty);
                        break;
                    case eSoundStates.SoundOn:
                        bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.sound;
                        tbVolume.Enabled = true;
                        OnSoundOn?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public UInt16 SoundVolume
        {
            get
            {
                return mSoundVolume;
            }
            set
            {
                mSoundVolume = value;
                tbVolume.Value = mSoundVolume;
                OnSoundVolumeChanged?.Invoke(this, new SoundVolumeEventArgs(SoundVolume));
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public int SelectedSoundTrack
        {
            get
            {
                return mSelectedSoundTrack;
            }
            set
            {
                mSelectedSoundTrack = value;
                OnSelectedSoundTrackChanged?.Invoke(this, new SelectedSoundTrackEventArgs(SelectedSoundTrack));
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnSoundOff;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnSoundOn;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler<SelectedSoundTrackEventArgs> OnSelectedSoundTrackChanged;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler<SoundVolumeEventArgs> OnSoundVolumeChanged;
        #endregion

        #region Constructor
        public SoundPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        public void LoadSoundTracks(int numberOfAudioTracks)
        {
            cbTracks.Items.Clear();
            for (int i = 0; i < numberOfAudioTracks; i++)
            {
                cbTracks.Items.Add("Track" + i.ToString());
            }
            if (cbTracks.Items.Count > 0)
            {
                cbTracks.SelectedIndex = 0;
            }
        }
        #endregion

        #region Event Methods
        private void bSound_Click(object sender, EventArgs e)
        {
            if (SoundState == eSoundStates.SoundOff)
            {
                SoundState = eSoundStates.SoundOn;
            }
            else if (SoundState == eSoundStates.SoundOn)
            {
                SoundState = eSoundStates.SoundOff;
            }
        }
        private void tbVolume_Scroll(object sender, EventArgs e)
        {
            SoundVolume = (UInt16)tbVolume.Value;
        }
        private void cbTracks_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedSoundTrack = cbTracks.SelectedIndex;
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
        #endregion
    }
}
