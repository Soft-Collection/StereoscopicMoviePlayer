#include "stdafx.h"
#include "StereoImageManagerWrap.h"

void* StereoImageManagerCreateNew(HWND hWnd)
{
	CStereoImageManager* cStereoImageManager = new CStereoImageManager(hWnd);
	return((void*)cStereoImageManager);
}
void StereoImageManagerDispose(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	delete cStereoImageManager;
}
void StereoImageManagerStereoStart(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoStart();
}
void StereoImageManagerStereoStop(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoStop();
}
BOOL StereoImageManagerStereoIsStarted(void* handle)
{
	if (handle == NULL) return false;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->StereoIsStarted());
}
int StereoImageManagerStereoGetFrequency(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->StereoGetFrequency());
}
void StereoImageManagerStereoSetCOMPort(void* handle, LPCWSTR comPort)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSetCOMPort(comPort);
}
void StereoImageManagerStereoSetGlassesTimeOffset(void* handle, int offset)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSetGlassesTimeOffset(offset);
}
void StereoImageManagerStereoSetTransparentTimePercent(void* handle, int percent)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSetTransparentTimePercent(percent);
}
void StereoImageManagerStereoLRBoth(void* handle, int lrboth)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoLRBoth(lrboth);
}
void StereoImageManagerStereoSwapLR(void* handle, BOOL swaplr)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSwapLR(swaplr);
}
void StereoImageManagerStereoVerticalLR(void* handle, BOOL verticallr)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoVerticalLR(verticallr);
}
void StereoImageManagerPlayerOpen(void* handle, LPCWSTR fileName)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerOpen(fileName);
}
void StereoImageManagerPlayerClose(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerClose();
}
BOOL StereoImageManagerPlayerIsOpened(void* handle)
{
	if (handle == NULL) return false;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerIsOpened());
}
void StereoImageManagerPlayerPlay(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerPlay();
}
void StereoImageManagerPlayerPause(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerPause();
}
void StereoImageManagerPlayerStop(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerStop();
}
BOOL StereoImageManagerPlayerIsPlaying(void* handle)
{
	if (handle == NULL) return false;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerIsPlaying());
}
INT64 StereoImageManagerPlayerGetDuration(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerGetDuration());
}
INT64 StereoImageManagerPlayerGetCurrentPlayingTime(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerGetCurrentPlayingTime());
}
void StereoImageManagerPlayerSeek(void* handle, INT64 seek_target_ms)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerSeek(seek_target_ms);
}
int StereoImageManagerPlayerGetNumberOfAudioTracks(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerGetNumberOfAudioTracks());
}
void StereoImageManagerPlayerSetAudioTrack(void* handle, int audio_track_index)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerSetAudioTrack(audio_track_index);
}
void StereoImageManagerPlayerMute(void* handle, BOOL mute)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerMute(mute);
}
UINT16 StereoImageManagerPlayerGetVolume(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->PlayerGetVolume());
}
void StereoImageManagerPlayerSetVolume(void* handle, UINT16 volume)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->PlayerSetVolume(volume);
}
