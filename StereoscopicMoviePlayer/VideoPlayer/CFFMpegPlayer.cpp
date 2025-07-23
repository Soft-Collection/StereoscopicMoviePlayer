#include "stdafx.h"
#include "CFFMpegPlayer.h"
#include <tchar.h>
#include "../Common/CTools.h"

#define DEBUG_MODE 0
#define SEEKING_FRAME_NUMBER 6

BOOL CFFMpegPlayer::mStaticInitialized = FALSE;

CFFMpegPlayer::CFFMpegPlayer(void* user, dOnNewVideoFrame onNewVideoFrame, dOnNewAudioFrame onNewAudioFrame)
{
	InitStatic();
	//-------------------------------------------------------
	mMutexVideoFrameBuffer = new std::mutex();
	mMutexAudioFrameBuffer = new std::mutex();
	//-------------------------------------------------------
	mMutexDecodeVideo = new std::mutex();
	mMutexDecodeAudio = new std::mutex();
	mMutexSampleConversion = new std::mutex();
	mMutexColorConversion = new std::mutex();
	//-------------------------------------------------------
	mMutexSeek = new std::mutex();
	//-------------------------------------------------------
	mPlayerPausedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//-------------------------------------------------------
	mUser = user;
	mOnNewVideoFrame = onNewVideoFrame;
	mOnNewAudioFrame = onNewAudioFrame;
	//------------------------------------------------
	mPlayerThread = NULL;
	mPlayerThreadRunning.store(false);
	mPlayerPausedOnSeek.store(false);
	mPlayerIsSeeking.store(false);
	mPlayerIsSeekingFrameCounter.store(0);
	mClosing.store(false);
	mIsEOF.store(false);
	//------------------------------------------------
	mFileName = std::wstring(L"");
	//------------------------------------------------
	mFormatContext = NULL;
	//------------------------------------------------
	mVideoDuration.store(0);
	mVideoTracksNumber.store(0);
	mVideoStreamIndex.store(-1);
	mAudioTracksNumber.store(0);
	mAudioStreamIndex.store(-1);
	mCurrentPlayingTime.store(0);
	//------------------------------------------------
	mLastVideoTime = { std::chrono::steady_clock::time_point(), 0 };
	mLastAudioTime = { std::chrono::steady_clock::time_point(), 0 };
	//------------------------------------------------
	mVideoFrameBuffer = NULL;
	mAudioFrameBuffer = NULL;
	//------------------------------------------------
	mDeviationBuffer = NULL;
	//------------------------------------------------
	mFFDecodeVideo = NULL;
	mFFDecodeAudio = NULL;
	mFFSampleConversion = NULL;
	mFFColorConversion = NULL;
	//------------------------------------------------
	avformat_network_init();
	avdevice_register_all();
}

CFFMpegPlayer::~CFFMpegPlayer()
{
	Close();
	if (mPlayerPausedEvent != nullptr)
	{
		CloseHandle(mPlayerPausedEvent);
		mPlayerPausedEvent = nullptr;
	}
	if (mMutexSeek != nullptr)
	{
		delete mMutexSeek;
		mMutexSeek = nullptr;
	}
	if (mMutexColorConversion != nullptr)
	{
		delete mMutexColorConversion;
		mMutexColorConversion = nullptr;
	}
	if (mMutexSampleConversion != nullptr)
	{
		delete mMutexSampleConversion;
		mMutexSampleConversion = nullptr;
	}
	if (mMutexDecodeAudio != nullptr)
	{
		delete mMutexDecodeAudio;
		mMutexDecodeAudio = nullptr;
	}
	if (mMutexDecodeVideo != nullptr)
	{
		delete mMutexDecodeVideo;
		mMutexDecodeVideo = nullptr;
	}
	if (mMutexAudioFrameBuffer != nullptr)
	{
		delete mMutexAudioFrameBuffer;
		mMutexAudioFrameBuffer = nullptr;
	}
	if (mMutexVideoFrameBuffer != nullptr)
	{
		delete mMutexVideoFrameBuffer;
		mMutexVideoFrameBuffer = nullptr;
	}
}

