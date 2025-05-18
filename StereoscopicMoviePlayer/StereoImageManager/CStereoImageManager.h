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
	std::mutex* mMutexWave;
	std::mutex* mMutexPlayer;
	std::mutex* mMutexRender;
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
	static void OnNewVideoFrameStatic(void* user, BYTE* frameData, int width, int height, int channels, INT64 pts);
	void OnNewVideoFrame(BYTE* frameData, int width, int height, int channels, INT64 pts);
	static void OnNewAudioFrameStatic(void* user, BYTE* frameData, int nb_samples, int samplesPerSec, int bitsPerSample, int Channels, INT64 pts);
	void OnNewAudioFrame(BYTE* frameData, int nb_samples, int samplesPerSec, int bitsPerSample, int Channels, INT64 pts);
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
	void PlayerReopen();
	BOOL PlayerIsOpened();
	void PlayerPlay();
	void PlayerPause();
	void PlayerStop();
	BOOL PlayerIsPlaying();
	INT64 PlayerGetDuration();
	INT64 PlayerGetCurrentPlayingTime();
	void PlayerSeek(INT64 seek_target_seconds);
	int PlayerGetNumberOfAudioTracks();
	void PlayerSetAudioTrack(int audio_track_index);
	void PlayerMute(BOOL mute);
	UINT16 PlayerGetVolume();
	void PlayerSetVolume(UINT16 volume);
};
#endif // __CSTEREOIMAGEMANAGER_H__