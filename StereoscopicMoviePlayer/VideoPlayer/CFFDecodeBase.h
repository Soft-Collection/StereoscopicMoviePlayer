#ifndef __CFFDECODEBASE_H__
#define __CFFDECODEBASE_H__

#include "CFFCommon.h"
#include <vector>

class CFFDecodeBase
{
protected:
	AVCodec*           mCodec;
	AVCodecContext*    mCodecContext;
	BOOL               mResourcesMustBeReallocated;
protected:
	BOOL AllocateResources(AVFormatContext* formatContext, AVPacket* packet);
	BOOL DeallocateResources();
	virtual BOOL AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet) = 0;
	virtual AVDictionary* SetOpts() = 0;
public:
	void ReallocateResources();
	void FlushBuffers();
public:
	CFFDecodeBase();
	~CFFDecodeBase();
	int Decode(AVFormatContext* formatContext, AVPacket* packet, std::vector<AVFrame*>& decodedFrames);
};

#endif //__CFFDECODEBASE_H__