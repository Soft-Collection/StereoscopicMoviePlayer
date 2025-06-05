#include "CStereoImageManager.h"

extern "C" 
{
	__declspec(dllexport) void* StereoImageManagerCreateNew(HWND hWnd);
	__declspec(dllexport) void StereoImageManagerDispose(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoStart(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoStop(void* handle);
	__declspec(dllexport) BOOL StereoImageManagerStereoIsStarted(void* handle);
	__declspec(dllexport) int StereoImageManagerStereoGetFrequency(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoSetCOMPort(void* handle, LPCWSTR comPort);
	__declspec(dllexport) void StereoImageManagerStereoSetGlassesTimeOffset(void* handle, int offset);
	__declspec(dllexport) void StereoImageManagerStereoSetTransparentTimePercent(void* handle, int percent);
    __declspec(dllexport) void StereoImageManagerStereoLRBoth(void* handle, int lrboth);
    __declspec(dllexport) void StereoImageManagerStereoSwapLR(void* handle, BOOL swaplr);
    __declspec(dllexport) void StereoImageManagerStereoVerticalLR(void* handle, BOOL verticallr);
    __declspec(dllexport) void StereoImageManagerStereoWindowSizeChanged(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerOpen(void* handle, LPCWSTR fileName);
    __declspec(dllexport) void StereoImageManagerPlayerClose(void* handle);
    __declspec(dllexport) BOOL StereoImageManagerPlayerIsOpened(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerPlay(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerPause(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerStop(void* handle);
    __declspec(dllexport) BOOL StereoImageManagerPlayerIsPlaying(void* handle);
    __declspec(dllexport) BOOL StereoImageManagerPlayerIsEOF(void* handle);
    __declspec(dllexport) INT64 StereoImageManagerPlayerGetDuration(void* handle);
    __declspec(dllexport) INT64 StereoImageManagerPlayerGetCurrentPlayingTime(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerSeek(void* handle, INT64 seek_target_ms);
    __declspec(dllexport) int StereoImageManagerPlayerGetNumberOfAudioTracks(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerSetAudioTrack(void* handle, int audio_track_index);
    __declspec(dllexport) void StereoImageManagerPlayerMute(void* handle, BOOL mute);
    __declspec(dllexport) UINT16 StereoImageManagerPlayerGetVolume(void* handle);
    __declspec(dllexport) void StereoImageManagerPlayerSetVolume(void* handle, UINT16 volume);
}