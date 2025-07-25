#ifndef __CSTEREOVIDEOMANAGER_H__
#define __CSTEREOVIDEOMANAGER_H__

#include "CImage.h"
#include <mutex>
#include <thread>
#include <atomic>
#include "../StereoRendering/CStereoDirect2D.h"
#include "../SerialPort/CComPort.h"
#include "../VideoPlayer/CFFCommon.h"
#include "../VideoPlayer/CFFMpegPlayer.h"
#include "../VideoPlayer/CWavePlaying.h"

class CStereoVideoManager
{
private:
	HWND mHWnd;
	std::wstring mComPortName;
	CComPort* mComPort;
	CStereoDirect2D* mStereoDirect2D;
	//----------------------------------------
	std::mutex* mMutexWave1;
	std::mutex* mMutexWave2;
	std::mutex* mMutexPlayer;
	std::mutex* mMutexRender1;
	std::mutex* mMutexRender2;
	std::atomic<bool> mThreadRenderRunning;
	std::thread* mThreadRender;
	std::mutex* mMutexCOMPort;
	//----------------------------------------
	bool mImageToPlayIsLeft;
	//----------------------------------------
	CFFMpegPlayer* mPlayer;
	CWavePlaying* mWave;
private:
	void ThreadRenderFunction();
	static void OnNewVideoFrameStatic(void* user, AVFrame* frame);
	void OnNewVideoFrame(AVFrame* frame);
	static void OnNewAudioFrameStatic(void* user, AVFrame* frame);
	void OnNewAudioFrame(AVFrame* frame);
	static void SendSyncStatic(void* user, int syncType);
	void SendSync(int syncType);
public:
	CStereoVideoManager(HWND hWnd);
	~CStereoVideoManager();
public:
	void StereoStart();
	void StereoStop();
	BOOL StereoIsStarted();
	int StereoGetFrequency();
	void StereoSetCOMPort(LPCWSTR comPort);
	void StereoSetGlassesTimeOffset(int offset);
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
	void StereoVerticalLR(BOOL verticallr);
	void StereoWindowSizeChanged();
	void PlayerOpen(LPCWSTR fileName);
	void PlayerClose();
	BOOL PlayerIsOpened();
	void PlayerPlay();
	void PlayerPause();
	void PlayerStop();
	BOOL PlayerIsPlaying();
	BOOL PlayerIsEOF();
	INT64 PlayerGetDuration();
	INT64 PlayerGetCurrentPlayingTime();
	void PlayerSeek(INT64 seek_target_ms);
	int PlayerGetNumberOfAudioTracks();
	void PlayerSetAudioTrack(int audio_track_index);
	void PlayerMute(BOOL mute);
	UINT16 PlayerGetVolume();
	void PlayerSetVolume(UINT16 volume);
};
#endif // __CSTEREOVIDEOMANAGER_H__