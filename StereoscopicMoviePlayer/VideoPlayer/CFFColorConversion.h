#ifndef __CFFCOLORCONVERSION_H__
#define __CFFCOLORCONVERSION_H__

#include "CFFCommon.h"

class CFFColorConversion
{
private:
    AVFilterGraph* mFilterGraph;
    AVFilterContext* mBuffersrcCtx;
    AVFilterContext* mBuffersinkCtx;
    AVBufferRef* m_hw_device_ctx;
    CFFCommon::ColorTargetParams mColorTargetParams;
    BOOL mResourcesMustBeReallocated;
private:
    BOOL AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams);
    BOOL DeallocateResources();
    BOOL AreParamsChanged(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams);
public:
    CFFColorConversion();
    ~CFFColorConversion();
    void ReallocateResources();
    int PerformColorConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::ColorTargetParams colorTargetParams);
};

#endif // __CFFCOLORCONVERSION_H__