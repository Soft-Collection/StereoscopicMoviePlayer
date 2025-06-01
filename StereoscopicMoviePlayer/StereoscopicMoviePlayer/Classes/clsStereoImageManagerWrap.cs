using System;
using System.Runtime.InteropServices;

public class clsStereoImageManagerWrap
{
    #region Common

    #region Constants
    private const string DllFileName = "StereoImageManager.dll";
    #endregion

    #endregion

    #region Stereo Image Manager

    #region External Functions

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr StereoImageManagerCreateNew(IntPtr hWnd);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerDispose(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoStart(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoStop(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerStereoIsStarted(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int StereoImageManagerStereoGetFrequency(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSetCOMPort(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string comPort);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSetGlassesTimeOffset(IntPtr StereoImageManagerHandle, int offset);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSetTransparentTimePercent(IntPtr StereoImageManagerHandle, int percent);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoLRBoth(IntPtr StereoImageManagerHandle, int lrboth);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSwapLR(IntPtr StereoImageManagerHandle, bool swaplr);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoVerticalLR(IntPtr StereoImageManagerHandle, bool verticallr);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerOpen(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string fileName);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerClose(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerPlayerIsOpened(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerPlay(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerPause(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerStop(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerPlayerIsPlaying(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerPlayerIsEOF(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern Int64 StereoImageManagerPlayerGetDuration(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern Int64 StereoImageManagerPlayerGetCurrentPlayingTime(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerSeek(IntPtr StereoImageManagerHandle, Int64 seek_target_ms);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int StereoImageManagerPlayerGetNumberOfAudioTracks(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerSetAudioTrack(IntPtr StereoImageManagerHandle, int audio_track_index);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerMute(IntPtr StereoImageManagerHandle, bool mute);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UInt16 StereoImageManagerPlayerGetVolume(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerPlayerSetVolume(IntPtr StereoImageManagerHandle, UInt16 volume);

    #endregion

    #endregion
}
