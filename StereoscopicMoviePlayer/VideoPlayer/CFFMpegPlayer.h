#ifndef __CFFMPEGPLAYER_H__
#define __CFFMPEGPLAYER_H__

#include "CFFCommon.h"
#include "CFFMpegPlayer.h"
#include "CFFDecodeVideo.h"
#include "CFFDecodeAudio.h"
#include "CFFSampleConversion.h"
#include "CWavePlaying.h"
#include "CFFColorConversion.h"
#include <mutex>
#include <thread>
#include <atomic>

class CFFMpegPlayer
{
private:
	std::mutex*                  mMutexDecodeVideo;
	std::mutex*                  mMutexDecodeAudio;
	std::mutex*                  mMutexSampleConversion;
	std::mutex*                  mMutexColorConversion;
	//------------------------------------------------
	std::thread*                 mPlayerThread;
	std::atomic<bool>            mPlayerThreadRunning;
	std::atomic<bool>            mPlayerPaused;
	std::atomic<bool>            mPlayerPausedOnSeek;
	std::atomic<bool>            mPlayerIsSeeking;
	std::atomic<bool>            mPlayerSeekRequest;
	//------------------------------------------------
	std::wstring                 mFileName;
	static BOOL                  mStaticInitialized;
	//------------------------------------------------
	void* mUser;
	dOnNewVideoFrame             mOnNewVideoFrame;
	dOnNewAudioFrame             mOnNewAudioFrame;
	//------------------------------------------------
	AVFormatContext*             mFormatContext;
	//------------------------------------------------
	std::atomic<INT64>           mVideoDuration;
	std::atomic<int>             mVideoTracksNumber;
	std::atomic<int>             mVideoStreamIndex;
	std::atomic<int>             mAudioTracksNumber;
	std::atomic<int>             mAudioStreamIndex;
	std::atomic<INT64>           mCurrentPlayingTime;
	std::atomic<INT64>           mSeekTime;
	//------------------------------------------------
	class  CFFDecodeVideo*       mFFDecodeVideo;
	class  CFFDecodeAudio*       mFFDecodeAudio;
	class  CFFSampleConversion*  mFFSampleConversion;
	class  CFFColorConversion*   mFFColorConversion;
public:
	CFFMpegPlayer(void* user, dOnNewVideoFrame onNewVideoFrame, dOnNewAudioFrame onNewAudioFrame);
	~CFFMpegPlayer();
	void Open(std::wstring fileName);
	void Close();
	void Reopen();
	BOOL IsOpened();
	void Play();
	void Pause();
	void Stop();
	BOOL IsPlaying();
	INT64 GetDuration();
	INT64 GetCurrentPlayingTime();
	void Seek(INT64 seek_target_seconds);
	INT GetNumberOfVideoTracks();
	void SetVideoTrack(INT video_track_index);
	INT GetNumberOfAudioTracks();
	void SetAudioTrack(INT audio_track_index);
private:
	void MyThreadPlayerFunction();
	void OnVideoPacketReceived(AVFormatContext* formatContext, AVPacket* packet);
	void OnAudioPacketReceived(AVFormatContext* formatContext, AVPacket* packet);
	static void OnNewDecodedVideoFrameStatic(void* user, AVFrame* decodedFrame);
	void OnNewDecodedVideoFrame(AVFrame* decodedFrame);
	static void OnNewDecodedAudioFrameStatic(void* user, AVFrame* decodedFrame);
	void OnNewDecodedAudioFrame(AVFrame* decodedFrame);
	static void MyLogCallbackFunctionStatic(void* ptr, int level, const char* fmt, va_list vl);
private:
	static void InitStatic();
};

#endif //__CFFMPEGPLAYER_H__