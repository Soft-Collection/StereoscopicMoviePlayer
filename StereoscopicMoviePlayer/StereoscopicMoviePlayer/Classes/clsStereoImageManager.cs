using System;

public class clsStereoImageManager
{
    #region Variables
    private IntPtr mHandle = IntPtr.Zero;
    #endregion

    #region New / Dispose
    public clsStereoImageManager(IntPtr hWnd)
    {
        mHandle = clsStereoImageManagerWrap.StereoImageManagerCreateNew(hWnd);
    }
    public void Dispose()
    {
        GC.SuppressFinalize(this);
        clsStereoImageManagerWrap.StereoImageManagerDispose(mHandle);
    }
    ~clsStereoImageManager()
    {
        Dispose();
    }
    #endregion

    #region Methods
    public void StereoStart()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoStart(mHandle);
        }
    }
    public void StereoStop()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoStop(mHandle);
        }
    }
    public bool StereoIsStarted()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerStereoIsStarted(mHandle);
        }
        return false;
    }
    public int StereoGetFrequency()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerStereoGetFrequency(mHandle);
        }
        return 0;
    }
    public void StereoSetCOMPort(string comPort)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSetCOMPort(mHandle, comPort);
        }
    }
    public void StereoSetGlassesTimeOffset(int offset)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSetGlassesTimeOffset(mHandle, offset);
        }
    }
    public void StereoSetTransparentTimePercent(int percent)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSetTransparentTimePercent(mHandle, percent);
        }
    }
    public void StereoLRBoth(int lrboth)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoLRBoth(mHandle, lrboth);
        }
    }
    public void StereoSwapLR(bool swaplr)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSwapLR(mHandle, swaplr);
        }
    }
    public void StereoVerticalLR(bool verticallr)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoVerticalLR(mHandle, verticallr);
        }
    }
    public void StereoWindowSizeChanged()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoWindowSizeChanged(mHandle);
        }
    }
    public void PlayerOpen(string fileName)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerOpen(mHandle, fileName);
        }
    }
    public void PlayerClose()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerClose(mHandle);
        }
    }
    public bool PlayerIsOpened()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerIsOpened(mHandle);
        }
        return false;
    }
    public void PlayerPlay()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerPlay(mHandle);
        }
    }
    public void PlayerPause()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerPause(mHandle);
        }
    }
    public void PlayerStop()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerStop(mHandle);
        }
    }
    public bool PlayerIsPlaying()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerIsPlaying(mHandle);
        }
        return false;
    }
    public bool PlayerIsEOF()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerIsEOF(mHandle);
        }
        return false;
    }
    public Int64 PlayerGetDuration()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerGetDuration(mHandle);
        }
        return 0;
    }
    public Int64 PlayerGetCurrentPlayingTime()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerGetCurrentPlayingTime(mHandle);
        }
        return 0;
    }
    public void PlayerSeek(Int64 seek_target_ms)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerSeek(mHandle, seek_target_ms);
        }
    }
    public int PlayerGetNumberOfAudioTracks()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerGetNumberOfAudioTracks(mHandle);
        }
        return 0;
    }
    public void PlayerSetAudioTrack(int audio_track_index)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerSetAudioTrack(mHandle, audio_track_index);
        }
    }
    public void PlayerMute(bool mute)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerMute(mHandle, mute);
        }
    }
    public UInt16 PlayerGetVolume()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerPlayerGetVolume(mHandle);
        }
        return 0;
    }
    public void PlayerSetVolume(UInt16 volume)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerPlayerSetVolume(mHandle, volume);
        }
    }
    #endregion
}
