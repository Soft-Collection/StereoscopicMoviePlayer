#include "stdafx.h"
#include "CFFDecodeBase.h"
#include <tchar.h>

CFFDecodeBase::CFFDecodeBase()
{
	mCodec = NULL;
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
	return TRUE;
}

BOOL CFFDecodeBase::DeallocateResources()
{
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

void CFFDecodeBase::FlushBuffers()
{
	if (mCodecContext) avcodec_flush_buffers(mCodecContext);
}

int CFFDecodeBase::Decode(AVFormatContext* formatContext, AVPacket* packet, std::vector<AVFrame*>& decodedFrames)
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
	if (avcodec_send_packet(mCodecContext, packet) < 0) return (-1);
	while (TRUE)
	{
		AVFrame* tempFrame = av_frame_alloc();
		if (!tempFrame) return -1;
		int res = avcodec_receive_frame(mCodecContext, tempFrame);
		if (res == AVERROR(EAGAIN) || res == AVERROR_EOF)
		{
			av_frame_free(&tempFrame);
			break;
		}
		else if (res < 0)
		{
			av_frame_free(&tempFrame);
			return res;
		}
		av_dict_set(&tempFrame->metadata, "WasReallocated", wasReallocated ? "True" : "False", 0);
		decodedFrames.push_back(tempFrame);
	}
	return 0;
}