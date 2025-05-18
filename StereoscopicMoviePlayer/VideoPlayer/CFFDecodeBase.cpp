#include "stdafx.h"
#include "CFFDecodeBase.h"
#include <tchar.h>

CFFDecodeBase::CFFDecodeBase(void* user, dOnNewDecodedFrame onNewDecodedFrame)
{
	mUser = user;
	mOnNewDecodedFrame = onNewDecodedFrame;
	mCodec = NULL;
	mDecodedFrame = NULL;
	mCodecContext = NULL;
	mResourcesMustBeReallocated = FALSE;
}

CFFDecodeBase::~CFFDecodeBase()
{
	DeallocateResources();
}

BOOL CFFDecodeBase::AllocateResources(AVFormatContext* formatContext, AVPacket* packet)
{
	if (!formatContext) return FALSE;
	if (!packet) return FALSE;
	//-------------------------------------------------------
	mCodec = (AVCodec*)avcodec_find_decoder(formatContext->streams[packet->stream_index]->codecpar->codec_id);
	if (!mCodec) return FALSE;
	//-------------------------------------------------------
	mCodecContext = avcodec_alloc_context3(mCodec);
	if (!mCodecContext) return FALSE;
	//-------------------------------------------------------
	if (avcodec_parameters_to_context(mCodecContext, formatContext->streams[packet->stream_index]->codecpar) < 0) return FALSE;
	mCodecContext->thread_count = 0; // Let FFmpeg decide
	mCodecContext->thread_type = FF_THREAD_FRAME; // or FF_THREAD_SLICE
	//-------------------------------------------------------
	AVDictionary* opts = SetOpts();
	if (avcodec_open2(mCodecContext, mCodec, &opts) < 0) return FALSE;
	//-------------------------------------------------------
	mDecodedFrame = av_frame_alloc();
	return TRUE;
}

BOOL CFFDecodeBase::DeallocateResources()
{
	if (mDecodedFrame != NULL)
	{
		av_free(mDecodedFrame);
		mDecodedFrame = NULL;
	}
	if (mCodecContext != NULL)
	{
		avcodec_free_context(&mCodecContext);
		mCodecContext = NULL;
	}
	//mCodec must not be disposed.
	return (TRUE);
}

void CFFDecodeBase::ReallocateResources()
{
	mResourcesMustBeReallocated = TRUE;
}

int CFFDecodeBase::Decode(AVFormatContext* formatContext, AVPacket* packet)
{
	BOOL wasReallocated = FALSE;
	if (AreParamsChanged(formatContext, packet))
	{
		DeallocateResources();
		AllocateResources(formatContext, packet);
		wasReallocated = TRUE;
	}
	//-------------------------------------------------------
	if (!mCodecContext) return (-1);
	if (!mDecodedFrame) return (-1);
	if (avcodec_send_packet(mCodecContext, packet) < 0) return (-1);
	while (avcodec_receive_frame(mCodecContext, mDecodedFrame) >= 0)
	{
		if (mOnNewDecodedFrame) 
		{
			if (wasReallocated)
			{
				av_dict_set(&mDecodedFrame->metadata, "WasReallocated", "True", 0);
			}
			else
			{
				av_dict_set(&mDecodedFrame->metadata, "WasReallocated", "False", 0);
			}
			mOnNewDecodedFrame(mUser, mDecodedFrame);
			wasReallocated = FALSE;
		}
	}
	return 0;
}