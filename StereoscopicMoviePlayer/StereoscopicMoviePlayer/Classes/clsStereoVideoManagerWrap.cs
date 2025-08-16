using System;
using System.Runtime.InteropServices;

namespace StereoscopicMoviePlayer
{
    public class clsStereoVideoManagerWrap
    {
        #region Common

        #region Constants
        private const string DllFileName = "StereoVideoManager.dll";
        #endregion

        #endregion

        #region Stereo Image Manager

        #region External Functions

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr StereoVideoManagerCreateNew(IntPtr hWnd);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerDispose(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoStart(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoStop(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool StereoVideoManagerStereoIsStarted(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int StereoVideoManagerStereoGetFrequency(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoSetCOMPort(IntPtr StereoVideoManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string comPort);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoSetGlassesTimeOffset(IntPtr StereoVideoManagerHandle, int offset);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoLRBoth(IntPtr StereoVideoManagerHandle, int lrboth);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoSwapLR(IntPtr StereoVideoManagerHandle, bool swaplr);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoVerticalLR(IntPtr StereoVideoManagerHandle, bool verticallr);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerStereoWindowSizeChanged(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerOpen(IntPtr StereoVideoManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string fileName);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerClose(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool StereoVideoManagerPlayerIsOpened(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerPlay(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerPause(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerStop(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool StereoVideoManagerPlayerIsPlaying(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool StereoVideoManagerPlayerIsEOF(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Int64 StereoVideoManagerPlayerGetDuration(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Int64 StereoVideoManagerPlayerGetCurrentPlayingTime(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerSeek(IntPtr StereoVideoManagerHandle, Int64 seek_target_ms);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int StereoVideoManagerPlayerGetNumberOfAudioTracks(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerSetAudioTrack(IntPtr StereoVideoManagerHandle, int audio_track_index);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerMute(IntPtr StereoVideoManagerHandle, bool mute);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt16 StereoVideoManagerPlayerGetVolume(IntPtr StereoVideoManagerHandle);

        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StereoVideoManagerPlayerSetVolume(IntPtr StereoVideoManagerHandle, UInt16 volume);

        #endregion

        #endregion
    }
}
