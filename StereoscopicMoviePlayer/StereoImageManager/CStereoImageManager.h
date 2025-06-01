#ifndef __CSTEREOIMAGEMANAGER_H__
#define __CSTEREOIMAGEMANAGER_H__

#include "CImage.h"
#include <mutex>
#include <thread>
#include <atomic>
#include "../StereoRendering/CStereoDirect3D.h"
#include "../SerialPort/CComPort.h"
#include "../VideoPlayer/CFFMpegPlayer.h"
#include "../VideoPlayer/CWavePlaying.h"

class CStereoImageManager
{
private:
	HWND mHWnd;
	std::wstring mComPortName;
	CComPort* mComPort;
	CStereoDirect3D* mStereoDirect3D;
	//----------------------------------------
	CImage* mLeftImage;
	CImage* mRightImage;
	//----------------------------------------
	std::mutex* mMutexWave1;
	std::mutex* mMutexWave2;
	std::mutex* mMutexPlayer;
	std::mutex* mMutexRender1;
	std::mutex* mMutexRender2;
	std::atomic<bool> mThreadRenderRunning;
	std::thread* mThreadRender;
	std::mutex* mMutexCOMPort;
	std::atomic<bool> mThreadCOMPortRunning;
	std::thread* mThreadCOMPort;
	HANDLE mCOMPortEvent;
	//----------------------------------------
	bool mImageToPlayIsLeft;
	//----------------------------------------
	CFFMpegPlayer* mPlayer;
	CWavePlaying* mWave;
private:
	void ThreadRenderFunction();
	void ThreadCOMPortFunction();
	static void OnNewVideoFrameStatic(void* user, AVFrame* frame);
	void OnNewVideoFrame(AVFrame* frame);
	static void OnNewAudioFrameStatic(void* user, AVFrame* frame);
	void OnNewAudioFrame(AVFrame* frame);
public:
	CStereoImageManager(HWND hWnd);
	~CStereoImageManager();
public:
	void StereoStart();
	void StereoStop();
	BOOL StereoIsStarted();
	int StereoGetFrequency();
	void StereoSetCOMPort(LPCWSTR comPort);
	void StereoSetGlassesTimeOffset(int offset);
	void StereoSetTransparentTimePercent(int percent);
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
	void StereoVerticalLR(BOOL verticallr);
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
#endif // __CSTEREOIMAGEMANAGER_H__