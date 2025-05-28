#ifndef __CFFDECODEVIDEO_H__
#define __CFFDECODEVIDEO_H__

#include "CFFCommon.h"
#include "CFFDecodeBase.h"

class CFFDecodeVideo : public CFFDecodeBase
{
private:
	INT mLastWidth;
	INT mLastHeight;
private:
	BOOL AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet);
	AVDictionary* SetOpts();
public:
	CFFDecodeVideo();
	~CFFDecodeVideo();
};

#endif //__CFFDECODEVIDEO_H__