void CFFMpegPlayer::InitStatic()
{
	if (!mStaticInitialized)
	{
		//---------------------------------
		av_log_set_callback(MyLogCallbackFunctionStatic);
		//---------------------------------
		mStaticInitialized = TRUE;
	}
}

void CFFMpegPlayer::Open(std::wstring fileName)
{
	ResetEvent(mPlayerPausedEvent);
	mPlayerPausedOnSeek.store(false);
	mPlayerIsSeeking.store(false);
	mPlayerIsSeekingFrameCounter.store(0);
	mIsEOF.store(false);
	//-------------------------------------------------------
	mVideoDuration.store(0);
	mVideoStreamIndex.store(-1);
	mAudioStreamIndex.store(-1);
	mCurrentPlayingTime.store(0);
	//-------------------------------------------------------
	mLastVideoTime = { std::chrono::steady_clock::time_point(), 0 };
	mLastAudioTime = { std::chrono::steady_clock::time_point(), 0 };
	//-------------------------------------------------------
	mDeviationBuffer = new CDeviationBuffer(3.7); //Threshold must be slightly less then max deviation.
	//-------------------------------------------------------
	mFFColorConversion = new CFFColorConversion();
	mFFSampleConversion = new CFFSampleConversion();
	mFFDecodeVideo = new CFFDecodeVideo();
	mFFDecodeAudio = new CFFDecodeAudio();
	//-------------------------------------------------------
	if (mFFColorConversion != NULL) mFFColorConversion->ReallocateResources();
	if (mFFSampleConversion != NULL) mFFSampleConversion->ReallocateResources();
	if (mFFDecodeVideo != NULL) mFFDecodeVideo->ReallocateResources();
	if (mFFDecodeAudio != NULL) mFFDecodeAudio->ReallocateResources();
	//-------------------------------------------------------
	mFileName = fileName;
	std::string fileNameA = CTools::ConvertUnicodeToMultibyte(fileName);
	mFormatContext = avformat_alloc_context();
	if (avformat_open_input(&mFormatContext, fileNameA.c_str(), NULL, NULL) < 0) return;
	if (avformat_find_stream_info(mFormatContext, NULL) < 0) return;
	//-------------------------------------------------------
	mVideoFrameBuffer = new CAutoBuffer<AVFrame*>(this, OnVideoFrameReceivedStatic, av_frame_free, FramePTS);
	mAudioFrameBuffer = new CAutoBuffer<AVFrame*>(this, OnAudioFrameReceivedStatic, av_frame_free, FramePTS);
	//-------------------------------------------------------
	if (!mPlayerThreadRunning.load())
	{
		mPlayerThreadRunning.store(true);
		mPlayerThread = new std::thread(&CFFMpegPlayer::MyPlayerThreadFunction, this);
	}
	//-------------------------------------------------------
	mVideoTracksNumber.store(GetNumberOfVideoTracks());
	if (mVideoTracksNumber.load() > 0)
	{
		SetVideoTrack(0);
	}
	mAudioTracksNumber.store(GetNumberOfAudioTracks());
	if (mAudioTracksNumber.load() > 0)
	{
		SetAudioTrack(0);
	}
	//-------------------------------------------------------
	mClosing.store(false);
}

