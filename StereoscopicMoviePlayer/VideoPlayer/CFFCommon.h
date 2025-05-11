#ifndef __CFFCOMMON_H__
#define __CFFCOMMON_H__

#include <windows.h>
#include "MMSystem.h"

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavdevice/avdevice.h"
	#include "libavformat/avformat.h"
	#include "libavformat/avio.h"
	#include "libswscale/swscale.h"
	#include "libswresample/swresample.h"
	#include "libavutil/imgutils.h"
	#include "libavutil/opt.h"
	#include "libavutil/log.h"
}

class CFFCommon
{
public:
	typedef struct
	{
		AVPixelFormat    PixelFormatsID;
		int              ResizedWidth;
		int              ResizedHeight;
	} ColorTargetParams;
	typedef struct
	{
		AVSampleFormat   SampleFormatsID;
		AVChannelLayout  ChannelLayout;
		int              SampleRate;
	} SampleTargetParams;
};

typedef void(*dOnNewVideoFrame)(void* user, BYTE* frameData, int width, int height, int channels, INT64 pts);
typedef void(*dOnNewAudioFrame)(void* user, BYTE* frameData, int nb_samples, int samplesPerSec, int bitsPerSample, int Channels, INT64 pts);
typedef void(*dOnNewDecodedFrame)(void* user, AVFrame* decodedFrame);

#endif //__CFFCOMMON_H__