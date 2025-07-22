#include "CStereoVideoManager.h"

extern "C" 
{
	__declspec(dllexport) void* StereoVideoManagerCreateNew(HWND hWnd);
	__declspec(dllexport) void StereoVideoManagerDispose(void* handle);
	__declspec(dllexport) void StereoVideoManagerStereoStart(void* handle);
	__declspec(dllexport) void StereoVideoManagerStereoStop(void* handle);
	__declspec(dllexport) BOOL StereoVideoManagerStereoIsStarted(void* handle);
	__declspec(dllexport) int StereoVideoManagerStereoGetFrequency(void* handle);
	__declspec(dllexport) void StereoVideoManagerStereoSetCOMPort(void* handle, LPCWSTR comPort);
	__declspec(dllexport) void StereoVideoManagerStereoSetGlassesTimeOffset(void* handle, int offset);
    __declspec(dllexport) void StereoVideoManagerStereoLRBoth(void* handle, int lrboth);
    __declspec(dllexport) void StereoVideoManagerStereoSwapLR(void* handle, BOOL swaplr);
    __declspec(dllexport) void StereoVideoManagerStereoVerticalLR(void* handle, BOOL verticallr);
    __declspec(dllexport) void StereoVideoManagerStereoWindowSizeChanged(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerOpen(void* handle, LPCWSTR fileName);
    __declspec(dllexport) void StereoVideoManagerPlayerClose(void* handle);
    __declspec(dllexport) BOOL StereoVideoManagerPlayerIsOpened(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerPlay(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerPause(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerStop(void* handle);
    __declspec(dllexport) BOOL StereoVideoManagerPlayerIsPlaying(void* handle);
    __declspec(dllexport) BOOL StereoVideoManagerPlayerIsEOF(void* handle);
    __declspec(dllexport) INT64 StereoVideoManagerPlayerGetDuration(void* handle);
    __declspec(dllexport) INT64 StereoVideoManagerPlayerGetCurrentPlayingTime(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerSeek(void* handle, INT64 seek_target_ms);
    __declspec(dllexport) int StereoVideoManagerPlayerGetNumberOfAudioTracks(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerSetAudioTrack(void* handle, int audio_track_index);
    __declspec(dllexport) void StereoVideoManagerPlayerMute(void* handle, BOOL mute);
    __declspec(dllexport) UINT16 StereoVideoManagerPlayerGetVolume(void* handle);
    __declspec(dllexport) void StereoVideoManagerPlayerSetVolume(void* handle, UINT16 volume);
}