void CFFMpegPlayer::Close()
{
	mClosing.store(true);
	if (mPlayerThreadRunning.load())
	{
		SetEvent(mPlayerPausedEvent);
		mPlayerThreadRunning.store(false);
		if (mPlayerThread && mPlayerThread->joinable())
		{
			mPlayerThread->join();
		}
		delete mPlayerThread;
		mPlayerThread = nullptr;
	}
	std::unique_lock<std::mutex> lock1(*mMutexAudioFrameBuffer); // Lock the mutex
	if (mAudioFrameBuffer != NULL)
	{
		mAudioFrameBuffer->Clear();
		delete mAudioFrameBuffer;
		mAudioFrameBuffer = NULL;
	}
	lock1.unlock();
	std::unique_lock<std::mutex> lock2(*mMutexVideoFrameBuffer); // Lock the mutex
	if (mVideoFrameBuffer != NULL)
	{
		mVideoFrameBuffer->Clear();
		delete mVideoFrameBuffer;
		mVideoFrameBuffer = NULL;
	}
	lock2.unlock();
	if (mFormatContext != NULL)
	{
		avformat_close_input(&mFormatContext);
		avformat_free_context(mFormatContext);
		mFormatContext = NULL;
	}
	std::unique_lock<std::mutex> lock3(*mMutexDecodeAudio); // Lock the mutex
	if (mFFDecodeAudio != NULL)
	{
		mFFDecodeAudio->FlushBuffers();
		delete mFFDecodeAudio;
		mFFDecodeAudio = NULL;
	}
	lock3.unlock();
	std::unique_lock<std::mutex> lock4(*mMutexDecodeVideo); // Lock the mutex
	if (mFFDecodeVideo != NULL)
	{
		mFFDecodeVideo->FlushBuffers();
		delete mFFDecodeVideo;
		mFFDecodeVideo = NULL;
	}
	lock4.unlock();
	std::unique_lock<std::mutex> lock5(*mMutexSampleConversion); // Lock the mutex
	if (mFFSampleConversion != NULL)
	{
		delete mFFSampleConversion;
		mFFSampleConversion = NULL;
	}
	lock5.unlock();
	std::unique_lock<std::mutex> lock6(*mMutexColorConversion); // Lock the mutex
	if (mFFColorConversion != NULL)
	{
		delete mFFColorConversion;
		mFFColorConversion = NULL;
	}
	lock6.unlock();
	if (mDeviationBuffer != NULL)
	{
		mDeviationBuffer->Clear();
		delete mDeviationBuffer;
		mDeviationBuffer = NULL;
	}
}

BOOL CFFMpegPlayer::IsOpened()
{
	return mPlayerThreadRunning.load();
}

void CFFMpegPlayer::Play()
{
	if (mPlayerThreadRunning.load())
	{
		SetEvent(mPlayerPausedEvent);
	}
}

void CFFMpegPlayer::Pause()
{
	if (mPlayerThreadRunning.load())
	{
		ResetEvent(mPlayerPausedEvent);
	}
}

void CFFMpegPlayer::Stop()
{
	if (mPlayerThreadRunning.load())
	{
		Pause();
		Seek(0);
	}
}

BOOL CFFMpegPlayer::IsPlaying()
{
	return mPlayerThreadRunning.load() && IsEventSet(mPlayerPausedEvent);
}

BOOL CFFMpegPlayer::IsEOF()
{
	return mPlayerThreadRunning.load() && mIsEOF.load();
}

INT64 CFFMpegPlayer::GetDuration()
{
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			if (mFormatContext->duration != AV_NOPTS_VALUE) {
				// Duration is in AV_TIME_BASE units (usually microseconds)
				return (INT64)((double)mFormatContext->duration / (double)AV_TIME_BASE * 1000.0);
			}
		}
	}
	return 0;
}

INT64 CFFMpegPlayer::GetCurrentPlayingTime()
{
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			return PtsToMS(AVMEDIA_TYPE_VIDEO, mCurrentPlayingTime.load());
		}
	}
	return 0;
}

void CFFMpegPlayer::Seek(INT64 seek_target_ms)
{
	if (mVideoStreamIndex < 0) return;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			std::unique_lock<std::mutex> lock1(*mMutexSeek); // Lock the mutex
			mPlayerIsSeeking.store(true);
			mPlayerIsSeekingFrameCounter.store(SEEKING_FRAME_NUMBER);
			mPlayerPausedOnSeek.store(!IsEventSet(mPlayerPausedEvent));
			mCurrentPlayingTime.store(MSToPts(AVMEDIA_TYPE_VIDEO, seek_target_ms));
			mIsEOF.store(false);
			ResetEvent(mPlayerPausedEvent);
			ClearAllBuffers();
			avformat_seek_file(mFormatContext, mVideoStreamIndex, INT64_MIN, MSToPts(AVMEDIA_TYPE_VIDEO, seek_target_ms), INT64_MAX, 0);
			SetEvent(mPlayerPausedEvent);
			lock1.unlock();
		}
	}
}

INT CFFMpegPlayer::GetNumberOfVideoTracks()
{
	INT retVal = 0;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			for (INT i = 0; i < mFormatContext->nb_streams; i++)
			{
				if (mFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
				{
					retVal++;
				}
			}
		}
	}
	return retVal;
}

