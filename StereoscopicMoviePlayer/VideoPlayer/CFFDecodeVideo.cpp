#include "stdafx.h"
#include "CFFDecodeVideo.h"
#include <tchar.h>

CFFDecodeVideo::CFFDecodeVideo(void* user, dOnNewDecodedFrame onNewDecodedFrame) : CFFDecodeBase(user, onNewDecodedFrame)
{
	mLastWidth = 0;
	mLastHeight = 0;
}

CFFDecodeVideo::~CFFDecodeVideo()
{
}

BOOL CFFDecodeVideo::AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet)
{
	if (mResourcesMustBeReallocated)
	{
		mResourcesMustBeReallocated = FALSE;
		return TRUE;
	}
	if (formatContext != NULL)
	{
		if (mLastWidth != formatContext->streams[packet->stream_index]->codecpar->width)
		{
			mLastWidth = formatContext->streams[packet->stream_index]->codecpar->width;
			return TRUE;
		}
	}
	if (formatContext != NULL)
	{
		if (mLastHeight != formatContext->streams[packet->stream_index]->codecpar->height)
		{
			mLastHeight = formatContext->streams[packet->stream_index]->codecpar->height;
			return TRUE;
		}
	}
	if (mCodecContext == NULL) return TRUE;
	return FALSE;
}

AVDictionary* CFFDecodeVideo::SetOpts()
{
	AVDictionary* opts = NULL;
	av_dict_set(&opts, "b", "2.5M", 0);
	return opts;
}
