#ifndef __CFFMPEGPLAYER_H__
#define __CFFMPEGPLAYER_H__

#include "CFFCommon.h"
#include "CFFMpegPlayer.h"
#include "CAutoBuffer.h"
#include "CAutoBuffer.cpp"
#include "CDeviationBuffer.h"
#include "CFFDecodeVideo.h"
#include "CFFDecodeAudio.h"
#include "CFFSampleConversion.h"
#include "CWavePlaying.h"
#include "CFFColorConversion.h"
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>

typedef void(*dOnNewVideoFrame)(void* user, AVFrame* frame);
typedef void(*dOnNewAudioFrame)(void* user, AVFrame* frame);

class CFFMpegPlayer
{
private:
	typedef struct {
		std::chrono::steady_clock::time_point Time;
		INT64 MS;
	} TimeData;
private:
	std::mutex*                   mMutexVideoFrameBuffer;
	std::mutex*                   mMutexAudioFrameBuffer;
	//------------------------------------------------
	std::mutex*                   mMutexDecodeVideo;
	std::mutex*                   mMutexDecodeAudio;
	std::mutex*                   mMutexSampleConversion;
	std::mutex*                   mMutexColorConversion;
	//------------------------------------------------
	std::mutex*                   mMutexSeek;
	//------------------------------------------------
	std::thread*                  mPlayerThread;
	std::atomic<bool>             mPlayerThreadRunning;
	std::atomic<bool>             mPlayerPausedOnSeek;
	std::atomic<bool>             mPlayerIsSeeking;
	std::atomic<int>              mPlayerIsSeekingFrameCounter;
	std::atomic<bool>             mClosing;
	std::atomic<bool>             mIsEOF;
	//------------------------------------------------
	std::wstring                  mFileName;
	static BOOL                   mStaticInitialized;
	//------------------------------------------------
	void*                         mUser;				  
	dOnNewVideoFrame              mOnNewVideoFrame;
	dOnNewAudioFrame              mOnNewAudioFrame;
	//------------------------------------------------
	AVFormatContext*              mFormatContext;
	//------------------------------------------------
	std::atomic<INT64>            mVideoDuration;
	std::atomic<int>              mVideoTracksNumber;
	std::atomic<int>              mVideoStreamIndex;
	std::atomic<int>              mAudioTracksNumber;
	std::atomic<int>              mAudioStreamIndex;
	std::atomic<INT64>            mCurrentPlayingTime;
	//------------------------------------------------
	TimeData                      mLastVideoTime;
	TimeData                      mLastAudioTime;
	//------------------------------------------------
	HANDLE                        mPlayerPausedEvent;
	//------------------------------------------------
	class CAutoBuffer<AVFrame*>*  mVideoFrameBuffer;
	class CAutoBuffer<AVFrame*>*  mAudioFrameBuffer;
	//------------------------------------------------
	class CDeviationBuffer*       mDeviationBuffer;
	//------------------------------------------------
	class CFFDecodeVideo*         mFFDecodeVideo;
	class CFFDecodeAudio*         mFFDecodeAudio;
	class CFFSampleConversion*    mFFSampleConversion;
	class CFFColorConversion*     mFFColorConversion;
public:
	CFFMpegPlayer(void* user, dOnNewVideoFrame onNewVideoFrame, dOnNewAudioFrame onNewAudioFrame);
	~CFFMpegPlayer();
	void Open(std::wstring fileName);
	void Close();
	BOOL IsOpened();
	void Play();
	void Pause();
	void Stop();
	BOOL IsPlaying();
	BOOL IsEOF();
	INT64 GetDuration();
	INT64 GetCurrentPlayingTime();
	void Seek(INT64 seek_target_ms);
	INT GetNumberOfVideoTracks();
	void SetVideoTrack(INT video_track_index);
	INT GetNumberOfAudioTracks();
	void SetAudioTrack(INT audio_track_index);
private:
	INT64 PtsToMS(AVMediaType mediaType, INT64 pts);
	INT64 MSToPts(AVMediaType mediaType, INT64 ms);
	void WaitBetweenFrames(AVMediaType mediaType, TimeData& lastTime, INT64 pts);
private:
	void MyPlayerThreadFunction();
	void OnVideoPacketReceived(AVPacket* packet);
	void OnAudioPacketReceived(AVPacket* packet);
	static void OnVideoFrameReceivedStatic(void* user, AVFrame* frame, INT deviation);
	void OnVideoFrameReceived(AVFrame* frame, INT deviation);
	static void OnAudioFrameReceivedStatic(void* user, AVFrame* frame, INT deviation);
	void OnAudioFrameReceived(AVFrame* frame, INT deviation);
	static void MyLogCallbackFunctionStatic(void* ptr, int level, const char* fmt, va_list vl);
	static void FramePTS(AVFrame* a, INT64** pts);
	void ClearAllBuffers();
	BOOL IsEventSet(HANDLE event);
private:
	static void InitStatic();
};

#endif //__CFFMPEGPLAYER_H__