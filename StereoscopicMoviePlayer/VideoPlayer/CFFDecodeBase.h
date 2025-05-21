#ifndef __CFFDECODEBASE_H__
#define __CFFDECODEBASE_H__

#include "CFFCommon.h"

typedef void(*dOnNewDecodedFrame)(void* user, AVFrame* decodedFrame);

class CFFDecodeBase
{
protected:
	dOnNewDecodedFrame mOnNewDecodedFrame;
	void*              mUser;
	AVCodec*           mCodec;
	AVCodecContext*    mCodecContext;
	AVFrame*           mDecodedFrame;
	BOOL               mResourcesMustBeReallocated;
protected:
	BOOL AllocateResources(AVFormatContext* formatContext, AVPacket* packet);
	BOOL DeallocateResources();
	virtual BOOL AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet) = 0;
	virtual AVDictionary* SetOpts() = 0;
public:
	void ReallocateResources();
public:
	CFFDecodeBase(void* user, dOnNewDecodedFrame onNewDecodedFrame);
	~CFFDecodeBase();
	int Decode(AVFormatContext* formatContext, AVPacket* packet);
};

#endif //__CFFDECODEBASE_H__