#ifndef __CFFSAMPLECONVERSION_H__
#define __CFFSAMPLECONVERSION_H__

#include "CFFCommon.h"

class CFFSampleConversion
{
private:
	SwrContext*                   mSwrContext;
	BOOL                          mResourcesMustBeReallocated;
	CFFCommon::SampleTargetParams mSampleTargetParams;
private:
	BOOL AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::SampleTargetParams sampleTargetParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::SampleTargetParams sampleTargetParams);
public:
	CFFSampleConversion();
	~CFFSampleConversion();
	int PerformSampleConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::SampleTargetParams sampleTargetParams);
	void ReallocateResources();
};

#endif //__CFFSAMPLECONVERSION_H__