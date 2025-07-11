#include "stdafx.h"
#include "CFFSampleConversion.h"
#include <tchar.h>

CFFSampleConversion::CFFSampleConversion()
{
	mSwrContext = NULL;
	mResourcesMustBeReallocated = FALSE;
	mSampleTargetParams = {};
}

CFFSampleConversion::~CFFSampleConversion()
{
	DeallocateResources();
}

BOOL CFFSampleConversion::AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::SampleTargetParams sampleTargetParams)
{
	if (!formatContext) return FALSE;
	if (!in_frame) return FALSE;
	// Set up SWR context once you've got codec information
	mSwrContext = swr_alloc();
	if (!mSwrContext) return FALSE;
	av_channel_layout_default(&in_frame->ch_layout, in_frame->ch_layout.nb_channels);
	if (swr_alloc_set_opts2(&mSwrContext,   // we're allocating a new context
		&sampleTargetParams.ChannelLayout,  // out_ch_layout
		sampleTargetParams.SampleFormatsID, // out_sample_fmt
		sampleTargetParams.SampleRate,      // out_sample_rate
		&in_frame->ch_layout,               // in_ch_layout
		(AVSampleFormat)in_frame->format,   // in_sample_fmt
		in_frame->sample_rate,              // in_sample_rate
		0,                                  // log_offset
		NULL) < 0) return FALSE;            // log_ctx
	if (swr_init(mSwrContext) < 0) return FALSE;
	return TRUE;
}

BOOL CFFSampleConversion::DeallocateResources()
{
	if (mSwrContext != NULL)
	{
		swr_free(&mSwrContext);
		mSwrContext = NULL;
	}
	return (TRUE);
}

void CFFSampleConversion::ReallocateResources()
{
	mResourcesMustBeReallocated = TRUE;
}

BOOL CFFSampleConversion::AreParamsChanged(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::SampleTargetParams sampleTargetParams)
{
	if (mResourcesMustBeReallocated)
	{
		mResourcesMustBeReallocated = FALSE;
		return TRUE;
	}
	if (memcmp(&mSampleTargetParams, &sampleTargetParams, sizeof(CFFCommon::SampleTargetParams)) == 0)
	{
		memcpy(&mSampleTargetParams, &sampleTargetParams, sizeof(CFFCommon::SampleTargetParams));
		return TRUE;
	}
	if (mSwrContext == NULL) return TRUE;
	return FALSE;
}

int CFFSampleConversion::PerformSampleConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::SampleTargetParams sampleTargetParams)
{
	AVDictionaryEntry* entry = av_dict_get(in_frame->metadata, "WasReallocated", NULL, 0);
	if (entry != NULL) 
	{
		if (strcmp(entry->value, "True") == 0)
		{
			ReallocateResources();
		}
	}
	if (AreParamsChanged(formatContext, in_frame, sampleTargetParams))
	{
		DeallocateResources();
		AllocateResources(formatContext, in_frame, sampleTargetParams);
	}
	AVFrame* tempFrame = av_frame_alloc();
	if (!tempFrame) return (-1);
	av_channel_layout_default(&in_frame->ch_layout, in_frame->ch_layout.nb_channels);
	tempFrame->ch_layout = sampleTargetParams.ChannelLayout;
	tempFrame->format = sampleTargetParams.SampleFormatsID;
	tempFrame->sample_rate = sampleTargetParams.SampleRate;
	tempFrame->nb_samples = in_frame->nb_samples;
	int res = 0;
	res = av_frame_get_buffer(tempFrame, 0);
	if (res < 0)
	{
		av_frame_free(&tempFrame);
		return res;
	}
	res = swr_convert_frame(mSwrContext, tempFrame, in_frame);
	if (res < 0)
	{
		av_frame_free(&tempFrame);
		return res;
	}
	tempFrame->pts = in_frame->pts;
	tempFrame->pkt_dts = in_frame->pkt_dts;
	out_frame = tempFrame;
	return 0;
}