void CFFMpegPlayer::SetVideoTrack(INT video_track_index)
{
	if (video_track_index >= mVideoTracksNumber) return;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			INT at_cnt = 0;
			for (INT i = 0; i < mFormatContext->nb_streams; i++)
			{
				if (mFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
				{
					if (at_cnt == video_track_index)
					{
						mVideoStreamIndex = i;
					}
					at_cnt++;
				}
			}
		}
	}
}

INT CFFMpegPlayer::GetNumberOfAudioTracks()
{
	INT retVal = 0;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			for (INT i = 0; i < mFormatContext->nb_streams; i++)
			{
				if (mFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
				{
					retVal++;
				}
			}
		}
	}
	return retVal;
}

void CFFMpegPlayer::SetAudioTrack(INT audio_track_index)
{
	if (audio_track_index >= mAudioTracksNumber) return;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			INT at_cnt = 0;
			for (INT i = 0; i < mFormatContext->nb_streams; i++)
			{
				if (mFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
				{
					if (at_cnt == audio_track_index)
					{
						mAudioStreamIndex = i;
					}
					at_cnt++;
				}
			}
		}
	}
}

void CFFMpegPlayer::MyPlayerThreadFunction()
{
	while (mPlayerThreadRunning.load())
	{
		WaitForSingleObject(mPlayerPausedEvent, INFINITE);
		AVPacket* packet = NULL;
		packet = av_packet_alloc();
		if (!packet)
		{
			av_packet_free(&packet);
			continue;
		}
		int res = av_read_frame(mFormatContext, packet);
		if (res >= 0)
		{
			packet->pts = packet->dts;
			if (packet->pts == AV_NOPTS_VALUE)
			{
				//Must be commented
				//av_packet_free(&packet);
				//continue;
			}
			AVMediaType mediaType = (AVMediaType)mFormatContext->streams[packet->stream_index]->codecpar->codec_type;
			if (mediaType == AVMEDIA_TYPE_AUDIO)
			{
				if (packet->stream_index == mAudioStreamIndex)
				{
					OnAudioPacketReceived(packet);
				}
				else
				{
					av_packet_free(&packet);
				}
			}
			else if (mediaType == AVMEDIA_TYPE_VIDEO)
			{
				if (packet->stream_index == mVideoStreamIndex)
				{
					OnVideoPacketReceived(packet);
				}
				else
				{
					av_packet_free(&packet);
				}
			}
			else
			{
				av_packet_free(&packet);
			}
		}
		else
		{
			if (res == AVERROR_EOF)	mIsEOF.store(true);
			av_packet_free(&packet);
		}
	}
}

void CFFMpegPlayer::OnVideoPacketReceived(AVPacket* packet)
{
	std::vector<AVFrame*> decodedFrames;
	std::unique_lock<std::mutex> lock1(*mMutexDecodeVideo); // Lock the mutex
	if (mFFDecodeVideo != NULL)	mFFDecodeVideo->Decode(mFormatContext, packet, decodedFrames);
	lock1.unlock();
	for (AVFrame* decodedFrame : decodedFrames)
	{
		std::unique_lock<std::mutex> lock2(*mMutexVideoFrameBuffer); // Lock the mutex
		if (mVideoFrameBuffer != NULL) mVideoFrameBuffer->Enqueue(decodedFrame);
		lock2.unlock();
	}
	av_packet_free(&packet);
}

void CFFMpegPlayer::OnAudioPacketReceived(AVPacket* packet)
{
	std::vector<AVFrame*> decodedFrames;
	std::unique_lock<std::mutex> lock1(*mMutexDecodeAudio); // Lock the mutex
	if (mFFDecodeAudio != NULL)	mFFDecodeAudio->Decode(mFormatContext, packet, decodedFrames);
	lock1.unlock();
	for (AVFrame* decodedFrame : decodedFrames)
	{
		std::unique_lock<std::mutex> lock2(*mMutexAudioFrameBuffer); // Lock the mutex
		if (mAudioFrameBuffer != NULL) mAudioFrameBuffer->Enqueue(decodedFrame);
		lock2.unlock();
	}
	av_packet_free(&packet);
}

