#include "stdafx.h"
#include "CFFColorConversion.h"
#include <tchar.h>

CFFColorConversion::CFFColorConversion()
{
	mPicture_Buf_RGB = NULL;
	mImg_Convert_Context = NULL;
	mColorTargetParams = {};
	mResourcesMustBeReallocated = FALSE;
}

CFFColorConversion::~CFFColorConversion()
{
	DeallocateResources();
}

BOOL CFFColorConversion::AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams)
{
	if (!formatContext) return FALSE;
	if (!in_frame) return FALSE;
	//-------------------------------------------------------
	mImg_Convert_Context = sws_getContext(in_frame->width,
		in_frame->height,
		(AVPixelFormat)in_frame->format,
		colorTargetParams.ResizedWidth,
		colorTargetParams.ResizedHeight,
		colorTargetParams.PixelFormatsID,
		SWS_BICUBIC, NULL, NULL, NULL);
	if (!mImg_Convert_Context) return FALSE;
	//-------------------------------------------------------
	int sizeRGB = av_image_get_buffer_size(colorTargetParams.PixelFormatsID, colorTargetParams.ResizedWidth, colorTargetParams.ResizedHeight, 1);
	mPicture_Buf_RGB = (uint8_t*)(av_malloc(sizeRGB));
	if (!mPicture_Buf_RGB) return FALSE;
	//-------------------------------------------------------
	return TRUE;
}

BOOL CFFColorConversion::DeallocateResources()
{
	if (mPicture_Buf_RGB != NULL)
	{
		av_free(mPicture_Buf_RGB);
		mPicture_Buf_RGB = NULL;
	}
	//-------------------------------------------------------
	if (mImg_Convert_Context != NULL)
	{
		av_free(mImg_Convert_Context);
		mImg_Convert_Context = NULL;
	}
	//-------------------------------------------------------
	return TRUE;
}

void CFFColorConversion::ReallocateResources()
{
	mResourcesMustBeReallocated = TRUE;
}

BOOL CFFColorConversion::AreParamsChanged(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams)
{
	if (mResourcesMustBeReallocated)
	{
		mResourcesMustBeReallocated = FALSE;
		return TRUE;
	}
	if (memcmp(&mColorTargetParams, &colorTargetParams, sizeof(CFFCommon::ColorTargetParams)) == 0)
	{
		memcpy(&mColorTargetParams, &colorTargetParams, sizeof(CFFCommon::ColorTargetParams));
		return TRUE;
	}
	if (mImg_Convert_Context == NULL) return TRUE;
	return FALSE;
}

int CFFColorConversion::PerformColorConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::ColorTargetParams colorTargetParams)
{
	AVDictionaryEntry* entry = av_dict_get(in_frame->metadata, "WasReallocated", NULL, 0);
	if (entry != NULL)
	{
		if (strcmp(entry->value, "True") == 0)
		{
			ReallocateResources();
		}
	}
	if (AreParamsChanged(formatContext, in_frame, colorTargetParams))
	{
		DeallocateResources();
		AllocateResources(formatContext, in_frame, colorTargetParams);
	}
	AVFrame* tempFrame = av_frame_alloc();
	int res = 0;
	res = av_image_fill_arrays(tempFrame->data, tempFrame->linesize, mPicture_Buf_RGB, colorTargetParams.PixelFormatsID, colorTargetParams.ResizedWidth, colorTargetParams.ResizedHeight, 1);
	if (res < 0)
	{
		av_frame_free(&tempFrame);
		return res;
	}
	res = sws_scale(mImg_Convert_Context, in_frame->data, in_frame->linesize, 0, in_frame->height, tempFrame->data, tempFrame->linesize);
	if (res < 0)
	{
		av_frame_free(&tempFrame);
		return res;
	}
	tempFrame->width = colorTargetParams.ResizedWidth;
	tempFrame->height = colorTargetParams.ResizedHeight;
	tempFrame->format = colorTargetParams.PixelFormatsID;
	tempFrame->pts = in_frame->pts;
	tempFrame->pkt_dts = in_frame->pkt_dts;
	out_frame = tempFrame;
	return 0;
}
