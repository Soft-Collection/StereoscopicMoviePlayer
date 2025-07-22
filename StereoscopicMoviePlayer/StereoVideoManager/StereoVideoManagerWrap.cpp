#include "stdafx.h"
#include "StereoVideoManagerWrap.h"

void* StereoVideoManagerCreateNew(HWND hWnd)
{
	CStereoVideoManager* cStereoVideoManager = new CStereoVideoManager(hWnd);
	return((void*)cStereoVideoManager);
}
void StereoVideoManagerDispose(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	delete cStereoVideoManager;
}
void StereoVideoManagerStereoStart(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoStart();
}
void StereoVideoManagerStereoStop(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoStop();
}
BOOL StereoVideoManagerStereoIsStarted(void* handle)
{
	if (handle == NULL) return false;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->StereoIsStarted());
}
int StereoVideoManagerStereoGetFrequency(void* handle)
{
	if (handle == NULL) return 0;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->StereoGetFrequency());
}
void StereoVideoManagerStereoSetCOMPort(void* handle, LPCWSTR comPort)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoSetCOMPort(comPort);
}
void StereoVideoManagerStereoSetGlassesTimeOffset(void* handle, int offset)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoSetGlassesTimeOffset(offset);
}
void StereoVideoManagerStereoLRBoth(void* handle, int lrboth)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoLRBoth(lrboth);
}
void StereoVideoManagerStereoSwapLR(void* handle, BOOL swaplr)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoSwapLR(swaplr);
}
void StereoVideoManagerStereoVerticalLR(void* handle, BOOL verticallr)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoVerticalLR(verticallr);
}
void StereoVideoManagerStereoWindowSizeChanged(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->StereoWindowSizeChanged();
}
void StereoVideoManagerPlayerOpen(void* handle, LPCWSTR fileName)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerOpen(fileName);
}
void StereoVideoManagerPlayerClose(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerClose();
}
BOOL StereoVideoManagerPlayerIsOpened(void* handle)
{
	if (handle == NULL) return false;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerIsOpened());
}
void StereoVideoManagerPlayerPlay(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerPlay();
}
void StereoVideoManagerPlayerPause(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerPause();
}
void StereoVideoManagerPlayerStop(void* handle)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerStop();
}
BOOL StereoVideoManagerPlayerIsPlaying(void* handle)
{
	if (handle == NULL) return false;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerIsPlaying());
}
BOOL StereoVideoManagerPlayerIsEOF(void* handle)
{
	if (handle == NULL) return false;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerIsEOF());
}
INT64 StereoVideoManagerPlayerGetDuration(void* handle)
{
	if (handle == NULL) return 0;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerGetDuration());
}
INT64 StereoVideoManagerPlayerGetCurrentPlayingTime(void* handle)
{
	if (handle == NULL) return 0;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerGetCurrentPlayingTime());
}
void StereoVideoManagerPlayerSeek(void* handle, INT64 seek_target_ms)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerSeek(seek_target_ms);
}
int StereoVideoManagerPlayerGetNumberOfAudioTracks(void* handle)
{
	if (handle == NULL) return 0;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerGetNumberOfAudioTracks());
}
void StereoVideoManagerPlayerSetAudioTrack(void* handle, int audio_track_index)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerSetAudioTrack(audio_track_index);
}
void StereoVideoManagerPlayerMute(void* handle, BOOL mute)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerMute(mute);
}
UINT16 StereoVideoManagerPlayerGetVolume(void* handle)
{
	if (handle == NULL) return 0;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	return(cStereoVideoManager->PlayerGetVolume());
}
void StereoVideoManagerPlayerSetVolume(void* handle, UINT16 volume)
{
	if (handle == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)handle;
	cStereoVideoManager->PlayerSetVolume(volume);
}
