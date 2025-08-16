using System;

namespace StereoscopicMoviePlayer
{
    public class clsStereoVideoManager
    {
        #region Variables
        private IntPtr mHandle = IntPtr.Zero;
        #endregion

        #region New / Dispose
        public clsStereoVideoManager(IntPtr hWnd)
        {
            mHandle = clsStereoVideoManagerWrap.StereoVideoManagerCreateNew(hWnd);
        }
        public void Dispose()
        {
            GC.SuppressFinalize(this);
            clsStereoVideoManagerWrap.StereoVideoManagerDispose(mHandle);
        }
        ~clsStereoVideoManager()
        {
            Dispose();
        }
        #endregion

        #region Methods
        public void StereoStart()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoStart(mHandle);
            }
        }
        public void StereoStop()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoStop(mHandle);
            }
        }
        public bool StereoIsStarted()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerStereoIsStarted(mHandle);
            }
            return false;
        }
        public int StereoGetFrequency()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerStereoGetFrequency(mHandle);
            }
            return 0;
        }
        public void StereoSetCOMPort(string comPort)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoSetCOMPort(mHandle, comPort);
            }
        }
        public void StereoSetGlassesTimeOffset(int offset)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoSetGlassesTimeOffset(mHandle, offset);
            }
        }
        public void StereoLRBoth(int lrboth)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoLRBoth(mHandle, lrboth);
            }
        }
        public void StereoSwapLR(bool swaplr)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoSwapLR(mHandle, swaplr);
            }
        }
        public void StereoVerticalLR(bool verticallr)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoVerticalLR(mHandle, verticallr);
            }
        }
        public void StereoWindowSizeChanged()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerStereoWindowSizeChanged(mHandle);
            }
        }
        public void PlayerOpen(string fileName)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerOpen(mHandle, fileName);
            }
        }
        public void PlayerClose()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerClose(mHandle);
            }
        }
        public bool PlayerIsOpened()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerIsOpened(mHandle);
            }
            return false;
        }
        public void PlayerPlay()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerPlay(mHandle);
            }
        }
        public void PlayerPause()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerPause(mHandle);
            }
        }
        public void PlayerStop()
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerStop(mHandle);
            }
        }
        public bool PlayerIsPlaying()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerIsPlaying(mHandle);
            }
            return false;
        }
        public bool PlayerIsEOF()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerIsEOF(mHandle);
            }
            return false;
        }
        public Int64 PlayerGetDuration()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerGetDuration(mHandle);
            }
            return 0;
        }
        public Int64 PlayerGetCurrentPlayingTime()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerGetCurrentPlayingTime(mHandle);
            }
            return 0;
        }
        public void PlayerSeek(Int64 seek_target_ms)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerSeek(mHandle, seek_target_ms);
            }
        }
        public int PlayerGetNumberOfAudioTracks()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerGetNumberOfAudioTracks(mHandle);
            }
            return 0;
        }
        public void PlayerSetAudioTrack(int audio_track_index)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerSetAudioTrack(mHandle, audio_track_index);
            }
        }
        public void PlayerMute(bool mute)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerMute(mHandle, mute);
            }
        }
        public UInt16 PlayerGetVolume()
        {
            if (mHandle != IntPtr.Zero)
            {
                return clsStereoVideoManagerWrap.StereoVideoManagerPlayerGetVolume(mHandle);
            }
            return 0;
        }
        public void PlayerSetVolume(UInt16 volume)
        {
            if (mHandle != IntPtr.Zero)
            {
                clsStereoVideoManagerWrap.StereoVideoManagerPlayerSetVolume(mHandle, volume);
            }
        }
        #endregion
    }
}