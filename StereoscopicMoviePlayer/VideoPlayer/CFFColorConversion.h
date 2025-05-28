#ifndef __CFFCOLORCONVERSION_H__
#define __CFFCOLORCONVERSION_H__

#include "CFFCommon.h"

class CFFColorConversion
{
private:
	uint8_t*                     mPicture_Buf_RGB;
	SwsContext*                  mImg_Convert_Context;
	CFFCommon::ColorTargetParams mColorTargetParams;
	BOOL                         mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(AVFormatContext* formatContext, AVFrame* in_frame, CFFCommon::ColorTargetParams colorTargetParams);
public:
	CFFColorConversion();
	~CFFColorConversion();
	int PerformColorConversion(AVFormatContext* formatContext, AVFrame* in_frame, AVFrame*& out_frame, CFFCommon::ColorTargetParams colorTargetParams);
	void ReallocateResources();
};

#endif //__CFFCOLORCONVERSION_H__