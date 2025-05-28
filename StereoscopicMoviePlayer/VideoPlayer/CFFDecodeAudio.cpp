#include "stdafx.h"
#include "CFFDecodeAudio.h"
#include <tchar.h>

CFFDecodeAudio::CFFDecodeAudio() : CFFDecodeBase()
{
	mLastCodecID = AVCodecID::AV_CODEC_ID_AAC;
	mLastSampleRate = 0;
}

CFFDecodeAudio::~CFFDecodeAudio()
{
}

BOOL CFFDecodeAudio::AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet)
{
	if (mResourcesMustBeReallocated)
	{
		mResourcesMustBeReallocated = FALSE;
		return TRUE;
	}
	if (formatContext != NULL)
	{
		if (mLastCodecID != formatContext->streams[packet->stream_index]->codecpar->codec_id)
		{
			mLastCodecID = formatContext->streams[packet->stream_index]->codecpar->codec_id;
			return TRUE;
		}
	}
	if (formatContext != NULL)
	{
		if (mLastSampleRate != formatContext->streams[packet->stream_index]->codecpar->sample_rate)
		{
			mLastSampleRate = formatContext->streams[packet->stream_index]->codecpar->sample_rate;
			return TRUE;
		}
	}
	if (mCodecContext == NULL) return TRUE;
	return FALSE;
}

AVDictionary* CFFDecodeAudio::SetOpts()
{
	AVDictionary* opts = NULL;
	return opts;
}
