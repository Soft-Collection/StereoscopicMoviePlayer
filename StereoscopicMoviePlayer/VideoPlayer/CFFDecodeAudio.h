#ifndef __CFFDECODEAUDIO_H__
#define __CFFDECODEAUDIO_H__

#include "CFFCommon.h"
#include "CFFDecodeBase.h"

class CFFDecodeAudio : public CFFDecodeBase
{
private:
	AVCodecID mLastCodecID;
	INT       mLastSampleRate;
private:
	BOOL AreParamsChanged(AVFormatContext* formatContext, AVPacket* packet);
	AVDictionary* SetOpts();
public:
	CFFDecodeAudio();
	~CFFDecodeAudio();
};

#endif //__CFFDECODEAUDIO_H__