#include "stdafx.h"
#include "CFFMpegPlayer.h"
#include <tchar.h>
#include "../Common/CTools.h"

BOOL CFFMpegPlayer::mStaticInitialized = FALSE;

CFFMpegPlayer::CFFMpegPlayer(void* user, dOnNewVideoFrame onNewVideoFrame, dOnNewAudioFrame onNewAudioFrame)
{
	InitStatic();
	//-------------------------------------------------------
	mPlayerMutex = new std::mutex();
	mPlayerThread = nullptr;
	mPlayerThreadRunning.store(false);
	mPlayerPaused.store(false);
	mPlayerPausedOnSeek.store(false);
	mPlayerIsSeeking.store(false);
	mPlayerSeekRequest.store(false);
	//-------------------------------------------------------
	mUser = user;
	mOnNewVideoFrame = onNewVideoFrame;
	mOnNewAudioFrame = onNewAudioFrame;
	//-------------------------------------------------------
	mFileName = std::wstring(L"");
	//-------------------------------------------------------
	mFormatContext = NULL;
	//-------------------------------------------------------
	mVideoDuration.store(0);
	mVideoTracksNumber.store(0);
	mVideoStreamIndex.store(-1);
	mAudioTracksNumber.store(0);
	mAudioStreamIndex.store(-1);
	mCurrentPlayingTime.store(0);
	mSeekTime.store(0);
	//-------------------------------------------------------
	mFFDecodeVideo = NULL;
	mFFDecodeAudio = NULL;
	mFFColorConversion = NULL;
	mFFSampleConversion = NULL;
	//-------------------------------------------------------
	avformat_network_init();
	avdevice_register_all();
}

