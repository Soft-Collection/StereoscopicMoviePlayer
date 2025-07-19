#include "stdafx.h"
#include "CFFColorConversion.h"
#include <string>

CFFColorConversion::CFFColorConversion()
{
    mFilterGraph = nullptr;
    mBuffersrcCtx = nullptr;
    mBuffersinkCtx = nullptr;
    mResourcesMustBeReallocated = FALSE;
    mColorTargetParams = {};
    m_hw_device_ctx = nullptr;
}

CFFColorConversion::~CFFColorConversion()
{
    DeallocateResources();
}

BOOL CFFColorConversion::AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams)
{
    // Create DXVA2 device context
    av_hwdevice_ctx_create(&m_hw_device_ctx, AV_HWDEVICE_TYPE_DXVA2, nullptr, nullptr, 0);
    const AVFilter* buffersrc = avfilter_get_by_name("buffer");
    const AVFilter* buffersink = avfilter_get_by_name("buffersink");
    mFilterGraph = avfilter_graph_alloc();
    char args[512];
    snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=1/25:pixel_aspect=1/1", in_frame->width, in_frame->height, in_frame->format);
    if (avfilter_graph_create_filter(&mBuffersrcCtx, buffersrc, "in", args, nullptr, mFilterGraph) < 0) return FALSE;
    if (avfilter_graph_create_filter(&mBuffersinkCtx, buffersink, "out", nullptr, nullptr, mFilterGraph) < 0) return FALSE;
    AVFilterInOut* outputs = avfilter_inout_alloc();
    AVFilterInOut* inputs = avfilter_inout_alloc();
    outputs->name = av_strdup("in");
    outputs->filter_ctx = mBuffersrcCtx;
    outputs->pad_idx = 0;
    outputs->next = nullptr;
    inputs->name = av_strdup("out");
    inputs->filter_ctx = mBuffersinkCtx;
    inputs->pad_idx = 0;
    inputs->next = nullptr;
    char filterStr[256];
    snprintf(filterStr, sizeof(filterStr), "scale=width=%d:height=%d,format=pix_fmts=%s", colorTargetParams.ResizedWidth, colorTargetParams.ResizedHeight, av_get_pix_fmt_name((AVPixelFormat)colorTargetParams.PixelFormatsID));
    if (avfilter_graph_parse_ptr(mFilterGraph, filterStr, &inputs, &outputs, nullptr) < 0 || avfilter_graph_config(mFilterGraph, nullptr) < 0)
    {
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        return FALSE;
    }
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    memcpy(&mColorTargetParams, &colorTargetParams, sizeof(CFFCommon::ColorTargetParams));
    return TRUE;
}

BOOL CFFColorConversion::DeallocateResources()
{
    if (mFilterGraph)
    {
        avfilter_graph_free(&mFilterGraph);
        mFilterGraph = nullptr;
        mBuffersrcCtx = nullptr;
        mBuffersinkCtx = nullptr;
    }
    if (m_hw_device_ctx)
    {
        av_buffer_unref(&m_hw_device_ctx);
    }
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
    if (memcmp(&mColorTargetParams, &colorTargetParams, sizeof(CFFCommon::ColorTargetParams)) != 0) 
    {
        return TRUE;
    }
    if (mFilterGraph == nullptr || mBuffersrcCtx == nullptr || mBuffersinkCtx == nullptr) 
    {
        return TRUE;
    }
    return FALSE;
}

int CFFColorConversion::PerformColorConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::ColorTargetParams colorTargetParams)
{
    AVDictionaryEntry* entry = av_dict_get(in_frame->metadata, "WasReallocated", NULL, 0);
    if (entry && strcmp(entry->value, "True") == 0) 
    {
        ReallocateResources();
    }
    if (AreParamsChanged(formatContext, in_frame, colorTargetParams)) 
    {
        DeallocateResources();
        if (!AllocateResources(formatContext, in_frame, colorTargetParams)) return AVERROR(EINVAL);
    }
    if (av_buffersrc_add_frame_flags(mBuffersrcCtx, in_frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) return AVERROR(EINVAL);
    out_frame = av_frame_alloc();
    if (!out_frame) return AVERROR(ENOMEM);
    if (av_buffersink_get_frame(mBuffersinkCtx, out_frame) < 0) 
    {
        av_frame_free(&out_frame);
        return AVERROR(EINVAL);
    }
    out_frame->pts = in_frame->pts;
    out_frame->pkt_dts = in_frame->pkt_dts;
    return 0;
}