void CFFMpegPlayer::OnVideoFrameReceivedStatic(void* user, AVFrame* frame, INT deviation)
{
	if (user == NULL) return;
	CFFMpegPlayer* cFFMpegPlayer = (CFFMpegPlayer*)user;
	cFFMpegPlayer->OnVideoFrameReceived(frame, deviation);
}

void CFFMpegPlayer::OnVideoFrameReceived(AVFrame* frame, INT deviation)
{
	std::unique_lock<std::mutex> lock2(*mMutexSeek); // Lock the mutex
	if (frame->pts == AV_NOPTS_VALUE) 
	{
		av_frame_free(&frame);
		return;
	}
	if (mPlayerIsSeeking.load())
	{
		if (mPlayerIsSeekingFrameCounter.load() > 1)
		{
			mPlayerIsSeekingFrameCounter--;
			av_frame_free(&frame);
			return;
		}
		else if (mPlayerIsSeekingFrameCounter.load() == 1)
		{
			mPlayerIsSeekingFrameCounter--;
			if (mPlayerPausedOnSeek.load())
			{
				ResetEvent(mPlayerPausedEvent);
			}
			else
			{
				SetEvent(mPlayerPausedEvent);
			}
		}
		else //if (mPlayerIsSeekingFrameCounter.load() == 0)
		{
			mPlayerIsSeeking.store(false);
		}
	}
	mCurrentPlayingTime.store(frame->pts);
	lock2.unlock();
	mDeviationBuffer->AddDeviation(deviation);
	//printf("Video: Delay = %d, Deviation = %d\n", mDeviationBuffer->GetDelayTimeInUS(), deviation);
	//fflush(stdout);
	std::this_thread::sleep_for(std::chrono::microseconds(mDeviationBuffer->GetDelayTimeInUS()));
	//WaitBetweenFrames(AVMEDIA_TYPE_VIDEO, mLastVideoTime, (INT64)((double)frame->pts * 1.0));
	AVFrame* convertedFrame = NULL;
	std::unique_lock<std::mutex> lock1(*mMutexColorConversion); // Lock the mutex
	if (mFFColorConversion != NULL)
	{
		double widthHeightRatio = (double)frame->width / (double)frame->height;
		int ResizedHeight = frame->height;
		int ResizedWidth = (int)((double)ResizedHeight * widthHeightRatio);
		ResizedWidth &= 0xFFFFFFF8;
		int res = 0;
		res = mFFColorConversion->PerformColorConversion(mFormatContext, frame, convertedFrame, { AVPixelFormat::AV_PIX_FMT_RGB32, ResizedWidth, ResizedHeight });
		if (res >= 0)
		{
			if (convertedFrame != NULL)
			{
				if (mOnNewVideoFrame != NULL)
				{
					if (!mClosing.load())
					{
						mOnNewVideoFrame(mUser, convertedFrame);
					}
					else
					{
						av_frame_free(&convertedFrame);
					}
				}
				else
				{
					av_frame_free(&convertedFrame);
				}
			}
		}
	}
	lock1.unlock();
	av_frame_free(&frame);
}

void CFFMpegPlayer::OnAudioFrameReceivedStatic(void* user, AVFrame* frame, INT deviation)
{
	if (user == NULL) return;
	CFFMpegPlayer* cFFMpegPlayer = (CFFMpegPlayer*)user;
	cFFMpegPlayer->OnAudioFrameReceived(frame, deviation);
}