CFFMpegPlayer::~CFFMpegPlayer()
{
	Close();
	if (mPlayerMutex != nullptr)
	{
		delete mPlayerMutex;
		mPlayerMutex = nullptr;
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
	//-------------------------------------------------------
	if (mFFDecodeVideo == NULL) mFFDecodeVideo = new CFFDecodeVideo(this, OnNewDecodedVideoFrameStatic);
	if (mFFDecodeAudio == NULL) mFFDecodeAudio = new CFFDecodeAudio(this, OnNewDecodedAudioFrameStatic);
	if (mFFColorConversion == NULL) mFFColorConversion = new CFFColorConversion();
	if (mFFSampleConversion == NULL) mFFSampleConversion = new CFFSampleConversion();
	//-------------------------------------------------------
	if (mFFDecodeVideo != NULL) mFFDecodeVideo->ReallocateResources();
	if (mFFDecodeAudio != NULL) mFFDecodeAudio->ReallocateResources();
	if (mFFColorConversion != NULL) mFFColorConversion->ReallocateResources();
	if (mFFSampleConversion != NULL) mFFSampleConversion->ReallocateResources();
	//-------------------------------------------------------
	mFileName = fileName;
	std::string fileNameA = CTools::ConvertUnicodeToMultibyte(fileName);
	mFormatContext = avformat_alloc_context();
	if (avformat_open_input(&mFormatContext, fileNameA.c_str(), NULL, NULL) < 0) return;
	if (avformat_find_stream_info(mFormatContext, NULL) < 0) return;
	//-------------------------------------------------------
	if (!mPlayerThreadRunning.load())
	{
		mPlayerThreadRunning.store(true);
		mPlayerThread = new std::thread(&CFFMpegPlayer::MyThreadPlayerFunction, this);
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
}

void CFFMpegPlayer::Close()
{
	if (mPlayerThreadRunning.load())
	{
		mPlayerPaused.store(false);
		mPlayerThreadRunning.store(false);
		if (mPlayerThread && mPlayerThread->joinable())
		{
			mPlayerThread->join();
		}
		delete mPlayerThread;
		mPlayerThread = nullptr;
	}
	std::unique_lock<std::mutex> lock1(*mPlayerMutex); // Lock the mutex
	//--------------------------------------------------------------
	if (mFormatContext != NULL)
	{
		avformat_close_input(&mFormatContext);
		avformat_free_context(mFormatContext);
		mFormatContext = NULL;
	}
	if (mFFSampleConversion != NULL)
	{
		delete mFFSampleConversion;
		mFFSampleConversion = NULL;
	}
	if (mFFColorConversion != NULL)
	{
		delete mFFColorConversion;
		mFFColorConversion = NULL;
	}
	if (mFFDecodeAudio != NULL)
	{
		delete mFFDecodeAudio;
		mFFDecodeAudio = NULL;
	}
	if (mFFDecodeVideo != NULL)
	{
		delete mFFDecodeVideo;
		mFFDecodeVideo = NULL;
	}
	//--------------------------------------------------------------
	lock1.unlock();
}

void CFFMpegPlayer::Reopen()
{
	if (mPlayerThreadRunning.load()) Close();
	Open(mFileName);
}

BOOL CFFMpegPlayer::IsOpened()
{
	return mPlayerThreadRunning.load();
}

void CFFMpegPlayer::Play()
{
	if (mPlayerThreadRunning.load())
	{
		mPlayerPaused.store(false);
	}
}

void CFFMpegPlayer::Pause()
{
	if (mPlayerThreadRunning.load())
	{
		mPlayerPaused.store(true);
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
	return mPlayerThreadRunning.load() && !mPlayerPaused.load();
}

INT64 CFFMpegPlayer::GetDuration()
{
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			if (mFormatContext->duration != AV_NOPTS_VALUE) {
				// Duration is in AV_TIME_BASE units (usually microseconds)
				int64_t duration_microseconds = mFormatContext->duration;
				double duration_seconds = duration_microseconds / (double)AV_TIME_BASE;
				return duration_seconds;
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
			AVRational time_base = mFormatContext->streams[mVideoStreamIndex]->time_base;
			int64_t target_ts = mCurrentPlayingTime.load() * av_q2d(time_base);
			return target_ts;
		}
	}
	return 0;
}

void CFFMpegPlayer::Seek(INT64 seek_target_seconds)
{
	if (mVideoStreamIndex < 0) return;
	if (mPlayerThreadRunning.load())
	{
		if (mFormatContext != NULL)
		{
			AVRational time_base = mFormatContext->streams[mVideoStreamIndex]->time_base;
			int64_t target_ts = seek_target_seconds / av_q2d(time_base);
			mPlayerSeekRequest.store(true);
			mPlayerIsSeeking.store(true);
			mPlayerPausedOnSeek.store(mPlayerPaused.load());
			mPlayerPaused.store(false);
			mSeekTime.store(target_ts);
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

void CFFMpegPlayer::MyThreadPlayerFunction()
{
	while (mPlayerThreadRunning.load())
	{
		AVPacket* packet;
		packet = av_packet_alloc();
		//----------------------------------------------------------------
		std::unique_lock<std::mutex> lock1(*mPlayerMutex); // Lock the mutex
		//----------------------------------------------------------------
		if (mPlayerSeekRequest.load())
		{
			avformat_seek_file(mFormatContext, mVideoStreamIndex, INT64_MIN, mSeekTime.load(), INT64_MAX, 0);
			mPlayerSeekRequest.store(false);
		}
		if (av_read_frame(mFormatContext, packet) >= 0)
		{
			AVMediaType mediaType = (AVMediaType)mFormatContext->streams[packet->stream_index]->codecpar->codec_type;
			if (mediaType == AVMEDIA_TYPE_AUDIO)
			{
				if (packet->stream_index == mAudioStreamIndex)
				{
					OnAudioPacketReceived(mFormatContext, packet);
				}
			}
			else if (mediaType == AVMEDIA_TYPE_VIDEO)
			{
				if (packet->stream_index == mVideoStreamIndex)
				{
					OnVideoPacketReceived(mFormatContext, packet);
				}
			}
		}
		av_packet_unref(packet);
		//----------------------------------------------------------------
		lock1.unlock();
		//----------------------------------------------------------------
		av_packet_free(&packet);
		//----------------------------------------------------------------
		while (mPlayerPaused.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	}
}

void CFFMpegPlayer::OnVideoPacketReceived(AVFormatContext* formatContext, AVPacket* packet)
{
	if (mFFDecodeVideo != NULL)
	{
		mFFDecodeVideo->Decode(formatContext, packet);
	}
}

void CFFMpegPlayer::OnAudioPacketReceived(AVFormatContext* formatContext, AVPacket* packet)
{
	if (mFFDecodeAudio != NULL)
	{
		mFFDecodeAudio->Decode(formatContext, packet);
	}
}

void CFFMpegPlayer::OnNewDecodedVideoFrameStatic(void* user, AVFrame* decodedFrame)
{
	if (user == NULL) return;
	CFFMpegPlayer* cFFMpegPlayer = (CFFMpegPlayer*)user;
	cFFMpegPlayer->OnNewDecodedVideoFrame(decodedFrame);
}

void CFFMpegPlayer::OnNewDecodedVideoFrame(AVFrame* decodedFrame)
{
	mCurrentPlayingTime.store(decodedFrame->pts);
	if (mPlayerIsSeeking.load())
	{
		if ((-500 < mSeekTime.load() - decodedFrame->pts) && (mSeekTime.load() - decodedFrame->pts < 500))
		{
			mPlayerPaused.store(mPlayerPausedOnSeek.load());
			mPlayerIsSeeking.store(false);
		}
		if (mPlayerIsSeeking.load()) return;
	}
	if (mFFColorConversion != NULL)
	{
		AVFrame* convertedFrame = NULL;
		double widthHeightRatio = (double)decodedFrame->width / (double)decodedFrame->height;
		int ResizedHeight = min(decodedFrame->height, 480);
		int ResizedWidth = (int)((double)ResizedHeight * widthHeightRatio);
		ResizedWidth &= 0xFFFFFFF8;
		mFFColorConversion->PerformColorConversion(mFormatContext, decodedFrame, convertedFrame, { AVPixelFormat::AV_PIX_FMT_RGB32, ResizedWidth, ResizedHeight });
		if (convertedFrame != NULL)
		{
			if (mOnNewVideoFrame != NULL)
			{
				mOnNewVideoFrame(mUser, (BYTE*)convertedFrame->data[0], convertedFrame->width, convertedFrame->height, convertedFrame->linesize[0] / convertedFrame->width, convertedFrame->pts);
			}
		}
	}
	av_frame_unref(decodedFrame);
}

void CFFMpegPlayer::OnNewDecodedAudioFrameStatic(void* user, AVFrame* decodedFrame)
{
	if (user == NULL) return;
	CFFMpegPlayer* cFFMpegPlayer = (CFFMpegPlayer*)user;
	cFFMpegPlayer->OnNewDecodedAudioFrame(decodedFrame);
}

void CFFMpegPlayer::OnNewDecodedAudioFrame(AVFrame* decodedFrame)
{
	if (mPlayerIsSeeking.load())
	{
		return;
	}
	if (mFFSampleConversion != NULL)
	{
		AVFrame* convertedFrame = NULL;
		AVChannelLayout ch_layout;
		av_channel_layout_default(&ch_layout, 2); //2 Channels
		mFFSampleConversion->PerformSampleConversion(mFormatContext, decodedFrame, convertedFrame, { AVSampleFormat::AV_SAMPLE_FMT_S16, ch_layout, 44100 });
		if (convertedFrame != NULL)
		{
			if (mOnNewAudioFrame != NULL)
			{
				int bitsPerSample = 0;
				switch (convertedFrame->format)
				{
				case AV_SAMPLE_FMT_U8:
					bitsPerSample = 8;
					break;
				case AV_SAMPLE_FMT_S16:
					bitsPerSample = 16;
					break;
				case AV_SAMPLE_FMT_S32:
					bitsPerSample = 32;
					break;
				case AV_SAMPLE_FMT_FLT:
					bitsPerSample = 32;
					break;
				case AV_SAMPLE_FMT_DBL:
					bitsPerSample = 64;
					break;
				case AV_SAMPLE_FMT_U8P:
					bitsPerSample = 8;
					break;
				case AV_SAMPLE_FMT_S16P:
					bitsPerSample = 16;
					break;
				case AV_SAMPLE_FMT_S32P:
					bitsPerSample = 32;
					break;
				case AV_SAMPLE_FMT_FLTP:
					bitsPerSample = 32;
					break;
				case AV_SAMPLE_FMT_DBLP:
					bitsPerSample = 64;
					break;
				}
				mOnNewAudioFrame(mUser, convertedFrame->data[0], convertedFrame->nb_samples, convertedFrame->sample_rate, bitsPerSample, convertedFrame->ch_layout.nb_channels, convertedFrame->pts);
			}
		}
	}
	av_frame_unref(decodedFrame);
}

void CFFMpegPlayer::MyLogCallbackFunctionStatic(void* ptr, int level, const char* fmt, va_list vl)
{
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
}