void CFFMpegPlayer::OnAudioFrameReceived(AVFrame* frame, INT deviation)
{
	if (frame->pts == AV_NOPTS_VALUE)
	{
		//Must be commented
		//av_frame_free(&frame);
		//return;
	}
	if (mPlayerIsSeeking.load())
	{
		av_frame_free(&frame);
		return;
	}
	//WaitBetweenFrames(AVMEDIA_TYPE_AUDIO, mLastAudioTime, (INT64)((double)frame->pts * mAudioCoefficient));
	AVFrame* convertedFrame = NULL;
	std::unique_lock<std::mutex> lock1(*mMutexSampleConversion); // Lock the mutex
	if (mFFSampleConversion != NULL)
	{
		AVChannelLayout ch_layout;
		av_channel_layout_default(&ch_layout, 2); //2 Channels
		int res = 0;
		res = mFFSampleConversion->PerformSampleConversion(mFormatContext, frame, convertedFrame, { AVSampleFormat::AV_SAMPLE_FMT_S16, ch_layout, 44100 });
		if (res >= 0)
		{
			if (convertedFrame != NULL)
			{
				if (mOnNewAudioFrame != NULL)
				{
					if (!mClosing.load())
					{
						mOnNewAudioFrame(mUser, convertedFrame);
					}
				}
			}
		}
	}
	lock1.unlock();
	av_frame_free(&convertedFrame);
	av_frame_free(&frame);
}

INT64 CFFMpegPlayer::PtsToMS(AVMediaType mediaType, INT64 pts)
{
	if (!mFormatContext) return 0;
	AVRational time_base = mFormatContext->streams[(mediaType == AVMEDIA_TYPE_VIDEO) ? mVideoStreamIndex : mAudioStreamIndex]->time_base;
	INT64 ms = (INT64)((double)pts * av_q2d(time_base) * 1000.0);
	return ms;
}

INT64 CFFMpegPlayer::MSToPts(AVMediaType mediaType, INT64 ms)
{
	if (!mFormatContext) return 0;
	AVRational time_base = mFormatContext->streams[(mediaType == AVMEDIA_TYPE_VIDEO) ? mVideoStreamIndex : mAudioStreamIndex]->time_base;
	int64_t pts = (INT64)((double)ms / av_q2d(time_base) / 1000.0);
	return pts;
}

void CFFMpegPlayer::WaitBetweenFrames(AVMediaType mediaType, TimeData& lastTime, INT64 pts)
{
	INT64 ms = PtsToMS(mediaType, pts);
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::steady_clock::duration diffMS = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime.Time);
	std::chrono::steady_clock::duration ms50 = std::chrono::milliseconds(50);
	if ((diffMS > ms50) || (ms - lastTime.MS > 50)) lastTime = { now, ms };
	while (std::chrono::steady_clock::now() < lastTime.Time + std::chrono::milliseconds(ms - lastTime.MS)) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	lastTime = { std::chrono::steady_clock::now(), ms };
}

void CFFMpegPlayer::MyLogCallbackFunctionStatic(void* ptr, int level, const char* fmt, va_list vl)
{
#if DEBUG_MODE
	va_list vl2;
	char line[1024];
	int print_prefix = 1;
	int report_file_level = AV_LOG_DEBUG;
	va_copy(vl2, vl);
	av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
	va_end(vl2);
	if (report_file_level >= level)
	{
		OutputDebugString(line);
	}
#endif // DEBUG_MODE
}

void CFFMpegPlayer::FramePTS(AVFrame* frame, INT64** pts)
{
	*pts = &frame->pts;
}

void CFFMpegPlayer::ClearAllBuffers()
{
	std::unique_lock<std::mutex> lock1(*mMutexDecodeAudio); // Lock the mutex
	if (mFFDecodeAudio != NULL)
	{
		mFFDecodeAudio->FlushBuffers();
	}
	lock1.unlock();
	std::unique_lock<std::mutex> lock2(*mMutexDecodeVideo); // Lock the mutex
	if (mFFDecodeVideo != NULL)
	{
		mFFDecodeVideo->FlushBuffers();
	}
	lock2.unlock();
	std::unique_lock<std::mutex> lock3(*mMutexAudioFrameBuffer); // Lock the mutex
	if (mAudioFrameBuffer != NULL)
	{
		mAudioFrameBuffer->Clear();
	}
	lock3.unlock();
	std::unique_lock<std::mutex> lock4(*mMutexVideoFrameBuffer); // Lock the mutex
	if (mVideoFrameBuffer != NULL)
	{
		mVideoFrameBuffer->Clear();
	}
	lock4.unlock();
}

BOOL CFFMpegPlayer::IsEventSet(HANDLE event)
{
	return (WaitForSingleObject(event, 0) == WAIT_OBJECT_0);
}